#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>

using namespace std;

void print_subset(vector<int> subset){
	cout << "< ";
	for (int i = 0; i < subset.size(); i++){
		cout << subset[i] << " ";
	}
	cout << ">" << endl;
}

vector< vector<int> > k_subsets(int k, int num){
	vector< vector<int> > subsets;
	if (k == 1 && num > 0){
		for (int i = 0; i < num; i++){
			vector<int> tuple;
			tuple.push_back(i);
			subsets.push_back(tuple);
		}
	} else if (num >= k){
		subsets = k_subsets(k, num - 1);
		vector< vector<int> > temp_subsets = k_subsets(k - 1, num - 1);
		for (int i = 0; i < temp_subsets.size(); i++){
			temp_subsets[i].push_back(num - 1);
			subsets.push_back(temp_subsets[i]);
		}
	}
	return subsets;
}

class GameSpec {
	public:
		int num_players, num_resistance, num_spies;
		int missions[5], wins[5];
		vector< vector< vector<int> > > teams;

		GameSpec(int num_players){
			this->num_players = num_players;
			
			if (num_players == 5){
				missions[0] = 2;
				missions[1] = 3;
				missions[2] = 2;
				missions[3] = 3;
				missions[4] = 3;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 1;
				wins[4] = 1;

				num_resistance = 3;
				num_spies = 2;
			} else if (num_players == 6){
				missions[0] = 2;
				missions[1] = 3;
				missions[2] = 3;
				missions[3] = 3;
				missions[4] = 4;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 1;
				wins[4] = 1;

				num_resistance = 4;
				num_spies = 2;
			} else if (num_players == 7){
				missions[0] = 2;
				missions[1] = 3;
				missions[2] = 3;
				missions[3] = 4;
				missions[4] = 4;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;

				num_resistance = 4;
				num_spies = 3;
			} else if (num_players == 8){
				missions[0] = 3;
				missions[1] = 4;
				missions[2] = 4;
				missions[3] = 5;
				missions[4] = 5;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;

				num_resistance = 5;
				num_spies = 3;
			} else if (num_players == 9){
				missions[0] = 3;
				missions[1] = 4;
				missions[2] = 4;
				missions[3] = 5;
				missions[4] = 5;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;

				num_resistance = 6;
				num_spies = 3;
			} else if (num_players == 10){
				missions[0] = 3;
				missions[1] = 4;
				missions[2] = 4;
				missions[3] = 5;
				missions[4] = 5;

				wins[0] = 1;
				wins[1] = 1;
				wins[2] = 1;
				wins[3] = 2;
				wins[4] = 1;
				
				num_resistance = 6;
				num_spies = 4;
			} else {
				cout << "Resistance can only be played by 5 - 10 people." << endl;
			}

			for (int i = 0; i < 5; i++){
				teams.push_back(k_subsets(missions[i], num_players));
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
	int k, N;
	cin >> k >> N;
	vector< vector<int> > subsets = k_subsets(k, N);
	for (int i = 0; i < subsets.size(); i++){
		print_subset(subsets[i]);
	}
}


