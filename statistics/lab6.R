# still lab3 file

# task 14
# R = resistances
# R ~ N(mu, sigma)
mu=200
sigma=10
n=50

# Rbar ~ N(mu, sigma/sqrt(n))
# P(199 < Rbar < 202) = F(202) - F(199)
pnorm(202, mu, sigma/sqrt(n)) - pnorm(199, mu, sigma/sqrt(n))

#b)
#Rtotal ~ N(n*mu, sqrt(n)*sigma)
#P(Rtotal < 10020)
pnorm(10020, n*mu, sqrt(n)*sigma)

# task 15
# X - blood cholesterol level
# X ~ ?(mu, sigma)
mu = 202
sigma = 14
n = 64

# Xbar - average blood cholesterol level
# Xbar ~app N(my, sigma/sqrt(n)) # can approximate by central limit theorem (large sample(?))
# P(198 < Xbar < 206) = F(206) - F(198)
pnorm(206, mu, sigma/sqrt(n)) - pnorm(198, mu, sigma/sqrt(n))

#16
# X - the strength of a thread
# X ~ ?(mu=0.5, sigma=0.2)
# Xtotal - strength of a rope made of 100 threads
# Xtotal ~app N(n*mu, sqrt(n)*sigma)
# P(Xtotal >= 47) = 1 - P(Xtotal < 47)
mu = 0.5
sigma = 0.2
n = 100
1-pnorm(47, mu*n, sqrt(n)*sigma)

# LAB4 FILE
data = read.csv("data_est.csv", sep=";")
data

#task 1
diam = na.omit(data$diamonds)
diam
#a)
# Population - all synthetic diamonds produced by this company
# Sample - 12 diamonds
# Random variable - weight of synthetic diamond
#b) the point estimate of mean weight
mean(diam)
#c) estimate the mean weight with 95% confidence
xbar = mean(diam)
s = sd(diam)
n = 12
alpha = 0.05
L=xbar-qt(1-alpha/2,n-1)*s/sqrt(n) # in R the args change the order
U=xbar+qt(1-alpha/2,n-1)*s/sqrt(n)
c(L, U)
# we are 95% confident that interval 0.498 to 0.569
# covers the True mean weight of the population of
# synthetic diamonds produced
x=t.test(diam, conf.level=1-alpha)
x$conf.int
U-L
x$conf.int[2] - x$conf.int[1]
#d)
alpha=0.02
L=xbar-qt(1-alpha/2,n-1)*s/sqrt(n)
U=xbar+qt(1-alpha/2,n-1)*s/sqrt(n)
c(L, U)
U-L
# For 98% confidence th interval length is 0.087
alpha=0.01
L=xbar-qt(1-alpha/2,n-1)*s/sqrt(n)
U=xbar+qt(1-alpha/2,n-1)*s/sqrt(n)
c(L, U)
U-L
# For 99% confidence th interval length is 0.099
alpha=0.001
L=xbar-qt(1-alpha/2,n-1)*s/sqrt(n)
U=xbar+qt(1-alpha/2,n-1)*s/sqrt(n)
c(L, U)
U-L
# For 99.9% confidence th interval length is 0.142
alpha=0
L=xbar-qt(1-alpha/2,n-1)*s/sqrt(n)
U=xbar+qt(1-alpha/2,n-1)*s/sqrt(n)
c(L, U)
U-L
# For 100% confidence th interval length is Inf

#e)
var(diam)
sqrt(var(diam))

#f)
alpha=0.05
s = sd(diam)
L=sqrt(((n-1)*s^2)/qchisq(1-alpha/2, n-1))
U=sqrt(((n-1)*s^2)/qchisq(alpha/2, n-1))
c(L, U)
# We are 95% sure that the interval from 0.039 to 0.094 covers the
# True standard deviation of the weights of all synthetic diamonds
# in the whole population

# Task 2


