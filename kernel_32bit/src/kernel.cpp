#include <stdint.h>


volatile uint16_t* vga = (uint16_t*)0xB8000;

uint32_t cursor = 0;


char buffer[128];



const char keymap[128] =
{
    0, 27,
    '1','2','3','4','5','6','7','8','9','0','-','=',
    8,9,

    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',0,

    'a','s','d','f','g','h','j','k','l',';','\'',
    '`',0,'\\',

    'z','x','c','v','b','n','m',',','.','/',
    0,'*',0,' '
};



void putchar(char c)
{
    if(c == '\n')
    {
        cursor = ((cursor / 80) + 1) * 80;
        return;
    }

    vga[cursor] = (0x07 << 8) | c;
    cursor++;
}



void print(const char* str)
{
    while(*str)
    {
        putchar(*str);
        str++;
    }
}



void clear_screen()
{
    for(int i=0;i<2000;i++)
        vga[i] = (0x07 << 8) | ' ';


    cursor = 0;
}



void disable_cursor()
{
    asm volatile(
        "mov $0x3D4, %%dx\n"
        "mov $0x0A, %%al\n"
        "out %%al, %%dx\n"

        "inc %%dx\n"

        "mov $0x20, %%al\n"
        "out %%al, %%dx\n"

        :
        :
        :"ax","dx"
    );
}



void keyboard_init()
{
    uint8_t status;


    do
    {
        asm volatile(
            "inb $0x64, %0"
            :"=a"(status)
        );

    }while(status & 2);


    uint8_t cmd = 0xAE;

    asm volatile(
        "outb %0,$0x64"
        :
        :"a"(cmd)
    );


    uint8_t enable = 0xF4;

    asm volatile(
        "outb %0,$0x60"
        :
        :"a"(enable)
    );
}



uint8_t read_scancode()
{
    uint8_t status;


    while(true)
    {
        asm volatile(
            "inb $0x64,%0"
            :"=a"(status)
        );


        if(status & 1)
        {
            uint8_t code;

            asm volatile(
                "inb $0x60,%0"
                :"=a"(code)
            );

            return code;
        }
    }
}



void read_line()
{
    int pos=0;


    while(true)
    {
        uint8_t sc = read_scancode();


        if(sc & 0x80)
            continue;


        if(sc == 0x1C)
        {
            buffer[pos]=0;
            putchar('\n');
            return;
        }


        if(sc == 0x0E)
        {
            if(pos>0)
            {
                pos--;
                cursor--;

                vga[cursor]=(0x07<<8)|' ';
            }

            continue;
        }


        char c = keymap[sc];


        if(c)
        {
            buffer[pos++]=c;
            putchar(c);
        }
    }
}



bool strcmp(const char* a,const char* b)
{
    while(*a && *b)
    {
        if(*a!=*b)
            return false;

        a++;
        b++;
    }


    return *a==0 && *b==0;
}



void halt()
{
    while(true)
        asm volatile("hlt");
}



void shell()
{
    while(true)
    {
        print("> ");

        read_line();


        if(strcmp(buffer,"help"))
        {
            print(
                "Commands:\n"
                "help\n"
                "clear\n"
                "info\n"
                "halt\n\n"
            );
        }

        else if(strcmp(buffer,"clear"))
        {
            clear_screen();
        }

        else if(strcmp(buffer,"info"))
        {
            print(
                "Mode: protected mode\n"
                "Kernel: NasuaOS 32-bit\n"
                "Kernel 32-bit is basic shell not full os\n\n"
            );
        }

        else if(strcmp(buffer,"halt"))
        {
            halt();
        }

        else
        {
            print("Unknown command\n");
        }
    }
}



extern "C"
void kernel_main()
{
    disable_cursor();

    keyboard_init();

    clear_screen();


    print("NasuaOS 32-bit\n");


    shell();
}