#ifndef GAME_SPEC_H
#define GAME_SPEC_H

class GameSpec {
	public:
		int num_players, num_resistance, num_spies;
		int missions[5], wins[5];
		GameSpec(int num_players);
};

#endif