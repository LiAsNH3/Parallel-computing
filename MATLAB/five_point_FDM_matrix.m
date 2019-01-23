N=5;
h=1/(N-1);
A=zeros(N*N,N*N);
F=zeros(N*N,1);
for i=2:N-1
    for j=2:N-1
      A(i+(j-1)*N,i+(j-1)*N)=-4;
      A(i+(j-1)*N,i+(j)*N)=1;
      A(i+(j-1)*N,i+(j-2)*N)=1;
      A(i+(j-1)*N,i+1+(j-1)*N)=1;
      A(i+(j-1)*N,i-1+(j-1)*N)=1;
      F(i+(j-1)*N,1)=h*h*h*(i-1)*h*(j-1);
    end
end

%施加边界条件
i=1;
for j=1:N
    A(i+(j-1)*N,:)=zeros(1,N*N);
    A(i+(j-1)*N,i+(j-1)*N)=1;
    F(i+(j-1)*N,1)=h*(j-1);
end

i=N;
for j=1:N
    A(i+(j-1)*N,:)=zeros(1,N*N);
    A(i+(j-1)*N,i+(j-1)*N)=1;
    F(i+(j-1)*N,1)=1-h*(j-1);
end

j=N;
for i=1:N
    A(i+(j-1)*N,:)=zeros(1,N*N);
    A(i+(j-1)*N,i+(j-1)*N)=1;
    F(i+(j-1)*N,1)=0;
end

j=1;
for i=1:N
    A(i+(j-1)*N,:)=zeros(1,N*N);
    A(i+(j-1)*N,i+(j-1)*N)=1;
    F(i+(j-1)*N,1)=0;
end

y=A\F;
u=reshape(y,N,N);
mesh(linspace(0,1,N),linspace(0,1,N),u)


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%去掉多余的信息，通过做行变换，去掉边界项的方程
i=1;
for j=1:N
    index=find(A(:,i+(j-1)*N)~=0);
    for k=1:length(index)
       if index(k)~=i+(j-1)*N
          A(index(k),:)=A(index(k),:)-A(i+(j-1)*N,:); 
          F(index(k),1)=F(index(k),1)-F(i+(j-1)*N,1);
       end
    end
    A(i+(j-1)*N,i+(j-1)*N)=0;
    F(i+(j-1)*N,1)=0;
end


i=N;
for j=1:N
    index=find(A(:,i+(j-1)*N)~=0);
    for k=1:length(index)
       if index(k)~=i+(j-1)*N
          A(index(k),:)=A(index(k),:)-A(i+(j-1)*N,:); 
          F(index(k),1)=F(index(k),1)-F(i+(j-1)*N,1);
       end
    end
    A(i+(j-1)*N,i+(j-1)*N)=0;
    F(i+(j-1)*N,1)=0;
end

j=N;
for i=1:N
    index=find(A(:,i+(j-1)*N)~=0);
    for k=1:length(index)
       if index(k)~=i+(j-1)*N
          A(index(k),:)=A(index(k),:)-A(i+(j-1)*N,:); 
          F(index(k),1)=F(index(k),1)-F(i+(j-1)*N,1);
       end
    end
    A(i+(j-1)*N,i+(j-1)*N)=0;
    F(i+(j-1)*N,1)=0;
end

j=1;
for i=1:N
    index=find(A(:,i+(j-1)*N)~=0);
    for k=1:length(index)
       if index(k)~=i+(j-1)*N
          A(index(k),:)=A(index(k),:)-A(i+(j-1)*N,:); 
          F(index(k),1)=F(index(k),1)-F(i+(j-1)*N,1);
       end
    end
    A(i+(j-1)*N,i+(j-1)*N)=0;
    F(i+(j-1)*N,1)=0;
end

A(all(A==0,2),:) = [];
A(:,all(A==0,1)) = [];
F=F(find(F~=0));

%恢复边界条件
uu=reshape(A\F,N-2,N-2);
u2=zeros(N,N);
u2(2:end-1,2:end-1)=uu;
for j=1:N
   u2(1,j)=h*(j-1); 
   u2(N,j)=1-h*(j-1);
end

figure(2)
mesh(linspace(0,1,N),linspace(0,1,N),u2)
