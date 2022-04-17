#include "Sourcer.h"
#include "random.h"

Sourcer::Sourcer(int type1)
{
	type = type1;
}


Sourcer::~Sourcer(void)
{
}

void Sourcer::sourcer(Communication* comm)
{
	int K = comm->K;
	int* source = comm->source;
	if(type == 0)
	{
		for (int i = 0; i < K; i++)
		{
			source[i] = 0;
		}
	}
	else if(type == 1)
	{
		for (int i = 0; i < K; i++)
		{
			source[i] = 1;
		}
	}
	else
	{
		for (int i = 0; i < K; i++)
		{
			source[i] = Random01();;
		}
	}
}
