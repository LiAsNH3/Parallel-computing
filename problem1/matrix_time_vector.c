// -*- encoding: utf8 -*-
/**
* @file matrix_time_vector.c
* @author lsa
* @brief: 并行计算矩阵乘向量
* \f[Ax=b\f]
* 将矩阵按列卷帘存储，向量对应划分存储
* @version 0.1
* @date 2018/12/16
*
* @copyright Copyright (c) 2018
*
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"mpi.h"


int main(int argc, char *argv[])
{
	int i, j, k;//循环变量
	int myrank, numrank;//进程信息
	int m = 0, n = 0;//全局矩阵矩阵信息
	int pm = 0, pn = 0, x_pm = 0, x_pn = 0;//局部矩阵信息
	int namelen;
	double *a = NULL, *x = NULL, *b = NULL;//局部矩阵指针
	double wall_time_first = 0, wall_time_end = 0;//计算墙上时间
	FILE *fp_a, *fp_b, *fp_x;//定义文件指针，访问文件
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	char file_name[15];//记录文件名称
	MPI_Comm mycomm;//定义进程


	MPI_Init(&argc, &argv);//初始化并行进程
	MPI_Comm_dup(MPI_COMM_WORLD, &mycomm);//注册进程名称
	MPI_Comm_size(mycomm, &numrank);//获取总进程数
	MPI_Comm_rank(mycomm, &myrank);//获取当前进程的编号

	MPI_Get_processor_name(processor_name, &namelen);//获取进程名称

													 //读取局部矩阵A
	sprintf(file_name, "%c%c%d%s", 'A', '0', myrank, ".txt");//按进程编号写入字符串文件名----------------文件名
	fp_a = fopen(file_name, "r");//打开文件，异常处理
	if (fp_a == NULL) {
		printf("myrank=%d,failed open file:%s\n", myrank, file_name);
		MPI_Finalize();
		return -1;
	}

	fscanf(fp_a, "%d%d", &pm, &pn);//读取首行矩阵维度信息

	a = (double*)calloc(sizeof(double), pm*pn);//动态分配存储矩阵内存
	if (a == NULL) {//异常处理
		printf("Failed to allocate memory!!!\n");
		return -1;
	}

	//从文件读取矩阵A的局部矩阵
	for (i = 0; i<pm; i++) {
		for (j = 0; j<pn; j++) {
			fscanf(fp_a, "%lf", a + i*pn+ j);
		}
	}
	fclose(fp_a);


	sprintf(file_name, "%c%d%s", 'x', myrank, "0.txt");//--------------------------------------文件名

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
	//计算矩阵乘向量
	if (pn != x_pm) {
		printf("myrank=%d,X_pm=%d,pn=%d!!!!\n", myrank, x_pm,pn);
	}
	for (i = 0; i<pm; i++) {
		*(b + i) = 0;
		for (j = 0; j<pn; j++) {
			*(b + i) += *(a + i*pn+ j)*(*(x + j));
		}
	}


	//进行规约操作，将结果存入进程0
	double *bb;
	bb = (double *)calloc(sizeof(double), pm);
	if (bb == NULL) {
		printf("Failed to allocate memory!!!\n");
		MPI_Finalize();
		return -1;
	}
	MPI_Reduce(b, bb, pm, MPI_DOUBLE, MPI_SUM, 0, mycomm);
	wall_time_end = MPI_Wtime();


	double wall_time, max_wall_time;
	wall_time = wall_time_end - wall_time_first;
	MPI_Reduce(&wall_time, &max_wall_time, 1, MPI_DOUBLE, MPI_MAX, 0, mycomm);
	printf("%s %d %.12lf\n", processor_name, myrank, wall_time_end - wall_time_first);

	//结果输出
	if (myrank == 0) {
		fp_b = fopen("b.txt", "w+");//------------文件名
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
		printf("%s %d %.12lf\n", processor_name, myrank, wall_time_end - wall_time_first);
	}

	//释放内存，结束进程
	free(a);
	free(x);
	free(b);
	free(bb);
	MPI_Finalize();
	return 0;

}


