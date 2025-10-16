# task1
sin(2*pi)
cos(3/4)
tan(pi)
log(100, base=10)
log(15)
log(1/7, base=7)
exp(3)
64^(1/3)

# task2
vec = 1:10
vec = seq(1, 10)
sum(vec)

# task3
x = seq(2, 20, 2)
x*x == x^2
rev(x)
sum(x)
length(x)
sqrt(sum(x^2))

# task4
y = seq(5, 10, length=13)
length(y)

# task5
z1 = rep(c(1,2), 5)
z2 = rep(c(1,2), each=5)
z1*z2
t(z1)%*%z2
z1%*%t(z2)

# task6
a = c(1,3,6,2,7,4)
min(a)
which(a==min(a))
which(a<=4)
sum(a)
sum(a^2)
sqrt(sum(a^2))
a[3]
a = a+4
b = a[-4]
c = a+b # not possible - they're of different lengths
d = a[a>4]

# task7
A = cbind(c(2,1,1), c(3,-1,1), c(0,2,-1))
t(A)
det(A)
sum(diag(A))
A*A
A%*%A
A%*%diag(A)
solve(A)
round(solve(A)%*%A)
#a = A[,3]
#b = A[2,]
#class(b)
#b = as.matrix(b) # unnecessary (perhaps)
#a%*%b
#a%*%t(b)
a = matrix(A[,3], ncol=1)
b = matrix(A[2,], nrow=1)
b %*% a
a %*% b

# task8
x = 1:10
y = c(3, 5, 4, 8, 6, 9, 11, 10, 13, 12)
plot(x, y)
df = data.frame(x, y)
plot(df, xlab="X", ylab="Y")
rows = rbind(x, y)
plot(rows)
cols = cbind(x, y)
plot(cols)

# task9
x = seq(-3, 4, by=0.1)
f = x^2 + 3*x - 5
plot(x, f, type="l")
