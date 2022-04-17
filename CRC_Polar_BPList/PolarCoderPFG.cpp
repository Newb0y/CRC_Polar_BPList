#include "PolarCoderPFG.h"
#include "FileName.h"
#include<fstream>
#include "MemoryManagementUtil.h"
#include"utility.h"
#include "DataProcessingUtil.h"
#include<iostream>
#include< iomanip >
PolarCoderPFG::PolarCoderPFG(void)
{
	ifstream iConfig(PFG_CONFIG_FILE);
	iConfig>>L;
	iConfig>>pathSelectorType;
	iConfig>>FGType;
	iConfig.close();
	metricPAI = (Metric*)createVoidArray(sizeof(Metric), L);
	ReadMetricPAI();
	locationMapping = createIntegerArray(L, N);
	if(FGType==0)
	{
		setLocationMapping();
	}
	else if(FGType==1)
	{
		setLocationMapping1();
	}
	LLRs = createDoubleArray(L, N);
	frozenPositions = createIntegerArray(L, N);
	validPath = new bool[N];
	estimateCodewords = createIntegerArray(L, N);
	estimateConstructionSequences = createIntegerArray(L, N);
	softInfoCodewords = createDoubleArray(L, N);
	softInfoConstructionSequences = createDoubleArray(L, N);
}


PolarCoderPFG::~PolarCoderPFG(void)
{
	deleteIntegerArray(locationMapping, L);
	deleteVoidArray(metricPAI);
	deleteDoubleArray(LLRs, L);
	deleteIntegerArray(frozenPositions, L);
	delete [] validPath;
	deleteIntegerArray(estimateCodewords, L);
	deleteIntegerArray(estimateConstructionSequences, L);
	deleteDoubleArray(softInfoCodewords, L);
	deleteDoubleArray(softInfoConstructionSequences, L);
}

void PolarCoderPFG::ReadMetricPAI()
{
	int size=L;
	ifstream in(MetricPAI_FILE);
	int i=0;
	while(i<size){
		int *PAI=new int[n];
		for(int j=0;j<n;j++)	in>>PAI[j];
		metricPAI[i].PAI=PAI;
		in>>metricPAI[i].metric;
		i++;
	}
	in.close();

}

void PolarCoderPFG::setLocationMapping()
{
	int* bin1 = new int[n]; //信息位序号排列前的二进制表示
	int* bin2 = new int[n]; //信息位序号排列后的二进制表示
	for(int l  = 0; l < L; l ++)//可以自己任意设置（采用随机）
	{
		int* permutation = metricPAI[l].PAI;
		for (int i = 0; i < N; i ++)
		{
			for(int j = 0; j < n; j++)
			{
				bin1[j] = (i >> j) & 1;
			}
			for(int j = 0; j < n; j++)
			{
				bin2[j] = bin1[permutation[j]];
			}
			int index_perm = 0; //信息位序号排列后的十进制表示
			for(int j = 0; j < n; j++)
			{
				index_perm += bin2[j] << j;
			}
			locationMapping[l][i] = index_perm;
		}
	}
	/*for (int l = 0; l < L; l++)
	{
		for (int i = 0; i < N; i++)
			cout << locationMapping[l][i] << " ";
		cout << endl;
	}
	system("pause");*/
}

void PolarCoderPFG::setLocationMapping1()//从文件中读
{
	for (int i = 0; i < N; i ++)
	{
		locationMapping[0][i] = i;
	}
	ifstream in(PAI_FILE);
	double metric = 0;
	for(int l  = 1; l < L; l ++)//可以自己任意设置（采用随机）
	{
		for (int i = 0; i < N; i ++)
		{
			in>>locationMapping[l][i];
		}
		in>>metric;
	}
	in.close();
}

//排列对数似然比
void PolarCoderPFG::permutateLLRs(Communication * comm)
{
	double* LLR = comm->LLR;
	for(int l = 0; l < L; l ++)
	{
		for (int i = 0; i < N; i++)
		{
			int permIndex = locationMapping[l][i];
			LLRs[l][permIndex] = LLR[i];
			limiting(LLRs[l][permIndex]);
		}
	}
}

void PolarCoderPFG::permutateFrozenPositions()//排列因子图上冻结位不变，但是因子图左侧的序列索引变了
{
	for(int l = 0; l < L; l ++)
	{
		for (int i = 0; i < N; i++)
		{
			int permIndex = locationMapping[l][i];
			frozenPositions[l][permIndex] = mark_frozenPosition[i];
		}
	}

	/*cout << "mark_forzenPosition:" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << mark_frozenPosition[i];
	}cout << endl;*/
	//cout << "forzenPosition:" << endl;
	/*for (int l = 0; l < L; l++)
	{
		for (int i = 0; i < N; i++)
		{
			cout << locationMapping[l][i]<<" ";
		}cout << endl;
		for (int i = 0; i < N; i++)
		{
			cout << frozenPositions[l][i] ;
		}cout << endl;
	}
	system("pause");*/
}

