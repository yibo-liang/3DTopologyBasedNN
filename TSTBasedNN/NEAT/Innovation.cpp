
#include <map>
#include "Innovation.h"

Innovation *Innovation::s_instance = 0;
int Innovation::getInnovation(int pool_id)
{
	int result;
	if (inno_map.count(pool_id) > 0) {
		result = inno_map[pool_id];
		inno_map[pool_id] = inno_map[pool_id] + 1;
		return result;
	}
	else {
		inno_map[pool_id] = 0;
		return 0;
	}
}

int Innovation::resetInnovation(int pool_id)
{
	inno_map[pool_id] = 0;
	return 0;
}
