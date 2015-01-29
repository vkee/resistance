#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "game_spec.h"
#include "game_tree.h"
#include "r_game.h"


using namespace std;

vector< set<int> > k_subsets_set(int k, int num){
	vector< set<int> > subsets;
	if (k == 1 && num > 0){
		for (int i = 0; i < num; i++){
			set<int> tuple;
			tuple.insert(i);
			subsets.push_back(tuple);
		}
	} else if (num >= k){
		subsets = k_subsets_set(k, num - 1);
		vector< set<int> > temp_subsets = k_subsets_set(k - 1, num - 1);
		for (int i = 0; i < temp_subsets.size(); i++){
			temp_subsets[i].insert(num - 1);
			subsets.push_back(temp_subsets[i]);
		}
	}
	return subsets;
}

RGame::RGame(GameSpec* spec, double vconf, double mconf, double cconf){
	this->spec = spec;
	this->vconf = vconf;
	this->mconf = mconf;
	this->cconf = cconf;
	mission = 0;
	rpoints = 0;
	spoints = 0;

	subsets = k_subsets_set(spec->num_spies, spec->num_players);
	cout << "> ... Loading game trees" << endl;
	int part = subsets.size() / 10;
	for (int i = 0; i < subsets.size(); i++){
		trees[subsets[i]] = new GameTree(subsets[i], spec);
		prob_map[subsets[i]] = 1.0 / subsets.size();
		if (i % part == 0 && i > 0){
			cout << "> ... Loaded " << (100.0 * i /subsets.size()) << "%" << endl;
		}
	}
	cout << "> ... Loaded 100%" << endl;
}

// update overall game statistics and player statistics
void RGame::vote_update(vector<int> team, set<int> votes){
	set<int> spy_set;
	GameTree* curr_tree;
	double factor_total = 0.0, action_prob, factor;
	map<set<int>, double> prob_factors;
	int num_players = spec->num_players;

	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		if (prob_map[spy_set] > 0){
			curr_tree = trees[spy_set];
			action_prob = curr_tree->curr_node->get_child(team)->uniform_win_prob;
			factor = 1.0;
			for (int j = 0; j < spec->num_players; j++){
				if (votes.count(j) > 0){
					if (spy_set.count(j) > 0){
						factor *= 1 - action_prob;
					} else {
						factor *= action_prob;
					}
				} else {
					if (spy_set.count(j) > 0){
						factor *= action_prob;
					} else {
						factor *= 1 - action_prob;
					}
				}
			}
			prob_factors[spy_set] = factor;
			factor_total += factor;		
		}	
	}

	double prob_update, normalization = 0.0;
	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		if (prob_map[spy_set] > 0){
			prob_update = vconf * prob_factors[spy_set] + (1 - vconf) * factor_total / subsets.size();
			prob_map[spy_set] *= prob_update;
			normalization += prob_map[spy_set];
		}
	}

	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		if (prob_map[spy_set] > 0){
			prob_map[spy_set] /= normalization;
		}
	}

	if (votes.size() >= num_players/2.0){
		for (int i = 0; i < subsets.size(); i++){
			spy_set = subsets[i];
			if (prob_map[spy_set] > 0){
				trees[spy_set]->mission_vote(team);
			}
		}
		most_recent_team = team;
	}
}

// update overall game statistics and player statistics
void RGame::mission_update(int num_fails){
	set<int> spy_set;
	GameTree* curr_tree;
	double prob_no_fail, prob_fail;

	double factor_total = 0.0;
	map<set<int>, double> prob_factors;
	// Zero impossible probabilities and update based on actions as above
	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		if (prob_map[spy_set] > 0){
			curr_tree = trees[spy_set];
			if (find_num_spies(spy_set, most_recent_team) < num_fails){
				prob_map[spy_set] = 0;
			} else {
				prob_factors[spy_set] = 1.0;
				if (find_num_spies(spy_set, most_recent_team) > 0){
					prob_no_fail = curr_tree->curr_node->get_child(0)->uniform_win_prob;
					prob_fail = curr_tree->curr_node->get_child(1)->uniform_win_prob;
					if (num_fails > 0 && prob_fail + prob_no_fail > 0){
						prob_factors[spy_set] = prob_no_fail / (prob_fail + prob_no_fail);
					} else if (prob_fail + prob_no_fail > 0){
						prob_factors[spy_set] = prob_fail / (prob_fail + prob_no_fail);
					}
				}

				// may result in errors (double check this part)
				if (num_fails > 0 && find_num_spies(spy_set, most_recent_team) != num_fails){
					prob_factors[spy_set] *= (1 - cconf);
				} else {
					prob_factors[spy_set] *= cconf;
				}

				factor_total += prob_factors[spy_set];
			}
		}
	}

	double prob_update, normalization = 0.0;
	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		if (prob_map[spy_set] > 0){
			prob_update = mconf * prob_factors[spy_set] + (1 - mconf) * factor_total / subsets.size();
			prob_map[spy_set] *= prob_update;
			normalization += prob_map[spy_set];
		}
	}

	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		if (prob_map[spy_set] > 0){
			prob_map[spy_set] /= normalization;
		}
	}

	int result = (num_fails >= spec->wins[mission]);
	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		// This syntax choice is a check to make sure everything works that may initially fail
		if (prob_map[spy_set] > 0){
			trees[spy_set]->mission_outcome(result);
		}
	}

	// Remember to update tree nodes
	if (num_fails >= spec->wins[mission]){
		spoints++;
	} else {
		rpoints++;
	}
	mission++;
}

vector<double> RGame::player_stats(){
	int num_players = spec->num_players;
	vector<double> player_probs;
	for (int j = 0; j < num_players; j++){
		player_probs.push_back(0);
	}

	set<int> spy_set;
	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		for (int j = 0; j < num_players; j++){
			if (spy_set.count(j) > 0){
				player_probs[j] += prob_map[spy_set];
			}
		}
	}
	return player_probs;
}

vector<double> RGame::player_spec_stats(int player){
	int num_players = spec->num_players;
	vector<double> player_probs;
	for (int j = 0; j < num_players; j++){
		player_probs.push_back(0);
	}

	set<int> spy_set;
	double normalization = 0.0;
	for (int i = 0; i < subsets.size(); i++){
		spy_set = subsets[i];
		for (int j = 0; j < num_players; j++){
			if (spy_set.count(j) > 0 && spy_set.count(player) == 0){
				player_probs[j] += prob_map[spy_set];
				normalization += prob_map[spy_set];
			}
		}
	}

	for (int j = 0; j < num_players; j++){
		player_probs[j] *= spec->num_spies / normalization;
	}

	return player_probs;
}

RGame::~RGame(){
	if (!trees.empty()){
		trees.clear();
	}
}