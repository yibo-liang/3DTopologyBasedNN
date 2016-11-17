
#include "Signal.h"

Signal::Signal(const Signal & obj)
{
	this->age = obj.age;
	this->id = obj.id;
	this->strength = obj.strength;
}

Signal::Signal()
{
}

Signal::~Signal()
{
}
