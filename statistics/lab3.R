setwd("C:/Users/szymo/Documents/University/semester 3/statistics")

# STILL LAB2 EXERCISES FILE

# Task 1
data = read.csv("flights.csv", sep=";")

class(data)
head(data)
tail(data)

for(col in names(data)){
  year = substr(col, 2, 5)
  
  avg = mean(data[,col])
  std = sd(data[,col])
  md = quantile(data[,col], probs=0.5)
  #first_quartile = quantile(data[,col], probs=0.25)
  #third_quartile = quantile(data[,col], probs=0.75)
  quantiles = quantile(data[,col], probs=c(0.25, 0.75))
  var = std ** 2
  print(paste("Mean no. of passengers from", year, "is equal", round(avg, 2)))
  print(paste("Standard deviation of passengers from", year, "is equal", round(std, 2)))
  print(paste("Median no. of passengers from", year, "is equal",round(md, 2)))
  #print(first_quartile) # 25% passengers at most ...
  #print(third_quartile) # 75% passengers at least ...
  print(paste0("Q1-Q3 of passengers from ", year, " are ", quantiles[1], "-", quantiles[2]))
  print(paste("Variance of passengers from", year, "is equal", round(var, 2)))
  
  variability = std / avg * 100
  print(paste0("Variability index of passengers from ", year, " is equal ", round(variability, 2), "%"))
}

summary(data)
boxplot(data, main="Boxplots of passengers", col=2:7)

?quantile

# Task 2
notes = read.csv("notes.csv", sep=";", dec=",")
boxplot(notes, col=2:5)

# Task 3
strw = read.csv("strawberries.csv", sep=";")
mean(strw[,1])
mean(na.omit(strw[,2]))
par(mfrow=c(1,2))
hist(strw[,1], main="crop 2000", freq=F)
hist(strw[,2], main="crop 2010", freq=F)

par(mfrow=c(1,1))
boxplot(strw)

pie(table(strw[,1]))
pie(table(cut(strw[,1],4))) # for some reason it doesn't work

    