//----------------------------------------------------------------------
//EASTRISING TECHNOLOGY CO,.LTD.//
// Module    : ER-TFT090-2  9 INCH TFT LCD  800*480
// Lanuage   : C51 Code
// Create    : JAVEN LIU
// Date      : 2015-01-26
// Drive IC  : RA8875     FLASH:W25Q128FV  128M BIT   FONT CHIP: 30L32S4W
// INTERFACE : 4Line SPI   
// MCU 		 : STC12LE5C60S2     1T MCU
// MCU VDD	 : 3.3V
// MODULE VDD : 5V OR 3.3V 
//----------------------------------------------------------------------

//===========================================================
#include <STC12C5A.H>
#include <stdio.h>
#include <intrins.h>
#include"RA8875.h"
//=============================================================
//write one byte to SD
sbit SD_CLK = P1^7;
sbit SD_DI  = P1^5;
sbit SD_DO  = P1^6;
sbit SD_CS  = P1^4;

#define uchar      unsigned char
#define uint       unsigned int
#define ulong      unsigned long

//===========================================================

//===========================================================
//Buffer
unsigned char xdata DATA[512];

//===========================================================
//write one byte to SD
void SdWrite(unsigned char n)
{

unsigned char i;

for(i=8;i;i--)
{
SD_CLK=0;
SD_DI=(n&0x80);
n<<=1;
SD_CLK=1;
}
SD_DI=1; 
} 
//===========================================================
//read one byte
unsigned char SdRead()
{
unsigned char n,i;
for(i=8;i;i--)
{
SD_CLK=0;
SD_CLK=1;
n<<=1;
if(SD_DO) n|=1;

}
return n;
}
//============================================================
//Detecting the response of the SD card
unsigned char SdResponse()
{
unsigned char i=0,response;

while(i<=8)
{
response = SdRead();
if(response==0x00)
break;
if(response==0x01)
break;
i++;
}
return response;
} 
//================================================================
//Send commands to the SD card
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{

SdWrite(command|0x40);
SdWrite(((unsigned char *)&argument)[0]);
SdWrite(((unsigned char *)&argument)[1]);
SdWrite(((unsigned char *)&argument)[2]);
SdWrite(((unsigned char *)&argument)[3]);
SdWrite(CRC);
}
//================================================================
//Initialize the SD card
unsigned char SdInit(void)
{
int delay=0, trials=0;
unsigned char i;
unsigned char response=0x01;

SD_CS=1;
for(i=0;i<=9;i++)
SdWrite(0xff);
SD_CS=0;

//Send Command 0 to put MMC in SPI mode
SdCommand(0x00,0,0x95);


response=SdResponse();

if(response!=0x01)
{
return 0;
} 

while(response==0x01)
{
SD_CS=1;
SdWrite(0xff);
SD_CS=0;
SdCommand(0x01,0x00ffc000,0xff);
response=SdResponse();
} 

SD_CS=1;
SdWrite(0xff);
return 1; 
}
//================================================================
//Specify the address to write  the SD card data, a maximum of 512 bytes
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
unsigned char dataResp;
//Block size is 512 bytes exactly
//First Lower SS

SD_CS=0;
//Then send write command
SdCommand(0x18,address,0xff);

if(SdResponse()==00)
{
SdWrite(0xff);
SdWrite(0xff);
SdWrite(0xff);
//command was a success - now send data
//start with DATA TOKEN = 0xFE
SdWrite(0xfe);
//now send data
for(count=0;count<len;count++) SdWrite(*Block++);

for(;count<512;count++) SdWrite(0);
//data block sent - now send checksum
SdWrite(0xff);
SdWrite(0xff);
//Now read in the DATA RESPONSE token
dataResp=SdRead();
//Following the DATA RESPONSE token
//are a number of BUSY bytes
//a zero byte indicates the MMC is busy

while(SdRead()==0);

dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
SD_CS=1;
SdWrite(0xff);
if(dataResp==0x0b)
{
//printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
return 0;
}
if(dataResp==0x05)
return 1;

//printf("Invalid data Response token.\n");
return 0;
}
//printf("Command 0x18 (Write) was not received by the MMC.\n");
return 0;
}

