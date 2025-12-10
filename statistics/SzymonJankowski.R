# Szymon Jankowski
# 164231
# group 1

# task 1
data = read.csv("IrisSepalLength.csv", sep=";", dec=",")
data

#a)
cols = names(data)
for(i in 1:length(cols)){
  col_data = data[, i]
  cat(paste0("Basic statistical characteristics for ", cols[i], ":\n"))
  cat(paste0("  Mean: ", mean(col_data), "\n"))
  cat(paste0("  Median: ", median(col_data), "\n"))
  cat("  Quartiles:\n")
  quartiles = quantile(col_data)
  for(j in 1:length(quartiles)){
    cat(paste0("    ", (j-1)*25, "%: ", quartiles[j], "\n"))
  }
  cat(paste0("  Standard deviation: ", sd(col_data), "\n"))
  cat(paste0("  Variability index: ", round(sd(col_data)/mean(col_data)*100, 2), "%\n\n"))
  
}
quantile(col_data)

#b)
cols = names(data)
par(mfrow=c(1, length(data)))
colours = c("red", "green", "blue")
for(i in 1:length(cols)){
  col_data = data[, i]
  hist(col_data, freq=T, main=cols[i], col=colours[i], xlab="sepal length")
}

#c)
par(mfrow=c(1,1))
boxplot(data, main="Lenghts of the three species of Iris", ylab="sepal length")

# task 2
# A - the number of cars with mouldy air conditioning among chosen
# A ~ bin(n=9, p=0.2)
p = 0.2
n = 9
#a)
# probability that exactly 4 cars have mouldy air conditioning: P(A==4)
dbinom(4, n, p)
# the probability is 0.066
# probability that at least 5 cars have mouldy air conditioning: P(A>=5)
sum(dbinom(5:9, n, p))
# the probability is 0.196
# probability that less than 3 cars have mouldy air conditioning P(A<3) = P(A<=2)
sum(dbinom(0:2, n, p))
# the probability is 0.738

#b) compute expected value of A
EX = n*p
EX
# the expected value of A is 1.8

#c) compute standard deviation of A
std = sqrt(n*p*(1-p))
std
# the standard deviation of A is 

# task 3
# X - the number of damaged roller in a conveyor belt in a factory ABC
# X ~ exp(lambda=0.2325581)
mu = 4.3
lambda = 1/mu

#a)
x = 0:25
curve(dexp(x, lambda), xlim=c(0, 25), main=paste0("The density function of exponential distribution with lambda=", round(lambda, 4)), ylab="density")

#b)
# probability that the number of damaged roller exceed 10: P(X>10) = 1 - P(X<=10) = 1 - F(10)
1 - pexp(10, lambda)
# the probability is 0.098
# probability that the number of damaged roller is between 5 and 8: P(5<=X<=8) = P(X<=8) - P(X<5) = F(8) - F(5)
pexp(8, lambda) - pexp(5, lambda)
# the probability is 0.157

f = function(x){lambda*exp(-lambda*x)}
integrate(f, 10, Inf)
integrate(f, 5, 8)
# the results are indeed correct

# task 4
# X - train delay of a PKP train
# X ~ N(mu=5, sig=2)
mu = 5
sig = 2

#a)
# probability that the average train delay among 40 is between 4.8 and 5.1 minutes:
# Xbar - average train delay among n=40 trains
# Xbar ~ N(mu, sig/sqrt(n))
n = 40
# P(4.8<=Xbar<=5.1) = P(Xbar<=5.1) - P(Xbar<4.8) = F(5.1) - F(4.8)
pnorm(5.1, mu, sig/sqrt(n)) - pnorm(4.8, mu, sig/sqrt(n))
# the probability is 0.361

# probability that the total train delay among 40 does not exceed 3 hours (180 minutes):
# Xtotal - total train delay among n=40 trains
# Xtotal ~ N(mu*n, sig*sqrt(n))
n = 40
# P(Xtotal<=180) = F(180)
pnorm(180, mu*n, sig*sqrt(n))
# the probability is 0.057

#b)
n_samples = 200
size = 40
vector_mean = rep(0,0)
vector_sum = rep(0,0)
for(i in 1:n_samples){
  sample = rnorm(size, mu, sig)
  vector_mean[i] = mean(sample)
  vector_sum[i] = sum(sample)
}

# i.
hist(vector_mean, freq=F, col="blue", xlab="Means of samples", main="Histogram and density function of normal distribution of means of samples")
x = seq(min(vector_mean), max(vector_mean), length=100)
curve(dnorm(x, mu, sig/sqrt(n)), xlim=c(min(vector_mean), max(vector_mean)), col="red", add=T, main="ttt")

# ii.
hist(vector_sum, freq=F, col="blue", xlab="Sums of samples", main="Histogram and density function of normal distribution of sums of samples")
x = seq(min(vector_sum), max(vector_sum), length=100)
curve(dnorm(x, mu*n, sig*sqrt(n)), xlim=c(min(vector_sum), max(vector_sum)), col="red", add=T)

# task 5
# X - drying time of a quick-drying concrete
# n - sample size (15)
data = read.csv("time.csv", sep=";")$group1
sample_mean = mean(data)
sample_std = sd(data)
n = length(data)

#a)
# population - all quick-drying concretes of a new type
# sample - 15 concretes of a new type selected from population
# measurement - drying time of a concrete

#b) estimate with 95% confidence the mean drying time of all quick-drying concretes
alpha = 0.05
L = sample_mean - qt(1-alpha/2, n-1)*sample_std/sqrt(n)
U = sample_mean + qt(1-alpha/2, n-1)*sample_std/sqrt(n)
c(L, U)
# we are 95% confident that the interval 24.858 to 27.809 covers the true population mean drying-time of all quick-drying concretes

#c) estimate with 96% confidence the standard deviation of the drying time of all quick-drying concretes
alpha = 0.04
L = sqrt((n-1)*sample_std^2/qchisq(1-alpha/2, n-1))
U = sqrt((n-1)*sample_std^2/qchisq(alpha/2, n-1))
c(L, U)
# we are 96% confident that the interval 1.922 to 4.302 covers the true population standard deviation of the drying time of all quick-drying concretes

# task 6
# n - sample size of drivers (1000)
# x - the number of drivers from a sample presently unable to work (130)
n = 1000
x = 130
#a) estimate the proportion of all drivers who are unable to work
# p_hat - sample proportion of drivers presently unable to work
p_hat = x/n
p_hat
# the estimate proportion is 0.13

#b) estimate with 90% confidence the true proportion of unemployed workers
alpha = 0.1
L = p_hat - qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
U = p_hat + qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
c(L, U)
# we are 90% confident that the interval 0.1125 to 0.1475 covers the true proportion of unemployed workers

#c)
max_error = 0.04
alpha = 0.05
# error = qnorm(1-alpha/2)*sqrt(p_hat*(1-p_hat)/n)
# error/qnorm(1-alpha/2) = sqrt(p_hat*(1-p_hat)/n)
# p_hat*(1-p_hat)/n = (error/qnorm(1-alpha/2))^2
# n = p_hat*(1-p_hat)/(error/qnorm(1-alpha/2))^2
# min_n = ceiling(p_hat*(1-p_hat)/(max_error/qnorm(1-alpha/2))^2)
min_n = ceiling(p_hat*(1-p_hat)/(max_error/qnorm(1-alpha/2))^2)
min_n
# at least 272 drivers should be examined to obtain an estimation error +-0.04 on confidence level 0.95 with sample proportion 0.13
