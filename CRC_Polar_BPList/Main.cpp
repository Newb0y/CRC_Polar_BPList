#include<cstdlib>
#include "Simulation.h"
#include "PolarCoderBPL.h"
#include "PolarCoderBPL2.h"
#include "ResultBPL.h"
#include "RayleighChannel.h"
using namespace std;

void main()
{   
	Simulation sim;
	Sourcer sourcer1(2);
	PolarCoderBPL2 polarCoder1;
	Modem modem1(2);
	Communication comm1;
	//Result result1;
	ResultBPL result1(polarCoder1.N);
	Channel channel1;
	//RayleighChannel channel1;
	sim.polarCoder = &polarCoder1;
	sim.modem = &modem1;
	sim.comm = &comm1;
	sim.result = &result1;
	sim.sourcer = &sourcer1;
	sim.channel = &channel1;
	sim.simulation();
	system("PAUSE");
}