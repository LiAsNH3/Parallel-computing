/*��
*@file matrix_generate.c
@date  2019/1/13
@author lsa
@brif ����һ������������������һ���������ĳ���
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main()
{
	int n = 0, i, j;
	FILE *fp1, *fp2;
	printf("please enter the size of Matrix:\n");//����ϵ������Ĵ�С
	scanf("%d", &n);


	//�������ϵ�����󣬲�������ļ�
	fp1 = fopen("A.txt", "w+");
	if (fp1 == NULL)return -1;
	fprintf(fp1, "%d  %d\n", n, n);
	for (i = 0; i<n; i++) {
		for (j = 0; j<n; j++) {
			fprintf(fp1, "%.5lf ", rand() / (double)(RAND_MAX / 10) + 1);
		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);

	//�������һ����������������ļ�
	fp2 = fopen("x.txt", "w+");
	if (fp2 == NULL)return -1;
	fprintf(fp2, "%d  %d\n", n, 1);
	for (i = 0; i<n; i++) {
		fprintf(fp2, "%.5lf\n", rand() / (double)(RAND_MAX / 10) + 1);
	}
	fclose(fp2);
	printf("succesfullly done !!!\n");
	return 0;
}