int 35
storer c
loadi 2
cmpr c
jnz error_no_name

loadi 1
int 36
int 15

label end
stop

label error_no_name
str filename_not_given
storer x
int 10
jmp end