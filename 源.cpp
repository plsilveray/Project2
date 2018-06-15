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
#define xi1 0.5
#define xi2 0.5
using std::vector;

int FEs;
int group[N];
vector<int> seps;
vector<int> nonseps;
int FEs_used = 0;
int flag = 0;      //记录group数组是否为空

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
	
	int all_dims[N];
	double x[N], X[N];
	vector<int> pdim(1);
	int i;
	for (i = 0; i < N; i++)
		x[i] = ((double)(rand() % 1000) / 1000.0)*(popmax - popmin) + popmin;
	double dv[N], pv[N];
	Diff(x, dv, N);
	FEs_used += FEs;
	for (i = 0; i++; i < N)
	{
		X[N] = x[N] + sigma;
		X[i] = x[i];
		Diff(X, pv, i);
		FEs_used += FEs;
		if (pv[i] - dv[i] >= -xi1 && pv[i] - dv[i] <= xi1)
			seps.push_back(i);
		else
			nonseps.push_back(i);
	}
	
	auto it = nonseps.begin();
	*it = 0;
	int j = 0;
	while (!nonseps.empty())
	{
		pdim = { nonseps[0] };
		group[j] = nonseps[0];
		flag = 1;
		it = nonseps.erase(it);
		while (!pdim.empty() && !nonseps.empty())
		{
			for (i = 0; i < N; i++)
				X[i] = x[i];
			X[pdim[0]] += sigma;
			Diff(X, pv, nonseps.size());
			FEs_used += FEs;
			for (i = 0; i < nonseps.size(); i++)
			{
				if (pv[i] - dv[i] >= -xi2 && pv[i] - dv[i] <= xi2)
				{
					group[j + 1] = i;
					pdim = { i };
				}
			}
			it = nonseps.begin();
			while (it != nonseps.end())
			{
				if (*it = group[j])
					it = nonseps.erase(it);
				++it;
			}
		}
		if (group[j] = 1 || group[j] = -1)
			seps.push_back(group[j]);
		else
			j += 1;
	}
}
void main()
	{
	Identify();
	if(flag)
	{ }
	}