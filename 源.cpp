#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<vector>
#define N 30
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
		if (group[j].size()== 1)
			seps.push_back(*group[j].begin());
		else
			j += 1;
	}
}


#define c1 2 //加速度因子
#define c2 2
#define maxgen 10000  // 迭代次数
#define sizepop 100 // 种群规模
#define popmax 100  // 个体最大取值
#define popmin -100 // 个体最小取值
#define dim 30 // 粒子的维数
#define pi 3.1415926 //圆周率

double pop[sizepop][dim]; // 定义种群数组
double V[sizepop][dim]; // 定义种群速度数组
double fitness[sizepop]; // 定义种群的适应度数组
double result[maxgen];  //定义存放每次迭代种群最优值的数组
double pbest[sizepop][dim];  // 个体极值的位置
double gbest[dim]; //群体极值的位置
double fitnesspbest[sizepop]; //个体极值适应度的值
double fitnessgbest; // 群体极值适应度值
double genbest[maxgen][dim]; //每一代最优值取值粒子

							 //适应度函数
double func2(double x[], vector<int> g)
{
	double rt1;
	int i;

	rt1 = 0;
	for(auto it = g.begin();it!=g.end();it++)
		for (i = 0; i < dim; i++)
		{
		if (*it == i)
		rt1 += x[i] * x[i];
		}
	return rt1;
}
double Vmax(double a[sizepop][dim], int k)
{
	int i;
	double vmax = a[k][0], v;
	for (i = 0; i<dim; i++)
	{
		if (a[k][i]>vmax)
			vmax = a[k][i];
	}
	v = vmax;
	return v;
}
double Vmin(double a[sizepop][dim], int k)
{
	int i;
	double vmin = a[k][0], v;
	for (i = 0; i<dim; i++)
	{
		if (a[k][i]<vmin)
			vmin = a[k][i];
	}
	v = vmin;
	return v;
}
// 种群初始化
void pop_init(vector<int> g)
{
	for (int i = 0; i<sizepop; i++)
	{
		for (int j = 0; j<dim; j++)
		{
			pop[i][j] = ((double)(rand() % 1000) / 1000.0)*(popmax - popmin) + popmin;
			V[i][j] = ((double)(rand() % 1000) / 1000.0)*(Vmax(pop, i) - Vmin(pop, i)) + Vmin(pop, i);
			V[i][j] = 0.2*V[i][j];//0.2(xmax-xmin)
		}
		fitness[i] = func2(pop[i],g); //计算适应度函数值
	}
}


// max()函数定义
double * min(double * fit, int size)
{
	int index = 0; // 初始化序号
	double min = *fit; // 初始化最小值为数组第一个元素
	static double best_fit_index[2];
	for (int i = 1; i<size; i++)
	{
		if (*(fit + i) < min)
			min = *(fit + i);
		index = i;
	}
	best_fit_index[0] = index;
	best_fit_index[1] = min;
	return best_fit_index;

}
// 迭代寻优
void PSO_func(vector<int> g)
{
	pop_init(g);
	double * best_fit_index; // 用于存放群体极值位置(序号)
	best_fit_index = min(fitness, sizepop); //求群体极值位置 
	int index = (int)(*best_fit_index);
	// 群体极值位置
	for (int i = 0; i<dim; i++)
	{
		gbest[i] = pop[index][i];
	}
	// 个体极值位置
	for (int i = 0; i<sizepop; i++)
	{
		for (int j = 0; j<dim; j++)
		{
			pbest[i][j] = pop[i][j];
		}
	}
	// 个体极值适应度值
	for (int i = 0; i<sizepop; i++)
	{
		fitnesspbest[i] = fitness[i];
	}
	//群体极值适应度值
	double bestfitness = *(best_fit_index + 1);
	fitnessgbest = bestfitness;

	//迭代寻优
	for (int i = 0; i<maxgen; i++)
	{
		for (int j = 0; j<sizepop; j++)
		{
			//速度更新及粒子更新
			for (int k = 0; k<dim; k++)
			{
				// 速度更新
				double rand1 = (double)rand() / RAND_MAX; //0到1之间的随机数
				double rand2 = (double)rand() / RAND_MAX;
				V[j][k] = V[j][k] / 2 + c1*rand1*(pbest[j][k] - pop[j][k]) + c2*rand2*(gbest[k] - pop[j][k]);
				if (V[j][k] > Vmax(pop, k))
					V[j][k] = Vmax(pop, k);
				if (V[j][k] < Vmin(pop, k))
					V[j][k] = Vmin(pop, k);
				// 粒子更新
				pop[j][k] = pop[j][k] + V[j][k];
				if (pop[j][k] > popmax)
					pop[j][k] = popmax;
				if (pop[j][k] < popmin)
					pop[j][k] = popmin;
			}
			fitness[j] = func2(pop[j],g); //新粒子的适应度值
		}
		for (int j = 0; j<sizepop; j++)
		{
			// 个体极值更新
			if (fitness[j] < fitnesspbest[j])
			{
				for (int k = 0; k<dim; k++)
				{
					pbest[j][k] = pop[j][k];
				}
				fitnesspbest[j] = fitness[j];
			}
			// 群体极值更新
			if (fitness[j] < fitnessgbest)
			{
				for (int k = 0; k<dim; k++)
					gbest[k] = pop[j][k];
				fitnessgbest = fitness[j];
			}
		}
		for (int k = 0; k<dim; k++)
		{
			genbest[i][k] = gbest[k]; // 每一代最优值取值粒子位置记录
		}
		result[i] = fitnessgbest; // 每代的最优值记录到数组
								  // printf("%d,%g\n",i,result[i]) ;
	}
}

void optimizer(vector<int> g)
{
	    srand((unsigned)time(NULL)); // 初始化随机数种子
		PSO_func(g);
		double * best_arr;
		best_arr = min(result, maxgen);
		int best_gen_number = *best_arr; // 最优值所处的代数
		double best = *(best_arr + 1); //最优值
		printf("迭代了%d次，最优值为:%g.\n", maxgen, best);	
}
void main()
{
	int n, i, iter;
	Identify();
	if (!group.empty())
		subcmpt = group;                           
	printf("seps是否为空：%d\n",seps.empty());
	if (!seps.empty())                          //如果不存在seps，那么subcmpt等于group
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
	int FEs_max = 3000000;                 //FEs_max初始化？？
	FEs_max -= FEs_used;
	FEs = 0;
	printf("subcmpt的个数：%d\n", subcmpt.size());
	while (FEs < FEs_max)                      //将seps切分成多个subcmpt然后对各个subcmpt使用optimizer函数
	{
		for (i = 0; i < subcmpt.size(); i++)
		{
			iter = maxgen;
			optimizer(subcmpt[i]);
			FEs = FEs + sizepop*iter;
			iter = 1;
		}

	}
	printf("end");
	getchar();
}