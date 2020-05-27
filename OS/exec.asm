int 35
storer c
loadi 2
cmpr c 
jnz error_no_file

loadi 1
int 36
loadr x
storer c
loadi 1454
int 3
var a
storev a
loadi 0
cmpr c
jz invoke
str file_not_found
storer x
int 10

label end
stop

label invoke
loadv a
storer x
int 4
jmp end

label error_no_file
str file_not_given
storer x
int 10
jmp end