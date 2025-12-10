therapy=c(8,12,26,10,23,21,16,22,18,17,36,9)
mean(therapy)
#the mean time to recover by ALL patients using the therapy is estimated to be 18h10min
alpha=0.05
n=length(therapy)
s=sd(therapy)
xbar=mean(therapy)
L=xbar-qt(1-alpha/2,n-1)*s/sqrt(n)
U=xbar+qt(1-alpha/2,n-1)*s/sqrt(n)
L
U
U-L
# we are 95% confident that the interval from 13th to 24th covers the true mean time spent by ALL patients under recovery 

For L 0.005
For U 0.045
Replace 1-alpha with this, you get -11

100% sure (almost: alpha 0.01), but then U-L 14.5441 - too wide (useless)
ttt = t.test(therapy
ttt$conf.int