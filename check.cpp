#include "FileReader.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main(void){
	FileReader fr("FT10.txt");
	string str;

	while(getline(cin,str)!=NULL){
		cout<<"["<<str<<"]"<<endl;
	}
}

vector<string> split(const string &str, char delim){
	istringstream iss(str); string tmp; vector<string> res;
	while(getline(iss, tmp, delim)) res.push_back(tmp);
	return res;
}