//=======================================================================
//Specified address from the SD card to read data, a maximum of 512 bytes
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
//Block size is 512 bytes exactly
//First Lower SS

 //printf("MMC_read_block\n");

SD_CS=0;
//Then send write command
SdCommand(0x11,address,0xff);

if(SdResponse()==00)
{
//command was a success - now send data
//start with DATA TOKEN = 0xFE
while(SdRead()!=0xfe);

for(count=0;count<len;count++) *Block++=SdRead(); 

for(;count<512;count++) SdRead();

//data block sent - now send checksum
SdRead();
SdRead();
//Now read in the DATA RESPONSE token
SD_CS=1;
SdRead();
return 1;
}
 //printf("Command 0x11 (Read) was not received by the MMC.\n");
return 0;
}


void Display_JPG()///SD card physical address of the first data address initial value
{ unsigned long jn;  
  unsigned int in;
  unsigned long AddTemp=314880;///SD card physical address of the first data address initial value
  unsigned char mn=5;  //Total number of picture

	Write_Dir(0x40,0x00);//Set the Graphics mode
	LCD_CmdWrite(0x02);//start write data	

	for(;mn!=0;mn--) 
	 {	Active_Window(0,799,0,479);
		   MemoryWrite_Position(0,0);	//cursor position

	 	for(jn=0;jn<1500;jn++)      //A picture information  comprising 1500x512 bytes
		    {
		    SdReadBlock(DATA,AddTemp+(jn*512),512);   //A picture information  comprising 1500x512 bytes
		    LCD_CmdWrite(0x02);//start data write
		    for(in=0;in<512;in++)                   
			   {   
			    LCD_DataWrite(DATA[in]);	 
		       }
		     } 
		     AddTemp = AddTemp+((jn+4)*512);         
			 
		 	NextStep(); 
	}
                       


} 
 

