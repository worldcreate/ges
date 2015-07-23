#include "Ges.h"
#include "FileReader.h"
#include <algorithm>
#include <iostream>

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

	#ifdef DEBUG
		cout<<"debug "<<endl;
		for(int i=0;i<m_SettingTable.size();i++){
			for(int j=0;j<m_SettingTable[i].size();j++){
				cout<<"("<<m_SettingTable[i][j].machine<<","<<m_SettingTable[i][j].jobIndex<<","<<m_SettingTable[i][j].time<<") ";
			}
			cout<<endl;
		}
		cout<<endl;
	#endif

	for(int i=0;i<m_SettingTable.size()*m_SettingTable[0].size()+2;i++)
		m_Penalty.push_back(1);

	initialSolution();
}

void Ges::initialSolution(){
	solution.resize(m_SettingTable.size());
	for(int i=0;i<solution.size();i++){
		solution[i].resize(m_SettingTable[i].size());
		for(int j=0;j<solution[i].size();j++){
			solution[i][j]=m_SettingTable[i][j];
		}
		random_shuffle(solution[i].begin(),solution[i].end());
	}
}

void Ges::execute(){
	vector<vector<JobPair> > _solution=solution;
	int L=getMakespan(_solution)-1;
	while(m_Iter<m_MaxIter){

	}
}

int Ges::getMakespan(const vector<vector<JobPair> > &solution){
	return 0;
}

Ges::~Ges(){

}