.global keyboard_interrupt
.extern interrupt_dispatch

keyboard_interrupt:
    pushal

    push $33
    call interrupt_dispatch
    add $4, %esp

    # Tell the PIC we handled IRQ 1
    mov $0x20, %al
    out %al, $0x20

    popal
    iret