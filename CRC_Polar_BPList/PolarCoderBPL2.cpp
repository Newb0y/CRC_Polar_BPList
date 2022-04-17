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
	cout<<"译码器：BPL\t"<<"最大迭代次数："<<maxIterNum<<"\tListSize = "<<L<<"\t路径选择规则："<<pathSelectorType<<"\n";
	ofstream oResult(RESULT_FILE,ios::app);
	oResult<<"译码器：BPL\t"<<"最大迭代次数："<<maxIterNum<<"\tListSize = "<<L<<"\t路径选择规则："<<pathSelectorType<<"\n";
	oResult.close();
}

void PolarCoderBPL2::init()
{
	for(int l = 0; l < L; l++)
	{
		for (int i = 0; i < N; i++)  //初始化
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
void PolarCoderBPL2::RToL(int l) //从右到左一次迭代
{
	for (int lamda = n - 1; lamda >= 0; lamda--)
	{
		int nums1 = 1 << lamda;//分支的个数
		int nums2 = 1 << (n -1 - lamda);//两个关联节点的间距
		int nums3 = 1 << (n - lamda);//一个分支上节点的个数
		for(int beta = 0; beta < nums1; beta ++)
		{
			for(int dafai = 0; dafai < nums2; dafai ++)
			{
				int index1 = beta * nums3 + dafai;
				int index2 = index1 + nums2;
				R2L[l][lamda][index1] = box_plus(R2L[l][lamda + 1][index1], L2R[l][lamda][index2] + R2L[l][lamda + 1][index2]);//L[i][j]=f(L[i+1][j],R[i][j+Ni]+L[i+1][j+Ni]),其中Ni=2^(n-i),f(x,y)=(1+x*y)/x+y是盒式加法函数.
				R2L[l][lamda][index2] = box_plus(R2L[l][lamda + 1][index1], L2R[l][lamda][index1]) + R2L[l][lamda + 1][index2];//L[i][j+Ni]=f(R[i][j],L[i+1][j])+L[i+1][j+Ni]
				//防止溢出
				limiting(R2L[l][lamda][index1]);
				limiting(R2L[l][lamda][index2]);
			}
		}
	}
}
void PolarCoderBPL2::LToR(int l) //从左到右一次迭代
{
	for (int lamda = 0; lamda < n; lamda ++)
	{
		int nums1 = 1 << lamda;//分支的个数
		int nums2 = 1 << (n -1 - lamda);//两个关联节点的间距
		int nums3 = 1 << (n - lamda);//一个分支上节点的个数
		for(int beta = 0; beta < nums1; beta ++)
		{
			for(int dafai = 0; dafai < nums2; dafai ++)
			{
				int index1 = beta * nums3 + dafai;
				int index2 = index1 + nums2;
				L2R[l][lamda + 1][index1] = box_plus(L2R[l][lamda][index1], L2R[l][lamda][index2] + R2L[l][lamda + 1][index2]);
				L2R[l][lamda + 1][index2] = box_plus(R2L[l][lamda + 1][index1], L2R[l][lamda][index1]) + L2R[l][lamda][index2];//L[i][j+Ni]=f(R[i][j],L[i+1][j])+L[i+1][j+Ni]
				//防止溢出
				limiting(L2R[l][lamda + 1][index1]);
				limiting(L2R[l][lamda + 1][index1]);
			}
		}
	}
}
bool PolarCoderBPL2::check(int* u, int* x)//检查一下译码是否正确：xN=uN*GN，假设我们译出的信源序列为u^(即代码中的source),然后我们用译出的信源序列为u^去乘生成矩阵GN=GN=F的3次克罗内克积（不加Bn比特反转矩阵时）即u^*GN=x^(即代码中的tempcode),若x^=x,则说明译码正确，反之若x^!=x,则说明译码错误，errorcount=errorcount+1
{
	int* uTemp = createIntegerArray(N);
	copyIntegerArray(u, uTemp, N);
	int errorcount = 0;
	//再次编码
	for (int lamda = 0; lamda < n; lamda ++)
	{
		int nums1 = 1 << lamda;//分支的个数
		int nums2 = 1 << (n -1 - lamda);//两个关联节点的间距
		int nums3 = 1 << (n - lamda);//一个分支上节点的个数
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
	//记录错误比特的数量
	resultOnce.errorBitDistribution = new int[N];
	for (int i = 0; i < N; i++)
	{
		resultOnce.errorBitDistribution[i] = (Estimate[i] == constructionSequence[i]) ? 0 : 1;
	}

	return resultOnce;
}


