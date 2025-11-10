2+3
1+3

bike=c(1,2,3,0,1,15,7)
ozone=read.csv("ozone.csv", sep=";")
sort(bike)
table(bike)
discrete.histogram(bike, freq=T)
?discrete.histogram
plot(table(bike)/length(bike))
pie(table(bike))

ozone
oz=ozone$ozone
oz

#br=c(0,2,4,6,8,10,12)
br=seq(0,12,2)
0:12

cut(oz,breaks=br)
table(cut(oz,breaks=br))

hist(oz, breaks=br, prob=T, main="oz", xlab="X label")
pie(table(cut(oz,6)))
pie(table(cut(oz,br)))

hist(oz, freq=T)
