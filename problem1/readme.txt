// -*- encoding: utf8 -*-
并行计算矩阵乘向量
 \f[Ax=b\f]
 大体思想为将给定的矩阵按列循环划分，再将向量对应划分，最后归约求和到０进程，输出到结果到文件。

 该程序包实现流程为：
 １．先随机生成给定维数的矩阵和向量；
 ２．在将矩阵和向量按给定的进程数进行划分；
 ３．实现并行串行的矩阵乘以向量；

 文件说明：
matrix_generate.c 　随机生成矩阵和向量的函数
matrix_disove.c 　　按给定的进程数对矩阵和向量进行划分
C_problem1.c　　　　矩阵乘以向量的串行程序
matrix_time_vector.c　矩阵乘以向量的并行程序

 程序编译
gcc -o  mg matrix_generate.c 
gcc -o md matrix_disove.c 
gcc -o cp1 C_problem1.c
mpicc -o  mtv matrix_time_vector.c

以８个进程，1000维的矩阵为例给出运行示例
生成矩阵和向量
./mg
1000
#分块系数矩阵
./md
1  8
A.txt
#分块列向量
/md
8 1
x.txt
＃并行程序运行
mpirun -np 8 mtv
#串行程序运行
./cp1

注：其中的A.txt x.txt是在程序中给定的文件名，若要修改在matrix_generate.c文件中；
　　串行和并行程序的墙上时间都将打印到屏幕。