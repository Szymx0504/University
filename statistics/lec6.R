therapy=c(8,12,26,10,23,21,16,22,18,17,36,9)
xbar=mean(therapy)
n=length(therapy)

alpha=0.05
ssq=var(therapy)
s=sd(therapy)
The standard deviation of time under the therapy by ALL patients is estimated for 8h7min

L=(n-1)*ssq/qchisq(1-alpha/2,n-1)
U=(n-1)ssq/qchisq(alpha/2,n-1)

sqrt(L)
sqrt(U)
We are 95% confident that the interval from 5h to 14h covers the true standard deviation of time spent under the therapy by ALL patients

TeachingDemos package
sss=sigma.test(therapy)
sss$conf.int

n=150
t=70
phat=t/n

the estimated proportion of ALL students in favour of a new dress code is 46.7%
alpha=0.1
L=phat-qnorm(1-alpha/2)*sqrt(phat*(1-phat)/n)
U=phat+qnorm(1-alpha/2)*sqrt(phat*(1-phat)/n)
L
U
We are 90% confident that true proportion of ALL students in favour of a new dress code is between 39% and 54%
If we change alpha to 0.01, we get 36% and 57%
  
bbb=binom.test(t,n, conf.level=0.9)
bbb$conf.int
ppp=prop.test(t,n,conf.level=0.9)
ppp$conf.int

Example 3:
  conf.int from 43% to 49%, phat=0.46
0.46-1.98*sqrt(0.46*0.54/n)=0.43

0.46*0.54*1.96**2