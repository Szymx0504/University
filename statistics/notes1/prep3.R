n = 10
p = 0.6
probs = dbinom(0:n, n, p)
plot(probs, type="l")

x = 0:n
x>=2
which(x>=2)
sum(probs[x>=2])

exp(1)
y = pexp(x, 0.25) # cumulative sum
plot(x, y, type="l")
plot(x, dexp(x, 0.25), type="l") # prob at a given point

plot(0:180, pbinom(0:180, 180, 0.5))

# P(X>30.45)=1-F(30.45)
mu=30; sig=0.2
1-pnorm(30.45, mu, sig)

# TASKS

# task 1
# rbinom() counts successes
n=30;p=0.3
rbinom(1, n, p) #typically <10
p=0.5
rbinom(1, n, p) # ~15
p=0.7
rbinom(1, n, p) # typically >20

#task 2
library("arm")
n=30; p=0.3
successes = rbinom(20, n, p)
#hist(successes, prob=T, breaks=seq(0,30,1))
title = paste0("p = ", p)
#IMPORTANT: so called line graph in questions
discrete.histogram(successes, main=title)

par(mfrow=c(2,3))
n=30
for(p in seq(0.3, 0.7, 0.1)){
  successes = rbinom(20, n, p)
  title = paste0("p = ", p)
  discrete.histogram(successes, freq=F, main=title, xlim=c(0,30))
}

# task 3
# S - number of wells contaminated
# S ~ bin(n=5, p=0.3)
# p - probability well is contaminated
# n - sample size (5)
n=5; p=0.3
#a)
# P(S==3)
dbinom(3, n, p)
#b) P(S>=3)
1 - pbinom(2, n, p)
#c) P(S<3)
pbinom(2, n, p)

# OR (works only for discrete cases (bin distribution))
S = 0:5
probs = dbinom(S, n, p)
probs
prob[S==3]
sum(probs[S>=3])
sum(probs[S<3])

# task 4
# p - probability that a fluorescent bulb burns for at least 500 hours
# B - the number of bulbs which burn for at least 500 hours
# B ~ bin(n=8, p=0.9)
# n - sample size (8)
p=0.9; n = 8
B = 0:8
probs = dbinom(B, n, p)
#a) P(B==8)
probs[B==8]
#b) P(B==7)
probs[B==7]
#c) P(B>5)
sum(probs[B>5])
#d) E(B)
sum(probs*B) # n*p
#e) SD(B)
variance = sum(probs*B^2) - sum(probs*B)^2
sqrt(variance) # sqrt(n*p*(1-p))
#!IMPORTANT!
# std=sqrt(n*p*(1-p)) for bin distribution!

# task 5
# X - number of days a power cell is functioning
# X ~ exp(lambda=0.01)
par(mfrow=c(1,1))
lambda = 0.01
n = 2
x=0:500
curve(dexp(x, lambda), xlim=c(0,500))
# <= and  < don't matter here as it's continuous
# !IMPORTANT! DISTINCT BETWEEN CONTINUOUS AND DISCRETE DIST
# WHETHER TO SUBTRACT 1 OR NOT
#a) P(X>=200) = 1 - P(X<200) = 1 - F(200)
1 - pexp(200, lambda)
#b) P(X<=100) = F(100)
pexp(100, lambda)
#c) P(X<500) = F(500)
pexp(500, lambda)

# task 6
# X - magnitude of earthquake recorded in a region of North America
# X ~ exp(lambda=1/2.4)
mu = 2.4
#IMPORTANT lambda from mean
lambda = 1/mu
#a) P(X>3) = 1 - P(X<=3) = 1 - F(3)
1 - pexp(3, lambda)
#b) P(2<=X<=3) = P(X<=3) - P(X<2) = F(3) - F(2)
pexp(3, lambda) - pexp(2, lambda)

f = function(x){lambda*exp(-lambda*x)}
integrate(f, 3, Inf)
integrate(f, 2, 3)

# for continuous random variables, the expectation is defined as:
# E(X) = integral(x * f(x))
f2 = function(x){lambda*exp(-lambda*x)*x}
integrate(f2, 0, Inf)
# OR
integrate(function(x){x*f(x)}, 0, Inf)

# task 7
# X - measured resistance of the wires produced by company A
# X ~ N(mu=0.13, sig=0.005)
mu = 0.13
sig = 0.005
x = seq(0.1, 0.16, 0.01)
curve(dnorm(x, mu, sig), xlim=c(0.1, 0.16))
# P(0.12<X<0.14) = P(X<0.14) - P(X<=0.12) = F(0.14) - F(0.12)
pnorm(0.14, mu, sig) - pnorm(0.12, mu, sig)

# task 8
# X - paint-drying time
# X ~ N(120, 15)
# mu - mean of paint-drying time (120 minutes)
# sig - standard deviation of paint-drying time (15 minutes)
mu=120; sig=15
x = 60:180
curve(dnorm(x, mu, sig), xlim=c(60,180))
# P(110<=X<=135) = P(X<=135) - P(X<110) = F(135) - F(110)
pnorm(135, mu, sig) - pnorm(110, mu, sig)

