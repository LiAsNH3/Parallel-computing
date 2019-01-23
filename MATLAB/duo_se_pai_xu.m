function [B,M,L]=duo_se_pai_xu(A,F)
%��ɫ���������ɫ����Ϊ����Ӧ�ģ�������ľ������Ϊ�Խ���
%���صľ���BΪ���������A��ɫ�����Ľ��
%���ص�����M�Ƕ�F��ɫ����Ľ��
%LΪ��ɫ����
n=length(F);

if A~=A'
    warning('Matrix A is not a symmetric matrix!!!');
end
% A=diag(zeros(n,1))+diag(ones(n-1,1),-1)+diag(ones(n-1,1),1)+diag(ones(n-2,1),2)...
%  +diag(ones(n-2,1),-2)+diag(ones(n-3,1),-3)...
%      +diag(ones(n-3,1),3)++diag(ones(n-4,1),4)++diag(ones(n-4,1),-4);
% F=1:n;

L=1;
index=zeros(n,1);%Ϊ0ʱ����I��Ϊ1ʱ����T
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

%LΪ��ɫ����
j=1;
index1=zeros(n,1);%��ɫ��ͬ��˳������
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