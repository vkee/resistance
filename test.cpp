#include<iostream>
#include "game_tree.h"
using namespace std;

int main(){
	GameSpec* spec = new GameSpec(5);
	set<int> spies;
	spies.insert(0);
	spies.insert(1);
	GameTree* tree = new GameTree(spies, spec);
	vector<int> team;
	team.push_back(0);
	team.push_back(2);
	tree->mission_vote(team);
	tree->mission_outcome(0);

	team.clear();
	team.push_back(0);
	team.push_back(1);
	team.push_back(4);
	tree->mission_vote(team);
	tree->mission_outcome(1);

	team.clear();
	team.push_back(1);
	team.push_back(3);
	tree->mission_vote(team);
	tree->mission_outcome(1);

	team.clear();
	team.push_back(1);
	team.push_back(3);
	team.push_back(4);
	tree->mission_vote(team);
	tree->mission_outcome(1);
}