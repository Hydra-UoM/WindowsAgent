#include "stdafx.h"
#ifndef MYTIMER_H_
#define MYTIMER_H_

#include <iostream>
#include <conio.h>
#include <time.h>

class MyTimer {
public:
	MyTimer();
	void           start();
	void           stop();
	void           reset();
	bool           isRunning();
	unsigned long  getTime();
	bool           isOver(unsigned long seconds);
	virtual		   ~MyTimer();
private:
	bool           resetted;
	bool           running;
	unsigned long  beg;
	unsigned long  end;
};

#endif /* MYTIMER_H_ */