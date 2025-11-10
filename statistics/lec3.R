n=3
p=0.6
# X ~ bin(n,p)
dbinom(0, n, p)
?dbinom

x = 0:n
probs = dbinom(x, n, p)
rbind(x, probs)

# P(X>=2)
sum(dbinom(2:3, n, p))
sum(probs[x>=2])

probs[x==2]

# EX (expected value)
expect=sum(x*probs); expect

variance=sum(x**2*probs)-expect**2; variance
std = sqrt(variance); std

plot(x, dbinom(x,n,p), type="h")
plot(2:10, dbinom(2:10, 10, 0.6), type="h")

f=function(x)(0.01*exp(-0.01*x))
integrate(f,0,50)
1-exp(-0.5) #???

# RED FORMULAS on slides 14-16 to be remembered on exam

lambda = 0.01
# P(X<=50)=F(50)
pexp(50, lambda)

mu = 30
sig = 0.2
# X~N*mu, sig)
# P(X<29.55)=F(29.55)
pnorm(29.55, mu, sig)
# P(X>30.45)=1-F(30.45)
1-pnorm(30.45, mu, sig)

pbinom(70, 180, 0.5)
