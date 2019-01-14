/**
*@file C_problem1.c
*@author lsa
*@date 20191/13
*@brif ���ھ�����������ĵĴ��м������
���ļ��ж�ȡ���ݲ���������������������ļ���
*/

#include<stdlib.h>
#include<stdio.h>
//�������Linux����ϵͳ�������ͷ�ļ�
#include<sys/time.h>
#include<sys/resource.h>

/**
 * @brief ��ȡ��������е�ǽ��ʱ���cpuʱ��
 * 
 * @param cpu Ϊһ��double ��ָ�룬��¼cpuʱ��
 * @param wall Ϊһ��double ��ָ�룬��¼ǽ��ʱ�䡡
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
	int m_A, n_A,n_x,m_x;//ά����¼
	double *A, *x, *b;
	double cpu0, cpu1, waLL0, waLL1;
	int i, j,k;//ѭ������

	//��ȡϵ������
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

	//��ȡ����
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

	//��������������
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

	//�����������ļ�
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