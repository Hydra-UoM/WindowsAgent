#include "MyTimer.h"

MyTimer::~MyTimer() {
	// TODO Auto-generated destructor stub
}

MyTimer::MyTimer() {
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}


void MyTimer::start() {
	if (!running) {
		if (resetted)
			beg = (unsigned long)clock();
		else
			beg -= end - (unsigned long)clock();
		running = true;
		resetted = false;
	}
}


void MyTimer::stop() {
	if (running) {
		end = (unsigned long)clock();
		running = false;
	}
}


void MyTimer::reset() {
	bool wereRunning = running;
	if (wereRunning)
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	if (wereRunning)
		start();
}


bool MyTimer::isRunning() {
	return running;
}


unsigned long MyTimer::getTime() {
	if (running)
		return ((unsigned long)clock() - beg);
	else
		return end - beg;
}


bool MyTimer::isOver(unsigned long seconds) {
	return seconds >= getTime();
}