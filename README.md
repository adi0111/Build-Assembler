# Build Assembler
In this project, I learned how to build the assembler that recongize the assembler code to machine code.
## Getting Started
Learning the Nand2Tetris and need to know the different A instruction and C intstruction 
```
A-instruction: @value 
// Where value is either a non-negative decimal number
// or a symbol referring to such number.
value (v ¼ 0 or 1)
Binary: 0 v vv v v v v v v v v v v v v


C-instruction: dest¼comp;jump 
// Either the dest or jump fields may be empty.
// If dest is empty, the ‘‘¼’’ is omitted;
// If jump is empty, the ‘‘;’’ is omitted.
comp dest jump
Binary: 1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
The translation of each of the three fields comp, dest, jump to their binary forms is
specified in the following three tables.
```
and use the symbol tables to help recognize the 
code's address like LOOP, END, M=1, i=1

The example is like this
``` 
// Computes sum=1+...+100.
@i // i=1
M=1
@sum // sum=0
M=0
(LOOP)
@i // if (i-100)=0 goto END
D=M
@100
D=D-A
@END
D;JGT
@i // sum+=i
D=M
@sum
M=D+M
@i // i++
M=M+1
@LOOP // goto LOOP
0;JMP
(END) // infinite loop
@END
0;JMP
```


```
0000000000010000
1110111111001000
0000000000010001
1110101010001000
0000000000010000
1111110000010000
0000000001100100
1110010011010000
0000000000010010
1110001100000001
0000000000010000
1111110000010000
0000000000010001
1111000010001000
0000000000010000
1111110111001000
0000000000000100
1110101010000111
```
##Testing
use the commmand:

./assembler test.asm test.hack

to compare whether the generated machine code is same as the machine code should be.
