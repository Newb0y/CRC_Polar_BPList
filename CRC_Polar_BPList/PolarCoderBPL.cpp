#include "PolarCoderBPL.h"
#include "MemoryManagementUtil.h"
#include "utility.h"
#include "FileName.h"
#include<fstream>

PolarCoderBPL::PolarCoderBPL(void)
{
	ifstream iBPL(BPL_FILE);
	iBPL>>L;
	iBPL>>max_iter;
	iBPL.close();
	metricPAI = (Metric*)createVoidArray(sizeof(Metric), L);
	ReadMetricPAI();
	LL = createDoubleArray(L, n + 1, N);
	RR = createDoubleArray(L, n + 1, N);
	state_scale_new = createIntegerArray(L, n);
	for (size_t l_index = 0; l_index < L; l_index++)
	{
		for(int i=0;i<M-1;i++)
		{
			state_scale_new[l_index][i]=1<<metricPAI[l_index].PAI[M-2-i];
		}
	}
	IsthisPathValid = new bool[L];
	res = createIntegerArray(L, N);
	res_C = createIntegerArray(L, N);
}


PolarCoderBPL::~PolarCoderBPL(void)
{
	deleteDoubleArray(LL, L, n + 1);
	deleteDoubleArray(RR, L, n + 1);
}

void PolarCoderBPL::decoder(Communication* comm)
{
	double* signal = comm->signal;
	for (int l = 0; l < L; l++)
	{
		decoderCore(comm, l);
	}
	double* pm = createDoubleArray(L);
	double* channel_output =createDoubleArray(N) ;
	for (size_t i = 0; i < L; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			res_C[i][j] = 1 - 2 * res_C[i][j];
		}
	}
	for (size_t i = 0; i < L; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			double temp=res_C[i][j] - signal[j];
			pm[i] += temp*temp;//计算平方欧式距离 
		}
	}
	double min = pm[0];
	int record_path = 0;
	for (size_t i = 1; i < L; i++)
	{
		if (pm[i] < min && IsthisPathValid[i] == true)
		{
			min = pm[i];
			record_path = i;
		}
	}
	for (size_t i = 0; i < N; i++)
	{	
		comm->Estimate[i] = res[record_path][i];
	}
}

void PolarCoderBPL::decoderCore(Communication* comm, int path)
{
	double* LLR = comm->LLR;
	double* signal = comm->signal;
	double sigma=comm->sigma;
	double* softInfo_codeword = createDoubleArray(N);
	double* softInfo_source = createDoubleArray(N);

	int unfrozenNum;
	int wn = 0, ww = 0;
	int* esti = createIntegerArray(N);
	int* estis = createIntegerArray(N);
	int* estimate = createIntegerArray(N);
	int* estimates = createIntegerArray(N);
	int tt = 0, flag = 0;
	int crc_flag=0;
	double n0 = sigma * sigma;

	unfrozenNum = N - K;
	int M_m = unfrozenNum; // M_m 是unfrozenNum的拷贝
	for (int beta = 0; beta < N; beta++)  //初始化
	{
		for (int i = 0; i < M; i++)
		{
			LL[path][i][beta] = 0.0;
			RR[path][i][beta] = 0.0;
		}
	}
	
	for (int beta = 0; beta < N; beta++)  //初始化
	{
		LL[path][M - 1][beta] = LLR[beta]; //log(exp(-pow((signal[beta]-1.0),2)/(2.0*sigma*sigma))/exp(-pow((signal[beta]+1.0),2)/(2.0*sigma*sigma)));	
		if (LL[path][M - 1][beta] > Inf)
		{
			LL[path][M - 1][beta] = Inf;
		}
		else if (LL[path][M - 1][beta] < -Inf)
		{
			LL[path][M - 1][beta] = -Inf;
		}
		RR[path][0][beta] = 0.0;
	}
	for (int beta = 0; beta < N; beta++)
	{
		if(mark_frozenPosition[beta] == 0)
		{
			RR[path][0][beta] = Inf;
		}
	}

	M_m = unfrozenNum;
	wn=0;
	for (int k = 0; k < max_iter; k++)
	{
		//记录迭代次数
		vTOc(path);//图的右边传到左边
		cTOv(path);//图的左边传到右边
		for (int fai = 0; fai < N; fai++)
		{
			softInfo_codeword[fai] = LL[path][M - 1][fai] + RR[path][M - 1][fai];//codeword
			softInfo_source[fai]=LL[path][0][fai] + RR[path][0][fai]; //info
		}

		for (int fai = 0; fai < N; fai++)
		{
			if (softInfo_codeword[fai] >= 0) { estimates[fai] = 0; }
			else estimates[fai] = 1;

			if (softInfo_source[fai] >= 0) { estimate[fai] = 0; }
			else estimate[fai] = 1;
		}

		Decoding(esti, estimate,path);

		flag=0;
		for (int i = 0; i < N; i++)
		{
			if (esti[i] != estimates[i])
			{
				flag = 1;
				break;
			}
		}
		if (flag==0) 
		{
			for (size_t i = 0; i < N; i++)
			{
				res[path][i] = estimate[i];
				res_C[path][i] = estimates[i];
			}
			IsthisPathValid[path] = true;
			return;
		}
	}
	deleteIntegerArray(esti);
	deleteIntegerArray(estis);
	deleteIntegerArray(estimate);
	deleteIntegerArray(estimates);
	deleteDoubleArray(softInfo_codeword);
	deleteDoubleArray(softInfo_source);
}

