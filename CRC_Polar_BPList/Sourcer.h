#pragma once
#include "Communication.h"
class Sourcer
{
	public:
		int type;//��Դ����
		Sourcer(int type1);
		~Sourcer(void);
		void sourcer(Communication* comm);
};

