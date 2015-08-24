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
		m_Jobpair->time=0;
	}
	Node(const JobPair* jobpair):m_Checked(false),m_R(0),m_Q(0){
		m_Jobpair=new JobPair();
		(*m_Jobpair)=(*jobpair);
	}
	Node(const Node& node){
		m_Jobpair=new JobPair();
		(*m_Jobpair)=(*node.m_Jobpair);
		m_R=node.m_R;
		m_Q=node.m_Q;
		this->m_Index=node.getIndex();
		m_Checked=node.isCheck();
		m_Prev.clear();
		m_Next.clear();
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
		cout<<","<<m_Prev.size()<<","<<m_R<<","<<m_Q<<","<<m_Index<<") ";
		if(m_Next.size()>0)
			m_Next[0]->print();
	}
	void printAll(){
		cout<<"("<<m_Jobpair->machine<<","<<m_Jobpair->jobIndex<<","<<m_Next.size();
		cout<<","<<m_Prev.size()<<","<<m_R<<","<<m_Q<<","<<m_Index<<") ";
		for(int i=0;i<m_Next.size();i++){
			m_Next[i]->print();
			cout<<endl;
		}
	}
	bool isCheck() const{
		return m_Checked;
	}
	void check(){
		m_Checked=true;
	}
	void reset(){
		for(int i=0;i<m_Next.size();i++){
			m_Next[i]->resetChild();
		}
		m_Checked=false;
	}
	void resetChild(){
		m_Next[0]->resetChild();
		m_Checked=false;
	}
	void setIndex(int a_Index){
		m_Index=a_Index;
	}
	int getIndex() const{
		return m_Index;
	}
	~Node(){
		delete m_Jobpair;
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