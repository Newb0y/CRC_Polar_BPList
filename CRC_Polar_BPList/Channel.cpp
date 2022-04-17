#include "Channel.h"


Channel::Channel(void)
{
}


Channel::~Channel(void)
{
}

void Channel::transmit(Communication* comm)
{
	int N = comm->N;
	double sigma = comm->sigma;
	double* signal = comm->signal;
	double* receive = comm->receive;
	for (int i = 0; i < N; i++)
	{
		receive[i] = signal[i] + GaussRand(sigma);
		//receive[i] = signal[i];
	}
}
