#ifndef _NEIGHBOURGENERATOR_H_
#define _NEIGHBOURGENERATOR_H_

#include "JobPair.h"
#include <vector>

using namespace std;

class NeighbourGenerator{
public:
	NeighbourGenerator();
	NeighbourGenerator(vector<vector<JobPair> >&,int);
	void makeNeighbour();
	vector<vector<JobPair> > getSolution(int);
private:
	vector<vector<JobPair> >& m_solution;
	int m_L;
};

#endif