#pragma once
#include "Communication.h"
class Modem
{
private:
    int m_Ary;

	const int *m_pSignalMapper;
    double *m_pSignal;

public:
    Modem(int mary);
    ~Modem();

    double GetPowerFactor();
    double GetSignal(int label);
	void modulator(Communication* comm);
	void demodulator(Communication* comm);

private:
    void SetSignalValue();
};

