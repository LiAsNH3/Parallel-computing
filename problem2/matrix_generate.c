//coding = UTF8
/**
 * @file matrxi_generate.c
 * @author lsa
 * @brief 对于微分方程
 * \Delta u=x*y,(x,y)\in (0,1)\times(0,1)
 * u(0,y)=y,u(1,y)=1-y,u(x,0)=0,u(x,1)=0
 *的五点差分格式的系数矩阵生成和右端项生成，并应用多色排序方法对矩阵
 *和右端项进行变换
 * @version 0.1
 * @date 2019-01-14
 *
 */

#include <stdio.h>
#include<stdlib.h>

int main() {
	int N =5,i,j,matrix_size;//从0开始编号到最终编号为N
	int ni = 0;
	double *A, *F;
	double h;

	//获取需要划分的网格数目
	printf("please enter the number of mesh(x equal to y):");
	scanf("%d",&N);

	h = 1.0 / N;
	matrix_size = (N + 1)*(N + 1);
	A = (double *)calloc(sizeof(double), matrix_size*matrix_size);
	F = (double *)calloc(sizeof(double), matrix_size);
	
	//将矩阵初始化为0
	for (i = 0; i < matrix_size; i++) {
		for (j = 0; j < matrix_size; j++) {
			*(A + i + j*matrix_size) = 0;
		}
		*(F + i) = 0;
	}


    //含边界信息的大矩阵生成
	printf("h=%lf\n", h);
	for (i = 1; i<N; i++) {
		for (j = 1; j < N; j++) {
			ni = i + j*(N + 1);
			*(A + ni + ni*matrix_size) = -4;
			*(A + (i + (j + 1) * (N + 1)) + ni*matrix_size) = 1;
			*(A + (i + (j - 1) * (N + 1)) + ni*matrix_size) = 1;
			*(A + (i + 1 + j * (N + 1)) + ni*matrix_size) = 1;
			*(A + (i - 1 + j * (N + 1)) + ni*matrix_size) = 1;
			*(F + ni) = h*i*h*j*h*h;
			//printf("%lf \n", *(F + ni));
		}
	}

	//下边界
	j = 0;
	for (i = 0; i < N+1; i++) {
		*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) =1;
		*(F + i + j*(N + 1)) = 0;
	}

	//上边界
	j = N;
	for (i = 0; i <= N; i++) {
		*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) =1;
		*(F + i + j*(N + 1)) = 0;
	}

	//左边界
	i = 0;
	for (j = 0; j <= N; j++) {
		*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) =1;
		*(F + i + j*(N + 1)) = h*j;
	}

	//右边界
	i = N;
	for (j = 0; j <= N; j++) {
		*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) = 1;
		*(F + i + j*(N + 1)) = 1-h*j;
	}

	//F存储着包含边界点方程右端项
	//A存储着包含边界点的系数矩阵，由A，F可以求出所有点的函数值，矩阵维度为matrix_size

