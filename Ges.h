#ifndef _GES_H_
#define _GES_H_

#include <list>
#include <vector>
#include <stack>
#include "JobPair.h"

using namespace std;

class Ges{
public:
	Ges(int ,char **);
	void initialSolution();
	void execute();
	~Ges();
private:
	int getMakespan(const vector<vector<JobPair> >&);

	int m_Iter;
	int m_MaxIter;
	list<int> m_Penalty;
	vector<vector<JobPair> > solution; // 縦Job 横Machineの配列
	stack<JobPair> m_EP;
	const char* m_FileName;
	vector<vector<JobPair> > m_SettingTable;
};

#endif