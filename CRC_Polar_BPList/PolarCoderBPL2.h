#pragma once
#include "PolarCoderPFG.h"
#include <thread>
class PolarCoderBPL2 : public PolarCoderPFG
{
	public:
		void decoderCore(Communication* comm);
		void outputDetailedInfo();
		PolarCoderBPL2(void);
		~PolarCoderBPL2(void);
		void decoderBP(int path);
		ResultOnce resultAnalysis(Communication* comm);
	private:
		thread* threads;
		int maxIterNum;
		double*** R2L;
		double*** L2R;
		void init();
		void RToL(int l);
		void LToR(int l);
		bool check(int* u, int* x);
};

