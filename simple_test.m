D=importdata('s1.txt');
plot(D(:,1),D(:,2),'k.'); 
hold on;
tic
C=MT(D', 0.05, -1e5);
toc

plot(C(:,1),C(:,2),'*g')

