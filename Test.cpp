#include "Test.h"
#include <iostream>
#include "FileReader.h"
#include "JobPair.h"

using namespace std;

Test::Test(){
}

void Test::test(){
	FileReader fr("FT3.txt");
	vector<vector<JobPair> > table=fr.getTable();
	for(int i=0;i<table.size();i++){
		for(int j=0;j<table[i].size();j++){
			cout<<"("<<table[i][j].machine<<","<<table[i][j].jobIndex<<") ";
		}
		cout<<endl;
	}
}