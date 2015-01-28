#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "game_spec.h"
#include "game_tree.h"

using namespace std;

// Given a set of actual spies and the team that is chosen, returns how many spies are in the team
int find_num_spies(set<int> spies, vector<int> team){
	int overlap = 0;
	for (int i = 0; i < team.size(); i++){
		overlap += spies.count(team[i]);
	}
	return overlap;
}

// Node Methods
// Parent Class for Voting Node and Mission Node
Node::Node(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission){
	this->parent = parent;
	this->spec = spec;

	this->spoints = spoints;
	this->rpoints = rpoints;
	this->mission = mission;

	this->spies = spies;
}

// These 3 methods are overloaded - IGNORE
double Node::make_children(){
	return -100;
};

Node* Node::get_child(int outcome){
	return NULL;
}

Node* Node::get_child(vector<int> voted_team){
	return NULL;
}

// Voting Node Methods
double VotingNode::make_children(){
	if (spoints == 3){ // if spies have 3 points, there is no chance for the resistance to win
		uniform_win_prob = 0;
	} else if (rpoints == 3){ // if the resistance have 3 points, they are guaranteed to win
		uniform_win_prob = 1;
	} else {
		uniform_win_prob = 0;
		MissionNode* child;
		int num_children = spec->teams[mission].size();
		for (int i = 0; i < num_children; i++){
			child = new MissionNode(this, spec, spies, spoints, rpoints, mission);
			children[spec->teams[mission][i]] = child;
			uniform_win_prob += child->make_children(find_num_spies(spies, spec->teams[mission][i]));
		}
		uniform_win_prob /= num_children;
	}
	return uniform_win_prob;
}

// Get the node that corresponds to the team that was voted to go in the current round
Node* VotingNode::get_child(vector<int> voted_team){
	return children[voted_team];
}

VotingNode::~VotingNode(){
	if (!children.empty()){
		children.clear();
	}
}

// Mission Node Methods
// Node representing whether or not the mission succeeded
double MissionNode::make_children(int num_spies){
	VotingNode* rchild = new VotingNode(this, spec, spies, spoints, rpoints + 1, mission + 1);
	children.push_back(rchild);
	uniform_win_prob = rchild->make_children();

	if (num_spies >= spec->wins[mission]){
		VotingNode* schild = new VotingNode(this, spec, spies, spoints + 1, rpoints, mission + 1);
		children.push_back(schild);
		uniform_win_prob += schild->make_children();
		uniform_win_prob /= 2;
	}
	return uniform_win_prob;
}

// Gets the child given the outcome, where 0 is success and 1 is failed mission
Node* MissionNode::get_child(int outcome){
	return children[outcome];
}

MissionNode::~MissionNode(){
	for (int i = 0; i < children.size(); i++){
		delete children[i];
	}
}

// Game Tree Methods
GameTree::GameTree(set<int> spies, GameSpec* spec){
	this->spies = spies;
	this->spec = spec;

	curr_node = new VotingNode(NULL, spec, spies, 0, 0, 0);
	double result = curr_node->make_children();
	//cout << result << endl;
}
		
void GameTree::mission_vote(vector<int> voted_team){
	curr_node = curr_node->get_child(voted_team);
	//cout << "Uniform probability of resistance win: " << curr_node->uniform_win_prob << endl;
}

void GameTree::mission_outcome(int outcome){
	curr_node = curr_node->get_child(outcome);
	//cout << "Uniform probability of resistance win: " << curr_node->uniform_win_prob << endl;
}