void PolarCoderPFG::permutateEstimate(int* pai, double* target, int len)
{
	vector<double> temp(len);
	for(int i = 0; i < len; i ++)
	{
		temp[i] = target[pai[i]];
	}
	for(int i = 0; i < len; i ++)
	{
		target[i] = temp[i];
	}
	vector<double>().swap(temp);
}

void PolarCoderPFG::decoder(Communication * comm)
{
	permutateLLRs(comm);
	permutateFrozenPositions();
	decoderCore(comm);
	//路径选择并输出
	/*
		0：精灵辅助
		1：最小距离
		2：大数逻辑
		3：因子图加权
		4：比特位加权
	*/
	switch (pathSelectorType)
    {
		case 0:	pathSelectorOfGenius(comm); break;
		case 1: pathSelectorOfMinDistance(comm); break;
		case 2:	pathSelectorOfMajorityLogic(comm); break;
		case 3: pathSelectorOfFactorGraphWeighted(comm); break;
		case 4:	pathSelectorOfBitWeighted(comm); break;
    }

	/*int tag = 0;
	for (int l = 0; l < L; l++)
	{
		if (validPath[l] == 0)
			tag++;
	}
	if (tag)
	{
		int i = 1;
		ofstream errorLLR(ERROR, ios::app);
		errorLLR << i;
		errorLLR.close();
	}
	cout << "*************************************" << endl;
	for (int l = 0; l < L; l++)
		cout << setw(12) << validPath[l];
	cout << endl;
	for (int i = 0; i < K; i++)
	{
		double LLR_sum = 0;
		double valid_LLR_sum = 0;
		cout << setw(5) << Iw_N[N - i - 1].index;
		for (int l = 0; l < L; l++)
		{
			if(validPath[l]) valid_LLR_sum+= softInfoConstructionSequences[l][Iw_N[N - i - 1].index];
			LLR_sum += softInfoConstructionSequences[l][Iw_N[N - i - 1].index];
			cout << setw(12) << softInfoConstructionSequences[l][Iw_N[N - i - 1].index];
		}	
		cout << setw(12) << "sum_LLR:" << LLR_sum;
		cout << setw(18) << "valid_LLR_sum:" << valid_LLR_sum;
		cout << endl;
	}
	for (int i = 0; i < K; i++)
	{
		cout << comm->constructionSequence[Iw_N[N - i - 1].index];
	}cout << endl;
	for (int i = 0; i < K; i++)
	{
		cout << comm->Estimate[Iw_N[N - i - 1].index];
	}cout << endl;
	int tag = 0;
	int *error = new int[K];
	for (int i = 0; i < N; i++)
	{
		if (comm->Estimate[i] != comm->constructionSequence[i])
		{
			error[tag] = i;
			tag++;
		}
	}cout << endl;
	cout << "tag=" << tag << endl;
	for (int i = 0; i < tag; i++)
	{
		double errorLLR_sum = 0;
		cout << setw(5) << error[i];
		cout << setw(2) << comm->Estimate[error[i]];
		for (int l = 0; l < L; l++)
		{
			errorLLR_sum += softInfoConstructionSequences[l][error[i]];
			cout << setw(15) << softInfoConstructionSequences[l][error[i]];
		}
		cout << setw(15) <<"sum_errorLLR:" << errorLLR_sum;
		cout << endl;
	}
	delete[] error;
	if (tag) system("PAUSE");

	/*double min_errorLLR = 0;
	for (int i = 0; i < N; i++)
	{
		for(int l=0;l<L;l++)
			if (min_errorLLR > softInfoConstructionSequences[l][i])
			{
				min_errorLLR = softInfoConstructionSequences[l][i];
			}
	}
	ofstream errorLLR(errorllr, ios::app);
	errorLLR << min_errorLLR << " ";
	errorLLR.close();*/
}

