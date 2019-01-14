/**
*@file C_problem1.c
*@author lsa
*@date 20191/13
*@brif 对于矩阵乘以向量的的串行计算程序
从文件中读取数据并计算出结果，将其输出到文件中
*/

#include<stdlib.h>
#include<stdio.h>
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
	FILE *fp_A, *fp_x, *fp_b;
	int m_A, n_A,n_x,m_x;//维数记录
	double *A, *x, *b;
	double cpu0, cpu1, waLL0, waLL1;
	int i, j,k;//循环变量

	//读取系数矩阵
	fp_A = fopen("A.txt", "r+");
	if (fp_A == NULL)return -1;
	fscanf(fp_A, "%d%d\n", &m_A, &n_A);
	A = (double *)calloc(m_A*n_A, sizeof(double));
	for (i = 0; i<m_A; i++) {
		for (j = 0; j<n_A; j++) {
			fscanf(fp_A, "%lf ",(A+i*n_A+j));
		}
	}
	fclose(fp_A);

	//读取向量
	fp_x = fopen("x.txt", "r+");
	if (fp_x == NULL)return -1;
	fscanf(fp_x, "%d%d\n", &m_x, &n_x);
	x = (double *)calloc(m_x*n_x, sizeof(double));
	for (i = 0; i<m_x; i++) {
		for (j = 0; j<n_x; j++) {
			fscanf(fp_A, "%lf ", (x + i*n_x + j));
		}
	}
	fclose(fp_x);

	if (n_A != m_x) {
		printf("matrix dimension!!!");
		return -1;
	}

	//计算矩阵乘以向量
	b = (double *)calloc(m_A*n_x, sizeof(double));
	gettime(&cpu0, &waLL0);
	for (i = 0; i < m_A; i++) {
		for (j = 0; j < n_x; j++) {
			*(b + i*n_x + j) = 0;
			for (k = 0; k < n_A; k++) {
				*(b + i*n_x + j) += *(A + i*n_A + k)*(*(x + k*n_x + j));
			}
		}
	}
	gettime(&cpu1, &waLL1);

	//将结果输出到文件
	fp_b = fopen("b_c.txt", "w+");
	if (fp_b == NULL)return -1;
	//fprintf(fp_b, "%d  %d\n", m_A, n_x);
	for (i = 0; i<m_A; i++) {
		for (j = 0; j<n_x; j++) {
			fprintf(fp_b, "%lf \n", *(b + i*n_x + j));
		}
	}
	fclose(fp_b);

	printf("cputime=%.10lf,walltime=%.10lf\n",cpu1 - cpu0, waLL1 - waLL0);

	return 0;

}