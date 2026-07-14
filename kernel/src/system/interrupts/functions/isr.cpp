#include "isr.h"
#include "pic.h"

#include "page_fault.h"

#include "system/drivers/uart/driver.h"
#include "system/drivers/timer/driver.h"

#include "libs/asm/asm.h"



extern "C"
void isr_handler(Registers* regs)
{
    if(regs->vector == 32)
    {
        pit_handler();
        pic_send_eoi(0);
        return;
    }

    if(regs->vector == 14)
    {
        page_fault_handler(regs->error);
        return;
    }

    /*
        // Spurious interrupt (Local APIC, wektor 0xFF wg konwencji Intela) -
        // nie jest to prawdziwy błąd procesora, EOI nie jest wymagane, po
        // prostu ignorujemy i wracamy.
        if(regs->vector == 0xFF)
        {
            return;
        }
    */

    Uart::puts("\n========== EXCEPTION ==========\n");
    Uart::puts("Vector: ");
    Uart::puthex(regs->vector);
    Uart::puts("\n");
    Uart::puts("Error: ");
    Uart::puthex(regs->error);
    Uart::puts("\n");

    while(true)
    {
        asm volatile("cli\nhlt");
    }
}