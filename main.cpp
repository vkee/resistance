#include <iostream>
#include <string>
#include <vector>
#include <map>

int main(){
	int num_players;
	string curr_player_name;
	Player* curr_player;
	vector<Player*> players;
	map<string, Player*> name_player_pairs;

	cout << "Please enter the number of players playing:" << endl;
	cin >> num_players;

	cout << "Please enter the names (space-separated) of the players:" << endl;
	for (int i = 0; i < num_players; i++){
		cin >> curr_player_name;
		curr_player = new Player(curr_player_name);
		players.push_back(curr_player);
		name_player_pairs[curr_player_name] = curr_player;
	}

	RGame* game = new RGame(players);
	game->send_game();

	int team_size;
	vector<Player*> team;
	while(!game->done){
		cout << "Please enter the names (space-separated) of the players on the team:" << endl;
		team_size = game->team_size;
		for (int i = 0; i < team_size; i++){
			cin >> curr_player_name;
			team.push_back(name_player_pairs[curr_player_name]);
		}
		game->update;
		game->print_statistics;
	}

}

class RGame {
	private:
		vector<Player> players;
		bool is_over;
	public:
		RGame(vector<Player*> players){
			players = players;
			is_over = false;
			// INITIALIZE GAME PARAMETERS
		}

		void send_game(){
			for (Player player : players){
				player.define_game(&this);
			}
		}

		void update(vector<Player*> team){
			// WRITE UPDATE CODE
		}

		void print_statistics(){
			// WRITE DISPLAY CODE
		}

		int team_size(){
			// WRITE TEAM SIZE CODE
		}

		bool done(){
			return is_over;
		}
};

class Player{
	private:
		string name;
		RGame* game;
	public:
		Player(string name){
			name = name;
		}

		void define_game(RGame* game){
			game = game;
		}
};