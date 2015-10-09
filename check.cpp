#include "FileReader.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

vector<string> split(const string &str, char delim){
	istringstream iss(str); string tmp; vector<string> res;
	while(getline(iss, tmp, delim)) res.push_back(tmp);
	return res;
}

int myAtoi(string& src){
	
}

int main(void){
	FileReader fr("FT10.txt");
	string str;

	while(getline(cin,str)!=NULL){
		vector<string> ele=split(str,' ');
		for(int i=0;i<ele.size();i++){
			vector<string> param=split(ele[i],',');
			for(int j=0;j<param.size();j++){
				cout<<param[j]<<":"<<atoi(param[j].c_str())<<endl;
			}
			cout<<endl;
		}
	}
}

