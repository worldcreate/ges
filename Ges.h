#ifndef _GES_H_
#define _GES_H_

#include <list>
#include <vector>
#include <stack>
#include "JobPair.h"
#include "Node.h"

using namespace std;

class Ges{
public:
	Ges(int ,char **);
	void initialSolution();
	void execute();
	~Ges();
private:
	int getMakespan(const vector<vector<JobPair> >&);
	Node* makeGraph(const vector<vector<JobPair> >&);
	void setLongestPath(Node*);

	int m_Iter;
	int m_MaxIter;
	list<int> m_Penalty;
	vector<vector<JobPair> > m_Solution; // 縦Machine 横Jobの配列
	stack<JobPair> m_EP;
	const char* m_FileName;
	vector<vector<JobPair> > m_SettingTable;
};

#endif