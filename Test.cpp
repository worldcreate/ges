#include "Test.h"
#include <iostream>
#include <vector>
#include "FileReader.h"
#include "JobPair.h"
#include "Gt.h"

using namespace std;

Test::Test(){
}

void Test::test(){
	Gt gt("FT3.txt");
	gt.execute();
	vector<vector<int> > matrix=gt.getMatrix();
	for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}