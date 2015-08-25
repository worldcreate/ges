#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <stack>
#include "Node.h"
#include "JobPair.h"

using namespace std;

class Graph{
public:
	Graph();
	Graph(const vector<vector<JobPair> >&,const vector<vector<JobPair> > &);
	Graph(const Graph&);
	int size() const;
	Node* operator[](int n) const;
	void print();
	~Graph();
	int getMakespan();
	void removeNode(int);
private:
	void setLongestPath();
	void topologicalSort();
	void visit(Node*,stack<Node*>&);
	vector<Node*> array;

};

#endif