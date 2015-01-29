#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "game_spec.h"
#include "game_tree.h"
#include "r_game.h"

using namespace std;

void print_statistics(RGame* game, vector<string> player_names){
	cout << "****************STATISTICS****************" << endl;
	vector<double> stats = game->player_stats();
	for (int i = 0; i < player_names.size(); i++){
		cout << player_names[i] << ": " << stats[i] << endl;
	}
	cout << "******************************************" << endl;
}

void print_player_statistics(RGame* game, vector<string> player_names, string player_name, int player_num){
	cout << "************STATISTICS FOR " << player_name << "************" << endl;
	vector<double> stats = game->player_spec_stats(player_num);
	for (int i = 0; i < player_names.size(); i++){
		cout << player_names[i] << ": " << stats[i] << endl;
	}
}

int main(){
	double voting_confidence = 0.1;
	double mission_confidence = 0.8;
	double coord_confidence = 0.8;

	int num_players = 0;
	cout << "Welcome to the Resistance Game Calculator developed by Matthew Brennan and Vincent Kee." << endl;
	
	while (num_players < 5 || num_players > 10){
		cout << "Resistance can be played by 5 - 10 people." << endl;
		cout << "Please enter the number of players playing: ";
		cin >> num_players;
	}

	GameSpec* game_spec = new GameSpec(num_players);
	RGame* game = new RGame(game_spec, voting_confidence, mission_confidence, coord_confidence);

	map<string, int> name_map;
	vector<string> player_names;
	string curr_player_name;

	while (player_names.size() != num_players){
		player_names.clear();
		cout << "Please enter the names (space-separated) of the " << num_players << " players: ";
		for (int i = 0; i < num_players; i++){
			cin >> curr_player_name;
			name_map[curr_player_name] = i;

			// Making sure that the player name is not already input
			if (find(player_names.begin(), player_names.end(), curr_player_name) == player_names.end()){
				player_names.push_back(curr_player_name);
			}
		}
	}

	int mission = 0, num_votes = -1, num_fails = -1, num_spec_stats = -1;
	vector<int> team;
	set<int> votes;
	while (game->rpoints < 3 && game->spoints < 3){
		while (num_votes < num_players / 2.0){

			while (team.size() != game_spec->missions[mission]){
				team.clear();
				cout << "Please enter the names (space-separated) of the players on the proposed team (" 
					<< game_spec->missions[mission] << " players on round #" << mission + 1 << "): ";
				for (int i = 0; i < game_spec->missions[mission]; i++){
					cin >> curr_player_name;

					// Making sure that the player name is input correctly 
					if (find(player_names.begin(), player_names.end(), curr_player_name) != player_names.end()){
						// Making sure that the player name is not already input
						if (find(team.begin(), team.end(), name_map[curr_player_name]) == team.end()){
							team.push_back(name_map[curr_player_name]);
						}
					}
				}
			}

			sort(team.begin(), team.end());
			
			// Making sure that the number of votes is correct
			while (num_votes < 0 || num_votes > game_spec->num_players){
				cout << "Please enter the number of votes approving the proposed team (" << game_spec->num_players << " max): ";
				cin >> num_votes;
			}

			// Making sure that names of the votors are correct
			while (votes.size() < num_votes){
				cout << "Please enter the " << num_votes << " names (space-separated) of the players who voted for the proposed team: ";
				for (int i = 0; i < num_votes; i++){
					cin >> curr_player_name;

					// Making sure that the player name is input correctly
					if (find(player_names.begin(), player_names.end(), curr_player_name) != player_names.end()){
						// Making sure that the player name is not already input
						if (find(votes.begin(), votes.end(), name_map[curr_player_name]) == votes.end()){
							votes.insert(name_map[curr_player_name]);
						}
					}
				}
			}

			game->vote_update(team, votes);

			print_statistics(game, player_names);

			team.clear();
			votes.clear();
		}
		
		// Making sure that the number of fails is correct
		while (num_fails < 0 || num_fails > game_spec->num_spies){
			cout << "Please enter the number of cards failing the mission (at least " << game_spec->wins[mission] <<
			 " out of " << game_spec->missions[mission] << " cards must be fails for the mission to fail): ";
			cin >> num_fails;
		}

		game->mission_update(num_fails);

		if (!(game->rpoints > 2 || game->spoints > 2)){
			print_statistics(game, player_names);

			// Making sure that the number of players is correct
			while (num_spec_stats < 0 || num_spec_stats > num_players){
				cout << "Please enter the number of players who would like to know statistics from their perspectives: ";
				cin >> num_spec_stats;
			}

			if (num_spec_stats != 0){
				cout << "Please enter the names of the players who would like to know statistics from their perspectives (" << 
				num_spec_stats << " max): ";
				for (int i = 0; i < num_spec_stats; i++){
					cin >> curr_player_name;

					// Making sure that the player name is input correctly
					if (find(player_names.begin(), player_names.end(), curr_player_name) != player_names.end()){
						print_player_statistics(game, player_names, curr_player_name, name_map[curr_player_name]);
					} else {
						i += -1;
					}
				}
			}

			mission++;
			num_votes = -1;
			num_fails = -1;
			num_spec_stats = -1;
		}
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