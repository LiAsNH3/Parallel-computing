// -*- encoding: utf8 -*-
/**
* @file resolve_matrix.c
* @author lsa
* @brief 将矩阵按所给的进程要求循环划分存储
* @version 0.1
* @date 2018-12-16
*
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int i, j,k,s;//循环变量
	int pm, pn;//进程数
	int  m, n;//被划分矩阵的维数
	double *a;
	FILE *fp, *fp1;
	char file_name[15];

	//读取初始信息
	 printf("please enter the process numbers(row and col):");
	 scanf("%d%d",&pm,&pn);

	//读取原始矩阵,文件第一行为矩阵的维数
	 printf("please enter the filename:");
	 scanf("%s", file_name);
	fp=fopen(file_name,"r");
	if (fp == NULL) {
		printf("file open failed!\n");
		return -1;
	}


	fscanf(fp, "%d%d", &m, &n);//读取维数
	if (pm>m || pn>n) {
		printf("erro:number of process >colucms or rows!!!!!!\n");
		return -1;
	}
	//分配内存，存储原始矩阵
	a = (double*)calloc(sizeof(double), m*n);
	if (a == NULL) {
		printf("failed!\n");
		return -1;
	}

	for (i = 0; i<m; i++) {
		for (j = 0; j<n; j++) {
			fscanf(fp, "%lf", a + i + j*m);
		}
	}
	fclose(fp);

	//输出分块矩阵
	for (i = 0; i<pm; i++) {
		for (s = 0; s<pn; s++) {
			//每个进程的文件,循环划分（卷帘划分）
			sprintf(file_name + 1, "%d%d%s", i, s, ".txt");
			fp1 = fopen(file_name, "w+");
			if (fp1 == NULL) {
				printf("file open failed!\n");
				return -1;
			}

			//输出的矩阵包含局部维数信息
			int p_c = 0, p_r = 0;
			for (j = i; j<m; j += pm, p_c++);
			for (k = s; k<n; k += pn, p_r++);
			fprintf(fp1, "%d  %d\n", p_c, p_r);

			for (j = i; j<m; j += pm) {
				for (k = s; k<n; k += pn) {
					fprintf(fp1, "%lf ", *(a + j + k*m));
				}
				fprintf(fp1, "\n");
			}
			fclose(fp1);
		}
	}
	printf("suucssfull !!!\n");
	free(a);
	return 0;
}