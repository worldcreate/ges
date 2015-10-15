#ifndef _NEIGHBOURGENERATOR_H_
#define _NEIGHBOURGENERATOR_H_

#include "JobPair.h"
#include <vector>

using namespace std;

class Node;

class NeighbourGenerator{
public:
	NeighbourGenerator();
	NeighbourGenerator(vector<vector<JobPair> >&,vector<vector<JobPair> >&,int);
	void makeNeighbour();
	// vector<vector<JobPair> > getSolution(int);
private:
	void findCriticalPath(Node*,Node*,vector<Node*>&,vector<Node*>&);
	// vector<Node*> changeAfter(const vector<Node*>&,int,int);
	// vector<Node*> changeBefore(const vector<Node*>&,int,int);f
	vector<vector<JobPair> > m_solution;
	vector<vector<JobPair> > m_SettingTable;
	vector<vector<Node*> > m_CriticalPathList;
	int m_L;
};

#endif