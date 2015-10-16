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
	// vector<vector<JobPair> > getNeighbour(int);
private:
	void findCriticalPath(Node*,Node*,vector<Node*>&,vector<Node*>&);
	vector<vector<JobPair> > changeBackward(const vector<vector<JobPair> >&,JobPair*,JobPair*);
	vector<vector<JobPair> > changeForward(const vector<vector<JobPair> >&,JobPair*,JobPair*);
	void insertBefore(vector<JobPair>&,int,int);
	void insertAfter(vector<JobPair>&,int,int);
	vector<vector<JobPair> > m_solution;
	vector<vector<JobPair> > m_SettingTable;
	vector<vector<Node*> > m_CriticalPathList;
	// NeighbourList
	int m_L;
};

#endif