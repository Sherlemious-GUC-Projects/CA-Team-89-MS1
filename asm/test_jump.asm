LDI R00 12 # x <- 18
LDI R05 07 # location of the skip
LDI R10 00 # tmp_0 <- 0
LDI R11 01 # tmp_1 <- 1
JR R10 R05 # jump to the skip if x == 19
LDI R00 16 # x <- 19 (this should be skipped)
LDI R11 02 # tmp_1 <- 2 (this should be skipped)
ADD R00 R11 # x <- x + 1
SB R00 R00 # store x in memory
