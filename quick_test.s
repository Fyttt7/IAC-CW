.text
.globl f
f:
li a0, 10
  addi sp, sp, -4
  sw a0, 0(sp)
li a0, 5
  lw t0, 0(sp)
  addi sp, sp, 4
  add a0, t0, a0
  addi sp, sp, -4
  sw a0, 0(sp)
li a0, 2
  lw t0, 0(sp)
  addi sp, sp, 4
  sub a0, t0, a0
ret