void PolarCoderBPL::vTOc(int path)
{
	for (int i = M - 1; i > 0; i--)
	{
		vector<bool> isthisPositionOccupied(N, false);
		int step = state_scale_new[path][i - 1];
		for (int j = 0; j < N; j++)
		{
			if (isthisPositionOccupied[j] == true || isthisPositionOccupied[j + step] == true)
				continue;
			LL[path][i - 1][j] = box_plus(LL[path][i][j], (LL[path][i][j + step] + RR[path][i - 1][j + step]));
			LL[path][i - 1][j + step] = box_plus(RR[path][i - 1][j], LL[path][i][j]) + LL[path][i][j + step];
			if (LL[path][i - 1][j] > Inf)
			{
				LL[path][i - 1][j] = Inf;
			}
			else if (LL[path][i - 1][j] < -Inf)
			{
				LL[path][i - 1][j] = -Inf;
			}
			if (LL[path][i - 1][j + step] > Inf)
			{
				LL[path][i - 1][j + step] = Inf;
			}
			else if (LL[path][i - 1][j + step] < -Inf)
			{
				LL[path][i - 1][j + step] = -Inf;
			}
			isthisPositionOccupied[j] = true;
			isthisPositionOccupied[j + step] = true;
		}
		vector<bool>().swap(isthisPositionOccupied);
	}
}


void PolarCoderBPL::cTOv(int path)
{
	for (int i = 0; i < M - 1; i++)
	{
		vector<bool> isthisPositionOccupied(N, false);
		int step = state_scale_new[path][i];
		for (int j = 0; j < N; j++)
		{
			//RR[path][i+1][2 * j] = box_plus(RR[path][i][j], (RR[path][i][j + N / 2] + LL[path][i + 1][2 * j + 1]));
			if (isthisPositionOccupied[j] == true || isthisPositionOccupied[j + step] == true)
				continue;

			RR[path][i+1][j] = box_plus(RR[path][i][j], (RR[path][i][j + step] + LL[path][i + 1][j + step]));

			//RR[path][i+1][2 * j + 1] = box_plus(RR[path][i][j], LL[path][i + 1][2 * j]) + RR[path][i][j + N / 2];

			RR[path][i+1][j + step] = box_plus(RR[path][i][j], LL[path][i + 1][j]) + RR[path][i][j + step];

			if (RR[path][i+1][j] > Inf) { RR[path][i+1][j] = Inf; }
			else if (RR[path][i+1][j] < -Inf) { RR[path][i+1][j] = -Inf; }
			if (RR[path][i+1][j + step] > Inf) { RR[path][i+1][j + step] = Inf; }
			else if (RR[path][i+1][j + step] < -Inf) { RR[path][i+1][j + step] = -Inf; }
			isthisPositionOccupied[j] = true;
			isthisPositionOccupied[j + step] = true;
		}
		vector<bool>().swap(isthisPositionOccupied);
	}
}

void PolarCoderBPL::Decoding(int *estimates,int *estimate,int path)
{
	int* copyCodeWord = createIntegerArray(N);
	for (size_t i = 0; i < N; i++)
		copyCodeWord[i] = estimate[i];
	int state_Num = log2(N);
	int info_length = N;
	for (size_t state_index = 0; state_index < state_Num; state_index++)
	{
		int step = state_scale_new[path][state_index];
		vector<bool> isThisIndexOccupied(N, false);
		for (size_t phi = 0; phi < info_length; phi++)
		{
			if (isThisIndexOccupied[phi] == true || isThisIndexOccupied[phi] == true)
				continue;
			copyCodeWord[phi] = (copyCodeWord[phi] + copyCodeWord[phi + step]) % 2;
			copyCodeWord[phi + step] = copyCodeWord[phi + step];
			isThisIndexOccupied[phi] = true;
			isThisIndexOccupied[phi + step] = true;
		}
	}
	for (size_t i = 0; i < info_length; i++)
	{
		estimates[i] = copyCodeWord[i];
	}
	deleteIntegerArray(copyCodeWord);
}

void PolarCoderBPL::ReadMetricPAI()
{
	int size=L;
	ifstream in(MetricPAI_FILE);
	int i=0;
	while(in>>metricPAI[i].metric&&i<size){
		int *PAI=new int[n];
		for(int j=0;j<n;j++)	in>>PAI[j];
		metricPAI[i].PAI=PAI;
		i++;
	}
	in.close();
}