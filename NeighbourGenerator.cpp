#include "NeighbourGenerator.h"
#include "Graph.h"

NeighbourGenerator::NeighbourGenerator(){}

NeighbourGenerator::NeighbourGenerator(vector<vector<JobPair> >& a_solution,int L):m_L(L){
	m_solution=a_solution;
}

void NeighbourGenerator::makeNeighbour(vector<vector<JobPair> >& a_settingTable){
	Graph g(m_solution,a_settingTable);
	for(int i=0;i<g.size();i++){
		
	}
}

vector<vector<JobPair> > NeighbourGenerator::getSolution(int index){

}