void PolarCoderPFG::pathSelectorOfMinDistance(Communication * comm)
{
	double* receive = comm->receive;
	double* pm = new double[L];
	int* estimate = comm->Estimate;

	for (int l = 0; l < L; l++)
	{
		pm[l] = 0;
		for (int i = 0; i < N; i++)
		{
			double temp = (1 - 2 * estimateCodewords[l][i]) - receive[i];
			pm[l] += temp * temp;//计算平方欧式距离 
		}
	}
	double min = (numeric_limits<double>::max)();
	int resultPath = 0;
	for (int l = 0; l < L; l++)
	{
		if (validPath[l] && pm[l] < min)
		{
			min = pm[l];
			resultPath = l;
		}
	}
	for (int i = 0; i < N; i++)
	{
		estimate[i] = estimateConstructionSequences[resultPath][i];
	}

	delete[] pm;
}
void PolarCoderPFG::pathSelectorOfMajorityLogic(Communication * comm)
{
	int* estimate = comm->Estimate;

	/*for (int i = 0; i < N; i++)
	{
		int count0 = 0;
		int count1 = 0;
		int count_Vailidpath = 0;
		for (int l = 0; l < L; l++)
		{
			if (validPath[l])
			{
				count0 += estimateConstructionSequences[l][i] == 0 ? 1 : 0;
				count1 += estimateConstructionSequences[l][i] == 1 ? 1 : 0;
			}
		}
		estimate[i] = count0 > count1 ? 0 : 1;
	}*/

	//improved
	for (int i = 0; i < N; i ++)
	{
		int count0 = 0;
		int count1 = 0;
		int count_Vailidpath = 0;
		for (int l = 0; l < L; l++)
		{
			count_Vailidpath += validPath[l];
		}
		if(!count_Vailidpath)
			for (int l = 0; l < L; l++)
			{
				count0 += estimateConstructionSequences[l][i] == 0 ? 1 : 0;
				count1 += estimateConstructionSequences[l][i] == 1 ? 1 : 0;
			}
		else 
		{
			for (int l = 0; l < L; l++)
			{

				if (validPath[l])
				{
					count0 += estimateConstructionSequences[l][i] == 0 ? 1 : 0;
					count1 += estimateConstructionSequences[l][i] == 1 ? 1 : 0;
				}
			}
		}
		estimate[i] = count0 > count1 ? 0 : 1;
	}
	for (int i = 0; i < (N - K); i++)
	{
		estimate[Iw_N[i].index] = 0;
	}
}
void PolarCoderPFG::pathSelectorOfGenius(Communication * comm)
{
	double* receive = comm->receive;
	int* estimate = comm->Estimate;
	int* codeword = comm->codeword;
	int resultPath = 0;
	for (int l = 0; l < L; l ++)
	{
		if (validPath[l])
		{
			int errorBitNum = 0;
			for (int i = 0; i < N; i ++)
			{
				errorBitNum += (codeword[i] == estimateCodewords[l][i]) ? 0 : 1;
			}
			if(errorBitNum == 0)
			{
				resultPath = l;
				break;
			}	
		}
	}
	for (int i = 0; i < N; i ++)
	{
		estimate[i] = estimateConstructionSequences[resultPath][i];
	}
}
void PolarCoderPFG::pathSelectorOfFactorGraphWeighted(Communication * comm)
{
	int* estimate = comm->Estimate;
	//计算权重
	double* weighted = createDoubleArray(L);
	for(int l = 0; l < L; l++)
	{
		//weighted[l] = 1.0/metricPAI[l].metric;
		//weighted[l] = 1- (metricPAI[l].metric/comm->K);
		weighted[l] = 1.0;//权重相同
	}
	normalized(weighted, L);

	for (int i = 0; i < N; i ++)
	{
		double count = 0;
		for (int l = 0; l < L; l ++)
		{
			if (validPath[l])
			{
				count += weighted[l] * softInfoConstructionSequences[l][i];
			}
		}
		estimate[i] = count >= 0 ? 0 : 1;
	}

	deleteDoubleArray(weighted);
}
void PolarCoderPFG::pathSelectorOfBitWeighted(Communication * comm)
{
	int* estimate = comm->Estimate;
	//计算权重
	double** weighted = createDoubleArray(N, L);
	double* metric = createDoubleArray(N);//第i个位置存放索引为i的可靠度
	for (int i = 0; i < N; i ++)
	{
		metric[Iw_N[i].index] = Iw_N[i].metric;
	}
	double sum = 0;
	for (int i = 0; i < N; i ++)
	{
		for(int l = 0; l < L; l++)
		{
			int permIndex = locationMapping[l][i];
			//weighted[i][l] = -log(metric[permIndex]);
			weighted[i][l] = 1- metric[permIndex];
		}
		normalized(weighted[i], L);
	}
	
	for (int i = 0; i < N; i ++)
	{
		double count = 0;
		for (int l = 0; l < L; l ++)
		{
			if (validPath[l])
			{
				count += weighted[i][l] * softInfoConstructionSequences[l][i];
			}
		}
		estimate[i] = count >= 0 ? 0 : 1;
	}

	deleteDoubleArray(weighted, N);
	deleteDoubleArray(metric);
}