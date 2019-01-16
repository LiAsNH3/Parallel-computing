/**
 * @file matrix_disolve.c
 * @author lsa
 * @brief 红黑迭代方法并行数据准备，将经过红黑排序后的矩阵
 * 按所给的进程数p分解，即将红色点部分划分为p部分，将黑色点
 * 部分划分为p部分，每个进程拥有两部分数据，每次迭代先计算红
 * 色部分，再计算黑色部分
 * 
 * 数据要求：
 * 每个矩阵文件的第一行为矩阵的维数
 * 红色顶点编号在前，黑色部分编号在后
 * 
 * @version 0.1
 * @date 2019-01-15
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


int main()
{
    FILE *fp_A,*fp_F;//读取系数矩阵和右端项的文件指针
    int i,j,k;//循环变量
    double *A,*F;//存储系数矩阵和右端项的内存指针
    int m_A,n_A;//系数矩阵维数
    int m_F,n_F;//右端项维数
    int p_num;//进程数
    int c[2]={8,8};//红色和黑色顶点数
    int *read_init;//存储每个进程读取红和黑色顶点在全矩阵中的行数起始
    int *read_num;//存储每个进程读取红和黑色顶点在全矩阵中的行数
    char filename[50]="A0.txt";

    //获得进程数和红黑两种颜色的标号数量
    printf("please enter the number of process:");
    scanf("%d",&p_num);
    printf("please enter the number of red and black points(red black):");
    scanf("%d%d",&c[0],&c[1]);

    //读取系数矩阵
    fp_A=fopen("A_sorted.txt","r+");
    if(fp_A==NULL)printf("failed to open file !!!\n");
    fscanf(fp_A,"%d%d",&m_A,&n_A);
    A=(double *)calloc(sizeof(double),m_A*n_A);
    for(i=0;i<m_A;i++){
        for(j=0;j<n_A;j++){
            fscanf(fp_A,"%lf",A+i*n_A+j);
        }
    }
    fclose(fp_A);
    
    //读取右端项
    fp_F=fopen("F_sorted.txt","r+");
    if(fp_F==NULL)printf("failed to open file !!!");
    fscanf(fp_F,"%d%d",&m_F,&n_F);
    F=(double *)calloc(sizeof(double),m_F*n_F);
    for(i=0;i<m_F;i++){
        fscanf(fp_F,"%lf",F+i);
    }
    fclose(fp_F);

    // printf("m_F=%d,n_F=%d\n",m_F,n_F);
    // for(i=0;i<m_F;i++)printf("%lf\n",*(F+i));


/*****************计算每个进程需要读取的行数************************/

    read_num=(int *)calloc(sizeof(int),p_num*2); 

    //对于红色顶点需要读取的行数
    for(i=0;i<p_num;i++){
        if(c[0]%p_num==0){
            *(read_num+i*2+0)=c[0]/p_num;
        }
        else{
            if(i<c[0]%p_num){
                *(read_num+i*2+0)=c[0]/p_num+1;
            }
            else{
                *(read_num+i*2+0)=c[0]/p_num;
            }
        }
    }

    //对于黑色顶点需要读取的行数
    for(i=0;i<p_num;i++){
        if(c[1]%p_num==0){
            *(read_num+i*2+1)=c[1]/p_num;
        }
        else{
            if(i<c[1]%p_num){
                *(read_num+i*2+1)=c[1]/p_num+1;
            }
            else{
                *(read_num+i*2+1)=c[1]/p_num;
            }
        }
    }

    // for(i=0;i<p_num;i++){
    //     printf("%d  %d\n",*(read_num+i*2+0),*(read_num+i*2+1));
    // }

/****************计算每个进程读取的起始行数在全矩阵中*********************/
    read_init=(int *)calloc(sizeof(int),p_num*2);
    
    //计算红色部分顶点，每个进程的读取起始行数
    int zj1;
    *(read_init+0*2+0)=0;//0进程读取红色部分从第0行开始
    for(i=1;i<p_num;i++){
        zj1=0;
        for(j=0;j<i;j++){
            zj1=zj1+*(read_num+j*2+0);
        }
        *(read_init+i*2+0)=zj1;
    }

    //计算黑色部分顶点，每个进程的读取起始行数
    int zj2;
    *(read_init+0*2+1)=c[0];//0进程读取黑色部分从黑色顶点编号开始
    for(i=1;i<p_num;i++){
        zj2=0;
        for(j=0;j<i;j++){
            zj2=zj2+*(read_num+j*2+1);
        }
        *(read_init+i*2+1)=zj2+c[0];
    }

    // printf("\n");
    //  for(i=0;i<p_num;i++){
    //     printf("%d  %d\n",*(read_init+i*2+0),*(read_init+i*2+1));
    // }

/*****************将系数矩阵按所给进程数进行划分********************************/

    FILE *fp;
    for(k=0;k<p_num;k++){
        sprintf(filename,"%c%d.txt",'A',k);
        fp=fopen(filename,"w+");
        fprintf(fp,"%d %d\n",*(read_num+k*2+0)+*(read_num+k*2+1),n_A);
        //红色部分分块
        for(i=*(read_init+k*2+0);i<*(read_init+k*2+0)+*(read_num+k*2+0);i++){
            for(j=0;j<n_A;j++){
                fprintf(fp,"%lf ",*(A+i*n_A+j));
            }
            fprintf(fp,"\n");
        }

        //黑色部分
        for(i=*(read_init+k*2+1);i<*(read_init+k*2+1)+*(read_num+k*2+1);i++){
            for(j=0;j<n_A;j++){
                fprintf(fp,"%lf ",*(A+i*n_A+j));
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
    }


/*****************将右端项按所给进程数进行划分********************************/

    for(k=0;k<p_num;k++){
        sprintf(filename,"%c%d.txt",'F',k);
        fp=fopen(filename,"w+");
        fprintf(fp,"%d %d\n",*(read_num+k*2+0)+*(read_num+k*2+1),1);
        //红色部分分块
        for(i=*(read_init+k*2+0);i<*(read_init+k*2+0)+*(read_num+k*2+0);i++){
                fprintf(fp,"%lf\n",*(F+i));
        }

        //黑色部分
        for(i=*(read_init+k*2+1);i<*(read_init+k*2+1)+*(read_num+k*2+1);i++){
            fprintf(fp,"%lf\n",*(F+i));
        }
        fclose(fp);
    }
    free(A);
    free(F);
    free(read_init);
    free(read_num);
    return 0;
}