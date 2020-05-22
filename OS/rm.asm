int 35
storer c
loadi 2
cmpr c
jnz error_no_name

loadi 1
int 36
int 16

storer c
loadi 1
cmpr c
jz error

label end
stop

label error_no_name
str could_not_remove_file..._make_sure_it_exists
storer x
int 10
jmp end

label error
str error_when_removing 
storer x
int 10
jmp end