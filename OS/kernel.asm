label kernelmain
call helloworld
call shell
jmp end

label shell
str shell
storer c
int 3

var a
storev a

loadi 0
cmpr c
jz load

jmp error

label load
loadv a
storer x
int 4
jmp shellEnd

label error
str shell_not_found
storer x
int 10
jmp shellEnd

label shellEnd
ret

label helloworld
str Loading_Shell...
storer x
int 10
ret

label end
stop