LDI 00 04 # n <- 4
LDI 01 01 # cumsum <- 1
LDI 02 00 # i <- 2
LDI 03 00 # cond <- 0
LDI 04 00 # the location of the beginning of the loop
LDI 10 00 # temp val of 0
LDI 11 01 # temp val of 1
SB 02 01 # copy i to mem @ copyI (START OF LOOP)
LB 03 02 # cond <- copyI
SUB 03 00 # cond <- cond - n
BEQZ 03 02 # if cond == 0, goto END
ADD 01 11 # cumsum <- cumsum + 1
ADD 02 11 # i <- i + 1
JR 00 04 # goto START OF LOOP
SB 01 00 # store cumsum in mem @ 0 (END)

