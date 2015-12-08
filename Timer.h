#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer{
public:
	Timer();
	void start();
	void lap();
	double getSub();
private:
	LARGE_INTEGER freq;
	LARGE_INTEGER prev;
	double sub;
};

#endif