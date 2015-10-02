#include <iostream>
#include <stdlib.h>
#include "Test.h"
#include "Ges.h"
#include "Util.h"

using namespace std;

int main(int argc,char *argv[]){
	int i=1;
	int seed=300;
	while(argc>i){
		if(argv[i][0]=='-'){
			const char *arg=&argv[i][2];
			switch(argv[i][1]){
				case 'T':{
					Test t;
					t.test();
					exit(0);
				}
				break;
				case 's':
					seed=atoi(arg);
				break;
			}
		}
		i++;
	}
	Util::setSeed(seed);
	Ges g(argc,argv);
	g.execute();
}