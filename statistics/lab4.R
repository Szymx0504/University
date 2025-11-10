# TASK 1
n = 30
p = 0.3
rbinom(1, n, p) # less than 30/2=15

p = 0.5
rbinom(1, n, p) # moved to the middle ~15

n = 30
p = 0.7
set.seed(42)
rbinom(1, n, p) # bigger number of successes

# TASK 2
library("arm")
n = 30
p = 0.3
nums = rbinom(20, n, p)
discrete.histogram(nums, freq=F, main=paste("p =", p)) # 'line graph'

par(mfrow=c(2, 3))
for(i in 0:5){
  discrete.histogram(rbinom(20, n, p+(0.1*i)), freq=F, main=paste("p =", p+0.1*i), xlim=c(0,30))
}

# TASK 3
# S - number of wells that are contaminated
# S~Bin(n,p)
# p - probability that well is contaminated
# n - sample size (5)
p = 0.3
n = 5
x = 0:5
prob = dbinom(x, n, p)
rbind(x, prob)
#a)
#P(S==3)
prob[x==3]
#b)
#P(S>=3)
sum(prob[x>=3])
#c)
#P(S<3)
sum(prob[x<3])
sum(prob[x<=2])

# TASK 4
# B denotes the number of bulbs which burn for at least 500 hours
# B~bin(n,p)
# n - sample size
# p - probability that a bulb will burn for at least 500 hours
n = 8
p = 0.9
B = 0:8

prob = dbinom(B, n, p)

#P(B==8)
prob[B==8]
#P(B==7)
prob[B==7]
#P(B>5)
sum(prob[B>5])
#E(B)
expect = sum(prob * b); expect
n * p
# SD(B)
variance = sum(b^2 * prob) - expect^2
std = sqrt(variance)
std

# TASK 5
# X - number of power cell failures
# X~exp(lamda)
lambda = 0.01
par(mfrow=c(1,1))
curve(dexp(x,lambda), xlim=c(0, 500))
#a) P(X>200) = 1-P(X<200) = 1-F(200) # < and <= don't matter as it's a continuous variable
1 - pexp(200, lambda)
#b) P(X<100)=F(100)
pexp(100, lambda)
#c) P(X<500)=F(500)
pexp(500, lambda)

# task 6 home