#include "reg60s2.h"
#include <intrins.h>
#include <math.h>
#define DataPort P2
sbit rs=P0^0;
sbit e=P0^2;
sbit rw=P0^1;
long trigger,rawfreq;
unsigned char t,i,O,tlsave,thsave;
unsigned char *s;
unsigned code sheet[]={0x30,0x31,0x32,0x33,0x34,0x35,
0x36,0x37,0x38,0x39};

void DelayUs2x(unsigned char t)
{   
 while(--t);
}
void delay(unsigned char t)
{
     while(t--)
 	 {
     //????1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 	 }
}
bit LCD_Check_Busy(void) 
{ 
	_nop_();_nop_();_nop_();_nop_();
	DataPort=0xFF; 
 	rs=0; 
 	rw=1; 
 	e=0; 
 	_nop_();_nop_();_nop_();_nop_();
 	e=1;
	return (bit)(DataPort & 0x80);
}
void write_command (unsigned char com)
{
	 while(LCD_Check_Busy());
	 rs=0;
	 rw=0;
	 _nop_();_nop_();_nop_();_nop_(); 
	 DataPort=com;
	 _nop_();_nop_();_nop_();_nop_();
	 e=1;
	 _nop_();_nop_();_nop_();_nop_(); 
	 _nop_();_nop_();_nop_();_nop_(); 
	 e=0;
}
void write_data (unsigned char info)
{
	 while(LCD_Check_Busy());
	 rs=1;
	 rw=0;
	 _nop_();_nop_();_nop_();_nop_();
	 DataPort=info;
	 _nop_();_nop_();_nop_();_nop_();
	 e=1;
	 _nop_();_nop_();_nop_();_nop_();
	 _nop_();_nop_();_nop_();_nop_(); 
	 e=0;
}

void display(long f)
{
	write_command(0x80+0x42);
	if(f<100)
  	{
		write_data(sheet[f/10]);
		write_data('.');
		write_data(sheet[f%10]);
		write_data(' ');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');

	}
	else if(f<1000)
	{	
		write_data(sheet[f/100]);
		write_data(' ');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');
		write_data(' ');
	}
	else if(f<10000)
	{																							
	
		write_data(sheet[f/1000]);//?
		write_data('.');
		write_data(sheet[f%1000/100]);//?
		write_data(sheet[f%100/10]);
		write_data(sheet[f%10]);
		
		write_data(' ');
		write_data('k');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');
		write_data(' ');
	}
	else if(f<100000)
	{
		
		write_data(sheet[f/10000]);
		write_data(sheet[f%10000/1000]);
		write_data('.');
		write_data(sheet[f%1000/100]);
		write_data(sheet[f%100/10]);
		
		write_data(' ');
		write_data('k');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');
		write_data(' ');

	}
	else if(f<1000000)
	{
		write_data(sheet[f/100000]);
		write_data(sheet[f%100000/10000]);
		write_data(sheet[f%10000/1000]);
		write_data('.');
		write_data(sheet[f%1000/100]);
		
		write_data(' ');
		write_data('k');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');
		write_data(' ');
	}
	else if(f<10000000)
	{
		write_data(sheet[f/1000000]);
		write_data('.');
		write_data(sheet[f%1000000/100000]);
		write_data(sheet[f%100000/10000]);
		write_data(sheet[f%10000/1000]);
		
		write_data(' ');
		write_data('M');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');
		write_data(' ');
	}
	else if(f<100000000)
	{
		write_data(sheet[f/10000000]);
		write_data(sheet[f%10000000/1000000]);
		write_data('.');
		write_data(sheet[f%1000000/100000]);
		write_data(sheet[f%100000/10000]);
		
		write_data(' ');
		write_data('M');
		write_data('H');
		write_data('z');
		write_data(' ');
		write_data(' ');
		write_data(' ');
	}
}


void write_str(unsigned char *s) 
{             
	while (*s) 
	{     
		write_data( *s);     
		s ++;     
	}
}



void initial(void)		//25??@24MHz
{
//	AUXR &= 0xBF;		//?????12T??
	BRT=255;
	AUXR=0x14;
//	AUXR|=0x10;
	WAKE_CLKO=0x04;
	TMOD |= 0x15;		//???????
	TL1 = 0xB0;		//??????
	TH1 = 0x3C;		//??????
	TF1 = 0;		//??TF1??
	TR0=1;
	ET0=1;
	ET1=1;
	TR1=1;
	TR0=1;	//???1????
	EA=1;
	write_command(0x38);
	delay(5);
	write_command(0x38);
	delay(5);
	write_command(0x38);
	delay(5);	   
	write_command(0x38);
	write_command(0x08);
	write_command(0x01);
	write_command(0x06); 
	delay(3);
	write_command(0x0c);
	write_command(0x80+0x40);
	write_data('f');
	write_data('=');
	write_str(" Shawnigan Lake");
}



void main ()
{
initial();
while(1){}
}


void count () interrupt 1  //t0 count
{
  trigger+=65536;
}

void time () interrupt 3 
{
   
	TL1 = 0xB0;		//??????
	TH1 = 0x3C;	
	t++;
	
	if(t==10)
	{
		TR0=0;
		TR1=0;
		thsave=TH0;tlsave=TL0;
		TR0=0;
		TH0=0;TL0=0;
		trigger=trigger+thsave*256;
		trigger=trigger+tlsave;
		rawfreq=trigger;
		display(4*rawfreq);
		trigger=0;
		t=0;
		i++;
		if(i==8)
		{
			i=0;
			O++;
			switch (O){
			case 1:write_str(" Shawnigan Lake ");break;
			case 2:write_str("School Astronomy");break;
			case 3:write_str("  Antena Tuner  ");break;
			case 4:write_str("Coded-Dennis Ren");O=0;break;}
			
		}
		TR0=1;
		TR1=1;
	}
}