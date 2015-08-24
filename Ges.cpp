#include "Ges.h"
#include "FileReader.h"
#include "Gt.h"
#include <algorithm>
#include <iostream>

using namespace std;

Ges::Ges(int argc,char **argv){
	m_Iter=0;
	m_MaxIter=100;
	m_FileName="FT3.txt";
	m_kMax=3;
	int i=1;
	while(argc>i){
		if(argv[i][0]=='-'){
			const char *arg=&argv[i][2];
			switch(argv[i][1]){
				case 'f':
					m_FileName=arg;
				break;
				case 'i':
					m_MaxIter=atoi(arg);
				break;
			}
		}
		i++;
	}
	FileReader fr(m_FileName);
	m_SettingTable=fr.getTable();

	for(int i=0;i<m_SettingTable.size()*m_SettingTable[0].size()+2;i++)
		m_Penalty.push_back(1);

	initialSolution();
}

void Ges::initialSolution(){
	Gt gt(m_FileName);
	gt.execute();
	vector<vector<int> > matrix=gt.getMatrix();

	#ifdef DEBUG
		cout<<"Matrix"<<endl;
		for(int i=0;i<matrix.size();i++){
			for(int j=0;j<matrix[i].size();j++){
				cout<<matrix[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	#endif

	m_Solution.resize(m_SettingTable.size());
	for(int m=0;m<matrix.size();m++){
		for(int i=0;i<matrix[m].size();i++){
			int job=matrix[m][i];
			for(int j=0;j<matrix[m].size();j++){
				if(m_SettingTable[job][j].machine==m){
					m_Solution[m].push_back(m_SettingTable[job][j]);
					break;
				}
			}
		}
	}

	#ifdef DEBUG
		cout<<"m_Solution"<<endl;
		for(int i=0;i<m_Solution.size();i++){
			for(int j=0;j<m_Solution[i].size();j++){
				cout<<"("<<m_Solution[i][j].jobIndex<<","<<m_Solution[i][j].machine;
					cout<<","<<m_Solution[i][j].time<<") ";
			}
			cout<<endl;
		}
	#endif
}

void Ges::execute(){
	vector<vector<JobPair> > _solution=m_Solution;
	Graph graph(_solution);
	int L=graph.getMakespan()-1;
	cout<<"makespan="<<L<<endl;
	Ejection(_solution,L);
	while(m_Iter<m_MaxIter){
		m_Iter++;
	}
}

void Ges::Ejection(vector<vector<JobPair> > _solution,int L){
	Graph graph(_solution);
	deque<Node*> bottleneckNode;

	// ボトルネックノード抽出
	for(int i=0;i<graph.size();i++){
		if(graph[i]->m_R+graph[i]->m_Q-graph[i]->m_Jobpair->time>L){
			if(graph[i]->m_Jobpair->machine!=-1)
				bottleneckNode.push_back(graph[i]);
		}
	}
	#ifdef DEBUG
		cout<<"bottleneck Node"<<endl;
		for(int i=0;i<bottleneckNode.size();i++){
			cout<<bottleneckNode.at(i)->getIndex()<<" ";
		}
		cout<<endl;
	#endif
	graph.print();
	Ejection(graph,bottleneckNode,0,L);
}

void Ges::Ejection(Graph graph,deque<Node*> bottleneckNode,int count,int L){
	if(count>=m_kMax || bottleneckNode.empty()){
		return;
	}
	while(!bottleneckNode.empty()){
		Node* node=bottleneckNode.front();
		bottleneckNode.pop_front();
		int index=node->getIndex();

		graph.removeNode(index);
		#ifdef DEBUG
			cout<<count<<"times"<<endl;
			cout<<"remove node["<<index<<"]"<<endl;
			graph.print();

		#endif
			
		/**
		 * TODO
		 * 判定とか
		 */
		
		Ejection(graph,bottleneckNode,count+1,L);
	}
}

Ges::~Ges(){

}