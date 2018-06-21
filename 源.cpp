#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<vector>
#define N 30
#define pi 3.1415926
#define popmax 100  // ����ȡֵ
#define popmin -100
#define delta 10   //Diff�����еĲ���
#define sigma 0.5   //Identify�����еĲ���
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
void Diff(double x[],double v[],int dim)  //���������v��N���� FEs.
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


#define c1 2 //���ٶ�����
#define c2 2
#define maxgen 10000  // ��������
#define sizepop 100 // ��Ⱥ��ģ
#define popmax 100  // �������ȡֵ
#define popmin -100 // ������Сȡֵ
#define dim 30 // ���ӵ�ά��
#define pi 3.1415926 //Բ����

double pop[sizepop][dim]; // ������Ⱥ����
double V[sizepop][dim]; // ������Ⱥ�ٶ�����
double fitness[sizepop]; // ������Ⱥ����Ӧ������
double result[maxgen];  //������ÿ�ε�����Ⱥ����ֵ������
double pbest[sizepop][dim];  // ���弫ֵ��λ��
double gbest[dim]; //Ⱥ�弫ֵ��λ��
double fitnesspbest[sizepop]; //���弫ֵ��Ӧ�ȵ�ֵ
double fitnessgbest; // Ⱥ�弫ֵ��Ӧ��ֵ
double genbest[maxgen][dim]; //ÿһ������ֵȡֵ����

							 //��Ӧ�Ⱥ���
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
// ��Ⱥ��ʼ��
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
		fitness[i] = func2(pop[i],g); //������Ӧ�Ⱥ���ֵ
	}
}


// max()��������
double * min(double * fit, int size)
{
	int index = 0; // ��ʼ�����
	double min = *fit; // ��ʼ����СֵΪ�����һ��Ԫ��
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
// ����Ѱ��
void PSO_func(vector<int> g)
{
	pop_init(g);
	double * best_fit_index; // ���ڴ��Ⱥ�弫ֵλ��(���)
	best_fit_index = min(fitness, sizepop); //��Ⱥ�弫ֵλ�� 
	int index = (int)(*best_fit_index);
	// Ⱥ�弫ֵλ��
	for (int i = 0; i<dim; i++)
	{
		gbest[i] = pop[index][i];
	}
	// ���弫ֵλ��
	for (int i = 0; i<sizepop; i++)
	{
		for (int j = 0; j<dim; j++)
		{
			pbest[i][j] = pop[i][j];
		}
	}
	// ���弫ֵ��Ӧ��ֵ
	for (int i = 0; i<sizepop; i++)
	{
		fitnesspbest[i] = fitness[i];
	}
	//Ⱥ�弫ֵ��Ӧ��ֵ
	double bestfitness = *(best_fit_index + 1);
	fitnessgbest = bestfitness;

	//����Ѱ��
	for (int i = 0; i<maxgen; i++)
	{
		for (int j = 0; j<sizepop; j++)
		{
			//�ٶȸ��¼����Ӹ���
			for (int k = 0; k<dim; k++)
			{
				// �ٶȸ���
				double rand1 = (double)rand() / RAND_MAX; //0��1֮��������
				double rand2 = (double)rand() / RAND_MAX;
				V[j][k] = V[j][k] / 2 + c1*rand1*(pbest[j][k] - pop[j][k]) + c2*rand2*(gbest[k] - pop[j][k]);
				if (V[j][k] > Vmax(pop, k))
					V[j][k] = Vmax(pop, k);
				if (V[j][k] < Vmin(pop, k))
					V[j][k] = Vmin(pop, k);
				// ���Ӹ���
				pop[j][k] = pop[j][k] + V[j][k];
				if (pop[j][k] > popmax)
					pop[j][k] = popmax;
				if (pop[j][k] < popmin)
					pop[j][k] = popmin;
			}
			fitness[j] = func2(pop[j],g); //�����ӵ���Ӧ��ֵ
		}
		for (int j = 0; j<sizepop; j++)
		{
			// ���弫ֵ����
			if (fitness[j] < fitnesspbest[j])
			{
				for (int k = 0; k<dim; k++)
				{
					pbest[j][k] = pop[j][k];
				}
				fitnesspbest[j] = fitness[j];
			}
			// Ⱥ�弫ֵ����
			if (fitness[j] < fitnessgbest)
			{
				for (int k = 0; k<dim; k++)
					gbest[k] = pop[j][k];
				fitnessgbest = fitness[j];
			}
		}
		for (int k = 0; k<dim; k++)
		{
			genbest[i][k] = gbest[k]; // ÿһ������ֵȡֵ����λ�ü�¼
		}
		result[i] = fitnessgbest; // ÿ��������ֵ��¼������
								  // printf("%d,%g\n",i,result[i]) ;
	}
}

void optimizer(vector<int> g)
{
	    srand((unsigned)time(NULL)); // ��ʼ�����������
		PSO_func(g);
		double * best_arr;
		best_arr = min(result, maxgen);
		int best_gen_number = *best_arr; // ����ֵ�����Ĵ���
		double best = *(best_arr + 1); //����ֵ
		printf("������%d�Σ�����ֵΪ:%g.\n", maxgen, best);	
}
void main()
{
	int n, i, iter;
	Identify();
	if (!group.empty())
		subcmpt = group;                           
	printf("seps�Ƿ�Ϊ�գ�%d\n",seps.empty());
	if (!seps.empty())                          //���������seps����ôsubcmpt����group
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
	int FEs_max = 3000000;                 //FEs_max��ʼ������
	FEs_max -= FEs_used;
	FEs = 0;
	printf("subcmpt�ĸ�����%d\n", subcmpt.size());
	while (FEs < FEs_max)                      //��seps�зֳɶ��subcmptȻ��Ը���subcmptʹ��optimizer����
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