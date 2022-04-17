#pragma once
#include "Result.h"
class ResultBPL : public Result
{
	public:
		int N;
		int* errorBitDistribution;
		void init(double snr1);
		void addResultOnce(ResultOnce* resultOnce);
		ResultBPL(int N1);
		~ResultBPL(void);
	protected:
		void outputOtherResultToConsole();
		void outputOtherResultToFile();
};

