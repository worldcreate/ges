#ifndef _GES_H_
#define _GES_H_

#include <list>
#include <vector>
#include <stack>
#include <deque>
#include "JobPair.h"
#include "Node.h"
#include "Graph.h"

using namespace std;

class Ges{
public:
	Ges(int ,char **);
	void initialSolution();
	void execute();
	~Ges();
private:
	void Ejection(vector<vector<JobPair> > ,int);
	void Ejection(Graph,deque<Node*>,int,int);

	int m_Iter;
	int m_MaxIter;
	list<int> m_Penalty;
	vector<vector<JobPair> > m_Solution; // 縦Machine 横Jobの配列
	stack<JobPair> m_EP;
	const char* m_FileName;
	vector<vector<JobPair> > m_SettingTable;
	int m_kMax;
};

#endif