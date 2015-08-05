#include "Ges.h"
#include "FileReader.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>

using namespace std;

Ges::Ges(int argc,char **argv){
	m_Iter=0;
	m_MaxIter=100;
	m_FileName="FT3.txt";
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
	m_Solution.resize(m_SettingTable.size());
	for(int i=0;i<m_SettingTable.size();i++){
		for(int j=0;j<m_SettingTable[i].size();j++){
			m_Solution[m_SettingTable[i][j].machine].push_back(m_SettingTable[i][j]);
		}
	}
	for(int i=0;i<m_Solution.size();i++){
		random_shuffle(m_Solution[i].begin(),m_Solution[i].end());
	}
}

void Ges::execute(){
	vector<vector<JobPair> > _solution=m_Solution;
	int L=getMakespan(_solution)-1;
	while(m_Iter<m_MaxIter){
		m_Iter++;
	}
}

Node* Ges::makeGraph(const vector<vector<JobPair> > &solution){
	Node* root=new Node();
	Node* leaf=new Node();
	Node* now=root;
	for(int i=0;i<solution.size();i++){
		now=root;
		for(int j=0;j<solution[i].size();j++){
			Node* node=new Node(&solution[i][j]);
			now->addNode(node);
			now=node;
		}
		now->addNode(leaf);
	}
	stack<Node*> search;
	search.push(root);
	while(!search.empty()){
		Node *node=search.top();
		search.pop();
		for(int i=0;i<node->m_Next.size();i++){
			search.push(node->m_Next[i]);
		}
		int nextMachine=node->m_Jobpair->next;
		if(nextMachine==-1)
			continue;
		Node *now=root->m_Next[nextMachine];
		while(now->m_Jobpair->machine!=-1){
			if(node->m_Jobpair->jobIndex==now->m_Jobpair->jobIndex){
				node->addNode(now);
				break;
			}
			now=now->m_Next[0];
		}
	}
	return root;
}

void Ges::setLongestPath(Node* root){
	queue<Node*> search;
	search.push(root);
	while(!search.empty()){
		Node* node=search.front();
		search.pop();
		if(node->isCheck())
			continue;
		int max=0;
		for(int i=0;i<node->m_Prev.size();i++){
			if(max<node->m_Prev[i]->m_R){
				max=node->m_Prev[i]->m_R;
			}
		}
		node->m_R=max+node->m_Jobpair->time;
		for(int i=0;i<node->m_Next.size();i++){
			search.push(node->m_Next[i]);
		}
		node->check();
	}
}

int Ges::getMakespan(const vector<vector<JobPair> > &solution){
	Node *root=makeGraph(solution);
	setLongestPath(root);
	for(int i=0;i<3;i++){
		root->m_Next[i]->print();
		cout<<endl;
	}
	return 0;
}

Ges::~Ges(){

}