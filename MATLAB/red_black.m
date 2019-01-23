%红黑迭代法
[B,M,L]=duo_se_pai_xu(A,F);
c=[1201,1200];%记录每种颜色的顶点数
x=zeros(length(M),1);
for k=1:1000
   for i=1:c(1)
       zj=0;
       for j=c(1)+1:length(M)
           zj=zj+B(i,j)*x(j);
       end
       x(i)=(M(i)-zj)/(B(i,i));
   end
   for i=c(1)+1:length(M)
      zj=0;
      for j=1:c(1)
          zj=zj+B(i,j)*x(j);
      end
      x(i)=(M(i)-zj)/(B(i,i));
   end
end
xx=B\M;