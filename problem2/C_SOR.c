/**
 * @file C_SOR.c
 * @author lsa
 * @brief SOR迭代法的串行程序，利用五点差分格式生成的矩阵
 * 和右端项，计算possion方程的数值解；
 * 所用的矩阵为红黑排序之前的，精度控制由迭代次数max_N控制
 * @version 0.1
 * @date 2019-01-14
 * 
 */


#include<stdio.h>
#include<stdlib.h>
//仅仅针对Linux操作系统，有这个头文件
#include<sys/time.h>
#include<sys/resource.h>

/**
 * @brief 获取计算过程中的墙上时间和cpu时间
 * 
 * @param cpu 为一个double 型指针，记录cpu时间
 * @param wall 为一个double 型指针，记录墙上时间　
 */
void gettime(double *cpu, double *wall)
{
	struct timeval tv;
	struct rusage ru;
	if (cpu != NULL) {
		getrusage(RUSAGE_SELF, &ru);
		*cpu = ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec*1e-6;
	}
	if (wall != NULL) {
		gettimeofday(&tv, NULL);
		*wall = tv.tv_sec + (double)tv.tv_usec*1e-6;
	}
}

int main()
{
    int i,j,k;//循环变量
    int m_A,m_F;//系数矩阵的行数
    int n_A,n_F;//系数矩阵的列数
    int c[2]={8,8};
    double *A;//存储系数矩阵的指针
    double *F;//存储右端项的指针
    FILE *fp_A,*fp_F;//对应于数据的文件指针
    double w=1;//SOR方法中松弛因子
    double *x;//迭代过程量
    int max_N=1000;//最大跌迭次数
    double eps;//迭代精度控制
    double cpu0, cpu1, waLL0, waLL1;
    

/***************读取系数矩阵和右端项*********************/
    printf("please enter the number of red points and black points:");
    scanf("%d %d",&c[0],&c[1]);
    fp_A=fopen("A_sorted.txt","r+");
    if(fp_A==NULL)printf("failed to open file !!!\n");
    fscanf(fp_A,"%d%d",&m_A,&n_A);
    A=(double *)calloc(sizeof(double),m_A*n_A);
    if(A==NULL)printf("failed to calloc A !!!\n");
    for(i=0;i<m_A;i++){
        for(j=0;j<n_A;j++){
            fscanf(fp_A,"%lf",A+i*n_A+j);
        }
    }
    fclose(fp_A);


    fp_F=fopen("F_sorted.txt","r+");
    if(fp_F==NULL)printf("failed to open file !!!\n");
    fscanf(fp_F,"%d%d",&m_F,&n_F);
    F=(double *)calloc(sizeof(double),m_F*n_F);
    if(A==NULL)printf("failed to calloc A !!!\n");
    for(i=0;i<m_F;i++){
        for(j=0;j<n_F;j++){
            fscanf(fp_A,"%lf",F+i*n_F+j);
        }
    }
    fclose(fp_F);


    if(n_F!=1 || m_A!=m_F || m_A!=n_A)printf("matrix dimension !!!\n");

    x=(double *)calloc(sizeof(double),n_A*1);
    for(i=0;i<n_A;i++)
        *(x+i)=0;

    //SOR主循环迭代
    double zj1=0,zj2=0;//迭代所需的中间变量
    gettime(&cpu0, &waLL0);
    for(k=0;k<max_N;k++){
        for(i=0;i<c[0];i++){
            zj1=0;
            for(j=c[0];j<n_A;j++){
                zj1=zj1+(*(A+i*n_A+j))*(*(x+j));
            }
            *(x+i)=(*(F+i)-zj1)/(*(A+i*n_A+i));
        }

        for(i=c[0];i<n_A;i++){
            zj2=0;
            for(j=0;j<c[0];j++){
               zj2=zj2+*(A+i*n_A+j)*(*(x+j)); 
            }
            *(x+i)=(*(F+i)-zj2)/(*(A+i*n_A+i));
        }
    }
    gettime(&cpu1, &waLL1);

    fp_A=fopen("u_c.txt","w+");
    if(fp_A==NULL)printf("fialed to open file !!!\n");
    for(i=0;i<n_A;i++){
        fprintf(fp_A,"%lf\n",*(x+i));
    }
    fclose(fp_A);
    free(A);
    free(F);
    free(x);
    printf("cputime=%.10lf,walltime=%.10lf\n",cpu1 - cpu0, waLL1 - waLL0);
    return 0;

}