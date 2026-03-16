.text
.globl f
f:
    addi  sp, sp, -16
    sw    ra, 12(sp)
    sw    s0, 8(sp)
    addi  s0, sp, 16
    sw a0, -12(s0)
    sw a1, -16(s0)
    lw a0, -12(s0)
  addi sp, sp, -4
  sw a0, 0(sp)
    lw a0, -16(s0)
  lw t0, 0(sp)
  addi sp, sp, 4
  add a0, t0, a0
    j .f_ret_targ0
.f_ret_targ0:
    lw    s0, 8(sp)
    lw    ra, 12(sp)
    addi  sp, sp, 16
    ret
