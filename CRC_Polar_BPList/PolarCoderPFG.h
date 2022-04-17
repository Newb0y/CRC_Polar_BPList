/**
@Date 2020/12/25
@Author 李秉豪
@Description 所有涉及排列因子图的基类
*/
#pragma once
#include "PolarCoder.h"
class PolarCoderPFG : public PolarCoder
{
	public:
		int L;
		int pathSelectorType;
		int FGType;
		double** LLRs;
		int** frozenPositions;
		bool* validPath;//有效路径标记
		Metric* metricPAI;
		int** locationMapping;
		int** estimateCodewords;
		int** estimateConstructionSequences;
		double** softInfoCodewords;
		double** softInfoConstructionSequences;
		void ReadMetricPAI();
		void permutateEstimate(int* pai, double* target, int len);
		PolarCoderPFG(void);
		~PolarCoderPFG(void);
		void decoder(Communication * comm);
		virtual void decoderCore(Communication* comm){return;}
		
	private:
		void setLocationMapping();
		void setLocationMapping1();
		void permutateLLRs(Communication * comm);
		void permutateFrozenPositions();
		void pathSelectorOfMinDistance(Communication * comm);
		void pathSelectorOfMajorityLogic(Communication * comm);
		void pathSelectorOfGenius(Communication * comm);
		void pathSelectorOfFactorGraphWeighted(Communication * comm);
		void pathSelectorOfBitWeighted(Communication * comm);
};

