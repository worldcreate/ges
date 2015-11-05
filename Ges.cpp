#include "Ges.h"
#include "FileReader.h"
#include "Gt.h"
#include "Util.h"
#include "NeighbourGenerator.h"
#include <algorithm>
#include <iostream>
#include <climits>
#include <stdexcept>

using namespace std;

Ges::Ges(int argc,char **argv){
	m_Iter=0;
	m_MaxIter=100;
	m_IterRand=100;
	m_FileName="FT3.txt";
	m_kMax=3;
	m_stagLS=50;
	m_maxT=10;
	int i=1;
	while(argc>i){
		if(argv[i][0]=='-'){
			const char *arg=&argv[i][2];
			switch(argv[i][1]){
				case 'f':
					m_FileName=arg;
				break;
				case 'm':
					m_MaxIter=atoi(arg);
				break;
				case 'r':
					m_IterRand=atoi(arg);
				break;
				case 's':
					m_stagLS=atoi(arg);
				break;
			}
		}
		i++;
	}
	FileReader fr(m_FileName);
	m_SettingTable=fr.getTable();

	for(int i=0;i<m_SettingTable.size()*m_SettingTable[0].size()+2;i++)
		m_Penalty.push_back(1);

	initialSolution();
}

void Ges::initialSolution(){
	Gt gt(m_FileName);
	gt.execute();
	vector<vector<int> > matrix=gt.getMatrix();

	#ifdef DEBUG
		cout<<"Matrix"<<endl;
		for(int i=0;i<matrix.size();i++){
			for(int j=0;j<matrix[i].size();j++){
				cout<<matrix[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	#endif

	m_Solution.resize(m_SettingTable.size());
	for(int m=0;m<matrix.size();m++){
		for(int i=0;i<matrix[m].size();i++){
			int job=matrix[m][i];
			for(int j=0;j<matrix[m].size();j++){
				if(m_SettingTable[job][j].machine==m){
					m_Solution[m].push_back(m_SettingTable[job][j]);
					break;
				}
			}
		}
	}

	#ifdef DEBUG
		cout<<"m_Solution"<<endl;
		for(int i=0;i<m_Solution.size();i++){
			for(int j=0;j<m_Solution[i].size();j++){
				cout<<"("<<m_Solution[i][j].jobIndex<<","<<m_Solution[i][j].machine;
					cout<<","<<m_Solution[i][j].time<<") ";
			}
			cout<<endl;
		}
	#endif
}

void Ges::execute(){
	vector<vector<JobPair> > _solution=m_Solution;
	while(m_Iter<m_MaxIter){
		m_Solution=_solution;
		Graph graph(m_Solution,m_SettingTable);
		#ifdef DEBUG
			cout<<"[execute]"<<endl;
			graph.print();
			cout<<"-------- _solution -----------"<<endl;
			for(int i=0;i<_solution.size();i++){
				for(int j=0;j<_solution[i].size();j++){
					cout<<"("<<_solution[i][j].jobIndex<<","<<_solution[i][j].machine<<",";
					cout<<_solution[i][j].time<<") ";
				}
				cout<<endl;
			}
		#endif
		int L=graph.getMakespan()-1;

		//cout<<"L="<<L<<endl;
		Routine(_solution,L);
	}

	cout<<"result"<<endl;
	for(int i=0;i<m_Solution.size();i++){
		for(int j=0;j<m_Solution[i].size();j++){
			cout<<"("<<m_Solution[i][j].jobIndex<<","<<m_Solution[i][j].machine<<","<<m_Solution[i][j].time<<") ";
		}
		cout<<endl;
	}
	Graph g(m_Solution,m_SettingTable);
	cout<<"makespan="<<g.getMakespan()<<endl;
}

void Ges::Routine(vector<vector<JobPair> >& solution,int L){
	#ifdef DEBUG
		cout<<"Enter routine"<<endl;
	#endif
	vector<vector<JobPair> > _solution=solution;
	while(!m_EP.empty())
		m_EP.pop();

	vector<vector<JobPair> > I;
	Ejection(_solution,I,L);
	#ifdef DEBUG
		cout<<"1.I list"<<endl;

		for(int i=0;i<I.size();i++){
			for(int j=0;j<I[i].size();j++){
				cout<<"("<<I[i][j].jobIndex<<","<<I[i][j].machine<<",";
				cout<<I[i][j].time<<") ";
			}
			cout<<endl;
		}
	#endif

	vector<JobPair> candidate;

	// I listが殻だった場合の例外処理
	try{
		// Iから一つJobPairを選択する
		candidate=selectEP(I);
	}catch(exception& err){
		#ifdef DEBUG
			cout<<endl;
			cout<<"ERROR!"<<endl;
			cout<<endl;
			cout<<err.what()<<endl;
			cout<<"L="<<L<<endl;
			cout<<"--------------------"<<endl;

			Graph graph(_solution,m_SettingTable);
			graph.print();

			cout<<"Ejection()"<<endl;
			Ejection(_solution,I,L);
		#endif
		exit(0);
	}
	#ifdef DEBUG
		cout<<"candidate"<<endl;
		for(int i=0;i<candidate.size();i++){
			cout<<"("<<candidate[i].jobIndex<<","<<candidate[i].machine<<",";
			cout<<candidate[i].time<<") ";
		}
		cout<<endl;
	#endif


	// 選択されたJobPairを抜出EPに入れる
	for(int i=0;i<candidate.size();i++){
		JobPair jp=candidate[i];
		int machine=jp.machine;

		vector<JobPair>::iterator it=_solution[machine].begin();
		for(;it!=_solution[machine].end();it++){
			if((*it)==jp){
				_solution[machine].erase(it);
				break;
			}
		}
		m_EP.push(jp);
	}

	#ifdef DEBUG
		cout<<"after Eject"<<endl;
		for(int i=0;i<_solution.size();i++){
			for(int j=0;j<_solution[i].size();j++){
				cout<<"("<<_solution[i][j].jobIndex<<","<<_solution[i][j].machine<<",";
				cout<<_solution[i][j].time<<") ";
			}
			cout<<endl;
		}
		cout<<"graph"<<endl;
		Graph g(_solution,m_SettingTable);
		g.print();
	#endif

	Perturb(_solution,L);
	
	do{
		m_Iter++;
		//cout<<"Iter="<<m_Iter<<endl;

		// GES-1のために解とEPを保持
		vector<vector<JobPair> > beforeSolution=_solution;
		stack<JobPair> beforeEP=m_EP;

		// EPより一つ作業を取得
		JobPair tarJob=m_EP.top();
		#ifdef DEBUG
			cout<<"select EP Job"<<endl;
			cout<<"("<<tarJob.machine<<","<<tarJob.jobIndex<<","<<tarJob.time<<")"<<endl;
		#endif
		m_EP.pop();
		m_Penalty[tarJob.index]++;

		// 挿入可能な場所に挿入する
		vector<vector<vector<JobPair> > > solutionCandidates;
		int index;
		int count=0;
		int min=INT_MAX;
		for(int i=0;i<_solution[tarJob.machine].size()+1;i++){
			vector<vector<JobPair> > __solution=_solution;
			insertJob(__solution,tarJob,i);
			// feasible scheduleならば
			if(m_EP.empty()){
				LocalSearch(__solution);
			}
			// TODO
			// 挿入した際cycleが生じたらその候補は捨てる
			Graph g;
			try{
				g=Graph(__solution,m_SettingTable);
			}catch(runtime_error& e){
				#ifdef DEBUG
					cout<<"Error in Insert"<<endl;
					cout<<e.what()<<endl;
				#endif
				continue;
			}
			int makespan=g.getMakespan();
			if(makespan<min){
				min=makespan;
				index=count;
			}
			solutionCandidates.push_back(__solution);
			count++;
			#ifdef DEBUG
				cout<<"Insert["<<i<<"]"<<endl;
				for(int j=0;j<__solution.size();j++){
					for(int k=0;k<__solution[j].size();k++){
						cout<<"("<<__solution[j][k].jobIndex<<","<<__solution[j][k].machine<<",";
						cout<<__solution[j][k].time<<") ";
					}
					cout<<endl;
				}
			#endif
		}
		// makespanが最小のものを選択する
		// 候補が0ではない場合
		if(!solutionCandidates.empty())
			_solution=solutionCandidates[index];
		#ifdef DEBUG
			cout<<"min schedule"<<endl;
			for(int i=0;i<_solution.size();i++){
				for(int j=0;j<_solution[i].size();j++){
					cout<<"("<<_solution[i][j].jobIndex<<","<<_solution[i][j].machine<<","<<_solution[i][j].time<<")";
				}
				cout<<endl;
			}
		#endif

		Graph g(_solution,m_SettingTable);
		if(g.getMakespan()>L){
			vector<vector<JobPair> > I;
			Ejection(_solution,I,L);
			// Iが空だった場合
			if(I.empty()){
				// GES-1
				_solution=beforeSolution;
				m_EP=beforeEP;
			}else{
				// Iから一つJobPairを選択する
				vector<JobPair> candidate=selectEP(I);	
			}
			#ifdef DEBUG
				cout<<"2.I list"<<endl;

				for(int i=0;i<I.size();i++){
					for(int j=0;j<I[i].size();j++){
						cout<<"("<<I[i][j].jobIndex<<","<<I[i][j].machine<<",";
						cout<<I[i][j].time<<") ";
					}
					cout<<endl;
				}
			#endif

			
			#ifdef DEBUG
				cout<<"candidate"<<endl;
				for(int i=0;i<candidate.size();i++){
					cout<<"("<<candidate[i].jobIndex<<","<<candidate[i].machine<<",";
					cout<<candidate[i].time<<") ";
				}
				cout<<endl;
			#endif

			// 選択されたJobPairを抜出EPに入れる
			for(int i=0;i<candidate.size();i++){
				JobPair jp=candidate[i];
				int machine=jp.machine;

				vector<JobPair>::iterator it=_solution[machine].begin();
				for(;it!=_solution[machine].end();it++){
					if((*it)==jp){
						_solution[machine].erase(it);
						break;
					}
				}
				m_EP.push(jp);
			}


			#ifdef DEBUG
				cout<<"after Eject"<<endl;
				for(int i=0;i<_solution.size();i++){
					for(int j=0;j<_solution[i].size();j++){
						cout<<"("<<_solution[i][j].jobIndex<<","<<_solution[i][j].machine<<",";
						cout<<_solution[i][j].time<<") ";
					}
					cout<<endl;
				}
				cout<<"graph"<<endl;
				Graph g(_solution,m_SettingTable);
				g.print();
			#endif
		}
		Perturb(_solution,L);
		Graph gr(_solution,m_SettingTable);
		#ifdef DEBUG
			cout<<"[end of routine]"<<endl;
			gr.print();
			cout<<"-------- _solution --------------"<<endl;
			for(int i=0;i<_solution.size();i++){
				for(int j=0;j<_solution[i].size();j++){
					cout<<"("<<_solution[i][j].jobIndex<<","<<_solution[i][j].machine<<",";
					cout<<_solution[i][j].time<<") ";
				}
				cout<<endl;
			}
		#endif
		//cout<<"makespan="<<gr.getMakespan()<<endl;
	}while(!m_EP.empty() && m_Iter<m_MaxIter);
	#ifdef DEBUG
		cout<<"routine end"<<endl;
		cout<<"iter="<<m_Iter<<endl;
		cout<<"EP.size()="<<m_EP.size()<<endl;
	#endif
	solution=_solution;
}

vector<JobPair> Ges::selectEP(vector<vector<JobPair> >& I){
	int index=-1;
	int size=INT_MAX;

	if(I.empty()){
		throw exception();
	}

	#ifdef DEBUG
		cout<<"penalty"<<endl;
		for(int i=0;i<m_Penalty.size();i++){
			cout<<m_Penalty[i]<<",";
		}
		cout<<endl;
	#endif
	for(int i=0;i<I.size();i++){
		int tSize=0;
		for(int j=0;j<I[i].size();j++){
			tSize+=m_Penalty[I[i][j].index];
		}
		if(size>tSize){
			size=tSize;
			index=i;
		}
	}
	return I[index];
}

void Ges::Ejection(vector<vector<JobPair> > _solution,vector<vector<JobPair> >& a_I,int L){
	Graph graph(_solution,m_SettingTable);
	#ifdef DEBUG
		cout<<"Enter Ejection"<<endl;
		graph.print();
		cout<<endl;
	#endif
	deque<Node*> bottleneckNode;

	// ボトルネックノード抽出
	for(int i=0;i<graph.size();i++){
		if(graph[i]->m_R+graph[i]->m_Q-graph[i]->m_Jobpair->time>L){
			if(graph[i]->m_Jobpair->machine!=-1)
				bottleneckNode.push_back(graph[i]);
		}
	}
	#ifdef DEBUG
		cout<<"bottleneck Node"<<endl;
		for(int i=0;i<bottleneckNode.size();i++){
			cout<<bottleneckNode.at(i)->getIndex()<<" ";
		}
		cout<<endl;
		graph.print();
	#endif

	vector<JobPair> candidates;
	Ejection(graph,bottleneckNode,candidates,a_I,0,L);
}

void Ges::Ejection(Graph graph,deque<Node*> bottleneckNode,vector<JobPair> a_candidates,vector<vector<JobPair> >& a_I,int count,int L){
	if(count>=m_kMax || bottleneckNode.empty()){
		return;
	}
	// 辞書順に抜き出しす
	while(!bottleneckNode.empty()){
		Node* node=bottleneckNode.front();
		bottleneckNode.pop_front();
		int index=node->getIndex();

		Graph _graph(graph);
		_graph.removeNode(index);
		a_candidates.push_back(*_graph[index]->m_Jobpair);
		#ifdef DEBUG
			cout<<count<<"times"<<endl;
			cout<<"remove node["<<index<<"]"<<endl;
			_graph.print();
			cout<<"candidates list"<<endl;
			for(int i=0;i<a_candidates.size();i++){
				cout<<"("<<a_candidates[i].jobIndex<<","<<a_candidates[i].machine<<",";
				cout<<a_candidates[i].time<<") ";
			}
			cout<<endl;
		#endif

		if(_graph.getMakespan()<=L){
			a_I.push_back(a_candidates);
		}
		
		Ejection(_graph,bottleneckNode,a_candidates,a_I,count+1,L);
		a_candidates.pop_back();
	}
}

void Ges::Perturb(vector<vector<JobPair> >& solution,int L){
	#ifdef DEBUG
		cout<<"before Perturb"<<endl;
		for(int i=0;i<solution.size();i++){
			for(int j=0;j<solution[i].size();j++){
				cout<<"("<<solution[i][j].jobIndex<<","<<solution[i][j].machine<<",";
				cout<<solution[i][j].time<<") ";
			}
			cout<<endl;
		}

	#endif

	// 指定した回数だけランダムに解を遷移させる
	// ただし、遷移してL以下にならなかった場合は
	// 解を遷移させずにカウントする
	int cnt=0;
	while(m_IterRand>cnt){
		vector<vector<JobPair> > _solution=solution;
		int machineRand=Util::getRand(0,_solution.size()-1);
		if(_solution[machineRand].size()<2){
			continue;
		}
		int jobRand=Util::getRand(0,_solution[machineRand].size()-2);
		

		JobPair jp=_solution[machineRand][jobRand];
		_solution[machineRand][jobRand]=_solution[machineRand][jobRand+1];
		_solution[machineRand][jobRand+1]=jp;
		
		// TODO
		// 解を遷移した際、cycleが生じるとその解は捨てる
		Graph g;
		try{
			g=Graph(_solution,m_SettingTable);
			cnt++;
		}catch(runtime_error& e){
			#ifdef DEBUG
				cout<<"Error in Perturb"<<endl;
				cout<<e.what()<<endl;
			#endif
			continue;
		}
		if(g.getMakespan()<=L){
			solution=_solution;
		}

	}
	#ifdef DEBUG
		cout<<"after Perturb"<<endl;
		for(int i=0;i<solution.size();i++){
			for(int j=0;j<solution[i].size();j++){
				cout<<"("<<solution[i][j].jobIndex<<","<<solution[i][j].machine<<",";
				cout<<solution[i][j].time<<") ";
			}
			cout<<endl;
		}
	#endif
}

void Ges::insertJob(vector<vector<JobPair> >& solution,JobPair &jp,int index){
	solution[jp.machine].push_back(jp);

	for(int i=solution[jp.machine].size()-1;i>index;i--){
		JobPair tJob=solution[jp.machine][i];
		solution[jp.machine][i]=solution[jp.machine][i-1];
		solution[jp.machine][i-1]=tJob;
	}

}

void Ges::LocalSearch(vector<vector<JobPair> >& solution){
	deque<vector<vector<JobPair> > > tabuList;
	vector<vector<JobPair> > _solution=solution;

	Graph g(_solution,m_SettingTable);
	int notImprove=0;
	int prevMakespan=g.getMakespan();
	do{
		NeighbourGenerator ng(_solution,m_SettingTable);
		ng.makeNeighbour();
		int size=ng.getNeighbourSize();
		Graph g(_solution,m_SettingTable);
		int makespan=g.getMakespan();
		vector<vector<JobPair> > __solution;

		bool flag=false;
		for(int i=0;i<size;i++){
			__solution=ng.getNeighbour(i);
			if(tabuCheck(tabuList,__solution,_solution))
				continue;
			Graph _g(__solution,m_SettingTable);
			int _makespan=_g.getMakespan();
			if(_makespan>makespan)
				continue;
			_solution=__solution;
			addTabuList(tabuList,_solution);
			flag=true;
			break;
		}
		if(!flag){
			int minMakespan=INT_MAX;
			int index=-1;
			for(int i=0;i<size;i++){
				__solution=ng.getNeighbour(i);
				if(tabuCheck(tabuList,__solution,_solution))
					continue;
				Graph _g(__solution,m_SettingTable);
				int _makespan=_g.getMakespan();
				#ifdef DEBUG
					cout<<"i["<<i<<"]="<<_makespan<<endl;
					cout<<"make="<<_makespan<<endl;
					cout<<"INT_MAX="<<INT_MAX<<endl;
					cout<<"minMakespan="<<minMakespan<<endl;
					cout<<"index="<<index<<endl;
				#endif
				if(minMakespan>_makespan){
					minMakespan=_makespan;
					index=i;
				}
			}
			if(index!=-1){
				#ifdef DEBUG
					cout<<"ret index="<<index<<endl;
				#endif
				_solution=ng.getNeighbour(index);
				addTabuList(tabuList,_solution);
			}
		}
		g=Graph(_solution,m_SettingTable);
		int _makespan=g.getMakespan();
		if(prevMakespan==_makespan){
			notImprove++;
		}else{
			notImprove=0;
		}
		prevMakespan=_makespan;
		#ifdef DEBUG
			cout<<"notImprove:"<<notImprove<<endl;
		#endif
		solution=_solution;
	}while(notImprove<m_stagLS);
}

bool Ges::tabuCheck(deque<vector<vector<JobPair> > >& tabuList,vector<vector<JobPair> >& _solution,vector<vector<JobPair> >&solution){
	return false;
	int machine=-1;
	for(int i=0;i<_solution.sie();i++){
		for(int j=0;j<_solution[i].size();j++){
			if(solution[i][j].index!=_solution[i][j].index){
				machine=i;
				break;
			}
		}
		if(machine!=-1)
			break;
	}
	for(int i=0;i<tabuList.size();i++){
		bool flag=true;
		for(int j=0;j<_solution[machine].size();j++){
			if(tabuList[i][machine][j].index!=_solution[machine][j])
				flag=false;
		}
		if(flag)
			break;
	}
	return flag;
}

void Ges::addTabuList(deque<vector<vector<JobPair> > >& tabuList,vector<vector<JobPair> >& solution){
	tabuList.push_back(solution);
	if(tabuList.size()>m_maxT){
		tabuList.pop_front();
	}
}

Ges::~Ges(){

}