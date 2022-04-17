#pragma once
#include "Communication.h"
class Sourcer
{
	public:
		int type;//信源类型
		Sourcer(int type1);
		~Sourcer(void);
		void sourcer(Communication* comm);
};

