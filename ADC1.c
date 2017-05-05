/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC12C5Axx Series MCU A/D Conversion Demo -------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

#include "REG51.H"
#include <stdio.h>
#include <intrins.h>
#include <math.h>
#include <stdlib.h>

#define FOSC    24000000L 
#define BAUD    9600

typedef unsigned char BYTE;
typedef unsigned int WORD;

/*Declare SFR associated with the ADC */
sfr ADC_CONTR   =   0xBC;           //ADC control register
sfr ADC_RES     =   0xBD;           //ADC high 8-bit result register
sfr ADC_LOW2    =   0xBE;           //ADC low 2-bit result register
sfr P1ASF       =   0x9D;           //P1 secondary function control register

#define DataPort P2
sbit rs=P0^0;
sbit e=P0^2;
sbit rw=P0^1;

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks
#define sample_size 100


unsigned int xdata gold1[]={785,786,846,915,950,1115,1220,1285,1402,1535,1746,1912,2495,3500,4500,5620,6460,7510,8740,9500,10330,11830,12600,13860,14650,15780,16520,17560,18010,18810,19880,20310,21120,21760,22580,23180,23210};
unsigned int xdata sen1[]={0   ,2  ,115,300,370,700 ,920 ,1076,1310,1550,2025,2345,3597,5585,7715,10026,11751,14020,16590,18220,20000,23273,24930,27730,29424,31890,33470,35642,37950,39267,41185,41910,42840,43058,43160,43200,43210};
unsigned int xdata gold2[]={235	,237	,268	,323	,375	,596	,750	,851	,1081	,1222	,1501	,1798	,2316	,2890	,3678	,4607	,5590	,8630	,11030	,12250	,13400	,14870	,16110	,17480	,18760	,19350	,20120	,20200,23100};
unsigned int xdata sen2[]={0,15,38,123,210,590,860,1020,1395,1635,2095,2590,3440,4385,5690,7225,8870,13895,17900,20000,21960,24420,26600,28820,30970,31940,33120,33260,38000};
unsigned char toooooo=0b01010101;
void InitUart();
void InitADC();
BYTE GetADCResult(BYTE ch);
unsigned int ShowResult(BYTE ch);



void DelayUs2x(unsigned char t)
{   
 while(--t);
}
void delay(unsigned int t)
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
void write_str(unsigned char *s) 
{         
	while (*s) 
	{     
		write_data( *s);     
		s ++;     
	}
}

void main()
{
	char ch1good=0;
	float slope;
	unsigned int interval_size,interval_height;
	unsigned char cur;
	unsigned long ach1,ach2;
	unsigned int ich1,ich2;
	unsigned int xdata buf1[sample_size];
	unsigned int xdata buf2[sample_size];
	float ch1,ch2,bro,bro1;
	char bufff[10],buff[10];
	unsigned int i;
	InitUart();                     //Init UART, use to show ADC result
	InitADC();                      //Init ADC sfr
	printf("helooooooooooooooooooooooooooooo");
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
	write_command(0x80);
	write_str("Ti:saph    Fidel");
	delay(5);
	write_command(0x80+0x40);
	delay(5);
	write_str("   . W       . W");
	
	while (1)
	{
	//ShowResult(7);              //Show Channel0


		buf1[cur]=(ShowResult(0));	
		delay(1);
		buf2[cur]=(ShowResult(1));

		cur++;
		if(cur==sample_size){
			cur=0;
		
			ach1=0;
			for(i=0;i<sample_size;i++){
				ach1+=buf1[i];
			}
			ch1=(float)ach1/sample_size;
			ich1=(unsigned long)ch1*100;
			ch1good=0;
			
			write_command(0x80+0x40);
			if(ich1<=sen1[0]){
				write_str("< 75mW");
			}
			else if(ich1>sen1[36]){
				write_str("Max");
			}
			else{
				i=0;
				while(ich1>=sen1[i])
					i++;
				
				
				interval_size=gold1[i]-gold1[i-1];
				interval_height=sen1[i]-sen1[i-1];
				slope=(float)interval_size/interval_height;
				bro=(float)(gold1[i-1]+slope*(ich1-sen1[i-1]))/10;
				
		
				sprintf(bufff,"%4.0fmW  ",bro);
				write_str(bufff);
				printf(bufff);
				ch1good=1;
			}
			

			write_command(0x80+0x4a);
			ach2=0;
			i=0;
			for(i=0;i<sample_size;i++){
//				
				ach2+=buf2[i];
			}
			ch2=(float)ach2/sample_size;
			delay(1000);
			
			ich2=(float)ch2*100;


			if(ich2==0){
				write_str("< 23mW");
			}
			else if(ich2>=sen2[27]){
				write_str("Max");
			}
			else{
				i=0;
				while(ich2>=sen2[i])
					i++;
				interval_size=gold2[i]-gold2[i-1];
				interval_height=sen2[i]-sen2[i-1];
				slope=(float)interval_size/interval_height;
				bro1=(float)(gold2[i-1]+slope*(ich2-sen2[i-1]))/10;
				printf("i:%u #:%u, x:%u h:%u sl:%f ans:%f\n",i,ich2,interval_size,interval_height,slope,bro1);
				sprintf(buff,"%4.0fmW",bro1);
				write_str(buff);
				
			}
		}
	}
}

/*----------------------------
Send ADC result to UART
----------------------------*/
unsigned int ShowResult(BYTE ch)
{
	unsigned int bruh,dude;
	bruh=GetADCResult(ch);
	printf("Channel %u\t",(int)ch);
	dude=ADC_LOW2+bruh*4;
//	if (ch==1)
	//	printf("Value: %u\r",(unsigned int)dude);     //Show ADC high 8-bit result   
	return dude;
	//Show Channel NO.
	//if you want show 10-bit result, uncomment next line
//	SendData(ADC_LOW2);             //Show ADC low 2-bit result
}

/*----------------------------
Get ADC result
----------------------------*/
BYTE GetADCResult(BYTE ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //Return ADC result
}

/*----------------------------
Initial UART
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                    //8 bit data ,no parity bit
    TMOD = 0x20;                    //T1 as 8-bit auto reload
    TH1 = TL1 = 0xFD; //Set Uart baudrate
    TR1 = 1;                        //T1 start running
}

/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC()
{
    P1ASF = 0x00;                   //Open 8 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    delay(2);                       //ADC power-on and delay
} 


