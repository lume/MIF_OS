int 32
storer c

var x
storev x

call printall
stop

label printall

var i
loadi 0
storev i

label loop

loadv x
storer x
loadv i
cmpr x 
jz exit

loadv i 
int 33
storer x
int 10

loadv i 
inc 
storev i 

jmp loop

label exit
ret