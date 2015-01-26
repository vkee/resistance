#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>

using namespace std;

void print_subset(vector<int> subset){
	cout << "< ";
	for (int i : subset){
		cout << i << " ";
	}
	cout << endl;
}

vector<vector<int>> k_subsets(int k, int max){
	// Implement
}

void k_subsets_suffix(vector<vector<int>> subsets, vector<int> suffix, int k, int max){
	// Implement recursively
}

class GameSpec {
	public:
		int num_players;
		int missions[5], wins[5];

		GameSpec(int num_players){
			num_players = num_players;
			
			if (num_players == 5){
				missions = {2, 3, 2, 3, 3};
				wins = {1, 1, 1, 1, 1};
			} else if (num_players == 6){
				missions = {2, 3, 3, 3, 4};
				wins = {1, 1, 1, 1, 1};
			} else if (num_players == 7){
				missions = {2, 3, 3, 4, 4};
				wins = {1, 1, 1, 2, 1};
			} else if (num_players >= 8 && num_players <= 10){
				missions = {3, 4, 4, 5, 5};
				wins = {1, 1, 1, 2, 1};
			} else {
				cout << "Number of players must be an integer" << endl;
			}

		}
};

class VotingNode {
	private:
		GameSpec* spec;
		int spoints, rpoints, mission;
		MissionNode* parent;
		map<vector<int>, MissionNode*> children;
	public:
		double uniform_win_prob;

		VotingNode(MissionNode* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission){
			parent = parent;
			spec = spec;

			spoints = spoints;
			rpoints = rpoints;
			mission = mission;
		}

		double makeChildren(){
			if (spoints == 3){
				children = NULL;
				uniform_win_prob = 0;
			} else if (rpoints == 3){
				children == NULL;
				uniform_win_prob = 1;
			} else {
				int num_children = 0;
				uniform_win_prob = 0;
			}
		}
};


class MissionNode {
	private:
		GameSpec* spec;
		int spoints, rpoints, mission;
		VotingxNode* parent;
		vector<VotingNode*> children;
	public:
		double uniform_win_prob;

		MissionNode(VotingNode* parent, GameSpec* spec, set<int> spies, int spoints, int rpoints, int mission){
			parent = parent;
			spec = spec;

			spoints = spoints;
			rpoints = rpoints;
			mission = mission;
		}

		double makeChildren(int num_spies){
			if (spoints == 3){
				children = NULL;
				uniform_win_prob = 0;
			} else if (rpoints == 3){
				children == NULL;
				uniform_win_prob = 1;
			} else {
				int num_children = 0;
				
				VotingNode* rchild = new VotingNode(&this, spec, spies, spoints, rpoints + 1, mission + 1);
				children.push_back(rchild);
				uniform_win_prob = rchild.makeChildren();
				num_children++;

				if (num_spies >= spec->wins[mission]){
					VotingNode* schild = new VotingNode(&this, spec, spies, spoints + 1, rpoints, mission + 1);
					children.push_back(schild);
					uniform_win_prob += schild.makeChildren();
					num_children++;
				}

				uniform_win_prob /= num_children;
				return uniform_win_prob;
			}
		}
};


int main(){
	cout << "Main Function" << endl;
}


