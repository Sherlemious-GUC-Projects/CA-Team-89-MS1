LDI 10 00 # tmp0 <- 0
LDI 11 01 # tmp1 <- 1
LDI 3e 09 # locOfIf <- 9
LDI 00 05 # n <- 5
LDI 01 01 # i <- 1
LDI 02 00 # sum <- 0
SB 01 00 # mem[0] <- i
LB 03 00 # cond <- mem[0]
SUB 03 00 # cond <- cond - n
BEQZ 03 04 # if cond == 0 goto 07
ADD 02 01 # sum <- sum + i
ADD 01 11 # i <- i + 1
SB 01 00 # mem[0] <- i
LB 03 00 # cond <- mem[0]
SUB 03 00 # cond <- cond - n
JR 10 3e # goto locOfIf
SB 02 01 # mem[1] <- sum
