call greeting

call invokeKernel

label invokeKernel
str kernel
storer c
int 3

push

call kernelFound

pop
storer x
int 4
stop
ret

label greeting
str Looking_For_Kernel...
storer x
int 10
ret

label kernelFound
str Kernel_found...
storer x
int 10

str Executing_Kernel
storer x
int 10
ret