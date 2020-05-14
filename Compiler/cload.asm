var a
jmp test
loadi 1

inc
inc
inc
label test
loadi 1
storer x
loadi 2
inc
addr x
storev a
jmp test
stop
