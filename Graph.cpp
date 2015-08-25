#include "Graph.h"

Graph::Graph(){

}

Graph::Graph(const vector<vector<JobPair> >& solution,const vector<vector<JobPair> >& settingTable){
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
		int tarIndex=node->m_Jobpair->nextIndex;
		int jobIndex=node->m_Jobpair->jobIndex;
		if(tarIndex==-1)
			continue;
		Node *now=root->m_Next[nextMachine];
		while(tarIndex!=-1){
			if(tarIndex==now->m_Jobpair->index){
				node->addNode(now);
				break;
			}
			now=now->m_Next[0];
			if(now==leaf){
				for(int i=0;i<settingTable[jobIndex].size();i++){
					if(settingTable[jobIndex][i].index==tarIndex){
						tarIndex=settingTable[jobIndex][i].nextIndex;
						nextMachine=settingTable[jobIndex][i].next;
						now=root->m_Next[nextMachine];
						break;
					}
				}
			}
		}
	}
	setLongestPath();
}

Graph::Graph(const Graph& graph){
	array.resize(graph.size());
	for(int i=0;i<array.size();i++){
		Node *node=new Node(*graph[i]);
		array[i]=node;
	}
	for(int i=0;i<array.size();i++){
		for(int j=0;j<graph[i]->m_Prev.size();j++){
			int idx=graph[i]->m_Prev[j]->getIndex();
			for(int k=0;k<array.size();k++){
				if(idx==array[k]->getIndex()){
					array[i]->m_Prev.push_back(array[k]);
					break;
				}
			}
		}
	}
	for(int i=0;i<array.size();i++){
		for(int j=0;j<graph[i]->m_Next.size();j++){
			int idx=graph[i]->m_Next[j]->getIndex();
			for(int k=0;k<array.size();k++){
				if(idx==array[k]->getIndex()){
					array[i]->m_Next.push_back(array[k]);
					break;
				}
			}
		}
	}
}

int Graph::size() const{
	return array.size();
}

Node* Graph::operator[](int n) const{
	return array[n];
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

void Graph::removeNode(int index){
	Node* tar=array[index];
	Node* prev=tar->m_Prev[0];
	Node* next=tar->m_Next[0];

	/* 投入順序のつなぎ変え */
	for(int i=0;i<prev->m_Next.size();i++){
		if(prev->m_Next[i]==tar){
			prev->m_Next[i]=next;
			break;
		}
	}
	for(int i=0;i<next->m_Prev.size();i++){
		if(next->m_Prev[i]==tar){
			next->m_Prev[i]=prev;
			break;
		}
	}

	
	// 技術的順序のつなぎ変え
	if(array[index]->m_Prev.size()==1){
		// 技術的順序の最初のノードの場合
		array[index]->m_Next[1]->m_Prev.pop_back();
	}else if(array[index]->m_Next.size()==1){
		// 技術的順序の最後のノードの場合
		array[index]->m_Prev[1]->m_Next.pop_back();
	}else{
		prev=array[index]->m_Prev[1];
		next=array[index]->m_Next[1];
		prev->m_Next[1]=next;
		next->m_Prev[1]=prev;
	}

	for(int i=index+1;i<array.size();i++){
		int max=0;
		for(int j=0;j<array[i]->m_Prev.size();j++){
			if(max<array[i]->m_Prev[j]->m_R){
				max=array[i]->m_Prev[j]->m_R;
			}
		}
		array[i]->m_R=max+array[i]->m_Jobpair->time;
	}

	for(int i=index-1;i>=0;i--){
		int max=0;
		for(int j=0;j<array[i]->m_Next.size();j++){
			if(max<array[i]->m_Next[j]->m_Q){
				max=array[i]->m_Next[j]->m_Q;
			}
		}
		array[i]->m_Q=max+array[i]->m_Jobpair->time;
	}
}

void Graph::print(){
	cout<<"- R,Q,J,M,N,P"<<endl;
	for(int i=0;i<array.size();i++){
		cout<<i<<" "<<array[i]->m_R<<","<<array[i]->m_Q<<","<<array[i]->m_Jobpair->jobIndex<<","<<array[i]->m_Jobpair->machine<<",";
		cout<<"N(";
		for(int j=0;j<array[i]->m_Next.size();j++){
			cout<<array[i]->m_Next[j]->getIndex()<<",";
		}
		cout<<"),";
		cout<<"P(";
		for(int j=0;j<array[i]->m_Prev.size();j++){
			cout<<array[i]->m_Prev[j]->getIndex()<<",";
		}
		cout<<")";
		cout<<endl;
	}
}

Graph::~Graph(){
	for(int i=0;i<array.size();i++){
		delete array[i];
	}
}