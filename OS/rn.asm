int 35
storer c
loadi 3
cmpr c
jnz error_not_enough

loadi 2
int 36
loadr x
storer c

loadi 1
int 36

int 17

jmp end

label error_not_enough
str usage:_[old_filename]_[new_filename]
storer x
int 10
jmp end

label error
str could_not_rename
storer x
int 10
jmp end

label end
stop