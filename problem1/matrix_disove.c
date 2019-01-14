/**
* @file resolve_matrix.c
* @author lsa
* @brief �����������Ľ���Ҫ��ѭ�����ִ洢
* @version 0.1
* @date 2018-12-16
*
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int i, j,k,s;//ѭ������
	int pm, pn;//������
	int  m, n;//�����־����ά��
	double *a;
	FILE *fp, *fp1;
	char file_name[15];

	//��ȡ��ʼ��Ϣ
	 printf("please enter the process numbers(row and col):");
	 scanf("%d%d",&pm,&pn);

	//��ȡԭʼ����,�ļ���һ��Ϊ�����ά��
	 printf("please enter the filename:");
	 scanf("%s", file_name);
	fp=fopen(file_name,"r");
	if (fp == NULL) {
		printf("file open failed!\n");
		return -1;
	}


	fscanf(fp, "%d%d", &m, &n);//��ȡά��
	if (pm>m || pn>n) {
		printf("erro:number of process >colucms or rows!!!!!!\n");
		return -1;
	}
	//�����ڴ棬�洢ԭʼ����
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

	//����ֿ����
	for (i = 0; i<pm; i++) {
		for (s = 0; s<pn; s++) {
			//ÿ�����̵��ļ�,ѭ�����֣��������֣�
			sprintf(file_name + 1, "%d%d%s", i, s, ".txt");
			fp1 = fopen(file_name, "w+");
			if (fp1 == NULL) {
				printf("file open failed!\n");
				return -1;
			}

			//����ľ�������ֲ�ά����Ϣ
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