/**
@Date 2020/12/25
@Author 李秉豪
@Description 所有Polar编译码算法的基类，以后有新的算法，直接继承该类，然后实现encoder和decoder即可
*/

#pragma once
#include <thread>
#include "Communication.h"
#include "ResultOnce.h"
#include "IW.h"
class PolarCoder
{
	public:
		int K;
		int N;
		int CRCLength;
		int KwithCRC;
		int n;
		int M;
		double R;
		IW* Iw_N;
		int* mark_frozenPosition;//冻结位标志:0为冻结位，1为信息位
		
		int** F;
		void constructor(Communication * comm);
		virtual void encoder(Communication * comm);
		virtual void decoder(Communication * comm) {return;}
		int CounteErrors(int *estimates, int *rawdata, int length);
		void CRC_encoder(int K, int* input, int* output);
		int CRC_check(int* u, int KwithCRC, int crcLength);
		virtual ResultOnce resultAnalysis(Communication* comm);
		void outputBaseInfo();
		virtual void outputDetailedInfo(){return;}
		PolarCoder();
		~PolarCoder(void);
	private:
		void readConsturction();
		void IW_NToMark_FrozenPosition();
		void Generate_F();
		int* sourceWithCRC;
};

