#include "RayleighChannel.h"


RayleighChannel::RayleighChannel(void)
{
}


RayleighChannel::~RayleighChannel(void)
{
}

void RayleighChannel::transmit(Communication* comm)
{
	int N = comm->N;
	double sigma = comm->sigma;
	double* signal = comm->signal;
	double* receive = comm->receive;
	double a = sqrt(0.5);
	for (int i = 0; i < N; i++)
	{
		double c = GaussRand(a);
		receive[i] = c * signal[i] + GaussRand(sigma);
	}
}
