#ifndef R_GAME
#define R_GAME

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector< set<int> > k_subsets_set(int k, int num);

class RGame {
	private:
		GameSpec* spec;
		double vconf, mconf, cconf;
		map<set<int>, GameTree*> trees;
		vector< set<int> > subsets;
		map<set<int>, double> prob_map;

		vector<int> most_recent_team;
	public:
		int mission, rpoints, spoints;

		RGame(GameSpec* spec, double vconf, double mconf, double cconf);

		// update overall game statistics and player statistics
		void vote_update(vector<int> team, set<int> votes);

		// update overall game statistics and player statistics
		void mission_update(int num_fails);

		vector<double> player_stats();

		vector<double> player_spec_stats(int player);

		~RGame();
};

#endif