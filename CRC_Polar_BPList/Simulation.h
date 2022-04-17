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
		Simulation(); //���캯��
		~Simulation(void); //����������������һ������ĳ�Ա������������ÿ��ɾ���������Ķ���ʱִ�У����������������򣨱���ر��ļ����ͷ��ڴ�ȣ�ǰ�ͷ���Դ��
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

