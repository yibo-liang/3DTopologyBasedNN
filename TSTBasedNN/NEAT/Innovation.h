
#pragma once

#ifndef INNOGLOBAL
#define INNOGLOBAL

#include <map>


class Innovation
{
	std::map<int, int> Innovation::inno_map;
	static Innovation *s_instance;
	Innovation()
	{
		
	}
public:

	int getInnovation(int pool_id);

	int resetInnovation(int pool_id);
	static Innovation *instance()
	{
		if (!s_instance)
			s_instance = new Innovation;
		return s_instance;
	}
};
#endif // !INNOGLOBAL
