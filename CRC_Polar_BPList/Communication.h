/**
@Date 2020/12/25
@Author �����
@Description ��������ͨ��ϵͳ���漰�������в���
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
		int result; //һ��ͨ�Ž������ȷΪ0������Ϊ1

		bool redecoder = false;

		Communication();
		~Communication(void);
		int communication(double snr);
		void sourcer();
	private:
		void Transmit(double sigma);
};

