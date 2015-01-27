#include<iostream>
#include "game_spec.h"
using namespace std;

int main(){
	GameSpec *game_spec = new GameSpec(10);

	cout << "Num Resistance: " << game_spec->num_resistance << endl;
	cout << "Num Spies: " << game_spec->num_spies << endl;

	delete game_spec;

	return 0;
}