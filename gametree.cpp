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

class GameTree {
	private:
		GameSpec* spec;
		set<int> spies;
		VotingNode* curr_node;
	public:
		GameTree(set<int> spies, GameSpec* spec){
			this->spies = spies;
			this->spec = spec;

			curr_node = new VotingNode(NULL, spec, spies, 0, 0, 0);
			curr_node.make_children();
		}

		void mission_vote(vector<int> voted_team){
			curr_node = curr_node.get_child(voted_team);
			cout << "Uniform probability of resistance win: " << curr_node.uniform_win_prob << endl;
		}

		void mission_outcome(int outcome){
			curr_node = curr_node.get_child(outcome);
			cout << "Uniform probability of resistance win: " << curr_node.uniform_win_prob << endl;
		}
};

class VotingNode {
	private:
		GameSpec* spec;
		int spoints, rpoints, mission;
		MissionNode* parent;
		map<vector<int>, MissionNode*> children;
		set<int> spies;
	public:
		double uniform_win_prob;

		VotingNode(MissionNode* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission){
			this->parent = parent;
			this->spec = spec;

			this->spoints = spoints;
			this->rpoints = rpoints;
			this->mission = mission;

			this->spies = spies;
		}

		double make_children(){
			if (spoints == 3){
				children = NULL;
				uniform_win_prob = 0;
			} else if (rpoints == 3){
				children == NULL;
				uniform_win_prob = 1;
			} else {
				uniform_win_prob = 0;
				MissionNode* child;
				for (int i = 0; i < spec->teams[mission].size(); i++){
					child = new MissionNode(this, spec, spies, spoints, rpoints, mission);
					children[spec->teams[mission][i]] = child;
					uniform_win_prob += child.make_children(find_num_spies(spies, spec->teams[mission][i]));
				}
				uniform_win_prob /= spec->teams[mission].size();
			}
			return uniform_win_prob;
		}

		MissionNode* get_child(vector<int> voted_team){
			return children[voted_team];
		}
};


class MissionNode {
	private:
		GameSpec* spec;
		int spoints, rpoints, mission;
		VotingNode* parent;
		vector<VotingNode*> children;
	public:
		double uniform_win_prob;

		MissionNode(VotingNode* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission){
			this->parent = parent;
			this->spec = spec;

			this->spoints = spoints;
			this->rpoints = rpoints;
			this->mission = mission;

			this->spies;
		}

		double make_children(int num_spies){
			VotingNode* rchild = new VotingNode(this, spec, spies, spoints, rpoints + 1, mission + 1);
			children.push_back(rchild);
			uniform_win_prob = rchild.make_children();

			if (num_spies >= spec->wins[mission]){
				VotingNode* schild = new VotingNode(this, spec, spies, spoints + 1, rpoints, mission + 1);
				children.push_back(schild);
				uniform_win_prob += schild.make_children();
				uniform_win_prob /= 2;
			}
			return uniform_win_prob;
		}

		VotingNode* get_child(int outcome){
			return children[outcome];
		}
};

int main(){
	GameSpec* spec = new GameSpec(5);
	set<int> spies;
	spies.insert(0);
	spies.insert(1);
	GameTree* tree = new GameTree(spies, spec);
}


