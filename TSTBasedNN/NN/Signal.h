#pragma once

#ifndef SIGNAL
#define SIGNAL

class Signal
{
public:
	int id;
	double strength;
	double age;

	Signal(const Signal& obj);

	Signal();
	~Signal();

private:

};


#endif