qt(0.9, 24)
qnorm(0.9)
qt(0.9, 7)

# example 3
# X_i ~ N(70, 10), i=1, ..., 9
# T ~ N(9*70, 3*10)
# P(X1+X2+...+X9 > 650) = P(T > 650)

# X_i ~ N(mu, sig)
mu = 70
sig = 1
n=8 # 9 -> 8 to decrease the chance of failure

# T = X_1 + ... + X_9 ~ N(n*mu, sqrt(n)*sig)
# P(T > 650)
muT = n * mu
sigT = sqrt(n) * sig
1-pnorm(650, muT, sigT)

qchisq(0.9,10)
