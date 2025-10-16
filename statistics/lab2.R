setwd("C:/Users/szymo/Documents/University/semester 3/statistics")

data = read.csv("flights.csv", sep=";")
class(data)
dim(data)

data$X1955
data[,1]

min(data$X1955)
max(data$X1955)
br=seq(225, 375, 25)
tit=substr(names(data)[1], 2, 5)
tit=paste("Passengers from", tit)
hist(data$X1955, breaks=br, main=tit, xlab="Passengers", col="blue")
barplot(t(colSums(data)))

par(mfrow = c(2,3))
colors=c("blue", "red", "green", "yellow", "lightgreen", "lightblue")
for(year in 1:6){
  hist(data[,year], main=tit, xlab="Passengers", col=colors[year])
}
for(year in 1:6){
  hist(data[,year], main=tit, xlab="Passengers", col=year)
}


# task 2
notes = read.csv("notes.csv", sep=";", dec=",")
notes
dim(notes)
length(na.omit(notes[,1]))
colSums(!is.na(notes))

install.packages("arm")
library("arm")

par(mfrow = c(1, 1))
dicrete.histogram(notes[,1])

par(mfrow = c(2,2))
for(group in 1:4){
  discrete.histogram(notes[,group], main=paste0("Group", group))
}

par(mfrow = c(2,2))
for(group in 1:4){
  pie(table(notes[,group]), main=paste0("Group", group))
}
