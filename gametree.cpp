#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "game_spec.h"

using namespace std;

int find_num_spies(set<int> spies, vector<int> team){
	int overlap = 0;
	for (int i = 0; i < team.size(); i++){
		overlap += spies.count(team[i]);
	}
	return overlap;
}

class Node {
	protected:
		GameSpec* spec;
		int spoints, rpoints, mission;
		Node* parent;
		set<int> spies;
	public:
		double uniform_win_prob;

		Node(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission){
			this->parent = parent;
			this->spec = spec;

			this->spoints = spoints;
			this->rpoints = rpoints;
			this->mission = mission;

			this->spies = spies;
		};

		double make_children(){
			return 0;
		}

		Node* get_child(int outcome){
			return new Node(this, spec, spies, spoints, rpoints, mission);
		}

		Node* get_child(vector<int> voted_team){
			return new Node(this, spec, spies, spoints, rpoints, mission);
		}
};

class VotingNode: public Node {
	private:
		map<vector<int>, Node*> children;
	public:
		VotingNode(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission)
			:Node(parent, spec, spies, spoints, rpoints, mission){}

		double make_children();

		Node* get_child(vector<int> voted_team){
			return children[voted_team];
		}

		~VotingNode(){
			if (!children.empty()){
				children.clear();
			}
		}
};

class MissionNode: public Node {
	private:
		vector<Node*> children;
	public:
		MissionNode(Node* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission)
			:Node(parent, spec, spies, spoints, rpoints, mission){}

		double make_children(int);

		Node* get_child(int outcome){
			return children[outcome];
		}

		~MissionNode(){
			for (int i = 0; i < children.size(); i++){
				delete children[i];
			}
		}
};

double VotingNode::make_children(){
	if (spoints == 3){
		uniform_win_prob = 0;
	} else if (rpoints == 3){
		uniform_win_prob = 1;
	} else {
		uniform_win_prob = 0;
		MissionNode* child;
		for (int i = 0; i < spec->teams[mission].size(); i++){
			child = new MissionNode(this, spec, spies, spoints, rpoints, mission);
			children[spec->teams[mission][i]] = child;
			uniform_win_prob += child->make_children(find_num_spies(spies, spec->teams[mission][i]));
		}
		uniform_win_prob /= spec->teams[mission].size();
	}
	return uniform_win_prob;
}

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

class GameTree {
	private:
		GameSpec* spec;
		set<int> spies;
		Node* curr_node;
	public:
		GameTree(set<int> spies, GameSpec* spec){
			this->spies = spies;
			this->spec = spec;

			curr_node = new VotingNode(NULL, spec, spies, 0, 0, 0);
			curr_node->make_children();
		}

		void mission_vote(vector<int> voted_team){
			curr_node = curr_node->get_child(voted_team);
			cout << "Uniform probability of resistance win: " << curr_node->uniform_win_prob << endl;
		}

		void mission_outcome(int outcome){
			curr_node = curr_node->get_child(outcome);
			cout << "Uniform probability of resistance win: " << curr_node->uniform_win_prob << endl;
		}
};

int main(){
	GameSpec* spec = new GameSpec(5);
	set<int> spies;
	spies.insert(0);
	spies.insert(1);
	GameTree* tree = new GameTree(spies, spec);
	vector<int> team;
	team.push_back(0);
	team.push_back(3);
	tree->mission_vote(team);
	tree->mission_outcome(1);
}


