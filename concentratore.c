//Gestione movimentazione concentratore, manuale, automatica.
//Soglie di intervento nel ciclo automatico (36, 37)
//Uscite ai motori (Finecorsa software)
//
//COLLEGAMENTI SULLE VARIE PORTE
//		TRISA	TRISB	TRISD
//	0			Man		MDx
//	1	A		Auto	MSx
//	2	B		Dx		MUp
//	3	C		Sx		MDown
//	4			Up		FcDx
//	5	D		Down	FcSx	
//	6	---		Notte	FcUp
//	7	---				FcDown
#include <pic.h>
#include <stdio.h>
#define XTAL_FREQ 20MHZ
#include "delay.c"
//Motori
#define MDx RD0
#define MSx RD1
#define MUp RD2
#define MDown RD3
//Ingressi
#define Man RB0
#define Auto RB1
#define Dx RB2
#define Sx RB3
#define Up RB4
#define Down RB5
#define Notte RB6
//Finecorsa
#define FcDx RD4
#define FcSx RD5
#define FcUp RD6
#define FcDown RD7
//Soglia intervento automatico
#define M -40
#define N 50
#define R 50
#define S -30
unsigned int A,B,C,D,a,b,c,d;
int MDX,MSX,MUP,MDOWN;
signed int X,Y;
int leggi_ad(char canale);
void main(void)
{ 
DelayMs(250);
DelayMs(250);
DelayMs(250);
DelayMs(250);
//Configurazione porte
   TRISA=0xFF; 
   TRISB=0xFF; 
   TRISD0=0;
   TRISD1=0;
   TRISD2=0;
   TRISD3=0;
   TRISD4=1;
   TRISD5=1;
   TRISD6=1;
   TRISD7=1;
//ADC
   ADCON1 = 0b10000000;
//Programma
   for (;;)
	{
//Acquisizione valori puntamento
	if ((Auto==1)&(Notte==0))
	{
	A= leggi_ad(0b10001001);
 	B= leggi_ad(0b10010001);
 	C= leggi_ad(0b10011001);
 	D= leggi_ad(0b10100001);
	a=A;
	b=B;
	c=C;
	d=D;
 	Y=(a-c);  //Variabile verticale
    X=(a-b);  //Variabile orizzontale 
//CICLO AUTOMATICO	
		if ((X<M)&(Y<50))
			{MDX=1;}
		else
	{	if ((X>-20)&(X<0))
			{MDX=0;}}
		if (X>N)
			{MSX=1;}
		else
	{	if ((X>0)&(X<20))
			{MSX=0;}}
		if (Y<S)
			{MUP=1;}
		else
	{	if ((X>-10)&(X<0))
			{MUP=0;}}
		if (Y>R)
			{MDOWN=1;}
		else
	{	if ((X>0)&(X<20))
			{MDOWN=0;}}
	}
//CICLO MANUALE
	if (Man==1)
	{
		if (Dx==1)
			{MDX=1;}
		else
			{MDX=0;}
		if (Sx==1)
			{MSX=1;}
		else
			{MSX=0;}
		if (Up==1)
			{MUP=1;}
		else
			{MUP=0;}
		if (Down==1)
			{MDOWN=1;}
		else
			{MDOWN=0;}
	}
//FASE NOTTE
	if ((Notte==1) & (Man==0))
	{
		if (FcSx==0)
		{
			MSX=1;
		}
		else
		{
			MSX=0;
		}
		if (FcDown==0)
			{
			MDOWN=1;
		}
		else
		{
			MDOWN=0;
		}
	}
//USCITE CON FINECORSA
	if ((MDX==1)&(FcDx==0))
		{MDx=1;}
	else
		{MDx=0;}
	if ((MSX==1)&(FcSx==0))
		{MSx=1;}
	else
		{MSx=0;}
	if ((MUP==1)&(FcUp==0))
		{MUp=1;}
	else
		{MUp=0;}
	if ((MDOWN==1)&(FcDown==0))
		{MDown=1;}
	else
		{MDown=0;}
	}
}
int leggi_ad(char canale)
{
   int valore;
   ADCON0=canale;
   DelayMs(150);
   ADGO = 1;
   while(ADGO==1)
	{};
   valore=ADRESL+(ADRESH<<8);
   return(valore);   
}