label kernelmain
call helloworld
str other
storer x
int 10
jmp end

label helloworld
str Hello_World
var x
storev x
storer x

strcat aaa_concat
storer x
int 10

ret

label end
stop