#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "game_spec.h"
#include "game_tree.h"

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

class SimpleRGame {
	private:
		GameSpec* spec;
		double cconf, pconf;
		vector< set<int> > subsets;
		map<set<int>, double> prob_map;
	public:
		int mission, rpoints, spoints;

		SimpleRGame(GameSpec* spec, double cconf, double pconf){
			this->spec = spec;
			this->cconf = cconf;
			this->pconf = pconf;
			mission = 0;
			rpoints = 0;
			spoints = 0;
			subsets = k_subsets_set(spec->num_spies, spec->num_players);
			set<int> spy_set;
			for (int i = 0; i < subsets.size(); i++){
				spy_set = subsets[i];
				prob_map[spy_set] = 1.0 / subsets.size();
			}
		}

		void update(vector<int> team, int num_fails){
			set<int> spy_set;
			double prob_no_fail, prob_fail;
			double normalization = 0.0;

			for (int i = 0; i < subsets.size(); i++){
				spy_set = subsets[i];
				if (find_num_spies(spy_set, team) < num_fails){
					prob_map[spy_set] = 0;
				} else if (num_fails > 0 && find_num_spies(spy_set, team) != num_fails){
					prob_map[spy_set] *= (1 - cconf);
				} else {
					prob_map[spy_set] *= cconf;
				}
				if (num_fails == 0 && find_num_spies(spy_set, team) > 0){
					prob_map[spy_set] *= (1 - pconf);
				} else {
					prob_map[spy_set] *= pconf;
				}

				normalization += prob_map[spy_set];
			}

			for (int i = 0; i < subsets.size(); i++){
				spy_set = subsets[i];
				prob_map[spy_set] /= normalization;
			}

			if (num_fails >= spec->wins[mission]){
				spoints++;
			} else {
				rpoints++;
			}
			mission++;
		}

		vector<double> player_stats(){
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

		vector<double> player_spec_stats(int player){
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

		~SimpleRGame(){
			if (!prob_map.empty()){
				prob_map.clear();
			}
		}
};

void print_statistics(SimpleRGame* game, vector<string> player_names){
	cout << "****************STATISTICS****************" << endl;
	vector<double> stats = game->player_stats();
	for (int i = 0; i < player_names.size(); i++){
		cout << player_names[i] << ": " << stats[i] << endl;
	}
	cout << "******************************************" << endl;
}

void print_player_statistics(SimpleRGame* game, vector<string> player_names, string player_name, int player_num){
	cout << "************STATISTICS FOR " << player_name << "************" << endl;
	vector<double> stats = game->player_spec_stats(player_num);
	for (int i = 0; i < player_names.size(); i++){
		cout << player_names[i] << ": " << stats[i] << endl;
	}
}

int main(){
	double coord_confidence = 0.8;
	double points_confidence = 0.6;

	int num_players = 0;
	cout << "Welcome to the Simple Resistance Game Calculator developed by Matthew Brennan and Vincent Kee." << endl;
	
	while (num_players < 5 || num_players > 10){
		cout << "Resistance can be played by 5 - 10 people." << endl;
		cout << "Please enter the number of players playing: ";
		cin >> num_players;
	}

	GameSpec* game_spec = new GameSpec(num_players);
	SimpleRGame* game = new SimpleRGame(game_spec, coord_confidence, points_confidence);

	map<string, int> name_map;
	vector<string> player_names;
	string curr_player_name;
	cout << "Please enter the names (space-separated) of the players: ";
	for (int i = 0; i < num_players; i++){
		cin >> curr_player_name;
		name_map[curr_player_name] = i;
		player_names.push_back(curr_player_name);
	}

	int mission = 0, num_fails, num_spec_stats;
	vector<int> team;
	while (game->rpoints < 3 && game->spoints < 3){
		cout << "Please enter the names (space-separated) of the players on the team (" << game_spec->missions[mission] << " players this round): ";
		for (int i = 0; i < game_spec->missions[mission]; i++){
			cin >> curr_player_name;
			team.push_back(name_map[curr_player_name]);
		}
		sort(team.begin(), team.end());

		cout << "Please enter the number of cards failing the mission (at least " << game_spec->wins[mission] << " out of " << game_spec->missions[mission] << " cards must be fails for the mission to fail): ";
		cin >> num_fails;

		game->update(team, num_fails);
		print_statistics(game, player_names);

		cout << "Please enter the number of players who would like to know statistics from their perspectives: ";
		cin >> num_spec_stats;
		if (num_spec_stats != 0){
			cout << "Please enter the names of the players who would like to know statistics from their perspectives: ";
			for (int i = 0; i < num_spec_stats; i++){
				cin >> curr_player_name;
				print_player_statistics(game, player_names, curr_player_name, name_map[curr_player_name]);
			}
		}

		team.clear();
		mission++;
	}

	// Case where spies win
	if (game->rpoints < 3){
		cout << "Spies win!" << endl;
	} else { // Case where resistance wins
		cout << "Resistance wins!" << endl;
	}

	delete game_spec;
	delete game;

	return 0;
}