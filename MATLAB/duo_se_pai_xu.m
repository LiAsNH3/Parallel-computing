function [B,M,L]=duo_se_pai_xu(A,F)
%多色排序矩阵，颜色种数为自适应的，所输入的矩阵必须为对角阵
%返回的矩阵B为对输入矩阵A多色排序后的结果
%返回的向量M是对F多色排序的结果
%L为颜色种数
n=length(F);

if A~=A'
    warning('Matrix A is not a symmetric matrix!!!');
end
% A=diag(zeros(n,1))+diag(ones(n-1,1),-1)+diag(ones(n-1,1),1)+diag(ones(n-2,1),2)...
%  +diag(ones(n-2,1),-2)+diag(ones(n-3,1),-3)...
%      +diag(ones(n-3,1),3)++diag(ones(n-4,1),4)++diag(ones(n-4,1),-4);
% F=1:n;

L=1;
index=zeros(n,1);%为0时属于I，为1时属于T
index(1)=1;
c=zeros(n,1);
c(1)=L;
t=zeros(n,1);

while ~all(index==1)
    s=find(index==0);
    k=s(1)
    adjk=find(A(k,:)~=0);
    for j=1:length(adjk)
        if index(adjk(j))==1
            t(c(adjk(j)))=1;
        end
    end
    for j=1:n
        if t(j)==0
            break;
        end
    end

    adjk
   
    c(k)=j;
    if j==L+1
        L=L+1;
    end
    for j=1:length(adjk)
        if index(adjk(j))==1
            t(c(adjk(j)))=0;
        end
    end
    index(k)=1;
end

%L为颜色种数
j=1;
index1=zeros(n,1);%颜色相同的顺序排列
for i=1:L
   in=find(c==i);
   index1(j:j+length(in)-1)=in;
   j=j+length(in);
end

B=zeros(n,n);
M=zeros(n,1);
for i=1:n
    M(i)=F(index1(i));
    for j=1:n
       B(i,j)=A(index1(i),index1(j)); 
    end
end
end