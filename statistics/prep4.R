# q - Quantile functions (INVERSE of p-functions)
# qnorm(0.9) - find x such that P(X<=x)=0.9

# qt(0.9, 24)
# This means the t-distribution was estimated using a sample of size 25

qnorm(0.9)     # Returns ~1.28 because P(Z ≤ 1.28) = 0.90
qt(0.9, 24)    # Returns ~1.32 because P(T ≤ 1.32) = 0.90 (t-distribution, df=24)
qt(0.9, 7)     # Returns ~1.41 because P(T ≤ 1.41) = 0.90 (t-distribution, df=7)

# Answer: "What value corresponds to this cumulative probability?

# TASKS
rm(list=ls())
data = read.csv("data_est.csv", sep=";", dec=".")
data

# task 1
weights = na.omit(data$diamonds) #returns a vector
#weights = na.omit(data[,"diamonds"]) #returns a dataframe
#weights = na.omit(data["diamonds"]) #returns a vector
weights[1:length(weights)]

#a)
# population: all synthetic diamonds produces
# sample: 12 synthetic diamonds
# random variable: the weight of a synthetic diamond
#b) the point estimate of mean weight
mean(weights)
#c) estimate the mean weight with 95% confidence
xbar = mean(weights)
alpha = 0.05
n = length(weights)
sig = sd(weights)
L = xbar - qt(1-alpha/2, n-1)*sig/sqrt(n)
U = xbar + qt(1-alpha/2, n-1)*sig/sqrt(n)
c(L, U)
# !IMPORTANT! round L down, and U up, lengths mathematically
# we are 95% confident that the interval 0.498 to 0.57 covers the true mean
# weight of the population of synthetic diamonds produced
x = t.test(weights, conf.level=1-alpha)
x$conf.int
U-L; x$conf.int[2] - x$conf.int[1]
#d) estimate the mean weight with 98% confidence
xbar = mean(weights)
sig = sd(weights)
alpha = 0.02
L = xbar - qt(1-alpha/2, n-1)*sig/sqrt(n)
U = xbar + qt(1-alpha/2, n-1)*sig/sqrt(n)
U-L
# for 95% confidence the interval length is 0.071
# for 98% confidence the interval length is 0.087
# for 99% confidence the interval length is 0.1
#e)
variance = var(weights)
std = sqrt(variance)
variance; std
#f)
alpha = 0.05
n = length(weights)
std = sd(weights)
# !IMPORTANT! the formula on the slides is for VARIANCE
L = sqrt((n-1)*std^2/qchisq(1-alpha/2, n-1))
U = sqrt((n-1)*std^2/qchisq(alpha/2, n-1))
c(L, U)
# we are 95% confident that the interval 0.039 to 0.095 covers the true standard deviation
# of weight of the population of synthetic diamonds produced

# task 2
milk = na.omit(data$milk)

#a)
# population - milk of all nursing women
# sample - milk of 20 nursing women
# measurement - the amount of PCB in the milk of nursing mothers
# better:
# measurement - the amount of PCB in the milk of a single nursing mother
#b)
mean(milk)
#c)
var(milk)
sd(milk)
#d) estimate the average amount of PCB in the milk with 95% confidence
alpha = 0.05
xbar = mean(milk)
n = length(milk)
std = sd(milk)
# !IMPORTANT! don't mix up qt() and qchisq() !!! (ALSO ^2 IS JUST THE NOTATION, NOT THE SQUARE)
L = xbar - qt(1-alpha/2, n-1)*std/sqrt(n)
U = xbar + qt(1-alpha/2, n-1)*std/sqrt(n)
c(max(L, 0), U)
# we are 95% confident that the interval 3.42 to 8.18 covers the true population mean
#e) estimate the variance and standard deviation of the
# amount of PCB in the milk with 95% confidence 
alpha = 0.05
n = length(milk)
std = sd(milk)
L = (n-1)*std^2/qchisq(1-alpha/2, n-1)
U = (n-1)*std^2/qchisq(alpha/2, n-1)
# interval for variance:
c(L, U)
# interval which covers standard deviation
c(sqrt(L), sqrt(U))
# we are 95% confident that the interval 14.951 to 55.151 covers the true  population variance and that the
# interval 3.866 to 7.427 covers the true population standard deviation

