#include<msp430.h>

//PIN 595

#define SIN595  	 BIT7
#define SCLK595      BIT5
#define LTCH595 	 BIT6
#define N_channel	 16			//source channels

//PIN 5916
#define SIN5916  	 BIT0
#define SCLK5916     BIT3
#define LTCH5916 	 BIT4

//All port 2 interrupts
/*#define rLEFT        BIT0
#define rRIGHT       BIT1
#define rCENTRE      BIT2
#define bLEFT        BIT3
#define bRIGHT       BIT4
#define bCENTRE      BIT5
*/
#define arr_size	96

//unsigned char source[2]={0xFF,0xFF};
//unsigned char sink[2][6]={ {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
//							{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} };
//unsigned char sink[6]={ 0xAA,0xAA,0x00,0x00,0x55,0x55 };

unsigned char sink[96];

void InitializeClocks(void);
void SendData(void);

char temp;
char centreStep=3;
char bCentreCount=0,rCentreCount=0;
int i,j,k;

unsigned char strobe=0;
char rx_count=0;

void serial_println_8(char val)
{
 while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = val;
 while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = '\r';
 while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = '\n';
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = 0;
    P1DIR |= (SIN595 + SCLK595 + LTCH595) + (SIN5916 + SCLK5916 + LTCH5916);

    P2DIR = 0;
    //P2REN |= rLEFT + rRIGHT + rCENTRE + bLEFT + bRIGHT + bCENTRE;
    //P2OUT |=rLEFT + rRIGHT + rCENTRE + bLEFT + bRIGHT + bCENTRE;
    //P2IE |= rLEFT + rRIGHT + rCENTRE + bLEFT + bRIGHT + bCENTRE;
    //P2IFG &= ~(rLEFT + rRIGHT + rCENTRE + bLEFT + bRIGHT + bCENTRE);

    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
   	P1SEL2 = BIT1 + BIT2;
   	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
   	UCA0BR0 = 130;                           // 1MHz 9600
   	UCA0BR1 = 6;                           // 1MHz 9600
   	UCA0MCTL = UCBRS2 + UCBRS1;               // Modulation UCBRSx = 6
   	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    P1OUT = 0;
    InitializeClocks();						  // Setup clock
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
    _bis_SR_register(GIE);

    while (1)
    {   

    	if(strobe=='S')
   		{
   			serial_println_8('K');
 			strobe=0;
  		}

  		for(i=0; i<N_channel; i++)
 		{
    		SendData(); // Send current Step data to TLC5916
    		P1OUT|=LTCH5916;
   			P1OUT&=~(LTCH5916);

   			if(!i)
   			{
   				P2OUT|=SIN595;
   			}
   			else
   			{
   				P2OUT |= SIN595 >> i;
   			}

   			P2OUT|=SCLK595;
    		P2OUT&=~SCLK595;
    		P2OUT|=LTCH595;
    		P2OUT&=~LTCH595;

      		_delay_cycles(10000);
 		}

  		if(strobe=='S')
   		{
   			rx_count=0;
			serial_println_8('K');
  			strobe=0;
  		}
    }
}

void SendData(void)
{
	P1OUT &= ~(SIN5916 + LTCH5916 + SCLK5916);
	for (j = 0; j < 6; j++)
	{
		temp=sink[(6*i)+j];

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;

			P1OUT &= ~(SIN5916);
			P1OUT |= SIN5916&temp;
			P1OUT |= SCLK5916;
			P1OUT &= ~SCLK5916;
			temp = temp >> 1;
	}
}

void InitializeClocks(void)
{
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;
}

//RX interrupt
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  strobe= UCA0RXBUF;                    // TX -> RXed character
  if(!(strobe=='S'))
  {
	if(rx_count<arr_size)
		sink[rx_count++]=strobe;
  }
}

// Port 2 interrupt service routine
/*#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	if(P2IFG&bCENTRE)
		{
			bCentreCount++;
			if(bCentreCount!=centreStep)
					P2IFG=0;
		}
	else if(P2IFG&rCENTRE)
		{
			rCentreCount++;
			if(rCentreCount!=centreStep)
					P2IFG=0;
		}
	if(P2IFG!=0x00)
		{
			serial_println_8(P2IFG);
			P2IFG=0;
//			serial_println_8('K');
		}*/

	//while (!(IFG2&UCA0TXIFG)); // USCI_A0 TX buffer ready?
	//	UCA0TXBUF ='a';
//}