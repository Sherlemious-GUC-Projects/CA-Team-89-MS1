LDI 00 04 # n value for the factorial
LDI 01 01 # set f to have a value of 1
LDI 02 02 # set i to have a value of 2
LDI 03 00 # set condition to 0
LDI 10 00 # set reg 10 to 0
LDI 11 01 # set reg 11 to 1
LDI 16 08 # set reg 16 to 8 (the beginning of the loop)
SB 02 00 # store i in current_i
LB 03 00 # load current_i into condition 
SUB 03 00 # subtract n from current_i
BEQZ 03 04 # if n == current_i, jump to end
MUL 01 02 # f = f * i
ADD 02 11 # i = i + 1
SB 02 00 # store i in current_i
JR 10 16 # jump to the beginning of the loop
SB 01 01 # store f in result
# NOT WORKING
