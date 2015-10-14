#include "NeighbourGenerator.h"
#include "Graph.h"

NeighbourGenerator::NeighbourGenerator(){
}

NeighbourGenerator::NeighbourGenerator(vector<vector<JobPair> >& a_solution,int L):m_L(L){
	m_solution=a_solution;
}

void NeighbourGenerator::makeNeighbour(vector<vector<JobPair> >& a_settingTable){
	vector<Node*> bottleneck;
	Graph g(m_solution,a_settingTable);
	for(int i=0;i<g.size();i++){
		if(g[i]->m_R+g[i]->m_Q-g[i]->m_Jobpair->time>m_L){
			bottleneck.push_back(g[i]);
		}
	}

	cout<<"bottleneck Node"<<endl;
	for(int i=0;i<bottleneck.size();i++){
		cout<<"("<<bottleneck[i]->m_Jobpair->machine<<","<<bottleneck[i]->m_Jobpair->jobIndex<<","<<bottleneck[i]->m_Jobpair->time<<") ";
	}
	cout<<endl;

	for(int i=0;i<bottleneck.size();i++){
		for(int j=0;j<bottleneck.size();j++){
			if(i==j)
				continue;
			if(bottleneck[i]->m_Jobpair->machine!=bottleneck[j]->m_Jobpair->machine)
				continue;

		}
	}
}

// vector<Node*> NeighbourGenerator::changeAfter(const vector<Node*>& bottleneck,int i,int j){
// 	return null;
// }

// vector<Node*> NeighbourGenerator::changeBefore(const vector<Node*>& bottleneck,int i,int j){
// 	return null;
// }

// vector<vector<JobPair> > NeighbourGenerator::getSolution(int index){
// 	return null;
// }

