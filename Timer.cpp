#include "Timer.h"

Timer::Timer(){
	QueryPerformanceFrequency(&freq);
}

void Timer::start(){
	QueryPerformanceCounter(&prev);
}

void Timer::lap(){
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	sub=(double)(now.QuadPart - prev.QuadPart) / freq.QuadPart;
	prev=now;
}

double Timer::getSub(){
	return sub;
}