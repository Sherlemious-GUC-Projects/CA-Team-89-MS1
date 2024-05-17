LDI 00 12 # x <- 18
LDI 05 07 # location of the skip
LDI 10 00 # tmp_0 <- 0
LDI 11 01 # tmp_1 <- 1
JR 10 05 # jump to the skip if x == 19
LDI 00 16 # x <- 19 (this should be skipped)
LDI 11 02 # tmp_1 <- 2 (this should be skipped)
ADD 00 11 # x <- x + 1
SB 00 00 # store x in memory
