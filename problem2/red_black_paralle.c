/**
 * @file red_block_paralle.c
 * @author lsa
 * @brief 利用红黑排序对原始方程的系数矩阵重组，在用SOR迭代方法
 * 迭代计算方程的数值解，本程序主要实现红黑排序后的方程的并行SOR
 * 迭代计算
 * @version 0.1
 * @date 2019-01-14
 * 
 */

#include<stdlib.h>
#include<stdio.h>
#include "mpi.h"

int main(int argc,char *argv[])
{
    int p_num;//进程总数
    int rank;//当前进程的进程编号
    int i,j,k;//循环变量
    int n_A,m_A;//系数矩阵的维数信息
    int n_A_part,m_A_part;//当前进程的部分系数矩阵的维数
    int n_F,m_F;//右端项的维数信息，m_F=1
    int n_F_part,m_F_part;//右端项部分的维数信息，m_F_part=1
    int c[2]={8,8};//记录红色和黑色两种颜色的格点数目
    int *read_init;//存储每个进程读取红和黑色顶点在全矩阵中的行数起始
    int *read_num;//存储每个进程读取红和黑色顶点在全矩阵中的行数
    char filename[50]="A0.txt";
    double *A_part,*F_part;//当前进程的局部系数矩阵和向量存储内存指针
    double *x_full;//解向量迭代量，与解向量维数一致
    double wall_time_first = 0, wall_time_end = 0;
    FILE *fp_A_part,*fp_F_part;
    MPI_Status status;
    MPI_Init(&argc,&argv);//并行进程启动

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);//获取当前进程的编号
    MPI_Comm_size(MPI_COMM_WORLD,&p_num);//获取进程的总数

