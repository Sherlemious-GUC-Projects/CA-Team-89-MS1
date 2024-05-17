LDI R10 00 # tmp0 <- 0
LDI R11 01 # tmp1 <- 1
LDI R33 09 # locOfIf <- 9
LDI R00 06 # n <- 5
LDI R01 01 # i <- 1
LDI R02 01 # prod <- 1
SB R01 00 # mem[0] <- i
LB R03 00 # cond <- mem[0]
SUB R03 R00 # cond <- cond - n
BEQZ R03 04 # if cond == 0 jump 4
MUL R02 R01 # prod <- prod * i
ADD R01 R11 # i <- i + 1
SB R01 00 # mem[0] <- i
LB R03 00 # cond <- mem[0]
SUB R03 R00 # cond <- cond - n
JR R10 R33 # goto locOfIf
SB R02 01 # mem[1] <- sum

