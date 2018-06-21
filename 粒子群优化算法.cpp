#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define c1 2 //加速度因子
#define c2 2
#define maxgen 100  // 迭代次数
#define sizepop 100 // 种群规模
#define popmax 5.12  // 个体最大取值
#define popmin -5.12 // 个体最小取值
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
double func(double x[])
{
double rt1;
	int i;

	rt1 = 0;

	for(i=0;i<30;i++){
		rt1 += x[i]*x[i]-10*cos(2*pi*x[i])+10;
	}
	return rt1;
}   
double Vmax(double a[sizepop][dim],int k)
{
	int i;
	double vmax=a[k][0],v;
	for(i=0;i<dim;i++)
	{if(a[k][i]>vmax)
	vmax=a[k][i];
	}
	v=vmax;
	return v; 
}
double Vmin(double a[sizepop][dim],int k)
{
	int i;
	double vmin=a[k][0],v;
	for(i=0;i<dim;i++)
	{
	if(a[k][i]<vmin)
	vmin=a[k][i];
	}
	v=vmin;
	return v; 
} 
// 种群初始化
void pop_init(void)
{
    for(int i=0;i<sizepop;i++)
    {
        for(int j=0;j<dim;j++)
        {
            pop[i][j] = ((double)(rand() % 1000) / 1000.0)*(popmax - popmin) + popmin; 
            V[i][j] = ((double)(rand() % 1000) / 1000.0)*(Vmax(pop,i) - Vmin(pop,i)) + Vmin(pop,i); 
			V[i][j]=0.2*V[i][j];//0.2(xmax-xmin)
        }
        fitness[i] = func(pop[i]); //计算适应度函数值
    }
}


// max()函数定义
double * min(double * fit,int size)
{
    int index = 0; // 初始化序号
    double min = *fit; // 初始化最小值为数组第一个元素
    static double best_fit_index[2];
    for(int i=1;i<size;i++)
    {
        if(*(fit+i) < min)
            min = *(fit+i);
            index = i;
    }
    best_fit_index[0] = index;
    best_fit_index[1] = min;
    return best_fit_index;

}
// 迭代寻优
void PSO_func(void)
{
    pop_init();
    double * best_fit_index; // 用于存放群体极值位置(序号)
    best_fit_index = min(fitness,sizepop); //求群体极值位置 
    int index = (int)(*best_fit_index);
    // 群体极值位置
    for(int i=0;i<dim;i++)
    {
        gbest[i] = pop[index][i];
    }
    // 个体极值位置
    for(int i=0;i<sizepop;i++)
    {
        for(int j=0;j<dim;j++)
        {
            pbest[i][j] = pop[i][j];
        }
    }
    // 个体极值适应度值
    for(int i=0;i<sizepop;i++)
    {
        fitnesspbest[i] = fitness[i];
    }
    //群体极值适应度值
    double bestfitness = *(best_fit_index+1);
    fitnessgbest = bestfitness;

    //迭代寻优
    for(int i=0;i<maxgen;i++)
    {
        for(int j=0;j<sizepop;j++)
        {
            //速度更新及粒子更新
            for(int k=0;k<dim;k++)
            {
                // 速度更新
                double rand1 = (double)rand()/RAND_MAX; //0到1之间的随机数
                double rand2 = (double)rand()/RAND_MAX;
                V[j][k] = V[j][k]/2 + c1*rand1*(pbest[j][k]-pop[j][k]) + c2*rand2*(gbest[k]-pop[j][k]);
                if(V[j][k] > Vmax(pop,k))
                    V[j][k] = Vmax(pop,k);
                if(V[j][k] < Vmin(pop,k))
                    V[j][k] =Vmin(pop,k);
                // 粒子更新
                pop[j][k] = pop[j][k] + V[j][k];
                if(pop[j][k] > popmax)
                    pop[j][k] = popmax;
                if(pop[j][k] < popmin)
                    pop[j][k] = popmin;
            }
            fitness[j] = func(pop[j]); //新粒子的适应度值
        }
        for(int j=0;j<sizepop;j++)
        {
            // 个体极值更新
            if(fitness[j] < fitnesspbest[j])
            {
                for(int k=0;k<dim;k++)
                {
                    pbest[j][k] = pop[j][k];
                }
                fitnesspbest[j] = fitness[j];
            }
            // 群体极值更新
            if(fitness[j] < fitnessgbest)
            {
                for(int k=0;k<dim;k++)
                    gbest[k] = pop[j][k];
                fitnessgbest = fitness[j];
            }
        }
        for(int k=0;k<dim;k++)
        {
            genbest[i][k] = gbest[k]; // 每一代最优值取值粒子位置记录
        }
        result[i] = fitnessgbest; // 每代的最优值记录到数组
       // printf("%d,%g\n",i,result[i]) ;
	}
}

// 主函数
int main(void)
{
    int k;
    srand((unsigned)time(NULL)); // 初始化随机数种子
   for(k=0;k<5;k++)
   {
	 
    PSO_func();
    double * best_arr;
    best_arr = min(result,maxgen);
    int best_gen_number = *best_arr; // 最优值所处的代数
    double best = *(best_arr+1); //最优值
    printf("迭代了%d次，最优值为:%g.\n",maxgen,best);
}
    return 0;
}
