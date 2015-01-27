#include <iostream>

using namespace std;

// Based on the game rules posted on http://en.wikipedia.org/wiki/The_Resistance_%28game%29
class GameSpec {
	public:
		int num_players, num_resistance, num_spies;
		int missions[5], wins[5];

		GameSpec(int num_players){
			this->num_players = num_players;
			
			if (num_players == 5){
				missions[0] = 2;
				missions[1] = 3;
				missions[2] = 2;
				missions[3] = 3;
				missions[4] = 3;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 1;
				wins[4] = 1;

				num_resistance = 3;
				num_spies = 2;
			} else if (num_players == 6){
				missions[0] = 2;
				missions[1] = 3;
				missions[2] = 3;
				missions[3] = 3;
				missions[4] = 4;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 1;
				wins[4] = 1;

				num_resistance = 4;
				num_spies = 2;
			} else if (num_players == 7){
				missions[0] = 2;
				missions[1] = 3;
				missions[2] = 3;
				missions[3] = 4;
				missions[4] = 4;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;

				num_resistance = 4;
				num_spies = 3;
			} else if (num_players == 8){
				missions[0] = 3;
				missions[1] = 4;
				missions[2] = 4;
				missions[3] = 5;
				missions[4] = 5;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;

				num_resistance = 5;
				num_spies = 3;
			} else if (num_players == 9){
				missions[0] = 3;
				missions[1] = 4;
				missions[2] = 4;
				missions[3] = 5;
				missions[4] = 5;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;

				num_resistance = 6;
				num_spies = 3;
			} else if (num_players == 10){
				missions[0] = 3;
				missions[1] = 4;
				missions[2] = 4;
				missions[3] = 5;
				missions[4] = 5;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;
				
				num_resistance = 6;
				num_spies = 4;
			} else {
				cout << "Resistance can only be played by 5 - 10 people." << endl;
			}
		}
};

int main(){
	GameSpec *game_spec = new GameSpec(11);

	cout << "Num Resistance: " << game_spec->num_resistance << endl;
	cout << "Num Spies: " << game_spec->num_spies << endl;

	delete game_spec;

	return 0;
}