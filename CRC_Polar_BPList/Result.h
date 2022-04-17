/**
@Date 2020/12/29
@Author 李秉豪
@Description 用于封装一个SNR下仿真的结果，如果有额外其他结果，建议继承该类重写outputOtherResultToConsole();outputOtherResultToFile();
*/
#pragma once
#include "ResultOnce.h"
#include "PolarCoder.h"
class Result
{
	public:
		double snr;
		int simulationNum;
		int errorBitNum;//错误比特数
		int errorFrameNum;
		PolarCoder* polar;
		Result();
		~Result(void);
		virtual void init(double snr1);
		virtual void addResultOnce(ResultOnce* resultOnce);
		void outputResultToConsole(int gap);
		void outputResultToFile();
		void outputBaseInfo();
	protected:
		virtual void outputOtherResultToConsole(){return;}
		virtual void outputOtherResultToFile(){return;}
	private:
		void outputToConsole();
		void outputToFile(ofstream& outfile);
};

