#ifndef GAME_TREE_H
#define GAME_TREE_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "game_spec.h"

using namespace std;

int find_num_spies(set<int> spies, vector<int> team);

// Parent Class for Voting Node and Mission Node
class Node {
	protected:
		GameSpec* spec;
		int spoints, rpoints, mission;
		Node* parent;
		set<int> spies;
	public:
		double uniform_win_prob;

		// spoints - # of pts the spies have
		// rpoints - # of pts the resistance have
		// mission - the mission/round number 
		Node(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission);

		// These 3 methods are overloaded - IGNORE
		virtual double make_children();

		virtual Node* get_child(int outcome);

		virtual Node* get_child(vector<int> voted_team);
};

// Node representing a Vote Decision
class VotingNode: public Node {
	private:
		map<vector<int>, Node*> children;
	public:
		VotingNode(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission)
			:Node(parent, spec, spies, spoints, rpoints, mission){};

		double make_children();

		// Get the node that corresponds to the team that was voted to go in the current round
		Node* get_child(vector<int> voted_team);

		~VotingNode();
};

class MissionNode: public Node {
	private:
		vector<Node*> children;
	public:
		MissionNode(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission)
			:Node(parent, spec, spies, spoints, rpoints, mission){};

		double make_children(int);

		Node* get_child(int outcome);

		~MissionNode();
};

class GameTree {
	private:
		GameSpec* spec;
		set<int> spies;
		Node* curr_node;
	public:
		GameTree(set<int> spies, GameSpec* spec);
		
		void mission_vote(vector<int> voted_team);

		void mission_outcome(int outcome);
};

#endif