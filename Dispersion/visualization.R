# Visualization of simulation results.
# Author: khbrodersen@gmail.com.

library(dplyr)
library(ggplot2)
library(readr)

kFile <- "~/concentrations.csv"

LoadData <- function(file) {
  file %>%
    read_csv() %>%
    dplyr::mutate(Containers = as.factor(Containers))
}

Main <- function() {
  kFile %>%
    LoadData() %>%
    ggplot(aes(x = Iteration, y = Concentration, color = Containers)) +
    theme_bw(base_size = 20) + 
    geom_line(size = 1) +
    ylab("Remaining concentration")
}

if (sys.nframe() == 0L) {
  Main()
}
