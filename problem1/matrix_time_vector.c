/**
* @file matrix_time_vector.c
* @author lsa
* @brief: ���м�����������
* \f[Ax=b\f]
* �������о����洢��������Ӧ���ִ洢������Լ��͵������̣������������ļ�
* @version 0.1
* @date 2018/12/16
*
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"mpi.h"


int main(int argc, char *argv[])
{
	int i, j, k;//ѭ������
	int myrank, numrank;//������Ϣ
	int m = 0, n = 0;//ȫ�־��������Ϣ
	int pm = 0, pn = 0, x_pm = 0, x_pn = 0;//�ֲ�������Ϣ
	int namelen;
	double *a = NULL, *x = NULL, *b = NULL;//�ֲ�����ָ��
	double wall_time_first = 0, wall_time_end = 0;//����ǽ��ʱ��
	FILE *fp_a, *fp_b, *fp_x;//�����ļ�ָ�룬�����ļ�
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	char file_name[15];//��¼�ļ�����
	MPI_Comm mycomm;//�������


	MPI_Init(&argc, &argv);//��ʼ�����н���
	MPI_Comm_dup(MPI_COMM_WORLD, &mycomm);//ע���������
	MPI_Comm_size(mycomm, &numrank);//��ȡ�ܽ�����
	MPI_Comm_rank(mycomm, &myrank);//��ȡ��ǰ���̵ı��

	MPI_Get_processor_name(processor_name, &namelen);//��ȡ��������

	//��ȡ�ֲ�����A
	sprintf(file_name, "%c%c%d%s", 'A', '0', myrank, ".txt");//�����̱��д���ַ����ļ���----------------�ļ���
	fp_a = fopen(file_name, "r");//���ļ����쳣����
	if (fp_a == NULL) {
		printf("myrank=%d,failed open file:%s\n", myrank, file_name);
		MPI_Finalize();
		return -1;
	}

	fscanf(fp_a, "%d%d", &pm, &pn);//��ȡ���о���ά����Ϣ

	a = (double*)calloc(sizeof(double), pm*pn);//��̬����洢�����ڴ�
	if (a == NULL) {//�쳣����
		printf("Failed to allocate memory!!!\n");
		return -1;
	}

	//���ļ���ȡ����A�ľֲ�����
	for (i = 0; i<pm; i++) {
		for (j = 0; j<pn; j++) {
			fscanf(fp_a, "%lf", a + i*pn+ j);
		}
	}
	fclose(fp_a);


	sprintf(file_name, "%c%d%s", 'x', myrank, "0.txt");//--------------------------------------�ļ���

	fp_x = fopen(file_name, "r");
	if (fp_x == NULL) {
		printf("myrank=%d,failed open file:%s\n", myrank, file_name);
		return -1;
	}


	fscanf(fp_x, "%d%d", &x_pm, &x_pn);
	x = (double *)calloc(sizeof(double), x_pn*x_pm);
	b = (double *)calloc(sizeof(double), pm * 1);

	if (x == NULL || b == NULL) {
		printf("Failed to allocate memory!!!\n");
		MPI_Finalize();
		return -1;
	}

	if (x_pn != 1) {
		printf("myrank=%d,X_pn=%d!!!!\n", myrank, x_pn);
	}
	for (i = 0; i<x_pm; i++) {
		fscanf(fp_x, "%lf", x + i);
	}
	fclose(fp_x);


	wall_time_first = MPI_Wtime();
	//������������
	if (pn != x_pm) {
		printf("myrank=%d,X_pm=%d,pn=%d!!!!\n", myrank, x_pm,pn);
	}
	for (i = 0; i<pm; i++) {
		*(b + i) = 0;
		for (j = 0; j<pn; j++) {
			*(b + i) += *(a + i*pn+ j)*(*(x + j));
		}
	}


	//���й�Լ������������������0
	double *bb;
	bb = (double *)calloc(sizeof(double), pm);
	if (bb == NULL) {
		printf("Failed to allocate memory!!!\n");
		MPI_Finalize();
		return -1;
	}
	MPI_Reduce(b, bb, pm, MPI_DOUBLE, MPI_SUM, 0, mycomm);
	wall_time_end = MPI_Wtime();


	//��ÿ�����̵�ǽ��ʱ����й�Լ�������ڣ����̴洢���ǽ��ʱ�䣬��Ϊ�������г������յ�ǽ��ʱ��
	double wall_time, max_wall_time;
	wall_time = wall_time_end - wall_time_first;
	MPI_Reduce(&wall_time, &max_wall_time, 1, MPI_DOUBLE, MPI_MAX, 0, mycomm);
	//printf("%s %d %.12lf\n", processor_name, myrank, wall_time_end - wall_time_first);

	//������
	if (myrank == 0) {
		fp_b = fopen("b.txt", "w+");//------------�ļ���
		if (fp_b == NULL) {
			printf("failed to open file\n");
			MPI_Finalize();
			return -1;
		}
		for (i = 0; i<pm; i++) {
			fprintf(fp_b, "%lf \n", *(bb + i));
			//printf("%lf\n",*(bb+i));
		}
		fclose(fp_b);
		//free(bb);
		printf("max wall time: %s %d %.12lf\n", processor_name, myrank, wall_time_end - wall_time_first);
	}

	//�ͷ��ڴ棬��������
	free(a);
	free(x);
	free(b);
	free(bb);
	MPI_Finalize();
	return 0;

}


