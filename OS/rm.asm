str testfile
storer x
int 16

storer c
loadi 1
cmpr c
jz error

label end
stop


label error
str could_not_remove_file..._make_sure_it_exists
storer x
int 10
jmp end