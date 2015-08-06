#include "Graph.h"

Graph::Graph(){

}

Graph::Graph(const vector<vector<JobPair> >& solution){
	Node* root=new Node();
	Node* leaf=new Node();
	Node* now=root;
	array.push_back(root);
	for(int i=0;i<solution.size();i++){
		now=root;
		for(int j=0;j<solution[i].size();j++){
			Node* node=new Node(&solution[i][j]);
			array.push_back(node);
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
	setLongestPath();
}

void Graph::setLongestPath(){
	topologicalSort();
	for(int i=0;i<array.size();i++){
		int max=0;
		for(int j=0;j<array[i]->m_Prev.size();j++){
			if(max<array[i]->m_Prev[j]->m_R){
				max=array[i]->m_Prev[j]->m_R;
			}
		}
		array[i]->m_R=max+array[i]->m_Jobpair->time;
	}
	for(int i=array.size()-1;i>=0;i--){
		int max=0;
		for(int j=0;j<array[i]->m_Next.size();j++){
			if(max<array[i]->m_Next[j]->m_Q){
				max=array[i]->m_Next[j]->m_Q;
			}
		}
		array[i]->m_Q=max+array[i]->m_Jobpair->time;
	}
}

void Graph::topologicalSort(){
	stack<Node*> sort;
	visit(array[0],sort);
	array.clear();
	while(!sort.empty()){
		array.push_back(sort.top());
		array[array.size()-1]->setIndex(array.size()-1);
		sort.pop();
	}
}

void Graph::visit(Node* node,stack<Node*>& sort){
	if(node->isCheck())
		return;
	node->check();
	for(int i=0;i<node->m_Next.size();i++){
		visit(node->m_Next[i],sort);
	}
	sort.push(node);
}

int Graph::getMakespan(){
	return array[0]->m_Q;
}

Graph::~Graph(){
	for(int i=0;i<array.size();i++){
		delete array[i];
	}
}