#include "Ges.h"
#include "FileReader.h"
#include "Gt.h"
#include <algorithm>
#include <iostream>
#include <climits>

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
	while(!m_EP.empty())
		m_EP.pop();

	Graph graph(_solution);
	int L=graph.getMakespan()-1;
	cout<<"L="<<L<<endl;
	vector<vector<JobPair> > I;
	Ejection(_solution,I,L);
	#ifdef DEBUG
		cout<<"I list"<<endl;

		for(int i=0;i<I.size();i++){
			for(int j=0;j<I[i].size();j++){
				cout<<"("<<I[i][j].jobIndex<<","<<I[i][j].machine<<",";
				cout<<I[i][j].time<<") ";
			}
			cout<<endl;
		}
	#endif
	vector<JobPair> candidate=selectEP(I);
	#ifdef DEBUG
		cout<<"candidate"<<endl;
		for(int i=0;i<candidate.size();i++){
			cout<<"("<<candidate[i].jobIndex<<","<<candidate[i].machine<<",";
			cout<<candidate[i].time<<") ";
		}
		cout<<endl;
	#endif
	for(int i=0;i<candidate.size();i++){
		JobPair jp=candidate[i];
		int machine=jp.machine;

		vector<JobPair>::iterator it=_solution[machine].begin();
		for(;it!=_solution[machine].end();it++){
			if((*it)==jp){
				_solution[machine].erase(it);
				break;
			}
		}
		m_EP.push(jp);

	}
	#ifdef DEBUG
		cout<<"after Eject"<<endl;
		for(int i=0;i<_solution.size();i++){
			for(int j=0;j<_solution[i].size();j++){
				cout<<"("<<_solution[i][j].jobIndex<<","<<_solution[i][j].machine<<",";
				cout<<_solution[i][j].time<<") ";
			}
			cout<<endl;
		}
		cout<<"graph"<<endl;
		Graph g(_solution);
		g.print();
	#endif
	while(m_Iter<m_MaxIter){
		m_Iter++;
	}
}

vector<JobPair> Ges::selectEP(vector<vector<JobPair> >& I){
	int index=-1;
	int size=INT_MAX;

	for(int i=0;i<I.size();i++){
		int tSize=0;
		for(int j=0;j<I[i].size();j++){
			tSize+=m_Penalty[I[i][j].index];
		}
		if(size>tSize){
			size=tSize;
			index=i;
		}
	}
	return I[index];
}

void Ges::Ejection(vector<vector<JobPair> > _solution,vector<vector<JobPair> >& a_I,int L){
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

	vector<JobPair> candidates;
	Ejection(graph,bottleneckNode,candidates,a_I,0,L);
}

void Ges::Ejection(Graph graph,deque<Node*> bottleneckNode,vector<JobPair> a_candidates,vector<vector<JobPair> >& a_I,int count,int L){
	if(count>=m_kMax || bottleneckNode.empty()){
		return;
	}
	// 辞書順に抜き出しす
	while(!bottleneckNode.empty()){
		Node* node=bottleneckNode.front();
		bottleneckNode.pop_front();
		int index=node->getIndex();

		graph.removeNode(index);
		a_candidates.push_back(*graph[index]->m_Jobpair);
		#ifdef DEBUG
			cout<<count<<"times"<<endl;
			cout<<"remove node["<<index<<"]"<<endl;
			graph.print();
			cout<<"candidates list"<<endl;
			for(int i=0;i<a_candidates.size();i++){
				cout<<"("<<a_candidates[i].jobIndex<<","<<a_candidates[i].machine<<",";
				cout<<a_candidates[i].time<<") ";
			}
			cout<<endl;
		#endif

		if(graph.getMakespan()<=L){
			a_I.push_back(a_candidates);
		}
		
		Ejection(graph,bottleneckNode,a_candidates,a_I,count+1,L);
		a_candidates.pop_back();
	}
}

Ges::~Ges(){

}