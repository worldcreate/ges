#include "NeighbourGenerator.h"
#include "Graph.h"
#include "Node.h"

NeighbourGenerator::NeighbourGenerator(){
}

NeighbourGenerator::NeighbourGenerator(vector<vector<JobPair> >& a_solution,vector<vector<JobPair> >& a_SettingTable,int L):m_L(L){
	m_solution=a_solution;
	m_SettingTable=a_SettingTable;
}

void NeighbourGenerator::makeNeighbour(){
	vector<Node*> bottleneck;
	Graph g(m_solution,m_SettingTable);
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

	m_CriticalPathList.clear();
	vector<Node*> criticalPath;
	findCriticalPath(g[0],g[g.size()-1],bottleneck,criticalPath);

	cout<<"critical Path"<<endl;
	for(int i=0;i<m_CriticalPathList.size();i++){
		for(int j=0;j<m_CriticalPathList[i].size();j++){
			cout<<"("<<m_CriticalPathList[i][j]->m_Jobpair->machine<<","<<m_CriticalPathList[i][j]->m_Jobpair->jobIndex<<") ";
		}
		cout<<endl;
	}
}

void NeighbourGenerator::findCriticalPath(Node* node,Node* leaf,vector<Node*>& bottleneck,vector<Node*>& criticalPath){
	if(node==leaf){
		criticalPath.push_back(leaf);
		m_CriticalPathList.push_back(criticalPath);
		return;
	}
	for(int i=0;i<node->m_Next.size();i++){
		vector<Node*> _criticalPath=criticalPath;
		for(int j=0;j<bottleneck.size();j++){
			cout<<"m_Next["<<i<<"]="<<node->m_Next[i]->m_Jobpair->index<<":bottleneck["<<j<<"]="<<bottleneck[j]->m_Jobpair->index<<endl;
			if(node->m_Next[i]->m_Jobpair->index==bottleneck[j]->m_Jobpair->index){
				_criticalPath.push_back(node);
				findCriticalPath(node->m_Next[i],leaf,bottleneck,_criticalPath);
				break;
			}
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

