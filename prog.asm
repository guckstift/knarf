push start jmp

:msg "Hello World"
:x 10
:y 1

:start

push msg prints

:loopstart

push x load
push loopend jz

push y load print
push y push y load push 2 mul store
push x push x load push 1 sub store

push loopstart jmp

:loopend

halt
