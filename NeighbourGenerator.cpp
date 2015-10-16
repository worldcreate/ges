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
	cout<<"setting table"<<endl;
	for(int i=0;i<m_SettingTable.size();i++){
		for(int j=0;j<m_SettingTable[i].size();j++){
			m_SettingTable[i][j].print();
		}
		cout<<endl;
	}

	cout<<"i=1:j=1"<<endl;
	m_SettingTable[1][1].print();
	cout<<endl;

	// vector<Node*> bottleneck;
	// Graph g(m_solution,m_SettingTable);
	// for(int i=0;i<g.size();i++){
	// 	if(g[i]->m_R+g[i]->m_Q-g[i]->m_Jobpair->time>m_L){
	// 		bottleneck.push_back(g[i]);
	// 	}
	// }

	// cout<<"bottleneck Node"<<endl;
	// for(int i=0;i<bottleneck.size();i++){
	// 	cout<<"("<<bottleneck[i]->m_Jobpair->machine<<","<<bottleneck[i]->m_Jobpair->jobIndex<<","<<bottleneck[i]->m_Jobpair->time<<") ";
	// }
	// cout<<endl;

	// m_CriticalPathList.clear();
	// vector<Node*> criticalPath;
	// findCriticalPath(g[0],g[g.size()-1],bottleneck,criticalPath);

	// cout<<"critical Path"<<endl;
	// for(int i=0;i<m_CriticalPathList.size();i++){
	// 	for(int j=0;j<m_CriticalPathList[i].size();j++){
	// 		cout<<"("<<m_CriticalPathList[i][j]->m_Jobpair->machine<<","<<m_CriticalPathList[i][j]->m_Jobpair->jobIndex<<") ";
	// 	}
	// 	cout<<endl;
	// }

	// for(int i=0;i<m_CriticalPathList.size();i++){
	// 	cout<<"m_CriticalPathList["<<i<<"]"<<endl;
	// 	for(int j=0;j<m_CriticalPathList[i].size();j++){
	// 		cout<<"("<<m_CriticalPathList[i][j]->m_Jobpair->machine<<","<<m_CriticalPathList[i][j]->m_Jobpair->jobIndex<<") ";
	// 	}
	// 	cout<<endl;

	// 	criticalPath.clear();
	// 	criticalPath=m_CriticalPathList[i];
	// 	for(int j=0;j<criticalPath.size()-1;j++){
	// 		for(int k=j+1;k<criticalPath.size();k++){
	// 			if(criticalPath[j]->m_Jobpair->machine!=
	// 				criticalPath[k]->m_Jobpair->machine)
	// 				continue;
	// 			if(criticalPath[j]->m_Jobpair->machine==-1)
	// 				continue;
	// 			JobPair *alphaI=findJobFromSetting(criticalPath[j]->m_Jobpair,PREV);
	// 			JobPair *gammaJ=findJobFromSetting(criticalPath[k]->m_Jobpair,NEXT);
	// 			vector<vector<JobPair> > solution;
	// 			// solution=changeBackward(m_solution,criticalPath[j]->m_Jobpair,criticalPath[k]->m_Jobpair);
	// 			solution=changeForward(m_solution,criticalPath[j]->m_Jobpair,criticalPath[k]->m_Jobpair);
	// 		}
	// 	}
	// }
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
			#ifdef DEBUG
				cout<<"m_Next["<<i<<"]="<<node->m_Next[i]->m_Jobpair->index<<":bottleneck["<<j<<"]="<<bottleneck[j]->m_Jobpair->index<<endl;
			#endif
			if(node->m_Next[i]->m_Jobpair->index==bottleneck[j]->m_Jobpair->index){
				_criticalPath.push_back(node);
				findCriticalPath(node->m_Next[i],leaf,bottleneck,_criticalPath);
				break;
			}
		}
	}
}

