#include "DataProcessingUtil.h"

void limiting(double& target)
{
	if (target > Inf)
	{
		target = Inf;
	}
	else if (target < -Inf)
	{
		target = -Inf;
	}
}
void normalized(double* array, int len)
{
	double sum = 0;
	for(int i = 0; i < len; i++)
	{
		sum +=array[i];
	}
	if(sum == 0) return;
	for(int i = 0; i < len; i++)
	{
		array[i] /= sum;
	}
}