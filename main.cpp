#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "game_spec.h"
#include "game_tree.h"

using namespace std;

int main(){
	double voting_confidence = 0.1;

	int num_players = 0;
	cout << "Welcome to the Resistance Game Calculator developed by Matthew Brennan and Vincent Kee." << endl;
	
	while (num_players < 5 || num_players > 10){
		cout << "Resistance can be played by 5 - 10 people." << endl;
		cout << "Please enter the number of players playing: ";
		cin >> num_players;
	}

	GameSpec* game_spec = new GameSpec(num_players);

	map<string, int> name_map;
	string curr_player_name;
	cout << "Please enter the names (space-separated) of the players:" << endl;
	for (int i = 0; i < num_players; i++){
		cin >> curr_player_name;
		name_map[curr_player_name] = i;
	}



	delete game_spec;

	return 0;
}

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

class RGame {
	private:
		GameSpec* spec;
		map<set<int>, GameTree*> trees;
		vector<PlayerGame*> players;
		vector< set<int> > subsets;
		map<set<int>, double> prob_map;
	public:
		RGame(GameSpec* spec){
			this->spec = spec;
			subsets = k_subsets_set(spec->num_spies, spec->num_players);
			for (int i = 0; i < subsets.size(); i++){
				trees[subsets[i]] = new GameTree(subsets[i], spec);
				prob_map[subsets[i]] = 1.0 / subsets.size();
			}

			Player* curr_player;
			for (int i = 0; i < spec->num_players; i++){
				curr_player = new PlayerGame(spec, trees);
				players.push_back(curr_player);
			}
		}

		// update overall game statistics and player statistics
		void vote_update(vector<int> team, vector<int> votes){

		}

		// update overall game statistics and player statistics
		void mission_update(int outcome){

		}
}