# decimal coding
# X ~ N(mu=2, sig=0.25)
mu=2; sig=0.25
x = seq(1, 3, 1/60)
curve(dnorm(x, mu, sig), xlim=c(1, 3))
pnorm(2.25, mu, sig) - pnorm(11/6, mu, sig)

# task 9
# X - maximum vehicle speed
# X ~ N(mu=46.8, sig=1.75)
mu=46.8; sig=1.75
#a) P(X<=50) = F(50)
pnorm(50, mu, sig)
#b) P(X>=48) = 1 - P(X<48) = 1 - F(48)
1 - pnorm(48, mu, sig)

# task 10
# X ~ bin(n=20, p=0.2)
n=20; p=0.2
nums = rbinom(500, n, p)
#a)
par(mfrow=c(1,3))
library("arm")
discrete.histogram(nums, main="Prob hist of generated data")
plot(0:20, dbinom(0:20, n, p), type="h", freq=F, main="binomial distribution")

mu = n * p
x = 0:20
probs = dbinom(x, n, p)
variance = sum(probs*x^2) - sum(probs*x)^2
sig = sqrt(variance)
mu; sig
# or sig=sqrt(n*p*(1-p))
# X ~(app) N(mu=4, sig=1.788854)
curve(dnorm(x, mu, sig), xlim=c(0, 20), main="normal distribution")
#b)
par(mfrow=c(1,1))
hist(nums, breaks=0:20, freq=F)
curve(dnorm(x, mu, sig), xlim=c(0,20), col="red", add=T)
#c) just play around

# task 11
# X - the number of students who applied for financial aid
# X ~ bin(n=100, p=0.25)
n=100; p=0.25

# P(X<=15) = F(15)
# exact probability
pbinom(15, n, p)
mu = n*p
sig = sqrt(n*p*(1-p))
mu; sig
# X ~(app) N(mu=25, sig=4.330127)
# approximate probability
pnorm(15, mu, sig)

# useful for checking for mistakes: (p & d)
#x=0:100
#curve(pbinom(x, n, p), xlim=c(0,100))

# task 12
# standard normal distribution - default params
# X - individual observation from standard normal distribution
# X ~ N(mu=0, sig=1)
vectormean = rep(0,0)
for(i in 1:200){
  sample = rnorm(30)
  sample_mean = mean(sample)
  vectormean[i] = sample_mean
}
hist(vectormean, freq=F)
# !IMPORTANT!
x=seq(min(vectormean), max(vectormean), length=100)
mu=0
# !IMPORTANT!
sigma=1/sqrt(30)
# The vectormean values ~ N(mu=0, sig=1/sqrt(30))
curve(dnorm(x,mu,sigma), add=T, col="red")

# task 13
# X - number of successes in 30 Bernoulli trials
# X ~ bin(n=30, p=0.4)
n=30; p=0.4
vector = rep(0,0)
sample_size = 10
for(i in 1:200){
  sample = rbinom(sample_size, n, p)
  vector[i] = mean(sample)
}
hist(vector, freq=F)
mu = n*p
sig = sqrt(n*p*(1-p)) / sqrt(sample_size)
mu; sig
# Xbar ~(app) N(mu=12, sig=0.8485281)
x=seq(min(vector), max(vector), length=100)
curve(dnorm(x, mu, sig), col="red", add=T)

# task 14
# n - sample size (50)
# X - resistance of a resistor
# X ~ N(mu=200, sig=10)
mu=200; sig=10; n=50
# Y - average resistance of 50 resistors
# Y ~ N(mu, sig/sqrt(n))
#a) P(199<=Y<=202) = P(Y<=202) - P(Y<199) = F(202) - F(199)
pnorm(202, mu, sig/sqrt(n)) - pnorm(199, mu, sig/sqrt(n))

# Z - total resistance of 50 resistors
# Z ~ N(n*mu, sig*sqrt(n))
#b) P(Z<=10020) = F(10020)
pnorm(10020, mu*n, sig*sqrt(n))

# task 15
# X - blood cholesterol level of a worker
# X ~ ?(mu=202, sig=14)
# n - sample size (64)
mu=202; sig=14; n=64
# Xbar - average blood cholesterol level of a worker
# Xbar ~(app) N(mu, sig/sqrt(n)) # can approximate by Central Limit Theorem (large sample, n>=30)

# P(198<=Xbar<=206) = P(Xbar<=206) - P(Xbar<198) = F(206) - F(198)
pnorm(206, mu, sig/sqrt(n)) - pnorm(198, mu, sig/sqrt(n))

# task 16
# X - strength of a thread
# X ~ ?(mu=0.5, sig=0.2)
# n - number of threads
mu=0.5; sig=0.2; n=100
# Xtotal - strength of a rope (summed strength of n threads)
# Xtotal ~(app) N(mu*n, sig*sqrt(n)) # can approximate by Central Limit Theorem (large sample, n>=30)

# P(Xtotal>=47) = 1 - P(Xtotal<47) = 1 - F(47)
1 - pnorm(47, mu*n, sig*sqrt(n))


