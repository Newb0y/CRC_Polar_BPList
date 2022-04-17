#include "Communication.h"
#include "random.h"
#include "utility.h"
#include "FileName.h"
#include<fstream>

Communication::Communication()
{
	ifstream iPolar(POLAR_FILE);
	iPolar>>K;
	iPolar>>N;
	iPolar.close();
	source = new int[K];
	constructionSequence = new int[N];
	codeword = new int[N];
	signal = new double[N];
	receive = new double[N];
	LLR = new double[N];
	Estimate = new int[N];
}



Communication::~Communication(void)
{
}

int Communication::communication(double snr)
{
	return 0;
}

//TODO：加信源类型
void Communication::sourcer()//
{
}


void Communication::Transmit(double sigma)
{  

}