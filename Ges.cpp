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
	vector<Node*> array;
	array.push_back(root);
	for(int i=0;i<solution.size();i++){
		now=root;
		for(int j=0;j<solution[i].size();j++){
			Node* node=new Node(&solution[i][j]);
			array.push_back(node);
			now->addNode(array.size()-1);
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
	vector<Node*> sort=topologicalSort(root);
	for(int i=0;i<sort.size();i++){
		int max=0;
		for(int j=0;j<sort[i]->m_Prev.size();j++){
			if(max<sort[i]->m_Prev[j]->m_R){
				max=sort[i]->m_Prev[j]->m_R;
			}
		}
		sort[i]->m_R=max+sort[i]->m_Jobpair->time;
	}
	for(int i=sort.size()-1;i>=0;i--){
		int max=0;
		for(int j=0;j<sort[i]->m_Next.size();j++){
			if(max<sort[i]->m_Next[j]->m_Q){
				max=sort[i]->m_Next[j]->m_Q;
			}
		}
		sort[i]->m_Q=max+sort[i]->m_Jobpair->time;
	}
}

vector<Node*> Ges::topologicalSort(Node* root){
	stack<Node*> sort;
	visit(root,sort);
	vector<Node*> sortVector;
	while(!sort.empty()){
		sortVector.push_back(sort.top());
		sortVector[sortVector.size()-1]->setIndex(sortVector.size()-1);
		sort.pop();
	}
	return sortVector;
}

void Ges::visit(Node* node,stack<Node*>& sort){
	if(node->isCheck())
		return;
	node->check();
	for(int i=0;i<node->m_Next.size();i++){
		visit(node->m_Next[i],sort);
	}
	sort.push(node);
}

int Ges::getMakespan(const vector<vector<JobPair> > &solution){
	Node *root=makeGraph(solution);
	setLongestPath(root);
	delete(root);
	return root->m_Q;
}

Ges::~Ges(){

}