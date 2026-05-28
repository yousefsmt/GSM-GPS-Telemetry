target extended-remote :3333
monitor reset init
monitor halt
load
tbreak main
monitor reset init
continue