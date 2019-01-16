#该程序集合是用红黑迭代方法计算求解Possion方程
$\frac{\partial^2 u}{\partial x^2}+\frac{\partial^2 u}{\partial y^2}=xy,(x,y)\in (0,1)\times(0,1)$
$u(0,y)=y,u(1,y)=1-y,u(x,0)=0,u(x,1)=0$
的五点差分格式对应的线性方程组；分别采用串行程序和并行两种方式对
线性方程组进行求解。

#文件说明
matrix_generate.c 生成possion方程对应的系数矩阵和右端项、红黑排序后的系数矩阵和向量、两者的对应规则以及其他的信息
matrix_disolve.c  按进程数对生成的红黑排序后的矩阵和向量进行分解
C_SOR.c  红黑迭代方法的串行实现
red_black_paralle.c 红黑迭代方法的并行实现
recover_result.c 结果转化函数，将红黑迭代的结果转为标准网格上的结果


#程序编译
gcc -o mg matrix_generate.c
gcc -o md matrix_disolve.c 
gcc -o cp2 C_SOR.c 
gcc -o rec recover_result.c 
mpicc -o rbp red_black_paralle.c 


#程序运行示例
以将对应的区域划分步长为1/4，进程数为3为例
1.运行矩阵生成函数
./mg
输入参数
please enter the number of mesh(x equal to y):5
（注：此处的参数输入是适合C语言的数据结构的；要求对每行或每列从0开始编号，一直到最后的顶点的编号为此处输入参数）
2.运行矩阵分解程序
./md
输入参数
please enter the number of process:3
please enter the number of red and black points(red black):8 8
（注：此处输入的数据可以在运行矩阵生成函数后在文件number_c_point.txt中找到）
3.运行串行程序
./cp2
please enter the number of red points and black points:8 8
4.运行并行程序
mpirun -np 3 rbp 
（注：此处无数据输入，但是在运行此处的并行程序时，需要将red_black_paralle.c中
第25行int c[2]={8,8};中的数据改为number_c_point.txt中对应的数据）
5.结果转化
./rec 


#notice
本程序只针对前述的方程进行计算，对于其他的方程需要修改matrix_generate.c中的数据；
本程序也仅适用红黑排序这种情形，对于多色排序不在适用；
本程序无自适应迭代控制，只是通过迭代次数来控制程序的迭代次数，若要修改程序的迭代次数
需要修改文件red_black_paralle.c中170行中for语句中控制变量的取值即可。