/////////////////////main////////////////////
void main(void)
{
	uint i;
	P0=0xff;
	P1=0xff;
	P2=0xff;
	P3=0xff;
    Delay100ms(5);

	//	LCD_Reset(); //RC Reset on board
	LCD_Initial();
	Write_Dir(0X01,0X80);//display on

	while(1)
		{

			//////////Touch function test
			//LCD_Reset();
			//LCD_Initial();
		
			Active_Window(0,799,0,479);//Set the working window size
		    Text_Foreground_Color1(color_white);//Set the foreground color
			Text_Background_Color1(color_blue);//Set the background color
			Write_Dir(0X8E,0X80);//Began to clear the screen (display window)
		    Chk_Busy();
		  	Write_Dir(0x21,0x10);//Select the internal CGROM  ISO/IEC 8859-1.
			Write_Dir(0x22,0x00);//Full alignment is disable.The text background color . Text don't rotation. 2x zoom
		  	FontWrite_Position(40,200);
		  	String("Touch to display the coordinate");

		  	Write_Dir(0xf0,0x04);//open interruption
		  	Write_Dir(0x71,0x00);//set to 4-wire touch screen
		  	Write_Dir(0x70,0xB2);//open the touch function, touch the parameter settings

			//Delay100ms(50);
  			while(next)
			{
		       Write_Dir(0xf1,0x04);//clear INT state      Must be clean TP_interrupt 
		       Delay10ms(5);
		       if(Touch_Status())
		       	{
			  		TP();
		       	}
			   else
			   	{
				  	FontWrite_Position(100,60); 
			      	LCD_CmdWrite(0x02);
				  	String("X = 0000");
				  	FontWrite_Position(100, 140); 
				  	LCD_CmdWrite(0x02);
				  	String("Y = 0000");	 
				}
	  		}

			NextStep();

			//full display test
			Test();


			/////The FLASH reading test
			Active_Window(0,799,0,479);//Set the work window size
			i=1;
			while(i<=8)
				{	Displaypicture(i);
					Delay100ms(5);   
					Chk_Busy();
					i+=1;
					NextStep();
				}



			/////External characters of the functional test
		    Text_Foreground_Color1(color_white);//Set the foreground color
			Text_Background_Color1(color_black);//Set the background color		
			Active_Window(0,799,0,479);;//Set the work window size
			Write_Dir(0X8E,0X80);//Start screen clearing (display window)
		    Chk_Busy();
			Write_Dir(0x21,0x20);//Select the external character
			Write_Dir(0x06,0x03);//Set the frequency
			Write_Dir(0x2E,0x80);//Font Write Type Setting Register Set up 32 x32 character mode     spacing   0 
			Write_Dir(0x2F,0x81);//Serial Font ROM Setting GT23L32S4W
			Write_Dir(0x05,0x28);// The waveform 3   2 byte dummy Cycle) 
		    Write_Dir(0x22,0x80);//Full alignment is enable.The text background color . Text don't rotation. 0x zoom		
		    Write_Dir(0x29,0x05);//Font Line Distance Setting
	
			FontWrite_Position(208,45);//Text written to the position
		    Write_Dir(0x40,0x80);//Set the character mode
		    LCD_CmdWrite(0x02);//start write data
		    String("深圳旭日东方科技有限公司");
		
			Text_Foreground_Color1(color_red);//Set the foreground color
			Write_Dir(0x2E,0x01);//Set the characters mode 16 x16 / spacing 1
		    FontWrite_Position(100,90);//Text written to the position
			String("TEL:755-33503874 FAX:755-33507642");
			FontWrite_Position(100,120);//Text written to the position
			String("WWW.BUY-DISPLAY.COM");
			FontWrite_Position(100,150);//Text written to the position
			String("E-mail:market@lcd-china.com");
			FontWrite_Position(100,180);//Text written to the position
			String("AD:Room 6G,Building A1,Zhujiang Square,Zhongxin Cheng,Longgang District,                      ShenZhen,China.");
		    Write_Dir(0x29,0x00);//Font Line Distance Setting
		    Write_Dir(0x22,0x05);//Full alignment is disable.The text background color . Text don't rotation. 2x zoom		
			Text_Foreground_Color1(color_green);//Set the foreground color
			Write_Dir(0x2E,0x00);//Set the characters mode 16 x16 / spacing 0
			FontWrite_Position(0x00,250);//Text written to the position
			String("ER-TFTM090-2，Optional Chinese / English character library,  MicroSD cord,Falsh.Font Support 2/3/4 times zoom."
		    "     Support8/16-bit 8080/6800 Series bus,Support serial 3/4wire SPI interface,I2C interface.Block Transfer Engine (BTE) Supports  with 2D，Geometry Accelerated Graphics Engine,Support DMA Direct Access FLASH。");
			Write_Dir(0x21,0x00);//Recovery of register
			Write_Dir(0x2F,0x00);//Recovery of register


			////////////RA8875 internal input character test
		    Text_Foreground_Color1(color_yellow);//Set the foreground color
		    Write_Dir(0x2E,0x01);//Set the characters mode 16 x16 / spacing 1
		    Write_Dir(0x40,0x80);//Set the character mode
		    Write_Dir(0x21,0x10);//Select the internal CGROM  ISO/IEC 8859-1.
		    FontWrite_Position(80,5);//Text written to the position
	

		    String("ShenZhen EastRising Technology .;LTD");
	
			Delay100ms(2);
			NextStep();



			//////////The cursor function test
		    Write_Dir(0x40,0xE0);//Set the text mode cursor
		    Write_Dir(0x41,0x00);//Close the graphics cursor
		    Write_Dir(0x44,0x1f);//The cursor flashing cycle
		    Write_Dir(0x4e,0x1f);//The cursor size
		    Write_Dir(0x4f,0x1f);//The cursor size
			Delay100ms(10);
			NextStep();
		
		    Write_Dir(0x21,0x00);//Recovery of register
		    Write_Dir(0x40,0x00);//Recovery of register

  
			////////PWM backlight control test    Need to short  J16 and open J15 on PCB .
   			Write_Dir(0x8b,0x0f);//Backlight brightness adjustment
			Delay100ms(3);
			NextStep();
   			Write_Dir(0x8b,0x3f);//Backlight brightness adjustment
			Delay100ms(3);
			NextStep();
   			Write_Dir(0x8b,0xff);//Backlight brightness adjustment
			Delay100ms(3);
			NextStep();


			//clear screen test:   part of the window 

		   	Write_Dir(0X8E,0X80);//Began to clear the screen (display window)
		   	Chk_Busy();		
		   	Active_Window(40,300,100,300);//Set the work window size
		   	Text_Background_Color1(color_green);//Set the background color
		   	Write_Dir(0X8E,0X40);//Set the screen clearing properties window (work window)
		   	Write_Dir(0X8E,0XC0);//Began to clear the screen
		   	Chk_Busy();
		     
		   	Active_Window(300,799,200,479);//Set the work window size
		   	Text_Background_Color1(color_cyan);//Set the background color
		   	Write_Dir(0X8E,0X40);//Set the screen clearing properties window (work window)
		   	Write_Dir(0X8E,0XC0);//Began to clear the screen
		   	Chk_Busy();

			/////////Memory write test
		   	Write_Dir(0x40,0x00);
			Active_Window(0,111,0,139);//Set the work window size	
		   	MemoryWrite_Position(0,0);//Memory write position
		   	LCD_CmdWrite(0x02);//start data write
  			//112X140 dot
  			for(i=0;i<31360;i++)
   			{
			    LCD_DataWrite(pic[i]);
				Chk_Busy();
		    }
			Delay100ms(3);
			NextStep();	
	
			/////// Geometric pattern drawing test
			Text_Background_Color1(color_black);//Set the background color
			Active_Window(0,799,0,479);;//Set the work window size
	 		Write_Dir(0X8E,0X40);//Set clear screen nature ( working window )
   			Write_Dir(0X8E,0XC0);//Began to clear the screen
			Chk_Busy();

			///////////Drawing curves
			Draw_Ellipse(210,120,205,105);
		    Text_Foreground_Color1(color_cyan);//Color Settings
		    Write_Dir(0XA0,0X10);//Setting parameters
		    Write_Dir(0XA0,0X90);//Start drawing
			Delay10ms(5);
		    Write_Dir(0XA0,0X91);//Start drawing
			Delay10ms(5);
		    Write_Dir(0XA0,0X92);//Start drawing
			Delay10ms(5);
		    Write_Dir(0XA0,0X93);//Start drawing
			Delay10ms(5);

			////////////drawing oval
			Draw_Ellipse(210,120,200,100);
			Text_Foreground_Color1(color_red);//Color Settings
			Write_Dir(0XA0,0X00);//Setting parameters
		    Write_Dir(0XA0,0X80);//Start drawing
			Delay10ms(5);
			Write_Dir(0XA0,0X40);//Set whether filling
		    Write_Dir(0XA0,0XC0);//Start drawing
			Delay10ms(5);
			/////////////drawing circle
			Draw_Circle(600,110,100);
			Text_Foreground_Color1(color_green);//Color Settings
			Write_Dir(0X90,0X00);//Setting parameters
		    Write_Dir(0X90,0X40);//Start drawing
			Delay10ms(10);		
			Write_Dir(0X90,0X20);//Setting parameters
		    Write_Dir(0X90,0X60);//Start drawing
			Delay10ms(10);
		 	/////////////drawing rectangle
		    Draw_Line(15,225,270,460);
		    Text_Foreground_Color1(color_blue);//Color Settings
			Write_Dir(0X90,0X10);//Setting parameters
		    Write_Dir(0X90,0X90);//Start drawing
		    Delay10ms(5);
			Write_Dir(0X90,0X30);//Setting parameters
		    Write_Dir(0X90,0XB0);//Start drawing
		    Delay10ms(5);
			///////////drawing triangle
			Draw_Line(300,420,460,270);
		    Draw_Triangle(540,460);//draw a triangle of three point
			Text_Foreground_Color1(color_purple);//Color Settings
		    Write_Dir(0X90,0X01);//Setting parameters
		    Write_Dir(0X90,0X81);//Start drawing
		    Delay10ms(5);
		    Write_Dir(0X90,0X21);//Setting parameters
		    Write_Dir(0X90,0XA1);//Start drawing
		    Delay10ms(5);
			///////////drawing rounded rectangle
		    Draw_Line(570,780,270,460);
		    Draw_Ellipse(0,0,20,30);//Set Radius
		    Text_Foreground_Color1(color_yellow);//Color Settings
		 	Write_Dir(0XA0,0X20);//Set whether filling
		    Write_Dir(0XA0,0XA0);//Start drawing
			Delay10ms(5);
		 	Write_Dir(0XA0,0X60);//Set whether filling
		    Write_Dir(0XA0,0XE0);//Start drawing
			Delay10ms(5);
			///////////drawing line
			Draw_Line(0,799,0,0);
		    Text_Foreground_Color1(color_red);//Color Settings
			Write_Dir(0X90,0X00);//Setting parameters
		    Write_Dir(0X90,0X80);//Start drawing
			Delay10ms(2);
			Draw_Line(799,799,0,479);//drawing line
		    Write_Dir(0X90,0X80);//Start drawing
			Delay10ms(2);
			Draw_Line(0,799,479,479);//drawing line
		    Write_Dir(0X90,0X80);//Start drawing
			Delay10ms(2);
			Draw_Line(0,0,0,479);//drawing line
		    Write_Dir(0X90,0X80);//Start drawing
			Delay10ms(2);
			NextStep();


			////////////BTE Color Fill
			BTE_Size(25,120);
    		Write_Dir(0x51,0xcc);//Raster Settings
			for(i=0;i<32;i++)
			{
				Text_Foreground_Color(i,0,0);
				BTE_Source(0,i*25,0,0);//BTE starting position			 				  
			    Write_Dir(0x50,0x80);//BET open
			    Chk_BTE_Busy();
			
				Text_Foreground_Color(0,i*2,0);
				BTE_Source(0,i*25,0,120);//BTE starting position		  
			    Write_Dir(0x50,0x80);//BET open
			    Chk_BTE_Busy();
			
				Text_Foreground_Color(0,0,i);
				BTE_Source(0,i*25,0,240);//BTE starting position			 				  
			    Write_Dir(0x50,0x80);//BET open
			    Chk_BTE_Busy();
			
				Text_Foreground_Color(i,i*2,i);
			    BTE_Source(0,i*25,0,360);//BTE starting position			 				  
			    Write_Dir(0x50,0x80);//BET open
			    Chk_BTE_Busy();
			}
			Delay100ms(2);
			NextStep();

			//////////BTE Color Expansion
		    Text_Background_Color1(color_purple);//Set the background color 
		    Text_Foreground_Color1(color_yellow);//Set the foreground color
		    BTE_Source(0,0,0,0);//BTE starting position
		    BTE_Size(120,100);//BTE size setting
		    Write_Dir(0x51,0x78);//Raster setting
		    Write_Dir(0x50,0x80);//BET open
			Chk_Busy();
		    LCD_CmdWrite(0x02);//start write data
  			for(i=0;i<1500;i++)
   			{
			    LCD_DataWrite(pic1[i]);
				Chk_Busy();
    		}
   			Chk_BTE_Busy();
			Delay100ms(2);
			NextStep();

			////////////BTE color expansion moves
  			BTE_Source(0,200,0,0);//BTE starting position
  			BTE_Size(120,100);//BBTE size setting
  			Text_Foreground_Color1(color_purple);//Set the foreground color (background color filter)
  			Write_Dir(0x51,0xc5);//start write data
  			Write_Dir(0x50,0x80);//BET open
  			Delay100ms(5); 
			NextStep();

			///////////Scroll function test
    		Scroll_Window(0,119,0,99);	//Specifies scrolling activity area
   			i=0; 
	 		while(i++<99){Delay10ms(10); Scroll(i,i);} //Note:  scroll offset value must be less than  scroll setting range
		    while(i-->0){Delay10ms(10); Scroll(i,i);}       
			while(i++<99){Delay10ms(10); Scroll(i,i);}
		    while(i-->0){Delay10ms(10); Scroll(i,i);}
			Delay100ms(5);
			NextStep();



		//show the SD card piture
		SdInit();        
		Display_JPG();        //SD card  picture
 		
		}



}




	