vector<vector<JobPair> > NeighbourGenerator::changeBackward(const vector<vector<JobPair> >& solution,JobPair* I,JobPair* J){
	// iの前にj
	vector<vector<JobPair> >_solution=solution;
	int machine=I->machine;
	int iIndex=-1,jIndex=-1;
	for(int i=0;i<solution[machine].size();i++){
		if(solution[machine][i].index==I->index){
			iIndex=i;
		}
		if(solution[machine][i].index==J->index){
			jIndex=i;
		}
	}

	cout<<"before solution["<<machine<<"]"<<endl;
	for(int i=0;i<_solution[machine].size();i++){
		cout<<"("<<_solution[machine][i].machine<<","<<_solution[machine][i].jobIndex<<") ";
	}
	cout<<endl;
	cout<<"iIndex="<<iIndex<<":jIndex="<<jIndex<<endl;
	insertBefore(_solution[machine],iIndex,jIndex);
	cout<<"after solution["<<machine<<"]"<<endl;
	for(int i=0;i<_solution[machine].size();i++){
		cout<<"("<<_solution[machine][i].machine<<","<<_solution[machine][i].jobIndex<<") ";
	}
	cout<<endl;
	return _solution;
}

vector<vector<JobPair> > NeighbourGenerator::changeForward(const vector<vector<JobPair> >& solution,JobPair* I,JobPair* J){
	// iをjの後に
	vector<vector<JobPair> >_solution=solution;
	int machine=I->machine;
	int iIndex=-1,jIndex=-1;
	for(int i=0;i<solution[machine].size();i++){
		if(solution[machine][i].index==I->index){
			iIndex=i;
		}
		if(solution[machine][i].index==J->index){
			jIndex=i;
		}
	}

	cout<<"before solution["<<machine<<"]"<<endl;
	for(int i=0;i<_solution[machine].size();i++){
		cout<<"("<<_solution[machine][i].machine<<","<<_solution[machine][i].jobIndex<<") ";
	}
	cout<<endl;
	cout<<"iIndex="<<iIndex<<":jIndex="<<jIndex<<endl;
	insertAfter(_solution[machine],jIndex,iIndex);
	cout<<"after solution["<<machine<<"]"<<endl;
	for(int i=0;i<_solution[machine].size();i++){
		cout<<"("<<_solution[machine][i].machine<<","<<_solution[machine][i].jobIndex<<") ";
	}
	cout<<endl;
	return _solution;
}

// vector<vector<JobPair> > NeighbourGenerator::getSolution(int index){
// 	return null;
// }

void NeighbourGenerator::insertBefore(vector<JobPair>& solution,int tar,int src){
	vector<JobPair>::iterator it;
	JobPair jp;
	int i;
	for(i=0,it=solution.begin();it!=solution.end();it++,i++)
		if(i==src){
			jp=solution[i];
			break;
		}
	solution.erase(it);
	
	for(i=0,it=solution.begin();it!=solution.end();it++,i++)
		if(i==tar){
			break;
		}
	solution.insert(it,jp);

}

void NeighbourGenerator::insertAfter(vector<JobPair>& solution,int tar,int src){
	vector<JobPair>::iterator it;
	JobPair jp;
	int i;
	for(i=0,it=solution.begin();it!=solution.end();it++,i++)
		if(i==src){
			jp=solution[i];
			break;
		}
	solution.erase(it);
	
	for(i=0,it=solution.begin();it!=solution.end();it++,i++)
		if(i==tar){
			break;
		}
	solution.insert(it,jp);
}

JobPair* NeighbourGenerator::findJobFromSetting(JobPair* jp,enum ORDER order){
	int jobIndex=jp->jobIndex;
	for(int i=0;i<m_SettingTable[jobIndex].size();i++){
		if(m_SettingTable[jobIndex][i].index!=jp->index)
			continue;
		if(i==0 && order==PREV){
			return NULL;
		}
		if(i==m_SettingTable[jobIndex].size()-1 && order==NEXT){
			return NULL;
		}
		return &m_SettingTable[jobIndex][i+order];
	}
}