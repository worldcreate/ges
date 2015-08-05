#ifndef _NODE_H_
#define _NODE_H_

#include "JobPair.h"
#include <vector>
#include <iostream>

using namespace std;

class Node{
public:
	Node():m_Checked(false),m_R(0),m_Q(0){
		m_Jobpair=new JobPair();
	}
	Node(const JobPair* jobpair):m_Checked(false),m_R(0),m_Q(0){
		m_Jobpair=new JobPair();
		(*m_Jobpair)=(*jobpair);
	}
	void addNode(Node* node){
		m_Next.push_back(node);
		node->addPrev(this);
	}
	void addPrev(Node* node){
		m_Prev.push_back(node);
	}
	void print(){
		cout<<"("<<m_Jobpair->machine<<","<<m_Jobpair->jobIndex<<","<<m_Next.size();
		cout<<","<<m_Prev.size()<<","<<m_R<<") ";
		if(m_Next.size()>0)
			m_Next[0]->print();
	}
	bool isCheck(){
		return m_Checked;
	}
	void check(){
		m_Checked=true;
	}
	~Node(){
		delete m_Jobpair;
		for(int i=0;i<m_Next.size();i++){
			delete m_Next[i];
		}
	}
	JobPair *m_Jobpair;
	vector<Node*> m_Next;
	vector<Node*> m_Prev;
	int m_R;
	int m_Q;
private:
	int m_Index;
	bool m_Checked;
};

#endif