/**************每个进程读取对应的系数矩阵******************************/

    //读取对应进程的系数矩阵
    sprintf(filename,"%c%d.txt",'A',rank);
    fp_A_part=fopen(filename,"r+");
    if(fp_A_part==NULL)printf("myrank=%d,failed to open file !!!",rank);
    fscanf(fp_A_part,"%d%d",&m_A_part,&n_A_part);//获取部分系数矩阵的维数

    //获取全局系数矩阵的维数
    n_A=n_A_part;
    MPI_Allreduce(&m_A_part,&m_A,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    //读取当前进程对应的部分系数矩阵
    A_part=(double *)calloc(sizeof(double),m_A_part*n_A_part);
    if(A_part==NULL)printf("myrank=%d,failed to calloc !!!",rank);
    for(i=0;i<m_A_part;i++){
        for(j=0;j<n_A_part;j++){
            fscanf(fp_A_part,"%lf",A_part+i*n_A_part+j);
        }
    }
    fclose(fp_A_part);

    //输出局部系数矩阵
    // if(rank==0){
    //     for(i=0;i<m_A_part;i++){
    //         for(j=0;j<n_A_part;j++){
    //             printf("%lf ",*(A_part+i*n_A_part+j));
    //         }
    //         printf("\n");
    //     }
    // }

/**************每个进程读取对应的右端项******************************/

    sprintf(filename,"%c%d.txt",'F',rank);
    fp_F_part=fopen(filename,"r+");
    if(fp_A_part==NULL)printf("myrank=%d,failed to open file !!!",rank);
    fscanf(fp_F_part,"%d%d",&m_F_part,&n_F_part); 

    m_F=n_A;
    n_F=1;

    F_part=(double *)calloc(sizeof(double),m_F_part*n_F_part);
    if(F_part==NULL)printf("myrank=%d,failed to calloc !!!",rank);

    for(i=0;i<m_F_part;i++){
        fscanf(fp_F_part,"%lf",F_part+i);
    }
    fclose(fp_F_part);

    //输出局部右端项
    // if(rank==0){
    //     for(i=0;i<m_F_part;i++){
    //         printf("%lf ",*(F_part+i));
    //     }
    // }

/**************每个进程初始化对应的迭代解向量*************************/

    x_full=(double *)calloc(sizeof(double),n_A_part*1);
    for(i=0;i<n_A_part;i++){
        *(x_full+i)=0;
    }

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

/*****************红黑迭代主程序***************************/

    double *red_F,*black_F,*x;
    x=(double *)calloc(sizeof(double),n_A_part);
    red_F=(double *)calloc(sizeof(double),*(read_num+rank*2+0));
    black_F=(double *)calloc(sizeof(double),*(read_num+rank*2+1));
    if(red_F==NULL || black_F==NULL|| x==NULL)printf("myrank=%d,failed to calloc !!!",rank);

    wall_time_first = MPI_Wtime();
    for(k=0;k<1000;k++){
        //先用前一步的黑色顶点的值计算红色顶点对应的值
        for(i=0;i<*(read_num+rank*2+0);i++){//先算出红色顶点对应的分块系数矩阵和迭代解向量的乘积
            *(red_F+i)=0;
            for(j=c[0];j<n_A_part;j++){
                *(red_F+i)=*(red_F+i)+*(A_part+i*n_A_part+j)*(*(x_full+j));
            }
        }

        //将计算的结果对应到迭代解向量x上
        for(i=0;i<*(read_num+rank*2+0);i++){
            *(x_full+*(read_init+rank*2+0)+i)=(*(F_part+i)-*(red_F+i))/(-4);
        }

        //进行归约操作，将每个进程上的计算的红色顶点的值收集起来，再广播给每个进程（顺序很重要）
        if(rank!=0){
            MPI_Send(x_full+*(read_init+rank*2+0),*(read_num+rank*2+0),MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
        }
        if(rank==0){
            for(i=1;i<p_num;i++){
                MPI_Recv(x_full+*(read_init+i*2+0),*(read_num+i*2+0),MPI_DOUBLE,i,i,MPI_COMM_WORLD,&status);
            }
        }
        //MPI_Gather(x_full+*(read_init+rank*2+0),*(read_num+rank*2+0),MPI_DOUBLE,x,n_A_part,MPI_DOUBLE,0,MPI_COMM_WORLD);

        MPI_Bcast(x_full,n_A_part,MPI_DOUBLE,0,MPI_COMM_WORLD);


        //利用新算出的红色顶点的值计算黑色顶点的值
        for(i=0;i<*(read_num+rank*2+1);i++){//先算出黑色顶点对应的分块系数矩阵和迭代解向量的乘积
            *(black_F+i)=0;
            for(j=0;j<c[0];j++){
                *(black_F+i)=*(black_F+i)+*(A_part+(i+*(read_num+rank*2+0))*n_A_part+j)*(*(x_full+j));
            }
            *(black_F+i)=*(F_part+i+*(read_num+rank*2+0))-*(black_F+i);
        }

        //将计算的结果对应到迭代解向量x上
        for(i=0;i<*(read_num+rank*2+1);i++){
            *(x_full+*(read_init+rank*2+1)+i)=*(black_F+i)/(-4);
        }

        //进行归约操作，将每个进程上的计算的红色顶点的值收集起来，再广播给每个进程（顺序很重要）
        if(rank!=0){
            MPI_Send(x_full+*(read_init+rank*2+1),*(read_num+rank*2+1),MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
        }
        if(rank==0){
            for(i=1;i<p_num;i++){
                MPI_Recv(x_full+*(read_init+i*2+1),*(read_num+i*2+1),MPI_DOUBLE,i,i,MPI_COMM_WORLD,&status);
            }
        }
        MPI_Bcast(x_full,n_A_part,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }
    wall_time_end = MPI_Wtime();

    if(rank==0){
        FILE *fp;
        fp=fopen("u_sorted.txt","w+");
        if(fp==NULL)printf("failed to open file !!!");
        fprintf(fp,"%d %d\n",n_A_part,1);
        for(i=0;i<n_A_part;i++)
        fprintf(fp,"%lf\n",*(x_full+i));
        //printf("k=%d\n",k);
    }

    free(x);
    free(red_F);
    free(black_F);
    free(A_part);
    free(F_part);
    free(x_full);
    free(read_init);
    free(read_num);

    double wall_time, max_wall_time;
	wall_time = wall_time_end - wall_time_first;
	MPI_Reduce(&wall_time, &max_wall_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if(rank==0){
        printf("max wall time:%d %.12lf\n",rank, wall_time_end - wall_time_first);
    }

    MPI_Finalize();
}