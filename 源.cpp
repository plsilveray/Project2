#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<vector>
#define N 5
#define pi 3.1415926
#define popmax 100  // 个体取值
#define popmin -100
#define delta 10   //Diff函数中的参数
#define sigma 0.5   //Identify函数中的参数
#define xi1 0.01
#define xi2 0.01
#define S_max 200
using std::vector;

int FEs;
int j = 0;
double dv[N], pv[N];
vector<vector<int>> group;
vector<vector<int>> subcmpt;
vector<int> seps;
vector<int> nonseps;
int FEs_used = 0;

double func(double x[])
{
	double rt1 = 0;
	int i;
	for (i = 0; i<N; i++) {
		rt1 += (x[i] * x[i]);
	}
	return rt1;
}
void Diff(double x[],double v[],int dim)  //输出：向量v【N】和 FEs.
{
	FEs = 0;
	double value1,value2;
	double Xd[N];
	int i,k;
	value1 = func(x);
	
	FEs += 1;
	for (i = 0; i<dim; i++)
	{
		for (k = 0; k < dim; k++)
			Xd[k] = x[k];
		Xd[i] = Xd[i] + delta;
		value2 = func(Xd);	
		v[i] = value2 - value1;
		FEs += 1;
	}
}
void Identify()
{
	double x[N], X[N];
	vector<int> pdim;
	int i;

	for (i = 0; i < N; i++)
	{
		x[i] = ((double)(rand() % 1000) / 1000.0)*(popmax - popmin) + popmin;
		
	}
	Diff(x, dv, N);
	FEs_used += FEs;	
	for (i = 0; i<N; i++)
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
	
	while (!nonseps.empty())
	{
		pdim = { nonseps[0] };
		group.push_back( pdim);
		it = nonseps.erase(it);
		
		while (!pdim.empty() && !nonseps.empty())
		{
			
			for (i = 0; i < N; i++)
				X[i] = x[i];
			X[pdim[0]] += sigma;
			Diff(X, pv, nonseps.size());
			FEs_used += FEs;
			pdim.clear();
			
			for (i = 0; i < nonseps.size(); i++)
			{
				
				if (pv[i] - dv[i] <= (-xi2) || pv[i] - dv[i] >= xi2)
				{
					group[j].push_back(i);
					
					pdim.push_back(i);
				}
			}
			
			auto it_g = group[j].begin();
			while(it_g!=group[j].end())
			{
				auto it = nonseps.begin();
				while(it!= nonseps.end()) 
				{
					if (*it = *it_g)
						it = nonseps.erase(it);
					else
					it++;
				}	
				it_g++;
			}
			
		}
		printf("%d\n", group[j].size());
		if (group[j].size()== 1)
			seps.push_back(*group[j].begin());
		else
			j += 1;
	}
}
void main()
{
	int n;
	Identify();	
	if (!group.empty())
		subcmpt = group;
	printf("%d,%d", !group.empty(), !seps.empty());
	if (!seps.empty())
	{
		j = subcmpt.size() + 1;
		if (seps.size() >= S_max)
		{
			n = floor(seps.size() / S_max);
			vector<vector<int>> seps_s;
			int i = 0;
			int k = 0;
			auto it_seps = seps.begin();
			while (it_seps != seps.end() && i < n)
			{
				while (k < (seps.size()) / n && it_seps != seps.end())
				{
					seps_s[i].push_back(*it_seps);
					it_seps++;
					k++;
				}
				k = 0;
				subcmpt[j] = seps_s[i];
				i++;
				j++;
			}
		}
		else	
			subcmpt[j] = seps;
	}
	int FEs_max = 0;                 //FEs_max初始化？？
	FEs_max -= FEs_used;
	FEs = 0;
	/*	while (FEs < FEs_max)                      //将seps切分成多个subcmpt然后对各个subcmpt使用optimizer
			for (i = 0; i < subcmpt.size()); i++)
			{optimizer(subcmpt[i])
			{

			}])*/
	int i;
	for (i = 0; i < (seps.size()); i++)
	printf("%d", seps[i]);
	getchar();
}