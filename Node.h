#ifndef _NODE_H_
#define _NODE_H_

#include "JobPair.h"
#include "MemoryManagement.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Node{
public:
	Node(bool flag=false):m_Checked(false),m_tempChecked(false),m_R(0),m_Q(0),output(flag){
		if(output){
			cout<<"Node default constructor"<<endl;
		}
		m_Jobpair=new JobPair();
		#ifdef DEBUG
			MemoryManagement::getInstance()->addAddress(m_Jobpair,string("JobPair (in Node)"));
		#endif
		m_Jobpair->time=0;
	}
	Node(const JobPair* jobpair,bool flag=false):m_Checked(false),m_tempChecked(false),m_R(0),m_Q(0),output(flag){
		if(output){
			cout<<"Node const JobPair* constructor"<<endl;
		}
		m_Jobpair=new JobPair();
		#ifdef DEBUG
			MemoryManagement::getInstance()->addAddress(m_Jobpair,string("JobPair (in Node)"));
		#endif
		(*m_Jobpair)=(*jobpair);
	}
	Node(const Node& node,bool flag=false):output(flag){
		if(output){
			cout<<"Node const Node& constructor"<<endl;
		}
		m_Jobpair=new JobPair();
		#ifdef DEBUG
			MemoryManagement::getInstance()->addAddress(m_Jobpair,string("JobPair (in Node)"));
		#endif
		(*m_Jobpair)=(*node.m_Jobpair);
		m_R=node.m_R;
		m_Q=node.m_Q;
		this->m_Index=node.getIndex();
		m_Checked=node.isCheck();
		m_tempChecked=node.istempCheck();
		m_Prev.clear();
		m_Next.clear();
		output=node.output;
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
	bool istempCheck() const{
		return m_tempChecked;
	}
	void check(){
		m_Checked=true;
	}
	void checkTemp(){
		m_tempChecked=true;
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
		if(output){
			cout<<"Node destructor"<<endl;
		}
		#ifdef DEBUG
			MemoryManagement::getInstance()->removeAddress(m_Jobpair);
		#endif
		delete m_Jobpair;
	}
	JobPair *m_Jobpair;
	vector<Node*> m_Next;
	vector<Node*> m_Prev;
	int m_R;	//0からそのnodeまでの最長経路
	int m_Q;	//nodeからn+1までの最長経路
	bool output;
private:
	int m_Index;
	bool m_Checked;
	bool m_tempChecked;
};

#endif