sin(2*pi)
cos(3/4)
tan(pi)
log(100, base=10)
log(15); log(15, base=exp(1))
log(1/7, base=7)
exp(3)
64^(1/3)

vector = 1:10
vector
sum(vector)
vec = seq(1, 10, 1)
vec
sum(vec)

x = seq(2, 20, 2)
x
correct = x*x==x^2
sum(!correct) # always matches
rev(x)
sum(x)
length(x)
sqrt(sum(x^2))

y = seq(5, 10, length.out=13)
y
length(y)

z1 = rep(c(1,2), 5)
# IMPORTANT
z2 = rep(c(1,2), each=5)
z1; z2
z1*z2 # piecewise multiplication
t(z1)%*%z2
z1%*%t(z2)

a = c(1,3,6,2,7,4, 0)
min(a)
# which is about indices
which.min(a) # the first one
which(a==min(a)) # all indices
which(a<=4)
sum(a)
sum(a^2)
length(a)
a[3]
a = a + 4
a
#IMPORTANT
b = a[-4] #exclude the 4th element, -c(4,5,...) for more elements
b
c = a + b
c
d = a[a>4] #filtering
d

A = matrix(c(2,1,1,3,-1,1,0,2,-1), ncol=3)
A
t(A)
det(A)
#IMPORTANT trace
sum(diag(A))
A*A
A%*%A
A %*% diag(A)
solve(A) # find the inverse
#IMPORTANT
A_inverse = solve(A)
round(A %*% A_inverse)
a = A[,3] # select col
b = A[2,] # select row
b
t(a) %*% b
a %*% t(b)

x = 1:10
y = c(3, 5, 4, 8, 6, 9, 11, 10, 13, 12)
plot(x, y)
df = data.frame(x, y)
df
plot(df, xlab="X", ylab="Y")
plot(rbind(x, y)) # these are just two strange points (dim too high)
plot(cbind(x, y)) # similar to data.frame
cbind(x, y)

x = seq(-3, 4, 0.1)
f = x^2+3*x-5
plot(x, f, type="l")

# A = cbind(c(2,1,1),c(3,-1,1),c(0,2,-1))
# also rbind()
# b=as.matrix(b)
dim(df)
length(df)

