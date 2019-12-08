//Gestione sonde temperatura, pompe, display.
//T1 Boiler	T2 Serpentina  T3 Scambiatore
//
//
//COLLEGAMENTI SULLE VARIE PORTE
//		TRISA	TRISB	TRISC	TRISD			TRISE
//	0			4		PStato	tutti i led		
//	1	T1		5		Min
//	2	T2		6		Ore
//	3	T3		7		
//	4			RS		
//	5			E				notte		
//	6	---		Pompa1		
//	7	---		Pompa2
//
//	
#include <pic.h>
#include <stdio.h>
#define XTAL_FREQ 20MHZ
#include "delay.c"
#include <lcd.c>
#define Nt 6
#define h 5
#define Tn 20	//Ora inizio notte
#define Tg 8	//Ora fine notte
#define Ta 30	//Temperatura minima acqua sanitaria
#define LED1 RD0
#define LED2 RD1
#define LED3 RD2
#define LED4 RD3
#define LED5 RD4
#define Pompa1 RB6
#define Pompa2 RB7
#define Notte RD5
unsigned long t1,t2,t3,t4,t5;
int S0,S1,S2,S3,S4,S5,S6,Tmil,Tsec,Tmin,Th,T1,T2,T3;
void main(void)
{
DelayMs(250);
DelayMs(250);
//Configurazione porte
TRISA=0xFF; //INGRESSI  
TRISB=0x00;	//USCITE
TRISC=0xFF;	//INGRESSI
TRISD=0x00;	//USCITE
//INIZIALIZZAZIONE STATI
S0=1;
S1=0;
S2=0;
S3=0;
S4=0;
S5=0;
S6=0;
//ADC
  ADCON1 = 0b10000010; 
//LCD
   DelayMs(250);
   LCD_INIT();
   DelayMs(250); 
   LCD_CLEAR();
DelayMs(250);
DelayMs(250);
//INIZIALIZZO TMR0
   T0CS=0;      //UTILIZZO CK INTERNO PER IL TIMER
   PSA=0;      //UTILIZZO PRESCALER 32
   PS2=1;
   PS1=0;
   PS0=0;
   TMR0=Nt;   //INIZIALIZZO TMR0 (SI INCREMENTA DA Nt a 255)
   T0IE=1;      //ABILITO INTERRUPT TIMER
   GIE=1;      //ABILITO INTERRUPT GENERALE
//INIZIALIZZO CONTEGGIO TEMPO T   
   Th=8;
   Tmin=00;
   Tsec=0;
//LED spenti all'avvio
PORTD=0x00;
//Uscite OFF all'avvio
PORTB=0x00;
	while (1)
	{
//Acquisizione temperature
   ADCON0=0b10001001;
   DelayMs(70);
   ADGO=1;
   while(ADGO==1)
	{};
   t1=ADRESL+(ADRESH<<8);
   ADCON0=0b10010001;
    DelayMs(70);
   ADGO=1;
   while(ADGO==1)
	{};
   t2=ADRESL+(ADRESH<<8);
   ADCON0=0b10011001;
   DelayMs(70);
   ADGO=1;
   while(ADGO==1)
	{};
   t3=ADRESL+(ADRESH<<8);
//Cambio range
T1=(t1*500/1024);
T2=(t2*500/1024);
T3=(t3*500/1024);
//Gestione pompa liquido
	if(T1>=(T3+h))
		{
		Pompa1=1;
		}
	else
	{
		if(T1<(T3-h))
		{
		Pompa1=0;
		}
	}
//Gestione pompa acqua sanitaria
	if((T2>=(T3+h))&(T2>=30))
		{
		Pompa2=1;
		}
	else
	{
		if(T2<(T3-h))
		{
		Pompa2=0;
		}
	}
//Ciclo notturno
if ((Th>Tg)&(Th<Tn))
{
Notte=0;
}
else
{
Notte=1;
}
//REGOLAZIONE ORA E MINUTI
    if (RC1==1)
	{
	Tmin=Tmin+1;
	DelayMs(250);
	DelayMs(250);
	}
    if (RC2==1)
	{
	Th=Th+1;
	DelayMs(250);
	DelayMs(250);
	}
//AGGIORNAMENTO STATI
if ((S0==1)&(RC0==1))
{
S0=0;
S1=1;
S2=0;
S3=0;
S4=0;
S5=0;
S6=0;
DelayMs(250);
DelayMs(250);
}
if ((S1==1)&(RC0==1))
{
S0=0;
S1=0;
S2=1;
S3=0;
S4=0;
S5=0;
S6=0;
DelayMs(250);
DelayMs(250);
}
if ((S2==1)&(RC0==1))
{
S0=0;
S1=0;
S2=0;
S3=1;
S4=0;
S5=0;
S6=0;
DelayMs(250);
DelayMs(250);
}
if ((S3==1)&(RC0==1))
{
S0=0;
S1=0;
S2=0;
S3=0;
S4=1;
S5=0;
S6=0;
DelayMs(250);
DelayMs(250);
}
if ((S4==1)&(RC0==1))
{
S0=0;
S1=0;
S2=0;
S3=0;
S4=0;
S5=1;
S6=0;
DelayMs(250);
DelayMs(250);
}
if ((S5==1)&(RC0==1))
{
S0=0;
S1=0;
S2=0;
S3=0;
S4=0;
S5=0;
S6=1;
DelayMs(250);
DelayMs(250);
}
if ((S6==1)&(RC0==1))
{
S0=1;
S1=0;
S2=0;
S3=0;
S4=0;
S5=0;
S6=0;
DelayMs(250);
DelayMs(250);
}
//AGGIORNAMENTO USCITE
if(S0==1)
{
    LED5=0;
	LCD_CLEAR();
	LCD_CMD(LCD_line1);
	LCD_PUTS("    ");
	LCD_PUTUN(Th);
	LCD_PUTS(":");
	LCD_PUTUN(Tmin);
	LCD_PUTS(":");
	LCD_PUTUN(Tsec);
	DelayUs(10);
	LCD_CMD(LCD_line2);
	LCD_PUTS("T");
    LCD_PUTCH(0b11011111);
	LCD_PUTS("Boiler ");	
	LCD_PUTUN(T3);				//Temperatura boiler
	DelayUs(10);
    LCD_PUTCH(0b11011111);
    LCD_PUTS("C");
}
if(S1==1)
{
    LED1=1;
    LED2=0;
    LED3=0;
    LED4=0;
    LED5=0;
	LCD_CLEAR();
	LCD_CMD(LCD_line1);
    LCD_PUTS("In Liquido");
	LCD_CMD(LCD_line2);
	LCD_PUTUN(T1);
	DelayMs(1);
    LCD_PUTCH(0b11011111);
    LCD_PUTS("C");
}
if(S2==1)
{
    LED1=0;
    LED2=1;
    LED3=0;
    LED4=0;
    LED5=0;
	LCD_CLEAR();
	LCD_CMD(LCD_line1);
    LCD_PUTS("Out Liquido");
	LCD_CMD(LCD_line2);
	LCD_PUTUN(T2);
	DelayMs(1);
    LCD_PUTCH(0b11011111);
    LCD_PUTS("C");
}
if(S3==1)
{
    LED1=0;
    LED2=0;
    LED3=1;
    LED4=0;
    LED5=0;
    LCD_CLEAR();
	LCD_CMD(LCD_line1);
    LCD_PUTS("     Boiler");
	LCD_CMD(LCD_line2);
	LCD_PUTUN(T3);
	DelayMs(1);
    LCD_PUTCH(0b11011111);
    LCD_PUTS("C");
}
if(S4==1)
{
    LED1=0;
    LED2=0;
    LED3=0;
    LED4=1;
    LED5=0;
    LCD_CLEAR();
	LCD_CMD(LCD_line1);
    LCD_PUTS("Scambio termico");
	LCD_CMD(LCD_line2);
	LCD_PUTSN(T1-T2);
	DelayMs(1);
    LCD_PUTCH(0b11011111);
    LCD_PUTS("C");
}
if(S5==1)
{
    LCD_CLEAR();
	LCD_CMD(LCD_line1);
    LCD_PUTS("     Pompa 1");
	LCD_CMD(LCD_line2);
	if(Pompa1==1)
	{
	LCD_PUTS("Accesa");
	}
	else
	{
	LCD_PUTS("Spenta");
	}
}
if(S6==1)
{
    LCD_CLEAR();
	LCD_CMD(LCD_line1);
    LCD_PUTS("     Pompa 2");
	LCD_CMD(LCD_line2);
	if(Pompa2==1)
	{
	LCD_PUTS("Accesa");
	}
	else
	{
	LCD_PUTS("Spenta");
	}
}
}
}
//RISPOSTA ALL'INTERRUPT (GENERATO DA TMR0 OGNI 1.6ms)
interrupt void T_1_6ms(void)
{
	Tmil=Tmil+1;   //INCREMENTO IL CONTEGGIO DEI MILLI SEC
    TMR0=Nt;      //INIZIALIZZO IL TIMER
    T0IF=0;         //AZZERO IL FLAG DI AVVENUTO INTERRUPT
	if(Tmil>=625)
      {
         Tmil=0;
         Tsec=Tsec+1;        
      }
      if(Tsec==60)
      {
         Tsec=0;
         Tmin=Tmin+1;
      }
      if((Tmin==60)&(RC1==0))
      {
		 Th=Th+1;
      }
	  if(Tmin>=60)
	  {
	  	Tmin=0;
	  }
      if(Th==24)
      {
         Th=0;
     }
}