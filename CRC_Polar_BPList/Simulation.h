#pragma once
#include "Result.h"
#include "PolarCoder.h"
#include "Modem.h"
#include "Communication.h"
#include "Sourcer.h"
#include "Channel.h"
class Simulation
{
	public:
		double startSNR;
		double endSNR;
		double stepSNR;
		int SNRType;//0:Eb_N0; 1:Es_N0
		int minSimulationNum;
		int maxSimulationNum;
		int maxErrorFrameNum;
		int gapSimulationNum;
		Simulation(); //构造函数
		~Simulation(void); //类的析构函数是类的一种特殊的成员函数，它会在每次删除所创建的对象时执行，有助于在跳出程序（比如关闭文件、释放内存等）前释放资源。
		void simulation();
		ResultOnce communication(double snr);
		Sourcer* sourcer;
		PolarCoder* polarCoder;
		Modem* modem;
		Communication* comm;
		Channel* channel;
		Result* result;
	private:
		double computeSigma(double snr, double R);
};

