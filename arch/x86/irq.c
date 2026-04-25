#include "irq.h"
#include "pic.h"
#include "io.h"
#include "keyboard.h"

// IRQ1 => keyboard interrupt
void irq1_handler(void) {
    keyboard_handle_interrupt();
    pic_send_eoi(1);
}
