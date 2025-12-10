# LAB4 FILE
rm(list=ls())
data = read.csv("data_est.csv", sep=";")

# TASK 2
milk = na.omit(data$milk)
# a)
# Population is all the milk of nursing mothers
# Sample is milk of 20 nursing mothers
# Measurement is the amount of PSB in the 20 milks of mothers
# b)
mu = mean(milk)
mu
# c)
variance = var(milk)
sigma = sd(milk)
sigma
# d)
n = length(milk)
alpha = 0.05
quant = qt(1-alpha/2, n-1)
L = mu - quant*sigma/sqrt(n)
U = mu + quant*sigma/sqrt(n)
c(L, U)
# we are 95% sure that the interval (3.42, 8.18) covers the true value of mean of amount of PCB in milk
t.test(milk, conf.level=1-alpha)
# e)

L = (n-1)*variance/qchisq(1-alpha/2, n-1)
U = (n-1)*variance/qchisq(alpha/2, n-1)
c(L, U)
c(sqrt(L), sqrt(U))

# TASK 3
# population: all cigarettes
# sample: 15 packs of cigarettes
# measurement: content of nicotine

# a) estimate with 95% confidence the mean content of nicotine
cigarettes = na.omit(data$cigarettes)
n=length(cigarettes)
alpha = 0.05
nbar = mean(cigarettes)
sd=0.7
L = nbar - qnorm(1-alpha/2) * sd/sqrt(n)
U = nbar + qnorm(1-alpha/2) * sd/sqrt(n)
c(L, U)
# how large sample is needed for the length of the 95% confidence interval 
# to be less than or equal to 0.3 milligrams?

# U - L:
# 2*quant(sd/sqrt(n)) <= 0.3
# n >= (2*quant(sd/0.3))^2
2*qnorm(1-alpha/2)*sd/sqrt(n)
# n >=
(2*qnorm(1-alpha/2)*sd/0.3)**2
# n should be an integer number
# n = 84

#c) compute the estimate of standard deviation 
# and compare the result with population standard
# deviation, which is known from past expercience.
# estimate of sd
sd(cigarretes)
sd
# we know sd so we dont estimate it

# TASK 7
# error = quant(1-alpha/2)*sd/sqrt(n)
# error <= 1
# n >= (quant(1-alpha/2)*sd/1)**2
var = 25
sd = sqrt(var)
alpha = 0.05
(qnorm(1-alpha/2)*sd)**2
# n = 97

# TASK 9
TT = 3
n = 100
p = TT/n

alpha = 0.05
L = max(p - qnorm(1-alpha/2) * sqrt(p*(1-p)/n), 0) * 100
U = (p + qnorm(1-alpha/2) * sqrt(p*(1-p)/n)) * 100
c(L, U)
# answer: from 0 to 0.063
binom.test(TT, n, conf.level=1-alpha)$conf.int * 100
prop.test(TT, n, conf.level = 1-alpha)$conf.int * 100

# TASK 13
# a)

# guess p - 0.5
# error = qnorm(1-alpha/2) * sqrt(p*(1-p)/n)
# error =app 0.05
# n =app quant * sqrt(p*(1-p)) / 0.05)**2
alpha = 0.02
p = 0.5
n = (qnorm(1-alpha/2)*sqrt(p*(1-p))/0.05) ** 2
round(n)