# task 3
cigarettes = na.omit(data$cigarettes)
# nicotine content ~ N(mu=?, sig=0.7)
sig = 0.7
# population - nicotine content of all newly marketed cigarettes
#a) estimate the population mean with 95% confidence
alpha = 0.05
xbar = mean(cigarettes)
n = length(cigarettes)
L = xbar - qnorm(1-alpha/2)*sig/sqrt(n)
U = xbar + qnorm(1-alpha/2)*sig/sqrt(n)
c(L, U)
# we are 95% confident that the interval 1.455 to 2.164 covers the true population mean
#install.packages("BSDA")
library(BSDA)
x = z.test(cigarettes, sigma.x=sig, conf.level=0.95)
x$conf.int
#b) calculate the smallest sample needed for the 95% confidence interval to be not longer than 0.3 milligrams
# len = Xbar+qnorm(1-alpha/2)*sig/sqrt(n)-Xbar+qnorm(1-alpha/2)*sig/sqrt(n)
# len = 2*qnorm(1-alpha/2)*sig/sqrt(n)
# sqrt(n) = 2*qnorm(1-alpha/2)*sig/len
# min_n = (2*qnorm(1-alpha/2)*sig/len)^2
alpha = 0.05
n = length(cigarettes)
min_n = (2*qnorm(1-alpha/2)*sig/0.3)^2
ceiling(min_n)
# a sample of size greater than or equal to 84
#c) compute the estimate of standard deviation and compare it with population standard deviation
std = sd(cigarettes)
std
# the estimate of standard deviation is noticeably smaller than population standard deviation

# task 4
signals = na.omit(data$signal)
# X - intensity of a signal recorded at location B
# X ~ N(mu=?, sig=3)
# n - number of times a signal is sent (10)
sig = 3
mu = 0
n = 10
# Xbar - average intensity of a signal among 10 "copies" of it
#IMPORTANT look it average/total
# Xbar ~ N(?, sig/sqrt(n))

# calculate the point estimate of mu
Xbar = mean(signals)
Xbar
# estimate the true signal intensity at location A with 95% confidence
alpha = 0.05
#IMPORTANT these formulas already account for /sqrt(n) for means
L = Xbar - qnorm(1-alpha/2)*sig/sqrt(n)
U = Xbar + qnorm(1-alpha/2)*sig/sqrt(n)
c(L, U)
# we are 95% confident that the interval 17.44 to 21.16 covers the true
# signal intensity at location A

# task 5
# X - time span of a phone call made during midday
# X ~ ?(mu=4.7, sig=2.2)
# n - sample size (1200)
# population - lengths of all phone calls made during midday using the company's service
# sample - 1200 phone calls' lengths selected from population
mu=4.7; sig=2.2; n=1200
# Xbar - average span of a phone call made during midday among the sample
#IMPORTANT: if don't know distribution, avg/total ~(app) normal + Central Limit Theorem
# Xbar ~(app) N(mu, sig/sqrt(n)) # can approximate by Central Limit Theorem because the sample if large (n>=30)

# estimate the population mean with 95% confidence
alpha = 0.05
L = mu - qt(1-alpha/2, n-1)*sig/sqrt(n)
U = mu + qt(1-alpha/2, n-1)*sig/sqrt(n)
c(L, U)
# we are 95% confident that the interval 4.575 to 4.825 covers the true population mean
alpha = 0.05
L = sqrt((n-1)*sig^2/qchisq(1-alpha/2, n-1))
U = sqrt((n-1)*sig^2/qchisq(alpha/2, n-1))
c(L, U)
# we are 95% confident that the interval 2.115 to 2.292 covers the true population standard deviation

# task 6
transistors = na.omit(data$lifetime)
n = length(transistors)
# population: lifetime of all General Electric transistors
# sample: 36 General Electric transistors selected from population
# X - a lifetime of a General Electric transistor
# Xbar - mean lifetime of General Electric transistors among the sample
avg = mean(transistors)
std = sd(transistors)
avg; std
# !IMPORTANT! how to write sample estimates to distribution:
# X ~ ?(mu=avg, sig=std) # using sample estimates for population parameters
# Xbar ~(app) N(avg, std/sqrt(n)) # can approximate by Central Limit Theorem because the sample is large (n>=30)

# !IMPORTANT! don't divide by sqrt(n), it's in the formula
#S = std/sqrt(n)
alpha = 0.05
L = avg - qt(1-alpha/2, n-1)*std/sqrt(n)
U = avg + qt(1-alpha/2, n-1)*std/sqrt(n)
c(L, U)
# we are 95% confident that the interval 1162.505 to 1231.375 covers the true population mean

# task 7
# X - setting time of cement mixture 
# X ~ N(mu=?, sig=5)
# n - sample size required to obtain a given precision
sig = 5

