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
	Graph(const vector<vector<JobPair> >&);
	~Graph();
	int getMakespan();
private:
	void setLongestPath();
	void topologicalSort();
	void visit(Node*,stack<Node*>&);
	vector<Node*> array;

};

#endif