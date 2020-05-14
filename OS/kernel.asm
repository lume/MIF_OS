jmp test
label main
jmp end

label test 
var x
loadi 10
storev x
jmp main 

label end
stop

