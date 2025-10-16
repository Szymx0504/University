data = read.csv("flights.csv", sep=";")

# TASK 1
class(data)
data
for(i in 1:dim(data)[1]){
  print(data[i,])
}
length(names(data))
dim(data)

for(year in names(data)){
  year_data = data[[year]]
  mean_val = mean(year_data)
  median_val = median(year_data)
  quartiles = quantile(year_data)
  sd_val = sd(year_data)
  variability = sd_val / mean_val
  cat(paste("Year", year, ":\n"))
  cat(paste("  Mean:", round(mean_val, 2), "\n"))
  cat(paste("  Median:", round(median_val, 2), "\n"))
  cat(paste("  Standard deviation:", round(sd_val, 2), "\n"))
  cat(paste("  Variability index:", round(variability, 3), "\n"))
  cat(paste("  Q1-Q3:", round(quartiles[2], 2), "-", round(quartiles[4], 2), "\n\n"))
}

# quantile() -> min (0%), Q1 (25%), median (50%), Q3 (75%), max (100%)

par(mfrow=c(2,3))
colours = c("blue", "red", "green", "yellow", "lightgreen", "lightblue")
for(i in 1:6){
  year = names(data)[i]
  year_data = data[,year]
  #min_val = floor(min(year_data) / 25) * 25  # round down to nearest 25
  #max_val = ceiling(max(year_data) / 25) * 25  # round up to nearest 25
  #br = seq(min_val, max_val, 25)
  br = seq(min(year_data), max(year_data)+1, length.out=6)
  hist(year_data, breaks=br, main=paste("Passengers from", substr(year, 2, 5)), xlab="Passengers", col=i)
}

par(mfrow=c(1,1))
boxplot(data)

# TASK 2

notes = read.csv("notes.csv", sep=";", dec=",")
notes
dim(notes)

for(i in 1:4){
  cat(paste(length(na.omit(notes[,i])), "\n"))
}
na.omit(notes[,1])

!is.na(notes)
colSums(!is.na(notes))

ncol(notes)
nrow(notes)

par(mfrow=c(2,2))
for(i in 1:ncol(notes)){
  group_data = notes[,i]
  group_name = names(notes)[i]
  clean_data = na.omit(group_data)
  
  plot(clean_data, type="l", main=paste("Line Graph -", group_name), xlab="Observation Number", ylab="Grade", col="blue", lwd=2)
  points(clean_data, pch=16, col="red")  
}

# hist(): Designed for continuous data, automatically handles binning
# pie(): Designed for categorical data, expects already categorized counts

par(mfrow=c(2,2))
for(group in 1:4){
  discrete.histogram(notes[,group], main=paste0("Group", group))
}

par(mfrow=c(2,2))
for(i in 1:4){
  pie(table(notes[,i]), main=names(notes)[i])
}

# TASK 3

strawberries = read.csv("strawberries.csv", sep=";")
strawberries
for(i in 1:2){
  cat(names(strawberries)[i], ":\n")
  cat(sum(is.na(strawberries[i])), "\n")
  cat(length(na.omit(strawberries[,i])), "\n\n")
}
is.na(strawberries) # works on the whole df, na.omit for a row/vector/list

for(i in 1:2){
  clean_crop = na.omit(strawberries[,i])
  mean_val = mean(clean_crop)
  sd_val = sd(clean_crop)
  variability = sd_val / mean_val
  
  cat(names(strawberries)[i], ":\n")
  cat("  Mean value: ", mean_val, "\n")
  cat("  Standard deviation: ", sd_val, "\n")
  cat("  Variability: ", variability, "\n\n")
}

par(mfrow=c(1,2))
for(i in 1:2){
  clean_crop = na.omit(strawberries[,i])
  hist(clean_crop, freq=F, main=names(strawberries)[i], xlab="Crop")
}

par(mfrow=c(1,1))
boxplot(strawberries)

max(na.omit(strawberries))
min(na.omit(strawberries))

par(mfrow=c(1,2))
for(i in 1:2){
  crop_data = na.omit(strawberries[,i])
  crop_name = names(strawberries)[i]
  
  min_val = min(crop_data)
  max_val = max(crop_data)
  intervals = seq(min_val, max_val, length.out=5) # there will be length.out-1 intervals
  
  freq_table = table(cut(crop_data, breaks=intervals))
  
  pie(freq_table, main=paste("Pie chart -", crop_name), col=rainbow(length(freq_table)), cex=0.85)
}
