#include <iostream>
#include <cstdio>
#include <windows.h>
#include <vector>

using namespace std;

int main(int argc,char *argv[]){
	vector<int> vec;
	int i=1;
	while(argc>i){
		if(argv[i][0]=='-'){
			const char *arg=&argv[i][2];
			switch(argv[i][1]){
				/*
				case '':
				break;
				*/
			}
		}
		i++;
	}


	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	#pragma omp parallel for
	for(int i=0;i<10000000;i++){
		vec.push_back(i);
	}
	cout<<endl;
	QueryPerformanceCounter(&end);
	cout<<"time:"<<(double)(end.QuadPart-start.QuadPart)/freq.QuadPart<<endl;
}