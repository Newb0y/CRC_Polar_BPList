#include "PolarCoderBPL2.h"
#include "MemoryManagementUtil.h"
#include"utility.h"
#include "FileName.h"
#include<fstream>
#include<iostream>
#include "DataProcessingUtil.h"
PolarCoderBPL2::PolarCoderBPL2(void)
{
	ifstream iBPL(BPL_FILE);
	iBPL>>maxIterNum;
	iBPL.close();
	R2L = createDoubleArray(L, n + 1, N);
	L2R = createDoubleArray(L, n + 1, N);
	threads = new thread[L];
}

PolarCoderBPL2::~PolarCoderBPL2(void)
{
	deleteDoubleArray(R2L, L, n + 1);
	deleteDoubleArray(L2R, L, n + 1);
	delete[] threads;
}

void PolarCoderBPL2::outputDetailedInfo()
{
	cout<<"��������BPL\t"<<"������������"<<maxIterNum<<"\tListSize = "<<L<<"\t·��ѡ�����"<<pathSelectorType<<"\n";
	ofstream oResult(RESULT_FILE,ios::app);
	oResult<<"��������BPL\t"<<"������������"<<maxIterNum<<"\tListSize = "<<L<<"\t·��ѡ�����"<<pathSelectorType<<"\n";
	oResult.close();
}

void PolarCoderBPL2::init()
{
	for(int l = 0; l < L; l++)
	{
		for (int i = 0; i < N; i++)  //��ʼ��
		{
			for (int lamda = 0; lamda <= n; lamda++)
			{
				R2L[l][lamda][i] = 0.0;
				L2R[l][lamda][i] = 0.0;
			}
			R2L[l][n][i] = LLRs[l][i];
			limiting(R2L[l][n][i]);
			if(frozenPositions[l][i] == 0)
			{
				L2R[l][0][i] = Inf;
			}
		}
		//L2R[l][0][7] = Inf;
		//L2R[l][0][N-1] = Inf;
	}
	
}

void PolarCoderBPL2::decoderCore(Communication* comm)
{
	init();
	for(int l = 0; l < L; l++)
	{
		 threads[l] = thread(&PolarCoderBPL2::decoderBP, this, l);
	}
	for(int l = 0; l < L; l++)
	{
		 threads[l].join();
	}
}


