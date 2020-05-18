str shell_is_loaded
storer x
int 10

call wait
call done

label wait
int 5
storer c
int 3
var a
storev a
loadi 0
cmpr c
jz invoke
str unknown_command
storer x
int 10
jmp wait

label invoke
loadv a
storer x
int 4
jmp wait
ret

label done
stop