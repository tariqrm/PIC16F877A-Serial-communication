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

// -------------- Prototypes --------------
void UART_Init(long baudRate);
void UART_TxChar(char ch);
char UART_RxChar(void);


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

    // Baud rate: SPBRG = (Fosc / (64 * baud)) - 1  [for BRGH=0]
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

void main(void)
{
    char input[10];
    char c;
    int i=0;
    int countD=0, countE=0, countF=0;
    
    UART_Init(9600);

    while(1)
    {
        char c = UART_RxChar(); // receiving from MCU1
        UART_TxChar(c); // transmit back

        if(c == '\n') 
        {
            break;  // Stop reading 
        }

        if(c == 'D'){
            countD++;
        }
        else if(c == 'E'){
            countE++;
        }
        else if(c == 'F'){
            countF++;
        }
    }
       
    UART_TxChar('#'); // to keep track of the counts  
    UART_TxChar(countD);
    UART_TxChar(countE);
    UART_TxChar(countF);
}
