    .syntax unified
    .cpu cortex-m4
    .fpu vfpv4-d16
    .thumb

    .extern usTicks
    .extern usDelay
    .global Process_Time

Process_Time:
    ldr r0, =usTicks                /* load address of usTicks */
    ldr r1, [r0]                    /* load contents of address */
    add r1, r1, #1                  /* increment usTicks */
    str r1, [r0]                    /* store usTicks value */
    ldr r0, =usDelay                /* load address of usDelay */
    ldr r1, [r0]                    /* load contents of address */
    cmp r1, #0                      /* see if usDelay is 0 */
    itt ne                          /* if not do 2 instructions */
    subne r1, r1, #1                /* if not subtract 1 */
    strne r1, [r0]                  /* if not store usDelay value */
    bx lr                           /* branch to caller */
