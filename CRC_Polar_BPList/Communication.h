/**
@Date 2020/12/25
@Author 李秉豪
@Description 包含整个通信系统所涉及到的所有参数
*/

#pragma once
#include <vector>
#include "RateAdapter.h"
#include "IW.h"
using namespace std;

class Communication
{
	public:
		int N;
		int K;
		int* source;
		int* constructionSequence;
		int* codeword;
		double* signal;
		double* receive;
		double* LLR;
		double sigma;
		int* Estimate;
		int result; //一次通信结果，正确为0，错误为1

		bool redecoder = false;

		Communication();
		~Communication(void);
		int communication(double snr);
		void sourcer();
	private:
		void Transmit(double sigma);
};

