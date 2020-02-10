    load 30
    dec
    jge daugiau
    store 31
    jmp pab  
    label daugiau
    store 32
    label pab
    stop

    org 30
    data 100
    data 0
    data 0  