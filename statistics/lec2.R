setwd("C:/Users/szymo/Documents/University/semester 3/statistics")

bike=c(1,2,3,0,1,15,7)

mean(bike)
quantile(bike)
summary(bike)
quantile(bike, probs=0.7)

"mean median min max var sd"

groupA = c(3.0, 3.0, 4.0, 4.5, 4.5)
groupB = c(2.0, 3.5, 4.0, 4.5, 5.0)

min(groupA); min(groupB)
mean(groupA); mean(groupB)
median(groupA); median(groupB)
max(groupA); max(groupB)
var(groupA); var(groupB)

sd(groupA)
sd(groupB)

sd(groupA)/mean(groupA)
sd(groupB)/mean(groupB)

par(mfrow = c(1,2))
boxplot(groupA)
boxplot(groupB)

par(mfrow=c(1,1))
boxplot(groupA, groupB)
