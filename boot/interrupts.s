.global keyboard_interrupt
.global pit_interrupt
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

pit_interrupt:
    pushal

    push $32
    call interrupt_dispatch
    add $4, %esp

    mov $0x20, %al
    out %al, $0x20

    popal
    iret