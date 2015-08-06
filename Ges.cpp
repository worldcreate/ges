#include "Ges.h"
#include "FileReader.h"
#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>

using namespace std;

Ges::Ges(int argc,char **argv){
	m_Iter=0;
	m_MaxIter=100;
	m_FileName="FT3.txt";
	int i=1;
	while(argc>i){
		if(argv[i][0]=='-'){
			const char *arg=&argv[i][2];
			switch(argv[i][1]){
				case 'f':
					m_FileName=arg;
				break;
				case 'i':
					m_MaxIter=atoi(arg);
				break;
			}
		}
		i++;
	}
	FileReader fr(m_FileName);
	m_SettingTable=fr.getTable();

	for(int i=0;i<m_SettingTable.size()*m_SettingTable[0].size()+2;i++)
		m_Penalty.push_back(1);

	initialSolution();
}

void Ges::initialSolution(){
	m_Solution.resize(m_SettingTable.size());
	for(int i=0;i<m_SettingTable.size();i++){
		for(int j=0;j<m_SettingTable[i].size();j++){
			m_Solution[m_SettingTable[i][j].machine].push_back(m_SettingTable[i][j]);
		}
	}
	for(int i=0;i<m_Solution.size();i++){
		random_shuffle(m_Solution[i].begin(),m_Solution[i].end());
	}
}

void Ges::execute(){
	vector<vector<JobPair> > _solution=m_Solution;
	Graph graph(_solution);
	int L=graph.getMakespan()-1;
	cout<<"makespan="<<L<<endl;
	while(m_Iter<m_MaxIter){
		m_Iter++;
	}
}

Ges::~Ges(){

}