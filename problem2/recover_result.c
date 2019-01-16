/**
 * @file recover_result.c
 * @author lsa
 * @brief 本程序是将通过红黑迭代计算后的数值结果
 * 恢复到区域网格的顶点标号对应的结果
 * @version 0.1
 * @date 2019-01-16
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
    int i;//循环变量
    double *u;//存储恢复后的数值结果
    int m,n;//解向量的个数
    int *index;//存储对应规则
    FILE *fp1,*fp2;

    fp1=fopen("sorted_number2_origin.txt","r+");
    if(fp1==NULL)printf("failed to open file !!!\n");
    fscanf(fp1,"%d%d",&m,&n);
    index=(int *)calloc(sizeof(int),m);
    if(index==NULL)printf("failed to calloc !!!\n");
    for(i=0;i<m;i++){
        fscanf(fp1,"%d%d",&n,index+i);
    }
    fclose(fp1);
    fp2=fopen("u_sorted.txt","r+");
    if(fp2==NULL)printf("failed to open file !!!");
    fscanf(fp2,"%d%d",&m,&n);
    u=(double *)calloc(sizeof(double),m);
    if(u==NULL)printf("failed to calloc !!!");
    for(i=0;i<m;i++){
        fscanf(fp2,"%lf",u+*(index+i));
    }
    fclose(fp2);
    fp1=fopen("u.txt","w+");
    if(fp1==NULL)printf("failed to open file !!!");
    for(i=0;i<m;i++){
        fprintf(fp1,"%lf\n",*(u+i));
    }
    free(u);
    u=NULL;
    free(index);
    index=NULL;
    return 0;
}