void PolarCoderBPL2::decoderBP(int path)
{
	validPath[path] = false;
	int* estimate = createIntegerArray(N);
	int* estimates = createIntegerArray(N);
	for (int it = 0; it < maxIterNum; it++)
	{
		RToL(path);
		LToR(path);
		
		for (int fai = 0; fai < N; fai++)
		{
			softInfoCodewords[path][fai] = R2L[path][n][fai] + L2R[path][n][fai];//codeword
			softInfoConstructionSequences[path][fai] = R2L[path][0][fai] + L2R[path][0][fai];//info
		}

		permutateEstimate(locationMapping[path], softInfoCodewords[path], N);
		permutateEstimate(locationMapping[path], softInfoConstructionSequences[path], N);

		for (int fai = 0; fai < N; fai++)
		{
			estimateCodewords[path][fai] = softInfoCodewords[path][fai] >= 0 ? 0 : 1;
			estimateConstructionSequences[path][fai] = softInfoConstructionSequences[path][fai] >= 0 ? 0 :1;
		}
		int tag = CRC_check(estimateConstructionSequences[path], KwithCRC, CRCLength);
		if (tag ==0)
		{
			validPath[path] = true;
			break;
		}
		/*if(check(estimateConstructionSequences[path], estimateCodewords[path]))
		{
			validPath[path] = true;
			break;
		}*/
	}	

/*	for (int fai = 0; fai < N; fai++)
	{
	cout << softInfoConstructionSequences[path][fai] << " ";
}cout << endl;*/

	deleteIntegerArray(estimate);
	deleteIntegerArray(estimates);
}
void PolarCoderBPL2::RToL(int l) //���ҵ���һ�ε���
{
	for (int lamda = n - 1; lamda >= 0; lamda--)
	{
		int nums1 = 1 << lamda;//��֧�ĸ���
		int nums2 = 1 << (n -1 - lamda);//���������ڵ�ļ��
		int nums3 = 1 << (n - lamda);//һ����֧�Ͻڵ�ĸ���
		for(int beta = 0; beta < nums1; beta ++)
		{
			for(int dafai = 0; dafai < nums2; dafai ++)
			{
				int index1 = beta * nums3 + dafai;
				int index2 = index1 + nums2;
				R2L[l][lamda][index1] = box_plus(R2L[l][lamda + 1][index1], L2R[l][lamda][index2] + R2L[l][lamda + 1][index2]);//L[i][j]=f(L[i+1][j],R[i][j+Ni]+L[i+1][j+Ni]),����Ni=2^(n-i),f(x,y)=(1+x*y)/x+y�Ǻ�ʽ�ӷ�����.
				R2L[l][lamda][index2] = box_plus(R2L[l][lamda + 1][index1], L2R[l][lamda][index1]) + R2L[l][lamda + 1][index2];//L[i][j+Ni]=f(R[i][j],L[i+1][j])+L[i+1][j+Ni]
				//��ֹ���
				limiting(R2L[l][lamda][index1]);
				limiting(R2L[l][lamda][index2]);
			}
		}
	}
}
void PolarCoderBPL2::LToR(int l) //������һ�ε���
{
	for (int lamda = 0; lamda < n; lamda ++)
	{
		int nums1 = 1 << lamda;//��֧�ĸ���
		int nums2 = 1 << (n -1 - lamda);//���������ڵ�ļ��
		int nums3 = 1 << (n - lamda);//һ����֧�Ͻڵ�ĸ���
		for(int beta = 0; beta < nums1; beta ++)
		{
			for(int dafai = 0; dafai < nums2; dafai ++)
			{
				int index1 = beta * nums3 + dafai;
				int index2 = index1 + nums2;
				L2R[l][lamda + 1][index1] = box_plus(L2R[l][lamda][index1], L2R[l][lamda][index2] + R2L[l][lamda + 1][index2]);
				L2R[l][lamda + 1][index2] = box_plus(R2L[l][lamda + 1][index1], L2R[l][lamda][index1]) + L2R[l][lamda][index2];//L[i][j+Ni]=f(R[i][j],L[i+1][j])+L[i+1][j+Ni]
				//��ֹ���
				limiting(L2R[l][lamda + 1][index1]);
				limiting(L2R[l][lamda + 1][index1]);
			}
		}
	}
}
bool PolarCoderBPL2::check(int* u, int* x)//���һ�������Ƿ���ȷ��xN=uN*GN�����������������Դ����Ϊu^(�������е�source),Ȼ���������������Դ����Ϊu^ȥ�����ɾ���GN=GN=F��3�ο����ڿ˻�������Bn���ط�ת����ʱ����u^*GN=x^(�������е�tempcode),��x^=x,��˵��������ȷ����֮��x^!=x,��˵���������errorcount=errorcount+1
{
	int* uTemp = createIntegerArray(N);
	copyIntegerArray(u, uTemp, N);
	int errorcount = 0;
	//�ٴα���
	for (int lamda = 0; lamda < n; lamda ++)
	{
		int nums1 = 1 << lamda;//��֧�ĸ���
		int nums2 = 1 << (n -1 - lamda);//���������ڵ�ļ��
		int nums3 = 1 << (n - lamda);//һ����֧�Ͻڵ�ĸ���
		for(int beta = 0; beta < nums1; beta ++)
		{
			for(int dafai = 0; dafai < nums2; dafai ++)
			{
				int index1 = beta * nums3 + dafai;
				int index2 = index1 + nums2;
				uTemp[index1] = uTemp[index1] ^ uTemp[index2];
				uTemp[index2] = uTemp[index2];
			}
		}
	}
	
	for (int i = 0; i<N; i++)
	{
		if (x[i] != uTemp[i])
		{
			errorcount++;
		}	
	}
	deleteIntegerArray(uTemp);
	return errorcount == 0;
}
ResultOnce PolarCoderBPL2::resultAnalysis(Communication* comm)
{
	ResultOnce resultOnce = PolarCoder::resultAnalysis(comm);
	int* constructionSequence = comm->constructionSequence;
	int* Estimate = comm->Estimate;
	//��¼������ص�����
	resultOnce.errorBitDistribution = new int[N];
	for (int i = 0; i < N; i++)
	{
		resultOnce.errorBitDistribution[i] = (Estimate[i] == constructionSequence[i]) ? 0 : 1;
	}

	return resultOnce;
}


