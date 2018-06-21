#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define c1 2 //���ٶ�����
#define c2 2
#define maxgen 100  // ��������
#define sizepop 100 // ��Ⱥ��ģ
#define popmax 5.12  // �������ȡֵ
#define popmin -5.12 // ������Сȡֵ
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
// ��Ⱥ��ʼ��
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
        fitness[i] = func(pop[i]); //������Ӧ�Ⱥ���ֵ
    }
}


// max()��������
double * min(double * fit,int size)
{
    int index = 0; // ��ʼ�����
    double min = *fit; // ��ʼ����СֵΪ�����һ��Ԫ��
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
// ����Ѱ��
void PSO_func(void)
{
    pop_init();
    double * best_fit_index; // ���ڴ��Ⱥ�弫ֵλ��(���)
    best_fit_index = min(fitness,sizepop); //��Ⱥ�弫ֵλ�� 
    int index = (int)(*best_fit_index);
    // Ⱥ�弫ֵλ��
    for(int i=0;i<dim;i++)
    {
        gbest[i] = pop[index][i];
    }
    // ���弫ֵλ��
    for(int i=0;i<sizepop;i++)
    {
        for(int j=0;j<dim;j++)
        {
            pbest[i][j] = pop[i][j];
        }
    }
    // ���弫ֵ��Ӧ��ֵ
    for(int i=0;i<sizepop;i++)
    {
        fitnesspbest[i] = fitness[i];
    }
    //Ⱥ�弫ֵ��Ӧ��ֵ
    double bestfitness = *(best_fit_index+1);
    fitnessgbest = bestfitness;

    //����Ѱ��
    for(int i=0;i<maxgen;i++)
    {
        for(int j=0;j<sizepop;j++)
        {
            //�ٶȸ��¼����Ӹ���
            for(int k=0;k<dim;k++)
            {
                // �ٶȸ���
                double rand1 = (double)rand()/RAND_MAX; //0��1֮��������
                double rand2 = (double)rand()/RAND_MAX;
                V[j][k] = V[j][k]/2 + c1*rand1*(pbest[j][k]-pop[j][k]) + c2*rand2*(gbest[k]-pop[j][k]);
                if(V[j][k] > Vmax(pop,k))
                    V[j][k] = Vmax(pop,k);
                if(V[j][k] < Vmin(pop,k))
                    V[j][k] =Vmin(pop,k);
                // ���Ӹ���
                pop[j][k] = pop[j][k] + V[j][k];
                if(pop[j][k] > popmax)
                    pop[j][k] = popmax;
                if(pop[j][k] < popmin)
                    pop[j][k] = popmin;
            }
            fitness[j] = func(pop[j]); //�����ӵ���Ӧ��ֵ
        }
        for(int j=0;j<sizepop;j++)
        {
            // ���弫ֵ����
            if(fitness[j] < fitnesspbest[j])
            {
                for(int k=0;k<dim;k++)
                {
                    pbest[j][k] = pop[j][k];
                }
                fitnesspbest[j] = fitness[j];
            }
            // Ⱥ�弫ֵ����
            if(fitness[j] < fitnessgbest)
            {
                for(int k=0;k<dim;k++)
                    gbest[k] = pop[j][k];
                fitnessgbest = fitness[j];
            }
        }
        for(int k=0;k<dim;k++)
        {
            genbest[i][k] = gbest[k]; // ÿһ������ֵȡֵ����λ�ü�¼
        }
        result[i] = fitnessgbest; // ÿ��������ֵ��¼������
       // printf("%d,%g\n",i,result[i]) ;
	}
}

// ������
int main(void)
{
    int k;
    srand((unsigned)time(NULL)); // ��ʼ�����������
   for(k=0;k<5;k++)
   {
	 
    PSO_func();
    double * best_arr;
    best_arr = min(result,maxgen);
    int best_gen_number = *best_arr; // ����ֵ�����Ĵ���
    double best = *(best_arr+1); //����ֵ
    printf("������%d�Σ�����ֵΪ:%g.\n",maxgen,best);
}
    return 0;
}