/********************************************************************************/
/*****************整合边界条件，去掉多余信息*************************************/
	//下边界
	int k = 0,r;
	j = 0;
	for (i = 0; i < N + 1; i++) {
		ni = i + j*(N + 1);
		//*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) = 1;
		//先遍历矩阵的ni列，找出该列中的非零元，记录非零元所在行号，用该行减去ni行的元素，对应的右端项F也进行这样的操作
		for (k = 0; k < matrix_size; k++) {//遍历列
			if (*(A + ni + k*matrix_size) != 0 && k!=ni){//非零元
				for (r = 0; r < matrix_size; r++) {//遍历行
					*(A + r + k*matrix_size) -= *(A + r + ni*matrix_size);//减去操作
				}
				*(F + k) -= *(F + ni);
			}
		}
		*(F + ni) = 0;
		*(A + ni + ni*matrix_size) = 0;
	}


	//上边界
	j = N;
	for (i = 0; i <= N; i++) {
		ni = i + j*(N + 1);
		//*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) = 1;
		//先遍历矩阵的ni列，找出该列中的非零元，记录非零元所在行号，用该行减去ni行的元素，对应的右端项F也进行这样的操作
		for (k = 0; k < matrix_size; k++) {//遍历列
			if (*(A + ni + k*matrix_size) != 0 && k != ni) {//非零元
				for (r = 0; r < matrix_size; r++) {//遍历行
					*(A + r + k*matrix_size) -= *(A + r + ni*matrix_size);//减去操作
				}
				*(F + k) -= *(F + ni);
			}
		}
		*(F + ni) = 0;
		*(A + ni + ni*matrix_size) = 0;
	}


	//左边界
	i = 0;
	for (j = 0; j <= N; j++) {
		ni = i + j*(N + 1);
		//*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) = 1;
		//先遍历矩阵的ni列，找出该列中的非零元，记录非零元所在行号，用该行减去ni行的元素，对应的右端项F也进行这样的操作
		for (k = 0; k < matrix_size; k++) {//遍历列
			if (*(A + ni + k*matrix_size) != 0 && k != ni) {//非零元
				for (r = 0; r < matrix_size; r++) {//遍历行
					*(A + r + k*matrix_size) -= *(A + r + ni*matrix_size);//减去操作
				}
				*(F + k) -= *(F + ni);
			}
		}
		*(F + ni) = 0;
		*(A + ni + ni*matrix_size) = 0;
	}

	//右边界
	i = N;
	for (j = 0; j <= N; j++) {
		ni = i + j*(N + 1);
		//*(A + i + j*(N + 1) + (i + j*(N + 1))*matrix_size) = 1;
		//先遍历矩阵的ni列，找出该列中的非零元，记录非零元所在行号，用该行减去ni行的元素，对应的右端项F也进行这样的操作
		for (k = 0; k < matrix_size; k++) {//遍历列
			if (*(A + ni + k*matrix_size) != 0 && k != ni) {//非零元
				for (r = 0; r < matrix_size; r++) {//遍历行
					*(A + r + k*matrix_size) -= *(A + r + ni*matrix_size);//减去操作
				}
				*(F + k) -= *(F + ni);
			}
		}
		*(F + ni) = 0;
		*(A + ni + ni*matrix_size) = 0;
	}

	//for (i = 0; i < matrix_size; i++)
	//	printf("%lf\n", *(F + i));


	/************************去掉矩阵多余的零，得到最终只含有非边界点的方程的系数矩阵和右端项**************/
	double *AA, *FF;
	int var;//当前矩阵的行数和列数
	int *index;
	var = matrix_size - 2 * (N - 1) - 2 * (N + 1);
	AA = (double *)calloc(sizeof(double), matrix_size*var);
	FF = (double *)calloc(sizeof(double), var);
	index = (int*)calloc(sizeof(int), 2*(N-1)+2*(N + 1));//分配内存记录当前矩阵中零行的标号
	if (index == NULL) {
		printf("falied!!!");
	}
	//printf("var=%d\n", var);


	/*********************计算全零行的标号*****************************/
	//下边界
	j = 0;
	k = 0;
	for (i = 0;i < N + 1; i++){
		*(index + k) = i + j*(N + 1);
		k++;
	}

	//上边界
	j = N;
	for (i = 0; i <=N && k<2 * (N - 1) +2 * (N + 1); i++) {
		*(index + k) = i + j*(N + 1);
		k++;
	}

	//左边界
	i = 0;
	for (j = 1; j < N && k<2 * (N - 1) +2 * (N + 1); j++) {
		*(index + k) = i + j*(N + 1);
		k++;
	}

	//右边界
	i = N;
	for (j = 1; j < N && k<2 * (N - 1)  +2 * (N + 1); j++) {
		*(index + k) = i + j*(N + 1);
		k++;
	}

	//输出全零行标号
	/*for (i = 0; i < 2 * (N - 1) +2 * (N + 1); i++)
		printf("index=%d\n", *(index + i));*/

	//得到非全零行标号
	int *index_fei;
	index_fei = (int*)calloc(sizeof(int), var);
	for (i = 0, j = 0; j < matrix_size; j++) {
		for (k = 0; k < 2 * (N - 1)  +2 * (N + 1); k++) {
			if (j == *(index+k))break;
		}
		if (k >= 2 * (N - 1)  +2 * (N + 1)) {
			*(index_fei + i) = j;
			i++;
		}
	}
	////输出标号
	//for (i = 0; i < var; i++)
	//	printf("%d\n", *(index_fei + i));



	/***********************************去掉矩阵中全零行和全零列******/
	//去掉系数矩阵中零行
	for (i = 0; i < var; i++) {
		for (j = 0; j < matrix_size; j++) {
			*(AA + j + i*matrix_size) = *(A + j + (*(index_fei + i))*matrix_size);
		}
	}

	//输出系数矩阵
	//for (i = 0; i < var; i++) {
	//	for (j = 0; j < matrix_size; j++)
	//			printf("%lf ", *(AA + i*matrix_size + j));
	//	printf("\n");
	//}
		

	free(A);
	A = (double *)calloc(sizeof(double), var*var);
	//去掉系数矩阵中零列
	for (i = 0; i < var; i++) {
		for (j= 0; j<var; j++) {
			*(A + j + i*var) = *(AA + (*(index_fei+j)) + i*matrix_size);
		}
	}
	free(AA);

	////输出系数矩阵
	//for (i = 0; i < var; i++) {
	//	for (j = 0; j < var; j++)
	//		printf("%lf ", *(A + i*var + j));
	//	printf("\n");
	//}



	/***********************去掉右端项里面多余的零*****************************************/
	for (i = 0; i < var; i++) {
		*(FF + i) = *(F + *(index_fei + i));
	}
	free(F);

	
	

	//将数据输入到文件
	FILE *fp;
	fp = fopen("A.txt", "w+");
	if (fp == NULL)printf("the file open failed!!!");
	fprintf(fp,"%d  %d\n",var,var);
	for (i = 0; i<var; i++) {
		for (j = 0; j<var; j++) {
			fprintf(fp, "%lf    ", *(A + i + j*var));
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	fp = fopen("F.txt", "w+");
	if (fp == NULL)printf("the file open failed!!!");
	fprintf(fp,"%d  %d\n",var,1);
	for (i = 0; i<var; i++) {
		fprintf(fp, "%lf\n", *(FF + i));
	}

	// FF 存储着无边界点的右端项
	// A 存储着无边界点的系数矩阵，由A，FF可以求出非边界点的函数值，矩阵维度为var


	/***************************************************************************************/
	//实现多色排序
	int L = 1;//记录颜色种数
	int *c;//记录每个节点所分配的颜色号
	int *I;//所有节点组成的集合
	int *T;//已经标记过颜色的节点集合
	int num_T = 0;//记录数组T中包含的实际节点个数
	int adj_num = 0;//记录点kk的邻接点的个数
	int *t;//辅助数组
	int flag = 0;//set S=I\T
	int kk = 0;//select kk \in S
	int *adj;//记录kk节点的临接点

	//给变量分配内存，分配失败打印信息，每个变量多分配一个内存，第0个为站位，与所给的多色排序算法匹配变量
	c = (int *)calloc(sizeof(int), var + 1);
	I = (int *)calloc(sizeof(int), var + 1);
	T = (int *)calloc(sizeof(int), var + 1);
	t = (int *)calloc(sizeof(int), var + 1);
	adj = (int *)calloc(sizeof(int), var + 1);
	if (c == NULL || I == NULL || T == NULL || adj == NULL || t == NULL) {
		printf("failed!!!!");
	}


	//初始化
	for (i = 0; i < var + 1; i++) {
		*(c + i) = 0;
		*(I + i) = i;
		*(T + i) = 0;
		*(t + i) = 0;
		*(adj + i) = 0;
	}
	*(c + 1) = L;
	*(T + 1) = 1;

	num_T = 1;



	while (num_T <= var) {//终止条件为所有的节点都标记颜色，即num_T>var
		kk = 0;//kk为几何I-T中的任意一个元素
		for (i = 1; i < var + 1; i++) {
			flag = 0;
			for (j = 1; j < num_T + 1; j++) {
				if (*(I + i) == *(T + j)) {
					flag = 1;
					break;
				}
			}
			if (flag != 1) {
				kk = (*(I + i));
				//printf("kk=%d \n", kk);
				break;
			}
		}

		for (i = 1; i < var + 1; i++)*(adj + i) = 0;

		for (i = 1, k = 1; i <var + 1; i++) {//寻找kk点的临接点
			if (*(A + i - 1 + (kk - 1)*var) != 0) {
				*(adj + k) = i;
				adj_num = k - 1;
				k++;
			}
		}
		adj_num = adj_num + 1;

		//选取元素属于adj交上T
		for (i = 1; i <adj_num + 1; i++) {
			flag = 0;
			for (j = 1; j < num_T + 1; j++) {
				if (*(adj + i) == *(T + j)) {
					flag = 1;
					break;
				}
			}
			if (flag == 1) {
				*(t + *(c + *(adj + i))) = 1;
			}
		}

		/*for (i = 0; i < var; i++) {
			printf("%d ", *(adj + i));
		}
		printf("\n");
		for (i = 0; i < var; i++) {
			printf("%d ", *(T + i));
		}
		printf("\n");
		for (i = 0; i < var; i++) {
			printf("%d ", *(t + i));
		}
		printf("\n");*/


		for (j = 1; j <= var; j++) {
			if (*(t + j) == 0)break;
		}
		*(c + kk) = j;
		if (j == L + 1)L++;


		for (i = 1; i <adj_num + 1; i++) {
			flag = 0;
			for (j = 1; j < num_T + 1; j++) {
				if (*(adj + i) == *(T + j)) {
					*(t + *(c + *(adj + i))) = 0;
					break;
				}
			}
		}


		*(T + num_T) = kk;
		num_T++;
	}


	//将颜色编号输出到文件c.txt
	fp = fopen("c.txt", "w+");
	if (fp == NULL) {
		printf("failed!!!!");
		return -1;
	}
	printf("L=%d\n", L);
	for (i = 0; i < var+1; i++) {
		fprintf(fp,"%d\n", *(c + i));
	}
	fclose(fp);
	
	free(I);
	free(adj);
	free(t);
	free(T);


	//向量c存储了对应顶点的颜色编号，c第一个元素为无用元素
	//L存储对应颜色种类


	/*****************************************************************************************************************/
	//对矩阵A和F按颜色编号重新编号，得到新的系数矩阵和右端项

	int *index_c;//对顶点按颜色种类排序，定义新的编号
	int *num_c_point;//记录相同颜色的顶点个数
	int max_num_c_point = 0;//记录对于不同颜色对应的最大顶点数
	int min_num_c_point = 100000;//记录对于不同颜色对应的最小顶点数

	num_c_point = (int *)calloc(sizeof(int), L);
	index_c = (int *)calloc(sizeof(int), var);

	//初始化记录颜色的矩阵
	for (i = 0;i < L;i++) {
		*(num_c_point+i) = 0;
	}

	//统计并计算对于每种颜色的顶点个数
	k = 0;
	for (j = 1;j <= L;j++) {
		for (i = 1;i < var+1;i++) {
			if (*(c + i) == j) {
				*(index_c + k) =i-1;
				*(num_c_point + j-1) += 1;
				k++;
			}
		}
	}

	//将每种颜色的顶点数目输出到文件
	fp=fopen("number_c_point.txt","w+");
	if(fp==NULL)printf("failed to open file !!!\n");
	for(j=0;j<L;j++){
		fprintf(fp,"color%d  %d\n",j+1,*(num_c_point+j));
	}
	fclose(fp);
	fp=NULL;

	//统计最大顶点数和最小顶点数（对于不同颜色）
	for (i = 0; i < L; i++) {
		if (*(num_c_point + i) > max_num_c_point) {
			max_num_c_point = *(num_c_point);
		}
		if (*(num_c_point + i) < min_num_c_point) {
			min_num_c_point = *(num_c_point + i);
		}
	}

	printf("max_numn_c_points=%d\n", max_num_c_point);
	printf("min_num_c_points=%d\n", min_num_c_point);

	/*//输出结果
	for (i = 0;i < var + 1;i++)printf("%d  ", *(c + i));
	printf("\n");
	for (i = 0;i < var;i++)printf("%d  ", *(index_c + i));
	printf("\n");
	for (i = 0;i < L;i++)printf("%d  ", *(num_c_point + i));
	printf("\n");*/

	//对于多色排序来说仅仅是矩阵的一个变换，现在已经将对应的标号关系求得存储在index_c中，对于排序后的结果访问只需按对应标号索引即可

	//输出重新编号的结果
	/*for (i = 0;i < var;i++)printf("%lf ", *(FF + i));
	printf("\n");
	for (i = 0;i < var;i++)printf("%lf ", *(FF + *(index_c + i)));*/

	//将多色排序后的标号与原始的标号的对应关系输出到文件
	fp = fopen("sorted_number2_origin.txt", "w+");
	if (fp == NULL)printf("failled to open file!!!!!");
	fprintf(fp,"%d %d\n",var,2);
	for (i = 0; i < var; i++) {
		fprintf(fp, "%d  %d\n", i, *(index_c + i));
	}
	fclose(fp);

	//将多色排序后的右端项结果输出到文件（文本文件）
	fp = fopen("F_sorted.txt", "w+");
	if (fp == NULL)printf("failled to open file!!!!!");
	fprintf(fp,"%d  %d\n", var,1);
	for (i = 0; i < var; i++) {
		fprintf(fp,"%lf\n", *(FF + *(index_c + i)));
	}
	fclose(fp);



	F=(double *)calloc(sizeof(double),var*1);
	if (F== NULL)printf("failled to open file!!!!!");
	for (i = 0; i < var; i++) {
		*(F+i)=*(FF + *(index_c + i));
	}
	//二进制文件，忽视其中是的换行符、空格、回车符，实际为内存中对应的数据块
	int n_var=1;
	fp=fopen("F_sorted","wb+");
	if(fp==NULL)printf("failed to open file !!! \n");
	fwrite(&var,sizeof(int),1,fp);//在前两个整型字节处存储矩阵的维数
    fwrite(&n_var,sizeof(int),1,fp);
	fwrite(F,sizeof(double),var,fp);
	fclose(fp);
	free(F);

	//将多色排序后的系数矩阵输出到文件（文本文件）
	fp = fopen("A_sorted.txt", "w+");
	if (fp == NULL)printf("failed to open file!!!!!");
	fprintf(fp,"%d  %d\n",var,var);
	for (i = 0; i < var; i++) {
		for (j = 0; j < var; j++) {
			fprintf(fp, "%lf ", *(A + *(index_c + i)*var + *(index_c + j)));
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

    AA=(double *)calloc(sizeof(double),var*var);
	if (AA == NULL)printf("failed to open file!!!!!");
	for (i = 0; i < var; i++) {
		for (j = 0; j < var; j++) {
			*(AA+i*var+j)=*(A + *(index_c + i)*var + *(index_c + j));
		}
	}
	//二进制文件
	fp=fopen("A_sorted","wb+");
	if(fp==NULL)printf("failed to open file !!!\n");
	fwrite(&var,sizeof(int),1,fp);
    fwrite(&var,sizeof(int),1,fp);
	fwrite(AA,sizeof(double),var*var,fp);
	fclose(fp);
	free(AA);

	free(index_c);
	free(num_c_point);
	

	free(c);
	free(A);
	free(FF);

	return 0;
}