# !IMPORTANT! ERROR IS THE MAX ACCEPTABLE DEVIATION FROM XBAR, NOT U-L!!
# error = qnorm(1-alpha/2)*sig/sqrt(n)
# sqrt(n) = qnorm(1-alpha/2)*sig/error
# n = (qnorm(1-alpha/2)*sig/error)^2
alpha = 0.05
n = (qnorm(1-alpha/2)*sig/1)^2
# !IMPORTANT!
# "musisz miec wiecej samples zeby zminimalizowac error"
ceiling(n)
# the sample size should be at least 97

# task 8
# X - weight of salmon grown at a commercial hatchery
# Xbar - average weight of salmon grown at a commercial hatchery
sig=0.3
error=0.1
# error = qnorm(1-alpha/2)*sig/sqrt(n)
# n = (qnorm(1-alpha/2)*sig/error)^2
# if we want to be 90% certain:
alpha=0.1
n = (qnorm(1-alpha/2)*sig/error)^2
ceiling(n)
# we are 90% confident that that our estimate of the mean weight of salmon is
# correct within +-0.1 pounds if sample size is at least 25
# if we want to be 99% sure:
alpha=0.01
n = (qnorm(1-alpha/2)*sig/error)^2
ceiling(n)
# we are 99% confident that that our estimate of the mean weight of salmon is
# correct within +-0.1 pounds if sample size is at least 60

# task 9
# population - all computers of a particular brand and model
# sample - 100 computers selected from population
# n - sample size (100)
n = 100
x = 3
p_hat = x/n
# X - number of times computers were down
# X ~ bin(100, 3/n) # using sample estimates for population parameters
alpha = 0.05

L = max(p_hat - qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n), 0)
U = p_hat + qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
c(L, U)
# we are 95% confident that interval 0 to 0.0635 covers the true population proportion of unscheduled downtime
binom.test(x, n, conf.level=1-alpha)$conf.int
prop.test(x, n, conf.level=1-alpha)$conf.int

# task 10
# population - all cans in a brewery
# sample - 100 cans selected from brewery
# p_hat - sample proportion of underfilled cans
# n - sample size (100)
# x - number of underfilled cans found in a sample
x = 4; n = 100; alpha = 0.05
p_hat = x/n
# estimate the true population proportion of underfilled cans
L = p_hat - qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
U = p_hat + qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
c(L, U)
# we are 95% confident that interval 0.0015 to 0.0785 covers the true population proportion of underfilled cans

# task 11
# population - all upholstery-installer teams in an automobile assembly plant, where each team is either arranging materials besides work stations or not
# sample - 120 teams randomly selected from population
# n - sample size (120)
# x - number of teams arranging materials beside their work station (24)
# p_hat - sample proportion  of teams arranging materials besides work stations
n=120; x=24
p_hat = x/n
alpha = 0.1
# estimate the true population proportion of teams arranging materials besides work stations
L = p_hat - qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
U = p_hat + qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
c(L, U)
# we are 90% confident that interval from 0.1399 to 0.2601 covers the true population proportion of teams arranging materials besides work stations

# task 12
# population - all construction workers , where each worker is either employed or unemployed
# sample - 1000 workers randomly selected from population
# n - sample size(1000)
# p_hat - sample proportion of workers who are unemployed
n = 1000; x = 122
alpha = 0.1
p_hat = x/n
# estimate the true population proportion of workers who are unemployed
L = p_hat - qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
U = p_hat + qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
c(L, U)
# we are 90% confident that interval 0.1049 to 0.1391 covers the true population proportion of workers who are unemployed

# task 13
alpha = 0.02
error = 0.05
# error - estimation error, we accept +- 0.05
# p_hat - sample proportion of people who have sight problems in a particular group

# error = qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
# p_hat*(1-p_hat)/n = (error/qnorm(1-alpha/2))^2
# n = p_hat*(1-p_hat)/(error/qnorm(1-alpha/2))^2
# a) nothing is known about p_hat, assume p_hat=0.5
p_hat = 0.5
n = p_hat*(1-p_hat)/(error/qnorm(1-alpha/2))^2
ceiling(n)
# 542 people should be examined to obtain an estimation error +-0.05 on confidence level 0.98
#b) p_hat = 0.3
p_hat = 0.3
n = p_hat*(1-p_hat)/(error/qnorm(1-alpha/2))^2
ceiling(n)
# 455 people should be examined to obtain an estimation error +-0.05 on confidence level 0.98
