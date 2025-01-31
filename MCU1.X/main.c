#include <xc.h>

#pragma config FOSC  = HS    
#pragma config WDTE  = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP   = OFF
#pragma config CPD   = OFF
#pragma config WRT   = OFF
#pragma config CP    = OFF

#define _XTAL_FREQ 20000000

// ------------ Prototypes ------------
void UART_Init(long baudRate);
void UART_TxChar(char ch);
char UART_RxChar(void);

void send16Bit(char address, char dataByte);
void displayMessage(const char *msg);  
void displayCount(char letter, char count, char suffix);


void UART_Init(long baudRate)
{
    // RC6 = TX (output), RC7 = RX (input)
    TRISC = 0x80;
    PORTC= 0x00;
    
    TX9=0;
    SYNC=0;
    TXEN=1;
    
    SPEN=1;
    CREN=1;

    SPBRG = (int)((20000000 / (64 * (baudRate + 1))));
}

void UART_TxChar(char ch)
{
    while(TXIF==0);  
    TXIF = 0;      
    TXREG = ch;    
}

char UART_RxChar(void)
{
    while(RCIF==0);  
    RCIF = 0;      
    return RCREG;  
}

// We assume RB0 -> DATA, RB1 -> CS, RB2 -> CLK

void send16Bit(char address, char dataByte)
{
    int word = ((int)address << 8) | (dataByte & 0xFF);
    int i;

    // Bring CS high, then low
    RB1 = 1;
    __delay_ms(50);
    RB1 = 0;
    __delay_ms(50);

    for(i = 0; i < 16; i++)
    {
        if(word & 0x8000) 
        { 
            RB0 = 1; 
        }
        else              
        { 
            RB0 = 0; 
        }

        __delay_ms(50);

        // Clock high
        RB2 = 1;
        __delay_ms(50);

        // Clock low
        RB2 = 0;
        __delay_ms(50);

        word <<= 1;
    }

    RB1 = 1;
    __delay_ms(50);
}

void display4Chars(char c1, char c2, char c3, char c4)
{
    send16Bit(0x20, c1);
    send16Bit(0x21, c2);
    send16Bit(0x22, c3);
    send16Bit(0x23, c4);
}

void displayCount(char letter, char count, char suffix)
{
    // letter, '=', digit, suffix
    
    display4Chars(letter, 0x3D, (0x30 + count), suffix);

    // Clear  display
    display4Chars(0x20, 0x20, 0x20, 0x20);
}

void main(void)
{
    
    // Configure PORTB for bit output
    TRISB = 0x00;
    PORTB = 0x00;
    send16Bit(0x04, 0x81);  // Configuration
    send16Bit(0x01, 0xFF);  // Intensity
    send16Bit(0x02, 0xFF);  // Intensity 32
    send16Bit(0x07, 0x00);  // Display test off
    
    UART_Init(9600);

    const char text[] =
    "The MAX6952 is a compact serial input/output display driver designed to interface "
    "microcontrollers with dot-matrix LED displays. This chip supports SPI and various other serial "
    "communication protocols, requiring only three control lines from your microcontroller to "
    "effectively drive the display.\n";
    
    int i = 0;
    while (text[i] != '\0')
    {
        UART_TxChar(text[i]);
        UART_RxChar();
        i++;
    }
    
    char c;
    char input[2];
     
    while(1)
    {
        c = UART_RxChar();
        if(c=='#'){
            for(i=0; i<3;i++){ // received DEF          
                c = UART_RxChar();
                input[i]=c;
            }
            
            // Display "D=x,"
            displayCount(0x44, input[0], 0x2C); // ','=0x2C
           
            // Display "E=x,"
            displayCount(0x45, input[1], 0x2C); // ','=0x2C
            
            // Display "F=x."
            displayCount(0x46, input[2], 0x2E); // '.'=0x2E
        }
    }
}
