#ifndef _JOBPAIR_H_
#define _JOBPAIR_H_

class JobPair{
public:
	JobPair(){
		machine=-1;
		time=-1;	// 加工時刻
		endTime=-1;	// 加工終了時刻
		jobIndex=-1;
		checked=false;
		prev=-1;
		next=-1;
	}
	int machine;
	int prev;
	int next;
	int time;
	int endTime;
	int jobIndex;
	bool isCheck(){return checked;}
	void check(){checked=true;}
private:
	bool checked;
};

#endif