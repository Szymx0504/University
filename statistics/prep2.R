# task 1
flights = read.csv("flights.csv", sep=";")
flights
install.packages("arm")
library("arm")
# a)
class(flights)
# class(flights[,1]) "integer"
#b)
par(mfrow=c(2,3))
colours = c("green", "yellow", "blue", "red", "orange", "purple")
for(year in 1:6){
  title = substr(names(flights)[year], 2, 5)
  hist(flights[,year], main=paste0("Year ", title), xlab="Number of monthly passengers", ylab="Frequency", col=colours[year])
}
#c)
mean(flights[,1])
for(year in 1:6){
  cat(paste0("Statistical characteristics for year ", names(flights)[year], "\n"))
  cat(paste0("Mean: ", mean(flights[,year]), "\n"))
  cat(paste0("Median: ", median(flights[,year]), "\n"))
  i = 0
  for(num in quantile(flights[,year])){
    cat(paste0(25*i, "%: ", num, "\n"))
    i = i + 1
  }
  cat(paste0("Standard deviation: ", sd(flights[,year]), "\n"))
  cat(paste0("Variability index: ", sd(flights[,year])/mean(flights[,year]), "\n"))
}

for(year in 1:6){ # bad approach, don't sharey!!!
  data = flights[,year]
  boxplot(flights[,year])
}
par(mfrow=c(1,1))
boxplot(flights) # much better

# task 2
notes = read.csv("notes.csv", sep=";", dec=",")
notes
table(na.omit(notes["group.M1"]))
#c)
for(group in 1:length(notes)){
  group_data = na.omit(notes[,group])
  group_name = names(notes)[group]
  
  freq_table = table(group_data)
  #IMPORTANT: change the table/freqtable name
  names(dimnames(freq_table)) = group_name

  print(freq_table)
}
#d)
library("arm")
discrete.histogram(notes[,1])
par(mfrow=c(2,2))
for(group in 1:length(notes)){
  title = names(notes)[group]
  discrete.histogram(notes[, group], main=title)
}
#e)
for(group in 1:length(notes)){
  title = names(notes)[group]
  data = na.omit(notes[,group])
  cat(paste0("Statistics for ", title, ":\n"))
  cat(paste0("Mean: ", mean(data), "\n"))
  cat(paste0("Median: ", median(data), "\n"))
  cat(paste0("Standard deviation: ", sd(data), "\n"))
  cat(paste0("Variability index: ", sd(data)/mean(data), "\n\n"))
}
#f)
boxplot(notes) # uses all available data in each column separately
boxplot(na.omit(notes)) # removes any row that has at least one NA anywhere in the dataframe
#!IMPORTANT!
# on the test use na.omit (to keep them balanced)

#g)
for(group in 1:length(notes)){
  print(paste0("Group ", names(notes)[group]))
  print(table(notes[,group]))
}
par(mfrow=c(2,2))
for(group in 1:length(notes)){
  title = names(notes)[group]
  #IMPORTANT pie() requires table()
  pie(table(notes[,group]), main=title)
}

# task 3
straws = read.csv("strawberries.csv", sep=";")
straws

#b)
# interval frequency table
for(year in 1:length(straws)){
  #intFreq = cut(straws[,year], breaks=seq(70,140, 10))
  # for each year, drop NANs separately
  data = na.omit(straws[,year])
  intFreq = cut(data, breaks=seq(min(data),max(data), length=7))
  print(table(intFreq))
}
#c)
par(mfrow=c(1,2))
for(year in 1:length(straws)){
  title = names(straws)[year]
  #data = straws[,year]
  data = na.omit(straws[,year])
  #IMPORTANT nice for hist breaks!!!
  br = seq(min(data), max(data), length=floor(sqrt(length(data))))
  hist(data, freq=F, main=title, xlab="Yields of strawberries", breaks=br)
}
#d)
for(year in 1:length(straws)){
  title = names(straws)[year]
  data = na.omit(straws[,year])
  cat(paste0("Statistics for ", title, ":\n"))
  cat(paste0("Mean: ", mean(data), "\n"))
  cat(paste0("Median: ", median(data), "\n"))
  cat(paste0("Standard deviation: ", sd(data), "\n"))
  cat(paste0("Variability index: ", sd(data)/mean(data), "\n\n"))
}
#e)
par(mfrow=c(1,1))
# omit all of them at the same time to preserve balance
boxplot(na.omit(straws))
#f)
non_na_data = na.omit(straws)
minimum=min(non_na_data[,1], non_na_data[,2])
maximum=max(non_na_data[,1], non_na_data[,2])
par(mfrow=c(1,2))
for(year in 1:length(non_na_data)){
  freqTable = table(cut(straws[,year], breaks=seq(minimum, maximum, length.out=7)))
  # length() may be tricky with 2D entities, sth dim() is needed
  #k = floor(sqrt(dim(non_na_data)[1]))
  #freqTable = table(cut(straws[,year], breaks=seq(minimum, maximum, length.out=k)))
  title = names(straws)[year]
  pie(freqTable, main=title)
}

# may be useful:
dim(notes)
length(na.omit(notes[,1]))
colSums(!is.na(notes))

