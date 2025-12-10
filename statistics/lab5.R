# task 6
mean = 2.4
lambda = 1/mean
curve(dexp(x, lambda), from=0, to=10) # or xlim

# P(R>3)
1-pexp(3,lambda)

# P(2<R<3)
pexp(3,lambda) - pexp(2,lambda)

# same calculations, using integrals
f=function(x){lambda*exp(-lambda*x)}
integrate(f,3,Inf)
g=function(x){x*lambda*exp(-lambda*x)}
h=function(x){x*f(x)}
integrate(g, 0, Inf)
integrate(h, 0, Inf)

# task 7
# X-wire resistance
# X~N(mu=0.13,sigma=0.005)
mu = 0.13
sigma = 0.005
title = paste0("N(", mu, ", ", sigma, ")")
curve(dnorm(x,mu,sigma), from=mu-3*sigma, to=mu+3*sigma, main=title, col="red")
# P(0.12<X<0.14) = F(0.14) - F(0.12)
pnorm(0.14, mu, sigma) - pnorm(0.12, mu, sigma)

# task 8
# B - paint-drying time
# B~N(mu=2, sigma=0.25)
mu=2
sigma=0.25
title=paste0("N(", mu, ", ", sigma, ")")
curve(dnorm(x,mu,sigma), from=mu-3*sigma, to=mu+3*sigma, main=title, col="darkblue")

mu=120
sigma=15
pnorm(135,mu,sigma) - pnorm(110,mu,sigma)

# task 9
# pnorm

# task 10
n=20
p=0.2
ss=500
data=rbinom(ss,n,p)
head(data)
tail(data)

library("arm")
par(mfrow=c(1,3))
discrete.histogram(data)
plot(dbinom(0:20,n,p), type="h")
mu = n*p
sig=sqrt(n*p*(1-p))
curve(dnorm(x,mu,sig), from=mu-3*sig, to=mu+3*sig)
#10b
par(mfrow=c(1,1))
hist(data,xlim=c(0,20), freq=F)
curve(dnorm(x,mu,sig), from=0, to=20, add=TRUE)

#11? or 10c
n=100; p=0.25
prob=dbinom(x,n,p)
sum(prob[x<=15])

pbinom(15,n,p)
pnorm(15,mu,sigma)

### task 12
set.seed(42571)
x=rnorm(30)
vectormean = rep(0,0)
for(i in 1:200){
  X=rnorm(30)
  vectormean[i] = mean(X)
}
vectormean
hist(vectormean, freq=F)
mu=0
sigma=1/sqrt(30)
curve(dnorm(x,mu,sigma), ad=T, col="red")
