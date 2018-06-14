#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<vector>
#define N 30
#define pi 3.1415926
#define popmax 5.12  // 个体取值
#define popmin -5.12
#define delta 0.5   //Diff函数中的参数
#define sigma 0.5   //Identify函数中的参数
using std::vector;
int FEs;
double func(double x[])
{
	double rt1;
	int i;

	rt1 = 0;

	for (i = 0; i<30; i++) {
		rt1 += x[i] * x[i] - 10 * cos(2 * pi*x[i]) + 10;
	}
	return rt1;
}
void Diff(double x[],double v[],int dim)  //输出：向量v【N】和 FEs.
{
	FEs = 0;
	double value1,value2;
	double Xd[N];
	int i;
	value1 = func(x);
	FEs += 1;
	for (i = 0; i++; i < dim)
	{
		Xd[i] = x[i] + delta;
		value2 = func(Xd);
		v[i] = value2 - value1;
		FEs += 1;
	}
}
void Identify()
{
	int FEs_used = 0;
	vector<int> seps;
	vector<int> nonseps;
	int all_dims[N];
	double x[N],X[N];
	
	int i;
	for (i = 0; i < N; i++)
		x[i] = ((double)(rand() % 1000) / 1000.0)*(popmax - popmin) + popmin;
	double dv[N],pv[N];
	Diff(x,dv,N);
	FEs_used += FEs;
	for(i=0;i++;i<N)
	{
		X[N] = x[N] + sigma;
	}
}

