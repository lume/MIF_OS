label kernelmain
call printHello

loadv x
inc
storev x

jmp end


label printHello
loadi 10
var x 
storev x
ret

inc 
inc


label end
stop