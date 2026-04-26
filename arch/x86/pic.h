#ifndef MYOS_ARCH_X86_PIC_H
#define MYOS_ARCH_X86_PIC_H

void pic_remap(void);
void pic_send_eoi(unsigned char irq);

#endif
