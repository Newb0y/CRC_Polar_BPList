#include "Simulation.h"
#include "FileName.h"
#include<fstream>
#include "random.h"
#include "utility.h"
using namespace std;

Simulation::Simulation()
{
	ifstream iSNR(SNR_FILE);
	iSNR>>startSNR;
	iSNR>>endSNR;
	iSNR>>stepSNR;
	iSNR>>SNRType;
	iSNR.close();

	ifstream iConfig(CONFIG_FILE);
	iConfig>>minSimulationNum;
	iConfig>>maxSimulationNum;
	iConfig>>maxErrorFrameNum;
	iConfig>>gapSimulationNum;
	iConfig.close();
}

Simulation::~Simulation()
{

}

/**
尽量不要改动此处代码，如需重写，建议继承Result和ResultOnce并重写相关的虚函数
*/
void Simulation::simulation()
{
	result->polar = polarCoder;
	result->outputBaseInfo();
	for(double snr = startSNR; snr <= endSNR; snr += stepSNR)
	{
		result->init(snr);
		int errorFrameNum = 0;
		int simulationNum = 1;
		while(simulationNum <= maxSimulationNum)
		{
			result->addResultOnce(&communication(snr));
			errorFrameNum = result->errorFrameNum;
			result->simulationNum = simulationNum;
			if(errorFrameNum >= maxErrorFrameNum && simulationNum >= minSimulationNum)
			{
				break;
			}			
			result->outputResultToConsole(gapSimulationNum);//输出仿真结果
			simulationNum ++;
		}
		result->outputResultToFile();
	}
}

ResultOnce Simulation::communication(double snr)
{
	comm->sigma = computeSigma(snr, polarCoder->R);
	
	sourcer->sourcer(comm);
	polarCoder->constructor(comm);
	polarCoder->encoder(comm);
	modem->modulator(comm);
	channel->transmit(comm);
	modem->demodulator(comm);
	polarCoder->decoder(comm);

	return polarCoder->resultAnalysis(comm);
}

double  Simulation::computeSigma(double snr, double R)
{
	double Eb_N0 = pow(10, 0.1  *snr);
	double Es_N0 = SNRType == 0 ? Eb_N0 * R : Eb_N0;
	double sigma= sqrt(modem->GetPowerFactor() * 0.5 / Es_N0);
	return sigma;
}
