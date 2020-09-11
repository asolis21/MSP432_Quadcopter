#include "UARTDEBUG.h"
#include "peripheral/uart_dev.h"

#include <stdint.h>
#include <stdarg.h>

void UARTDEBUG_init(int fclock, int baudrate)
{
    //Let us use UART0 (back-channel UART) to print debugging data
    uart_dev_init(UART0, fclock, baudrate);
}

void PrintString(char *string)
{
    /*while the string is not the null character*/
    while(*string)
    {
        uart_dev_print_char(UART0, *string);
        string++;
    }
}

/*Thanks! https://github.com/grbl/grbl/blob/master/grbl/print.c */
void PrintInteger(int integer)
{
    if(integer == 0)
    {
        uart_dev_print_char(UART0, '0');
    }

    if(integer < 0)
    {
        uart_dev_print_char(UART0, '-');
        integer = -integer;
    }

    char b[10];
    int digit = integer;

    uint8_t i = 0;
    while(digit)
    {
        b[i++] = digit % 10;
        digit /= 10;
    }

    while(i)
    {
        uart_dev_print_char(UART0, '0' + b[i-1]);
        i--;
    }
}

/*Thanks! https://github.com/grbl/grbl/blob/master/grbl/print.c */
void PrintFloat(float n, uint8_t decimal_places)
{
    if (n < 0)
    {
        uart_dev_print_char(UART0, '-');
      n = -n;
    }

    uint8_t decimals = decimal_places;
    while (decimals >= 2)
    {
      // Quickly convert values expected to be E0 to E-4.
      n *= 100;
      decimals -= 2;
    }

    if (decimals)
    {
      n *= 10;
    }

    // Add rounding factor. Ensures carryover through entire value.
    n += 0.5;

    // Generate digits backwards and store in string.
    uint8_t buf[32];
    uint8_t i = 0;
    uint32_t a = (long)n;
    // Place decimal point, even if decimal places are zero.
    buf[decimal_places] = '.';

    while(a > 0)
    {
    // Skip decimal point location
    if (i == decimal_places)
    {
        i++;
    }

    // Get digit
    buf[i++] = (a % 10) + '0';
    a /= 10;
    }

    while (i < decimal_places)
    {
      // Fill in zeros to decimal point for (n < 1)
     buf[i++] = '0';
    }

    // Fill in leading zero, if needed.
    if (i == decimal_places)
    {
    i++;
    buf[i++] = '0';
    }

    // Print the generated string.
    for (; i > 0; i--)
    {
        uart_dev_print_char(UART0, buf[i-1]);
    }
}

void UARTDEBUG_printf(const char *fs, ...)
{
    va_list valist;
    va_start(valist, fs);
    int i;
    char *s;
    float f;

    while(*fs)
    {
        if(*fs != '%')
        {
            uart_dev_print_char(UART0, *fs);
            fs++;
        }
        else
        {
            switch(*++fs)
            {
            case 'c':
                i = va_arg(valist, int);
                uart_dev_print_char(UART0, (char)i);
                break;
            case 's':
                s = va_arg(valist, char*);
                PrintString(s);
                break;
            case 'i':
                i = va_arg(valist, int);
                PrintInteger(i);
                break;
            case 'f':
                f = va_arg(valist, double);
                PrintFloat(f, 3);
                break;
            }

            ++fs;
        }
    }
}

int UARTDEBUG_gets(char *str, int length, bool terminal)
{
    char c;
    uint32_t i = 0;

    while(1)
    {
        c = uart_dev_get_char(UART0);

       /*put a '\n' and '\r' if it fits on the buffer*/
       if((c == '\n') || (c == '\r'))
       {
           if(i + 3 > length)
           {
               return length + 1;
           }

           str[i++] = '\r';
           str[i++] = '\n';
           str[i++] = 0;

           if(terminal)
           {
               PrintString("\r\n");
           }

           return i;
       }
       /*Erase data from buffer if backspace is received*/
       else if((c == 127) || (c == 8))
       {
           if(i > 0)
           {
               i--;
           }
           str[i] = 0;

           if(terminal)
           {
               PrintString("\x1B[1D");
               PrintString(" ");
               PrintString("\x1B[1D");
           }
       }
       /*Store character on the buffer*/
       else
       {
           if(i < length)
           {
               str[i++] = c;

               if(terminal)
               {
                   uart_dev_print_char(UART0, c);
               }
           }
           else
           {
               return length + 1;
           }
       }

    }
}
