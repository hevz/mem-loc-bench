#include <sys/regdef.h>

    .text

    .global func_templ
    .ent    func_templ
    .type   func_templ, @function
    .set    push
    .set    noreorder
func_templ:
    daddiu  sp,sp,-16
    sd      s8,0(sp)
    sd      ra,8(sp)
    move    s8,sp
    li      v0,2
    ld      ra,8(sp)
    ld      s8,0(sp)
    daddiu  sp,sp,16
    jr      ra
    nop

    .set    pop
    .end    func_templ
    .size   func_templ, . - func_templ

    .global icache_sync
    .ent    icache_sync
    .type   icache_sync, @function
    .set    push
    .set    noreorder
icache_sync:
    jr.hb   ra
    nop

    .set    pop
    .end    icache_sync
    .size   icache_sync, . - icache_sync
