sin(2*pi)
cos(3/4)
tan(pi)
log10(100)
logb(15)
log(1/7, base=7)
?log
exp(3)
64^(1/3)

vec = 1:10
sum(vec)


x = seq(2, 20, 2)
x*x == x^2
rev(x)
sum(x)
length(x)
sqrt(sum(x^2))

# task 4
# a)
y = seq(5, 10, length=13)
length(y)

# task 5
z1 = rep(c(1,2), 5)
z2 = rep(c(1,2), each=5)

z1*z2
t(z1)

t(z1)%*%z2
z1%*%t(z2)

# task 6
a = c(1, 3, 6, 2, 7, 4)
min(a)
which(a==min(a))
which.min(a)
max(a)
which.max(a)
which(a<=4)
sum(a*a)
sqrt(sum(a^2))
a[3]
a = a + 4
a
b=a[-4]
b
c=a+b # they're of different lengths
d = a[a>4]
d

# task 7
A = matrix(c(2,1,1,3,-1,1,0,2,-1), ncol=3)
A = cbind(c(2,1,1),c(3,-1,1),c(0,2,-1))
# also rbind()
A
# a)
t(A)
# b)
det(A)
# c)
sum(diag(A))
# d)
A*A
A%*%A
# e)
A%*%diag(A)
diag(A)%*%A
# f)
invA=solve(A)
invA
round(A%*%invA)
a=A[,3]
b=A[2,]
class(b)
b=as.matrix(b)
a%*%b
t(a)%*%t(b)


