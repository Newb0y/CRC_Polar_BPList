/**
@Date 2020/12/29
@Author 李秉豪
@Description 用于封装一次通信仿真的结果
*/
#pragma once
class ResultOnce
{
	public:
		int errorBitNum;//错误比特数
		int errorFrameNum;
		int* errorBitDistribution;
		ResultOnce(void);
		~ResultOnce(void);
};

