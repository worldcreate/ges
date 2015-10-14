#include "Test.h"
#include "FileReader.h"
#include "JobPair.h"
#include "Gt.h"
#include "Graph.h"
#include "NeighbourGenerator.h"
#include <iostream>
#include <vector>

using namespace std;

Test::Test(){
}

void Test::test(int argc,char** argv){
	vector<vector<JobPair> > m_Solution;	// 縦Machine 横投入順序の配列
	vector<vector<JobPair> > m_SettingTable;	// 縦Job 横技術的順序

	const char* m_FileName="FT3.txt";
	int i=0;
	while(argc>i){
		if(argv[i][0]=='-'){
			const char *arg=&argv[i][2];
			switch(argv[i][1]){
				case 'f':
					m_FileName=arg;
				break;
			}
		}
		i++;
	}

	FileReader fr(m_FileName);
	m_SettingTable=fr.getTable();



	// Gt gt(m_FileName);
	// gt.execute();
	// vector<vector<int> > matrix=gt.getMatrix();

	int temp[3][3]={
		{0,2,1},
		{0,1,2},
		{0,1,2}
	};

	vector<vector<int> > matrix=vector<vector<int> >(3,vector<int>(3,0));
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			matrix[i][j]=temp[i][j];
		}
	}


	m_Solution.resize(m_SettingTable.size());
	for(int m=0;m<matrix.size();m++){
		for(int i=0;i<matrix[m].size();i++){
			int job=matrix[m][i];
			for(int j=0;j<matrix[m].size();j++){
				if(m_SettingTable[job][j].machine==m){
					m_Solution[m].push_back(m_SettingTable[job][j]);
					break;
				}
			}
		}
	}

	// cout<<"(m,j,t,i)"<<endl;
	// for(int i=0;i<m_Solution.size();i++){
	// 	for(int j=0;j<m_Solution[i].size();j++){
	// 		cout<<"("<<m_Solution[i][j].machine<<","<<m_Solution[i][j].jobIndex<<","<<m_Solution[i][j].time<<","<<m_Solution[i][j].index<<") ";
	// 	}
	// 	cout<<endl;
	// }

	// cout<<"graph"<<endl;
	Graph graph(m_Solution,m_SettingTable);
	graph.printForTsort();


	// NeighbourGenerator ng(m_Solution,graph.getMakespan()-1);
	// ng.makeNeighbour(m_SettingTable);
}