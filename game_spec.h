#ifndef GAME_SPEC_H
#define GAME_SPEC_H

#include <vector>
#include <set>
using namespace std;

void print_subset(vector<int> subset);

vector< vector<int> > k_subsets(int k, int num);

class GameSpec {
	public:
		int num_players, num_resistance, num_spies;
		int missions[5], wins[5];
		vector< vector< vector<int> > > teams;
		GameSpec(int);
};

#endif