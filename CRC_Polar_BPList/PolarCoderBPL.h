#pragma once
#include "PolarCoder.h"
class PolarCoderBPL : public PolarCoder
{
	public:
		PolarCoderBPL(void);
		~PolarCoderBPL(void);
		void decoder(Communication * comm);
	private:
		Metric* metricPAI;
		int L;
		int max_iter;
		double*** LL;
		double*** RR;
		int** state_scale_new;
		bool* IsthisPathValid;
		int** res;
		int** res_C;
		void ReadMetricPAI();
		void vTOc(int path);
		void cTOv(int path);
		void decoderCore(Communication* comm, int path);
		void Decoding(int *estimates,int *estimate,int path);
};

