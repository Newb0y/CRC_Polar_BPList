#include "Modem.h"
#include<cstdlib>
//#include<iostream>


Modem::Modem(int mary)
{
    m_Ary = mary;
   
    m_pSignal = new double[m_Ary];
	m_pSignalMapper = NULL;
    SetSignalValue(); 
}

Modem::~Modem()
{
 
    delete [] m_pSignal;
}

void Modem::SetSignalValue()
{
	if (m_Ary == 2) 
	{
		m_pSignal[0] = 1;
		m_pSignal[1] = -1;
	}
}

double Modem::GetSignal(int label)
{
    return m_pSignal[label];
}

double Modem::GetPowerFactor()
{ 
    double sum = 0.0;

    for (int i = 0; i < m_Ary; i++) 
        sum +=m_pSignal[i]*m_pSignal[i];
    return (sum/m_Ary);
}

void Modem::modulator(Communication* comm)
{
	int N = comm->N;
	int* codeword = comm->codeword;
	double* signal = comm->signal;
	for(int i=0;i<N;i++)
	{
		signal[i] = GetSignal(codeword[i]);
	}
}


void Modem::demodulator(Communication* comm)
{
	double sigma=comm->sigma;
	double n0 = sigma * sigma;
	int N = comm->N;
	double* receive = comm->receive;
	double* LLR = comm->LLR;
	//cout << "LLR:";
	for(int i = 0; i < N; i ++)
	{
		LLR[i] = (2 * receive[i]) / n0;
		//cout<<LLR[i]<<" ";
	}
	//cout << endl;
}


