/**
@Date 2020/12/29
@Author �����
@Description ���ڷ�װһ��ͨ�ŷ���Ľ��
*/
#pragma once
class ResultOnce
{
	public:
		int errorBitNum;//���������
		int errorFrameNum;
		int* errorBitDistribution;
		ResultOnce(void);
		~ResultOnce(void);
};

