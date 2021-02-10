
//=============================================================
//Definition LCD screen interface

sbit	LCD_cs=P1^0;		//chip select
sbit	BL_ON=P2^6;		//External backlight control signal
sbit	rs=P1^1;		//data/command
sbit	wrb=P1^2;	     //write
sbit	rdb=P1^3;	     //read
sbit 	wait=P2^0;	     //Wait Signal Output)
sbit 	_int=P2^1;	     //Interrupt signal
sbit 	rst=P2^3;  		//reset 
sbit KEY    = P2^7;
#define HDATABUS	P3                  //high 8 bit
#define LDATABUS	P0                  //low 8 bit


typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;

#define	cSetD0		     0x01
#define	cSetD1		     0x02
#define	cSetD2		     0x04
#define	cSetD3		     0x08
#define	cSetD4		     0x10
#define	cSetD5		     0x20
#define	cSetD6		     0x40
#define	cSetD7		     0x80

#define	cClrD0		     0xfe
#define	cClrD1		     0xfd
#define	cClrD2		     0xfb
#define	cClrD3		     0xf7
#define	cClrD4		     0xef
#define	cClrD5		     0xdf
#define	cClrD6		     0xbf
#define	cClrD7		     0x7f

#define cClrD1D0         0xfc
#define cClrD1SetD0	     0x01
#define cSetD1ClrD0	     0x02
#define cSetD1D0		 0x03
#define cClrD3D2         0xf3
#define cClrD3SetD2      0x04
#define cSetD3ClrD2      0x08
#define cSetD3D2		 0x0c

#define cClrD2D1D0		 0xf8
#define cClrD2D1SetD0	 0x01
#define cClrD2SetD1ClrD0 0x02
#define cClrD2SetD1D0	 0x03
#define cSetD2ClrD1D0	 0x04
#define cSetD2ClrD1SetD0 0x05

#define cClrD6D5D4	     0x8f
#define cClrD6D5SetD4    0x10
#define cClrD6SetD5ClrD4 0x20
#define cClrD6SetD5D4	 0x30
#define cSetD6ClrD5D4	 0x40
#define cSetD6ClrD5SetD4 0x50
#define cSetD6D5ClrD4	 0x60
#define cSetD6D5D4		 0x70
/*
#define color_black   0x0000
#define color_white   0xffff
#define color_red     0xf800
#define color_green   0x07e0
#define color_blue    0x001f
#define color_yellow  color_red|color_green
#define color_cyan    color_green|color_blue
#define color_purple  color_red|color_blue
*/
unsigned char code PIC[];

//====================================================//

 void RA8875_Initial(void);
 void LCD_all(unsigned int i);
 void LCD_SetPos(unsigned int X,unsigned int Y);
 void delay(unsigned int ms);
 void delayus(unsigned int del);
void Write_Data_int(unsigned int );
void Write_Data_byte(unsigned char DH,unsigned char DL);
void write_command(unsigned char c);
void Chk_Wait(void);


void delayus(unsigned int del)
{

    if(del--);

}


void delay(unsigned int ms)
{unsigned int ti;
 while(ms--)
 { 
  for(ti=0;ti<1000;ti++){}//112
 }
}
void write_command(unsigned char c)
{
	LCD_cs=0;
	 rs=1;
 	 P3=0x00;
	 P0=c;
	 wrb=0;
	 wrb=1; 
	LCD_cs=1;
}



void Write_Data_byte(unsigned char DH,unsigned char DL)
{
	LCD_cs=0;
	 rs=0;
	 P3=DH;
	 P0=DL;
	 wrb=0;
	 wrb=1;  
	LCD_cs=1;
}

    
void Write_Data_int(unsigned int d)
{
	LCD_cs=0;
	 rs=0;
	 P3=d>>8;
	 P0=d;
	 wrb=0;
	 wrb=1;  
	LCD_cs=1;
}

unsigned int LCD_DataRead(void)
{uint Data=0;uchar d1,d2;
Chk_Wait();
	P0=0xff;
	P3=0xff;
	LCD_cs=0;
	_nop_();
	 _nop_();
	 rs=0;
	 _nop_();
	 _nop_();
	 rdb=0;
	 _nop_();
	 _nop_();
	d1=P0;
	d2=P3;
	 _nop_();
	 _nop_();
	 _nop_();
	 rdb=1;  
	 _nop_();
	 _nop_();
	 _nop_();
	LCD_cs=1;
	 _nop_();
	 _nop_();
	Data=d2;
	Data<<=8;
	Data|=d1;
	return Data;
}

uchar LCD_StatusRead(void)
{
		uchar Data;	
	Chk_Wait();
	P0=0xff;
	LCD_cs=0;
	 _nop_();
	 _nop_();
	 rs=1;
	 _nop_();
	 _nop_();
	 rdb=0;
	 _nop_();
	 _nop_();
	Data=P0;
	 _nop_();
	 _nop_();
	 _nop_();
	 rdb=1; 
	 _nop_();
	 _nop_();
	 _nop_();
	LCD_cs=1;
	 _nop_();
	 _nop_();
	return Data;
}

//------------------------//
//STATUS 
void Chk_Busy(void)
{
	uchar temp; 	
	do
	{
	temp=LCD_StatusRead();
	}while((temp&0x80)==0x80);
			   
}

void Chk_Busy_BTE(void)
{
	uchar temp;
   	
	do
	{
     temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);	   
}

void Chk_Busy_DIR_Access(void)
{
	uchar  temp;
   	
	do
	{
     temp=LCD_StatusRead();
	}while((temp&0x01)==0x01);	   
}

//REG[BFh]
void Chk_DMA_Busy(void)
{
	uchar temp; 	
	do
	{
	write_command(0xBF);
	temp = LCD_DataRead();
	}while((temp&0x01)==0x01);   
}


//REG[01h]		 

void Display_ON(void)
{	
    write_command(0x01);//PWRR
	Write_Data_int(0x80);
}

void Display_OFF(void)
{
	write_command(0x01);//PWRR
	Write_Data_int(0x00);
}

void Normal_Mode_exitSleep(void)
{	
    write_command(0x01);//PWRR
    Write_Data_int(0x00);
}
void Sleep_Mode(void)
{
	write_command(0x01);//PWRR
	Write_Data_int(0x02);
}
void Software_Reset(void)
{	
	write_command(0x01);//PWRR
    Write_Data_int(0x01);
    Write_Data_int(0x00);
    delay(1);// 
}							   
//-------------------------------//
//REG[04h] for test 
void PCLK_inversion(void)
{ 	uchar temp;
  	write_command(0x04);//PWRR
	temp = LCD_DataRead();
	temp |= cSetD7;
	Write_Data_int(temp);
}
void PCLK_non_inversion(void)
{ 	uchar temp;
  	write_command(0x04);//PWRR
	temp = LCD_DataRead();
	temp &= cClrD7;
	Write_Data_int(temp);
}

void PCLK_width(uchar setx) //bit[1:0]
{ 	uchar temp;
  	write_command(0x04);//PWRR
	temp = LCD_DataRead();
	temp &=0x80;
    temp |= setx;
	Write_Data_int(temp);
}

//-------------------------------//
//REG[05h]
void Serial_ROM_select0(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD7;
	Write_Data_int(temp);
}

void Serial_ROM_select1(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD7;
	Write_Data_int(temp);
}

void Serial_ROM_Address_set_24bit(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD6;
	Write_Data_int(temp);
}

void Serial_ROM_Address_set_32bit(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD6;
	Write_Data_int(temp);
}

void Select_Serial_Waveform_mode0(void)
{	uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD5;
	Write_Data_int(temp);
}

void Select_Serial_Waveform_mode3(void)
{	uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD5;
	Write_Data_int(temp);
}

void SERIAL_ROM_Read_Cycle_4bus(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
	Write_Data_int(temp);
}

void SERIAL_ROM_Read_Cycle_5bus(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
    temp |=cSetD3;
	Write_Data_int(temp);
}

void SERIAL_ROM_Read_Cycle_6bus(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
    temp |=cSetD4;
	Write_Data_int(temp);
}

void SERIAL_ROM_Font_mode(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD2;
	Write_Data_int(temp);
}

void SERIAL_ROM_DMA_mode(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD2;
	Write_Data_int(temp);
}

void SERIAL_ROM_Signal_mode(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
	Write_Data_int(temp);
}

void SERIAL_ROM_Dual_mode0(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
    temp |=cSetD1;
	Write_Data_int(temp);
}

void SERIAL_ROM_Dual_mode1(void)
{   uchar temp;
  	write_command(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
    temp |=cSetD1;
    temp |=cSetD0;
	Write_Data_int(temp);
}

void SROM_CLK_DIV(uchar CLK_DIV)
{
	write_command(0x06);
    Write_Data_int(CLK_DIV);
}


//-------------------------------//
//REG[10h]
void Color_256(void)
{
	uchar temp;
	write_command(0x10);//SYSR
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	Write_Data_int(temp);
} 

void Color_65K(void)
{
	uchar temp;
	write_command(0x10);//SYSR
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	write_command(0x10);//SYSR
	Write_Data_int(temp);
}
void MPU_8bit_Interface(void)
{
	uchar temp;
	write_command(0x10);//SYSR
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	Write_Data_int(temp);
}

void MPU_16bit_Interface(void)
{
	uchar temp;
	write_command(0x10);//SYSR
	temp = LCD_DataRead();
	temp |= cSetD1ClrD0 ;
	Write_Data_int(temp);
} 								 
//--------------------------------//
//REG[12h] 

uchar GPI_data(void)
{   uchar temp;
	write_command(0x12);//SYSR
	temp = LCD_DataRead();
	return temp;
}

void GPO_data(uchar setx)
{
  write_command(0x13); 
  Write_Data_int(setx);
}

							
//---------------------------------//
//REG[20h]

void One_Layer(void)
{
	uchar temp;
	write_command(0x20);//DPCR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}	
				 
void Two_Layers(void)
{	uchar temp;
	write_command(0x20);//DPCR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);
}

void HDIR_SEG0_SEGn(void)
{	uchar temp;
	write_command(0x20);//DPCR
	temp = LCD_DataRead();
	temp &= cClrD3 ;
	Write_Data_int(temp);
}
void HDIR_SEGn_SEG0(void)
{
	uchar temp;

	write_command(0x20);//DPCR
	temp = LCD_DataRead();
	temp |= cSetD3 ;
	Write_Data_int(temp);
}
void VDIR_COM0_COMn(void)
{
	uchar temp;

	write_command(0x20);//DPCR
	temp = LCD_DataRead();
	temp &= cClrD2 ;
	Write_Data_int(temp);
}
void VDIR_COMn_COM0(void)
{
	uchar temp;

	write_command(0x20);//DPCR
	temp = LCD_DataRead();
	temp |= cSetD2 ;
	Write_Data_int(temp);
}


//----------------------------------//
//REG[21h] 
void CGROM_Font(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}
void CGRAM_Font(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);
}

void Internal_CGROM(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD5 ;
	Write_Data_int(temp);
}
void External_CGROM(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp |= cSetD5 ;
	Write_Data_int(temp);
}

void ISO8859_1(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	Write_Data_int(temp);
}
void ISO8859_2(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	Write_Data_int(temp);
}
void ISO8859_3(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	Write_Data_int(temp);
}
void ISO8859_4(void)
{
	uchar temp;
	write_command(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	Write_Data_int(temp);
}								 
//--------------------------------//
//REG[22h]

void No_FullAlignment(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}
void FullAlignment(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);
}
void Font_with_BackgroundColor(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	Write_Data_int(temp);
}
void Font_with_BackgroundTransparency(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	Write_Data_int(temp);
}


void NoRotate_Font(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	Write_Data_int(temp);
}
void Rotate90_Font(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp |= cSetD4 ;
	Write_Data_int(temp);
}


void Horizontal_FontEnlarge_x1(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	Write_Data_int(temp);
}
void Horizontal_FontEnlarge_x2(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	Write_Data_int(temp);
}
void Horizontal_FontEnlarge_x3(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	Write_Data_int(temp);
}
void Horizontal_FontEnlarge_x4(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	Write_Data_int(temp);
}
void Vertical_FontEnlarge_x1(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	Write_Data_int(temp);
}
void Vertical_FontEnlarge_x2(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	Write_Data_int(temp);
}
void Vertical_FontEnlarge_x3(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	Write_Data_int(temp);
}
void Vertical_FontEnlarge_x4(void)
{
	uchar temp;
	write_command(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	Write_Data_int(temp);
}

//-------------------------------------------//
//REG[23h]									 
void CGRAM_Select_Number(uchar setx)
{ 
    write_command(0x23);//CGSR
	Write_Data_int(setx);
}	
								
//-------------------------------------------//
//REG[24h]REG[25h]REG[26h]REG[27h]
void Scroll_Offset(uint X,uint Y)
{

    write_command(0x24);//HOFS0
	Write_Data_int(X);
  
    write_command(0x25);//HOFS1	   
	Write_Data_int(X>>8);

	  
    write_command(0x26);//VOFS0
	Write_Data_int(Y);  

    write_command(0x27);//VOFS1	   
	Write_Data_int(Y>>8); 
}	 
//-------------------------------------------//
//REG[29h]
void Line_distance(uchar setx)
{ 
    write_command(0x29);//LDS
	Write_Data_int(setx);
}	  
//-------------------------------------------//
//REG[2Ah]REG[2Bh]REG[2Ch]REG[2Dh]
void Font_Coordinate(uint X,uint Y)
{
 
    write_command(0x2A);//HOFS0
	Write_Data_int(X);

    write_command(0x2B);//HOFS1	   
	Write_Data_int(X>>8);

 
    write_command(0x2C);//VOFS0
	Write_Data_int(Y);

    write_command(0x2D);//VOFS1	   
	Write_Data_int(Y>>8); 
}


//-------------------------------------------//
//REG[2Eh]
void Font_size_16x16_8x16(void) 
{
 	uchar temp;
	write_command(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	Write_Data_int(temp);
}

void Font_size_24x24_12x24(void) 
{
 	uchar temp;
	write_command(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	temp |= 0x40; 
	Write_Data_int(temp);
}

void Font_size_32x32_16x32(void) 
{
 	uchar temp;
	write_command(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	temp |= 0x80; 
	Write_Data_int(temp);
}


void Font_spacing_set(uchar setx) //bit[5:0]
{   uchar temp,temp1;
    temp1=setx&0x3F;
   	write_command(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0xc0;
	temp |= temp1; 
	Write_Data_int(temp);

}

//-------------------------------------------//
//REG[2Fh]
void GT_serial_ROM_select_GT21L16T1W(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  Write_Data_int(temp);
}

void GT_serial_ROM_select_GT23L16U2W(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x20;
  Write_Data_int(temp);
}

void GT_serial_ROM_select_GT23L24T3Y(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x40;
  Write_Data_int(temp);
}

void GT_serial_ROM_select_GT23L24M1Z(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x60;
  Write_Data_int(temp);
}

void GT_serial_ROM_select_GT23L32S4W(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x80;
  Write_Data_int(temp);
}

void Font_code_GB2312(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  Write_Data_int(temp);
}

void Font_code_GB12345(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
   temp |= 0x04;
  Write_Data_int(temp);
}

void Font_code_BIG5(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x08;
  Write_Data_int(temp);
}

void Font_code_UNICODE(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x0C;
  Write_Data_int(temp);
}

void Font_code_ASCII(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x10;
  Write_Data_int(temp);
}

void Font_code_UNIJIS(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x14;
  Write_Data_int(temp);
}

void Font_code_JIS0208(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x18;
  Write_Data_int(temp);
}

void Font_code_LATIN(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x1C;
  Write_Data_int(temp);
}

void Font_Standard(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  Write_Data_int(temp);
}

void Font_Arial(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  temp |= 0x01;
  Write_Data_int(temp);
}

void Font_Roman(void)
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  temp |= 0x02;
  Write_Data_int(temp);
}

void Font_Bold(void) //for ASCII
{ uchar temp;
  write_command(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= cClrD1D0;
  temp |= cSetD1D0;
  Write_Data_int(temp);
}

//-------------------------------------------//
//REG[30h]~REG[37h]
void Active_Window(uint XL,uint XR ,uint YT ,uint YB)
{
    //setting active window X
  
    write_command(0x30);//HSAW0
	Write_Data_int(XL); 
    write_command(0x31);//HSAW1	   
	Write_Data_int(XL>>8);

    write_command(0x34);//HEAW0
	Write_Data_int(XR);
    write_command(0x35);//HEAW1	   
	Write_Data_int(XR>>8);

    //setting active window Y
  
    write_command(0x32);//VSAW0
	Write_Data_int(YT);  
    write_command(0x33);//VSAW1	   
	Write_Data_int(YT>>8);
  
    write_command(0x36);//VEAW0
	Write_Data_int(YB); 
    write_command(0x37);//VEAW1	   
	Write_Data_int(YB>>8);
}
//--------------------------------------------//
//REG[38h]~REG[3Fh]

void Scroll_Window(uint XL,uint XR ,uint YT ,uint YB)
{
 
    write_command(0x38);//HSSW0
	Write_Data_int(XL);
    write_command(0x39);//HSSW1	   
	Write_Data_int(XL>>8);
  
    write_command(0x3c);//HESW0
	Write_Data_int(XR);  
    write_command(0x3d);//HESW1	   
	Write_Data_int(XR>>8);   
    
    write_command(0x3a);//VSSW0
	Write_Data_int(YT);
    write_command(0x3b);//VSSW1	   
	Write_Data_int(YT>>8);
  
    write_command(0x3e);//VESW0
	Write_Data_int(YB);  
    write_command(0x3f);//VESW1	   
	Write_Data_int(YB>>8);
}  


//--------------------------------------------//
//REG[40h]
void Graphic_Mode(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}

void Text_Mode(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);	 
} 

void Text_Cursor_Disable(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	Write_Data_int(temp);
}

void Text_Cursor_Enable(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	Write_Data_int(temp);	 	
} 

void Text_Cursor_Blink_Disable(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD5 ;
	Write_Data_int(temp);
}

void Text_Cursor_Blink_Enable(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD5 ;
	Write_Data_int(temp);
}

void Memory_Write_LeftRight_TopDown(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	Write_Data_int(temp);
}

void Memory_Write_RightLeft_TopDown(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	Write_Data_int(temp);
}

void Memory_Write_DownTop_LeftRight(void)
{	uchar temp;	
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	Write_Data_int(temp);
}

void Memory_Write_TopDown_LeftRight(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	Write_Data_int(temp);
} 

void MemoryWrite_Cursor_autoIncrease(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD1 ;
	Write_Data_int(temp);
}

void MemoryWrite_Cursor_NoautoIncrease(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD1 ;
	Write_Data_int(temp);
}

void MemoryRead_Cursor_autoIncrease(void)
{	uchar temp;
	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD0 ;
	Write_Data_int(temp);
}

void MemoryRead_Cursor_NoautoIncrease(void)
{
	uchar temp;

	write_command(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD0 ;
	Write_Data_int(temp);
}  										 

//--------------------------------------------//
//REG[41h]
void No_Graphic_Cursor(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}

void Graphic_Cursor(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set1(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set2(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6D5SetD4 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set3(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5ClrD4 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set4(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5D4 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set5(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5D4 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set6(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5SetD4 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set7(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5ClrD4 ;
	Write_Data_int(temp);
}

void Graphic_Cursor_Set8(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5D4 ;
	Write_Data_int(temp);
}

void Write_To_Bank1and2(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	Write_Data_int(temp);
}

void Write_To_CGRAM(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= 0xf3;
	temp |= cClrD3SetD2 ;
	Write_Data_int(temp);
}

void Write_To_GraphicCursor(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= 0xf3;
	temp |= cSetD3ClrD2 ;
	Write_Data_int(temp);
}

void Write_To_Pattern(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= 0xf3;
	temp |= cSetD3D2 ;
	Write_Data_int(temp);
}
						   
void Write_To_Bank1(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD0 ;
	Write_Data_int(temp);
}

void Write_To_Bank2(void)
{	uchar temp;
	write_command(0x41);//MWCR1
	temp = LCD_DataRead();
	temp |= cSetD0 ;
	Write_Data_int(temp);  
}	
							
//--------------------------------------------//
//--------------------------------------------//
//REG[44h]
void Text_Blink_Time(uchar setx)
{	
	write_command(0x44);//CURS
	Write_Data_int(setx);
}

//--------------------------------------------//
//REG[45h]  
void Memory_read_LeftRight_TopDown(void)
{	uchar temp;
	write_command(0x45);//MRCD
	temp = LCD_DataRead();
	temp &= cClrD1D0;
	Write_Data_int(temp);
}
void Memory_read_RightLeft_TopDown(void)
{	uchar temp;
	write_command(0x45);//MRCD
	temp = LCD_DataRead();
	temp |= cClrD1SetD0;
	Write_Data_int(temp);
}
void Memory_read_TopDown_LeftRight(void)
{	uchar temp;
	write_command(0x45);//MRCD
	temp = LCD_DataRead();
	temp |= cSetD1ClrD0;
	Write_Data_int(temp);
}
void Memory_read_DownTop_LeftRight(void)
{	uchar temp;
	write_command(0x45);//MRCD
	temp = LCD_DataRead();
	temp |= cSetD1D0;
	Write_Data_int(temp);
}			

//--------------------------------------------//
//REG[46h]~REG[49h]
void LCD_SetPos(uint X,uint Y)
{
    write_command(0x46);
	Write_Data_int(X);  
    write_command(0x47);	   
	Write_Data_int(X>>8);
 
    write_command(0x48);
	Write_Data_int(Y);  
    write_command(0x49);	   
	Write_Data_int(Y>>8);
}



//--------------------------------------------//
//REG[4Ah]~REG[4Dh]
void Memory_read_Coordinate(uint X,uint Y)
{

    write_command(0x4a);
	Write_Data_int(X);   
    write_command(0x4b);	   
	Write_Data_int(X>>8);
   
    write_command(0x4c);
	Write_Data_int(Y);
    write_command(0x4d);	   
	Write_Data_int(Y>>8);
} 
						  
//--------------------------------------------//
//REG[4Eh]
void Text_Cursor_Horizontal_Size(uchar setx)//bit[3:0]
{
    write_command(0x4E);//CURS	   
	Write_Data_int(setx);
} 
								
//REG[4Fh] 
void Text_Cursor_Vertical_Size(uchar setx)//bit[3:0]
{
    write_command(0x4F);//CURS	   
	Write_Data_int(setx);
} 	
	
//--------------------------------------------//
//REG[50h] 
void no_BTE_write(void)
{	uchar temp;
	write_command(0x50);//BECR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);  
}

void BTE_enable(void)
{	uchar temp;
	write_command(0x50);//BECR0
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);  
}

void BTE_contiguous_Data(void)
{	uchar temp;
	write_command(0x50);//BECR0
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	Write_Data_int(temp);  
}

void BTE_rectangular_Data(void)
{	uchar temp;												   
	write_command(0x50);//BECR0
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	Write_Data_int(temp);  
}									  

//---------------------------------------------//
//REG[51h] 
void BTE_ROP_Code(uchar setx)
{	 
    write_command(0x51);//BECR1	   
	Write_Data_int(setx); 
}									   

//---------------------------------------------//
//REG[52h] 
void Layer1_Visible(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	Write_Data_int(temp);  
}

void Layer2_Visible(void)
{   uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x01;
	Write_Data_int(temp);  
}	
						
void Transparent_Mode(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x03;
	Write_Data_int(temp);  
}

void Lighten_Overlay_Mode(void)
{   uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x02;
	Write_Data_int(temp);  
}	
						
void Boolean_OR(void)
{   uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x04;
	Write_Data_int(temp);
}

void Boolean_AND(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x05;
	Write_Data_int(temp);  
}  	 
	
  
void Floating_window_mode(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x06;
	Write_Data_int(temp);  
} 

void Floating_Window_transparent_with_BGTR_enable(void)
{   uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp|=cSetD5;
	Write_Data_int(temp);  
}

void Floating_Window_transparent_with_BGTR_disable(void)
{   uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=cClrD5;
	Write_Data_int(temp);  
}


void Layer1_2_scroll(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	Write_Data_int(temp);  
}  	 

void Layer1_scroll(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	temp|=0x40;
	Write_Data_int(temp);  
} 

void Layer2_scroll(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	temp|=0x80;
	Write_Data_int(temp);  
} 

void Buffer_scroll(void)
{	uchar temp;
	write_command(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	temp|=0xC0;
	Write_Data_int(temp);  
} 

//---------------------------------------------//
//REG[53h] 
void layer2_1_transparency(uchar setx)
{  
   write_command(0x53);//LTPR1
   Write_Data_int(setx); 
}

//---------------------------------------------//
//REG[54h]~ [5Fh]
void BTE_Source_Destination	(uint XL,uint XR ,uint YT ,uint YB)
{
	uchar temp,temp1;
    
	temp=XL;   
    write_command(0x54);//HSBE0
	Write_Data_int(temp);
	temp=XL>>8;   
    write_command(0x55);//HSBE1	   
	Write_Data_int(temp);

	temp=XR;   
    write_command(0x58);//HDBE0
	Write_Data_int(temp);
	temp=XR>>8;   
    write_command(0x59);//HDBE1	   
	Write_Data_int(temp); 
    
	temp=YT;   
    write_command(0x56);//VSBE0
	Write_Data_int(temp);
	temp=YT>>8;   
    write_command(0x57);//VSBE1
	temp1 = LCD_DataRead();
	temp1 &= 0x80;
    temp=temp|temp1; 
	write_command(0x57);//VSBE1  
	Write_Data_int(temp);


	temp=YB;   
    write_command(0x5a);//VDBE0
	Write_Data_int(temp);
	temp=YB>>8;   
    write_command(0x5b);//VDBE1
	temp1 = LCD_DataRead();
	temp1 &= 0x80;
	temp=temp|temp1;	
	write_command(0x5b);//VDBE1   
	Write_Data_int(temp);
}							

void Source_Layer1(void)
{   uchar temp;	
	write_command(0x57);//VSBE1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}

void Source_Layer2(void)
{	uchar temp;	
	write_command(0x57);//VSBE1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);
}

void Destination_Layer1(void)
{	uchar temp;	
	write_command(0x5b);//VDBE1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);
}

void Destination_Layer2(void)
{	uchar temp;	
	write_command(0x5b);//VDBE1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);
}
		  
//---------------------------------------------//
//---------------------------------------------//
void BTE_Size_setting(uint X,uint Y)
{	 
    write_command(0x5c);//BEWR0
	Write_Data_int(X); 
    write_command(0x5d);//BEWR1	   
	Write_Data_int(X>>8);
  
    write_command(0x5e);//BEHR0
	Write_Data_int(Y);   
    write_command(0x5f);//BEHR1	   
	Write_Data_int(Y>>8);
}									 

//---------------------------------------------//
//REG[60h]~REG[67h]

void Background_color(uint color)
{

    write_command(0x60);
	Write_Data_int((color&0xf800)>>11);
	
    write_command(0x61);
	Write_Data_int((color&0x07e0)>>5);
	
	write_command(0x62);
	Write_Data_int((color&0x001f));

}

 void Text_color(uint color) 
{

    write_command(0x63);
	Write_Data_int((color&0xf800)>>11);
	
    write_command(0x64);
	Write_Data_int((color&0x07e0)>>5);
	
	write_command(0x65);
	Write_Data_int((color&0x001f));

 
 
}



//--------------------------------------------//
void Text_Background_Color(uchar setR,setG,setB)
{
    write_command(0x60);//BGCR0
	Write_Data_int(setR);
   
    write_command(0x61);//BGCR0
	Write_Data_int(setG);

    write_command(0x62);//BGCR0
	Write_Data_int(setB);
} 

void Text_Foreground_Color(uchar setR,setG,setB)
{	    
    write_command(0x63);//BGCR0
	Write_Data_int(setR);
   
    write_command(0x64);//BGCR0
	Write_Data_int(setG);

    write_command(0x65);//BGCR0
	Write_Data_int(setB);

}


void BTE_Background_color(uchar setR,setG,setB)
{
    write_command(0x60);//BGCR0
	Write_Data_int(setR);
    write_command(0x61);//BGCR0
	Write_Data_int(setG);
    write_command(0x62);//BGCR0
	Write_Data_int(setB);
}


void BTE_Foreground_color(uchar setR,setG,setB)
{
    write_command(0x63);//BGCR0
	Write_Data_int(setR);
    write_command(0x64);//BGCR0
	Write_Data_int(setG);
    write_command(0x65);//BGCR0
	Write_Data_int(setB);
}


//======================================
//for test
void BTE_Background_red(uchar setx)
{	  
    write_command(0x60);//BGCR0
	Write_Data_int(setx);
}	
						
void BTE_Background_green(uchar setx)
{	  
    write_command(0x61);//BGCR1
	Write_Data_int(setx);
}							  

void BTE_Background_blue(uchar setx)
{	 
    write_command(0x62);//BGCR2
	Write_Data_int(setx);
} 								

void BTE_Foreground_red(uchar setx)
{	
    write_command(0x63);//FGCR0
	Write_Data_int(setx);
}								 

void BTE_Foreground_green(uchar setx)
{	  
    write_command(0x64);//FGCR1
	Write_Data_int(setx);
}							 

void BTE_Foreground_blue(uchar setx)
{  
    write_command(0x65);//FGCR2
	Write_Data_int(setx);
} 	

//======================================
	
void Pattern_Set_8x8(void)
{  uchar temp;

 write_command(0x66);//PTNO
 temp = LCD_DataRead();
 temp &= cClrD7;
 Write_Data_int(temp);
}	

void Pattern_Set_16x16(void)
{  uchar  temp;

 write_command(0x66);//PTNO
 temp = LCD_DataRead();
 temp |= cSetD7; 
 Write_Data_int(temp);
}								


void Pattern_Set_number(uchar setx)
{   uchar temp;
        
    write_command(0x66);//PTNO
    temp = LCD_DataRead();
    temp &= 0x80;    
	Write_Data_int(temp|setx);
} 	
							
void BackgroundColor_TransparentMode(uchar setR,uchar setG,uchar setB)
{
    write_command(0x67);//BGTR_R
	Write_Data_int(setR);

    write_command(0x68);//BGTR_G
	Write_Data_int(setG);

    write_command(0x69);//BGTR_B
	Write_Data_int(setB);

}	  

//REG[70h]~REG[74h]
//==============================================================================
//Subroutine:	Enable_TP 
//==============================================================================
void Enable_TP(void)
{   uchar temp;	  
	write_command(0x70);//TPCR0	
	temp = LCD_DataRead();
	temp |= cSetD7;   
	Write_Data_int(temp);
}
//==============================================================================
//Subroutine:	Disable_TP 
//==============================================================================
void Disable_TP(void)
{ uchar temp;
  write_command(0x70);//TPCR0
  temp = LCD_DataRead();
  temp &= cClrD7;   
  Write_Data_int(temp);
 }

//==============================================================================
//Subroutine:	Enable_Touch_WakeUp
//==============================================================================
void Enable_Touch_WakeUp(void)
{
    uchar temp;
	write_command(0x70);//TPCR0	
	temp = LCD_DataRead();
	temp |= cSetD3;   //cSetb3 = 00001000
	Write_Data_int(temp);
}
//==============================================================================
//Subroutine:	Disable_Touch_WakeUp
//==============================================================================
void Disable_Touch_WakeUp(void)
{	uchar temp;
	write_command(0x70);//TPCR0	
	temp = LCD_DataRead();
	temp &= cClrD3;
	Write_Data_int(temp);
}

//==============================================================================
//Subroutine:	TP manual or auto  mode enable
//==============================================================================
void TP_manual_mode(void)
{	uchar temp;
  	write_command(0x71);//TPCR1	
	temp = LCD_DataRead();
	temp |= cSetD6;
	Write_Data_int(temp);
}
void TP_auto_mode(void)
{	uchar temp;
  	write_command(0x71);//TPCR1	
	temp = LCD_DataRead();
	temp &= cClrD6;
	Write_Data_int(temp);
}
//==============================================================================
//Subroutine:	TP_IDLE_mode
//==============================================================================
void TP_IDLE_mode(void)
{ uchar temp;
  write_command(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xf0;
  Write_Data_int(temp);
 }
//==============================================================================
//Subroutine:	TP_event_mode
//==============================================================================
void TP_event_mode(void)
{ uchar temp;
  write_command(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xf0;
  temp |= 0x01;
  Write_Data_int(temp);
//  Write_Data_int(0xc1);
}
//==============================================================================
//Subroutine:	TP_latch_X
//==============================================================================
void TP_latch_X(void)
{ uchar temp;
  write_command(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xc0;
  temp |= 0x02;
  Write_Data_int(temp);
 }
 //==============================================================================
//Subroutine:	TP_latch_Y
//==============================================================================
void TP_latch_Y(void)
{ uchar temp;
  write_command(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xc0;
  temp |= 0x03;
  Write_Data_int(temp);
 }
//==============================================================================
//Subroutine:	Detect_TP_X_Y coordinate 
//==============================================================================
uchar ADC_X(void)
{	uchar temp;
	write_command(0x72);//TPXH	 X_coordinate high byte
	//Chk_Busy();
	temp=LCD_DataRead();
	return temp;
}

uchar ADC_Y(void)
{	uchar temp;
	write_command(0x73);//TPYH	  Y_coordinate high byte
	//Chk_Busy();
	temp=LCD_DataRead();
	return temp;
}

uchar ADC_XY(void)
{	uchar temp;
 	write_command(0x74);//TPXYL	  bit[3:2] Y_coordinate low byte  bit[1:0] X_coordinate low byte 
	temp=LCD_DataRead();
	return temp;
}   



//REG[80h]~REG[83h]
void Graphic_Cursor_Coordinate(uint X,uint Y)
{
    write_command(0x80);
	Write_Data_int(X);  
    write_command(0x81);	   
	Write_Data_int(X>>8);
  
    write_command(0x82);
	Write_Data_int(Y);  
    write_command(0x83);	   
	Write_Data_int(Y>>8);
}

//---------------------------------//
//REG[84h]

void Graphic_Cursor_Color_0(uchar setx)
{	  
    write_command(0x84);//GCC0
	Write_Data_int(setx); 
}  									  

void Graphic_Cursor_Color_1(uchar setx)
{	 
    write_command(0x85);//GCC1
	Write_Data_int(setx); 
}  	
 	 						  
//---------------------------------------------//
//REG[8A]
void PWM1_enable(void)
{
	uchar temp;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);  
 }

void PWM1_disable(void)
{
	uchar temp;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);  
 }

void PWM1_disable_level_low(void)
{
	uchar temp;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	Write_Data_int(temp);  
}

void PWM1_disable_level_high(void)
{
	uchar temp;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	Write_Data_int(temp);  
}

 void PWM1_fnuction_sel(void)
{
	uchar temp;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	Write_Data_int(temp);  
 }

 void PWM1_system_clk_out(void)
{
	uchar temp;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD4 ;
	Write_Data_int(temp);  
 }

 void PWM1_clock_ratio(uchar setx) //bit0~3 
{
	uchar temp,temp1;
	temp1= setx&0x0f;
	write_command(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	Write_Data_int(temp);  
 }

 void PWM1_duty_cycle(uchar setx) //bit0~7
{
  
    write_command(0x8b);//PTNO
	Write_Data_int(setx);
} 	

void PWM2_enable(void)
{
	uchar temp;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp);  
 }

void PWM2_disable(void)
{
	uchar temp;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);  
 }

void PWM2_disable_level_low(void)
{
	uchar temp;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	Write_Data_int(temp);  
}

void PWM2_disable_level_high(void)
{
	uchar temp;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	Write_Data_int(temp);  
}

 void PWM2_fnuction_sel(void)
{
	uchar temp;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	Write_Data_int(temp);  
 }

  void PWM2_system_clk_out(void)
{
	uchar temp;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD4 ;
	Write_Data_int(temp);  
 }

  void PWM2_clock_ratio(uchar setx) //bit0~3 
{
	uchar temp,temp1;
	temp1= setx&0x0f;
	write_command(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	Write_Data_int(temp);  
 }
 void PWM2_duty_cycle(uchar setx) //bit0~7
{  
    write_command(0x8d);//PTNO
	Write_Data_int(setx);
} 	


//---------------------------------------------//
//REG[8Eh]
void Stop_Memory_Clear(void)
{
	uchar temp;
	write_command(0x8e);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	Write_Data_int(temp);  
}						 
void Memory_Clear(void)
{
	uchar temp;
	write_command(0x8e);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	Write_Data_int(temp); 
	Chk_Busy(); 
}
void Clear_Full_Window(void)
{
	uchar temp;
	write_command(0x8e);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	Write_Data_int(temp);  
}
void Clear_Active_Window(void)
{
	uchar temp;
	write_command(0x8e);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	Write_Data_int(temp);  
}

									
//---------------------------------------------//
//REG[90h] 
void Draw_line(void)
{ 
  write_command(0x90);//DCR
  Write_Data_int(0x00);
  Write_Data_int(0x80);
}

void Draw_square(void)
{
  write_command(0x90);//DCR
  Write_Data_int(0x10);
  Write_Data_int(0x90);
}

void Draw_square_fill(void)
{
  write_command(0x90);//DCR
  Write_Data_int(0x10);
  Write_Data_int(0xb0);
}

void Draw_circle(void)
{
  write_command(0x90);//DCR
  Write_Data_int(0x40);
}

void Draw_circle_fill(void)
{
  write_command(0x90);//DCR
  Write_Data_int(0x60);
}

void Draw_Triangle(void)
{
   write_command(0x90);//DCR
   Write_Data_int(0x00);
   Write_Data_int(0x81);
}

void Draw_Triangle_fill(void)
{
   write_command(0x90);//DCR
   Write_Data_int(0x01);
   Write_Data_int(0xA1);
}

//---------------------------------------------//
//REG[90h]~/EG[9Dh]  									
void Geometric_Coordinate(uint XL,uint XR ,uint YT ,uint YB)
{	  
    write_command(0x91);
	Write_Data_int(XL);   
    write_command(0x92);	   
	Write_Data_int(XL>>8);
  
    write_command(0x95);
	Write_Data_int(XR);  
    write_command(0x96);	   
	Write_Data_int(XR>>8);
	     
    write_command(0x93);
	Write_Data_int(YT);  
    write_command(0x94);	   
	Write_Data_int(YT>>8);
   
    write_command(0x97);
	Write_Data_int(YB);   
    write_command(0x98);	   
	Write_Data_int(YB>>8);
}

void Circle_Coordinate_Radius(uint X,uint Y,uint R)
{
      
    write_command(0x99);
	Write_Data_int(X);  
    write_command(0x9a);	   
	Write_Data_int(X>>8);  
	   
    write_command(0x9b);
	Write_Data_int(Y); 
    write_command(0x9c);	   
	Write_Data_int(Y>>8);
 
    write_command(0x9d);
	Write_Data_int(R);
} 

//---------------------------------------------//
//REG[A0h] 
void Draw_Ellipse(void)
{
   write_command(0xA0);//DCR
   Write_Data_int(0x00);
   Write_Data_int(0x80);
}

void Draw_Ellipse_Curve(void)
{  uchar temp;

   write_command(0xA0);//DCR
   temp = LCD_DataRead();
   temp &=0x1F;
   temp |= 0x90;
   Write_Data_int(temp);
}

void Draw_Ellipse_Curve_Fill(void)
{  uchar temp;

   write_command(0xA0);//DCR
   temp = LCD_DataRead();
   temp &=0x1F;
   temp |= 0xD0;
   Write_Data_int(temp);
}

void Draw_Ellipse_fill(void)
{
   write_command(0xA0);//DCR
   Write_Data_int(0x40);
   Write_Data_int(0xC0);
}

void Draw_Circle_Square(void)
{
   write_command(0xA0);//DCR
   Write_Data_int(0x20);
   Write_Data_int(0xA0);
}

void Draw_Circle_Square_fill(void)
{
   write_command(0xA0);//DCR
   Write_Data_int(0x60);
   Write_Data_int(0xE0);
}


void Draw_Ellipse_Curve_part(uchar setx)
{
   uchar temp,temp1;
   write_command(0xA0);//DCR
   temp = LCD_DataRead();
   temp1 = setx&0x03;
   temp &=0xfc;
   temp |=temp1;
   Write_Data_int(temp);   
}


void Ellipse_Coordinate_setting(uint X,uint Y,uint ELL_A,uint ELL_B)
{

    write_command(0xA5);
	Write_Data_int(X);
    write_command(0xA6);	   
	Write_Data_int(X>>8);  
	   
    write_command(0xA7);
	Write_Data_int(Y); 
    write_command(0xA8);	   
	Write_Data_int(Y>>8);
  
    write_command(0xA1);
	Write_Data_int(ELL_A);
	write_command(0xA2);
	Write_Data_int(ELL_A>>8);
  
    write_command(0xA3);
	Write_Data_int(ELL_B);
	write_command(0xA4);
	Write_Data_int(ELL_B>>8);
} 



void Circle_Square_Coordinate_setting(uint XL,uint XR ,uint YT ,uint YB ,uint ELL_A,uint ELL_B)
{	    
  
    write_command(0x91);
	Write_Data_int(XL);  
    write_command(0x92);	   
	Write_Data_int(XL>>8);

    write_command(0x95);
	Write_Data_int(XR);   
    write_command(0x96);	   
	Write_Data_int(XR>>8);
	     
    write_command(0x93);
	Write_Data_int(YT);  
    write_command(0x94);	   
	Write_Data_int(YT>>8);

    write_command(0x97);
	Write_Data_int(YB);  
    write_command(0x98);	   
	Write_Data_int(YB>>8);
  
    write_command(0xA1);
	Write_Data_int(ELL_A);
	write_command(0xA2);
	Write_Data_int(ELL_A>>8);
 
    write_command(0xA3);
	Write_Data_int(ELL_B);
	write_command(0xA4);
	Write_Data_int(ELL_B>>8); 
}

 
void Draw_Triangle_3point_Coordinate(uint P1_X,uint P1_Y,uint P2_X,uint P2_Y,uint P3_X,uint P3_Y)
{	    
  
    write_command(0x91);
	Write_Data_int(P1_X);  
    write_command(0x92);	   
	Write_Data_int(P1_X>>8);
  
    write_command(0x93);
	Write_Data_int(P1_Y);  
    write_command(0x94);	   
	Write_Data_int(P1_Y>>8);
 
    write_command(0x95);
	Write_Data_int(P2_X);  
    write_command(0x96);	   
	Write_Data_int(P2_X>>8);
 
    write_command(0x97);
	Write_Data_int(P2_Y);  
    write_command(0x98);	   
	Write_Data_int(P2_Y>>8);

    write_command(0xA9);
	Write_Data_int(P3_X);  
    write_command(0xAA);	   
	Write_Data_int(P3_X>>8);
  
    write_command(0xAB);
	Write_Data_int(P3_Y);   
    write_command(0xAC);	   
	Write_Data_int(P3_Y>>8);   
}

//---------------------------------------------//
/*
//REG[B0]~REG[B3]
void DMA_Start_address_setting(ulong set_address)
{ 
  write_command(0xB0);
  Write_Data_int(set_address);

  write_command(0xB1);
  Write_Data_int(set_address>>8);

  write_command(0xB2);
  Write_Data_int(set_address>>16);

  write_command(0xB3);
  Write_Data_int(set_address>>24);

}

//input pic data quantity  for example  pic size = 800*480 = 384000
void DMA_Continuous_mode_size_setting(ulong set_size)
{ 
  write_command(0xB4);
  Write_Data_int(set_size);

  write_command(0xB6);
  Write_Data_int(set_size>>8);
 
  write_command(0xB8);
  Write_Data_int(set_size>>16);

}

void DMA_block_mode_size_setting(uint BWR,uint BHR,uint SPWR)
{
  write_command(0xB4);
  Write_Data_int(BWR);
  write_command(0xB5);
  Write_Data_int(BWR>>8);

  write_command(0xB6);
  Write_Data_int(BHR);
  write_command(0xB7);
  Write_Data_int(BHR>>8);

  write_command(0xB8);
  Write_Data_int(SPWR);
  write_command(0xB9);
  Write_Data_int(SPWR>>8);  
}

//---------------------------------------------//
//REG[BFh]
void DMA_Continuous_mode(void)
{ 
  write_command(0xBF);
  Write_Data_int(0x00);
}

void DMA_Block_mode(void)
{ 
  write_command(0xBF);
  Write_Data_int(0x02);
}

void DMA_Start_enable(void)
{  uchar temp;
  write_command(0xBF);
  temp = LCD_DataRead();
  temp |= 0x01;
  Write_Data_int(temp);
}
 */
//---------------------------------------------//
//REG[C0h]
void Key_Scan_Enable(void)
{ uchar temp;
  write_command(0xC0);
  temp = LCD_DataRead();
  temp |= 0x80;
  Write_Data_int(temp);  
}


void Key_Scan_setting(uchar setx)//bit[2:0]
{ uchar temp;
  write_command(0xC0);
  temp = LCD_DataRead();
  temp &= 0xf0;
  temp|= (setx&0x07);
  Write_Data_int(temp);  
} 

void Long_Key_Scan_enable(void)
{ uchar temp;
  write_command(0xC0);
  temp = LCD_DataRead();
  temp |= 0x40;
  Write_Data_int(temp);
}

void Key_Scan_Wakeup_Function_Enable(void)
{ uchar temp;
  write_command(0xC1);	
  temp = LCD_DataRead();
  temp |= 0x80;	// 
  Write_Data_int(temp);
}

void Long_Key_Timing_Adjustment(uchar setx)
{
   uchar temp,temp1;
  temp = setx & 0x1c;
  write_command(0xC1);
  temp1 = LCD_DataRead();
  temp1|=temp;
  Write_Data_int(temp1);   
 
}

//---------------------------------------------//
//REG[D0]
void Floating_window_start_point(uint X,uint Y)
{ 
   write_command(0xD0);
   Write_Data_int(X);
   write_command(0xD1);
   Write_Data_int(X>>8);

   write_command(0xD2);
   Write_Data_int(Y);
   write_command(0xD3);
   Write_Data_int(Y>>8);  
}


void Floating_window_size(uint width_X,uint high_Y)
{

   write_command(0xD4);
   Write_Data_int(width_X);
   write_command(0xD5);
   Write_Data_int(width_X>>8);

   write_command(0xD6);
   Write_Data_int(high_Y);
   write_command(0xD7);
   Write_Data_int(high_Y>>8);

}

void Floating_window_display_point(uint X,uint Y)
{

   write_command(0xD8);
   Write_Data_int(X);
   write_command(0xD9);
   Write_Data_int(X>>8);

   write_command(0xDA);
   Write_Data_int(Y);
   write_command(0xDB);
   Write_Data_int(Y>>8);

}


//REG[E0h] 
void Serial_ROM_direct_access_mode_enable(void)
{
   write_command(0xE0);
   Write_Data_int(0x01);
}

void Serial_ROM_direct_access_mode_disable(void)
{
   write_command(0xE0);
   Write_Data_int(0x00);
}
/*
void Direct_access_mode_Address_32bits(ulong set_address)
{
  uchar temp;  
  temp = set_address>>24;
  write_command(0xE1);
  Write_Data_int(temp);
  temp = set_address>>16;
  Write_Data_int(temp);
  temp = set_address>>8;
  Write_Data_int(temp);
  temp = set_address;
  Write_Data_int(temp);

}

void Direct_access_mode_Address_24bits(ulong set_address)
{
  uchar temp=0;
  
  temp = set_address>>16;
  write_command(0xE1);
  Write_Data_int(temp);  
  temp = set_address>>8;
  write_command(0xE1);
  Write_Data_int(temp);   
  temp = set_address;
  write_command(0xE1);
  Write_Data_int(temp);

}
  */
uchar Direct_access_mode_Read_Data_buffer(void)
{
   uchar Serial_Data;

   write_command(0xE2);
   Serial_Data=LCD_DataRead(); 
   return Serial_Data;
}

//---------------------------------------------//



//---------------------------------------------//
//REG[F0h] 

void KEYSCAN_Interrupt_Enable(void)
{ uchar temp;
  write_command(0xF0);	
  temp = LCD_DataRead();
  temp |= 0x10;	// 
  Write_Data_int(temp); 
}

void KEYSCAN_Interrupt_Disable(void)
{ uchar temp;
  write_command(0xF0);	
  temp = LCD_DataRead();
  temp &= 0x0F;	// 
  Write_Data_int(temp); 
}


void DMA_Interrupt_Enable(void)
{ uchar temp;
  write_command(0xF0);	
  temp = LCD_DataRead();
  temp |= 0x08;	// 
  Write_Data_int(temp); 
}

void DMA_Interrupt_Disable(void)
{ uchar temp;
  write_command(0xF0);	
  temp = LCD_DataRead();
  temp &= 0x17;	// 
  Write_Data_int(temp); 
}

void Touch_Panel_Interrupt_Enable(void)
{ uchar temp;
  write_command(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp |= 0x04;
  Write_Data_int(temp);
 }

void Touch_Panel_Interrupt_Disable(void)
{ uchar temp;
  write_command(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp &= 0x1B;
  Write_Data_int(temp);
 }

void BTE_Interrupt_Enable(void)
{
  uchar temp;
  write_command(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp |= 0x02;
  Write_Data_int(temp);
}

void BTE_Interrupt_Disable(void)
{
  uchar temp;
  write_command(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp &= 0x1D;
  Write_Data_int(temp);
}

void Font_BTE_read_write_Interrupt_Enable(void)
{
  uchar temp;
  write_command(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp |= 0x01;
  Write_Data_int(temp);
}

void Font_BTE_read_write_Interrupt_Disable(void)
{
  uchar temp;
  write_command(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp &= 0x1E;
  Write_Data_int(temp);
}

//---------------------------------------------//
//REG[F1h] 
void Clear_keyscan_interrupt(void)
{ uchar temp;
  write_command(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |=cSetD4 ;
  Write_Data_int(temp);
}

bit Keyscan_Status(void)
{   uchar temp;
	write_command(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x10)==0x10)
	return 1;
	else 
	return 0;
}

void Clear_DMA_interrupt(void)
{ uchar temp;
  write_command(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |=cSetD3 ;
  Write_Data_int(temp);

}

bit DMA_Status(void)
{   uchar temp;
	write_command(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x08)==0x08)
	return 1;
	else 
	return 0;
}

void clear_TP_interrupt(void)
{ uchar temp;
  write_command(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |= cSetD2;
  Write_Data_int(temp);
 }


bit Touch_Status(void)
{	uchar temp;
	write_command(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x04)==0x04)
	return 1;
	else 
	return 0;
}

void Clear_BTE_interrupt(void)
{ uchar temp;
  write_command(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |= cSetD1;
  Write_Data_int(temp);
}

bit BTE_Status(void)
{	uchar temp;
	write_command(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x02)==0x02)
	return 1;
	else 
	return 0;
}

void Clear_Font_BTE_read_write_Interrupt(void)
{ uchar temp;
  write_command(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |= cSetD0;
  Write_Data_int(temp);
}

bit Font_BTE_read_write_Status(void)
{	uchar temp;
	write_command(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x01)==0x01)
	return 1;
	else 
	return 0;
}



void Chk_Wait(void)
{
	
 while(wait==0);	   
}


void Show_String(uchar *str,uchar n)
{   Text_Mode();
	write_command(0x02);
	while(*str != '\0')
	{
	 Write_Data_int(*str);
	 ++str;	 	
	 Chk_Busy();
     //Chk_Wait();		
	}
   delay(n);
}

/*

void LCD_Print_J_Str(uchar *ptr, int char_num)
{
	int temp = 0;
	unsigned char SJIS_HB, SJIS_LB, JIS_HB, JIS_LB, LSB, Num=0;

	while(temp < char_num)
	{
		if(ptr[temp] <= 0x7F)//127
		{
			Write_Data_int(ptr[temp]);
			temp++;
		}
		else
		{
 	 		SJIS_HB = ptr[temp];
			temp++;
			SJIS_LB = ptr[temp];
			temp++;

			//========================================
			// Transform SJIS Low Byte
			//========================================
			if(SJIS_LB >= 0x9F)
			{
				JIS_LB = SJIS_LB - 0x9E;//0x7E;
				LSB = 0x01;
			}
			else if(SJIS_LB <= 0x7E) //126
			{
				JIS_LB = SJIS_LB - 0x3f;//0x1F;
			LSB = 0x00;
			}
			else
			{
				JIS_LB = SJIS_LB - 0x40;//0x20;
				LSB = 0x00;
			}

			//========================================
			// Transform SJIS High Byte
			//========================================
			if(SJIS_HB >= 0xE0)
			{
				SJIS_HB = SJIS_HB - 0xC1;	
			}
			else if(SJIS_HB <= 0x9F)
			{
				SJIS_HB = SJIS_HB - 0x81;	
			}

			JIS_HB = (((SJIS_HB << 1) & 0xFE) | LSB) + 0x01;//0x21;

			//========================================
			// Write JIS Code
			//========================================
			//JIS_HB = JIS_HB + 0x80;
			//if(JIS_LB >= 0x60)
				//JIS_LB = JIS_LB + 0x40;
			//else
			 	//JIS_LB = JIS_LB + 0x20;

			Write_Data_int(JIS_HB);
			delay(1);
			Write_Data_int(JIS_LB);
			delay(1);
		}
	}
}
*/




void LCD_all(unsigned int i)
{
    uint l=272,w;
	Active_Window(0,479,0,271);
	LCD_SetPos(0,0);	//cursor position
	write_command(0x02);//set CMD02 to  prepare data write
	while(l--)
	{
	    for(w=0;w<480;w++)
		{    
          	Write_Data_int(i);
		}
	}
}




void LCD_FULL(unsigned int i)
{
    unsigned int w,u;
	Active_Window(0,479,0,271);
	LCD_SetPos(0,0);	//cursor position
	write_command(0x02);//set CMD02 to  prepare data write
	    for(w=0;w<480;w++)
		{    
          	Write_Data_int(~i);

		}

  	for(w=0;w<270;w++)		    
        {  	Write_Data_int(~i);
	    	
			for(u=0;u<478;u++)
         	Write_Data_int(i);

          	Write_Data_int(~i);		
		}

	    for(w=0;w<480;w++)
		{    
          	Write_Data_int(~i);

		}	
	
}




void show_pic(unsigned char  p[])  //200*120
{
    unsigned int i,w,l,x1,x2,y1,y2,xa,xb,ya,yb;
    unsigned char k,m;
	xa=0;xb=199;ya=0;yb=119;
	i=0;
	for(m=0;m<3;m++)
	{		for(k=0;k<3;k++)
			{x1=xa;x2=xb;y1=ya;y2=yb;
				Active_Window(x1,x2,y1,y2);	
	    			LCD_SetPos(x1,y1);	//cursor position
					write_command(0x02);//set CMD02 to  prepare data write
						for(l=0;l<120;l++)
						{
					   	 for(w=0;w<200;w++)
							{    //  temp=p[i];  temp1=p[i+1];  temp=temp<<8;temp=temp|temp1 ; 
								//	Write_Data_int(temp);  i+=1;
								//	Write_Data_int(p[i++]);
				          	Write_Data_byte(p[i],p[i+1]);
							i+=2;
          		//Write_Data_byte((p[i])>>8,p[i]);
						//	i+=1;

							}							
						}
				xa+=200;xb+=200;i=0;
			}
			xa=0;xb=199;ya+=120;yb+=120;
	}

}


void DispRGBGray(void)
{  
	unsigned int DH, COL=480;
	unsigned char i,j,k,dbl,dbh,A,B,C;

	
	Active_Window(0,479,0,271);
	LCD_SetPos(0,0);	//cursor position
	write_command(0x02);//set CMD02 to  prepare data write

	// balck -> red	
	for(k=0;k<34;k++)
	{
		for(i=0;i<32;i++)
		{
			for(j=0;j<15;j++)
			{
				dbh=i<<3;
				dbl=0;
			Write_Data_byte(dbh,dbl);
			}
		}
	}

	// red -> black
	for(k=0;k<34;k++)
	{	

		for(i=31;i>0;i--)
		{
			for(j=0;j<15;j++)
			{
				dbh=i<<3;
				dbl=0;
			Write_Data_byte(dbh,dbl);
			}
		}
		
		dbh=0x00;dbl=0x00;
		for(i=0;i<15;i++)
		Write_Data_byte(dbh,dbl);
		

	}
	// balck -> green
	for(k=0;k<34;k++)
	{

		for(i=0;i<64;i+=2)
		{
			for(j=0;j<15;j++)
			{
				dbh=i>>3;
				dbl=i<<5;
			Write_Data_byte(dbh,dbl);
			}
		}
	}

	// green -> black
	for(k=0;k<34;k++)
	{
		for(i=63;i!=1;i-=2)
		{
			for(j=0;j<15;j++)
			{
				dbh=i>>3;
				dbl=i<<5;
			Write_Data_byte(dbh,dbl);
			}
		}
		dbh=0x00;dbl=0x00;
		for(i=0;i<15;i++)
		Write_Data_byte(dbh,dbl);
	}	


	// balck -> blue
	for(k=0;k<34;k++)
	{
		for(i=0;i<32;i++)
		{
			for(j=0;j<15;j++)
			{
				dbh=0;
				dbl=i;
			Write_Data_byte(dbh,dbl);
			}
		}
	}

	// blue -> black
	for(k=0;k<34;k++)
	{	

		for(i=31;i>0;i--)
		{
			for(j=0;j<15;j++)
			{
				dbh=0;
				dbl=i;
			Write_Data_byte(dbh,dbl);
			}
		}
		dbh=0x00;dbl=0x00;
		for(i=0;i<15;i++)
		Write_Data_byte(dbh,dbl);
	}



	// balck -> white
	for(k=0;k<34;k++)
	{   
    	A=0;
    	B=0;
    	C=0;
		for(i=0;i<32;i++)
		{    
    		for(j=0;j<15;j++)
    		{
     		DH=(A<<11)+(B<<5)+C;
			Write_Data_int(DH);
    		}

    		A=A+1;
   			B=B+2;
    		C=C+1;
  		}

	}


	// white -> black
	for(k=0;k<34;k++)
	{   A=31;
    	B=63;
    	C=31;	

		for(i=31;i>0;i--)
		{    
    		for(j=0;j<15;j++)
    		{
     		DH=(A<<11)+(B<<5)+C;
			Write_Data_int(DH);
    		}

    		A=A-1;
   			B=B-2;
    		C=C-1;
  		}

		dbh=0x00;dbl=0x00;
		for(i=0;i<15;i++)
		Write_Data_byte(dbh,dbl);
	}


}



void RA8875_Initial(void)
{ 
/*	reset = 0; //RA8875 RESET pin
	delay(1);
	reset = 1;
	delay(10);*/
	Software_Reset();
// 480x272
    write_command(0x88);      
	Write_Data_int(0x0a);
	delay(1);    
    write_command(0x89);        
	Write_Data_int(0x02);  
	delay(1);

    write_command(0x10);  //SYSR   bit[4:3]=00 256 color  bit[2:1]=  00 8bit MPU interface
    //Write_Data_int(0x0c);   // if 8bit MCU interface   and 65k color display  						    
	Write_Data_int(0x0F); // if 16bit MCU interface   and 65k color display  


    write_command(0x04);    //set PCLK invers
	Write_Data_int(0x82); //0x82
	delay(1);

    //Horizontal set
    write_command(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                   
	Write_Data_int(0x3B);//Horizontal display width(pixels) = (HDWR + 1)*8                   
    write_command(0x15); //Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
	Write_Data_int(0x00);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]            
    write_command(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                      
	Write_Data_int(0x01);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8             
    write_command(0x17); //HSTR//HSYNC Start Position[4:0]                                   
	Write_Data_int(0x00);//HSYNC Start Position(PCLK) = (HSTR + 1)*8                         
    write_command(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                  
	Write_Data_int(0x05);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8        
                                             
    //Vertical set                                            
    write_command(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                            
	Write_Data_int(0x0f); //Vertical pixels = VDHR + 1                                      
    write_command(0x1a); //VDHR1 //Vertical Display Height Bit [8]                  
	Write_Data_int(0x01); //Vertical pixels = VDHR + 1                           
    write_command(0x1b);  //VNDR0 //Vertical Non-Display Period Bit [7:0]                    
	Write_Data_int(0x02);  //VSYNC Start Position(PCLK) = (VSTR + 1)                          
    write_command(0x1c);  //VNDR1 //Vertical Non-Display Period Bit [8]                             
	Write_Data_int(0x00); //Vertical Non-Display area = (VNDR + 1)                            
    write_command(0x1d); //VSTR0 //VSYNC Start Position[7:0]                               
	Write_Data_int(0x07);//VSYNC Start Position(PCLK) = (VSTR + 1)                           
    write_command(0x1e); //VSTR1 //VSYNC Start Position[8]                   
	Write_Data_int(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)                            
    write_command(0x1f);    //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	Write_Data_int(0x09);   //VSYNC Pulse Width(PCLK) = (VPWR + 1) 

  //Active window  set 
  //setting active window X
    write_command(0x30); //Horizontal Start Point 0 of Active Window (HSAW0)
	Write_Data_int(0x00); //Horizontal Start Point of Active Window [7:0]
    write_command(0x31); //Horizontal Start Point 1 of Active Window (HSAW1)	   
	Write_Data_int(0x00); //Horizontal Start Point of Active Window [9:8]
    write_command(0x34); //Horizontal End Point 0 of Active Window (HEAW0)
	Write_Data_int(0xDF); //Horizontal End Point of Active Window [7:0]
    write_command(0x35); //Horizontal End Point 1 of Active Window (HEAW1)	   
	Write_Data_int(0x01); //Horizontal End Point of Active Window [9:8]

   //setting active window Y
    write_command(0x32); //Vertical Start Point 0 of Active Window (VSAW0)
	Write_Data_int(0x00); //Vertical Start Point of Active Window [7:0]
    write_command(0x33); //Vertical Start Point 1 of Active Window (VSAW1)	   
	Write_Data_int(0x00); //Vertical Start Point of Active Window [8]
    write_command(0x36); //Vertical End Point of Active Window 0 (VEAW0)
	Write_Data_int(0x0F); //Vertical End Point of Active Window [7:0]
    write_command(0x37); //Vertical End Point of Active Window 1 (VEAW1)	   
	Write_Data_int(0x01); //Vertical End Point of Active Window [8]




	write_command(0x01);//Display on
	Write_Data_int(0x80);

	//backlight on for  RA8875
	PWM1_enable();
    PWM1_fnuction_sel();
    PWM1_clock_ratio(0x0a); //bit0~3     
    PWM1_duty_cycle(0xff); // ¥þ

	
}




unsigned char code PIC[]= { 

//----- C:\Documents and Settings\Administrator\×ÀÃæ\u=3877710378,2400454657&fm=23&gp=0.bmp
//----- H - L, x direct, 8 pixel mode, 64k color R5G6B5.
//----- width x height = 200 x 120, Total bytes = 48000
0X15,0X7F,0X15,0X7F,0X1D,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X5F,0X15,0X5F,0X15,0X7F,
0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X5F,
0X1D,0X9F,0X15,0X7F,0X1D,0XBF,0X25,0X7E,0X45,0X17,0X96,0X37,0XC6,0X95,0XA5,0X91,
0X64,0XD3,0X45,0X5A,0X15,0X3F,0X05,0X9F,0X15,0XDF,0X0D,0X7F,0X1D,0X9F,0X1D,0X7F,
0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X3F,
0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,
0X15,0X5F,0X15,0X7F,0X1D,0X7F,0X15,0X7F,0X15,0X5F,0X15,0X5F,0X15,0X7F,0X15,0X7F,
0X15,0X7F,0X15,0X7F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,
0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X1D,0X9F,0X1D,0X9F,
0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,
0X1D,0XBF,0X1D,0XBF,0X1D,0X9F,0X1D,0X7F,0X1D,0X7F,0X1D,0X5F,0X1D,0X5F,0X1D,0X7F,
0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X25,0X7F,0X25,0X5F,0X2D,0X1C,
0X65,0XFD,0X6E,0X5E,0X2C,0XFA,0X25,0X7E,0X1D,0X9F,0X0D,0X5F,0X0D,0X3E,0X0D,0X3E,
0X15,0X5E,0X15,0X5E,0X15,0X7F,0X15,0X7F,0X1D,0X5F,0X25,0X3F,0X2D,0X1F,0X2D,0X1E,
0X25,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X15,0X3F,0X1D,0X3F,0X1D,0X3F,
0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X5F,0X1D,0X5F,
0X25,0X5E,0X25,0X5E,0X25,0X5E,0X25,0X7E,0X25,0X9E,0X25,0X9E,0X25,0X9F,0X25,0X9F,
0X25,0X9E,0X25,0X9E,0X2D,0X9F,0X2D,0XBF,0X35,0XBF,0X35,0XBF,0X3D,0XBF,0X35,0XDF,
0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFE,
0X36,0X3F,0X36,0X3F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X3E,0X1F,
0X3E,0X3F,0X3E,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,
0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3E,0X4E,0X3E,
0X4E,0X5E,0X4E,0X5E,0X4E,0X3E,0X4E,0X3E,0X4E,0X3F,0X4E,0X3E,0X4E,0X3F,0X4E,0X3F,
0X4E,0X3F,0X4E,0X3F,0X46,0X3F,0X4E,0X3F,0X46,0X3F,0X46,0X3F,0X3D,0XFF,0X35,0XFF,
0X36,0X1F,0X2E,0X1F,0X2D,0XFF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X2D,0XFF,0X2E,0X1F,
0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X3F,0X15,0X3F,
0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X0C,0XFE,0X0C,0XFE,0X15,0X7F,0X1D,0X1C,0X3C,0XB5,0X85,0X94,0XAD,0XB1,0X84,0X8D,
0X5C,0X71,0X3D,0X18,0X0C,0XFD,0X05,0X5F,0X0D,0X9F,0X0D,0X5F,0X15,0X5E,0X15,0X3D,
0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X0D,0X3F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X0D,0X3F,0X0D,0X3F,0X0D,0X3F,0X15,0X5F,
0X15,0X5F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X0D,0X7F,
0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,0X15,0X9F,
0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,
0X1D,0X9F,0X1D,0X9F,0X1D,0X7F,0X1D,0X7F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,
0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X5F,0X1D,0X5F,0X25,0X7F,0X25,0X3E,0X2C,0XFC,
0X5D,0XFC,0X6E,0X5D,0X24,0XFA,0X25,0X7E,0X15,0X9F,0X0D,0X3F,0X0D,0X3E,0X0D,0X3E,
0X15,0X5E,0X15,0X5E,0X15,0X7F,0X15,0X7F,0X15,0X5F,0X1D,0X3F,0X2D,0X1E,0X25,0X1E,
0X1D,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X1F,
0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,
0X25,0X5E,0X25,0X5E,0X25,0X5E,0X25,0X7E,0X25,0X7E,0X25,0X9F,0X25,0X9F,0X25,0X9F,
0X25,0X9E,0X25,0X9E,0X25,0X9E,0X25,0X9F,0X2D,0XBF,0X2D,0XBF,0X35,0XBF,0X35,0XBF,
0X35,0XDF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X35,0XFE,0X35,0XFE,
0X36,0X1E,0X36,0X1E,0X36,0X1F,0X3E,0X1F,0X3E,0X1F,0X3E,0X1F,0X3E,0X1F,0X3E,0X1F,
0X3E,0X1F,0X46,0X1F,0X46,0X1F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,
0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,
0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,
0X4E,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X1F,0X3D,0XFF,0X35,0XFF,
0X2E,0X1F,0X2D,0XFF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XFF,0X2D,0XFF,
0X15,0X5F,0X15,0X3F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X15,0X3F,0X15,0X3F,0X0D,0X1F,
0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X15,0X1F,
0X1D,0X3E,0X1D,0X3D,0X1D,0X7E,0X25,0X3A,0X4C,0XF4,0X85,0X92,0XAD,0X8F,0X84,0X6B,
0X5C,0X4F,0X44,0XF6,0X14,0XDA,0X05,0X1E,0X0D,0X3F,0X05,0X1D,0X15,0X3C,0X15,0X1C,
0X0C,0XDD,0X0C,0XDF,0X0C,0XDF,0X0C,0XFF,0X0C,0XFF,0X0C,0XFF,0X14,0XFF,0X15,0X1F,
0X14,0XFF,0X0D,0X1F,0X14,0XFF,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,
0X0C,0XFF,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X3F,
0X0D,0X3F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,
0X0D,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,
0X15,0X7F,0X15,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,
0X15,0X7F,0X15,0X7F,0X1D,0X5F,0X1D,0X5F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X5F,
0X1D,0X5F,0X1D,0X7F,0X1D,0X7F,0X15,0X5E,0X1D,0X5E,0X25,0X5F,0X25,0X1E,0X2C,0XFB,
0X65,0XDC,0X6E,0X3D,0X24,0XD9,0X25,0X5D,0X15,0X7F,0X05,0X3E,0X05,0X3E,0X0D,0X3E,
0X15,0X5E,0X15,0X5E,0X15,0X5F,0X15,0X7F,0X15,0X5F,0X1D,0X3F,0X25,0X1E,0X25,0X1E,
0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X1D,0X3F,0X1D,0X5F,0X1D,0X3F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X25,0X5F,
0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X7F,0X25,0X7F,0X25,0X9F,0X25,0X9F,0X25,0XBF,
0X1D,0X9F,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,
0X2D,0XFF,0X2D,0XFF,0X2E,0X1F,0X2D,0XFF,0X36,0X1F,0X36,0X1E,0X36,0X1E,0X36,0X1E,
0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3E,0X1F,0X46,0X1F,0X46,0X1F,0X46,0X1F,0X46,0X3F,
0X46,0X1F,0X46,0X1F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X5F,
0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X5F,0X4E,0X3F,0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,
0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,0X4E,0X3E,0X46,0X3F,0X46,0X3F,0X46,0X3F,
0X46,0X5F,0X46,0X3F,0X46,0X3F,0X3E,0X3F,0X3E,0X1F,0X3E,0X1F,0X35,0XFF,0X2D,0XDE,
0X2D,0XFF,0X25,0XFF,0X25,0XDF,0X1D,0XBE,0X1D,0XBF,0X1D,0XDF,0X25,0XDF,0X25,0XFF,
0X15,0X3F,0X15,0X1F,0X0C,0XDE,0X0C,0XDE,0X14,0XFF,0X15,0X1F,0X15,0X1F,0X0C,0XFF,
0X15,0X1F,0X0C,0XFF,0X0C,0XFF,0X0C,0XDE,0X0C,0XDE,0X0C,0XFF,0X0C,0XFF,0X15,0X1E,
0X14,0X9C,0X0C,0X9B,0X14,0XDB,0X2C,0XF9,0X65,0X96,0XB6,0XB7,0XD6,0XD4,0XBD,0XF1,
0X64,0X90,0X55,0X16,0X1C,0XBA,0X04,0XBD,0X04,0XFE,0X04,0XDD,0X14,0XFC,0X1C,0XFC,
0X0C,0XBD,0X0C,0XBE,0X0C,0XBF,0X0C,0XDE,0X0C,0XDF,0X0C,0XDF,0X0C,0XDF,0X0C,0XDF,
0X0C,0XDF,0X0C,0XDF,0X0C,0XDF,0X14,0XFF,0X14,0XFF,0X14,0XFF,0X14,0XFF,0X0C,0XFF,
0X0C,0XFF,0X0C,0XFF,0X0C,0XFF,0X0C,0XFF,0X0C,0XFF,0X0C,0XFF,0X0D,0X1F,0X0D,0X1F,
0X0D,0X1F,0X0D,0X1F,0X0D,0X3F,0X0D,0X3F,0X0D,0X3F,0X0D,0X3F,0X0D,0X3F,0X0D,0X3F,
0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,
0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,
0X1D,0X5F,0X1D,0X5F,0X1D,0X3F,0X1D,0X3F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X3F,
0X15,0X3F,0X15,0X5F,0X15,0X3F,0X15,0X3E,0X1D,0X3E,0X25,0X3E,0X25,0X1E,0X2C,0XDB,
0X6D,0XFC,0X76,0X3D,0X2C,0XD9,0X25,0X3D,0X15,0X5F,0X05,0X3E,0X05,0X1E,0X0D,0X3E,
0X15,0X5E,0X15,0X5E,0X15,0X7F,0X15,0X7F,0X15,0X5F,0X1D,0X3F,0X25,0X1E,0X25,0X1E,
0X1D,0X3F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,
0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X7F,0X25,0X7F,0X25,0X7F,
0X25,0X5F,0X25,0X5F,0X25,0X7F,0X25,0X7F,0X25,0X9F,0X25,0XBF,0X25,0XBF,0X25,0XBF,
0X25,0XDF,0X25,0XDF,0X25,0XFF,0X25,0XFF,0X25,0XFF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,
0X2E,0X1F,0X36,0X1F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X3E,0X3F,0X3E,0X3E,0X3E,0X3E,
0X46,0X3E,0X46,0X3E,0X46,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,0X4E,0X3F,
0X4E,0X3F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X7F,0X4E,0X7F,
0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5E,
0X4E,0X5E,0X4E,0X5E,0X4E,0X5E,0X4E,0X5E,0X46,0X5F,0X46,0X5F,0X46,0X5F,0X46,0X5F,
0X46,0X5F,0X3E,0X3F,0X3E,0X3F,0X36,0X1F,0X36,0X1F,0X35,0XFF,0X2D,0XDF,0X25,0XBF,
0X25,0XDF,0X1D,0XDF,0X1D,0XBF,0X15,0XBE,0X1D,0XBF,0X1D,0XBF,0X1D,0XDF,0X1D,0XDF,
0X1D,0X1F,0X14,0XFF,0X0C,0XDF,0X0C,0XDF,0X14,0XFF,0X14,0XFF,0X14,0XFF,0X14,0XDF,
0X14,0XFF,0X14,0XFF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XFF,0X1C,0XFF,
0X35,0X5F,0X2D,0X1E,0X1C,0XBC,0X34,0XB9,0X6D,0X57,0XAE,0X16,0XB5,0X91,0X94,0X6D,
0X6C,0X30,0X5C,0XD7,0X24,0X9B,0X14,0XBE,0X14,0XDF,0X0C,0XBF,0X1C,0XDE,0X1C,0XBE,
0X14,0XBE,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,
0X14,0XBF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,
0X14,0XDF,0X14,0XFF,0X0C,0XFF,0X0C,0XFF,0X14,0XFF,0X14,0XFF,0X0C,0XFF,0X0C,0XFF,
0X15,0X1F,0X15,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,0X0D,0X1F,
0X0D,0X3F,0X0D,0X3F,0X0D,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X1D,0X3F,
0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X14,0XFF,
0X14,0XFE,0X15,0X1F,0X15,0X3E,0X15,0X1E,0X1D,0X1E,0X25,0X3F,0X25,0X3E,0X2C,0XDB,
0X76,0X3C,0X7E,0X7D,0X34,0XF9,0X25,0X5D,0X15,0X5F,0X05,0X1E,0X05,0X1E,0X0D,0X3E,
0X15,0X5E,0X15,0X5F,0X15,0X7F,0X15,0X7F,0X15,0X5F,0X1D,0X5F,0X25,0X1E,0X25,0X1E,
0X25,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,
0X1D,0X5F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X7F,0X1D,0X9F,0X25,0X7F,0X25,0X7F,
0X25,0X7F,0X2D,0X7F,0X2D,0X9F,0X25,0X9F,0X25,0XBF,0X25,0XBF,0X25,0XDF,0X25,0XDF,
0X25,0XFF,0X26,0X1F,0X26,0X1F,0X26,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X3F,0X2E,0X3F,
0X36,0X3F,0X36,0X3E,0X36,0X3E,0X3E,0X5E,0X3E,0X5E,0X46,0X5E,0X46,0X5E,0X46,0X5E,
0X4E,0X7F,0X4E,0X5E,0X56,0X5F,0X56,0X5F,0X56,0X5F,0X5E,0X5F,0X5E,0X5F,0X5E,0X7F,
0X56,0X7F,0X56,0X7F,0X56,0X7F,0X56,0X7F,0X56,0X7F,0X56,0X9F,0X56,0X9F,0X56,0X9F,
0X56,0X7F,0X56,0X7F,0X56,0X7F,0X56,0X7F,0X4E,0X7F,0X4E,0X7F,0X56,0X7F,0X4E,0X7F,
0X4E,0X5F,0X4E,0X7F,0X4E,0X7F,0X46,0X5F,0X46,0X7F,0X46,0X7F,0X46,0X5F,0X46,0X5F,
0X3E,0X5F,0X36,0X3F,0X36,0X1F,0X2E,0X1F,0X2D,0XFF,0X2D,0XDF,0X25,0XDF,0X1D,0XBF,
0X15,0XBF,0X15,0XBF,0X15,0XBF,0X15,0XBE,0X15,0XBF,0X15,0XBF,0X15,0XDF,0X15,0XDF,
0X1C,0XFF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X1C,0XFF,0X14,0XDF,0X14,0XDF,
0X1C,0XFF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X1C,0XDF,
0X1C,0X9E,0X24,0X5D,0X1C,0X3B,0X3C,0X99,0X85,0XB9,0XB6,0X58,0XB5,0X72,0X8C,0X2D,
0X5B,0X8F,0X5C,0X76,0X34,0X9B,0X1C,0XBF,0X1C,0XFF,0X14,0XBF,0X1C,0XBF,0X14,0X7E,
0X1C,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,
0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XBF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,
0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XDF,0X14,0XFF,0X14,0XFF,0X14,0XFF,0X14,0XFF,
0X14,0XFF,0X14,0XFF,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,
0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,
0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X1D,0X1F,
0X1D,0X3F,0X1D,0X1F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X14,0XFF,
0X14,0XFF,0X15,0X1F,0X15,0X1E,0X15,0X1E,0X1D,0X3E,0X25,0X5F,0X2D,0X3E,0X34,0XFB,
0X86,0X7D,0X8E,0X9D,0X3D,0X19,0X2D,0X5C,0X15,0X7F,0X05,0X3E,0X05,0X3E,0X0D,0X5E,
0X15,0X5E,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X1D,0X7F,0X1D,0X5F,0X25,0X5E,0X25,0X3E,
0X25,0X5F,0X25,0X5F,0X1D,0X7F,0X1D,0X7F,0X25,0X7F,0X25,0X7F,0X25,0X7F,0X25,0X7F,
0X25,0X7F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0XBF,0X25,0XBF,0X25,0XBF,
0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XDF,0X2D,0XDF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,
0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X36,0X3F,0X36,0X3F,
0X3E,0X5E,0X3E,0X5E,0X3E,0X5E,0X46,0X5E,0X46,0X7E,0X4E,0X7E,0X4E,0X7E,0X4E,0X7E,
0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X66,0X9F,0X66,0X7F,0X66,0X7F,0X66,0X9F,
0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X56,0X9F,
0X56,0X9F,0X56,0X9F,0X56,0X9F,0X56,0X9F,0X56,0X9F,0X4E,0X9F,0X4E,0X7F,0X4E,0X7F,
0X4E,0X7F,0X46,0X7F,0X46,0X7F,0X46,0X7F,0X46,0X5F,0X3E,0X5F,0X3E,0X5F,0X3E,0X5F,
0X36,0X3F,0X2E,0X1F,0X2D,0XFF,0X25,0XFF,0X25,0XDF,0X1D,0XDF,0X1D,0XBF,0X15,0X9F,
0X15,0X9F,0X0D,0XBF,0X0D,0X9F,0X0D,0X9F,0X0D,0XBF,0X0D,0XBF,0X15,0XDF,0X15,0XDF,
0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,
0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,
0X2C,0XBF,0X34,0XBE,0X34,0X9C,0X54,0XDA,0X95,0XDA,0XBE,0X38,0XB5,0X31,0X8C,0X0D,
0X63,0X6E,0X5C,0X56,0X3C,0X7B,0X24,0X9F,0X24,0XDF,0X1C,0XBF,0X24,0XBF,0X1C,0X7F,
0X1C,0X9F,0X1C,0XBF,0X1C,0X9F,0X1C,0X9F,0X14,0X9F,0X14,0X9F,0X14,0X9F,0X14,0X9F,
0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,
0X1C,0XBF,0X1C,0XBF,0X14,0XBF,0X1C,0XBF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,
0X1C,0XDF,0X1C,0XDF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X1C,0XFF,0X1C,0XFF,
0X15,0X1F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3E,0X25,0X5E,0X2D,0X5F,0X2D,0X5E,0X3D,0X1B,
0X8E,0X7C,0X96,0XBC,0X3D,0X18,0X2D,0X7C,0X15,0X7E,0X05,0X5F,0X0D,0X5E,0X15,0X7F,
0X15,0X7E,0X15,0X7F,0X1D,0X9F,0X1D,0X9F,0X1D,0X9F,0X25,0X9F,0X2D,0X7F,0X2D,0X7E,
0X25,0X7F,0X25,0X7F,0X25,0X7F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,
0X25,0XBF,0X25,0XBF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X2D,0XDF,
0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2E,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X3F,
0X36,0X1F,0X36,0X1F,0X36,0X3F,0X36,0X3F,0X3E,0X3F,0X3E,0X5F,0X3E,0X5F,0X46,0X5F,
0X46,0X7E,0X4E,0X7E,0X4E,0X7E,0X4E,0X9F,0X56,0X9F,0X56,0XBF,0X56,0XBF,0X5E,0XBF,
0X5E,0XDF,0X66,0XBF,0X66,0XBF,0X66,0XBF,0X66,0XBF,0X66,0XBF,0X66,0X9F,0X66,0X9F,
0X5E,0X9F,0X5E,0XBF,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X56,0X9F,0X56,0X9F,0X56,0X9F,
0X56,0X9F,0X56,0X9F,0X56,0X9F,0X56,0X9F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,
0X46,0X7F,0X46,0X7F,0X3E,0X5F,0X3E,0X5F,0X3E,0X3F,0X36,0X3F,0X36,0X1F,0X36,0X3F,
0X2D,0XFF,0X25,0XFF,0X1D,0XDF,0X1D,0XBF,0X1D,0XBF,0X15,0X9F,0X15,0X9F,0X0D,0X7F,
0X0D,0X9F,0X0D,0X9F,0X0D,0X9F,0X0D,0X9F,0X0D,0XBF,0X0D,0XBF,0X15,0XDF,0X15,0XDF,
0X1C,0XBF,0X1C,0XBF,0X1C,0XDF,0X1C,0XDF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,
0X1C,0X9F,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0X9E,
0X24,0X5D,0X34,0X7C,0X3C,0X7A,0X5C,0XB8,0X9D,0XD8,0XC6,0X78,0XBD,0XB3,0XA4,0XD0,
0X6B,0XCE,0X6C,0X95,0X44,0X79,0X24,0X5D,0X1C,0X9F,0X14,0X9F,0X24,0XBF,0X1C,0X9F,
0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,
0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,
0X1C,0X9F,0X1C,0X7F,0X1C,0X7F,0X1C,0X9F,0X24,0XBF,0X24,0XBF,0X24,0XBF,0X1C,0XBF,
0X1C,0XBF,0X1C,0XDF,0X1C,0XDF,0X24,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,
0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XDE,0X1C,0XDE,0X1C,0XFE,0X1C,0XFF,0X24,0XFF,0X24,0XFF,0X25,0X1F,0X1D,0X3F,
0X1D,0X3F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X25,0X5E,0X2D,0X7F,0X35,0X5E,0X3D,0X1A,
0X96,0X7B,0X9E,0XBA,0X45,0X17,0X2D,0X5B,0X15,0X7E,0X05,0X5F,0X0D,0X7F,0X15,0X9F,
0X15,0X7E,0X1D,0X7E,0X1D,0X9F,0X1D,0X9F,0X1D,0XBF,0X25,0XBF,0X2D,0X9F,0X2D,0X9F,
0X2D,0X9F,0X25,0X9F,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,
0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2E,0X1F,
0X2E,0X1F,0X2E,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X3F,0X36,0X3F,0X3E,0X3F,
0X3E,0X3F,0X3E,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X5F,0X4E,0X5F,0X4E,0X7F,0X4E,0X7F,
0X56,0X9F,0X56,0X9F,0X5E,0X9F,0X5E,0XBF,0X5E,0XDF,0X5E,0XDF,0X66,0XDF,0X66,0XFF,
0X66,0XDF,0X66,0XDF,0X66,0XDF,0X66,0XDF,0X66,0XBF,0X5E,0XBF,0X5E,0XBF,0X66,0XBF,
0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X56,0X7F,0X56,0X7F,0X56,0X7F,0X56,0X7F,
0X56,0X9F,0X56,0X9F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X46,0X7F,
0X46,0X5F,0X3E,0X5F,0X3E,0X3F,0X36,0X3F,0X36,0X1F,0X2E,0X1F,0X2D,0XFF,0X2D,0XFF,
0X25,0XDF,0X1D,0XBF,0X1D,0XBF,0X15,0X9F,0X15,0X9F,0X0D,0X7F,0X0D,0X7F,0X0D,0X7F,
0X0D,0X9F,0X05,0X9F,0X0D,0X9F,0X0D,0X9F,0X0D,0XBF,0X0D,0XBF,0X15,0XDF,0X0D,0XDF,
0X24,0XFF,0X24,0XDF,0X1C,0XBF,0X1C,0X9E,0X1C,0X9E,0X1C,0X9E,0X1C,0X9E,0X1C,0XBF,
0X24,0XBF,0X24,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X24,0XBF,0X24,0XBF,0X24,0XBE,
0X34,0XFD,0X23,0XF7,0X65,0X9B,0XB7,0X5E,0XAE,0X77,0XB5,0XF3,0XC5,0XF1,0XA4,0XEE,
0XB5,0XD4,0X7C,0XF3,0X54,0XB7,0X3C,0XBB,0X1C,0X7D,0X14,0X9E,0X1C,0X9E,0X14,0X5D,
0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,0X1C,0X7F,
0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X1C,0X7F,0X1C,0X7F,
0X1C,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X7F,0X24,0X7F,0X24,0X7F,0X24,0X9F,0X24,0X9F,
0X24,0X7F,0X24,0X9F,0X24,0X9F,0X24,0XBF,0X24,0XBF,0X24,0XBF,0X24,0X9F,0X24,0X9F,
0X1C,0X7F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0X9F,0X1C,0XBF,
0X1C,0X9F,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XBF,0X1C,0XDF,0X1C,0XDE,
0X1C,0XDE,0X1C,0XFE,0X1C,0XFF,0X25,0X1F,0X25,0X3F,0X25,0X3F,0X25,0X5F,0X25,0X5F,
0X25,0X7F,0X25,0XBF,0X25,0X9F,0X2D,0XFF,0X25,0X3D,0X2D,0X7E,0X2D,0X5D,0X3D,0X19,
0XA6,0XDB,0XA6,0XB9,0X55,0X78,0X2D,0X3A,0X1D,0X7E,0X05,0X5E,0X15,0XBF,0X0D,0X7F,
0X1D,0X9F,0X1D,0X9F,0X1D,0XBF,0X1D,0XBF,0X25,0XDF,0X25,0XDF,0X2D,0XDF,0X2D,0XBF,
0X2D,0XBF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,
0X2D,0XDF,0X2D,0XFF,0X2D,0XFF,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X2E,0X1F,
0X2E,0X1F,0X2E,0X3E,0X2E,0X3E,0X36,0X3E,0X36,0X3F,0X3E,0X3F,0X3E,0X5F,0X46,0X5F,
0X46,0X3E,0X4E,0X3E,0X4E,0X3E,0X56,0X5F,0X56,0X5F,0X5E,0X7F,0X5E,0X9F,0X5E,0X9F,
0X66,0X9F,0X66,0XBF,0X66,0XBF,0X66,0XDF,0X66,0XDF,0X66,0XDF,0X66,0XDF,0X66,0XDF,
0X66,0XDF,0X66,0XDF,0X5E,0XDF,0X5E,0XDF,0X5E,0XDF,0X56,0XBF,0X56,0XBF,0X56,0X9F,
0X56,0X5F,0X56,0X5F,0X56,0X5F,0X56,0X5F,0X56,0X5F,0X56,0X5F,0X56,0X5F,0X56,0X7F,
0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X4E,0X7F,0X46,0X7F,0X46,0X5F,0X3E,0X3F,
0X3E,0X3F,0X36,0X3F,0X36,0X1F,0X2D,0XFF,0X2D,0XDF,0X25,0XDF,0X25,0XBF,0X1D,0XBF,
0X1D,0X9F,0X1D,0X9F,0X15,0X9F,0X15,0X7F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X05,0X5F,
0X05,0X5F,0X05,0X7E,0X05,0X7F,0X05,0X7F,0X0D,0X9F,0X0D,0X9F,0X0D,0X9F,0X0D,0XBF,
0X2C,0XDF,0X24,0XDF,0X24,0XBF,0X24,0X9E,0X1C,0X9E,0X1C,0X9E,0X24,0X9F,0X24,0XBE,
0X24,0X9F,0X24,0XBE,0X24,0X9F,0X24,0X9E,0X24,0X9F,0X24,0XBE,0X24,0X9F,0X24,0X9D,
0X2C,0X7A,0X34,0X57,0X75,0X99,0X9E,0X79,0XA5,0XD3,0X94,0XCC,0X9C,0XEC,0XA4,0XEC,
0XA5,0X50,0X84,0XF1,0X5C,0XB5,0X3C,0XB9,0X24,0X9C,0X1C,0XBE,0X24,0XDE,0X1C,0X9E,
0X1C,0X7E,0X1C,0X7F,0X24,0X7F,0X1C,0X7F,0X24,0X7F,0X1C,0X7F,0X24,0X7F,0X1C,0X7F,
0X24,0X7F,0X1C,0X7F,0X24,0X7F,0X1C,0X7F,0X24,0X7F,0X1C,0X7F,0X24,0X7F,0X24,0X7F,
0X24,0X5F,0X24,0X3F,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X7F,
0X24,0X5F,0X24,0X7F,0X24,0X7F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X7F,
0X24,0X7F,0X24,0X7F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0XBF,0X24,0XBF,
0X24,0XDF,0X24,0XDF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X25,0X1F,0X25,0X1F,0X25,0X3F,
0X25,0X3F,0X25,0X5F,0X25,0X5F,0X2D,0X5F,0X2D,0X7F,0X2D,0X7F,0X25,0X7F,0X25,0X9F,
0X1D,0X7F,0X25,0XBF,0X1D,0X9F,0X2D,0XDF,0X25,0X5D,0X35,0XBE,0X35,0X9E,0X4D,0X7A,
0XBF,0X7C,0XBF,0X3A,0X6D,0XF9,0X3D,0X9A,0X25,0XBE,0X0D,0X9F,0X15,0XDF,0X15,0X9F,
0X1D,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XFF,0X25,0XFF,0X2D,0XFF,0X35,0XFF,0X35,0XFF,
0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2E,0X1F,0X35,0XFF,0X36,0X1F,0X36,0X1F,0X36,0X1F,
0X2E,0X1F,0X2E,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X1F,0X36,0X3F,
0X36,0X5E,0X36,0X5E,0X36,0X5E,0X36,0X5E,0X3E,0X5E,0X46,0X5E,0X46,0X5E,0X4E,0X5E,
0X56,0X5E,0X56,0X5F,0X5E,0X7F,0X5E,0X7F,0X66,0X7F,0X66,0X9F,0X6E,0XBF,0X6E,0XBF,
0X6E,0XBF,0X6E,0XBF,0X6E,0XBF,0X6E,0XDF,0X6E,0XDF,0X66,0XDF,0X66,0XDF,0X5E,0XDF,
0X5E,0XBF,0X56,0XBF,0X56,0XBF,0X56,0XBF,0X4E,0X9F,0X4E,0X9E,0X4E,0X9E,0X4E,0X7E,
0X4E,0X3E,0X56,0X3F,0X56,0X3F,0X56,0X5F,0X56,0X5F,0X56,0X5F,0X4E,0X5F,0X4E,0X5F,
0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X46,0X5F,0X46,0X5F,0X46,0X5F,0X3E,0X3F,0X3E,0X1F,
0X35,0XFF,0X2D,0XFF,0X2D,0XDF,0X2D,0XBF,0X25,0X9F,0X1D,0X9F,0X1D,0X9F,0X1D,0X7F,
0X15,0X7F,0X15,0X7F,0X15,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,0X0D,0X5F,
0X0D,0X5F,0X0D,0X7F,0X0D,0X7F,0X0D,0X7F,0X0D,0X7F,0X0D,0X9F,0X15,0X9F,0X15,0X9F,
0X2C,0XDF,0X2C,0XDF,0X24,0XBF,0X24,0XBE,0X24,0X9E,0X24,0X9E,0X24,0XBF,0X24,0XBF,
0X24,0X9E,0X24,0X9E,0X24,0X9E,0X24,0X9E,0X24,0X9E,0X24,0X9E,0X24,0X9E,0X2C,0X9D,
0X2C,0X7A,0X44,0X77,0X75,0X36,0X7C,0XF2,0X9D,0X50,0X7C,0X09,0X84,0X29,0X94,0X8A,
0X7B,0XC9,0X7C,0X4E,0X5C,0X74,0X3C,0X57,0X24,0X5B,0X14,0X7D,0X14,0X5D,0X14,0X5D,
0X24,0X7E,0X24,0X7E,0X24,0X7F,0X24,0X7E,0X24,0X7F,0X24,0X7E,0X24,0X7F,0X24,0X7E,
0X1C,0X5E,0X1C,0X5E,0X24,0X5E,0X1C,0X7E,0X24,0X5E,0X24,0X7E,0X24,0X5F,0X24,0X5F,
0X24,0X5E,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X5F,0X24,0X5E,
0X24,0X5E,0X24,0X5E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,
0X24,0X7E,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0X9F,0X24,0XBF,0X24,0XBF,0X24,0XDF,
0X24,0XFF,0X2C,0XFF,0X2D,0X1F,0X2D,0X3F,0X2D,0X5F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,
0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X25,0XBF,0X25,0XBF,
0X25,0XDF,0X25,0XFF,0X25,0XBF,0X2D,0XFF,0X2D,0X9D,0X3D,0XDE,0X3D,0X9D,0X55,0X9A,
0XC7,0X7B,0XBF,0X18,0X76,0X18,0X3D,0X9A,0X25,0XBE,0X15,0XBF,0X15,0XFF,0X15,0XBF,
0X26,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X3F,0X2E,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,
0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,
0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X1F,0X36,0X3F,
0X36,0X5E,0X36,0X7D,0X3E,0X7E,0X3E,0X7E,0X46,0X7E,0X4E,0X7E,0X4E,0X7E,0X56,0X7E,
0X5E,0X9F,0X66,0X9F,0X66,0X9F,0X6E,0XBF,0X6E,0XBF,0X6E,0XDF,0X76,0XBF,0X6E,0XBF,
0X6E,0XBF,0X6E,0XBF,0X66,0XBF,0X66,0XBF,0X66,0XBF,0X5E,0X9F,0X5E,0X9F,0X56,0X9F,
0X56,0X7F,0X4E,0X7F,0X4E,0X7F,0X46,0X5F,0X3E,0X5E,0X3E,0X5E,0X3E,0X5E,0X3E,0X5E,
0X46,0X1E,0X4E,0X1E,0X4E,0X3F,0X4E,0X3F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,0X4E,0X5F,
0X46,0X3F,0X46,0X3F,0X46,0X3F,0X3E,0X3F,0X3E,0X1F,0X3E,0X1F,0X35,0XDF,0X2D,0XDF,
0X25,0XBF,0X25,0X9F,0X25,0X9F,0X1D,0X7F,0X1D,0X5E,0X1D,0X5E,0X15,0X5E,0X15,0X5F,
0X15,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,0X0D,0X3F,0X15,0X3F,0X15,0X5F,0X0D,0X5F,
0X15,0X5F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X7F,0X15,0X9F,
0X34,0XFF,0X2C,0XFF,0X2C,0XDF,0X2C,0XDF,0X2C,0XBF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,
0X2C,0XBF,0X2C,0XBF,0X2C,0XBF,0X2C,0XBF,0X2C,0XBF,0X2C,0XBF,0X2C,0XBE,0X2C,0XBD,
0X3C,0XBB,0X44,0X57,0X74,0XF5,0X7C,0XB0,0XAD,0XB1,0X9C,0XCC,0XA5,0X0C,0X9C,0XAB,
0X41,0XC1,0X5B,0X4A,0X53,0XF1,0X3B,0XF5,0X2C,0X7C,0X2C,0XDF,0X2C,0XBF,0X34,0XFF,
0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,
0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X5E,0X24,0X7E,0X24,0X7E,0X24,0X7E,
0X24,0X7E,0X24,0X5E,0X24,0X5E,0X24,0X5E,0X24,0X7E,0X24,0X7E,0X24,0X5E,0X24,0X5E,
0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X9E,0X24,0X9E,0X24,0X9E,0X24,0X9E,0X24,0X9E,
0X24,0X7E,0X24,0X9E,0X24,0XBF,0X24,0XBF,0X24,0XBF,0X24,0XDF,0X24,0XFF,0X2C,0XFF,
0X2D,0X1F,0X2D,0X3F,0X2D,0X3F,0X2D,0X5F,0X2D,0X7F,0X2D,0X9F,0X2D,0X9F,0X2D,0XBF,
0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X25,0XDF,0X25,0XDF,
0X25,0XFF,0X2E,0X1F,0X1D,0XBE,0X2E,0X1F,0X2D,0XDD,0X3D,0XFD,0X3D,0XBC,0X65,0XDA,
0XCF,0X9A,0XC7,0X37,0X7E,0X38,0X45,0X99,0X25,0XDD,0X15,0XFF,0X15,0XFF,0X15,0XDF,
0X26,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5F,0X36,0X5F,0X36,0X5F,0X36,0X5F,
0X36,0X5F,0X36,0X5F,0X36,0X5F,0X36,0X5F,0X36,0X5F,0X36,0X5F,0X36,0X3F,0X36,0X3F,
0X36,0X5F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X3E,0X3F,0X3E,0X5F,
0X3E,0X5E,0X3E,0X7E,0X46,0X7E,0X46,0X9E,0X4E,0X9E,0X56,0X9F,0X56,0XBF,0X5E,0X9F,
0X66,0XBF,0X66,0XBF,0X6E,0XBF,0X6E,0XDF,0X6E,0XDF,0X6E,0XBF,0X6E,0XBF,0X66,0X9F,
0X66,0X9F,0X5E,0X9F,0X5E,0X7F,0X5E,0X7F,0X56,0X5F,0X4E,0X5F,0X4E,0X3F,0X4E,0X3F,
0X46,0X3F,0X46,0X3F,0X3E,0X1F,0X3E,0X1E,0X36,0X1E,0X36,0X3E,0X3E,0X3E,0X3E,0X3E,
0X46,0X1F,0X46,0X1F,0X4E,0X3F,0X4E,0X3F,0X4E,0X5F,0X4E,0X5F,0X46,0X3F,0X46,0X3F,
0X3E,0X1F,0X3D,0XFF,0X3D,0XFF,0X35,0XFF,0X35,0XDF,0X2D,0XBF,0X2D,0X9F,0X25,0X7F,
0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X3F,0X1D,0X3F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X3F,0X15,0X3F,0X15,0X3F,
0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,0X15,0X5F,
0X34,0XFF,0X34,0XFF,0X34,0XFF,0X2C,0XFF,0X34,0XDF,0X2C,0XFF,0X34,0XFF,0X34,0XFF,
0X34,0XDF,0X2C,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XDF,0X2C,0XDF,0X2C,0XDF,0X34,0XBD,
0X44,0XBC,0X5C,0XB8,0XAE,0X3A,0XBD,0XF5,0XCE,0X33,0XBD,0X8F,0XD6,0X31,0XB5,0X4E,
0X8B,0XC9,0XA5,0X11,0X95,0X56,0X5C,0XB8,0X3C,0X7C,0X24,0X5E,0X1C,0X1D,0X1C,0X1E,
0X24,0X7F,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,
0X2C,0X9F,0X24,0X9E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,
0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X9E,0X1C,0X7E,0X1C,0X7E,
0X24,0X9E,0X1C,0X9E,0X1C,0X9E,0X1C,0X9E,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,
0X24,0XBE,0X24,0XDE,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X25,0X1E,0X25,0X3F,0X25,0X5F,
0X2D,0X7F,0X2D,0X7F,0X2D,0X9F,0X2D,0X9F,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X25,0XDF,
0X2D,0XFF,0X25,0XFF,0X25,0XFF,0X25,0XFF,0X2D,0XFF,0X25,0XFF,0X25,0XFF,0X25,0XFF,
0X1D,0XDF,0X26,0X1F,0X1D,0XDD,0X2E,0X3F,0X36,0X1E,0X3E,0X3D,0X3D,0XDC,0X76,0X7B,
0XDF,0XDA,0XD7,0X77,0X96,0XB9,0X4D,0XFA,0X2D,0XDD,0X1E,0X1F,0X15,0XFF,0X1E,0X1F,
0X26,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X36,0X5F,0X36,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,
0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X3E,0X3F,0X3E,0X5F,0X3E,0X5F,0X46,0X7F,
0X46,0X7E,0X46,0X9E,0X4E,0X9E,0X56,0XBF,0X56,0XBF,0X5E,0XDF,0X5E,0XDF,0X66,0XDF,
0X66,0XDF,0X66,0XDF,0X66,0XDF,0X66,0XDF,0X66,0XBF,0X5E,0X9F,0X56,0X9F,0X56,0X7F,
0X4E,0X7F,0X4E,0X5F,0X4E,0X5F,0X46,0X3F,0X46,0X1F,0X3E,0X1F,0X3D,0XFF,0X3D,0XFF,
0X3D,0XFF,0X35,0XFF,0X35,0XDF,0X35,0XFF,0X35,0XFF,0X36,0X1F,0X3E,0X1F,0X3E,0X3F,
0X46,0X1F,0X46,0X1F,0X46,0X3F,0X46,0X3F,0X46,0X3F,0X46,0X1F,0X3E,0X1F,0X3D,0XFF,
0X35,0XDF,0X35,0XBF,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X25,0X7F,0X25,0X5F,0X1D,0X3F,
0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,
0X1D,0X1F,0X1D,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,0X15,0X1F,
0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,0X1D,0X3F,
0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,
0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XDF,0X34,0XBD,
0X44,0X9A,0X7D,0X9B,0XE7,0X9E,0XE6,0XF8,0XCD,0XD1,0XB4,0XEC,0XC5,0XAF,0XB5,0X0C,
0XB4,0XEC,0XC5,0XB2,0XAD,0XD7,0X75,0X19,0X4C,0XBC,0X34,0XBF,0X34,0XBF,0X34,0XDF,
0X2C,0X7E,0X2C,0X7E,0X2C,0X7E,0X2C,0X7E,0X2C,0X7E,0X2C,0X7E,0X2C,0X7E,0X2C,0X7E,
0X2C,0X9F,0X2C,0X9E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X24,0X7E,0X2C,0X7E,0X24,0X9E,
0X24,0X9E,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,
0X24,0XBE,0X1C,0XDE,0X1C,0XDE,0X1C,0XDE,0X24,0XDE,0X24,0XFE,0X24,0XFE,0X24,0XFE,
0X25,0X1F,0X2D,0X3F,0X2D,0X3F,0X2D,0X5F,0X2D,0X5F,0X2D,0X5F,0X2D,0X7F,0X2D,0X9F,
0X2D,0XBF,0X2D,0XDF,0X2D,0XDF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X25,0XFF,0X25,0XFF,
0X2E,0X3F,0X26,0X3F,0X26,0X3F,0X26,0X1F,0X26,0X1F,0X26,0X3F,0X26,0X3F,0X26,0X3F,
0X1E,0X1F,0X2E,0X7F,0X1D,0XFE,0X36,0X5E,0X36,0X3E,0X3E,0X1C,0X3D,0XDB,0X7E,0XBB,
0XCF,0X57,0XCF,0X35,0X96,0XD9,0X4D,0XD9,0X25,0XDC,0X1E,0X3F,0X15,0XFF,0X1E,0X3F,
0X26,0X1F,0X2E,0X3E,0X2E,0X3E,0X2E,0X3E,0X2E,0X5F,0X2E,0X5F,0X2E,0X5E,0X2E,0X5E,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,0X36,0X5F,0X36,0X3F,0X36,0X3F,
0X36,0X3F,0X36,0X3F,0X3E,0X3F,0X3E,0X5F,0X46,0X5F,0X46,0X7F,0X46,0X7F,0X4E,0X7F,
0X56,0X9F,0X56,0X9F,0X56,0XBF,0X5E,0XBF,0X5E,0XDF,0X66,0XDF,0X66,0XDF,0X5E,0XDF,
0X5E,0XBF,0X5E,0XBF,0X5E,0XBF,0X56,0X9F,0X4E,0X9F,0X4E,0X7F,0X46,0X5F,0X46,0X5F,
0X3E,0X5F,0X3E,0X3F,0X3E,0X1F,0X36,0X1F,0X35,0XFF,0X35,0XDF,0X35,0XDF,0X35,0XBF,
0X35,0XBF,0X2D,0XBF,0X35,0XBF,0X35,0XBF,0X35,0XDF,0X35,0XDF,0X3D,0XFF,0X3D,0XFF,
0X3D,0XFF,0X3D,0XFF,0X3D,0XFF,0X3D,0XFF,0X3D,0XFF,0X35,0XDF,0X35,0XBF,0X35,0XBF,
0X2D,0X9F,0X2D,0X7F,0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X3F,0X1D,0X3F,0X1D,0X1F,
0X1D,0X3F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,
0X1D,0X1F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,
0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,
0X34,0XFE,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFE,0X2C,0XDE,0X34,0XBC,
0X4C,0X99,0X8D,0XD9,0XCE,0X97,0XC5,0XB0,0XB4,0XEB,0XA4,0X88,0XAC,0XC8,0XA4,0XA8,
0XBC,0XEA,0XBD,0X2E,0XA5,0X73,0X7D,0X16,0X44,0X79,0X34,0X7C,0X34,0X9D,0X2C,0X7D,
0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,
0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0XBF,0X34,0XBF,0X2C,0XDF,
0X2C,0XFF,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X3F,
0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X5F,0X2D,0X5F,0X2D,0X7F,
0X2D,0X9F,0X2D,0X9F,0X35,0XBF,0X35,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XDF,0X2D,0XFF,
0X2D,0XFF,0X2E,0X1F,0X2E,0X1F,0X2E,0X3F,0X2E,0X3F,0X26,0X3F,0X26,0X3F,0X26,0X3F,
0X26,0X7F,0X26,0X5F,0X26,0X5F,0X26,0X3F,0X26,0X3F,0X26,0X5F,0X26,0X5F,0X26,0X5F,
0X26,0X5F,0X36,0XBF,0X26,0X1E,0X36,0X7E,0X36,0X3D,0X3D,0XFC,0X3D,0XFB,0X8F,0X3D,
0XCF,0X36,0XD7,0X34,0XA7,0X19,0X55,0XF9,0X25,0XBC,0X1E,0X1F,0X0D,0XDF,0X1E,0X3F,
0X26,0X3F,0X2E,0X3E,0X2E,0X3E,0X2E,0X3E,0X2E,0X5E,0X2E,0X5E,0X2E,0X5E,0X2E,0X5E,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X36,0X5F,0X36,0X5F,
0X36,0X5F,0X3E,0X5F,0X3E,0X5F,0X46,0X7F,0X46,0X7F,0X4E,0X9F,0X4E,0X9F,0X4E,0X9F,
0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X5E,0X9F,0X56,0X7F,0X56,0X7F,
0X4E,0X7F,0X4E,0X7F,0X46,0X5F,0X46,0X5F,0X3E,0X5F,0X36,0X3E,0X2E,0X3E,0X2E,0X3E,
0X2E,0X3E,0X2E,0X1E,0X2D,0XFE,0X25,0XFE,0X25,0XBE,0X25,0XBE,0X25,0X9F,0X2D,0X9F,
0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X35,0X9F,0X35,0X9F,0X35,0XBF,0X35,0XBF,
0X35,0XBF,0X35,0XBF,0X35,0XBF,0X2D,0X9F,0X2D,0X9F,0X2D,0X7F,0X25,0X7F,0X25,0X5F,
0X25,0X5F,0X25,0X3F,0X1D,0X3F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1C,0XFF,
0X1D,0X1F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XFF,
0X1C,0XDF,0X1C,0XFE,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X34,0XBE,0X2C,0XDE,0X2C,0XDE,0X2C,0XDE,0X2C,0XDE,0X34,0XDF,0X34,0XDF,0X3C,0XDF,
0X34,0XBE,0X34,0XDE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XDE,0X2C,0XBD,0X34,0X9A,
0X54,0X97,0X8D,0X55,0X8C,0X6D,0X83,0X86,0XA4,0X68,0XBD,0X0A,0XAC,0XC9,0XAC,0XC9,
0X7B,0X44,0X83,0X86,0X8C,0X4D,0X7C,0XB2,0X4C,0X56,0X3C,0X9A,0X34,0XFE,0X24,0X9D,
0X2C,0XBD,0X2C,0XBD,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,
0X2C,0XBE,0X2C,0XBD,0X2C,0XBD,0X2C,0XBD,0X2C,0XDE,0X2C,0XFF,0X2D,0X3F,0X2D,0X5F,
0X2D,0X5F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,
0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XDF,0X2D,0XDF,0X35,0XFF,
0X35,0XDF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X35,0XFF,0X36,0X1F,0X36,0X3F,
0X2E,0X1F,0X2E,0X1F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X26,0X5F,
0X2E,0X7F,0X2E,0X7F,0X26,0X5F,0X26,0X5F,0X26,0X3F,0X26,0X3F,0X2E,0X3F,0X2E,0X5F,
0X1E,0X1E,0X26,0X7F,0X15,0XFD,0X2E,0X5E,0X36,0X5E,0X46,0X3D,0X56,0X5C,0XAF,0XFF,
0XEF,0XFA,0XEF,0XF8,0XBF,0XFD,0X6E,0X9C,0X2D,0XFD,0X1E,0X3F,0X0D,0XDE,0X1E,0X3F,
0X2E,0X3E,0X36,0X1E,0X2E,0X3E,0X2E,0X5E,0X2E,0X5F,0X2E,0X5F,0X2E,0X5E,0X2E,0X5E,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X7F,0X36,0X7F,0X36,0X7F,0X36,0X7F,
0X3E,0X5F,0X3E,0X7F,0X3E,0X7F,0X46,0X9F,0X46,0X9F,0X4E,0X9F,0X4E,0XBF,0X4E,0X9F,
0X56,0X9F,0X56,0X9F,0X56,0X9F,0X56,0X7F,0X56,0X7F,0X4E,0X5F,0X46,0X3F,0X46,0X1F,
0X3E,0X1F,0X3E,0X1F,0X36,0X1F,0X36,0X1F,0X2D,0XFE,0X25,0XFE,0X25,0XFE,0X25,0XFE,
0X25,0XFE,0X25,0XDE,0X1D,0XBE,0X1D,0XBE,0X1D,0X9E,0X25,0X9E,0X25,0X7F,0X25,0X7F,
0X25,0X7F,0X25,0X7F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,
0X2D,0X7F,0X2D,0X7F,0X2D,0X5F,0X25,0X5E,0X25,0X3E,0X25,0X3F,0X25,0X3F,0X25,0X3F,
0X25,0X1F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X14,0XFF,0X14,0XFF,0X14,0XDF,0X14,0XDF,0X1C,0XDF,0X1C,0XDF,
0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,0X1C,0XDF,
0X1C,0XFF,0X1C,0XFE,0X1C,0XFE,0X1C,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,
0X3C,0XDF,0X34,0XDF,0X2C,0XFE,0X2C,0XFD,0X2C,0XDF,0X3C,0XDF,0X44,0X9F,0X44,0X7F,
0X3C,0X9D,0X44,0XFE,0X34,0XFD,0X34,0XFE,0X35,0X1F,0X2C,0XBC,0X2C,0XBB,0X55,0X7B,
0X5C,0X94,0XB6,0X37,0XDE,0XB5,0XDE,0X11,0XDD,0XEF,0XD5,0XCF,0XCE,0X11,0XC5,0XF1,
0X73,0X46,0X8C,0X2A,0XA4,0XAD,0X8C,0X8F,0X64,0X93,0X55,0X1A,0X35,0X5F,0X1D,0X3F,
0X25,0X1D,0X35,0X3D,0X2D,0X5E,0X25,0X3E,0X25,0X1E,0X25,0X5F,0X25,0X5E,0X2D,0X3D,
0X35,0X3D,0X35,0X3C,0X3D,0X5C,0X35,0X5D,0X35,0X7E,0X2D,0X9F,0X25,0XBF,0X25,0XBF,
0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,0X2E,0X1F,0X2E,0X1F,0X36,0X1F,0X2E,0X1F,
0X36,0X3F,0X36,0X3F,0X36,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X36,0X3F,
0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3F,
0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5E,0X2E,0X5F,0X26,0X5F,0X26,0X3F,0X2E,0X3F,0X36,0X3F,0X36,0X3F,0X36,0X3E,
0X2E,0X5F,0X26,0X7F,0X1E,0X5F,0X2E,0XBF,0X2D,0XFE,0X4E,0X3E,0X76,0X7D,0XAF,0X3D,
0XBE,0X96,0XCF,0X17,0XA6,0XBB,0X55,0X9A,0X36,0X3F,0X15,0XDF,0X0D,0XFE,0X26,0X3F,
0X35,0XFE,0X3D,0XFD,0X36,0X1E,0X2E,0X5F,0X26,0X9F,0X26,0X7F,0X36,0X5F,0X36,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X36,0X5F,
0X36,0X7F,0X36,0X5F,0X36,0X7F,0X3E,0X7F,0X3E,0X7F,0X3E,0X7F,0X3E,0X5F,0X3E,0X5F,
0X46,0X5F,0X46,0X5F,0X46,0X3F,0X3E,0X3F,0X3E,0X1F,0X36,0X1F,0X35,0XFF,0X2D,0XFF,
0X2D,0XDF,0X2D,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XBF,0X25,0XBF,0X1D,0XBF,0X1D,0XBF,
0X1D,0X9F,0X1D,0X9F,0X1D,0X7F,0X1D,0X7F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,0X1D,0X5F,
0X1D,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,
0X25,0X3F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X1C,0XFF,0X1C,0XFE,0X1C,0XFF,0X1C,0XFF,
0X25,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XDF,0X1C,0XDF,0X1C,0XFF,
0X1C,0XFF,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X1C,0XFE,
0X14,0XFF,0X1C,0XFF,0X1C,0XFF,0X1D,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,
0X34,0XBF,0X2C,0XBE,0X2C,0XDD,0X24,0XFD,0X34,0XFF,0X3C,0XDF,0X3C,0X9F,0X3C,0X7F,
0X34,0X5C,0X34,0XBC,0X2C,0XDC,0X34,0XFD,0X3D,0X3F,0X34,0XFD,0X3D,0X3B,0X6D,0XDB,
0XB7,0X1C,0XDF,0X39,0XD6,0X53,0XCD,0XAF,0XBD,0X0C,0XBD,0X2C,0XB5,0X6F,0XBD,0XF1,
0XC6,0X12,0XD6,0X73,0XDE,0X93,0XCE,0X53,0X9D,0XF6,0X6D,0XDA,0X35,0XDF,0X1D,0XBF,
0X2D,0XBF,0X35,0XBE,0X35,0XDF,0X35,0XBF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X35,0XDF,
0X35,0XBE,0X3D,0XBD,0X3D,0XBD,0X3D,0XBD,0X35,0XDE,0X2D,0XFF,0X26,0X1F,0X26,0X1F,
0X2E,0X1F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,
0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,
0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,
0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5E,0X2E,0X5F,0X26,0X5F,0X26,0X3F,0X2E,0X3F,0X36,0X3F,0X3E,0X3E,0X36,0X3E,
0X26,0X1D,0X26,0X5F,0X1E,0X7F,0X26,0X9F,0X2E,0X3F,0X56,0X3D,0X7E,0X7D,0XB7,0X1C,
0XC6,0XB7,0XDF,0X5A,0XAE,0XDC,0X5D,0X79,0X36,0X1E,0X15,0XFF,0X16,0X3F,0X1E,0X3F,
0X35,0XFE,0X3D,0XFE,0X36,0X1F,0X2E,0X5F,0X26,0X7F,0X26,0X7F,0X2E,0X5F,0X36,0X3F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,
0X26,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2D,0XFF,0X2D,0XFF,0X2D,0XFF,
0X2D,0XFF,0X2D,0XFF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X25,0XBF,0X25,0XBF,0X25,0XBF,
0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X7F,
0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,
0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X3F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,
0X25,0X1F,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,
0X24,0XFF,0X24,0XFF,0X24,0XFF,0X1C,0XFF,0X24,0XDF,0X24,0XDF,0X24,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,
0X1C,0XFF,0X1C,0XFF,0X1C,0XFF,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,
0X1D,0X3F,0X1D,0X3F,0X1D,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X2C,0XFF,0X2C,0XFF,
0X34,0XDE,0X34,0XDE,0X2C,0XFD,0X35,0X1E,0X35,0X1F,0X35,0X1F,0X2C,0XFF,0X2C,0XFE,
0X2D,0X3D,0X35,0X7E,0X35,0X5E,0X3D,0X5F,0X45,0X5F,0X45,0X3E,0X45,0X7C,0X6E,0X1A,
0XBF,0X5B,0XC6,0X74,0XBD,0X4D,0XCD,0X8C,0XB4,0XA8,0XB4,0XEA,0XA4,0XCA,0XAD,0X4D,
0XA5,0X2C,0XB5,0X4D,0XC5,0XAD,0XBD,0XF0,0XA5,0XF4,0X75,0XD8,0X45,0XFD,0X2E,0X1F,
0X36,0X3F,0X36,0X1F,0X3E,0X1F,0X3E,0X1F,0X3E,0X3F,0X3E,0X3F,0X36,0X3F,0X36,0X3F,
0X36,0X1F,0X36,0X1F,0X3E,0X1F,0X3E,0X1E,0X3E,0X1F,0X36,0X1F,0X2E,0X3F,0X2E,0X5F,
0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X7F,0X2E,0X5F,0X26,0X5F,0X26,0X5F,0X2E,0X3F,0X2E,0X3F,0X36,0X3E,0X36,0X3E,
0X2E,0X5E,0X26,0X7F,0X26,0XBF,0X26,0X5F,0X36,0X3E,0X4D,0XFB,0X86,0X7B,0XAE,0XB9,
0XBE,0X55,0XD7,0X18,0XAE,0XBA,0X55,0X37,0X35,0XBC,0X1D,0XFE,0X16,0X3F,0X16,0X1F,
0X2E,0X1F,0X35,0XFE,0X36,0X1F,0X2E,0X3F,0X1E,0X5F,0X1E,0X5F,0X26,0X3F,0X2E,0X1F,
0X2E,0X1F,0X26,0X1F,0X26,0X1F,0X26,0X1F,0X25,0XFF,0X25,0XFF,0X25,0XFF,0X25,0XFF,
0X25,0XDF,0X25,0XFF,0X25,0XFF,0X25,0XFF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XBF,
0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,
0X25,0X7F,0X25,0X7F,0X25,0X7F,0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X3F,
0X2D,0X3F,0X2D,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,
0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,
0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XFF,
0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,0X24,0XFF,
0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X1D,0X1F,0X25,0X1F,
0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,
0X25,0X3F,0X25,0X3F,0X2D,0X3F,0X2D,0X1F,0X2D,0X1F,0X2C,0XFF,0X2C,0XDF,0X2C,0XDF,
0X3D,0X5F,0X35,0X3E,0X35,0X3D,0X35,0X5E,0X35,0X5F,0X2D,0X5F,0X25,0X5F,0X25,0X7E,
0X25,0XDE,0X2D,0XDE,0X2D,0X9F,0X35,0X7F,0X3D,0X3F,0X45,0X1D,0X4D,0X5B,0X6D,0XB8,
0X8D,0X93,0X8C,0X8C,0X94,0X07,0XBC,0XEA,0XA4,0X05,0XB4,0XC9,0X8C,0X27,0X8C,0X49,
0X6B,0X04,0X73,0X65,0X93,0XE6,0X94,0X8A,0X84,0XEF,0X6D,0X55,0X4D,0XDB,0X36,0X3F,
0X36,0X5F,0X2E,0X1F,0X36,0X1E,0X3E,0X3E,0X46,0X3E,0X3E,0X1E,0X36,0X3F,0X36,0X5F,
0X2E,0X5F,0X2E,0X5F,0X36,0X3F,0X3E,0X3F,0X3E,0X3E,0X36,0X3F,0X36,0X3F,0X2E,0X3F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X26,0X5F,0X26,0X5F,0X2E,0X5F,0X2E,0X5F,0X36,0X3E,0X36,0X5E,
0X2E,0X7F,0X26,0X7F,0X2E,0XBF,0X1E,0X1E,0X46,0X5D,0X5E,0X3A,0X96,0XDA,0XB6,0XD8,
0XC6,0X95,0XCE,0XD7,0XAE,0XB8,0X65,0X76,0X3D,0XDB,0X1D,0XBD,0X15,0XFF,0X0D,0XDF,
0X25,0XDF,0X2D,0XDE,0X2D,0XDE,0X2D,0XFF,0X1E,0X1F,0X1E,0X1F,0X25,0XFF,0X25,0XFE,
0X25,0XFF,0X25,0XFF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X1D,0XBF,
0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0X9F,
0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X7F,
0X25,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2C,0XFF,
0X2C,0XFF,0X2C,0XFF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,
0X2C,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,
0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XDF,0X24,0XDF,0X24,0XDF,0X24,0XFF,
0X24,0XFF,0X24,0XFF,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,
0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,0X25,0X1F,
0X25,0X1F,0X25,0X1F,0X2D,0X1F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,
0X2D,0X3F,0X2D,0X1F,0X2D,0X1F,0X2C,0XFF,0X34,0XFF,0X34,0XDF,0X34,0XDF,0X34,0XBF,
0X45,0X9F,0X45,0X9E,0X3D,0X9E,0X3D,0XBF,0X35,0XBF,0X2D,0XBF,0X2E,0X1F,0X36,0X5F,
0X26,0X1D,0X25,0XFD,0X2D,0XFF,0X3D,0XDF,0X45,0XBF,0X55,0XDE,0X76,0X3C,0X8E,0X79,
0XCF,0X3A,0XCE,0X34,0XCD,0X8F,0XDD,0XCF,0XBC,0XCA,0XDD,0XEF,0XBD,0X6E,0XC5,0XB0,
0X8B,0XE9,0XA4,0X6A,0XB4,0XCB,0XA4,0XCC,0X8C,0XF0,0X75,0X55,0X5D,0XDB,0X46,0X1F,
0X2E,0X3F,0X26,0X3F,0X36,0X1E,0X3E,0X1D,0X3E,0X1D,0X3E,0X1D,0X36,0X1E,0X2E,0X5F,
0X26,0X5F,0X26,0X5F,0X2E,0X5F,0X2E,0X3F,0X36,0X3E,0X36,0X1E,0X36,0X1E,0X2E,0X1F,
0X2E,0X3F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,0X2E,0X3F,
0X2E,0X5F,0X2E,0X5F,0X2E,0X5F,0X2E,0X3F,0X2E,0X3F,0X2E,0X5E,0X2E,0X3E,0X2E,0X3E,
0X26,0X3F,0X26,0X3F,0X2E,0X5F,0X25,0XDD,0X56,0X9D,0X86,0XFC,0XC7,0XDC,0XDF,0XBA,
0XDF,0X38,0XD6,0XF7,0XBE,0XF7,0X86,0X37,0X5E,0X5B,0X25,0XBC,0X15,0XDF,0X0D,0XDF,
0X1D,0XDF,0X25,0XBE,0X2D,0XBE,0X2D,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,
0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,0X25,0XBF,
0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X7F,0X25,0X7F,0X25,0X7E,
0X25,0X7F,0X25,0X7F,0X25,0X7F,0X25,0X7F,0X25,0X5F,0X25,0X5F,0X2D,0X3F,0X2D,0X3F,
0X2D,0X1F,0X2D,0X1F,0X34,0XFF,0X34,0XFF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,
0X34,0XBF,0X34,0XBF,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X2C,0XBF,
0X2C,0X9F,0X2C,0X9F,0X2C,0X9F,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X24,0X9E,
0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XDF,0X24,0XDF,0X24,0XFF,0X24,0XFF,
0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,
0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,
0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2C,0XFF,0X2C,0XFF,
0X2C,0XFF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XBE,0X34,0XBF,0X34,0XBE,
0X4D,0XBF,0X45,0XDE,0X45,0XFF,0X3E,0X1F,0X36,0X1F,0X2E,0X1F,0X3E,0X9F,0X4F,0X1F,
0X3E,0X7D,0X36,0X3C,0X3E,0X3E,0X46,0X3F,0X4E,0X1F,0X6E,0X7F,0X9E,0XFD,0XB6,0XFA,
0XD6,0XF8,0XCE,0X33,0XCD,0X8E,0XC4,0XEB,0XA3,0XE6,0XC4,0XEB,0XA4,0X8A,0XB5,0X0D,
0XAC,0XCC,0XBD,0X4D,0XCD,0X6E,0XBD,0X2E,0XA5,0X50,0X8D,0XB5,0X75,0XFA,0X56,0X1D,
0X36,0X3F,0X36,0X5F,0X3E,0X5E,0X3E,0X3D,0X3E,0X1C,0X3E,0X3D,0X36,0X3E,0X2E,0X3F,
0X26,0X5F,0X26,0X5F,0X2E,0X5F,0X2E,0X3F,0X2E,0X3E,0X36,0X1D,0X36,0X1D,0X2E,0X1E,
0X2E,0X1F,0X26,0X3F,0X2E,0X1F,0X26,0X3F,0X26,0X1F,0X26,0X1F,0X26,0X1F,0X26,0X1F,
0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,
0X2E,0X3F,0X2E,0X3F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,
0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,
0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X2E,0X1F,0X26,0X1E,0X26,0X1E,0X2E,0X1E,0X26,0X1E,
0X25,0XFF,0X2E,0X1F,0X2D,0XFF,0X35,0XBD,0X6E,0X7C,0XA7,0X7D,0XD7,0XFB,0XDF,0X98,
0XCE,0XB5,0XBE,0X33,0XB6,0X53,0X96,0X14,0X6E,0X39,0X2D,0X7A,0X15,0X9F,0X0D,0XBF,
0X15,0XBF,0X1D,0XBF,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X25,0XBF,0X25,0XBF,0X25,0XBF,
0X25,0X9F,0X25,0X9F,0X25,0X9F,0X25,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,
0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X3F,0X25,0X3F,
0X25,0X5F,0X25,0X5F,0X25,0X5F,0X25,0X3F,0X25,0X1E,0X25,0X1E,0X2C,0XFE,0X2C,0XFE,
0X34,0XFF,0X34,0XFF,0X34,0XDF,0X34,0XDE,0X34,0XBE,0X34,0XBE,0X34,0X9E,0X34,0X9E,
0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,
0X34,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,
0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0XBF,0X2C,0XDF,0X2C,0XDF,0X2C,0XFF,0X2C,0XFF,
0X2D,0X1F,0X2D,0X1F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,
0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,
0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XDF,0X2C,0XDE,
0X2C,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0X9E,0X34,0X9E,0X3C,0X9E,0X3C,0X9E,
0X4D,0XDF,0X4E,0X1F,0X46,0X7F,0X3E,0X9F,0X36,0X7F,0X2E,0X3F,0X4E,0X7F,0X6F,0X1F,
0X56,0X3B,0X45,0XF9,0X3E,0X1C,0X3E,0X1C,0X45,0XDB,0X6E,0X3B,0XAE,0X99,0XC6,0X96,
0XB5,0XB0,0XAD,0X2C,0XBD,0X0A,0XAC,0X26,0XA3,0XE4,0XB4,0X67,0X93,0XC5,0XA4,0X89,
0X94,0X07,0X9C,0X27,0XA4,0X68,0XAC,0XAA,0XA5,0X2E,0X95,0X72,0X7D,0XB6,0X5D,0XD9,
0X45,0XFB,0X46,0X7F,0X46,0X9F,0X36,0X3E,0X2D,0XFD,0X36,0X3E,0X36,0X3E,0X2E,0X1E,
0X2E,0X3E,0X2E,0X3F,0X2E,0X3F,0X2E,0X1F,0X2E,0X1E,0X2E,0X1E,0X35,0XFD,0X35,0XFD,
0X2D,0XFF,0X25,0XFF,0X2D,0XFF,0X25,0XFF,0X2D,0XFF,0X25,0XFF,0X25,0XDF,0X25,0XFF,
0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,0X25,0XDF,
0X2D,0XFF,0X2D,0XFF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,
0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XBF,
0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBE,0X25,0XBD,0X25,0XDD,0X25,0XBE,0X25,0XBE,
0X25,0XBF,0X35,0XFF,0X35,0X7F,0X45,0X5C,0X6D,0XD9,0XAE,0XFA,0XBE,0XB5,0XB5,0XF0,
0XB5,0XB0,0XA5,0X4F,0XAD,0X8F,0X85,0X4F,0X65,0X75,0X34,0XF8,0X25,0X5E,0X0D,0X5F,
0X15,0X7F,0X1D,0X7F,0X2D,0X5E,0X2D,0X5F,0X2D,0X3F,0X2D,0X5F,0X25,0X5F,0X25,0X5F,
0X25,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X2D,0X3F,0X25,0X1F,0X25,0X1F,
0X25,0X1E,0X25,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X2D,0X1F,0X25,0X1E,0X25,0X1E,
0X2D,0X1F,0X25,0X1E,0X2D,0X1E,0X2C,0XFE,0X2C,0XFE,0X2C,0XDE,0X2C,0XDE,0X34,0XBE,
0X34,0XFF,0X3C,0XDE,0X3C,0XBE,0X3C,0XBE,0X3C,0X9E,0X34,0X7E,0X34,0X7E,0X34,0X7E,
0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X7E,
0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X9E,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,
0X2C,0XBF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,0X2C,0XDF,0X2C,0XFF,0X34,0XFF,0X2C,0XFF,
0X35,0X1F,0X35,0X1F,0X35,0X3F,0X35,0X3F,0X35,0X3F,0X2D,0X3F,0X2D,0X1F,0X2D,0X1F,
0X2D,0X1F,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X34,0XFF,0X34,0XFF,
0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X34,0XDF,0X2C,0XDF,0X2C,0XDE,0X2C,0XBE,
0X34,0XBE,0X34,0X9E,0X34,0X9E,0X34,0X9E,0X34,0X9E,0X34,0X7D,0X34,0X7E,0X34,0X7D,
0X4E,0X1F,0X4E,0X5F,0X4E,0XBF,0X4E,0XDF,0X3E,0X7E,0X35,0XFC,0X5E,0X3C,0X7E,0XBD,
0X7E,0X5A,0X6D,0XF8,0X5E,0X3A,0X56,0X3A,0X55,0XF8,0X86,0X58,0XCE,0XB6,0XDE,0X93,
0XCD,0XF0,0XBD,0X6C,0XD5,0XAC,0XBC,0XE8,0XCD,0X28,0XCD,0X4A,0XB4,0XA8,0XD5,0XCD,
0X93,0XE5,0X8B,0XA4,0X93,0XC4,0XA4,0X88,0XA5,0X0C,0X95,0X0E,0X74,0XF0,0X5D,0X13,
0X55,0X97,0X5E,0X7C,0X4E,0X9F,0X36,0X1E,0X25,0XDD,0X36,0X1E,0X3E,0X3E,0X35,0XDC,
0X3D,0XFC,0X35,0XFC,0X35,0XFE,0X2D,0XFE,0X2D,0XFE,0X2D,0XFE,0X2D,0XDD,0X35,0XDE,
0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XDF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,
0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,
0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0XBF,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,
0X35,0XBF,0X35,0XBF,0X35,0XBF,0X35,0XBF,0X35,0X9F,0X35,0X9F,0X2D,0X9F,0X35,0X7F,
0X35,0X5F,0X35,0X5F,0X35,0X5F,0X2D,0X7E,0X2D,0X7D,0X2D,0X7D,0X25,0X7E,0X25,0X7F,
0X1C,0XFE,0X35,0X5F,0X2C,0XDC,0X4C,0XFA,0X6D,0X16,0XAE,0X36,0XAD,0X8F,0X94,0X6A,
0XBD,0X90,0XB5,0X4F,0XB5,0X8E,0X8D,0X0D,0X6C,0XF1,0X44,0XB6,0X2D,0X1D,0X0C,0XDE,
0X1D,0X5F,0X1D,0X3F,0X2D,0X1E,0X34,0XFF,0X34,0XDF,0X2C,0XDF,0X24,0XFF,0X1C,0XFE,
0X2C,0XDE,0X2C,0XDE,0X2C,0XDE,0X2C,0XDE,0X2C,0XBE,0X24,0XBE,0X24,0XBE,0X24,0XBE,
0X24,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0XDE,0X2C,0XDE,0X2C,0XDE,0X2C,0XDE,0X2C,0XBE,
0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0X9D,0X2C,0X9D,0X2C,0X9D,0X34,0X9D,
0X3C,0XDE,0X3C,0XBE,0X3C,0XBE,0X3C,0X9E,0X34,0X9E,0X34,0X7E,0X34,0X5E,0X34,0X5E,
0X34,0X5E,0X34,0X5E,0X34,0X3E,0X34,0X3E,0X34,0X3E,0X34,0X5E,0X34,0X5E,0X34,0X5E,
0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0XBF,0X34,0XBF,
0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XFF,0X34,0XFF,0X34,0XFF,
0X34,0XFF,0X35,0X1F,0X35,0X1F,0X35,0X3F,0X35,0X3F,0X2D,0X1F,0X2D,0X1F,0X2C,0XFF,
0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,0X34,0XDF,0X34,0XDF,
0X2C,0XBE,0X34,0XDE,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XBE,
0X34,0XBE,0X34,0X9E,0X34,0X9E,0X34,0X9D,0X34,0X7D,0X34,0X7D,0X34,0X7D,0X34,0X7D,
0X3D,0XFE,0X3E,0X1E,0X4E,0X9E,0X5E,0XDF,0X3D,0XDA,0X45,0X99,0X76,0X3B,0X8E,0X39,
0XA6,0X79,0X95,0XF6,0X75,0XD7,0X86,0XBA,0X7E,0X77,0XB7,0X38,0XEF,0X35,0XF6,0XF2,
0XF7,0X33,0XCD,0XEE,0XDD,0XED,0XAC,0X25,0XCD,0X29,0XC5,0X09,0XC5,0X2A,0XD5,0XCD,
0XC5,0X6B,0XC5,0X4B,0XD5,0XAC,0XC5,0X8C,0XC5,0XEE,0X9D,0X2D,0X8D,0X2F,0X8D,0XB3,
0X75,0X74,0X9F,0X5E,0X4D,0XFB,0X3E,0X3F,0X2E,0X1F,0X2D,0XDE,0X45,0XFD,0X45,0X7A,
0X4D,0X79,0X66,0X5C,0X35,0X5A,0X35,0XDD,0X25,0XBE,0X2D,0XFF,0X35,0XFE,0X2D,0X9E,
0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X2D,0X9F,0X35,0X9F,0X35,0X9F,0X35,0X9F,0X35,0X9F,
0X35,0X9F,0X35,0X9F,0X35,0X7F,0X2D,0X7F,0X2D,0X7F,0X2D,0X7F,0X35,0X7F,0X35,0X7F,
0X35,0X7F,0X35,0X7F,0X35,0X5F,0X35,0X5F,0X35,0X5F,0X35,0X5F,0X35,0X5F,0X35,0X5F,
0X2D,0X3E,0X35,0X3F,0X35,0X3F,0X35,0X3F,0X35,0X3F,0X35,0X3F,0X35,0X3F,0X35,0X1F,
0X35,0X1F,0X34,0XDF,0X34,0XFF,0X35,0X1E,0X2C,0XFC,0X2C,0XFD,0X2D,0X3E,0X24,0XFE,
0X2D,0X1F,0X2C,0XBE,0X44,0XBC,0X6D,0X3A,0X9D,0XF9,0XC6,0X77,0XFF,0X97,0XDE,0X51,
0XD5,0XF1,0XDE,0X52,0XCE,0X50,0XCE,0X92,0XA5,0XF4,0X85,0XD9,0X44,0XFB,0X1C,0X9C,
0X1C,0XDD,0X1C,0XDD,0X24,0X9D,0X2C,0X7D,0X2C,0X5E,0X2C,0X7F,0X2C,0XBF,0X24,0XDE,
0X2C,0X9E,0X2C,0X9E,0X34,0X9E,0X34,0X9E,0X34,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,
0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X9E,0X2C,0X7E,
0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7D,0X34,0X7D,
0X4D,0X1F,0X3C,0X9E,0X34,0X9D,0X3C,0XBE,0X34,0X9E,0X2C,0X7D,0X2C,0X7E,0X34,0X7E,
0X34,0X3E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X7E,0X3C,0X7F,0X3C,0X9F,0X34,0X9F,
0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X34,0X9F,0X2C,0XBF,
0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XDE,0X34,0XDE,0X34,0XDF,0X34,0XDF,0X2C,0XDF,
0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X2C,0XFF,0X2C,0XFF,0X2C,0XFF,
0X2C,0XDE,0X2C,0XDE,0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,0X2C,0XBE,
0X2C,0XBE,0X2C,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,
0X3C,0XBE,0X4D,0X1F,0X2C,0X5C,0X3C,0XDE,0X3C,0XBE,0X34,0X9D,0X2C,0X5C,0X3C,0XDE,
0X46,0XBF,0X3E,0X5E,0X5E,0X7D,0X7F,0X1E,0X7E,0XBC,0X7E,0X39,0X75,0X55,0XAE,0X58,
0XCE,0XB9,0X9D,0X52,0XB6,0XB8,0XAE,0XD7,0XB7,0X38,0XCF,0X36,0XDE,0X90,0XD5,0XAC,
0XDE,0X0F,0XE6,0X70,0XC5,0X4B,0X93,0XA4,0XE5,0XED,0XBC,0XC9,0X9B,0XE6,0XCD,0X8C,
0XB4,0XA9,0XC5,0X2B,0XDE,0X0E,0XCD,0X8C,0XCD,0XEF,0XB5,0X4E,0X9D,0X0D,0X9D,0X70,
0X95,0X92,0XAE,0XFA,0X65,0XDA,0X2D,0X39,0X25,0X5C,0X2D,0X7C,0X45,0X59,0X55,0X57,
0X6D,0X97,0X86,0X7B,0X4D,0X79,0X45,0X9C,0X35,0XBE,0X25,0X9E,0X2D,0XBE,0X2D,0X9E,
0X35,0X7F,0X35,0X7F,0X35,0X7F,0X35,0X7F,0X35,0X7F,0X35,0X7F,0X35,0X7F,0X35,0X7F,
0X35,0X5F,0X35,0X3F,0X35,0X3F,0X35,0X3E,0X35,0X3E,0X35,0X1E,0X35,0X3F,0X35,0X3F,
0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X2C,0XFE,0X2C,0XFE,0X34,0XDE,0X34,0XDE,
0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XDE,0X34,0XDE,0X34,0XDF,
0X3C,0XBF,0X34,0X7F,0X3C,0X9E,0X3C,0XBD,0X34,0X9C,0X34,0XBD,0X2C,0XDE,0X2C,0XBE,
0X2C,0X7E,0X3C,0X9D,0X5C,0XDB,0X85,0X7A,0XD6,0XFC,0XE7,0X18,0XF7,0X35,0XE6,0X51,
0XD5,0XAF,0XCD,0XD0,0XBD,0X8D,0XBD,0XCF,0X9D,0X51,0X85,0X76,0X54,0XB8,0X2C,0X7A,
0X24,0XBC,0X24,0XBD,0X2C,0X9E,0X34,0X7E,0X3C,0X5F,0X3C,0X5F,0X2C,0X7E,0X24,0X5D,
0X2C,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X7E,0X34,0X5E,0X34,0X5E,
0X34,0X5E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X34,0X7E,
0X34,0X5E,0X3C,0X7E,0X3C,0X7E,0X3C,0X7E,0X3C,0X7E,0X3C,0X7D,0X3C,0X7D,0X3C,0X7D,
0X55,0X5F,0X44,0XDE,0X3C,0XBE,0X3C,0XDE,0X34,0X9E,0X2C,0X9D,0X2C,0XBE,0X34,0XBE,
0X34,0X7E,0X3C,0X7E,0X3C,0X7E,0X3C,0X9F,0X3C,0X9F,0X3C,0XBF,0X3C,0XBF,0X3C,0XBF,
0X34,0XBF,0X34,0XBF,0X34,0XBF,0X34,0XBF,0X34,0XBF,0X34,0XBF,0X34,0XBF,0X34,0XBF,
0X34,0XBE,0X34,0XBE,0X34,0XDF,0X34,0XDE,0X34,0XDF,0X34,0XDE,0X34,0XDF,0X34,0XFF,
0X34,0XDF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,0X34,0XFF,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XBE,0X34,0XDE,0X34,0XBE,0X34,0XBE,
0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,
0X3C,0XDE,0X55,0X5F,0X34,0X9D,0X3C,0XDE,0X3C,0XFE,0X3C,0XFE,0X34,0XBD,0X3C,0XFE,
0X26,0X5F,0X35,0XFD,0X5D,0X99,0X7D,0X97,0X85,0X76,0X95,0XD6,0XA6,0X37,0XB6,0X57,
0XE7,0X39,0X9C,0X8E,0X8C,0X4D,0XA5,0X50,0XB6,0X12,0XA5,0X6D,0XDE,0X4E,0XF6,0XAF,
0XE6,0X30,0XDE,0X31,0XC5,0X2C,0XBC,0XAA,0XEE,0X30,0XC4,0XEC,0XC4,0XEC,0XD5,0XAF,
0XC5,0X2D,0XE6,0X31,0XCD,0X8E,0X9C,0X09,0X8B,0XC8,0XA4,0XAC,0XE6,0XF5,0XBD,0XD1,
0XE6,0XD5,0X9D,0X51,0X75,0X96,0X5D,0XBA,0X4D,0XDC,0X45,0X7B,0X5D,0X58,0X8E,0X3A,
0X9E,0X38,0X8D,0XD7,0X65,0X57,0X4D,0X39,0X3D,0X9D,0X2D,0X5E,0X2D,0X9F,0X2D,0X9F,
0X35,0X5F,0X35,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X3F,0X35,0X3F,0X3D,0X3F,0X35,0X3F,
0X35,0X1E,0X35,0X1E,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,0X34,0XFE,
0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0X9E,
0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0X7D,0X3C,0X7D,0X3C,0X7E,
0X34,0X5F,0X34,0X1E,0X3C,0X3D,0X3C,0X3C,0X34,0X1B,0X34,0X5C,0X34,0X7E,0X2C,0X5E,
0X34,0X7D,0X44,0X5B,0X5C,0X58,0X74,0X54,0XBD,0XF6,0XC5,0XB2,0XBC,0XEC,0XBC,0XEB,
0XD5,0XCF,0XCD,0X8E,0XB5,0X4C,0XB5,0X6E,0X94,0XCE,0X8C,0XF2,0X5C,0X54,0X33,0XD5,
0X2C,0X5A,0X24,0X5C,0X2C,0X3D,0X34,0X3D,0X3C,0X3E,0X3C,0X5E,0X34,0X5E,0X2C,0X5D,
0X34,0X3D,0X34,0X3D,0X34,0X5D,0X34,0X5D,0X34,0X5D,0X34,0X5D,0X34,0X5D,0X34,0X5D,
0X34,0X5E,0X34,0X5E,0X34,0X5E,0X34,0X7E,0X34,0X7E,0X34,0X7E,0X3C,0X7E,0X3C,0X7E,
0X3C,0X7F,0X3C,0X7E,0X44,0X7E,0X44,0X7E,0X44,0X9E,0X44,0X7D,0X44,0X9D,0X44,0X9D,
0X5D,0X7F,0X44,0XDE,0X3C,0XBE,0X3C,0XFE,0X34,0XBE,0X2C,0XBE,0X2C,0XDE,0X34,0XDE,
0X34,0X9E,0X34,0X9E,0X34,0X9E,0X34,0X9E,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,
0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDF,0X34,0XDE,
0X34,0XDE,0X34,0XDE,0X34,0XDF,0X34,0XDE,0X34,0XDF,0X34,0XDE,0X34,0XDF,0X34,0XFF,
0X34,0XDE,0X34,0XDE,0X34,0XDF,0X3C,0XFF,0X3C,0XFF,0X3C,0XFF,0X3C,0XFF,0X3C,0XFF,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,
0X34,0XBE,0X34,0XBE,0X34,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XDE,0X3C,0XBE,
0X3C,0XBD,0X55,0X9F,0X34,0XBD,0X3C,0XDE,0X3C,0XDE,0X3D,0X1E,0X34,0XDE,0X3C,0XFE,
0X26,0X7F,0X4E,0X5E,0X9E,0XBC,0XAD,0XF7,0XBD,0XF6,0X8C,0X8F,0XAE,0X14,0X74,0X0C,
0XBD,0XB2,0XE6,0X75,0X72,0XC6,0X62,0X64,0X62,0XC4,0X8C,0X07,0XDE,0X4E,0XC5,0X4A,
0XD5,0XAE,0XBD,0X0C,0XAC,0X49,0XB4,0XCB,0XC5,0X0C,0XA3,0XE8,0XCD,0X2D,0XC5,0X0D,
0XAC,0X6A,0XB4,0XAB,0XBC,0XCC,0XBC,0XED,0XC5,0X4E,0X9C,0X4A,0XB4,0XEC,0XDE,0X31,
0XEE,0X72,0X94,0X6B,0XB6,0X77,0X85,0XF8,0X5D,0X37,0X86,0X5C,0XA6,0XBC,0X9E,0X18,
0XC6,0XD9,0X7C,0X91,0X7D,0X56,0X5D,0X17,0X45,0X3A,0X3D,0X9E,0X35,0X9F,0X2D,0X7F,
0X35,0X5F,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X3D,0X1F,0X3D,0X1E,0X34,0XFE,0X3C,0XFE,
0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,
0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,
0X3C,0X7D,0X3C,0X7D,0X3C,0X7D,0X3C,0X7D,0X3C,0X5D,0X3C,0X5D,0X3C,0X3D,0X3C,0X3D,
0X3C,0X3E,0X3B,0XFE,0X44,0X1D,0X44,0X1C,0X3B,0XFB,0X3C,0X1C,0X34,0X5E,0X34,0X3D,
0X3C,0X3C,0X3B,0XD7,0X74,0XB7,0X95,0X15,0XDE,0XB7,0XEE,0XD5,0XE6,0X51,0XFE,0XF3,
0XF6,0X91,0XE6,0X71,0XD6,0X51,0XD6,0X93,0XBD,0XD1,0XB5,0XD4,0X8D,0X35,0X64,0X96,
0X3C,0X39,0X2C,0X3C,0X2C,0X1D,0X2C,0X1D,0X3C,0X1D,0X3C,0X3D,0X3C,0X5D,0X3C,0X7E,
0X34,0X3D,0X3C,0X3D,0X3C,0X3D,0X3C,0X5D,0X3C,0X5E,0X3C,0X5E,0X3C,0X5E,0X3C,0X7E,
0X3C,0X7E,0X3C,0X7E,0X3C,0X7E,0X3C,0X7E,0X3C,0X7E,0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,
0X3C,0X7F,0X3C,0X9F,0X44,0X9F,0X44,0X9E,0X44,0X9E,0X44,0X9E,0X44,0X9D,0X44,0X9D,
0X5D,0X5F,0X44,0XDE,0X3C,0XBD,0X3C,0XFE,0X34,0XDE,0X2C,0XBD,0X2C,0XDE,0X2C,0XBE,
0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,
0X34,0XDF,0X34,0XDE,0X34,0XDF,0X34,0XDE,0X34,0XDF,0X34,0XFE,0X34,0XFF,0X34,0XFE,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XFF,0X3C,0XDF,
0X34,0XBE,0X34,0XBE,0X34,0XDE,0X3C,0XDF,0X3C,0XDF,0X3C,0XDF,0X3C,0XDF,0X3C,0XDF,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,
0X34,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XDE,
0X3C,0XBD,0X5D,0XBF,0X3C,0XDE,0X3C,0XDD,0X3C,0XDD,0X45,0X1E,0X3C,0XFE,0X34,0XFD,
0X2E,0X1E,0X35,0X39,0XAE,0X7A,0XA4,0XB0,0XFF,0X18,0X9C,0X8D,0X8C,0XEE,0X6B,0XEB,
0X8C,0X2B,0XEE,0X74,0X93,0X67,0X83,0X25,0X8B,0XC7,0X9C,0X89,0XCD,0XCE,0XC5,0X8C,
0XC5,0X6C,0XC5,0X4C,0XA4,0X47,0XB4,0XCA,0XC5,0X0B,0X93,0XA5,0XBC,0XEB,0XCD,0X4C,
0X8B,0X85,0XAC,0XAA,0XBD,0X0C,0X5A,0X20,0X83,0X44,0XA4,0X48,0X9B,0XE6,0XF6,0XB1,
0XCD,0X0B,0X7B,0X04,0XB5,0X4F,0XB5,0XD3,0X95,0X53,0X84,0XF2,0X9D,0X74,0XCE,0XFA,
0XB5,0XF5,0X52,0XC9,0X9D,0X75,0X85,0X97,0X54,0XD7,0X4D,0X7D,0X35,0X7F,0X2D,0X5F,
0X35,0X5F,0X3D,0X3F,0X3D,0X3F,0X3D,0X1E,0X3D,0X1E,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,
0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,
0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0X7D,0X3C,0X7D,0X3C,0X7D,0X3C,0X7D,0X44,0X7D,
0X44,0X7D,0X44,0X7D,0X44,0X5D,0X44,0X5D,0X44,0X5D,0X44,0X3D,0X44,0X3D,0X3C,0X3D,
0X44,0X5E,0X3C,0X3D,0X4C,0X3D,0X4C,0X3C,0X43,0XFC,0X44,0X1D,0X3C,0X5E,0X3C,0X3C,
0X44,0X5A,0X4B,0XF6,0XA6,0X19,0XDE,0XF9,0XFF,0X78,0XFF,0X55,0XF6,0XF3,0XF6,0XD2,
0XDE,0X2E,0XD5,0XEE,0XCE,0X11,0XCE,0X73,0XAD,0X6F,0XB5,0XB1,0XA5,0X73,0X84,0XF5,
0X4C,0X58,0X44,0X7C,0X3C,0X7F,0X34,0X7E,0X3C,0X3C,0X3C,0X3C,0X3C,0X5D,0X3C,0X7E,
0X3C,0X5E,0X3C,0X7E,0X3C,0X7E,0X44,0X9E,0X44,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,
0X3C,0X9E,0X3C,0X9E,0X3C,0X9E,0X3C,0XBE,0X3C,0X9E,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,
0X3C,0XBF,0X3C,0XBF,0X3C,0XBF,0X44,0XBF,0X44,0XBE,0X44,0XBE,0X44,0XBD,0X44,0XBD,
0X55,0X5F,0X44,0XBD,0X3C,0XBD,0X3C,0XFE,0X3C,0XDE,0X34,0XDE,0X34,0XDE,0X2C,0XBE,
0X3C,0XFF,0X3C,0XFE,0X3C,0XDF,0X3C,0XFE,0X3C,0XFF,0X3C,0XDE,0X3C,0XDF,0X34,0XFE,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XFE,0X34,0XDE,0X34,0XDE,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,
0X34,0XBE,0X34,0XBE,0X3C,0XBE,0X3C,0XDE,0X3C,0XDF,0X3C,0XDF,0X3C,0XDF,0X3C,0XDF,
0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,0X34,0XBE,
0X3C,0XBE,0X3C,0XBE,0X3C,0XDE,0X3C,0XDE,0X3C,0XBE,0X3C,0XDE,0X3C,0XDE,0X3C,0XBE,
0X44,0XDD,0X65,0XDF,0X4D,0X1E,0X44,0XFD,0X3C,0XDD,0X45,0X3E,0X3D,0X1E,0X3C,0XFD,
0X4E,0X9F,0X66,0X1D,0XBE,0X18,0X51,0X82,0XE5,0XD1,0XC5,0X6E,0X52,0XC5,0X52,0XE6,
0X52,0X23,0XC5,0X2F,0XE6,0X11,0X7A,0XC3,0X8B,0XE7,0XCE,0X30,0XDE,0X51,0XC5,0X4D,
0XBD,0X2C,0XB4,0XC9,0XA4,0X68,0XC5,0X6C,0XBD,0X0B,0XA4,0X28,0XCD,0X6D,0XCD,0X4D,
0XB4,0XCA,0XBD,0X0C,0XBD,0X4D,0XAC,0XAA,0X93,0XE7,0X9B,0XE6,0XE5,0XED,0XEE,0X0D,
0X69,0XE0,0X9B,0X65,0XBC,0X8A,0X93,0XA7,0X94,0X0A,0XA4,0XAD,0XA5,0X10,0XCE,0X76,
0X63,0X09,0X19,0X01,0X84,0X4F,0XA5,0XD7,0X64,0XD6,0X4D,0X1A,0X35,0X5F,0X35,0X9F,
0X35,0X5F,0X3D,0X3F,0X3D,0X3F,0X3D,0X1F,0X3D,0X1E,0X3D,0X1E,0X3C,0XFE,0X3C,0XFE,
0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XBE,0X3C,0XBD,
0X3C,0X9D,0X3C,0X7E,0X3C,0X7E,0X44,0X7E,0X44,0X7D,0X44,0X7D,0X44,0X7D,0X44,0X5D,
0X44,0X5D,0X44,0X5D,0X44,0X5D,0X4C,0X5D,0X4C,0X5D,0X44,0X3D,0X44,0X3D,0X44,0X3D,
0X3C,0X5D,0X3C,0X1C,0X4C,0X1B,0X4C,0X1B,0X4B,0XDB,0X43,0XFC,0X44,0X3D,0X3C,0X1B,
0X44,0X38,0X4C,0X14,0XAE,0X17,0XD6,0XF6,0XE6,0XB3,0XD5,0XEE,0XCD,0XCE,0XBD,0X4B,
0XD5,0XEC,0XC5,0X8C,0XB5,0X90,0XB5,0XD1,0X94,0X8C,0XA5,0X0D,0XAD,0X2F,0X8C,0XD2,
0X53,0XF6,0X44,0X5B,0X3C,0X9F,0X3C,0X9F,0X44,0X9D,0X44,0X7C,0X44,0X5D,0X3C,0X7D,
0X44,0X9E,0X3C,0X9E,0X44,0X9E,0X44,0XBE,0X44,0XBE,0X44,0XBE,0X3C,0XBE,0X3C,0XBE,
0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XDE,0X3C,0XBE,0X3C,0XDE,0X3C,0XBE,0X3C,0XDE,
0X34,0XDF,0X34,0XDF,0X3C,0XDF,0X3C,0XDE,0X44,0XDE,0X44,0XDE,0X44,0XDD,0X44,0XDD,
0X5D,0X7F,0X44,0XDD,0X3C,0XDD,0X44,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X34,0XBE,
0X3C,0XFE,0X3C,0XFE,0X3C,0XDE,0X3C,0XFE,0X3C,0XDE,0X3C,0XFE,0X3C,0XDE,0X3C,0XFE,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,
0X34,0XFE,0X34,0XFF,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X3C,0XDE,
0X34,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBF,0X44,0XBF,0X44,0XBF,0X3C,0XDE,
0X3C,0XBE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X3C,0XBE,0X3C,0XBE,
0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X44,0XDE,
0X44,0XBD,0X65,0XDF,0X4C,0XFE,0X44,0XFE,0X3C,0XDD,0X45,0X1E,0X3C,0XFD,0X3C,0XFD,
0X3D,0X1C,0X8E,0X7F,0XA4,0XF2,0X48,0XE0,0XAC,0X07,0XFF,0X74,0X84,0X29,0X52,0XC5,
0X6A,0XE6,0X7B,0X26,0XD5,0XF0,0X8B,0XE7,0X5B,0X03,0X73,0X87,0X6A,0XC6,0X7B,0X07,
0X7B,0X05,0X62,0X62,0X62,0X42,0X83,0X46,0X72,0XE5,0X6A,0X83,0X8B,0X67,0X83,0X26,
0X7A,0XE5,0X8B,0X68,0X83,0X47,0X83,0X67,0X72,0XE4,0XAC,0X49,0XFE,0XF2,0X7A,0X60,
0X72,0X60,0X9B,0X64,0XA3,0X22,0XBB,0XA3,0XBC,0X06,0XB4,0X28,0XAC,0XCD,0XA5,0X10,
0X18,0XE0,0X10,0XA0,0X4A,0X46,0X9D,0X53,0X8D,0X77,0X54,0XD8,0X3D,0X5E,0X35,0XBF,
0X35,0X5F,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X3D,0X1F,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,
0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XBE,0X3C,0XBD,
0X44,0XBE,0X44,0X9E,0X44,0X9E,0X44,0X9E,0X44,0X9E,0X44,0X7E,0X44,0X7E,0X44,0X7E,
0X44,0X5D,0X44,0X5D,0X44,0X5D,0X44,0X5D,0X4C,0X5D,0X4C,0X3D,0X4C,0X3D,0X44,0X3D,
0X3C,0X5C,0X3C,0X1B,0X4C,0X1B,0X54,0X1B,0X4B,0XFB,0X4C,0X1C,0X4C,0X5D,0X44,0X5B,
0X44,0X37,0X64,0XD5,0XA5,0XF4,0XD6,0XB3,0XEF,0X33,0XE6,0XB0,0XEF,0X33,0XEE,0XF1,
0XEE,0XF0,0XD6,0X6F,0XC6,0X53,0XC6,0X95,0XA5,0X2E,0XBD,0XCF,0XCE,0X11,0XAD,0X93,
0X64,0X76,0X4C,0X7C,0X34,0X7E,0X34,0XBF,0X44,0XDD,0X4C,0XDD,0X4C,0XBE,0X44,0X9E,
0X3C,0X9E,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBE,
0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,
0X34,0XFF,0X34,0XFF,0X34,0XFF,0X3C,0XFF,0X3C,0XFE,0X44,0XFD,0X44,0XDD,0X44,0XDD,
0X65,0XDF,0X4D,0X1E,0X44,0XDD,0X44,0XFD,0X44,0XDE,0X3C,0XDE,0X3C,0XFE,0X3C,0XDE,
0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,0X34,0XDE,
0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,
0X34,0XFF,0X35,0X1F,0X34,0XFE,0X3C,0XFE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,
0X3C,0XDE,0X3C,0XDE,0X44,0XDE,0X44,0XDF,0X44,0XDF,0X44,0XDF,0X44,0XBE,0X44,0XDE,
0X3C,0XDE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,0X3C,0XDE,
0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X44,0XFF,0X44,0XFF,0X44,0XFF,0X44,0XFE,
0X44,0XBD,0X65,0X9F,0X4C,0XDD,0X44,0XFD,0X44,0XDD,0X45,0X1E,0X3C,0XFD,0X45,0X3E,
0X6D,0X7D,0XB6,0XFF,0X6A,0XCA,0X59,0X60,0X49,0X40,0XE6,0X91,0XBD,0XD0,0X63,0X06,
0X73,0X05,0X7B,0X04,0XD5,0XEF,0XCD,0XEE,0X4A,0X41,0X42,0X02,0X30,0XC0,0X20,0X00,
0X51,0X80,0X62,0X22,0X28,0XA0,0X28,0XA0,0X51,0XE3,0X41,0X40,0X28,0X80,0X51,0XC2,
0X41,0X40,0X28,0X60,0X41,0X00,0X30,0X80,0X9B,0XA7,0XF6,0X10,0XB3,0XE7,0X71,0XA0,
0XBB,0XE6,0XC4,0X05,0XB3,0X21,0XDC,0X45,0XBB,0X83,0XD4,0XEA,0XD5,0X8F,0X52,0X03,
0X20,0XE0,0X39,0XC4,0X20,0XC0,0X94,0XD1,0XAE,0X3A,0X5C,0XD7,0X3D,0X3D,0X35,0X5F,
0X3D,0X5F,0X3D,0X5E,0X3D,0X3E,0X3D,0X3F,0X3D,0X3F,0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,
0X3C,0XDE,0X3C,0XDE,0X44,0XDE,0X44,0XDE,0X44,0XDE,0X44,0XDE,0X44,0XBE,0X44,0XBE,
0X44,0X9D,0X44,0X9D,0X44,0X9D,0X44,0X9D,0X44,0X9E,0X44,0X9E,0X44,0X7E,0X44,0X7D,
0X44,0X3D,0X44,0X3D,0X44,0X3D,0X44,0X3D,0X44,0X3D,0X4C,0X3D,0X44,0X3D,0X44,0X3C,
0X4C,0X9C,0X4C,0X5B,0X54,0X7B,0X5C,0X9C,0X54,0X7C,0X5C,0X9D,0X5C,0XDE,0X5C,0XDB,
0X8D,0XFC,0XBF,0X1B,0XD7,0X16,0XEF,0X53,0XFF,0XF5,0XE6,0XD0,0XE6,0XF2,0XE6,0XD2,
0XD6,0X4F,0XBD,0XAD,0XB5,0XB1,0XBE,0X33,0X9D,0X0E,0XBD,0XCF,0XCD,0XCF,0XAD,0X0F,
0X9D,0X98,0X6D,0X5C,0X44,0XDE,0X34,0XBE,0X44,0XDD,0X54,0XFD,0X4C,0XDD,0X44,0X9D,
0X3C,0XBD,0X3C,0XBD,0X3C,0XBD,0X44,0XBE,0X44,0XBE,0X3C,0XBE,0X3C,0XBE,0X3C,0XBD,
0X3C,0XFD,0X3C,0XFD,0X34,0XFE,0X34,0XDE,0X34,0XBE,0X34,0XBE,0X34,0XBD,0X34,0XBD,
0X34,0XFF,0X34,0XFF,0X34,0XFF,0X3C,0XFE,0X3C,0XFD,0X3C,0XFD,0X44,0XDE,0X44,0XDD,
0X76,0X1F,0X55,0X3C,0X44,0XDD,0X44,0XDE,0X3C,0XBE,0X3C,0XBD,0X44,0XDE,0X3C,0XDD,
0X3C,0XDE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,
0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,
0X3D,0X1F,0X3D,0X1E,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XDE,
0X3C,0XFE,0X44,0XFE,0X44,0XFF,0X44,0XFF,0X44,0XDF,0X44,0XDE,0X44,0XDE,0X44,0XDE,
0X3C,0XFE,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,
0X3D,0X1E,0X3D,0X1E,0X3D,0X1F,0X45,0X1F,0X45,0X1F,0X45,0X1F,0X45,0X1F,0X45,0X1E,
0X44,0XBD,0X65,0X9F,0X4C,0XFD,0X4D,0X1E,0X45,0X1E,0X4D,0X3E,0X45,0X1E,0X4D,0X7F,
0XA5,0XFB,0XBE,0X3B,0X51,0XE7,0X51,0X83,0X41,0X00,0X94,0X0B,0XFF,0X98,0X83,0X87,
0X82,0XE3,0X7A,0XA0,0XA3,0XE6,0XEE,0X92,0X7B,0X48,0X5A,0X25,0X61,0XC5,0X48,0XE0,
0X6A,0X42,0X62,0X21,0X30,0XE0,0X20,0X40,0X6A,0XA8,0X28,0XE1,0X28,0XC1,0X52,0X05,
0X49,0XA2,0X28,0X80,0X49,0X20,0X7A,0X21,0XFE,0X10,0XEC,0XEB,0X89,0X80,0XCB,0X64,
0XDB,0XA3,0XB2,0XA0,0XE4,0X65,0XCC,0X05,0XC4,0X47,0XF6,0X51,0X83,0X47,0X51,0XE3,
0X39,0X41,0X28,0XE1,0X39,0X84,0X42,0X29,0XBE,0XDD,0X6D,0X19,0X55,0X3C,0X45,0X3D,
0X3D,0X1C,0X3D,0X1C,0X3D,0X3D,0X3D,0X3E,0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,0X3C,0XFF,
0X44,0XFF,0X44,0XDF,0X4C,0XDF,0X4C,0XBF,0X4C,0XBF,0X4C,0X9F,0X44,0XBF,0X4C,0XBF,
0X4C,0X9D,0X4C,0X9C,0X44,0X9D,0X44,0X9D,0X44,0X7D,0X3C,0X7E,0X44,0X7E,0X44,0X7E,
0X44,0X5D,0X44,0X5D,0X4C,0X5D,0X4C,0X5D,0X4C,0X5D,0X4C,0X5D,0X4C,0X5D,0X4C,0X5D,
0X5C,0X7B,0X4B,0XF8,0X6D,0X1C,0X75,0X9E,0X44,0X39,0X54,0XBB,0X65,0X1B,0X8E,0X1D,
0X85,0X55,0XCF,0X18,0XCE,0X71,0XF7,0X52,0XD6,0X2C,0XCE,0X0C,0XBD,0X8D,0XCE,0X31,
0XC5,0XD0,0XC5,0XD1,0XD5,0XF2,0XD6,0X12,0X94,0XED,0X9C,0XEC,0XB5,0X0C,0XBD,0X2E,
0XA5,0X32,0X85,0X57,0X3C,0XDA,0X34,0XDC,0X65,0X5E,0X65,0X1D,0X4C,0X9B,0X44,0X7B,
0X3C,0XBB,0X4D,0X3D,0X3C,0X7B,0X4C,0XDE,0X3C,0X3D,0X3C,0X5E,0X44,0XBF,0X44,0XDD,
0X3D,0X1C,0X35,0X1C,0X35,0X1E,0X2C,0XFF,0X2C,0XDF,0X34,0XDF,0X34,0XDE,0X34,0XDD,
0X34,0XDE,0X3C,0XFF,0X3C,0XDE,0X3C,0XDD,0X3D,0X3D,0X45,0X1D,0X3C,0X9F,0X44,0X9D,
0X86,0X5D,0X6D,0XBA,0X44,0XDC,0X44,0XFF,0X34,0XDF,0X3D,0X3F,0X44,0XDD,0X4D,0X1D,
0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3D,0X1E,0X3D,0X1E,
0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,0X3C,0XFE,
0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,
0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X3E,
0X3D,0X3F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,
0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X45,0X3F,0X45,0X3E,
0X4D,0X1D,0X65,0XBF,0X5D,0XBF,0X45,0X1E,0X45,0X1E,0X45,0X5F,0X45,0X5F,0X45,0X3E,
0XC6,0X18,0X94,0X51,0X51,0XC5,0X49,0XA3,0X49,0X82,0X5A,0X24,0XDE,0X33,0XBC,0XEC,
0X6A,0X00,0X9B,0X63,0X93,0X43,0XC5,0X2C,0XE6,0X12,0X5A,0X03,0X61,0XA3,0X82,0XA6,
0XA4,0X09,0X8B,0X66,0X30,0XE0,0X41,0X41,0X72,0XE9,0X41,0X84,0X20,0X80,0X5A,0X67,
0X49,0XA2,0X41,0X20,0X93,0X05,0XF5,0X8E,0XEC,0XCB,0XA2,0X40,0XBA,0XC2,0XDB,0X84,
0XD3,0X20,0XE3,0XE3,0XC3,0X82,0XBB,0XE6,0XE5,0XAE,0XC5,0X2D,0X72,0XC5,0X6A,0XA5,
0X51,0XA2,0X41,0X21,0X31,0X02,0X31,0X86,0X84,0XB4,0X8D,0X9A,0X5C,0XD8,0X54,0XFA,
0X4D,0X1B,0X45,0X3C,0X3D,0X3D,0X3D,0X3E,0X3D,0X3F,0X3D,0X3F,0X45,0X3F,0X45,0X3F,
0X4D,0X1F,0X4C,0XFF,0X4C,0XDF,0X4C,0XDF,0X4C,0XDF,0X4C,0XDF,0X44,0XDF,0X4C,0XDF,
0X4C,0XDD,0X4C,0XDC,0X4C,0XBD,0X44,0XBD,0X44,0XBE,0X44,0XBE,0X44,0XBE,0X44,0X9E,
0X44,0X9D,0X4C,0X9D,0X4C,0X9D,0X4C,0X9D,0X4C,0X9D,0X4C,0X9D,0X4C,0X9E,0X4C,0X9D,
0X54,0X1A,0X74,0XFC,0X4C,0X17,0X86,0X1E,0X7D,0XFD,0X44,0X14,0X7D,0X58,0XBE,0XFD,
0XBE,0X57,0XEF,0X98,0XE6,0XB1,0XF7,0X10,0XF7,0X30,0XE6,0XF0,0XFF,0XF6,0XDE,0XB3,
0XDE,0XD5,0XDE,0X53,0XEE,0X32,0XE6,0X52,0XAD,0X8F,0XAD,0XAF,0XCD,0XCE,0XDD,0XEF,
0XB5,0X2E,0X8D,0X53,0X55,0X16,0X5D,0XBC,0X6D,0X3B,0X8D,0XDE,0X4C,0X38,0X54,0XD9,
0X5D,0X7B,0X5D,0X9D,0X44,0XBB,0X5D,0X1F,0X4C,0XBF,0X4C,0XBF,0X44,0XBF,0X3C,0XBC,
0X3D,0X1B,0X35,0X1B,0X35,0X1E,0X2D,0X1F,0X34,0XFF,0X34,0XFF,0X3D,0X1E,0X3D,0X1E,
0X3D,0X1F,0X45,0X1F,0X3C,0XFE,0X3C,0XFC,0X3D,0X3C,0X3D,0X1D,0X44,0X9F,0X4C,0XBD,
0X8E,0X5B,0X75,0XB7,0X44,0XBB,0X44,0XFF,0X34,0XFF,0X3D,0X5F,0X44,0XFC,0X4D,0X1D,
0X44,0XFD,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1F,0X3D,0X1F,
0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,0X3D,0X1F,0X3D,0X1E,
0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,0X3D,0X1E,
0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X5F,
0X3D,0X5E,0X3D,0X5E,0X3D,0X7F,0X3D,0X7F,0X3D,0X7F,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,
0X3D,0X5E,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5E,
0X4D,0X3D,0X5D,0XDF,0X5D,0XBF,0X45,0X1E,0X45,0X3E,0X45,0X7F,0X45,0X5F,0X3D,0X5F,
0XE6,0XD7,0X73,0X69,0X49,0XE2,0X51,0XE2,0X41,0X80,0X49,0X80,0XC5,0X0D,0XF6,0X92,
0X8B,0X44,0X83,0X02,0X8B,0X23,0XBC,0XCA,0XFE,0XD2,0X8B,0X45,0X7A,0XE3,0X8B,0X66,
0X9B,0XC8,0X83,0X26,0X38,0XE0,0X59,0XC2,0X8B,0X89,0X49,0XA2,0X30,0XE0,0X8B,0X6A,
0X7A,0XA6,0X8B,0X06,0XB3,0XE9,0XFE,0X52,0XC3,0XC6,0X99,0XE0,0XE4,0X06,0XC2,0XE1,
0XCB,0X62,0XBB,0X62,0XB3,0XA4,0XDD,0X4C,0XD5,0X8E,0X9C,0X29,0X83,0X87,0X62,0X43,
0X72,0X84,0X59,0XA2,0X38,0XE0,0X39,0X42,0X63,0X0B,0XBE,0X9B,0X7D,0X17,0X65,0X19,
0X55,0X3C,0X45,0X3E,0X45,0X3E,0X3D,0X3E,0X3D,0X5F,0X3D,0X5F,0X45,0X5F,0X45,0X5F,
0X4D,0X3E,0X4D,0X3E,0X4D,0X1D,0X4D,0X1D,0X45,0X1E,0X45,0X1E,0X3D,0X3F,0X45,0X1E,
0X4D,0X1D,0X4D,0X1D,0X44,0XFD,0X44,0XFD,0X44,0XFD,0X44,0XFE,0X44,0XFE,0X44,0XFD,
0X4C,0XFD,0X4C,0XFD,0X4C,0XFD,0X4C,0XDD,0X4C,0XDD,0X4C,0XDD,0X4C,0XDD,0X4C,0XDD,
0X54,0X5C,0X64,0X9B,0X6C,0XD9,0X8D,0XDA,0X96,0X18,0X95,0XD6,0XD7,0X5B,0XDF,0X3A,
0XD6,0X96,0XCE,0X12,0XFF,0X75,0XF7,0X12,0XE6,0XAF,0XEF,0X10,0XEF,0X11,0XDE,0XB1,
0XCE,0X71,0XC5,0XCF,0XD5,0XCE,0XD5,0XEF,0XB5,0XAE,0XAD,0X8D,0XCD,0XCD,0XD5,0XCD,
0XCE,0X0F,0XAD,0XF2,0X8E,0X36,0X8E,0X59,0XAE,0X7A,0X8D,0X56,0X6C,0XB5,0X5C,0XB5,
0X7E,0X1B,0X65,0XBB,0X44,0X9A,0X54,0XFE,0X54,0XFF,0X55,0X1F,0X45,0X1E,0X3C,0XFC,
0X3D,0X3C,0X3D,0X3C,0X35,0X3E,0X35,0X3F,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X45,0X3F,
0X3D,0X3F,0X45,0X3F,0X45,0X1E,0X3D,0X1C,0X45,0X5C,0X3D,0X3C,0X44,0XDF,0X54,0XFE,
0X9E,0XDC,0X7D,0XF7,0X4C,0XBB,0X4D,0X3F,0X3D,0X3F,0X45,0X7F,0X44,0XFC,0X4D,0X1D,
0X45,0X1E,0X3D,0X1E,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,
0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,
0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,
0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X45,0X5F,0X45,0X5F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X3D,0X5F,0X3D,0X7F,0X3D,0X7F,0X3D,0X7F,0X3D,0X7F,0X3D,0X7F,0X3D,0X7F,0X45,0X5F,
0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X5E,
0X4D,0X3C,0X5D,0XBE,0X5D,0X9F,0X45,0X1D,0X45,0X3E,0X4D,0X7F,0X45,0X7F,0X45,0X3F,
0XD6,0X12,0X6A,0XC5,0X39,0X80,0X41,0XA0,0X38,0XE0,0X83,0X04,0XC4,0XEB,0XFE,0XF3,
0XCD,0X2C,0X83,0X25,0X82,0XE4,0X93,0X85,0XCD,0X0B,0XE6,0X0E,0XA4,0X27,0XAC,0X69,
0X7A,0X63,0X7A,0X64,0X6A,0X02,0X7A,0X84,0XBC,0XAC,0X6A,0X02,0X72,0X43,0XCC,0XEE,
0XAB,0XCA,0XCC,0X8D,0XF5,0X90,0XC3,0X87,0X9A,0X00,0XD3,0XA5,0XC2,0XE1,0XE4,0X05,
0XC3,0XA4,0X9B,0X02,0XD5,0X2B,0XEE,0X51,0XA4,0XAB,0X83,0XC8,0X94,0X2A,0X6A,0XC4,
0X83,0X06,0X61,0XE2,0X49,0X20,0X41,0X21,0X5A,0X67,0XB5,0XD6,0X95,0X75,0X6C,0XF6,
0X55,0X3C,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X3D,0X5F,0X3D,0X5E,0X45,0X3E,0X45,0X3D,
0X4D,0X3C,0X4D,0X3C,0X4D,0X3C,0X45,0X3C,0X45,0X3C,0X3D,0X3D,0X3D,0X5E,0X3D,0X5E,
0X45,0X3D,0X4D,0X3D,0X45,0X1D,0X45,0X1D,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X1E,
0X4D,0X1E,0X4D,0X1D,0X4C,0XFD,0X4C,0XFD,0X4C,0XFD,0X4C,0XFD,0X4C,0XFE,0X4C,0XDE,
0X64,0XDF,0X54,0X3A,0X8D,0XBB,0XC7,0X5D,0XD7,0X7A,0XC6,0X53,0XC6,0X12,0X9C,0X6C,
0XFF,0XFA,0XAC,0X6B,0XB4,0XCB,0XC5,0X8C,0XAC,0XC8,0XFF,0X72,0XF7,0X52,0XDE,0XB0,
0XD6,0X91,0XC5,0XCE,0XDE,0X2E,0XE6,0X90,0XCE,0X50,0XBD,0XEE,0XCE,0X0D,0XCD,0XCC,
0XB5,0X2A,0XAD,0X4D,0X95,0X4F,0XB6,0X75,0XCE,0XB6,0X8C,0XCF,0X9D,0XF5,0X96,0X37,
0X96,0X79,0X75,0X99,0X54,0X9A,0X4C,0X9D,0X4C,0XDF,0X4D,0X1F,0X45,0X3E,0X3D,0X5D,
0X45,0X5D,0X3D,0X5D,0X3D,0X5E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3E,0X3D,0X3F,0X3D,0X3F,
0X3D,0X1F,0X45,0X3F,0X45,0X3E,0X45,0X3D,0X45,0X7C,0X45,0X5C,0X4C,0XFF,0X65,0X3D,
0XB7,0X7E,0X8E,0X58,0X54,0XFA,0X4D,0X5F,0X3D,0X5F,0X45,0X7F,0X44,0XFC,0X4D,0X3D,
0X45,0X3E,0X3D,0X3F,0X3D,0X3F,0X3D,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,
0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X45,0X3F,0X3D,0X5F,
0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,
0X3D,0X5E,0X3D,0X5F,0X45,0X5F,0X3D,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X9F,
0X3D,0X5F,0X3D,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X3D,0X5E,0X45,0X3E,0X45,0X5E,0X45,0X5E,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5E,
0X4D,0X3C,0X65,0XBE,0X5D,0X9E,0X45,0X1D,0X45,0X3E,0X4D,0X7F,0X45,0X5F,0X45,0X3F,
0X9B,0XA9,0X49,0XA1,0X41,0X81,0X39,0X60,0X41,0X40,0XB4,0X48,0XDD,0X4C,0XFE,0X91,
0XF6,0XB4,0X83,0X27,0X93,0XA8,0X9B,0X87,0X72,0X00,0XFE,0XD2,0XFF,0X13,0XAC,0X6A,
0X72,0X03,0X92,0XA7,0X9B,0X07,0XA3,0X26,0XDC,0XEC,0X92,0XC3,0XAB,0X46,0XED,0X2E,
0XDC,0X8C,0XFD,0XF2,0XC3,0X27,0X99,0XA0,0XBA,0X82,0XDB,0X64,0XDB,0X62,0XF4,0X87,
0XB3,0XC5,0XD5,0X6D,0XCD,0X8E,0XB5,0X4D,0X9C,0XAC,0X7B,0XC9,0X83,0XE9,0X94,0X2A,
0X72,0XC5,0X5A,0X02,0X5A,0X02,0X41,0X60,0X4A,0X04,0X63,0X4A,0X95,0X33,0X7D,0X16,
0X55,0X1C,0X45,0X3F,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5E,0X45,0X5D,0X45,0X3C,
0X4D,0X3B,0X4D,0X3A,0X4D,0X3A,0X4D,0X3B,0X45,0X3B,0X45,0X3C,0X3D,0X5D,0X3D,0X3E,
0X45,0X3E,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X45,0X1E,0X44,0XFE,0X45,0X1E,0X4C,0XFE,
0X4C,0XFE,0X4C,0XFE,0X4C,0XFE,0X4C,0XFE,0X4C,0XDE,0X4C,0XDE,0X44,0XDE,0X4C,0XDE,
0X54,0X7D,0X5C,0X7A,0XCF,0X7F,0X84,0X90,0XB5,0XAF,0XFF,0X95,0X93,0XA5,0X8B,0X03,
0XBC,0X8A,0XFF,0X34,0X93,0XA6,0X8B,0X64,0XE6,0X8F,0XF7,0X11,0XD6,0X0D,0XD6,0X4F,
0XCE,0X2F,0XBD,0X8D,0XC5,0XCD,0XD6,0X4F,0XBD,0XCE,0XA5,0X2C,0XAD,0X2A,0XB5,0X29,
0XA4,0X87,0XA4,0X87,0X8B,0XC5,0XC5,0XCE,0X9C,0X68,0XA4,0XEB,0X9D,0X4F,0XCF,0X39,
0X8D,0XD4,0X7D,0X56,0X6D,0X1B,0X54,0XDD,0X4C,0XFF,0X3D,0X1F,0X3D,0X3D,0X3D,0X5D,
0X45,0X5E,0X45,0X5E,0X45,0X5D,0X45,0X3D,0X3D,0X3C,0X3D,0X3D,0X3D,0X3F,0X3D,0X3F,
0X3D,0X1F,0X45,0X5F,0X45,0X5E,0X45,0X5E,0X45,0X9C,0X45,0X3B,0X4C,0XFD,0X65,0X5C,
0XBF,0XDE,0X96,0X99,0X54,0XF9,0X4D,0X3E,0X3D,0X5E,0X3D,0X7F,0X45,0X1C,0X4D,0X3D,
0X45,0X3E,0X3D,0X3E,0X3D,0X3F,0X3D,0X3F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,
0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,0X3D,0X5F,
0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,0X3D,0X5E,
0X3D,0X5E,0X3D,0X7E,0X3D,0X7E,0X3D,0X7F,0X45,0X7F,0X3D,0X7F,0X45,0X7F,0X45,0X7F,
0X3D,0X5E,0X3D,0X5E,0X45,0X5E,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X3D,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X3D,
0X4D,0X3C,0X65,0XBD,0X5D,0X9E,0X44,0XFC,0X45,0X1E,0X4D,0X5F,0X45,0X5F,0X45,0X3E,
0X72,0X45,0X41,0X00,0X62,0X45,0X5A,0X43,0X83,0X24,0XC4,0XA8,0XED,0XCC,0XF6,0X2F,
0XE5,0XF1,0XE6,0X54,0XC5,0X30,0X8B,0X27,0X8A,0XC4,0XCD,0X0C,0XD5,0XCF,0X93,0XA8,
0X69,0XA3,0X8A,0X66,0X7A,0X03,0X8A,0XA4,0XA3,0X45,0X9A,0XC2,0XA2,0XE3,0XC3,0XE8,
0XE4,0X8B,0XE4,0X8C,0X70,0XC0,0XA2,0X22,0XD3,0XA6,0XBA,0XA1,0XE4,0X25,0XDC,0X66,
0XCC,0XC9,0XF6,0XD2,0X7B,0X45,0X7B,0XA7,0X9C,0XCC,0X8C,0X6A,0X94,0XAC,0X8C,0X4A,
0X72,0XE5,0X62,0X43,0X83,0X47,0X5A,0X23,0X4A,0X04,0X31,0X63,0X95,0X12,0XA6,0X5A,
0X5C,0XFB,0X4D,0X3E,0X4D,0X3E,0X45,0X5E,0X45,0X7E,0X45,0X7E,0X45,0X7D,0X4D,0X7C,
0X4D,0X5B,0X4D,0X3B,0X4D,0X3B,0X4D,0X3B,0X4D,0X3B,0X4D,0X3C,0X45,0X3D,0X45,0X3E,
0X45,0X1F,0X45,0X1F,0X45,0X1F,0X45,0X1F,0X45,0X1E,0X4D,0X1E,0X4D,0X1E,0X4D,0X1E,
0X4C,0XFE,0X4C,0XFE,0X4C,0XFE,0X4C,0XFE,0X4C,0XFE,0X4C,0XFF,0X4C,0XFF,0X54,0XDE,
0X54,0X3A,0X95,0XDD,0XCF,0X3D,0X29,0X62,0X41,0XA0,0XFE,0XF1,0XDD,0X6B,0X9B,0X43,
0X9B,0X23,0XFE,0X50,0XE5,0XED,0X83,0X42,0XB4,0XC8,0XDE,0X2E,0XFF,0X94,0XEE,0XB2,
0XE6,0X92,0XC5,0XCF,0XC5,0XCE,0XE6,0XD2,0XC5,0XF0,0XC5,0XCF,0XBD,0XAE,0XC5,0XED,
0XBD,0X4A,0XAC,0X45,0XCC,0XC8,0XBC,0X45,0X83,0X21,0XA4,0X87,0X84,0X49,0XBE,0X74,
0X5B,0X89,0X64,0X2F,0X75,0X5A,0X5C,0XFD,0X4D,0X1F,0X3D,0X3F,0X35,0X3E,0X3D,0X7E,
0X3D,0X3E,0X3D,0X3D,0X45,0X1C,0X45,0X3C,0X45,0X3B,0X45,0X5C,0X3D,0X5F,0X3D,0X5F,
0X3D,0X3F,0X45,0X7F,0X45,0X5F,0X45,0X7E,0X45,0X9C,0X3D,0X3B,0X4C,0XFC,0X6D,0X5B,
0XC7,0XBD,0X9E,0X98,0X5D,0X19,0X4D,0X1C,0X35,0X1D,0X45,0X9F,0X45,0X3D,0X4D,0X5D,
0X45,0X5E,0X3D,0X5E,0X3D,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5F,
0X45,0X5F,0X45,0X7F,0X45,0X5F,0X45,0X7F,0X45,0X5F,0X45,0X7F,0X45,0X5F,0X45,0X7F,
0X3D,0X7E,0X3D,0X7E,0X3D,0X7E,0X3D,0X7E,0X3D,0X7E,0X3D,0X7E,0X3D,0X7E,0X3D,0X7E,
0X3D,0X7E,0X3D,0X7E,0X3D,0X7F,0X3D,0X7F,0X3D,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5E,
0X45,0X3E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X4D,0X5D,
0X55,0X1B,0X65,0XBD,0X65,0XBE,0X4D,0X1C,0X4D,0X1D,0X4D,0X3E,0X45,0X3E,0X45,0X3E,
0X71,0XE7,0X51,0X23,0X6A,0X66,0X8B,0X68,0XBC,0XA9,0XD5,0X49,0XF6,0X4C,0XF6,0X2D,
0XE6,0X10,0XFE,0XF6,0XE6,0X13,0X93,0X69,0X7A,0X64,0X82,0XE5,0XA4,0X4B,0X83,0X88,
0X51,0XE3,0X62,0X24,0X59,0XC1,0X6A,0X42,0X7A,0XC2,0X8B,0X02,0X9B,0X64,0XA3,0XA5,
0X9B,0X03,0X71,0XA0,0XAB,0X85,0XAB,0X44,0XA3,0X43,0XAB,0X83,0XA3,0X21,0XE5,0X6A,
0XDD,0X4B,0XC4,0XEA,0X59,0XE0,0X94,0X28,0X9C,0X8A,0XA4,0XEC,0XB5,0X2D,0X8B,0XE8,
0X8B,0XA7,0X7A,0XC4,0X9B,0XC9,0X72,0XE6,0X49,0XE3,0X29,0X41,0X73,0XED,0XA6,0X18,
0X6D,0X5A,0X5D,0X7D,0X55,0X5D,0X4D,0X5D,0X4D,0X7E,0X45,0X7E,0X4D,0X9E,0X4D,0X9D,
0X4D,0X7D,0X55,0X5C,0X55,0X3C,0X55,0X3C,0X55,0X3D,0X5D,0X1D,0X5D,0X3E,0X55,0X3E,
0X4D,0X3F,0X45,0X3F,0X4D,0X3F,0X4D,0X3E,0X4D,0X1E,0X4D,0X1E,0X55,0X3E,0X55,0X3E,
0X55,0X3E,0X55,0X3E,0X55,0X3F,0X55,0X3F,0X55,0X1F,0X55,0X3E,0X55,0X1E,0X65,0X1D,
0X7D,0X3A,0XD7,0X5F,0X73,0XEE,0X5A,0X86,0X49,0XA0,0XA3,0XE7,0XFF,0XB5,0XAB,0X84,
0X8A,0XA0,0XA3,0X83,0XFE,0XAE,0XC4,0XE7,0XCD,0X4A,0XDE,0X0F,0XCD,0X8F,0XAC,0XCD,
0XB4,0XED,0X9C,0X2A,0X83,0XE8,0XBD,0X8F,0X9C,0X6C,0XA4,0XAE,0X8C,0X2B,0X94,0X8B,
0X93,0XC6,0XA3,0X84,0XF5,0X6A,0XAB,0X20,0XAB,0XE3,0XA4,0X66,0XB5,0X4E,0X94,0XEE,
0X21,0X81,0X4A,0XE9,0X7D,0X38,0X5C,0XFB,0X4D,0X1E,0X3D,0X3F,0X35,0X5F,0X3D,0X7F,
0X3D,0X3E,0X3D,0X1D,0X45,0X1C,0X45,0X1B,0X45,0X3B,0X45,0X3C,0X3D,0X5F,0X3D,0X5F,
0X3D,0X5F,0X45,0X5F,0X45,0X5F,0X45,0X5E,0X45,0X7C,0X45,0X3A,0X55,0X3B,0X7D,0XBB,
0XC7,0X9C,0XAE,0XB9,0X6D,0X59,0X55,0X3B,0X3D,0X1C,0X45,0XBF,0X45,0X5D,0X45,0X3D,
0X45,0X5E,0X45,0X5E,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X45,0X7F,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,
0X45,0X7F,0X45,0X7F,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,
0X45,0X5E,0X45,0X5E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X5E,0X45,0X5E,0X45,0X5E,
0X45,0X5E,0X45,0X5E,0X45,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X55,0X5D,
0X55,0X3B,0X75,0XDD,0X6D,0XDE,0X55,0X3C,0X4D,0X1D,0X4D,0X3E,0X4D,0X3E,0X45,0X5E,
0X61,0X86,0X51,0X23,0X49,0X41,0X9B,0XC7,0XBC,0XE8,0XE6,0X0B,0XFE,0XCD,0XF6,0X6D,
0XEE,0X2F,0XF6,0X92,0XCD,0X2F,0X72,0X44,0X59,0X80,0X6A,0X43,0X83,0X46,0XCD,0XD1,
0X6B,0X07,0X6A,0XE6,0X7B,0X67,0X7B,0X25,0X9C,0X08,0X93,0XA5,0XC5,0X2A,0XC4,0XC9,
0X51,0X60,0X6A,0X00,0XA3,0XC5,0XA3,0XA4,0XA4,0X05,0XAC,0X46,0XBC,0XC8,0XB4,0X46,
0XB4,0X27,0X93,0X43,0X82,0XE2,0XD5,0XAD,0XA4,0X69,0XB5,0X0B,0XAC,0X8A,0XB4,0XEB,
0XAC,0X69,0X82,0XE4,0X93,0X67,0X7A,0XC5,0X39,0X20,0X29,0X00,0X39,0XA3,0X6B,0XCE,
0X8D,0XB9,0X7D,0XBB,0X6D,0X7B,0X5D,0X7B,0X4D,0X5C,0X45,0X7D,0X45,0X9E,0X45,0X9E,
0X4D,0X9E,0X4D,0X7E,0X55,0X7E,0X55,0X5E,0X5D,0X5E,0X5D,0X3E,0X5D,0X3F,0X5D,0X5F,
0X4D,0X5F,0X45,0X5F,0X4D,0X5E,0X4D,0X5D,0X4D,0X5D,0X55,0X5D,0X55,0X5D,0X4D,0X5D,
0X55,0X7E,0X4D,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X5D,0X7E,0X5D,0X5D,0X6D,0X3B,
0XA5,0XDA,0XD6,0X9A,0X6B,0X4A,0X28,0XE0,0X49,0XA0,0XDD,0XAF,0XFF,0X56,0XF5,0XEF,
0XAB,0X84,0X92,0XC0,0XE5,0X89,0XFE,0XAE,0X9B,0XC4,0X49,0X80,0X30,0XC0,0X41,0X20,
0X6A,0X65,0X51,0XA1,0X31,0X20,0X73,0X47,0X51,0XE3,0X5A,0X46,0X29,0X21,0X29,0X60,
0X41,0X60,0XAB,0XA5,0XE4,0X46,0XC3,0X61,0XBC,0X24,0XA4,0X46,0XC6,0X11,0X5B,0X07,
0X20,0XE0,0X52,0XA8,0X8D,0X57,0X64,0XD8,0X4C,0XFC,0X3D,0X1E,0X35,0X3F,0X35,0X5F,
0X3D,0X5F,0X45,0X3E,0X4D,0X3C,0X4D,0X1B,0X45,0X3C,0X45,0X3D,0X3D,0X3F,0X3D,0X3F,
0X3D,0X3F,0X45,0X3F,0X45,0X1F,0X45,0X3E,0X45,0X7C,0X45,0X3B,0X65,0X7B,0X8E,0X3C,
0XCF,0X9C,0XBE,0XDA,0X7D,0X99,0X65,0X5B,0X45,0X3C,0X4D,0XDF,0X45,0X5E,0X3D,0X1D,
0X45,0X7E,0X45,0X7E,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,0X45,0X7F,
0X45,0X7F,0X45,0X7E,0X45,0X7F,0X45,0X7E,0X45,0X7F,0X45,0X7E,0X45,0X7F,0X45,0X7E,
0X45,0X7F,0X45,0X7F,0X4D,0X7F,0X45,0X7F,0X4D,0X7F,0X45,0X7F,0X4D,0X7F,0X45,0X7F,
0X4D,0X7F,0X4D,0X9F,0X4D,0X7F,0X45,0X7E,0X45,0X7F,0X45,0X7E,0X45,0X5E,0X45,0X5E,
0X4D,0X7F,0X4D,0X7E,0X4D,0X7F,0X4D,0X7E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,
0X45,0X3E,0X45,0X3E,0X45,0X3E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X55,0X3D,
0X5D,0X3A,0X75,0XFC,0X75,0XFD,0X5D,0X3C,0X4D,0X1C,0X4D,0X3D,0X4D,0X3E,0X4D,0X5E,
0X61,0XE6,0X51,0X62,0X51,0X40,0X9B,0XC4,0XEE,0X6C,0XCD,0XA7,0XEE,0XCC,0XFF,0X2E,
0XFF,0X30,0XEE,0X2E,0X93,0X86,0XB4,0X8B,0X93,0XC8,0X83,0X66,0X8B,0XA6,0XD5,0XEF,
0XB5,0X6F,0X4A,0X22,0XA4,0XED,0XA4,0X8B,0X72,0XE4,0X9B,0XE8,0XE6,0X0F,0XA3,0XE5,
0XA4,0X05,0XB4,0X86,0XCD,0X49,0XBC,0XE8,0XC5,0X4A,0XCD,0X8C,0XD5,0XEF,0XC5,0X4C,
0XA3,0XA5,0XBC,0X27,0XBC,0X68,0XB4,0X68,0XB4,0X89,0XB4,0XCA,0XB4,0X89,0XBC,0XA9,
0XC4,0XAA,0XC4,0X69,0XA3,0X87,0X72,0X02,0X59,0X80,0X38,0XE0,0X49,0XC2,0X41,0XE4,
0XB6,0X17,0XB6,0XBC,0X96,0X1A,0X6D,0X59,0X55,0X3A,0X55,0XDD,0X3D,0X7D,0X45,0XDF,
0X3D,0X9F,0X45,0X9F,0X45,0X7F,0X4D,0X7F,0X55,0X7F,0X55,0X5F,0X55,0X5F,0X55,0X7F,
0X55,0XDF,0X4D,0XDE,0X4D,0XBE,0X4D,0XBD,0X4D,0X9C,0X4D,0X9C,0X4D,0XBC,0X4D,0XBC,
0X4D,0XDD,0X4D,0XDE,0X4D,0XFE,0X4D,0XBD,0X4D,0X5C,0X55,0X7C,0X65,0XBC,0X7D,0X79,
0XDE,0XFB,0XA4,0X8F,0X5A,0X24,0X39,0X60,0XA4,0X6C,0XF6,0XD5,0XFF,0XD9,0XFF,0X15,
0XD5,0X2C,0XA3,0X63,0XBC,0X45,0XFE,0X4D,0XD5,0X4B,0X6A,0X41,0X38,0XE0,0X41,0X21,
0X9B,0XAA,0X51,0XA1,0X49,0XE2,0X7B,0XA9,0X51,0XE4,0X6A,0XC8,0X21,0X21,0X21,0X20,
0X7B,0X25,0XE5,0X8D,0XBA,0XE0,0XD3,0XC3,0XBC,0X66,0XC5,0X8C,0XAD,0X4F,0X42,0X24,
0X31,0X41,0X4A,0X05,0X5B,0X8E,0X7D,0X58,0X55,0X1B,0X45,0X3E,0X35,0X1F,0X3D,0X5F,
0X3D,0X1E,0X45,0X1C,0X4D,0X3C,0X55,0X5D,0X4D,0X3D,0X44,0XFD,0X3D,0X3F,0X45,0X7F,
0X3C,0XFF,0X45,0X5F,0X45,0X1F,0X55,0X9F,0X45,0X3C,0X44,0XFA,0X5D,0X19,0XA6,0XBD,
0XBE,0XB9,0XBE,0X98,0X8D,0X98,0X5C,0XF9,0X4D,0X7C,0X4D,0X9E,0X45,0X5E,0X4D,0X7F,
0X45,0X5E,0X45,0X7E,0X45,0X5E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7F,0X45,0X7F,
0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,
0X4D,0X7F,0X4D,0X7E,0X4D,0X7F,0X4D,0X7E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,
0X4D,0X7F,0X4D,0X7E,0X4D,0X5E,0X45,0X5E,0X45,0X3E,0X45,0X3E,0X45,0X3E,0X45,0X5E,
0X45,0X3E,0X45,0X5E,0X4D,0X5E,0X4D,0X5E,0X45,0X5E,0X45,0X5E,0X4D,0X5E,0X4D,0X5E,
0X45,0X3E,0X4D,0X3E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X3E,0X4D,0X3E,0X4D,0X1C,
0X54,0XF8,0X8E,0X9E,0X8E,0X9F,0X5D,0X3A,0X4C,0XFB,0X55,0X5D,0X55,0X7E,0X4D,0X5E,
0X59,0XE5,0X41,0X00,0X61,0XA0,0XAC,0X04,0XF6,0XCE,0XF6,0XED,0XEF,0X0E,0XF6,0XEE,
0XE5,0XEB,0XFE,0XAF,0XCD,0X6C,0XFE,0XF3,0X72,0XE4,0X8B,0XC7,0XAC,0X67,0X83,0X63,
0XDE,0X72,0XBD,0X4F,0XCD,0XB1,0X9B,0XEA,0XBC,0XEE,0XE5,0XF1,0XAC,0X08,0X93,0X44,
0XE5,0XAC,0XBC,0X86,0XC5,0X08,0XB4,0X87,0X9C,0X06,0XB4,0XEB,0XDE,0X11,0XA4,0X4A,
0X93,0X87,0XA3,0XE8,0XAC,0X69,0XB4,0XAA,0XA4,0X69,0XA4,0X28,0XC5,0X2C,0XB4,0X8A,
0XB4,0X49,0XB4,0X29,0XA3,0XA8,0X82,0X84,0X61,0XC1,0X49,0X40,0X41,0X20,0X41,0X82,
0X8C,0X2D,0XC6,0X78,0X7C,0XD2,0X7D,0X56,0X65,0X58,0X55,0X7A,0X45,0X9C,0X3D,0XBE,
0X46,0X1F,0X45,0XFF,0X45,0XFF,0X45,0XDF,0X4D,0XBF,0X4D,0X9F,0X4D,0X9F,0X4D,0X9E,
0X45,0X9D,0X45,0XBD,0X45,0XBD,0X4D,0XDC,0X4D,0XDC,0X4D,0XDC,0X45,0XDC,0X45,0XDC,
0X3D,0XDD,0X3D,0XFE,0X3D,0XDD,0X4E,0X1E,0X5E,0X3E,0X5D,0XFC,0X65,0XFB,0X9E,0X5A,
0XF7,0X1A,0X8B,0X28,0X39,0X20,0X62,0XA4,0XD6,0X73,0XF7,0X77,0XF7,0X37,0XF6,0XD5,
0XFF,0X14,0XC4,0XA9,0XA3,0X83,0XB3,0XE4,0XFF,0XB5,0XAC,0X4A,0X72,0XA6,0X8B,0X49,
0XDD,0X91,0X51,0X60,0X49,0XC1,0X83,0XEA,0X8B,0X8A,0X72,0XE8,0X52,0X86,0X6B,0X68,
0XBD,0X2D,0XB3,0XE6,0XCB,0X82,0XE4,0X86,0XA3,0XA3,0XE6,0X91,0X9C,0XAC,0X39,0XC3,
0X41,0X62,0X41,0XA3,0X5B,0X4A,0X8D,0XD7,0X5D,0X19,0X55,0X5E,0X45,0X1F,0X45,0X5F,
0X45,0X5F,0X45,0X3D,0X4D,0X3C,0X55,0X5D,0X55,0X5F,0X4D,0X3F,0X45,0X5F,0X4D,0X7F,
0X45,0X5F,0X4D,0X7F,0X45,0X1F,0X4D,0X5F,0X4D,0X5D,0X55,0X5B,0X86,0X1C,0XD7,0XDF,
0XE7,0X9C,0XC6,0X77,0XAE,0X3A,0X85,0XBB,0X44,0XF9,0X45,0X3C,0X55,0X9F,0X3C,0XFD,
0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X7E,0X45,0X7E,0X45,0X7E,0X45,0X7E,
0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,
0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X3E,0X4D,0X3E,0X4D,0X3E,
0X4D,0X5E,0X4D,0X5E,0X4D,0X3E,0X4D,0X3E,0X4D,0X3D,0X4D,0X3D,0X4D,0X3E,0X4D,0X3E,
0X45,0X3D,0X45,0X3E,0X45,0X5E,0X4D,0X5E,0X45,0X5E,0X45,0X5E,0X4D,0X5E,0X4D,0X5E,
0X4D,0X3E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X55,0X3C,
0X6D,0X7A,0XAF,0X5F,0XAF,0X5F,0X6D,0X7B,0X5D,0X5C,0X5D,0X7D,0X55,0X7E,0X55,0X7E,
0X41,0X82,0X51,0XC1,0XA3,0XE6,0XCD,0X2A,0XDE,0X0D,0XCD,0XED,0XCE,0X0E,0XDE,0X4F,
0XE6,0X0E,0XEE,0X0F,0XEE,0X51,0XF6,0XB3,0XBD,0X6E,0X94,0X28,0X8B,0X62,0X83,0X21,
0XD5,0X8F,0XFF,0X58,0XDD,0X71,0XDD,0X30,0XE5,0X51,0X9A,0XE7,0X71,0X80,0XAB,0X45,
0XC4,0X26,0XAB,0XA3,0XBC,0X25,0XBC,0X67,0XD5,0X4C,0XD5,0X6F,0XD5,0X50,0XAC,0X6E,
0XAC,0XF0,0X9C,0XAF,0XAD,0X51,0X8C,0X6D,0XA5,0X0F,0X9C,0XEE,0XA4,0XCD,0XAC,0XCE,
0XB4,0XEE,0XA4,0X4C,0XB4,0X6C,0X93,0X69,0X72,0X86,0X62,0X24,0X41,0X41,0X49,0X82,
0X6A,0X86,0XB5,0X31,0X94,0XB0,0X95,0X75,0X96,0X3A,0X76,0X1B,0X5D,0XFC,0X55,0XFD,
0X3D,0X9C,0X3D,0XBD,0X3D,0XBD,0X3D,0XDD,0X45,0XFE,0X46,0X1E,0X46,0X1E,0X46,0X1E,
0X45,0XFE,0X4D,0XFF,0X4D,0XFE,0X4E,0X1E,0X4E,0X1E,0X4E,0X1E,0X46,0X3F,0X46,0X3F,
0X3E,0X3F,0X3E,0X3F,0X35,0XFF,0X45,0XFE,0X56,0X1E,0X55,0X9A,0X6D,0XBA,0XB6,0XFC,
0XCD,0X73,0X71,0XE2,0X49,0X40,0X9C,0X6A,0XFF,0XF7,0XEF,0X96,0XD6,0XB4,0XEE,0XF6,
0XFF,0X16,0XFE,0XF5,0X9B,0X64,0XA3,0X64,0XDD,0X4C,0XF6,0X52,0X9B,0XCA,0XAC,0X6C,
0XC4,0XCC,0X51,0X80,0X8B,0XE9,0XBD,0X90,0X93,0XEA,0X83,0XA9,0XA5,0X0E,0X9C,0XCC,
0XBD,0X4C,0X9B,0X83,0XDC,0X85,0XBB,0XA1,0XD5,0X8B,0XD6,0X2F,0X7B,0XE8,0X63,0X27,
0X41,0X62,0X30,0XE0,0X42,0X45,0X8D,0X73,0X6D,0X17,0X5D,0X5C,0X4D,0X1F,0X4D,0X5F,
0X4D,0X5F,0X45,0X3D,0X4D,0X1C,0X4D,0X3D,0X55,0X5F,0X4D,0X3F,0X45,0X3F,0X45,0X3E,
0X4D,0X5F,0X4D,0X5F,0X55,0X5F,0X4D,0X3F,0X4D,0X5D,0X55,0X19,0X85,0XDA,0XCF,0X3C,
0XD6,0X97,0XD6,0X97,0XAE,0X18,0X85,0X98,0X5D,0X7A,0X4D,0X7D,0X4D,0X7E,0X55,0X9F,
0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,
0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,0X45,0X5E,
0X4D,0X3E,0X4D,0X3E,0X4D,0X3E,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,
0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,
0X45,0X3D,0X45,0X5D,0X4D,0X5E,0X4D,0X5E,0X45,0X5E,0X45,0X5D,0X4D,0X5E,0X4D,0X5E,
0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X7E,0X4D,0X5E,0X55,0X7E,0X55,0X7E,0X5D,0X7C,
0X6D,0X58,0XB7,0X9F,0XB7,0XBF,0X65,0X5A,0X5D,0X5B,0X55,0X3B,0X4D,0X5C,0X4D,0X5D,
0X4A,0X03,0X73,0X06,0XC5,0X0C,0XEE,0X50,0XE6,0X51,0XDE,0X72,0XE6,0XB4,0XDE,0X73,
0XFF,0X55,0XC4,0XEC,0XFF,0XB7,0XEE,0X93,0XE6,0X93,0XE6,0X92,0XAC,0X67,0X6A,0X40,
0X8B,0X05,0XD5,0X0F,0X9B,0X49,0X9A,0XE8,0X9A,0XE7,0X82,0X23,0XA3,0X06,0XA3,0X24,
0XAB,0X64,0XAB,0X63,0XBC,0X26,0XCC,0XC9,0XED,0XEF,0XC4,0XCD,0XB4,0X4D,0XAC,0XB0,
0XAD,0X74,0X84,0XB1,0X9D,0X94,0X4A,0XE9,0XA5,0XB3,0XB6,0X14,0X5B,0X28,0XA5,0X30,
0XBD,0XB3,0X83,0XCB,0XBD,0X31,0XA4,0X6E,0X8B,0XAB,0X7B,0X4A,0X6A,0XC8,0X51,0XE4,
0X83,0X48,0X93,0XCA,0XA4,0XCF,0X73,0XED,0X74,0X92,0X7D,0X56,0X75,0XB9,0X7E,0X7D,
0X7E,0XDE,0X76,0XBE,0X6E,0X7D,0X5E,0X5C,0X56,0X1C,0X4D,0XFB,0X45,0XFB,0X45,0XDB,
0X55,0XFE,0X55,0XDE,0X55,0XBD,0X4D,0XBC,0X4D,0XBC,0X4D,0XBC,0X45,0XDE,0X3D,0XFF,
0X46,0X7F,0X46,0X5F,0X46,0X3F,0X4E,0X1F,0X55,0X9C,0X65,0X9A,0X96,0X9D,0XD7,0X7E,
0X72,0X67,0X7A,0X02,0X93,0X86,0XDE,0X4F,0XFF,0XD5,0XD6,0XF2,0XCE,0X72,0XEF,0X56,
0XFF,0X16,0XF6,0X73,0XE5,0XCF,0XAB,0XC6,0XAB,0XE7,0XDD,0X8E,0XFF,0X36,0XE6,0X11,
0XC4,0XEB,0X61,0XE0,0X73,0X25,0XBD,0XB0,0X9C,0X8B,0XA4,0XAB,0XAD,0X4D,0XC5,0XCF,
0X93,0XC5,0XB4,0X65,0XD4,0XC5,0XB4,0X02,0XF6,0XD0,0X9C,0X89,0X73,0X66,0X9C,0X8C,
0X62,0X45,0X41,0X62,0X41,0XE3,0X74,0X6E,0X85,0X96,0X6D,0X5A,0X54,0XDC,0X55,0X3F,
0X4D,0X3E,0X4D,0X3D,0X45,0X1C,0X4D,0X3D,0X4D,0X3F,0X4D,0X3F,0X4D,0X1E,0X45,0X1E,
0X4D,0X1E,0X4D,0X1E,0X5D,0X7F,0X4D,0X1E,0X55,0X5D,0X5D,0X18,0X96,0X18,0XC6,0XD9,
0XAD,0X30,0XC5,0XB3,0XA5,0X74,0X7D,0X15,0X65,0X79,0X4D,0X3B,0X3C,0XDB,0X55,0X7F,
0X4D,0X5E,0X45,0X5E,0X4D,0X5E,0X45,0X5E,0X4D,0X5E,0X45,0X5E,0X4D,0X5E,0X45,0X5E,
0X45,0X5D,0X45,0X5D,0X45,0X5D,0X45,0X5D,0X45,0X5D,0X45,0X5D,0X45,0X5D,0X45,0X5D,
0X4D,0X3E,0X4D,0X3E,0X4D,0X3E,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,
0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3D,0X4D,0X3E,0X4D,0X3E,0X4D,0X3E,
0X45,0X5D,0X4D,0X5E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X5E,0X4D,0X7E,0X4D,0X7E,
0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X55,0X7E,0X55,0X7F,0X55,0X9F,0X5D,0X9D,
0X6D,0X58,0XC7,0XBF,0XC7,0XFF,0X6D,0X7A,0X6D,0XBC,0X65,0X9C,0X5D,0XDD,0X5D,0XDE,
0X62,0XA7,0XAC,0XEF,0XE6,0X74,0XFF,0X57,0XDE,0X74,0XD6,0X33,0XFF,0X59,0XEE,0XF6,
0XFF,0X78,0XCD,0X6F,0XFF,0X36,0XFF,0X57,0XFF,0X56,0XF6,0XD4,0X7A,0XE3,0X59,0XA0,
0X7A,0XC5,0X7A,0XC6,0X9B,0XA9,0X61,0XE2,0X48,0XE0,0X72,0X01,0X93,0X25,0XAB,0XE7,
0XAC,0X06,0X93,0X64,0XB4,0X88,0XB4,0XA9,0XA4,0X29,0X9C,0X4A,0XCD,0XD2,0XCE,0X35,
0XAE,0X16,0X8D,0X54,0XA6,0X37,0X53,0X8C,0XAE,0X36,0XB6,0X56,0X4B,0X09,0X8C,0XF0,
0XB5,0XF5,0X63,0X0A,0XB5,0X94,0XAD,0X53,0X9C,0XD1,0X8C,0X70,0X94,0XB1,0X5A,0XA7,
0X83,0X69,0X83,0X48,0XAC,0XCF,0X94,0X6E,0X52,0XA9,0X5B,0X2B,0X53,0X2C,0X53,0XAF,
0X4B,0X8E,0X54,0X30,0X6C,0XF4,0X7D,0XB7,0X86,0X59,0X86,0X7A,0X76,0X7A,0X76,0X5B,
0X6D,0XBB,0X6D,0XBC,0X6D,0XBC,0X6D,0XBC,0X6D,0XDC,0X65,0XDD,0X5D,0XFE,0X56,0X1F,
0X4E,0X1F,0X3D,0X9E,0X4D,0XDF,0X5D,0XFE,0X65,0X7A,0X96,0X3B,0XBE,0XBC,0XB5,0XB6,
0X40,0XC0,0X92,0XE5,0XDD,0XAD,0XFF,0X73,0XF7,0X71,0XD6,0XAF,0XD6,0X70,0XF7,0X55,
0XFF,0X35,0XB4,0X8A,0XFF,0X14,0XF6,0X50,0XCD,0X2B,0X9B,0X85,0XFE,0XD3,0XDD,0XCF,
0X83,0X03,0X83,0X03,0X8C,0X09,0XAD,0X4F,0X9C,0XCB,0X9C,0X89,0X94,0X48,0XBD,0X4B,
0X8B,0X83,0XB4,0X44,0XCC,0XE5,0XFE,0X6C,0XBD,0X2B,0X83,0XA6,0X8B,0XC7,0XA4,0X8B,
0X9B,0XEB,0X72,0XE7,0X52,0X43,0X5B,0X48,0X9D,0XD5,0X7D,0X78,0X5C,0XD9,0X5D,0X5E,
0X55,0X3E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5D,0X4D,0X5E,0X55,0X5E,0X55,0X3E,0X4D,0X3E,
0X55,0X3D,0X4D,0X1D,0X5D,0X7F,0X4C,0XFD,0X5D,0X5C,0X75,0XBA,0XBF,0X1B,0XE7,0X9A,
0XEE,0XB5,0XDE,0X13,0XCE,0X76,0XAE,0X78,0X6D,0X98,0X5D,0X9B,0X65,0XDF,0X55,0X7E,
0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,
0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,0X4D,0X5D,
0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,
0X4D,0X5D,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,
0X4D,0X7E,0X4D,0X7E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X7E,0X4D,0X9E,0X4D,0X9E,
0X55,0X9E,0X55,0X9E,0X55,0X9E,0X55,0X9E,0X55,0X9E,0X55,0X9F,0X55,0XBF,0X65,0X9D,
0X75,0X78,0XC7,0X9F,0XC7,0XDF,0X7D,0XBA,0X6D,0XBB,0X6D,0XDD,0X65,0XFE,0X5D,0XFE,
0X83,0XCB,0XE6,0XF7,0XFF,0XDB,0XFF,0XFC,0XDE,0X75,0XCD,0XF3,0XFF,0X78,0XFF,0X78,
0XFF,0X97,0XF7,0X15,0XEE,0XD3,0XFF,0X55,0XFF,0XF8,0XAC,0X6A,0X62,0X42,0X72,0XA4,
0X6A,0X64,0X59,0XC1,0X62,0X02,0X72,0XA3,0X9B,0XC7,0XB4,0X48,0XC5,0X0A,0XC5,0X0A,
0XD5,0XAD,0XDE,0X0F,0XDE,0X10,0XA4,0X8A,0X8B,0XC8,0X8B,0XC8,0XA4,0XCD,0XA4,0XEE,
0XAD,0X93,0X9D,0X52,0XB6,0X35,0XA5,0XB3,0XBE,0X75,0XAD,0XF3,0X9D,0X71,0XA5,0X51,
0XC6,0X35,0X73,0XCD,0XC6,0X37,0XC6,0X37,0XBD,0XF6,0XAD,0X95,0XB5,0XF6,0X6B,0X6B,
0X18,0X60,0X51,0XE2,0X7B,0X48,0XDE,0X34,0X93,0XEC,0X9C,0X2D,0X8B,0XCC,0X8B,0XAC,
0X8B,0XCC,0X7B,0X8B,0X6B,0X0A,0X5A,0XE9,0X53,0X4B,0X63,0XEE,0X74,0XD2,0X7D,0X55,
0X95,0XDA,0X95,0XFB,0X95,0XFB,0X95,0XFB,0X8D,0XFA,0X7D,0XDA,0X6D,0X9B,0X5D,0X9B,
0X66,0X1E,0X55,0XBC,0X65,0XDD,0X7E,0X1C,0X95,0XFA,0XBE,0X7A,0XAD,0X33,0X5A,0X25,
0X40,0XC0,0X93,0X46,0XDD,0XCD,0XFF,0X72,0XFF,0X70,0XEF,0X0F,0XE6,0XAF,0XFF,0X53,
0XF6,0XD2,0XCD,0X6C,0XE6,0X0F,0XFF,0XB4,0XAC,0X26,0X82,0XE2,0X93,0X86,0XCD,0X2C,
0X9B,0XC6,0X72,0XC3,0X8C,0X2B,0X9C,0XEE,0XA5,0X2C,0XBD,0XED,0XB5,0X4B,0X51,0XE0,
0XA4,0X24,0XA3,0XE3,0XD5,0X88,0XEE,0X4D,0XAC,0X89,0X94,0X09,0XA4,0X49,0XA4,0X49,
0XAC,0X6C,0X8B,0XA9,0X4A,0X22,0X3A,0X02,0X84,0XEF,0X8D,0X75,0X6D,0X18,0X65,0X5C,
0X55,0X1D,0X4D,0X3F,0X4D,0X7E,0X4D,0X5D,0X4D,0X5C,0X55,0X5D,0X55,0X3E,0X55,0X3D,
0X55,0X3D,0X55,0X1C,0X5D,0X5E,0X55,0X1C,0X5D,0X3B,0X7D,0XD9,0XB6,0XD8,0XD6,0XD5,
0XE6,0X52,0XDD,0XD1,0XBD,0XB2,0X9D,0X93,0X6D,0X55,0X55,0X38,0X55,0X5B,0X55,0X5D,
0X55,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,
0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,0X4D,0X5E,
0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,
0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,0X4D,0X7E,
0X4D,0X7E,0X4D,0X9E,0X4D,0X9E,0X4D,0XBF,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X55,0XBF,
0X55,0X9E,0X55,0X9F,0X55,0X9F,0X55,0XBF,0X55,0X9F,0X55,0XBF,0X55,0XBF,0X65,0X9D,
0X7D,0XB8,0XC7,0X9E,0XC7,0XBF,0X8E,0X3B,0X65,0X59,0X65,0XBB,0X5D,0XDC,0X55,0XBC,
0XBD,0X91,0XF7,0X9A,0XF7,0XBB,0XFF,0XFC,0XF7,0X79,0XFF,0XD8,0XFF,0XF7,0XFF,0X94,
0XFF,0XB5,0XEF,0X32,0XFF,0XB4,0XF6,0XF1,0XF6,0X90,0XBC,0XAA,0XEE,0X74,0X93,0XCA,
0X62,0X04,0XB4,0X6C,0X7A,0XC4,0XA4,0X08,0XAC,0X47,0XA4,0X05,0XD5,0XAC,0XB4,0X88,
0XC5,0X2B,0XDD,0XEF,0XCD,0X6E,0X9C,0X2A,0XB5,0X0D,0XA4,0XCC,0X9C,0X8B,0XAC,0XCC,
0X9C,0X2A,0X9C,0X09,0XA4,0X8A,0XBD,0X4D,0XBD,0X4D,0XB5,0X2C,0XD6,0X51,0XC5,0XD0,
0XCD,0XF2,0XAD,0X10,0XC5,0XF4,0XCE,0X36,0XBE,0X16,0XBD,0XF5,0XB5,0XD5,0X8C,0X6F,
0X52,0X65,0X5A,0X65,0X62,0X85,0XBC,0XEF,0XBC,0X8E,0XCC,0XAE,0XB3,0X89,0XBB,0X89,
0XB3,0X07,0XB3,0X27,0XB3,0X69,0XB3,0X8A,0XA3,0X8B,0X8B,0X6B,0X73,0X0B,0X62,0XEA,
0X52,0XAA,0X5B,0X0B,0X6B,0XAD,0X7C,0X50,0X8D,0X12,0X95,0XB6,0X96,0X19,0X96,0X5B,
0X7D,0XFA,0X86,0X5B,0X8E,0X3A,0X9E,0X18,0XCE,0XD9,0XE6,0XB6,0XB4,0X2A,0X59,0X40,
0X61,0XE0,0XA4,0X48,0XDE,0X0D,0XFF,0X50,0XFF,0X2E,0XFF,0X2F,0XFE,0XCF,0XFF,0X31,
0XFF,0X10,0XF6,0XD0,0XE6,0X4D,0XD5,0XCB,0XAC,0XA7,0X8B,0X63,0XA3,0XC7,0XDD,0XAF,
0XEE,0X51,0X83,0X46,0X7B,0XAB,0XAD,0X51,0X8C,0X6A,0X94,0XA9,0XBD,0X6B,0XA4,0X05,
0XBC,0X86,0XC4,0XE7,0XD5,0X89,0XA4,0X66,0XCD,0X8F,0XB4,0XAD,0XB4,0X6A,0XB4,0X8A,
0XA4,0X4B,0X8B,0XC9,0X49,0XC1,0X31,0X80,0X52,0XE6,0X95,0X72,0X85,0X97,0X6D,0X3A,
0X5D,0X1D,0X4D,0X3F,0X4D,0X5E,0X45,0X5C,0X4D,0X7B,0X55,0X9C,0X5D,0X5D,0X55,0X1D,
0X55,0X3C,0X55,0X3C,0X5D,0X3D,0X65,0X5D,0X5D,0X39,0X86,0X19,0XAE,0X95,0XC6,0X31,
0XDD,0XF0,0XE6,0X11,0XBD,0X6F,0X95,0X51,0X86,0X17,0X65,0XD9,0X55,0X3B,0X65,0XFF,
0X55,0X7E,0X55,0X9E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,
0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X4D,0X7E,
0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,
0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X7E,0X4D,0X7E,
0X4D,0X9E,0X4D,0X9E,0X4D,0XBF,0X4D,0XBF,0X4D,0X9E,0X4D,0X9E,0X55,0X9E,0X55,0XBF,
0X55,0XBF,0X55,0XBF,0X55,0XBF,0X55,0XBF,0X55,0XBF,0X55,0XBF,0X55,0XBF,0X65,0X9C,
0X9E,0X9B,0XD7,0XFF,0XD7,0XFF,0XAF,0X5F,0X65,0X79,0X6D,0XFC,0X65,0XFD,0X5D,0XFD,
0XD6,0X11,0XFF,0XB8,0XFF,0XFA,0XF7,0X78,0XE6,0XD4,0XFF,0X54,0XE6,0X6F,0XE6,0XAF,
0XD6,0X6E,0XC5,0XEC,0XC5,0XAB,0XDE,0X0D,0XE6,0X0D,0XEE,0X50,0XFF,0X15,0XD5,0XD2,
0XBC,0XAD,0XB4,0X6B,0X59,0XA0,0X9B,0XE7,0X9B,0XE5,0X9B,0XE5,0XB4,0XA8,0XBC,0XEA,
0XDD,0XEF,0XAC,0X4A,0X93,0XC9,0X93,0XC9,0XAC,0XAC,0XA4,0XAC,0XB5,0X0E,0XB4,0XEC,
0XC5,0X2D,0XBC,0XCB,0XAC,0X49,0X9C,0X07,0XA4,0X28,0XA4,0X48,0XA4,0X49,0XB4,0XCB,
0XAC,0XCC,0XC5,0X90,0XAC,0XCE,0XB5,0X30,0XAD,0X30,0XBD,0XB2,0XAD,0X51,0XAD,0X30,
0X7B,0X89,0X39,0X40,0X49,0X81,0X6A,0X03,0XDD,0X50,0XED,0X4F,0XB3,0X06,0XCB,0X47,
0XD3,0X26,0XD3,0X26,0XD3,0X27,0XCB,0X48,0XC3,0X69,0XBB,0X8A,0XBB,0XAB,0XAB,0XCC,
0XA4,0X0C,0X9B,0XEB,0X8B,0XAA,0X83,0X69,0X73,0X48,0X6B,0X49,0X63,0X6B,0X5B,0X8C,
0X64,0X0F,0X8D,0X74,0X8D,0X12,0X94,0XF0,0XE6,0X95,0XF6,0X73,0XBC,0X08,0X82,0X81,
0X8B,0X64,0XD5,0XEE,0XFF,0X31,0XFF,0X71,0XFE,0XAD,0XFF,0X0F,0XFE,0XAF,0XF6,0XF1,
0XEE,0XB0,0XEF,0X11,0XF7,0X52,0XDE,0X6F,0XFF,0X74,0XC4,0XEA,0X9B,0X66,0X93,0X26,
0XF6,0X73,0XD5,0XF1,0X73,0X29,0X94,0X6D,0X9C,0XCB,0XBD,0XAD,0XAC,0XA7,0XDD,0XCC,
0XC4,0XE8,0XF6,0X8F,0XB4,0XE9,0X94,0X28,0XC5,0X70,0XBD,0X30,0XBC,0XAC,0XBC,0X8B,
0XBC,0XAD,0XAC,0X2B,0X5A,0X22,0X4A,0X01,0X31,0XA0,0XA5,0X92,0X9D,0XF8,0X6C,0XF8,
0X65,0X5D,0X5D,0X5E,0X55,0X5C,0X55,0X7A,0X65,0XBA,0X6D,0XDB,0X6D,0X7D,0X5D,0X1C,
0X65,0X7D,0X5D,0X7C,0X5D,0X1C,0X65,0X7D,0X65,0X59,0XAF,0X1C,0XDF,0XD9,0XF7,0X96,
0XF6,0XB2,0XE5,0XD0,0XD5,0XF1,0XBE,0X54,0X8E,0X16,0X75,0XF9,0X65,0XBC,0X4D,0X3C,
0X5D,0X9E,0X55,0X9E,0X55,0X9E,0X55,0X9E,0X55,0X9E,0X55,0X9E,0X4D,0X9E,0X4D,0X9E,
0X4D,0X7E,0X55,0X7E,0X55,0X7E,0X55,0X7E,0X4D,0X9E,0X4D,0X9E,0X55,0X7E,0X55,0X7E,
0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0X9E,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,
0X4D,0X9E,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0X9E,0X4D,0X9E,0X45,0X7E,0X45,0X7E,
0X4D,0X7E,0X4D,0X9E,0X4D,0XBE,0X4D,0XBF,0X4D,0XBF,0X4D,0XBF,0X4D,0X9F,0X55,0X9F,
0X55,0X9F,0X55,0XBF,0X55,0XBF,0X55,0XDF,0X55,0XDF,0X55,0XBE,0X55,0XBE,0X5D,0X9B,
0XA6,0XDB,0XDF,0XFF,0XDF,0XFF,0XC7,0XDF,0X65,0X16,0X6D,0XBA,0X5D,0XBB,0X5D,0XFD,
0XDD,0XEC,0XDD,0XCC,0XE6,0X6F,0XDE,0X0D,0XD5,0XAC,0XD5,0XAB,0XC5,0X29,0XCD,0X8B,
0XDE,0X0D,0XDE,0X4E,0XFE,0XF1,0XFE,0XF1,0XFE,0XD1,0XF6,0X91,0XD5,0XAE,0XDE,0X10,
0XFF,0X96,0X93,0XE7,0X6A,0X81,0X93,0XA5,0X8B,0X85,0X83,0X44,0XA4,0X49,0XC5,0X2D,
0X8B,0X87,0X51,0XC1,0X39,0X00,0X39,0X00,0X49,0X81,0X62,0X45,0X6A,0XA6,0X7B,0X07,
0X8B,0XCA,0X9C,0X4C,0XAC,0XCD,0XBD,0X0E,0X9C,0X2A,0X9C,0X2A,0X94,0X09,0X93,0XE8,
0X94,0X09,0XA4,0X6A,0XA4,0X6A,0XA4,0X6A,0XAC,0XCB,0XAC,0XCB,0X9C,0X49,0X93,0XE8,
0X8B,0X67,0X6A,0X63,0X61,0XC0,0X72,0X22,0XA3,0X67,0XC4,0X2A,0XBB,0XA8,0XBB,0X46,
0XC3,0X46,0XC3,0X26,0XCB,0X47,0XCB,0X27,0XCA,0XE7,0XD3,0X49,0XDB,0X8A,0XCB,0X49,
0XBB,0X89,0XB3,0XA9,0XB3,0XA9,0XB3,0XE9,0XB3,0XEA,0XAB,0XEA,0XA3,0XCA,0X9B,0XC9,
0X93,0XA9,0X83,0X88,0X93,0XCA,0X9C,0X0A,0X83,0X26,0X7A,0XA3,0X9B,0X86,0XBC,0XCA,
0XD5,0XAC,0XE6,0X4D,0XF6,0XAE,0XF6,0XAE,0XFE,0XCF,0XFF,0X11,0XFF,0X54,0XF7,0X75,
0XEF,0X77,0XFF,0XFA,0XFF,0XFB,0XFF,0XDA,0XFF,0XB9,0XFE,0XB4,0XD4,0XCC,0XA3,0X67,
0XB4,0X8B,0XF7,0X16,0XC5,0X70,0XDD,0XF2,0XD5,0XAE,0XA4,0X05,0X8B,0X81,0XD5,0XCA,
0XAC,0XA7,0XD6,0X2F,0XB5,0X4E,0XBD,0X92,0XA4,0X90,0XB5,0X32,0XAC,0XEE,0XA4,0X0A,
0XD4,0X8E,0XE5,0X30,0X6A,0X21,0X41,0XA0,0X42,0X01,0X6B,0XAB,0X95,0X57,0X7C,0XF9,
0X64,0XFA,0X6D,0X9C,0X6D,0X99,0X96,0XDC,0X9E,0X7A,0X9E,0X1A,0X85,0X5B,0X75,0X1B,
0X6D,0XBD,0X5D,0X9E,0X54,0XFC,0X75,0XDE,0X6D,0X38,0X9E,0X37,0XD7,0X16,0XCE,0X0F,
0XD5,0XAF,0XD5,0X4E,0XB5,0X0D,0XAD,0X50,0X7D,0X11,0X75,0XB7,0X5D,0X9C,0X55,0X5E,
0X5D,0XDF,0X4D,0X5D,0X45,0X5D,0X5E,0X1F,0X4D,0X9E,0X4D,0XDE,0X45,0XFE,0X4E,0X7F,
0X45,0X9E,0X55,0X9E,0X5D,0X9E,0X55,0X9E,0X45,0XDE,0X3D,0XDE,0X4D,0XBE,0X4D,0X9E,
0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,
0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0X9E,
0X55,0X9E,0X4D,0X9E,0X45,0XBE,0X45,0XDF,0X45,0XDF,0X45,0XDF,0X4D,0X9F,0X55,0X9F,
0X5D,0XBF,0X65,0XFF,0X55,0XFE,0X4D,0XBC,0X55,0XDD,0X55,0XDD,0X55,0X9D,0X65,0X7A,
0XBF,0X3C,0XEF,0XFD,0XDF,0X9B,0XD7,0XBC,0X85,0X75,0X75,0X97,0X65,0XBA,0X5D,0XFD,
0XDD,0XAA,0XDD,0XAA,0XEE,0X2C,0XE6,0X0C,0XEE,0X2D,0XF6,0XAF,0XEE,0X6E,0XF6,0XB0,
0XF6,0XB0,0XE6,0X2F,0XDD,0XEE,0XBD,0X0B,0XB4,0XAA,0XAC,0X89,0X9C,0X07,0XAC,0XAA,
0XD5,0XEF,0X8B,0XC6,0X8B,0XA6,0XB5,0X0B,0XCD,0XCF,0XC5,0X6E,0XB5,0X0D,0XA4,0X8B,
0X9C,0X4B,0X83,0X48,0X7B,0X48,0X73,0X07,0X6A,0XA6,0X6A,0X86,0X51,0XE4,0X49,0XA2,
0X41,0XA2,0X49,0XE2,0X4A,0X03,0X62,0XA5,0X5A,0X84,0X7B,0X68,0X8B,0XC9,0X94,0X0A,
0XA4,0X6B,0XAC,0XAB,0XAC,0XAB,0XB4,0XCB,0XBD,0X0C,0XBC,0XEB,0XAC,0X69,0XA4,0X28,
0XA3,0XE9,0X8B,0X05,0X61,0XC0,0X9B,0X87,0XDD,0X4F,0X9B,0X26,0XAB,0X66,0XB3,0X87,
0XB3,0X46,0XBB,0X26,0XC3,0X47,0XCB,0X47,0XCB,0X06,0XD3,0X27,0XDB,0X48,0XD3,0X27,
0XCB,0X88,0XCB,0X87,0XC3,0X87,0XC3,0XA8,0XCB,0XC8,0XCB,0XC8,0XC3,0XC8,0XC3,0XC8,
0XBB,0XC8,0XC4,0X09,0XCC,0X6A,0XBB,0XE8,0X92,0XC4,0X8A,0X82,0X9B,0X04,0XAB,0X86,
0X93,0X22,0X9B,0X42,0XA3,0X82,0XB4,0X04,0XBC,0X66,0XCD,0X2B,0XEE,0XB2,0XFF,0XF9,
0XFF,0XFB,0XFF,0XFD,0XF7,0XFD,0XF7,0XBB,0XFF,0X99,0XFE,0XF6,0XF6,0X11,0XED,0XF0,
0XAC,0X8B,0XAC,0XAC,0XFF,0XB8,0XC4,0XED,0XBC,0X89,0X9B,0XA3,0XB4,0XA5,0XBD,0X27,
0XBD,0X6A,0XC5,0XEE,0XAD,0X30,0XC5,0XD5,0XCE,0X16,0XAC,0XF1,0X5A,0X64,0X6A,0X64,
0XF5,0XD3,0XBC,0X4B,0X93,0XE6,0X52,0X60,0X4A,0X61,0X84,0X6C,0X9D,0X13,0X9D,0XB8,
0XAE,0XBC,0X8E,0X39,0XA6,0XD9,0X9E,0X76,0XBE,0XD9,0X95,0X96,0X7C,0XD6,0X75,0X3A,
0X65,0X7C,0X65,0XBD,0X65,0X7C,0X65,0X39,0XB7,0X5D,0XD7,0X9A,0XDF,0X14,0XF7,0X55,
0XEE,0X52,0XEE,0X51,0XDE,0X30,0XD6,0X72,0XA5,0XF2,0X96,0X57,0X7D,0XFB,0X65,0XBE,
0X5D,0X9D,0X55,0X9C,0X55,0XBD,0X5E,0X3F,0X55,0XDE,0X45,0XDE,0X35,0XFD,0X35,0XFD,
0X4D,0XDE,0X55,0XBE,0X5D,0X9E,0X55,0X9E,0X3D,0XDE,0X35,0XFE,0X4D,0XBF,0X55,0XDF,
0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDE,0X4D,0XDE,0X4D,0XBE,0X4D,0XBE,
0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,0X4D,0XBE,
0X55,0X9E,0X55,0X9E,0X45,0XBE,0X3D,0XDE,0X45,0XDF,0X45,0XDF,0X4D,0X9F,0X55,0X9F,
0X55,0X7E,0X5D,0XBE,0X4D,0XDD,0X4D,0XDC,0X56,0X1D,0X5D,0XFD,0X5D,0XBC,0X75,0XBA,
0XCF,0X7B,0XF7,0XFB,0XEF,0XB9,0XEF,0XFB,0XA6,0X35,0X8E,0X18,0X6D,0XDA,0X5D,0XDB,
0XF6,0X6E,0XEE,0X6E,0XEE,0X6F,0XDD,0XED,0XD5,0XAC,0XCD,0X6B,0XAC,0X68,0X9B,0XE6,
0X93,0XA6,0X9C,0X07,0XAC,0XAA,0XB4,0XEC,0XC5,0X2D,0XBD,0X4D,0XAC,0XCC,0XBD,0X2D,
0XB5,0X0D,0X9C,0X4A,0X9C,0X29,0X83,0X87,0X83,0X87,0X5A,0X43,0X52,0X01,0X39,0X40,
0X5A,0X64,0X5A,0X64,0X73,0X48,0X83,0X89,0X8B,0XCB,0X9C,0X6D,0X9C,0X8E,0X9C,0X6D,
0X83,0XE9,0X83,0XC8,0X7B,0X67,0X7B,0X87,0X5A,0X84,0X62,0X83,0X5A,0X42,0X52,0X22,
0X5A,0X42,0X62,0X63,0X6A,0X84,0X7B,0X06,0X93,0XC9,0XA4,0X0A,0XAC,0X4B,0XB4,0XAC,
0XAC,0X2B,0XA3,0XCA,0X9B,0X68,0X7A,0X85,0X8A,0XC5,0X8A,0XE5,0XDD,0X2E,0XA3,0X46,
0XBB,0XC8,0XB3,0X46,0XBB,0X66,0XC3,0XA7,0XC3,0X66,0XC3,0X46,0XCB,0X87,0XD3,0X87,
0XD3,0X67,0XD3,0X47,0XCB,0X26,0XCB,0X26,0XCB,0X26,0XCB,0X46,0XD3,0X46,0XD3,0X66,
0XD3,0X66,0XE4,0X08,0XEC,0X49,0XCB,0X45,0XAA,0X61,0XBA,0XC2,0XCB,0X65,0XC3,0X44,
0XC3,0X84,0XB3,0X42,0XB3,0X22,0XBB,0X62,0XB3,0X63,0XB3,0XA5,0XDD,0X6D,0XFF,0X56,
0XFF,0XD9,0XF7,0XFB,0XF7,0XFB,0XFF,0XFC,0XFF,0XFA,0XFF,0XD7,0XFF,0X33,0XFF,0X33,
0X93,0XE8,0X62,0X63,0X8B,0X87,0XB4,0X6A,0X61,0XC0,0X8B,0X22,0X93,0XC2,0XBD,0X28,
0XDE,0X4E,0XD6,0X50,0XC5,0XF3,0XCD,0XF4,0XCD,0XF5,0X83,0X8A,0X39,0X40,0X94,0X09,
0XC5,0XF1,0X84,0X69,0XBE,0XAF,0XA6,0X0C,0XA5,0XCB,0XB6,0X0D,0XAD,0X8D,0X9D,0X6E,
0X85,0X0D,0X64,0X6B,0X7D,0X2D,0X85,0X2E,0XB6,0X75,0X64,0X2E,0X8D,0XB6,0X86,0X1B,
0X65,0X7B,0X55,0X5A,0X75,0XFA,0X7D,0X96,0XBF,0X38,0XC6,0XB3,0XD6,0XB2,0XE6,0X91,
0XE6,0X11,0XDE,0X10,0XD6,0X0E,0XCE,0X6F,0XA5,0XCD,0X9D,0XF1,0X85,0X76,0X75,0X79,
0X65,0XBB,0X65,0XFC,0X6E,0X3E,0X65,0XFE,0X5D,0XFE,0X55,0XFE,0X4E,0X1E,0X45,0XDC,
0X55,0XFD,0X55,0XBC,0X55,0X9C,0X55,0XDD,0X46,0X1D,0X3D,0XDE,0X4D,0XDF,0X5D,0XFF,
0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,
0X4D,0XDE,0X4D,0XDE,0X4D,0XDE,0X4D,0XDE,0X4D,0XDE,0X4D,0XDE,0X4D,0XDE,0X4D,0XBE,
0X55,0X9D,0X4D,0X9D,0X45,0XBE,0X45,0XDE,0X45,0XDE,0X45,0XBE,0X4D,0X9E,0X55,0X9E,
0X55,0X7E,0X55,0X9E,0X45,0XBE,0X4D,0XFE,0X5E,0X1E,0X5D,0XDC,0X65,0XBB,0X86,0X19,
0XD7,0XBB,0XF7,0XFA,0XEF,0X98,0XF7,0XFA,0XB6,0X76,0X9E,0X57,0X75,0XF9,0X5D,0X99,
0XB4,0XA9,0XAC,0X88,0XA4,0X48,0X9C,0X27,0XAC,0X69,0XBD,0X0B,0XBD,0X2C,0XBD,0X2D,
0XC5,0X8E,0XC5,0X4E,0XB4,0XCD,0X9C,0X2A,0X8B,0XA9,0X73,0X06,0X5A,0X23,0X5A,0X43,
0X49,0XC1,0X52,0X23,0X62,0XA5,0X39,0X80,0X52,0X43,0X49,0XC2,0X6A,0XE6,0X73,0X48,
0X4A,0X24,0X4A,0X24,0X52,0X65,0X39,0XC2,0X31,0X81,0X4A,0X24,0X4A,0X45,0X4A,0X44,
0X62,0XE6,0X6B,0X27,0X7B,0X88,0X94,0X6C,0X8C,0X0A,0X94,0X4B,0X94,0X2B,0X9C,0X6C,
0X83,0X88,0X83,0X88,0X83,0X68,0X7B,0X27,0X7B,0X06,0X6A,0XA5,0X6A,0X64,0X72,0XC5,
0X93,0X88,0X6A,0X23,0X8B,0X27,0X61,0XC1,0X49,0X00,0X69,0XE1,0XB4,0X0A,0XC4,0X8B,
0XBC,0X4A,0XAB,0X87,0XA3,0X46,0XB3,0X87,0XB3,0X67,0XAB,0X26,0XB3,0X67,0XC3,0X88,
0XD3,0XA8,0XD3,0X88,0XD3,0X67,0XD3,0X47,0XD3,0X47,0XD3,0X66,0XDB,0X66,0XDB,0X66,
0XE3,0X86,0XE3,0XC7,0XE3,0X65,0XC2,0X81,0XBA,0X40,0XDB,0X43,0XF3,0XE6,0XE3,0X84,
0XDB,0X64,0XD3,0X44,0XD3,0X64,0XDB,0XA4,0XCB,0X83,0XC3,0X84,0XD4,0X89,0XE5,0XF0,
0XFF,0XF9,0XFF,0XFA,0XF7,0XFA,0XF7,0XD9,0XFF,0XF8,0XEF,0X55,0XDE,0X90,0XD6,0X0E,
0XB5,0X0C,0X7B,0X25,0X8B,0X87,0X8B,0X46,0X9B,0XA5,0XD5,0X8B,0XB4,0XC7,0XC5,0X6A,
0XAC,0XC9,0XBD,0X4D,0XCD,0XD2,0XC5,0XB3,0XB5,0X10,0X62,0X65,0X52,0X01,0XC5,0X8E,
0X9C,0XA9,0X84,0X26,0X84,0X85,0X8C,0XA5,0X8C,0X42,0X8B,0XE1,0XBC,0XE6,0XAC,0XA6,
0XAD,0X28,0X9D,0X28,0X84,0XA8,0XAD,0XEF,0X95,0X4F,0X19,0XE4,0X7D,0X74,0X65,0X36,
0X6D,0XDB,0X6D,0X9A,0X86,0X38,0XBF,0X59,0XD7,0X75,0XDF,0X32,0XFF,0XD5,0XF7,0X13,
0XF6,0XD4,0XE6,0X92,0XD6,0X4E,0XD6,0XCE,0XBE,0X2C,0XBE,0X70,0XB6,0X15,0XA6,0X39,
0X6D,0X97,0X6E,0X1A,0X75,0XDB,0X5D,0X39,0X5D,0X7B,0X5D,0XDC,0X5E,0X1D,0X56,0X1C,
0X66,0X1C,0X5D,0XBB,0X55,0XBB,0X55,0XFC,0X46,0X3E,0X45,0XFE,0X4D,0XBF,0X55,0XDF,
0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X4D,0XDF,0X45,0XDE,
0X4D,0XDE,0X45,0XDE,0X4D,0XDE,0X45,0XDE,0X4D,0XDE,0X45,0XDE,0X4D,0XDE,0X4D,0XDE,
0X4D,0XBD,0X4D,0XBD,0X4D,0XBE,0X45,0XBE,0X45,0XBE,0X4D,0XBE,0X4D,0X9D,0X55,0X9D,
0X55,0XBE,0X4D,0XBE,0X45,0XBE,0X4E,0X1F,0X5E,0X1E,0X5D,0XBB,0X6D,0X99,0X96,0X38,
0XD7,0X79,0XE7,0X77,0XDE,0XF4,0XE7,0X56,0XB6,0X34,0X9E,0X15,0X86,0X18,0X6D,0XF9,
0XAC,0XAB,0XB4,0XEC,0XB5,0X0C,0XB4,0XEC,0XAC,0XAB,0X94,0X09,0X7B,0X67,0X6A,0XC5,
0X5A,0X43,0X52,0X03,0X39,0X61,0X39,0X60,0X41,0XC2,0X4A,0X03,0X5A,0X65,0X6B,0X08,
0X62,0XC7,0X73,0X4A,0X8C,0X0D,0X7B,0X8A,0XA5,0X10,0X94,0X6E,0XA5,0X10,0XAD,0X51,
0X73,0X8A,0X6B,0X8A,0X6B,0X8A,0X52,0XA6,0X42,0X65,0X63,0X28,0X63,0X49,0X5B,0X08,
0X4A,0X67,0X41,0XE6,0X39,0XA4,0X52,0X46,0X42,0X05,0X52,0X46,0X52,0X45,0X5A,0X65,
0X6B,0X07,0X7B,0X88,0X8B,0XE9,0X94,0X2A,0X9C,0X6A,0X94,0X29,0X94,0X09,0XA4,0X6A,
0X8B,0XA7,0X8B,0X86,0X9B,0XE7,0X93,0XA6,0X9B,0XE8,0X7A,0XE4,0X6A,0X42,0XC5,0X0D,
0XCC,0XED,0XA3,0XC9,0X9B,0X47,0XA3,0X69,0X9B,0X49,0X9B,0X08,0X9B,0X29,0XAB,0X49,
0XB3,0X68,0XBB,0X68,0XC3,0X67,0XC3,0X67,0XC3,0X67,0XCB,0X67,0XCB,0X86,0XD3,0X86,
0XEC,0X49,0XDB,0XA5,0XC2,0XE2,0XBA,0X40,0XBA,0X80,0XDB,0X63,0XEB,0XA4,0XD3,0X02,
0XCA,0XC2,0XD3,0X03,0XE3,0X63,0XE3,0X83,0XDB,0X83,0XDB,0XC4,0XD4,0X27,0XCC,0X89,
0XF6,0X72,0XF7,0X35,0XEF,0X35,0XD6,0XB3,0XD6,0XD3,0XE7,0X13,0XE7,0X11,0XE6,0XD1,
0XFF,0XB5,0XC5,0X6D,0X83,0X45,0X9C,0X08,0XE6,0X0F,0XA4,0X06,0XD5,0XAC,0XCD,0XCC,
0X83,0XA5,0X94,0X09,0X94,0X0B,0X93,0XEC,0X72,0XE7,0X5A,0X02,0X93,0X86,0XCD,0X2B,
0XA3,0X02,0XD4,0X45,0XBB,0XE2,0XE5,0X06,0XEC,0XA4,0XDC,0X00,0XEC,0X61,0XD4,0X00,
0XD4,0XC3,0XBC,0XC5,0XA4,0X67,0XD6,0X92,0X6B,0XAA,0X19,0XA5,0X7D,0XB5,0X86,0X5A,
0X65,0X78,0X7D,0XD9,0X8D,0XD5,0XCF,0X36,0XCE,0XB1,0XF7,0X72,0XD6,0X0D,0XEE,0X91,
0XFF,0X15,0XE6,0X92,0XCE,0X0D,0XCE,0X6D,0XC6,0X0B,0XCE,0X4E,0XC5,0XD1,0XAD,0XB3,
0XA6,0XB8,0XA7,0X3B,0X9E,0X7A,0X85,0X98,0X75,0X78,0X6D,0XB9,0X5D,0X99,0X5D,0XB9,
0X7E,0X7C,0X66,0X1A,0X4D,0XDA,0X4E,0X1B,0X46,0X3D,0X46,0X1E,0X4D,0XDF,0X4D,0XBF,
0X4D,0XDF,0X45,0XDE,0X4D,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,
0X45,0XBE,0X45,0XDE,0X45,0XBE,0X45,0XDE,0X45,0XBE,0X45,0XDE,0X45,0XBE,0X45,0XBD,
0X4D,0XBD,0X4D,0XBD,0X4D,0XBE,0X45,0XBE,0X4D,0XBE,0X4D,0XBD,0X55,0XBC,0X4D,0XBC,
0X4D,0XFE,0X45,0XDE,0X3D,0XDF,0X46,0X1F,0X5E,0X1E,0X5D,0X9A,0X7D,0X97,0XAE,0X56,
0XC6,0X94,0XCE,0X71,0XC5,0XEF,0XCE,0X51,0XAD,0X90,0X95,0X71,0X85,0XB5,0X75,0XB6,
0X83,0XA8,0X73,0X26,0X52,0X22,0X49,0XE1,0X41,0XA1,0X39,0X80,0X4A,0X03,0X52,0X24,
0X6A,0XE7,0X6B,0X08,0X5A,0XA7,0X6B,0X29,0X8B,0XEC,0X94,0X6E,0XA4,0XF1,0XB5,0X52,
0XC5,0XD5,0XCE,0X36,0XDE,0XB8,0XD6,0X77,0XF7,0X7B,0XDE,0XF9,0XE7,0X3A,0XEF,0X9B,
0XA5,0X73,0X9D,0X31,0X95,0X31,0X7C,0X2D,0X74,0X0C,0X8D,0X10,0X95,0X51,0X95,0X11,
0X8C,0X91,0X6B,0X4C,0X4A,0X69,0X63,0X2B,0X6B,0X4B,0X73,0X8C,0X62,0XE9,0X5A,0XC7,
0X4A,0X45,0X52,0X85,0X4A,0X44,0X4A,0X02,0X52,0X43,0X5A,0X84,0X5A,0XA4,0X6A,0XE4,
0X7B,0X46,0X9C,0X6A,0X8B,0XC7,0X93,0XE8,0XA4,0X6A,0X9C,0X09,0X93,0XC8,0XA4,0X2A,
0XC5,0X0E,0XA4,0X0A,0X93,0X89,0X9B,0XCA,0X9B,0XAA,0X9B,0X8A,0X9B,0X8B,0X9B,0X8A,
0XA3,0X89,0XAB,0X89,0XAB,0XA9,0XB3,0XA8,0XB3,0X88,0XB3,0X67,0XB3,0X46,0XBB,0X45,
0XD4,0X08,0XBB,0X24,0XAA,0X81,0XB2,0XA1,0XC3,0X01,0XD3,0X63,0XD3,0X63,0XCA,0XE1,
0XE3,0XA5,0XEB,0XC6,0XEB,0XC5,0XDB,0X63,0XDB,0X83,0XE4,0X06,0XDC,0X26,0XC3,0XE6,
0XAB,0XC6,0XDD,0XCF,0XF7,0X15,0XEF,0X35,0XEF,0X34,0XFF,0X95,0XFF,0XB4,0XFF,0XB4,
0XF7,0X33,0XFF,0X55,0XCD,0X8E,0XCD,0X8E,0XA4,0X48,0XC5,0X2B,0XD5,0XEE,0XAC,0XA9,
0X9C,0X49,0XA4,0X6B,0X8B,0XAA,0X7B,0X28,0X51,0XA1,0X6A,0X83,0XBC,0XCA,0XC4,0X67,
0XDB,0XA3,0XFC,0X84,0XEC,0X23,0XF4,0X63,0XFC,0X22,0XFC,0X82,0XFC,0X00,0XFC,0XA2,
0XED,0X04,0XC4,0X64,0XCD,0X4B,0XCD,0XB0,0X41,0XE4,0X19,0X64,0X43,0X8D,0X85,0XF7,
0XA6,0XBC,0XA6,0X79,0XDF,0X9B,0XEF,0XB8,0XBD,0XCD,0XF7,0X11,0XB4,0XE9,0XDE,0X30,
0XFF,0X56,0XDE,0X93,0XC6,0X0F,0XD6,0X8F,0XCE,0X2C,0XD6,0X2D,0XC5,0X4D,0XAD,0X0E,
0X85,0X4F,0X96,0X13,0X9D,0X73,0XAD,0XD5,0X9E,0X17,0X9E,0XBA,0X96,0X9A,0XA7,0X5E,
0X86,0X5A,0X6E,0X3A,0X56,0X1A,0X45,0XFA,0X45,0XFC,0X45,0XFE,0X4D,0XFF,0X4D,0XDF,
0X45,0XDF,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,
0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDD,
0X45,0XDC,0X45,0XDC,0X45,0XBE,0X45,0XBE,0X4D,0XBE,0X4D,0XBD,0X55,0XDC,0X4D,0XDC,
0X45,0XFE,0X3D,0XFF,0X35,0XFF,0X46,0X1F,0X5E,0X1F,0X65,0XBA,0X8D,0XF6,0XBE,0XD6,
0XEF,0X96,0XEF,0X54,0XEE,0XF2,0XF7,0X54,0XD6,0X92,0XAD,0XF2,0X9E,0X35,0X8E,0X16,
0X52,0X64,0X4A,0X03,0X39,0XA1,0X62,0XC6,0X7B,0XAA,0X83,0XEB,0XA4,0XEF,0XB5,0X71,
0XAD,0X10,0XAD,0X31,0X9C,0XAF,0XAD,0X52,0XCE,0X56,0XDE,0XB8,0XF7,0X9C,0XFF,0XFD,
0XFF,0XFE,0XFF,0XDD,0XF7,0XBD,0XEF,0X7C,0XFF,0XFE,0XF7,0XDD,0XF7,0XFE,0XFF,0XFE,
0XB6,0X36,0XAD,0XD5,0XA5,0XB4,0X8C,0XF1,0X84,0XD1,0X9D,0XD5,0XA6,0X16,0XA5,0XF5,
0X9D,0X74,0X7C,0X50,0X73,0XEE,0X9D,0X33,0XAD,0X94,0XB5,0XF6,0XA5,0X73,0X9D,0X32,
0X9C,0XF2,0X8C,0X8F,0X63,0X4A,0X42,0X46,0X52,0X87,0X63,0X09,0X63,0X09,0X5A,0XC8,
0X4A,0X45,0X39,0X61,0X41,0XC3,0X52,0X44,0X5A,0X64,0X5A,0X64,0X83,0X68,0X7B,0X06,
0X83,0X67,0X7A,0XE5,0X7B,0X06,0X8B,0X88,0X93,0XA9,0X93,0XA9,0X93,0XA9,0X93,0XA9,
0X9C,0X0B,0XA4,0X0B,0XA4,0X2B,0XA4,0X0B,0XA3,0XEA,0XA3,0XC9,0XA3,0XA8,0XAB,0XA8,
0XAB,0XA7,0XA3,0X04,0XA2,0XE3,0XAB,0X44,0XB3,0X64,0XBB,0X84,0XC3,0X85,0XBB,0X64,
0XC3,0X85,0XCB,0XA5,0XCB,0X64,0XC3,0X43,0XD3,0XA4,0XE4,0X47,0XE4,0X67,0XCC,0X06,
0X9B,0X03,0XD5,0X0C,0XFE,0XB3,0XFE,0XF4,0XF6,0XD3,0XE6,0X71,0XDD,0XEF,0XD5,0XCE,
0XC5,0X8D,0X8B,0XC6,0XFF,0X34,0XD5,0XEF,0XAC,0X8A,0XC5,0X6D,0X6A,0XC3,0X7B,0X45,
0X8B,0XA8,0X9C,0X2B,0X9C,0X0B,0X93,0XAA,0X62,0X23,0X9B,0XC8,0XBC,0XA9,0XB3,0XA3,
0XF4,0X43,0XEB,0XE1,0XF4,0X62,0XE4,0X22,0XE3,0XE1,0XF4,0X22,0XE3,0XE1,0XE4,0X43,
0XCC,0X64,0XB4,0X26,0XDD,0XCF,0XA4,0X4C,0X62,0X47,0X39,0X65,0X32,0X07,0X8D,0X54,
0XBE,0X99,0XBE,0X37,0XCE,0X96,0XC5,0XF2,0XC5,0X6E,0XD5,0XEF,0XD5,0XCE,0XDE,0X51,
0XFF,0X56,0XE6,0XB4,0XD6,0X52,0XE6,0XD3,0XDE,0X50,0XE6,0X50,0XD5,0X6E,0XBD,0X2D,
0X95,0X0C,0XA5,0XAF,0XA4,0X6C,0XC5,0X70,0X9D,0X2F,0X95,0X91,0X74,0XAF,0X96,0X16,
0X6C,0XF3,0X6D,0X97,0X56,0X3A,0X46,0X1B,0X3D,0XFC,0X45,0XFE,0X45,0XFF,0X45,0XFF,
0X45,0XFF,0X45,0XFF,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,
0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XDE,0X45,0XFD,
0X45,0XDC,0X45,0XFD,0X45,0XDE,0X45,0XDF,0X4D,0XDF,0X4D,0XDE,0X55,0XFC,0X4D,0XFC,
0X46,0X1E,0X3E,0X3F,0X36,0X1F,0X3E,0X1F,0X5E,0X1F,0X75,0XFA,0XA6,0X56,0XD7,0X15,
0XE7,0X13,0XDE,0XB0,0XDE,0X6F,0XEE,0XD0,0XD6,0X50,0XAD,0X8E,0X9D,0XB2,0X85,0X72,
0X9C,0XCF,0X8C,0X4D,0X84,0X0B,0XB5,0X92,0XD6,0X96,0XD6,0X96,0XEF,0X59,0XF7,0X9A,
0XEF,0X7A,0XF7,0XBC,0XE7,0X1A,0XF7,0XBC,0XFF,0XFE,0XFF,0XFD,0XFF,0XFE,0XF7,0XDD,
0XFF,0XDD,0XEF,0X5B,0XD6,0XB9,0XE7,0X3B,0XFF,0XFE,0XFF,0XFF,0XD7,0X1B,0XAD,0XF6,
0XAE,0X17,0XAD,0XF6,0XAE,0X58,0X9D,0XD6,0X95,0X95,0XA6,0X37,0X9E,0X17,0X9D,0XD6,
0XA5,0XF6,0X8D,0X13,0X84,0XD2,0XA5,0XD6,0XA5,0XB5,0XA5,0XD6,0XA5,0X95,0XAD,0XD6,
0XB6,0X37,0XAD,0XD6,0X84,0X92,0X73,0XEF,0X8C,0XB2,0XA5,0X75,0X95,0X13,0X84,0X50,
0X63,0X0B,0X4A,0X47,0X7B,0XED,0X42,0X26,0X4A,0X25,0X41,0XE3,0X31,0X20,0X39,0X20,
0X39,0X00,0X39,0X20,0X51,0XE1,0X6A,0X84,0X72,0XC4,0X7A,0XE5,0X83,0X26,0X7B,0X06,
0X7B,0X48,0X83,0X49,0X83,0X69,0X8B,0X89,0X8B,0X89,0X93,0XA9,0X9B,0XC9,0XA3,0XC9,
0XAC,0X09,0XAB,0XC8,0XB3,0XC8,0XB3,0XA7,0XA3,0X45,0XA3,0X25,0XAB,0X65,0XAB,0X45,
0XAB,0X44,0XB3,0X65,0XB3,0X64,0XBB,0X64,0XCB,0XC5,0XDC,0X47,0XD4,0X27,0XC3,0XC5,
0XAB,0X44,0XB4,0X07,0XBC,0X6A,0XBC,0X8A,0XBC,0XAB,0XB4,0X8B,0XB4,0X8A,0XBC,0XCB,
0XAC,0X8A,0XB5,0X0C,0XAC,0XCB,0XB5,0X2D,0XAC,0XCC,0X83,0XA8,0X83,0XA8,0X5A,0X43,
0X6A,0XC6,0X6A,0XA5,0X62,0X65,0X62,0X43,0X59,0XC0,0XBC,0XAA,0XBC,0X88,0XBC,0X05,
0XF4,0XE6,0XDB,0XE1,0XE4,0XA4,0XE4,0XC5,0XDC,0X64,0XCB,0XC2,0XDC,0X65,0XBB,0XC4,
0XA3,0XA4,0XB4,0X89,0XDD,0XF0,0X8B,0X68,0X93,0X6A,0X38,0XE1,0X31,0X43,0X84,0X0D,
0XCE,0X55,0XEF,0X18,0X7B,0X69,0X62,0X64,0XEE,0X93,0XBD,0X0C,0XDE,0X31,0XCD,0XAF,
0XA4,0XAC,0X8C,0X0A,0X83,0XC9,0X9C,0X4B,0X93,0XE8,0X93,0XE8,0X93,0X66,0X8B,0X45,
0X9C,0XA8,0XB5,0X0A,0XA3,0XA5,0XD5,0X2B,0XA4,0XC9,0X9D,0X4B,0X7C,0X49,0XAE,0X33,
0X4B,0X4B,0X5C,0XB3,0X5E,0X1A,0X4E,0X5D,0X46,0X1D,0X46,0X1E,0X46,0X1E,0X3D,0XDE,
0X46,0X1F,0X46,0X1F,0X46,0X1F,0X46,0X1F,0X46,0X1F,0X45,0XFE,0X45,0XFE,0X45,0XFE,
0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,0X45,0XFE,
0X3D,0XFC,0X3D,0XFD,0X3D,0XFF,0X45,0XDF,0X4D,0XDF,0X4D,0XFE,0X56,0X1C,0X4E,0X1C,
0X46,0X3E,0X3E,0X7F,0X3E,0X5F,0X3E,0X1F,0X5D,0XFE,0X7D,0XF9,0XAE,0X75,0XDF,0X34,
0XD6,0X70,0XCD,0XEC,0XCD,0XAB,0XE6,0X4D,0XDE,0X4E,0XC5,0XCE,0XBE,0X32,0XAE,0X13,
0XE7,0X38,0XE7,0X39,0XDE,0XD7,0XF7,0XBB,0XF7,0XDB,0XFF,0XFD,0XEF,0X9A,0XF7,0XDC,
0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,0XEF,0X7B,0XFF,0XFE,0XFF,0XFE,0XEF,0X7C,0XF7,0XDD,
0XE7,0X3B,0XD6,0XB8,0XBD,0XF6,0XDF,0X1A,0XD7,0X1A,0XD6,0XFA,0XB6,0X37,0X9D,0X54,
0X84,0XF3,0X84,0XF3,0X7C,0XD3,0X7C,0XF3,0X85,0X55,0X85,0X35,0X7D,0X15,0X85,0X55,
0X95,0XB6,0X95,0X95,0X95,0XB6,0X9D,0XD7,0X9D,0XD7,0X95,0XB6,0X9D,0XD7,0XAE,0X39,
0XA5,0XD7,0X9D,0XB7,0X9D,0X97,0X95,0X56,0X9D,0X76,0XA5,0XB8,0XA5,0XD8,0X9D,0X97,
0X6B,0XAF,0X7C,0X51,0XA5,0X95,0X6B,0X6D,0X5A,0XC9,0X83,0XED,0X7B,0XAB,0X9C,0X4C,
0X8B,0XA9,0X7B,0X47,0X7B,0X26,0X7A,0XE5,0X62,0X42,0X6A,0X63,0X72,0XC4,0X6A,0X64,
0X72,0X85,0X6A,0X85,0X7A,0XC6,0X83,0X07,0X82,0XE7,0X8B,0X27,0X93,0X48,0X93,0X48,
0X93,0X06,0X93,0X06,0X9B,0X27,0XAB,0X88,0XB3,0XC9,0XB3,0XA8,0XB3,0XA8,0XB3,0XC8,
0XAB,0XE7,0XAB,0XC6,0XAB,0XA5,0XAB,0X84,0XB3,0X64,0XB3,0X64,0XAB,0X43,0XAB,0X23,
0XAB,0X65,0XB3,0XC7,0XBC,0X49,0XBC,0X6A,0XAC,0X2A,0XA3,0XE9,0XA3,0XC8,0XA3,0XE9,
0X93,0XE8,0X8B,0XC7,0X62,0XA3,0X7B,0X86,0X62,0XE4,0X6B,0X26,0X52,0X44,0X62,0XE7,
0X6B,0X08,0X6A,0XE7,0X72,0XE7,0X7B,0X06,0XAC,0X4A,0XAC,0X28,0X82,0XE1,0XA3,0XC4,
0XAB,0XA3,0XB4,0X04,0XA4,0X23,0X9C,0X24,0XAC,0X45,0XB4,0X67,0XB4,0X88,0XBC,0XCA,
0X9C,0XA9,0XB5,0X8D,0X9C,0X89,0X83,0X45,0X82,0XE5,0X61,0XE1,0X49,0X40,0X72,0XA4,
0XE6,0X32,0XE6,0X11,0X59,0XE1,0X83,0X05,0XF6,0XB4,0XE6,0X52,0XB4,0XED,0XE6,0X93,
0X73,0X05,0X83,0XC8,0X41,0X80,0X62,0X84,0X39,0X20,0X62,0X43,0X30,0XE0,0X93,0XA6,
0XD5,0XCC,0XAC,0X03,0XE4,0XC7,0XDC,0X85,0XC5,0X46,0XB5,0X47,0X9C,0XC8,0XB5,0XF0,
0X19,0X63,0X6C,0XD4,0X5D,0XDA,0X45,0XBB,0X4E,0X1E,0X46,0X1E,0X46,0X5F,0X3E,0X1E,
0X46,0X3F,0X46,0X1E,0X3D,0XFE,0X3D,0XFE,0X46,0X1E,0X46,0X1E,0X46,0X1E,0X45,0XFE,
0X45,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFD,
0X35,0XFC,0X3E,0X3E,0X3E,0X3F,0X45,0XFF,0X45,0XFF,0X45,0XDD,0X45,0XDB,0X56,0X5D,
0X46,0X5E,0X46,0X9F,0X3E,0X7F,0X3D,0XFF,0X5D,0XDD,0X96,0X9A,0XD7,0X58,0XF7,0XB5,
0XEF,0X12,0XFF,0X31,0XFF,0X31,0XF6,0X8E,0XF6,0XAF,0XD6,0X0E,0XCE,0X71,0XB6,0X11,
0XFF,0XFC,0XFF,0XFD,0XF7,0XBB,0XFF,0XFD,0XFF,0XFC,0XFF,0XFD,0XF7,0XDC,0XFF,0XFD,
0XDF,0X19,0XD6,0XF8,0XE7,0X3A,0XDF,0X19,0XDF,0X1A,0XE7,0X7B,0XE7,0X5B,0XE7,0X5B,
0XEF,0X7B,0XE7,0X3A,0XD6,0XD9,0XF7,0XFE,0XFF,0XFF,0XF7,0XFF,0XEF,0XFE,0XD7,0X3C,
0XC6,0XDB,0XB6,0X7A,0XA6,0X19,0X9D,0XD8,0X9D,0XF9,0X95,0XB8,0X8D,0X77,0X8D,0X98,
0X74,0XD4,0X7C,0XF5,0X7D,0X15,0X7D,0X15,0X7D,0X15,0X85,0X36,0X85,0X36,0X85,0X36,
0X95,0XB8,0X95,0XB8,0X95,0X97,0X95,0X77,0X95,0X97,0XA5,0XF9,0XA6,0X19,0XAE,0X19,
0XA5,0XF9,0X95,0X56,0XA5,0XB7,0X94,0XF3,0X3A,0X27,0X21,0X43,0X4A,0X25,0X39,0X82,
0X62,0X85,0X7B,0X48,0X7B,0X27,0X83,0X47,0XA4,0X2B,0XA4,0X0A,0X93,0X88,0XA4,0X0A,
0X93,0X88,0X8B,0X68,0X8B,0X48,0X8B,0X27,0X83,0X06,0X82,0XE6,0X82,0XC5,0X82,0XC5,
0X8B,0X06,0X8A,0XE6,0X8A,0XE6,0X93,0X06,0X9B,0X27,0X93,0X06,0X9B,0X26,0X9B,0X47,
0X9B,0XA7,0X9B,0XA6,0X9B,0XA6,0XA3,0XA6,0XAB,0X85,0XAB,0X65,0XA3,0X44,0XA3,0X24,
0XA3,0X24,0X92,0XC3,0X69,0XE0,0X51,0X20,0X59,0XC0,0X83,0X26,0X8B,0X88,0X7B,0X06,
0X93,0XE9,0X9C,0X4A,0X83,0XC8,0X94,0X6B,0X73,0X88,0X6B,0XA9,0X52,0XC7,0X6B,0X6A,
0X6B,0X4A,0X52,0X45,0X52,0X03,0X41,0X60,0X51,0XC0,0X93,0X85,0XB4,0X68,0XBC,0XA9,
0XBC,0X47,0XBC,0XA8,0XB5,0X08,0XB5,0X29,0XBD,0X6A,0XB5,0X0A,0XAC,0X8A,0X9C,0X6A,
0X9C,0XCB,0XA5,0X2C,0X8C,0X27,0X8B,0XA5,0X93,0X65,0X7A,0X82,0X51,0XA0,0X72,0X82,
0XD5,0X2C,0XCC,0XAA,0XB4,0X47,0XE5,0XED,0XE6,0X0F,0XF6,0XB3,0XF6,0XF5,0XBD,0X4F,
0XFF,0X36,0XAC,0XAB,0X73,0X04,0X9C,0X49,0X41,0XA0,0XA4,0XAC,0X94,0X0A,0XAC,0XAA,
0XCC,0XE8,0XBB,0XA1,0XEC,0X84,0XE4,0X83,0XCD,0X02,0XB4,0XE3,0XBD,0X68,0X8C,0X27,
0X18,0XE0,0X5B,0XAF,0X6D,0X99,0X5D,0XFD,0X4D,0XFE,0X45,0XFE,0X3E,0X5E,0X36,0X3D,
0X46,0X3E,0X3E,0X1E,0X3D,0XFE,0X3D,0XFE,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3D,0XFE,
0X3E,0X1E,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3E,0X1E,
0X35,0XFD,0X3E,0X5E,0X35,0XFF,0X35,0XFF,0X45,0XFF,0X45,0XFE,0X4E,0X1C,0X56,0X5D,
0X3D,0XFC,0X46,0X3F,0X46,0X3F,0X4D,0XFF,0X65,0XFB,0X96,0X58,0XC6,0X94,0XDE,0X90,
0XCD,0XCD,0XC5,0X6B,0XCD,0X69,0XEE,0X2C,0XEE,0X2C,0XD5,0XEC,0XB5,0X8D,0XB5,0XCE,
0XE7,0X5A,0XEF,0XBB,0XE7,0X5A,0XDF,0X39,0XD6,0XD8,0XD6,0XF8,0XCE,0X97,0XD6,0XD8,
0XF7,0XDC,0XE7,0X5A,0XFF,0XFE,0XFF,0XFE,0XE7,0X7B,0XF7,0XDD,0XFF,0XFD,0XEF,0XBC,
0XF7,0XDC,0XF7,0XDC,0XE7,0X5A,0XFF,0XFE,0XEF,0XDD,0XEF,0XDE,0XE7,0X7D,0XC6,0XDA,
0XAE,0X18,0XA5,0XF8,0X9D,0XD8,0XA6,0X19,0XA6,0X1A,0XA6,0X3B,0XA6,0X3B,0XA6,0X5B,
0X9E,0X1B,0XA6,0X5D,0XA6,0X3C,0X95,0XDA,0X95,0XB9,0X95,0XD9,0X8D,0X99,0X85,0X57,
0X7C,0XF5,0X7D,0X15,0X85,0X35,0X7D,0X35,0X85,0X35,0X8D,0X55,0X8D,0X76,0X8D,0X96,
0X95,0X76,0X95,0XB6,0XA5,0XF7,0XB6,0X37,0X84,0X70,0X4A,0X88,0X4A,0X46,0X4A,0X45,
0X39,0X82,0X39,0X41,0X41,0X41,0X41,0X61,0X49,0X62,0X59,0XC4,0X6A,0X46,0X6A,0X66,
0X8B,0X88,0X9B,0XC8,0X9B,0XE9,0XA4,0X0A,0XAC,0X4B,0XAC,0X4A,0XA4,0X09,0XA4,0X09,
0X93,0X67,0X8B,0X47,0X8B,0X26,0X8B,0X26,0X82,0XE5,0X7A,0XA4,0X7A,0XA4,0X7A,0XA4,
0X8B,0X25,0X8B,0X45,0X93,0X45,0X93,0X45,0X93,0X45,0X9B,0X25,0X9B,0X24,0X9B,0X25,
0X93,0X04,0X93,0X45,0X82,0XE4,0X72,0X83,0X9C,0X09,0XDE,0X52,0XEE,0XF5,0XCE,0X12,
0XF7,0X17,0XFF,0X79,0XDE,0XF5,0XD6,0XD5,0X9D,0X50,0X8C,0XEF,0X7C,0X2E,0X94,0XF2,
0XA5,0X32,0X94,0X4E,0X7B,0X69,0X49,0XC0,0X49,0X80,0X8B,0X64,0XA4,0X27,0X9B,0XA6,
0XB3,0XE8,0XB4,0X08,0XA4,0X27,0XAC,0X87,0XBC,0XEA,0XC4,0XEA,0XB4,0X6A,0XAC,0X6A,
0XB5,0X4D,0XBD,0X8D,0XA4,0XC9,0XAC,0XA9,0X93,0XA5,0X83,0X03,0X62,0X81,0X83,0X44,
0XB4,0X07,0XE5,0X6B,0XD5,0X47,0XBC,0XC4,0X8B,0X40,0XA4,0X26,0XDD,0XF1,0XDD,0XD1,
0XB4,0XCC,0XDE,0X30,0X93,0XE5,0XD6,0X0D,0XA4,0X88,0X9C,0X69,0X9C,0X6A,0XCD,0X8E,
0XCC,0X86,0XDC,0X42,0XFC,0XE4,0XFD,0X04,0XD5,0X02,0XBD,0X02,0XDE,0X8B,0X6B,0X02,
0X41,0XA2,0X52,0XCB,0X75,0X58,0X76,0X3D,0X4D,0XDC,0X46,0X1D,0X3E,0X5D,0X36,0X3D,
0X3E,0X3E,0X3E,0X1E,0X3E,0X1E,0X3D,0XFE,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,
0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3D,0XFE,0X3E,0X1E,
0X35,0XFD,0X46,0X9F,0X2D,0XFE,0X35,0XFF,0X46,0X3F,0X46,0X1E,0X56,0X5D,0X4E,0X1B,
0X56,0X5D,0X56,0X5E,0X56,0X3E,0X5E,0X3D,0X86,0X5B,0XB6,0XF9,0XDF,0X15,0XEF,0X12,
0XC5,0XAC,0XFF,0X52,0XF6,0X8E,0XFE,0XCE,0XF6,0X4D,0XD5,0XCB,0XE6,0X90,0XC5,0XEE,
0XD6,0XD8,0XE7,0X7A,0XEF,0X9B,0XEF,0XBB,0XF7,0XFD,0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,
0XF7,0XDC,0XEF,0X9C,0XFF,0XFE,0XFF,0XFD,0XEF,0X7B,0XF7,0XBC,0XEF,0X9B,0XE7,0X5B,
0XE7,0X5A,0XE7,0X5A,0XD6,0XF9,0XEF,0X9C,0XDF,0X5B,0XE7,0X7C,0XDF,0X5C,0XC6,0XBA,
0XA5,0X96,0X9D,0XB7,0X9D,0XB7,0XA6,0X19,0X95,0X97,0X95,0X77,0X85,0X37,0X85,0X37,
0X8D,0X79,0X95,0XBA,0X95,0XFA,0X95,0XDA,0X95,0XDA,0X9D,0XFA,0X9E,0X1A,0X9E,0X3A,
0XA6,0X5B,0XAE,0X7B,0XA6,0X5A,0XA6,0X39,0X9D,0XF8,0X95,0XB7,0X8D,0X56,0X85,0X15,
0X85,0X35,0X7C,0XF4,0X8D,0X35,0X84,0XF3,0X7C,0X70,0X84,0X4F,0X6B,0X6B,0X94,0X6F,
0X94,0X6E,0X8B,0XCC,0X94,0X0D,0X93,0XEC,0X6A,0XA7,0X5A,0X46,0X62,0X66,0X51,0XE4,
0X39,0X00,0X49,0X80,0X51,0XC0,0X5A,0X42,0X7B,0X05,0X83,0X66,0X83,0X87,0X93,0XC8,
0XA4,0X4A,0XA4,0X6A,0XA4,0X6B,0XAC,0X6B,0XA4,0X6A,0XA4,0X2A,0X9C,0X29,0X9C,0X09,
0X8B,0X87,0X8B,0X66,0X8B,0X25,0X83,0X05,0X83,0X05,0X8B,0X05,0X8B,0X05,0X8B,0X05,
0X9B,0XA7,0X8B,0X05,0X6A,0X42,0X6A,0XA3,0XA4,0XCC,0XF7,0X76,0XFF,0XFA,0XFF,0XD9,
0XFF,0X79,0XFF,0XFC,0XF7,0X9A,0XDF,0X58,0XAE,0X34,0XA5,0XF5,0X9D,0XB5,0XB6,0X58,
0XAD,0XD6,0XA5,0X53,0X6B,0X28,0X39,0XA0,0X6A,0XC3,0X9C,0X28,0XAC,0X69,0XBC,0XCB,
0XB4,0X49,0XB4,0X28,0X9C,0X07,0X9C,0X27,0XA4,0X48,0XA4,0X07,0X9B,0XA6,0X9B,0XC7,
0XD6,0X30,0X9C,0XA9,0X6A,0XE2,0X7B,0X04,0X83,0X04,0X9B,0XE8,0X73,0X04,0X52,0X00,
0XA3,0XC7,0XED,0XAC,0XDD,0X66,0XDD,0XC6,0XBC,0XC4,0X7A,0X80,0X7A,0XA3,0XEE,0X12,
0XBC,0XAB,0XF6,0XB1,0XFF,0X92,0X83,0X82,0XA4,0XC8,0X94,0X27,0X94,0X28,0XC5,0X2C,
0XB3,0XA2,0XD4,0X02,0XE4,0X42,0XDC,0XA2,0XBC,0X60,0XBD,0X04,0XD6,0X2A,0X6A,0XE2,
0X6A,0XC6,0X42,0X07,0X74,0X94,0X7D,0XFB,0X4D,0X9B,0X46,0X3D,0X46,0X7E,0X36,0X1D,
0X3E,0X3E,0X3E,0X3E,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,
0X3E,0X1E,0X3E,0X1E,0X3E,0X1E,0X36,0X1E,0X3E,0X1E,0X36,0X1E,0X3E,0X1E,0X3E,0X1E,
0X35,0XDC,0X4E,0XBF,0X35,0XFE,0X3E,0X3F,0X4E,0X7F,0X4E,0X3D,0X5E,0X7D,0X4D,0XDA,
0X5E,0X3C,0X5E,0X1B,0X5D,0XDA,0X65,0XD9,0X8E,0X18,0XB6,0X76,0XCE,0X72,0XD6,0X2F,
0XDE,0X2F,0XC5,0X4B,0XB4,0X46,0XD5,0X28,0XF6,0X4D,0XCD,0X28,0XBD,0X2A,0XBD,0X6C,
0XF7,0XDC,0XFF,0XFD,0XFF,0XFE,0XF7,0XBC,0XF7,0XDD,0XEF,0X9C,0XE7,0X5A,0XDF,0X1A,
0XDE,0XF9,0XDF,0X1A,0XE7,0X3A,0XD6,0XD9,0XDE,0XFA,0XDF,0X1A,0XCE,0X57,0XD6,0XB8,
0XEF,0X9C,0XEF,0X9C,0XE7,0X3A,0XFF,0XFE,0XEF,0XBD,0XF7,0XFE,0XF7,0XFE,0XE7,0X9D,
0XB6,0X38,0XB6,0X59,0XB6,0X5A,0XBE,0X9B,0X8D,0X35,0X84,0XF4,0X74,0X93,0X74,0X93,
0X7C,0XF3,0X7D,0X14,0X85,0X55,0X8D,0XB6,0X85,0X75,0X74,0XF4,0X7C,0XF4,0X85,0X56,
0X8D,0X57,0X8D,0X57,0X8D,0X78,0X9D,0XD9,0XA6,0X1B,0XAE,0X5C,0XAE,0X5C,0XAE,0X7D,
0XC7,0X3F,0X95,0XBA,0XA6,0X1B,0XA5,0XB9,0X4B,0X0D,0X4A,0XCB,0X4A,0X89,0X5A,0XC9,
0X41,0XE4,0X5A,0X86,0X5A,0XA6,0X6B,0X07,0X8C,0X2C,0X8C,0X2C,0X83,0XEB,0X94,0X8D,
0X9C,0XCD,0X9C,0XAC,0X8C,0X2A,0X7B,0XC9,0X7B,0XA8,0X73,0X67,0X6B,0X26,0X6B,0X26,
0X5A,0X83,0X5A,0XA4,0X6A,0XE5,0X6B,0X25,0X73,0X46,0X7B,0X87,0X83,0XA7,0X83,0XC7,
0XAC,0XAB,0XA4,0X6A,0X9C,0X09,0X93,0XC8,0X93,0XC8,0X9B,0XC8,0X93,0XA8,0X93,0X87,
0X83,0X05,0X8B,0X66,0X8B,0X87,0X7B,0X46,0X6B,0X05,0X7B,0XE8,0XBE,0X11,0XFF,0XFA,
0XFF,0XBC,0XFF,0XFD,0XFF,0XFC,0XDF,0X79,0XAE,0X76,0X9E,0X15,0X85,0X75,0X95,0X96,
0X9D,0XB6,0X84,0X70,0X4A,0X64,0X4A,0X42,0X8B,0XE7,0X94,0X28,0X8B,0XC7,0X9C,0X29,
0XA4,0X68,0XAC,0XC9,0XA4,0XEA,0XAD,0X2B,0XBD,0X6C,0XBD,0X2C,0XBD,0X0B,0XC5,0X6C,
0XBD,0XAD,0X6B,0X24,0X39,0X60,0X41,0X80,0X59,0XC2,0X9B,0XCA,0X83,0XA8,0X39,0X60,
0X51,0X60,0XCD,0X0A,0XEE,0X29,0XED,0XE6,0XDD,0X24,0XD4,0XC6,0X9B,0X24,0X72,0X01,
0XFF,0X14,0XD5,0XAD,0XB4,0XE8,0XA4,0X86,0X93,0XE4,0XCD,0XAD,0XD5,0XCE,0XA3,0XE6,
0XAB,0X81,0XD4,0X43,0XCC,0X43,0XC4,0X84,0XB4,0X83,0XBD,0X68,0XA5,0X08,0X7B,0XE6,
0X62,0X84,0X29,0X02,0X63,0X8E,0X8D,0XB9,0X55,0XB9,0X4E,0X3C,0X46,0X7E,0X36,0X3E,
0X36,0X3E,0X3E,0X3E,0X3E,0X3E,0X3E,0X3E,0X3E,0X1E,0X36,0X1E,0X3E,0X1E,0X36,0X1E,
0X3E,0X3E,0X36,0X3E,0X3E,0X1E,0X36,0X1E,0X36,0X1E,0X36,0X1E,0X3E,0X1E,0X3E,0X1E,
0X35,0XDC,0X4E,0X7E,0X36,0X1D,0X3E,0X3D,0X46,0X5E,0X56,0X5D,0X6E,0X9E,0X6E,0X5C,
0X65,0X99,0X6D,0XB8,0X75,0XF8,0X96,0X78,0XB6,0XF8,0XD7,0X16,0XDE,0X92,0XDE,0X0E,
0XDE,0X30,0XEE,0X90,0XFE,0XF0,0XDD,0X8A,0XDD,0X69,0XFE,0XCF,0XCD,0X8B,0XDE,0X4F,
0XD6,0XB9,0XDE,0XF9,0XDE,0XD9,0XCE,0X57,0XD6,0XB9,0XCE,0X98,0XD6,0XB8,0XD6,0XB9,
0XEF,0X7C,0XFF,0XDD,0XF7,0XBD,0XE7,0X3A,0XF7,0X9C,0XEF,0X9C,0XD6,0XB9,0XE7,0X1A,
0XFF,0XFE,0XFF,0XFE,0XEF,0X5C,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XEF,0XBE,
0XBE,0X38,0XBE,0X79,0XC6,0X9A,0XCE,0XFB,0X95,0X34,0X95,0X55,0X95,0X55,0X9D,0XB6,
0XB6,0X99,0XAE,0X78,0XAE,0X99,0XB6,0X99,0X9D,0XD6,0X74,0XB2,0X64,0X31,0X74,0X72,
0X9D,0XF9,0X95,0X98,0X8D,0X37,0X84,0XF6,0X7C,0XB5,0X74,0X95,0X7C,0XB5,0X7C,0XD6,
0X85,0X18,0X95,0X79,0X8D,0X58,0XBE,0X9C,0X6C,0X11,0X42,0X69,0X63,0X6C,0X7C,0X0E,
0X84,0X2D,0X6B,0X8A,0X5A,0XE7,0X52,0XC7,0X5B,0X08,0X52,0XE7,0X4A,0XA6,0X52,0XA7,
0X42,0X24,0X42,0X24,0X52,0XC7,0X6B,0X89,0X73,0XCA,0X84,0X2B,0X8C,0X6C,0X84,0X2B,
0X94,0X8C,0X94,0X8C,0X8C,0X6B,0X83,0XEA,0X7B,0XA8,0X73,0X87,0X73,0X87,0X7B,0X67,
0X62,0XA3,0X6A,0XC3,0X6A,0XE4,0X7B,0X46,0X8B,0XA8,0X93,0XE9,0X94,0X09,0X93,0XE9,
0X93,0XC8,0X83,0X26,0X83,0X46,0X93,0XE8,0X94,0X4A,0X8C,0X4A,0X9C,0XCC,0XB5,0X91,
0XE6,0XB9,0XF7,0X7D,0XE7,0X5B,0XBE,0XB7,0X9E,0X15,0X96,0X16,0X95,0XF8,0X9E,0X39,
0XA5,0XF8,0X63,0XCD,0X52,0XE6,0X8C,0X6A,0XAD,0X4D,0XAD,0X0C,0XAC,0XCC,0X94,0X29,
0X9C,0X68,0X94,0X67,0X84,0X28,0X73,0XC8,0X73,0X47,0X6A,0XC4,0X6A,0XC2,0X7B,0X64,
0XA5,0X2B,0X52,0XA2,0X41,0XE2,0X49,0X83,0X49,0X42,0X83,0X29,0X9C,0X4B,0X52,0X22,
0X41,0X40,0X83,0X25,0XBC,0XA5,0XED,0XC7,0XED,0X66,0XE4,0XE7,0XB3,0X44,0X8A,0X62,
0X82,0X61,0XE5,0XCE,0X93,0XE5,0XFF,0X73,0XBC,0XEB,0XB4,0XA9,0XE5,0XAD,0XDD,0X2A,
0XD4,0XC6,0XED,0X88,0XDD,0X48,0XCD,0X29,0XCD,0XAB,0XC5,0XED,0X84,0X67,0X94,0XAA,
0X62,0X84,0X39,0X21,0X6B,0X2B,0X9D,0XB7,0X6E,0X1A,0X45,0XFB,0X3E,0X1E,0X35,0XFE,
0X36,0X1E,0X3E,0X3E,0X3E,0X3E,0X3E,0X3E,0X3E,0X3E,0X36,0X3E,0X36,0X3E,0X36,0X3E,
0X3E,0X3E,0X3E,0X3E,0X36,0X3E,0X36,0X1E,0X36,0X1E,0X36,0X1E,0X36,0X1E,0X3E,0X1D,
0X3D,0XFC,0X4E,0X3D,0X46,0X5D,0X36,0X1C,0X3E,0X1C,0X4E,0X3C,0X6E,0X9D,0X9F,0X3F,
0X75,0XB8,0X7D,0XB6,0X85,0XF5,0XA6,0X55,0XBE,0XB5,0XDE,0XD3,0XE6,0X71,0XE6,0X0F,
0XDE,0X30,0XEE,0XB2,0XB4,0X47,0XEE,0X0C,0XEE,0X0C,0XEE,0X0C,0XBC,0XE9,0XBD,0X2A,
0XFF,0XDE,0XFF,0XDE,0XFF,0XDE,0XEF,0X5B,0XEF,0X7C,0XDE,0XFA,0XE7,0X3B,0XEF,0X7C,
0XFF,0XDE,0XFF,0XFE,0XFF,0XFE,0XFF,0XDD,0XFF,0XDD,0XFF,0XFE,0XF7,0X7C,0XFF,0XBD,
0XFF,0XBE,0XE7,0X1B,0XD6,0X99,0XFF,0XDE,0XFF,0XDE,0XFF,0XFF,0XFF,0XFF,0XF7,0XFE,
0XC6,0X79,0XC6,0X99,0XCE,0XBA,0XD7,0X5C,0X9D,0X54,0X9D,0X95,0X9D,0X95,0XA5,0XB6,
0XBE,0XBA,0XB6,0X79,0XAE,0X59,0XAE,0X59,0XA5,0XF8,0X95,0X76,0X8D,0X55,0X95,0X76,
0XB6,0X7A,0XAE,0X59,0XA5,0XF8,0X9D,0X97,0X8D,0X35,0X84,0XF4,0X8D,0X35,0X95,0X97,
0X8D,0X36,0XB6,0X9B,0X8D,0X35,0X9D,0X96,0X53,0X4C,0X21,0X85,0X3A,0X26,0X63,0X8B,
0X8C,0XB0,0X6B,0XAC,0X73,0XED,0X74,0X2E,0X5B,0X4C,0X63,0X8C,0X74,0X50,0X74,0X2F,
0X73,0XEE,0X5B,0X2B,0X6B,0X8C,0X6B,0XAC,0X5A,0XE9,0X5B,0X09,0X63,0X2A,0X4A,0X46,
0X52,0X87,0X62,0XE8,0X73,0X49,0X73,0X69,0X7B,0X89,0X8B,0XEB,0X94,0X2C,0X94,0X4B,
0X9C,0X8A,0X9C,0XAA,0X94,0X8A,0X8C,0X4A,0X7B,0XC8,0X73,0X47,0X6B,0X06,0X72,0XE5,
0X83,0X46,0X7B,0X05,0X7B,0X05,0X83,0X46,0X83,0X66,0X83,0X66,0X83,0XC8,0X94,0X2C,
0XDE,0X79,0XFF,0XBF,0XFF,0XFF,0XCF,0X3A,0XA6,0X77,0X96,0X37,0X96,0X59,0XA6,0X7B,
0XB6,0XDB,0X7C,0XB1,0X53,0X07,0X6B,0X87,0X73,0XA6,0X62,0XE4,0X6A,0XE5,0X52,0X43,
0X49,0XC0,0X41,0XA0,0X31,0X61,0X21,0X22,0X28,0XE2,0X28,0XE1,0X39,0X80,0X52,0X82,
0XC6,0X51,0X42,0X42,0X39,0XA3,0X49,0XA5,0X62,0X07,0X9B,0XAB,0XC5,0X6E,0X62,0XA2,
0X5A,0X65,0X9C,0X2B,0XC5,0X09,0XDD,0X27,0XF5,0X07,0XFD,0X49,0XDC,0X07,0XC3,0X86,
0X71,0XA0,0XC4,0XCA,0XE6,0X31,0XB4,0XAC,0XFE,0XD5,0XED,0XF0,0XC4,0X46,0XC4,0X45,
0XDD,0X07,0XD5,0X28,0XC5,0X4A,0XAC,0XC9,0XCE,0X30,0XB5,0X8F,0X84,0XCB,0X95,0X0D,
0X94,0X4B,0X6A,0XA5,0X6A,0XA7,0X95,0X12,0X76,0X39,0X45,0XFA,0X45,0XFE,0X3D,0XFF,
0X3E,0X1E,0X3E,0X3E,0X3E,0X5F,0X3E,0X5E,0X3E,0X3E,0X36,0X3E,0X36,0X3E,0X3E,0X3E,
0X3E,0X3E,0X36,0X3E,0X36,0X3E,0X36,0X1E,0X36,0X1E,0X36,0X1E,0X36,0X1E,0X3E,0X1D,
0X4E,0X1D,0X4E,0X1C,0X5E,0XDF,0X46,0X5C,0X3D,0XFA,0X4E,0X1B,0X6E,0X1B,0XAF,0X9F,
0XC7,0XBF,0XC7,0X5B,0XBE,0XF7,0XC6,0XD5,0XCE,0XB2,0XDE,0X91,0XEE,0X50,0XEE,0X30,
0X9C,0X29,0XFF,0X55,0XD5,0X6B,0XED,0XEC,0XF6,0X2D,0XE5,0XCB,0XF6,0XD0,0XD5,0XCD,
0XEF,0X7C,0XF7,0X9D,0XFF,0XFF,0XF7,0XBD,0XFF,0XFE,0XE7,0X3B,0XF7,0XBD,0XFF,0XFE,
0XEF,0X5C,0XE7,0X1A,0XFF,0XFE,0XFF,0XFE,0XEF,0X5C,0XFF,0XDD,0XFF,0XFF,0XFF,0XFE,
0XFF,0XFF,0XE7,0X3B,0XCE,0X79,0XFF,0XDE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,
0XC6,0X79,0XC6,0X99,0XCE,0XDA,0XE7,0X9D,0XA5,0X95,0XAD,0XD6,0XA5,0X95,0XA5,0X95,
0XAD,0XD7,0X9D,0X97,0X95,0X35,0X8D,0X35,0X95,0X56,0XA5,0XD7,0XAE,0X19,0XB6,0X59,
0XAD,0XF8,0XAE,0X39,0XB6,0X59,0XAE,0X39,0X9D,0XB7,0X95,0X55,0X9D,0XB6,0XAE,0X39,
0XA6,0X18,0XAE,0X39,0XAE,0X58,0XA5,0XD6,0X32,0X27,0X29,0XE6,0X19,0X44,0X10,0XE2,
0X8C,0X91,0X84,0X91,0X63,0X6D,0X32,0X08,0X11,0X45,0X11,0X25,0X3A,0X6A,0X7C,0X92,
0X95,0X34,0X6B,0X8E,0X63,0X6D,0X63,0X4D,0X4A,0X89,0X6B,0X6D,0X94,0XD2,0X8C,0X70,
0X63,0X2B,0X73,0X8C,0X7B,0XED,0X7B,0XAC,0X73,0X4B,0X6B,0X09,0X5A,0XA8,0X52,0X65,
0X63,0X26,0X6B,0X87,0X73,0XC8,0X73,0XE9,0X7B,0XCA,0X7B,0XEA,0X8C,0X4C,0X9C,0XAD,
0X94,0X2B,0XA4,0X8C,0XA4,0X6B,0X8B,0XC8,0X83,0X67,0X83,0XA8,0X7B,0X87,0X6B,0X07,
0X73,0X4B,0XB5,0X95,0XDF,0X5B,0XCF,0X19,0XAE,0X77,0X96,0X16,0X95,0XF8,0X9E,0X19,
0X9D,0XB7,0X9D,0X95,0X6B,0XEB,0X73,0XE9,0X94,0XAB,0X7B,0XA8,0X63,0X06,0X6A,0XE6,
0X62,0X83,0X62,0XA5,0X5A,0XC8,0X63,0X0B,0X73,0X6D,0X73,0X8D,0X84,0X0C,0X9D,0X0E,
0XD6,0XF4,0X42,0X83,0X52,0X66,0X62,0X88,0X7A,0XCA,0XA4,0X2D,0XEE,0XD2,0XA4,0X89,
0X9C,0X8D,0X94,0X0B,0X9B,0XE7,0XB3,0XE5,0XB3,0X63,0XDC,0X47,0XCB,0XC7,0XB3,0X25,
0X9B,0X03,0X69,0XE0,0XC5,0X0E,0XF6,0XB5,0XDD,0XB1,0XC4,0X8C,0X9A,0XE2,0XBC,0X04,
0XBC,0X44,0X9B,0XE3,0X9C,0X66,0X83,0XE6,0XCE,0X31,0XA5,0X4F,0XAD,0XF0,0XB5,0XB0,
0XD6,0X11,0X8B,0XA8,0X51,0XC3,0X73,0XED,0X7D,0XF7,0X55,0XFA,0X56,0X3F,0X46,0X3F,
0X3E,0X1E,0X3E,0X3E,0X46,0X5F,0X46,0X5E,0X3E,0X3E,0X3E,0X3E,0X3E,0X3D,0X3E,0X5D,
0X36,0X5E,0X36,0X3E,0X36,0X3E,0X36,0X1E,0X36,0X1E,0X36,0X1E,0X3E,0X1D,0X3E,0X1D,
0X4E,0X3D,0X4E,0X1C,0X77,0X3F,0X66,0X7D,0X4D,0XD9,0X5D,0XD8,0X65,0X76,0XBF,0X7D,
0XBE,0XD9,0XBE,0X97,0XC6,0X94,0XCE,0X92,0XDE,0X91,0XE6,0X70,0XF6,0X2F,0XF6,0X10,
0XBC,0XCB,0XCD,0X4D,0XFE,0XB1,0XBC,0X67,0XF6,0X4E,0XDD,0XCC,0XCD,0X6B,0XCD,0XCD,
0XEF,0X7C,0XD6,0XB9,0XFF,0XFF,0XEF,0X9D,0XF7,0XBD,0XFF,0XFF,0XFF,0XFF,0XF7,0XDE,
0XF7,0XDE,0XE7,0X7C,0XF7,0XBD,0XEF,0X9D,0XFF,0XFE,0XE7,0X5C,0XAD,0X95,0X84,0X70,
0X8C,0X90,0XEF,0XBC,0XEF,0X7C,0XF7,0X9D,0XF7,0XDE,0XFF,0XFF,0XFF,0XFF,0XE7,0X7E,
0XB6,0X38,0XD7,0X1C,0XD7,0X3C,0XDF,0X9D,0XB6,0X16,0X4A,0XC9,0X42,0X68,0X7C,0X2F,
0XD7,0X1C,0XDF,0X5E,0XB6,0X39,0XB6,0X19,0XA5,0XB7,0X9D,0X76,0XA5,0X97,0XBE,0X5A,
0XB6,0X39,0X95,0X35,0XA5,0XD8,0XA5,0XD8,0X9D,0X97,0XA5,0XD8,0X95,0X76,0X8D,0X35,
0XA5,0XD8,0XC6,0XBB,0XC6,0XDB,0XBE,0X9A,0X32,0X49,0X2A,0X07,0X21,0X86,0X21,0XA7,
0X53,0X0D,0XAD,0XD8,0X42,0X8B,0X21,0XA8,0X21,0XA7,0X29,0XC7,0X29,0XE8,0X32,0X29,
0X9D,0X56,0X7C,0X32,0X32,0X29,0X29,0XE8,0X31,0XE8,0X29,0XA7,0X42,0XAA,0XAD,0XB6,
0X73,0XEF,0X6B,0X8E,0X4A,0XAA,0X31,0XE7,0X63,0X6D,0X9D,0X34,0X73,0XCF,0X6B,0XAE,
0X84,0X8F,0X63,0X8A,0X7C,0X2D,0X94,0XF0,0X5B,0X4A,0X4A,0XA7,0X63,0X49,0X6B,0X8A,
0X6B,0X49,0X73,0XAA,0X6B,0X48,0X6B,0X27,0X7B,0XA9,0X8C,0X6B,0X8C,0X2B,0X84,0X0A,
0X8C,0X4C,0X7B,0XEB,0XA5,0X92,0XE7,0X9A,0XC6,0X97,0XCF,0X1A,0XBE,0X99,0XA5,0XD7,
0XA5,0XF6,0XA5,0XD5,0X84,0X8F,0X84,0X8D,0XCE,0X74,0X7B,0XEA,0X6B,0X47,0X6B,0X47,
0X73,0X68,0X73,0X69,0X63,0X48,0X7C,0X2D,0XA5,0X94,0XB5,0XF6,0XBE,0X35,0XB5,0XD2,
0XDF,0X35,0X5B,0X05,0X52,0X44,0X5A,0X26,0X51,0XA4,0X7A,0XC7,0XE6,0X71,0X9C,0X27,
0X8B,0XA8,0X9C,0X0B,0X83,0X26,0XAC,0X29,0X9B,0X66,0X9B,0X66,0X8A,0XE5,0X9B,0X66,
0X93,0X66,0X82,0XE4,0X49,0X60,0XB4,0X6B,0XCD,0X2E,0X93,0X46,0XC4,0XCA,0XBC,0X67,
0XC4,0XE7,0XCD,0X68,0XB5,0X29,0XCE,0X0E,0XB5,0X8E,0XAD,0X2D,0XAD,0X4D,0XC5,0XD0,
0XDE,0X31,0XA4,0X4A,0X49,0XC2,0X52,0XC8,0X75,0X55,0X76,0X3B,0X45,0X7B,0X4E,0X1F,
0X4E,0X3F,0X3D,0XDC,0X4D,0XFD,0X46,0X1E,0X4E,0X7F,0X46,0X7E,0X35,0XB9,0X46,0X5B,
0X36,0X5D,0X36,0X7F,0X46,0X5F,0X2D,0X9D,0X46,0X7F,0X3E,0X3E,0X35,0XBA,0X46,0X3C,
0X3D,0XDD,0X4D,0XFD,0X7E,0X7F,0XB7,0XBF,0X96,0XBC,0X8E,0X58,0X96,0X15,0XB6,0X35,
0XB5,0XB2,0XCE,0X52,0XCE,0X71,0XC5,0XEE,0XEE,0XB1,0XFE,0XF2,0XCC,0X48,0XC4,0X27,
0XFF,0X54,0X7A,0XA1,0XED,0XEE,0XFE,0X90,0XAC,0X47,0XAC,0X68,0X9C,0X27,0X7B,0X43,
0XEF,0X9C,0XE7,0X5C,0XF7,0XFE,0XEF,0XBD,0XF7,0XDE,0XC6,0X78,0X7C,0X2F,0X84,0X4F,
0XDF,0X1B,0XF7,0XDD,0XF7,0XFE,0XFF,0XFF,0XEF,0XBD,0XD6,0XD9,0X42,0X68,0X29,0XA4,
0X84,0X6E,0XE7,0X7A,0XEF,0X9C,0XFF,0XFE,0XFF,0XFE,0XF7,0XBE,0XF7,0XDF,0XEF,0XBF,
0XC6,0XBB,0XBE,0X79,0XBE,0X99,0XE7,0XFE,0XB6,0X17,0X19,0X43,0X08,0X80,0X63,0X4B,
0XEF,0X9E,0XEF,0XDF,0XC6,0X9A,0XBE,0X59,0X74,0X10,0X32,0X29,0X29,0XC8,0X5B,0X6E,
0XAD,0XD8,0XB6,0X39,0XCF,0X1D,0XB6,0X39,0X74,0X52,0X74,0X32,0X95,0X56,0XB6,0X3A,
0XAE,0X19,0XC6,0X9B,0XC6,0XDB,0XB6,0X59,0X32,0X28,0X21,0XC7,0X21,0X87,0X21,0X87,
0X4A,0XCD,0XA5,0X98,0X42,0X6C,0X21,0XA8,0X21,0XA7,0X21,0XC7,0X21,0XC6,0X29,0XE7,
0X9D,0X76,0X7C,0X52,0X29,0XE8,0X29,0XC8,0X21,0XA7,0X21,0XA7,0X32,0X49,0X95,0X55,
0X7C,0X92,0X32,0X29,0X3A,0X6A,0X32,0X29,0X29,0XE8,0X6B,0XF1,0X95,0X36,0X5B,0X6E,
0X3A,0X68,0X21,0X84,0X4A,0XE9,0X84,0X70,0X84,0X90,0X74,0X0E,0X7C,0X4F,0X73,0XCD,
0X8C,0XB0,0X94,0XF0,0X6B,0XAB,0X73,0XCB,0X84,0X4D,0X84,0X4C,0X8C,0X6C,0X63,0X68,
0X3A,0X03,0X4A,0X64,0XA5,0X50,0XE7,0X79,0XC6,0X77,0XCE,0XD9,0XD7,0X1A,0XB6,0X16,
0XC6,0X98,0XC6,0X98,0XB6,0X15,0XBE,0X35,0XE7,0X59,0X94,0XCE,0X8C,0X4B,0X84,0X0B,
0X83,0XEB,0X7B,0XCB,0X8C,0X8D,0X84,0XAE,0X84,0X90,0X84,0XB1,0X84,0XB0,0X9D,0X51,
0XEF,0XB8,0X7B,0XE8,0X7B,0X69,0X7B,0X6A,0X83,0X2A,0X93,0XEB,0XEE,0XD2,0X83,0X84,
0X73,0X05,0X8B,0X88,0X7B,0X27,0X93,0XC9,0X8B,0X68,0X8B,0X88,0X7B,0X06,0X8B,0X67,
0X83,0X25,0X93,0XA7,0X6A,0X82,0X49,0X60,0XC5,0X0C,0XFE,0XD3,0XB4,0XAA,0XD5,0XAD,
0XB4,0XC7,0XAC,0XA5,0XA4,0XC7,0XBD,0XCD,0XAD,0X2C,0X9C,0XAB,0XA4,0XCB,0XA4,0X8A,
0XAC,0X8B,0X9C,0X29,0X62,0XA5,0X63,0X8A,0X85,0X95,0X86,0X7B,0X4D,0XBB,0X4E,0X1E,
0X46,0X3D,0X46,0X1D,0X4E,0X3D,0X45,0XFC,0X35,0XFD,0X3E,0X1D,0X3D,0XDA,0X4E,0X9C,
0X36,0X5E,0X3E,0X7F,0X56,0X9F,0X35,0X7C,0X3D,0XDE,0X3D,0XDD,0X3D,0XBA,0X4E,0X1A,
0X46,0X3C,0X55,0XDB,0X8D,0XFB,0XD7,0X7F,0XCF,0X5C,0XCF,0X18,0XCE,0X94,0XE6,0XD4,
0XBD,0X2C,0X9C,0X48,0X6B,0X43,0XC5,0XED,0XE6,0X0F,0XFE,0XF3,0XD4,0X69,0XBB,0X86,
0XCC,0X69,0XFE,0XB2,0XAB,0XA6,0XD5,0X0C,0XDD,0X8E,0XFF,0X13,0XAC,0XAA,0X8B,0XE7,
0XDF,0X1A,0XF7,0XDD,0XF7,0XFD,0XFF,0XFE,0XFF,0XFE,0XA5,0X33,0X29,0X84,0X52,0XE9,
0XC6,0X57,0XEF,0X9C,0XEF,0X9C,0XFF,0XFE,0XF7,0XBD,0XEF,0XBC,0X52,0XC9,0X6B,0XAC,
0X94,0XF0,0XE7,0X7B,0XEF,0XBC,0XFF,0XFE,0XFF,0XFF,0XF7,0XBF,0XF7,0XBF,0XF7,0XFF,
0XB6,0X39,0XCE,0XFB,0XD7,0X5C,0XDF,0XBD,0XAD,0XD6,0X32,0X07,0X21,0X43,0X63,0X4B,
0XE7,0X9D,0XE7,0XBE,0XBE,0X79,0XCE,0XFB,0X6B,0XCF,0X11,0X24,0X00,0X82,0X53,0X2D,
0XC6,0X9A,0XAD,0XF7,0XC6,0XBB,0XCE,0XFC,0X5B,0X6E,0X00,0XC3,0X5B,0X8E,0XD7,0X5D,
0XC6,0XBB,0XC6,0XDB,0XD7,0X3C,0XBE,0X99,0X4A,0XEB,0X32,0X49,0X3A,0X8A,0X3A,0X6A,
0X4A,0XEC,0XA5,0XB8,0X42,0XAC,0X32,0X09,0X32,0X08,0X32,0X28,0X2A,0X07,0X32,0X27,
0X9D,0X76,0X7C,0X72,0X29,0XC8,0X32,0X29,0X29,0XC8,0X2A,0X08,0X2A,0X08,0X8C,0XF4,
0X84,0XB3,0X19,0X65,0X32,0X29,0X32,0X49,0X09,0X05,0X42,0XCB,0XAD,0XF8,0X53,0X0C,
0X21,0XC6,0X32,0X27,0X32,0X07,0X42,0XA9,0XA5,0X95,0X6B,0XCD,0X29,0XC5,0X29,0XC5,
0X31,0XE5,0X6B,0XCC,0X9D,0X11,0X7C,0X2E,0X52,0XC8,0X4A,0X86,0X7C,0X0C,0XA5,0X71,
0X4A,0XA6,0X63,0X28,0XCE,0X96,0XF7,0XFD,0XD7,0X19,0XCE,0XF9,0XE7,0X9C,0XA5,0X94,
0XB5,0XF6,0XCE,0XD9,0XCE,0XB8,0XD6,0XF8,0XEF,0X7A,0X9C,0XEF,0X9C,0XAE,0XA4,0XEE,
0X9C,0XCE,0X8C,0X6D,0X9D,0X0F,0XAD,0XD3,0X95,0X32,0X84,0XD1,0X95,0X52,0XAD,0XF3,
0XCE,0XD4,0X5B,0X25,0X52,0X65,0X49,0XC4,0X5A,0X26,0X7B,0X28,0XE6,0X91,0X8B,0XC5,
0X8B,0XC8,0X94,0X0B,0X9C,0X2B,0X94,0X0A,0X93,0XC9,0X94,0X0A,0X8B,0X88,0X93,0XC9,
0X9C,0X09,0X7B,0X05,0X83,0X46,0X83,0X45,0X62,0X62,0XC5,0X4D,0XFF,0X55,0X93,0XE6,
0XAC,0XA7,0XA4,0X86,0XAD,0X2A,0XAD,0X2B,0X73,0XA7,0X52,0X83,0X73,0X86,0X73,0X65,
0X83,0XA7,0X94,0X29,0X63,0X46,0X53,0X69,0X5C,0XB0,0X6D,0XF8,0X45,0XB9,0X46,0X3C,
0X36,0X3C,0X36,0X5B,0X46,0X9C,0X3E,0X3C,0X36,0X5E,0X3E,0X7F,0X4E,0X1D,0X56,0X3E,
0X45,0XFF,0X56,0X5F,0X7F,0X3F,0X66,0X3E,0X55,0XFD,0X4D,0XDC,0X55,0XDB,0X4D,0XD9,
0X4D,0XF8,0X5D,0X96,0X95,0X75,0XCD,0XF6,0XB4,0XEF,0X9B,0XC8,0XAC,0X07,0XE5,0X8C,
0XB4,0X05,0XA4,0X05,0X94,0X66,0XCE,0X4E,0XAC,0X89,0XF6,0X51,0XFD,0XB1,0XB3,0X47,
0XC3,0XC8,0XED,0X4E,0XE5,0X0D,0XCC,0X8B,0XFF,0X57,0XFE,0XF5,0XFF,0X56,0XC4,0XED,
0XCE,0XB8,0XF7,0XFD,0XF7,0XFD,0XFF,0XFE,0XFF,0XFE,0XA5,0X53,0X39,0XE5,0X84,0X6F,
0XCE,0XB8,0XF7,0XFD,0XFF,0XFE,0XEF,0X9C,0XFF,0XFE,0XEF,0XBC,0X4A,0XA8,0X73,0XED,
0XA5,0X73,0XEF,0XBC,0XF7,0XBC,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XF7,0XBF,0XF7,0XDF,
0XC6,0X9A,0XC6,0XDB,0XD7,0X3C,0XE7,0XDE,0XAD,0XD6,0X29,0XC5,0X19,0X23,0X73,0XEE,
0XF7,0XFF,0XEF,0XDE,0XBE,0X79,0XDF,0X7D,0X7C,0X50,0X21,0XA6,0X08,0XC3,0X6B,0XCE,
0XD7,0X1C,0XCE,0XDA,0XD7,0X1C,0XCE,0XDA,0X4A,0XCB,0X00,0X61,0X6B,0XCF,0XD7,0X3C,
0XC6,0XDB,0XBE,0X7A,0XCF,0X1B,0XB6,0X58,0X4A,0XEA,0X32,0X27,0X42,0XAA,0X3A,0X69,
0X53,0X2D,0XAD,0XF8,0X4A,0XEC,0X3A,0X6A,0X3A,0X8A,0X3A,0X89,0X3A,0X88,0X42,0XA9,
0X9D,0X76,0X7C,0XB3,0X2A,0X09,0X4A,0XEC,0X3A,0X8A,0X42,0XCB,0X3A,0X69,0X8D,0X34,
0X84,0XD2,0X32,0X29,0X3A,0X8A,0X32,0X49,0X32,0X08,0X3A,0X8B,0XA5,0XD8,0X53,0X2D,
0X21,0XC6,0X3A,0X69,0X21,0XC6,0X21,0XC6,0XBE,0X79,0X7C,0X51,0X32,0X07,0X42,0X69,
0X29,0XC6,0X4A,0XA9,0XBE,0X37,0X73,0XCD,0X31,0XE5,0X3A,0X26,0X3A,0X25,0X8C,0XAF,
0X94,0XCF,0X94,0XAF,0XE7,0X9B,0XF7,0XFD,0XD7,0X3A,0XCE,0XD9,0XE7,0X9C,0X7C,0X4F,
0X63,0X8C,0XBE,0X57,0XDF,0X5A,0XF7,0XBC,0XFF,0XFD,0X9C,0XF0,0X83,0XEB,0X9C,0XCE,
0XAD,0X71,0XA5,0X50,0X94,0XEF,0XBE,0X55,0X9D,0X93,0X95,0X53,0XBE,0X97,0XB6,0X34,
0XE7,0X97,0X84,0X6B,0X8C,0X2B,0X83,0XAB,0X8B,0XCC,0X8B,0XCA,0XDE,0X71,0X83,0XE6,
0X52,0X63,0X62,0XA5,0X73,0X48,0X73,0X27,0X83,0XA9,0X8C,0X0A,0X83,0X88,0X83,0XC9,
0X8B,0XE9,0X94,0X2A,0X94,0X09,0X83,0X87,0X73,0X25,0X7B,0X86,0X8B,0XC8,0X8B,0XE7,
0X9C,0X48,0X83,0XC6,0X7B,0X86,0X6B,0X67,0X73,0XE9,0X6B,0XA8,0X8C,0X6B,0X84,0X2A,
0X7B,0XC8,0X8C,0X6A,0X63,0X88,0X4B,0X89,0X4C,0X6F,0X5D,0XF7,0X4E,0X3A,0X4E,0XBE,
0X36,0X3C,0X36,0X5C,0X3E,0X7C,0X3E,0X7D,0X3E,0XBF,0X4E,0XFF,0X56,0X5E,0X45,0X7B,
0X4D,0XBD,0X65,0XFD,0XB7,0X9F,0XB7,0X7E,0X9E,0XDC,0X8E,0X9A,0X9E,0XDB,0X8E,0X98,
0X97,0X39,0XA6,0XF7,0XD6,0XB6,0XE6,0X13,0XA4,0X08,0X72,0X40,0XA3,0X63,0XFE,0X0D,
0XA3,0X62,0XBC,0XA7,0XD6,0X2D,0XAD,0X2A,0XF6,0XD2,0X8B,0X04,0XFE,0X12,0XE4,0XCD,
0X8A,0X22,0XBB,0XA7,0XFE,0XD4,0XCC,0X8B,0XDD,0X4E,0XFF,0X56,0XFF,0X15,0XE5,0XB0,
0XDF,0X19,0XF7,0XFD,0XF7,0XFD,0XFF,0XFD,0XF7,0XBC,0XAD,0X93,0X52,0XA8,0X94,0XD0,
0XDF,0X19,0XFF,0XFD,0XFF,0XFD,0XE7,0X7B,0XFF,0XFE,0XEF,0X9B,0X42,0X47,0X73,0XCC,
0X9D,0X31,0XEF,0X9B,0XF7,0XDD,0XF7,0XBD,0XF7,0XBE,0XFF,0XFF,0XF7,0XBF,0XEF,0XDF,
0XC6,0X9A,0XBE,0X79,0XCF,0X1C,0XE7,0XDF,0XAD,0XD6,0X29,0XA5,0X19,0X03,0X84,0X50,
0XF7,0XFF,0XEF,0XFF,0XBE,0X59,0XD7,0X3C,0X74,0X0F,0X21,0XA6,0X10,0XE3,0X6B,0XCE,
0XE7,0X7D,0XCE,0XDA,0XD7,0X3B,0XCE,0XFA,0X53,0X0B,0X08,0XE2,0X7C,0X70,0XDF,0X7D,
0XD7,0X1C,0XBE,0X79,0XCE,0XFB,0XBE,0X58,0X42,0XA9,0X21,0XC5,0X32,0X27,0X21,0XC6,
0X5B,0X6D,0XAE,0X18,0X4B,0X0C,0X3A,0X89,0X42,0XA9,0X42,0XA9,0X3A,0X87,0X42,0XC9,
0XA5,0XB6,0X84,0XF3,0X32,0X49,0X53,0X4D,0X42,0XCB,0X4B,0X2C,0X42,0XCA,0X9D,0X95,
0X8D,0X13,0X4B,0X0B,0X4A,0XEB,0X3A,0XAA,0X53,0X2C,0X4B,0X2C,0XAE,0X18,0X5B,0X8E,
0X53,0X2D,0X32,0X69,0X3A,0X69,0X3A,0X69,0X95,0X55,0X5B,0X6E,0X3A,0X49,0X21,0X86,
0X32,0X28,0X29,0XE7,0XA5,0X95,0X5B,0X6C,0X3A,0X27,0X4A,0XA9,0X29,0XC5,0X6B,0XAC,
0XAD,0XB4,0XA5,0X93,0XF7,0XDD,0XF7,0XFD,0XDF,0X3B,0XD6,0XF9,0XEF,0XFD,0X74,0X2E,
0X4A,0XE9,0XD7,0X1A,0XEF,0XBC,0XF7,0XBC,0XFF,0XFD,0X9C,0XAF,0X73,0X4A,0XAD,0X51,
0XB5,0XB2,0XBE,0X14,0XB6,0X34,0X95,0X31,0X5B,0XAC,0X95,0X53,0XAE,0X15,0XA5,0XF3,
0XCE,0XF5,0X6B,0X88,0X5A,0XC7,0X5A,0XA7,0X73,0X6B,0X7B,0X89,0XCE,0X51,0X94,0X89,
0X83,0XEA,0X7B,0XEB,0X7B,0XEB,0X7B,0XCA,0X84,0X0B,0X7B,0XCA,0X6B,0X48,0X6B,0X67,
0X73,0X88,0X73,0X67,0X6B,0X47,0X73,0XA8,0X7B,0XA8,0X62,0XE5,0X52,0X63,0X63,0X05,
0XBD,0X6F,0XCE,0X12,0X9C,0XCE,0X63,0X69,0X7C,0X2D,0X63,0X6A,0X63,0X8A,0X73,0XCA,
0X6B,0X88,0X74,0X0A,0X5B,0X89,0X54,0X0C,0X4C,0XF1,0X56,0X18,0X46,0X3A,0X3E,0X7E,
0X3E,0X3F,0X3E,0X3F,0X3E,0X3E,0X36,0X5D,0X36,0X7E,0X4E,0XFF,0X6E,0XBE,0X5D,0XBA,
0X65,0X78,0X6D,0X14,0XC6,0XB5,0XD6,0X92,0XBD,0XF0,0XAD,0X6E,0XC5,0XF1,0XC5,0XF0,
0XAD,0XEF,0XA5,0X8D,0XCD,0X6D,0XCD,0X2C,0X94,0X05,0X62,0XA0,0X93,0XC4,0XDD,0XCD,
0X9B,0X44,0XBC,0X68,0XC5,0X4B,0X72,0XC1,0XE5,0XEF,0XED,0X8D,0XAB,0X24,0XFE,0X50,
0XC3,0XE6,0X92,0X60,0XE5,0X4C,0XFE,0X2F,0XBC,0X07,0XFE,0X50,0XFE,0X91,0XE5,0XCE,
0XE7,0X7B,0XF7,0XDC,0XF7,0XFD,0XF7,0XFD,0XF7,0XFC,0XAD,0XB4,0X4A,0XA8,0X7C,0X0D,
0XD6,0XD8,0XFF,0XFD,0XE7,0X7B,0XF7,0XFD,0XFF,0XFD,0XF7,0XDC,0X52,0XC8,0X7C,0X2E,
0X94,0XD0,0XE7,0X7B,0XFF,0XFE,0XFF,0XFE,0XFF,0XDE,0XFF,0XFF,0XF7,0XBF,0XF7,0XFF,
0XC6,0X9A,0XC6,0XBA,0XDF,0X7D,0XE7,0XDE,0XAD,0XF7,0X3A,0X28,0X19,0X23,0X74,0X0F,
0XEF,0XDE,0XEF,0XFF,0XC6,0XBA,0XD7,0X1B,0X6B,0XEF,0X21,0X85,0X11,0X23,0X6B,0XEE,
0XEF,0XFE,0XC6,0X78,0XD7,0X1B,0XDF,0X3B,0X5B,0X2B,0X00,0X81,0X6B,0XCE,0XE7,0XBD,
0XDF,0X5D,0XC6,0X9A,0XD7,0X1C,0XC6,0XDA,0X42,0XC9,0X29,0XE6,0X32,0X48,0X2A,0X07,
0X5B,0X8D,0XAE,0X17,0X4B,0X0B,0X3A,0X89,0X42,0XA9,0X42,0XA8,0X3A,0X87,0X42,0XC9,
0XA5,0XF6,0X84,0XF3,0X32,0X69,0X43,0X0B,0X3A,0XAA,0X42,0XEB,0X42,0XEA,0X9D,0XD6,
0X8D,0X54,0X3A,0XAA,0X43,0X0B,0X42,0XCB,0X42,0XEB,0X53,0X6D,0XBE,0X9A,0X5B,0X8E,
0X53,0X6D,0X42,0XEB,0X4B,0X2C,0X53,0X2D,0XAE,0X18,0X7C,0X72,0X63,0X8E,0X53,0X2D,
0X53,0X2D,0X53,0X4D,0XAD,0XF7,0X74,0X30,0X4A,0XEA,0X3A,0X28,0X3A,0X47,0X52,0XEA,
0XAD,0XD6,0XBE,0X37,0XFF,0XFF,0XFF,0XFF,0XDF,0X5B,0XCE,0XF9,0XF7,0XFE,0X84,0X90,
0X5B,0X4A,0XEF,0XBC,0XF7,0XDC,0XEF,0X7B,0XFF,0XDC,0X94,0X6F,0X63,0X09,0XAD,0X52,
0XBD,0XF4,0XDE,0XF8,0XFF,0XFD,0X84,0X8F,0X32,0X47,0XA5,0XF6,0XA6,0X15,0XCF,0X59,
0XEF,0XFA,0X8C,0XCE,0X63,0X4A,0X6B,0X6B,0X84,0X0E,0X8C,0X4D,0XBD,0XF0,0X7C,0X28,
0X73,0XCA,0X63,0X6A,0X4A,0XA6,0X63,0X6A,0X73,0XCB,0X6B,0X89,0X73,0XEB,0X8C,0X8D,
0X73,0XEA,0X84,0X4C,0X73,0XEA,0X84,0X4B,0X8C,0X6C,0X7C,0X2B,0X7C,0X2B,0X73,0XA9,
0XA4,0XEF,0XFF,0XDC,0XE7,0X5B,0XA5,0X95,0XAD,0XD6,0X95,0X33,0X9D,0X53,0XC6,0X98,
0X7C,0X4E,0X7C,0X8E,0X5C,0X2D,0X5C,0XD0,0X55,0X54,0X56,0X39,0X36,0X1A,0X36,0X3D,
0X3E,0X1F,0X3E,0X1F,0X3E,0X3F,0X3E,0X3D,0X2E,0X1D,0X56,0XBE,0X9F,0XBF,0XAF,0X3D,
0XA6,0X59,0XA5,0X92,0XD6,0X2F,0XC5,0X28,0X93,0XE4,0X7B,0X63,0X8C,0X07,0X8C,0X48,
0X7C,0X47,0X84,0X06,0XBC,0XEA,0XCD,0X2B,0X94,0X26,0X73,0X42,0XA4,0X87,0XCD,0X6C,
0XAC,0X08,0XE5,0XCF,0X62,0X00,0XA4,0X27,0XB4,0X68,0XF6,0X0E,0XDC,0XEA,0XCC,0X67,
0XFE,0XF0,0X92,0XE1,0X92,0XE1,0XFE,0X2E,0XF6,0X2E,0XB4,0X46,0XFF,0X52,0XDD,0XAB,
0XE7,0X5B,0XFF,0XFD,0XFF,0XFD,0XF7,0XBC,0XFF,0XFE,0XA5,0X53,0X4A,0X67,0X84,0X4F,
0XDE,0XF9,0XFF,0XFE,0XF7,0XDD,0XFF,0XFE,0XFF,0XFD,0XDE,0XF9,0X52,0XC8,0X63,0X4A,
0XAD,0XB4,0XEF,0X9B,0XFF,0XDD,0XF7,0XBD,0XFF,0XDE,0XFF,0XFF,0XE7,0X5D,0XEF,0X9E,
0XD7,0X1C,0XB6,0X38,0XC6,0XDB,0XDF,0XBE,0XB6,0X38,0X3A,0X48,0X10,0XE3,0X73,0XEF,
0XEF,0XFF,0XEF,0XFF,0XCE,0XFB,0XD7,0X1B,0X7C,0X71,0X21,0XA5,0X19,0X65,0X73,0XEE,
0XF7,0XFF,0XD7,0X1B,0XFF,0XFF,0XD7,0X1B,0X4A,0XA9,0X19,0X43,0X7C,0X4F,0XE7,0X7C,
0XDF,0X3D,0XBE,0X9A,0XCE,0XDB,0XCF,0X1B,0X3A,0X88,0X2A,0X06,0X32,0X48,0X32,0X28,
0X53,0X4C,0XA5,0XD7,0X42,0XCB,0X3A,0X89,0X42,0XC9,0X42,0XA9,0X3A,0X87,0X42,0XE9,
0X9D,0XB5,0X7C,0XB2,0X3A,0XAA,0X42,0XCB,0X3A,0XCA,0X32,0X89,0X42,0XCA,0X9D,0XB5,
0X84,0XF2,0X32,0X89,0X42,0XEA,0X3A,0XA9,0X32,0X69,0X53,0X8D,0XB6,0X59,0X53,0X6D,
0X3A,0XAA,0X53,0X4D,0X43,0X0B,0X42,0XCB,0XAE,0X39,0X74,0X71,0X42,0XAB,0X4B,0X0C,
0X53,0X2D,0X4A,0XEC,0XA5,0X96,0X7C,0X71,0X63,0XCF,0X5B,0X6D,0X63,0XCE,0X74,0X0F,
0XAD,0XD7,0XCE,0X9A,0XFF,0XFF,0XF7,0XFF,0XD7,0X1A,0XC6,0X98,0XE7,0X9C,0X84,0X8F,
0X5B,0X4A,0XDF,0X3A,0XFF,0XFD,0XFF,0XFD,0XFF,0XFE,0XAD,0X12,0X7B,0XAD,0XA4,0XF1,
0XD6,0XB8,0XEF,0X9B,0XF7,0XFE,0X9D,0X74,0X53,0X6D,0XAE,0X58,0XB6,0X98,0XEF,0XFE,
0XE7,0XDB,0X95,0X10,0X42,0X47,0X39,0XC5,0X4A,0X47,0X84,0X2D,0XD6,0XD5,0XAD,0XD1,
0X74,0X0C,0X74,0X2E,0X53,0X2A,0X9D,0X52,0X8C,0XD0,0X5B,0X4A,0X5B,0X49,0X6B,0XCC,
0X6B,0XEC,0X4A,0XC7,0X74,0X2C,0X6B,0XCA,0X53,0X28,0X74,0X2C,0X63,0XAA,0X63,0X6A,
0X7B,0XCD,0XDE,0XDB,0XCE,0X9B,0XA5,0X97,0XAD,0XF9,0XB6,0X5A,0XA5,0XF8,0XAE,0X39,
0XA5,0XF7,0X9E,0X17,0X85,0X95,0X75,0XB7,0X5D,0XB8,0X5E,0X7D,0X46,0X7E,0X36,0X7F,
0X36,0X3F,0X36,0X1F,0X3E,0X3E,0X46,0X5E,0X3D,0XFC,0X66,0X3C,0XBF,0X7E,0XCF,0X1B,
0XCE,0X56,0XD6,0X33,0XF6,0X90,0XB4,0XC7,0X6B,0XA4,0X4B,0X64,0X54,0X08,0X4C,0X09,
0X4C,0X08,0X64,0X28,0XC5,0XAF,0XC5,0X4D,0X83,0XA5,0X73,0X02,0XBD,0X0A,0XD5,0X8D,
0XF6,0X30,0X8B,0X44,0X93,0XE6,0XD6,0X4F,0XE6,0X2F,0XBC,0XA8,0XFE,0X4F,0XCC,0XA8,
0XCD,0X08,0XFE,0XCE,0X8B,0X00,0X9B,0XA2,0XEE,0X4C,0XE5,0XEB,0XE5,0XEB,0XDD,0XEB,
0XE7,0X3A,0XFF,0XFE,0XFF,0XFE,0XEF,0X7B,0XFF,0XFE,0X94,0XD1,0X42,0X47,0XA5,0X53,
0XEF,0X9C,0XF7,0XDD,0XF7,0XBD,0XE7,0X5B,0XFF,0XFD,0XD6,0XD9,0XA5,0X53,0XBD,0XF5,
0XD6,0XF9,0XEF,0X9B,0XE7,0X1A,0XE7,0X1B,0XF7,0X9D,0XFF,0XDF,0XCE,0X9A,0XCE,0XDB,
0X9D,0X55,0XAE,0X18,0XC6,0XBA,0XB6,0X58,0XAD,0XD6,0X8C,0XD2,0X6B,0XAE,0X95,0X13,
0XCE,0XFB,0XC6,0X9A,0XAD,0XF7,0XC6,0X9A,0X8C,0XF3,0X3A,0X48,0X32,0X08,0X7C,0X50,
0XFF,0XFF,0XBE,0X37,0XE7,0X9D,0XDF,0X3B,0X52,0XEA,0X19,0X03,0X73,0XEE,0XFF,0XFF,
0XEF,0XBF,0XCE,0XFC,0XD7,0X3C,0XDF,0X7D,0X3A,0X69,0X32,0X27,0X3A,0X48,0X3A,0X69,
0X42,0XCB,0X9D,0X55,0X3A,0X6A,0X3A,0X49,0X42,0XA9,0X42,0XA9,0X3A,0X88,0X42,0XC9,
0X8D,0X34,0X74,0X71,0X42,0XEB,0X43,0X0B,0X43,0X0B,0X32,0X68,0X3A,0XCA,0X95,0X95,
0X74,0X70,0X43,0X0A,0X4B,0X2B,0X32,0X68,0X3A,0XCA,0X63,0XCF,0X9D,0XB6,0X5B,0XAE,
0X42,0XEB,0X42,0XEA,0X42,0XEB,0X4B,0X2C,0X9D,0XB6,0X6C,0X30,0X42,0XCB,0X32,0X6A,
0X42,0XCB,0X3A,0X6A,0XB6,0X59,0X6C,0X10,0X3A,0X8A,0X3A,0X69,0X32,0X48,0X63,0XAE,
0XB6,0X18,0XD6,0XDB,0XEF,0XDF,0XF7,0XFF,0XDF,0X5C,0XD7,0X3A,0XF7,0XFD,0X95,0X12,
0X94,0XF1,0XE7,0X7B,0XEF,0XBC,0XFF,0XFD,0XF7,0X7C,0XBD,0XD6,0XC5,0XD6,0XE6,0XFA,
0XF7,0XBC,0XEF,0XBC,0XB5,0XF7,0XB6,0X59,0X8D,0X14,0X95,0X75,0XA6,0X37,0XD7,0XBC,
0XD7,0X7B,0XAD,0XF5,0X6B,0X8D,0X52,0XAA,0X4A,0X89,0X94,0XD1,0XDF,0X58,0XC6,0X95,
0X4B,0X09,0X74,0X70,0X7C,0X70,0XF7,0XFF,0XD7,0X3B,0X6C,0X0E,0X3A,0XA8,0X3A,0X87,
0X74,0X6F,0X63,0XCC,0X32,0X67,0X7C,0XAF,0X6C,0X2D,0X42,0XC8,0X7C,0XAF,0X4B,0X09,
0X63,0X4D,0XD6,0XDC,0XD7,0X1E,0XB6,0X3A,0X9D,0X99,0XAE,0X5B,0XA6,0X3A,0X9D,0XF9,
0X95,0XB7,0X9E,0X59,0X8E,0X19,0X75,0XF9,0X55,0X99,0X56,0X7E,0X3E,0X3E,0X2D,0XFE,
0X3E,0X7F,0X36,0X3E,0X3E,0X1C,0X56,0X9D,0X4D,0XFB,0X65,0XB8,0XAE,0X17,0XAD,0X10,
0X94,0X0B,0XC4,0XEC,0XED,0XED,0XAC,0X66,0X6B,0XA6,0X54,0X29,0X4C,0XCE,0X3C,0XAD,
0X44,0X6B,0X5C,0X6A,0XBD,0XD1,0XB4,0XED,0X73,0X04,0X73,0X23,0XD5,0XAD,0XE6,0X0F,
0XD5,0X4C,0X72,0X80,0XCE,0X2E,0XEF,0X32,0XF7,0X11,0XEE,0X8F,0XED,0XED,0XFE,0X6F,
0XAC,0X04,0XFE,0XCE,0XDD,0XAA,0X9B,0XA2,0XCD,0X28,0XFF,0X30,0XD5,0X69,0XFE,0XEF,
0XEF,0X9C,0XE7,0X5B,0XDF,0X1A,0XDE,0XFA,0XEF,0X7C,0XCE,0X98,0XC6,0X37,0XEF,0X7C,
0XE7,0X5B,0XEF,0X7C,0XF7,0XBD,0XE7,0X1B,0XFF,0XFE,0XE7,0X5B,0XCE,0X98,0XCE,0X77,
0XF7,0XDC,0XEF,0X9B,0XEF,0X5B,0XFF,0XFE,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XEF,0X9E,
0XAD,0XF7,0XAE,0X17,0XC6,0XDA,0XC6,0XFB,0XBE,0X99,0X8C,0XD2,0X7C,0X30,0X73,0XEF,
0X9D,0X55,0XBE,0X59,0XB5,0XF8,0X9D,0X76,0XAD,0XF8,0X84,0XB3,0X84,0X92,0X95,0X14,
0XBE,0X18,0XAD,0XB6,0XBE,0X58,0XCE,0XBA,0X9D,0X13,0X63,0X6D,0X8C,0X91,0XD6,0XFB,
0XD6,0XFC,0XC6,0X9C,0XD7,0X1D,0XC6,0XDB,0X63,0XCE,0X21,0X85,0X29,0XE7,0X21,0XC7,
0X3A,0X49,0X8C,0XD3,0X4A,0XEC,0X32,0X29,0X3A,0X49,0X32,0X48,0X3A,0X68,0X42,0XC9,
0X74,0X50,0X74,0X71,0X3A,0XAA,0X42,0XEB,0X3A,0XCA,0X4B,0X2B,0X3A,0XCA,0X7C,0XD1,
0X74,0X90,0X32,0X88,0X42,0XEA,0X43,0X0B,0X4B,0X4C,0X42,0XEB,0X9D,0XB6,0X4B,0X4C,
0X42,0XEA,0X42,0XEA,0X42,0XCA,0X42,0XEA,0X95,0X74,0X63,0XEF,0X42,0XEB,0X42,0XCB,
0X3A,0XAA,0X3A,0XAA,0XA5,0XF7,0X74,0X70,0X3A,0X89,0X4B,0X0B,0X32,0X27,0X63,0XCE,
0XB5,0XF8,0XC6,0X7A,0XF7,0XFF,0XE7,0X5D,0XD7,0X1B,0XCE,0XD9,0XD7,0X19,0XD6,0XF9,
0XDF,0X5A,0XDF,0X5A,0XE7,0X5B,0XEF,0X7C,0XEF,0X3B,0XE6,0XFA,0XEF,0X3C,0XFF,0XBD,
0XFF,0XFE,0XF7,0XFF,0XD6,0XFC,0XC6,0X9C,0X9D,0X97,0X95,0X76,0X9D,0XD6,0XCF,0X5B,
0XAE,0X37,0XBE,0X58,0X8C,0XD3,0X9D,0X15,0X9D,0X34,0XA5,0XB5,0XBE,0X76,0XC6,0XD8,
0X95,0X74,0X7C,0XF3,0XA5,0XF7,0XD7,0X9D,0XB6,0X58,0X7C,0XD2,0X32,0XA9,0X22,0X27,
0X74,0X70,0X4B,0X4C,0X32,0X68,0X5B,0XAD,0X5B,0XCD,0X22,0X06,0X32,0X88,0X7C,0XB1,
0X95,0X13,0XE7,0X5D,0XD7,0X1D,0XB6,0X5A,0X9D,0XD8,0X95,0XB8,0X9D,0XD9,0X9D,0XF9,
0X9E,0X39,0X8D,0XF8,0X8E,0X5A,0X7E,0X5B,0X4D,0X79,0X56,0X3E,0X3D,0XFE,0X46,0X5E,
0X36,0X3B,0X3E,0X39,0X4E,0X19,0X56,0X3A,0X4D,0X98,0X86,0X59,0XAD,0XF3,0X9C,0X4B,
0X62,0X21,0XC4,0XCB,0XDD,0X2B,0XAC,0X49,0X53,0X06,0X43,0XCA,0X3C,0X6E,0X2C,0X2C,
0X44,0X2A,0X74,0XED,0XCE,0X12,0XA4,0X6B,0X7B,0X45,0X9C,0X69,0XDE,0X0F,0XE6,0X2F,
0X9B,0XC6,0XAC,0X88,0XE7,0X12,0XCE,0X8F,0XD6,0X8F,0XE6,0X8F,0XEE,0X4D,0XC5,0X09,
0XF6,0X4E,0X93,0X63,0XFE,0XF1,0XE5,0XCC,0XB4,0X46,0XBC,0X87,0XF6,0X6F,0X9B,0XA4,
0XEF,0X7C,0XEF,0X7C,0XF7,0X9D,0XF7,0XBD,0XF7,0X9D,0XCE,0X99,0XBE,0X16,0XD6,0XD9,
0XFF,0XFF,0XF7,0XDE,0XFF,0XFF,0XEF,0X9C,0XEF,0X7C,0XD6,0XD9,0XCE,0X98,0XC6,0X36,
0XF7,0XBC,0XEF,0X9B,0XEF,0X9B,0XFF,0XFD,0XFF,0XFE,0XFF,0XDE,0XFF,0XFE,0XEF,0XDE,
0XAD,0XD6,0XA5,0XF6,0XBE,0XBA,0XC6,0XFA,0XC6,0XDA,0X95,0X34,0X84,0X91,0X84,0X51,
0XCE,0XBB,0XDF,0X5E,0XC6,0X9B,0XAD,0XD7,0XB6,0X18,0X84,0XB2,0X7C,0X31,0X7C,0X51,
0XC6,0X9A,0XB5,0XF8,0XC6,0X79,0XCE,0X9A,0X95,0X13,0X63,0X6D,0X7C,0X10,0XB5,0XD7,
0XB6,0X39,0XA5,0XB8,0XBE,0X5A,0XC6,0X9A,0X7C,0X71,0X42,0X89,0X4B,0X0C,0X53,0X0C,
0X53,0X2D,0X9D,0X56,0X5B,0X6E,0X3A,0X6A,0X3A,0X8A,0X3A,0X69,0X32,0X07,0X2A,0X07,
0X74,0X51,0X6C,0X30,0X2A,0X28,0X2A,0X27,0X21,0XC6,0X2A,0X27,0X22,0X07,0X64,0X0E,
0X6C,0X4F,0X2A,0X47,0X32,0X89,0X32,0X69,0X3A,0XCA,0X3A,0X89,0X8D,0X34,0X4B,0X2B,
0X53,0X8C,0X53,0X8C,0X53,0X6C,0X53,0X4B,0X95,0X74,0X63,0XEE,0X42,0XEB,0X42,0XEB,
0X42,0XEB,0X3A,0XA9,0X95,0X74,0X6C,0X2F,0X3A,0XA9,0X53,0X4B,0X3A,0X88,0X6C,0X0F,
0XBE,0X59,0XD6,0XDB,0XFF,0XFF,0XEF,0XBE,0XEF,0X9D,0XEF,0X9D,0XEF,0XDD,0XEF,0X9C,
0XF7,0XFD,0XF7,0XFD,0XFF,0XFE,0XFF,0XFE,0XF7,0XBD,0XEF,0X5C,0XF7,0X7D,0XFF,0XFF,
0XFF,0XFE,0XF7,0XFF,0XE7,0X9F,0XDF,0X7F,0XCF,0X3E,0XC6,0XFC,0XC7,0X3C,0XE7,0XFF,
0XF7,0XFF,0XBE,0X7A,0X95,0X36,0XCE,0XBC,0XAD,0XF8,0XAD,0XF7,0XD7,0X5B,0XCF,0X3B,
0XB6,0X79,0X8D,0X55,0X9D,0XF8,0XD7,0X9E,0XBE,0XDB,0X85,0X34,0X4B,0X4C,0X4B,0X4D,
0X7C,0XF3,0X5B,0XCE,0X3A,0XCA,0X5C,0X0F,0X6C,0X71,0X2A,0X89,0X32,0XAA,0X84,0XF2,
0XAD,0XF6,0XF7,0XFF,0XE7,0XBE,0XC6,0XDA,0XA6,0X39,0XA6,0X59,0XA6,0X59,0X9E,0X19,
0X9E,0X19,0X96,0X39,0X85,0XF9,0X6D,0XD9,0X5D,0X99,0X4D,0XBB,0X35,0X7B,0X3D,0XBA,
0X45,0XD8,0X66,0XBA,0X76,0XFB,0X5D,0XF8,0X5D,0XB7,0X86,0X58,0XB6,0X33,0X8C,0X07,
0X72,0XC2,0XCD,0X0B,0XD4,0XC9,0XAB,0XE7,0X63,0X06,0X5B,0XA9,0X4C,0X0A,0X43,0XC8,
0X63,0XE8,0X84,0X49,0XDD,0XF0,0XE5,0XF0,0XC5,0X6D,0XCE,0X2F,0XEE,0XF2,0XAC,0X88,
0X7A,0XE2,0XDE,0X2F,0XEF,0X73,0XC6,0X2D,0XBD,0XCB,0XF7,0X31,0XF6,0XEF,0XB4,0XC7,
0XE5,0XED,0XE5,0XAD,0XE5,0X8C,0XFE,0XD1,0XB4,0X06,0XAB,0XC5,0XDD,0X2B,0XED,0X8C,
0XF7,0XDD,0XFF,0XDE,0XFF,0XFE,0XFF,0XFE,0XFF,0XFE,0XEF,0X7C,0XE7,0X5B,0XF7,0XDD,
0XFF,0XFE,0XF7,0XBD,0XFF,0XFE,0XFF,0XFE,0XFF,0XFE,0XF7,0XDD,0XFF,0XFE,0XF7,0XDD,
0XFF,0XFD,0XFF,0XFC,0XFF,0XFD,0XFF,0XFD,0XFF,0XFE,0XF7,0XBD,0XF7,0XFE,0XFF,0XFF,
0XC6,0XB9,0XBE,0XB9,0XD7,0X9C,0XE7,0XFE,0XF7,0XFF,0XDF,0X7D,0XDF,0X5C,0XE7,0X7D,
0XE7,0X9E,0XEF,0XFF,0XD7,0X1D,0XBE,0X7A,0XD7,0X1C,0XB6,0X39,0XB6,0X18,0XB6,0X38,
0XDF,0X3D,0XCE,0XBB,0XD6,0XFC,0XDF,0X1C,0XB5,0XF7,0X8C,0XB2,0X95,0X14,0XB5,0XF8,
0XDF,0X5E,0XC6,0X9B,0XCE,0XDC,0XD7,0X5D,0XA5,0X96,0X6B,0XEF,0X74,0X30,0X74,0X30,
0X74,0X31,0XAE,0X19,0X7C,0X93,0X6B,0XD0,0X6C,0X11,0X7C,0X72,0X74,0X30,0X6C,0X0F,
0X8D,0X14,0X8D,0X34,0X63,0XCE,0X63,0XEF,0X5B,0XAE,0X5B,0XCE,0X4B,0X4C,0X7C,0XB1,
0X8D,0X13,0X4B,0X0B,0X42,0XEB,0X3A,0X89,0X42,0XCA,0X3A,0XAA,0X84,0XF3,0X4B,0X2C,
0X3A,0XA9,0X42,0XC9,0X42,0XE9,0X42,0XE9,0X84,0XD1,0X63,0XCD,0X4B,0X0B,0X4B,0X2B,
0X4B,0X2B,0X3A,0X88,0X84,0XB1,0X63,0XEE,0X42,0XA9,0X4B,0X0A,0X32,0X46,0X5B,0X8C,
0XC6,0X99,0XDF,0X1C,0XFF,0XFF,0XDF,0X3C,0XDF,0X3C,0XE7,0X3B,0XDF,0X3B,0XD6,0XD9,
0XDF,0X1A,0XDF,0X1A,0XDF,0X1A,0XE7,0X3B,0XDF,0X1B,0XDE,0XDA,0XDF,0X1B,0XEF,0X5C,
0XEF,0X5C,0XD6,0XFC,0XBE,0X3B,0X9D,0X79,0X9D,0X98,0X95,0X97,0XAE,0X58,0XC7,0X1A,
0XC6,0XFB,0XC6,0XDC,0XCE,0XBD,0XC6,0X9C,0XBE,0X7A,0XDF,0X9D,0XCF,0X3B,0XE7,0XFF,
0XC7,0X1C,0XB6,0XBB,0XC7,0X1C,0XE7,0XFF,0XD7,0XBF,0XA6,0X18,0X53,0X8E,0X4B,0X4D,
0X85,0X34,0X64,0X51,0X43,0X0C,0X64,0X51,0X85,0X34,0X43,0X4C,0X43,0X2C,0X95,0X94,
0XBE,0X96,0XF7,0XFD,0XEF,0XFC,0XC7,0X19,0XA6,0X16,0XC7,0X1A,0XA6,0X37,0X9E,0X16,
0X9E,0X37,0X9E,0X58,0X9E,0X9A,0X8E,0X7A,0X97,0X3E,0X8F,0X3F,0X76,0XFF,0X87,0X5F,
0X97,0X9E,0XAF,0XFF,0X9F,0X7F,0X5D,0X77,0X55,0X56,0X86,0X37,0XAE,0X52,0X73,0XE5,
0X8C,0X05,0XDE,0X0D,0XDD,0X08,0XBB,0XC4,0X93,0X43,0X9B,0XE6,0X9C,0X66,0XA4,0XA6,
0X9B,0XC4,0XBC,0X46,0XFE,0X0E,0XD4,0XE9,0XDE,0X4E,0XD6,0XAF,0XBD,0XAC,0X52,0X40,
0XC5,0X2C,0XFF,0X75,0XE6,0XF2,0XCE,0X2E,0XD6,0X2D,0XEF,0X0F,0XFF,0X90,0XF6,0XCE,
0XA3,0XE5,0XFE,0XD1,0XE5,0X8C,0XF5,0XED,0XF5,0XAD,0X92,0XA1,0XAB,0X44,0XF5,0X8C,
0XFF,0XFE,0XFF,0XFE,0XF7,0XDD,0XFF,0XFE,0XFF,0XDD,0XFF,0XDD,0XFF,0XFE,0XFF,0XFE,
0XF7,0X9C,0XEF,0X9C,0XEF,0X7C,0XF7,0XBD,0XFF,0XFE,0XF7,0XBC,0XF7,0XBD,0XEF,0X7B,
0XD6,0XB7,0XCE,0X56,0XD6,0X97,0XC6,0X36,0XC6,0X36,0XAD,0X53,0X9D,0X12,0XA5,0X95,
0X95,0X33,0X84,0XF2,0X95,0X74,0X9D,0XD5,0XB6,0X78,0XAE,0X17,0XC6,0X79,0XCE,0XBA,
0XD6,0XFB,0XDF,0X7D,0XCE,0XFB,0XC6,0X9A,0XE7,0X9E,0XDF,0X7D,0XEF,0XFF,0XF7,0XFF,
0XE7,0X9E,0XDF,0X5D,0XE7,0XBE,0XF7,0XFF,0XEF,0XBE,0XDF,0X5D,0XE7,0XBE,0XF7,0XFF,
0XEF,0XBF,0XCE,0XDC,0XC6,0XBB,0XD7,0X3C,0XAD,0XF7,0X74,0X50,0X7C,0X50,0X74,0X50,
0X8C,0XF4,0XBE,0X5A,0X95,0X35,0X84,0X93,0X84,0X93,0X95,0X15,0X8C,0XF4,0X84,0XD3,
0XAE,0X18,0XA5,0XF7,0X7C,0X91,0X74,0X71,0X6C,0X30,0X74,0X91,0X7C,0X91,0XA5,0XD6,
0XA5,0XF6,0X74,0X70,0X74,0X70,0X6C,0X0F,0X7C,0X71,0X74,0X91,0XB6,0X59,0X8D,0X13,
0X74,0X70,0X7C,0X90,0X74,0X4F,0X74,0X2F,0X9D,0X94,0X7C,0X90,0X63,0XAD,0X5B,0X8C,
0X63,0XAD,0X4B,0X0A,0X8C,0XF2,0X7C,0XB0,0X5B,0X8C,0X5B,0X8B,0X4A,0XE9,0X63,0XCD,
0XCE,0X99,0XDF,0X1B,0XF7,0XBE,0XD6,0XBA,0XDE,0XFB,0XDF,0X3B,0XDF,0X1B,0XD6,0XDA,
0XE7,0X5B,0XE7,0X3B,0XE7,0X5C,0XEF,0X7D,0XEF,0X7D,0XEF,0X7C,0XEF,0X9D,0XF7,0XDE,
0XFF,0XFE,0XF7,0XBF,0XDF,0X3F,0XB5,0XFB,0XAD,0XFA,0X9D,0X97,0XA6,0X17,0XAE,0X57,
0XC6,0XDB,0XB6,0X3A,0X95,0X37,0XB6,0X3A,0XC6,0XFC,0XAE,0X17,0X6C,0X50,0X7C,0XD2,
0X95,0X96,0XA5,0XF8,0XA5,0XF8,0XA6,0X18,0X9D,0XD7,0X85,0X14,0X3A,0XEB,0X2A,0X69,
0X6C,0X71,0X5B,0XEF,0X32,0XAA,0X53,0XCF,0X7C,0XF3,0X3B,0X2C,0X3B,0X0B,0X95,0XB4,
0XC6,0XD7,0XF7,0XFB,0XEF,0XFB,0XC6,0XF7,0X85,0X10,0XD7,0X9B,0X85,0X11,0X8D,0X73,
0XA6,0X98,0X85,0XB5,0XB7,0X5C,0X96,0X9A,0X86,0X7A,0X86,0X9C,0X76,0X5C,0X86,0XBD,
0X7E,0X5A,0XA7,0X1D,0X7D,0XD8,0X75,0X77,0X75,0XB8,0X9E,0X78,0XA5,0XD0,0X73,0X85,
0X62,0XE1,0XCD,0X6B,0XE5,0X49,0XE5,0X09,0XD5,0X2A,0XD5,0X6A,0XCD,0X47,0XD5,0XA8,
0XE5,0X8A,0XFD,0XCC,0XF5,0X09,0XBB,0XA3,0XB4,0X86,0XCE,0X0B,0X83,0X82,0XB5,0X09,
0XEE,0X70,0XEE,0X70,0XD6,0X6F,0XDE,0XF1,0XE6,0XF0,0XDE,0X8E,0XE6,0XEF,0XEE,0XEF,
0XFE,0XAF,0XD5,0X2A,0XED,0XAC,0X9B,0X22,0XE5,0X6B,0XED,0X6B,0XA2,0XE1,0XBB,0X84,
0XD6,0XB8,0XD6,0XB8,0XBE,0X15,0XDF,0X19,0XD6,0XB8,0XD6,0X98,0XCE,0X77,0XBD,0XF5,
0XAD,0X93,0XB5,0XD4,0X8C,0X6F,0X94,0XB0,0XBD,0XF5,0XB5,0XB4,0XA5,0X12,0XA5,0X52,
0XA5,0X51,0X9C,0XCF,0XAD,0X52,0X9C,0XD0,0X9C,0XD0,0X6B,0X4B,0X42,0X47,0X42,0X88,
0X5B,0X8C,0X4B,0X0A,0X43,0X0A,0X42,0XC9,0X4B,0X2B,0X42,0XA9,0X4A,0XEA,0X53,0X0B,
0X5B,0X6C,0X6B,0XEF,0X6B,0XCE,0X63,0X8D,0X7C,0X50,0X74,0X0F,0X84,0XB1,0X84,0XB1,
0X95,0X34,0X8C,0XF3,0X9D,0X54,0XAD,0XD7,0XAE,0X17,0XB6,0X58,0XC6,0XBA,0XC6,0XDA,
0XDF,0X5D,0XC6,0XBA,0XC6,0XBA,0XDF,0X7C,0XCE,0XFA,0XAD,0XF6,0XB6,0X57,0XB6,0X58,
0X9D,0X95,0XC6,0X9A,0XB6,0X19,0XAD,0XF8,0XA5,0XB7,0XB6,0X39,0XAE,0X18,0XAD,0XF7,
0XBE,0X7A,0XBE,0X9A,0XA5,0XB7,0XA5,0XB6,0X9D,0X96,0XA5,0XD6,0XA5,0XB6,0XBE,0X99,
0XB6,0X38,0X8D,0X33,0X95,0X54,0X8D,0X13,0X95,0X55,0X95,0X35,0XBE,0X79,0X9D,0X75,
0X84,0XB1,0X8C,0XF2,0X8C,0XF2,0X8D,0X12,0XBE,0X58,0XAD,0XF6,0X95,0X33,0X8C,0XF2,
0X95,0X12,0X7C,0X91,0XB6,0X16,0XAE,0X16,0X95,0X32,0X95,0X32,0X8C,0XF1,0XA5,0X94,
0XBE,0X36,0XD6,0XB9,0XE7,0X3B,0XCE,0X78,0XE7,0X3B,0XF7,0XBD,0XF7,0XDE,0XFF,0XFE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XFF,0XFF,
0XFF,0XFE,0XFF,0XFF,0XEF,0XBF,0XC6,0X7D,0XBE,0X9C,0XAE,0X39,0XB6,0XB8,0XBE,0XD9,
0XD7,0X5D,0XEF,0XDF,0XCE,0XFE,0XEF,0XFF,0X9D,0XD7,0X4B,0X4C,0X53,0XCE,0X4B,0X8D,
0X5B,0XEF,0X7C,0XF3,0X9D,0XD6,0XB6,0X79,0XAE,0X79,0X95,0XB6,0X4B,0X4C,0X3A,0XCA,
0X5B,0XCE,0X53,0XAE,0X2A,0X89,0X4B,0X6C,0X6C,0X70,0X32,0XCA,0X32,0XCA,0X8D,0X73,
0XD7,0X59,0XF7,0XFC,0XF7,0XFC,0XCF,0X18,0X6C,0X2D,0XE7,0XFC,0X64,0X0D,0X85,0X11,
0XAE,0X98,0X64,0X2F,0XBF,0X5C,0X85,0XD7,0X3B,0XAF,0X54,0X73,0X4C,0X94,0X5C,0XF6,
0X5C,0XD4,0X75,0X57,0X43,0X4E,0X7C,0XF5,0X85,0X15,0XA5,0XF6,0XA4,0XCE,0X9C,0X2A,
0X9C,0X2A,0XDE,0X10,0XDD,0X6C,0XD5,0X4B,0XC5,0X8C,0XBD,0XCC,0XAD,0X89,0XBD,0XC9,
0XC5,0XAB,0XD5,0X6B,0XED,0X4B,0XD4,0X46,0XED,0X89,0XAB,0XE2,0XAC,0X02,0XFF,0X50,
0XFE,0XEF,0XFE,0XF0,0XF7,0X72,0XDF,0X31,0XAD,0X8B,0XEF,0X73,0XF7,0X74,0XEE,0XD1,
0XFF,0X72,0XEE,0X6E,0XDD,0X8B,0XF6,0X4E,0XA3,0X82,0XF5,0XEC,0XE5,0X49,0XBB,0XC3,
0X8C,0X8F,0X84,0X4E,0X5B,0X09,0X8C,0X6E,0X94,0XAF,0X9C,0XF0,0X94,0XD0,0X8C,0X8F,
0X94,0XB0,0XA5,0X31,0X5B,0X09,0X73,0XCC,0XB5,0XD4,0XBD,0XF4,0XAD,0X93,0XC6,0X35,
0XC6,0X55,0XBD,0XD3,0XDE,0XD7,0XCE,0X76,0XE6,0XF9,0XA5,0X32,0X73,0XAC,0X74,0X0E,
0X74,0X0E,0X5B,0XAC,0X5B,0XAC,0X4B,0X4B,0X53,0X6C,0X42,0XA9,0X4A,0XCA,0X4A,0XA9,
0X32,0X27,0X4A,0XEA,0X53,0X4B,0X4B,0X0A,0X5B,0X6C,0X4A,0XCA,0X53,0X2B,0X4A,0XEA,
0X53,0X2B,0X42,0XCA,0X4A,0XEA,0X4B,0X0B,0X4B,0X0B,0X4B,0X2B,0X53,0X4C,0X4B,0X0B,
0X5B,0X8D,0X53,0X4C,0X5B,0X6C,0X74,0X2E,0X6C,0X0E,0X63,0XAC,0X74,0X2E,0X74,0X6F,
0X74,0X50,0X8C,0XF3,0X95,0X35,0XA5,0XD7,0XA5,0X97,0XB6,0X19,0XAD,0XF8,0XAD,0XF7,
0X9D,0X76,0XAD,0XF8,0XB6,0X39,0XBE,0X9A,0XC6,0XBA,0XBE,0X9A,0XB6,0X38,0XB6,0X58,
0XCE,0XFB,0XB6,0X58,0XC6,0XBA,0XBE,0X79,0XBE,0X9A,0XBE,0X79,0XCE,0XFC,0XBE,0X79,
0XBE,0X79,0XC6,0X99,0XBE,0X78,0XBE,0X58,0XD6,0XFB,0XC6,0X99,0XAD,0XB6,0X9D,0X74,
0XB6,0X17,0XA5,0XB5,0XC6,0X98,0XBE,0X77,0XA5,0X94,0XA5,0X94,0XAD,0XF5,0XBE,0X56,
0XD6,0XB8,0XDE,0XF9,0XE7,0X1A,0XC6,0X57,0XEF,0X7C,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XF7,0XFF,0XFF,0XFF,0XF7,0XFF,0XF7,0XFE,0XF7,0XFE,
0XF7,0XFD,0XFF,0XFE,0XEF,0X7F,0XCE,0XBE,0XC6,0XBC,0XBE,0XDA,0XCF,0X5A,0XDF,0XDC,
0XE7,0XFF,0XEF,0XFF,0XEF,0XFF,0XB6,0X5A,0X7C,0XD2,0X74,0X90,0X43,0X2B,0X53,0XEE,
0X53,0X8D,0X63,0XEE,0X95,0X95,0XDF,0XDE,0XDF,0XBD,0XA5,0XF6,0X3A,0XEA,0X2A,0X47,
0X5B,0XCE,0X5B,0XCD,0X3A,0XC9,0X43,0X4B,0X5C,0X0E,0X32,0XC9,0X3A,0XEA,0X7C,0XF1,
0XDF,0X7B,0XF7,0XFD,0XF7,0XFE,0XD7,0X5A,0X7C,0X6F,0XEF,0XFE,0X7C,0X90,0X95,0X74,
0XC6,0XFA,0X64,0X0F,0XCF,0X7D,0X95,0XD8,0X22,0X6A,0X43,0X90,0X54,0X32,0X5C,0X94,
0X6C,0XB4,0X85,0X35,0X3A,0X48,0X9D,0X12,0X95,0X11,0XBD,0XB3,0XBC,0XCD,0XE5,0XB0,
0XDD,0XB1,0XDD,0XF1,0XAB,0XE7,0X8B,0X43,0X7B,0X64,0X7B,0XE6,0X7B,0XE4,0X84,0X25,
0X8C,0X46,0XC5,0X6C,0XFE,0X50,0XFD,0XAC,0XED,0XEB,0X9B,0XA1,0XEE,0X2A,0XF6,0X4B,
0XF6,0X4C,0XFE,0XEF,0XDE,0XF0,0XC6,0X6E,0X5A,0XE2,0XE6,0XF2,0XEF,0X13,0XE6,0X91,
0XEF,0X11,0XEE,0XD0,0XDE,0X4D,0XD5,0XCC,0XFE,0XAF,0XC4,0XC7,0XFF,0X52,0XC4,0X86,
0XC6,0X55,0XBE,0X34,0X84,0X2C,0XB5,0XF3,0XCE,0X96,0XDE,0XF7,0XDE,0XF8,0XE7,0X59,
0XDF,0X18,0XEF,0X79,0XA5,0X51,0XC6,0X55,0XEF,0X9A,0XF7,0XBA,0XEF,0X9A,0XF7,0XFB,
0XEF,0X9A,0XDE,0XF7,0XFF,0XFC,0XF7,0X9A,0XFF,0XFD,0XDE,0XF8,0XB5,0XD4,0XC6,0X97,
0X84,0XB0,0X74,0X6F,0X74,0XB0,0X6C,0X4F,0X7C,0X90,0X63,0XED,0X74,0X0E,0X73,0XEE,
0X5B,0X4B,0X6B,0XED,0X7C,0X4E,0X74,0X0E,0X7C,0X6F,0X6B,0XED,0X74,0X4F,0X63,0XCD,
0X74,0X2F,0X63,0XED,0X63,0XED,0X63,0XCD,0X53,0X6C,0X4B,0X2B,0X4B,0X0A,0X42,0XE9,
0X4B,0X2B,0X53,0X4B,0X53,0X2B,0X5B,0X8B,0X53,0X4A,0X4A,0XE8,0X5B,0X6B,0X5B,0X8C,
0X53,0X4B,0X4A,0XEB,0X53,0X0C,0X63,0XCF,0X53,0X2D,0X5B,0X8E,0X53,0X4D,0X5B,0X4D,
0X6B,0XCF,0X6B,0XF0,0X6B,0XF0,0X6B,0XF0,0X74,0X30,0X7C,0X51,0X7C,0X91,0X84,0XB2,
0X84,0XD3,0X84,0XB2,0X95,0X55,0X95,0X34,0XA5,0X96,0XA5,0XB7,0XB6,0X19,0XB6,0X39,
0XBE,0X59,0XCE,0XDA,0XD6,0XDA,0XD6,0XFB,0XE7,0X5D,0XE7,0X9D,0XDF,0X3C,0XDF,0X5C,
0XDF,0X5C,0XD7,0X1B,0XE7,0X7C,0XDF,0X5C,0XCE,0XB8,0XCE,0XF9,0XDF,0X7B,0XE7,0X7B,
0XEF,0XBC,0XE7,0X5A,0XDE,0XD9,0XC6,0X16,0XEF,0X7C,0XF7,0XDE,0XF7,0X9E,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XF7,0XFF,0XF7,0XFF,0XF7,0XFF,0XF7,0XFE,0XEF,0XDD,
0XF7,0XFC,0XFF,0XFE,0XEF,0XBF,0XEF,0XBF,0XE7,0XBF,0XDF,0XBD,0XD7,0X9A,0XDF,0XFC,
0XEF,0XFF,0XE7,0XBF,0XEF,0XFF,0XAE,0X39,0X95,0X94,0X85,0X73,0X3B,0X0A,0XB6,0XB9,
0XA6,0X36,0X7C,0XB1,0X8D,0X12,0XCF,0X5B,0XD7,0X9C,0XB6,0X77,0X6C,0X2E,0X5B,0XCD,
0X6C,0X4F,0X6C,0X6F,0X53,0X8C,0X5B,0XED,0X6C,0X4F,0X43,0X6B,0X4B,0X6B,0X7C,0XB0,
0XD6,0XFB,0XF7,0XDF,0XEF,0XDF,0XDF,0X3C,0X9D,0X35,0XD7,0X1C,0XA5,0XB7,0XA5,0XB7,
0XDF,0X7E,0X8D,0X15,0XCF,0X5E,0XAE,0X5B,0X2A,0X4B,0X32,0XAD,0X43,0X2F,0X43,0X0F,
0X5B,0X4F,0X84,0X71,0X41,0XE4,0XBD,0X4F,0XD6,0X31,0XEE,0X71,0XD4,0XCA,0XE5,0X0B,
0XDD,0X4D,0XE5,0X8E,0XCC,0X88,0XCC,0XC8,0XC4,0XEA,0XC5,0X2C,0XBC,0XEC,0XB4,0XCB,
0XB5,0X0B,0XF6,0XD2,0XED,0XEF,0XDD,0X8C,0X83,0XC3,0XC6,0X4C,0XEF,0X50,0XDE,0X8E,
0XE6,0X2E,0XEE,0X90,0XE6,0XD1,0XEF,0X53,0XAD,0X0A,0XEE,0XD2,0XF6,0XD2,0XE6,0X90,
0XD6,0XB0,0XE7,0X32,0XEF,0X53,0XCE,0X2E,0XB5,0X2A,0XDE,0X2E,0XAC,0X47,0XEE,0X4E,
0XF7,0XBB,0XF7,0XBB,0XAD,0X92,0XEF,0X7A,0XFF,0XFC,0XFF,0XFC,0XEF,0X7A,0XF7,0XDB,
0XFF,0XDC,0XFF,0XFC,0XCE,0X56,0XEF,0X7A,0XF7,0XDB,0XF7,0XBA,0XF7,0XBB,0XEF,0X7A,
0XF7,0XDB,0XDE,0XF8,0XF7,0XBB,0XDE,0XD7,0XFF,0XDC,0XCE,0X76,0XAD,0XB4,0XD7,0X19,
0X95,0X12,0X84,0XD0,0X84,0XF1,0X74,0X6F,0X7C,0X90,0X6B,0XEE,0X74,0X2F,0X7C,0X4F,
0X74,0X2E,0X7C,0X8F,0X84,0XB0,0X74,0X4E,0X7C,0X6F,0X74,0X2E,0X84,0XB0,0X74,0X2F,
0X7C,0XB1,0X7C,0X90,0X7C,0XB1,0X84,0XD2,0X74,0X70,0X6C,0X4F,0X6C,0X4F,0X6C,0X2E,
0X74,0X6F,0X84,0XD1,0X7C,0XB0,0X7C,0X90,0X6C,0X0E,0X63,0XAC,0X74,0X2E,0X7C,0X4E,
0X94,0XF1,0X6B,0XED,0X63,0XAC,0X74,0X4F,0X5B,0XAD,0X6B,0XEF,0X63,0XCE,0X6C,0X0F,
0X63,0XAE,0X63,0XCF,0X63,0XCF,0X5B,0X8E,0X5B,0X8E,0X5B,0X6D,0X5B,0X8D,0X5B,0X6D,
0X5B,0X6D,0X53,0X4D,0X63,0XCF,0X5B,0X6D,0X5B,0X8E,0X5B,0X6E,0X63,0XAF,0X6C,0X10,
0X6B,0XAF,0X7C,0X51,0X84,0X91,0X84,0X71,0X84,0X91,0X94,0XF2,0X95,0X12,0X9D,0X53,
0XAD,0XD5,0XA5,0XB5,0XB6,0X17,0XBE,0X58,0XB6,0X16,0XBE,0X57,0XCE,0XF9,0XCE,0X98,
0XDE,0XF8,0XC6,0X35,0XB5,0X73,0XA4,0XF2,0XDE,0XDA,0XE7,0X3C,0XDE,0XFB,0XEF,0X7C,
0XE7,0X5C,0XE7,0X3C,0XDF,0X1B,0XD6,0XFB,0XCE,0XFA,0XCE,0XFA,0XC6,0XB8,0XBE,0X77,
0XB6,0X15,0XAD,0XB4,0X84,0X73,0X84,0X93,0X74,0X72,0X85,0X12,0X85,0X31,0X9D,0XF4,
0XB6,0X58,0XC6,0XFB,0X9D,0X96,0XA5,0XD7,0X9D,0XF6,0XA6,0X57,0XA6,0X98,0X9D,0XF6,
0XC7,0X3B,0X85,0X12,0X84,0XD1,0XBE,0XD9,0XD7,0X9C,0XDF,0XBD,0XA6,0X16,0X85,0X32,
0X9D,0XD5,0X9D,0XD5,0X85,0X32,0X8D,0X93,0X9D,0XD5,0X85,0X12,0X85,0X12,0X9D,0XB5,
0XCE,0XDB,0XFF,0XFF,0XEF,0X9F,0XD6,0XFC,0XAD,0XB7,0XAD,0XD8,0XB6,0X3A,0X95,0X56,
0XC6,0XDC,0X9D,0X97,0XBE,0XBC,0XBE,0X9B,0X42,0XCC,0X32,0X4B,0X32,0X8C,0X29,0XC9,
0X63,0X2D,0X8B,0XEE,0X39,0X00,0X93,0XC7,0XCD,0X4C,0XDD,0X8B,0XAB,0X83,0X92,0XA0,
0XD4,0XEB,0XED,0XAE,0XED,0XAB,0XFE,0X4E,0XE5,0XCE,0XDD,0XCF,0XE5,0XCF,0XE5,0XF0,
0XCD,0X8E,0XB4,0XEB,0XE6,0X10,0X7B,0X23,0X9D,0X0A,0XCE,0XD1,0XCE,0XD1,0XD6,0XB2,
0XE6,0XD4,0XDE,0X72,0XD6,0X72,0XDE,0XB3,0XEE,0XF4,0XDE,0X31,0XFF,0X55,0XE6,0XB3,
0XE7,0X56,0XE7,0X56,0XD6,0XD3,0XEF,0X75,0XBD,0XCE,0XCE,0X0F,0XA4,0XA9,0X62,0X80,
0XEF,0X5B,0XE7,0X1A,0XBD,0XD5,0XF7,0XBC,0XEF,0X5B,0XEF,0X7B,0XF7,0X9C,0XFF,0XFD,
0XF7,0X7B,0XF7,0XBC,0XCE,0X77,0XE7,0X1A,0XFF,0XDD,0XFF,0XFD,0XEF,0X5B,0XF7,0XBC,
0XFF,0XFD,0XD6,0XB8,0XFF,0XDC,0XEF,0X9B,0XFF,0XFD,0XD6,0XD8,0XC6,0X76,0XDF,0X39,
0X8C,0XD0,0X8C,0XD1,0X8D,0X11,0XAE,0X16,0X84,0XD1,0X84,0XD2,0X74,0X70,0X74,0X91,
0X6C,0X6F,0X7C,0XD0,0X95,0X52,0X84,0XD0,0X84,0XB1,0X95,0X33,0X95,0X34,0X8C,0XF3,
0X84,0X92,0X84,0XB3,0X84,0XF3,0X8D,0X14,0X84,0XD2,0X74,0X4F,0X7C,0X90,0X6C,0X2D,
0X84,0XAF,0X8D,0X32,0X7C,0XB1,0X85,0X14,0X74,0X72,0X73,0XF0,0X84,0X30,0XA5,0X11,
0XC6,0X14,0X7C,0X2B,0X8D,0X2F,0X74,0XAE,0X74,0X8F,0X7C,0XB0,0X7C,0X90,0X7C,0X91,
0X84,0XB2,0X8D,0X13,0X84,0XD2,0X74,0X70,0X7C,0X91,0X84,0XD2,0X7C,0XB1,0X7C,0XB1,
0X7C,0X91,0X74,0X50,0X74,0X30,0X74,0X70,0X7C,0X91,0X74,0X71,0X74,0X50,0X74,0X31,
0X74,0X31,0X63,0XAF,0X6C,0X2E,0X74,0X6F,0X6B,0XEE,0X74,0X2E,0X7C,0X6D,0X6B,0XEA,
0X74,0X2B,0X63,0XAC,0X74,0X30,0X63,0XEF,0X6C,0X0E,0X74,0X2E,0X94,0XD2,0X84,0X2F,
0X83,0XED,0X84,0X0D,0X7B,0X8C,0XA5,0X13,0XB5,0X54,0XB5,0X95,0XB5,0X74,0XBD,0XF5,
0XAD,0XB4,0XBE,0X56,0X9D,0X74,0XBE,0X78,0XAE,0X37,0XA6,0X16,0XA6,0X55,0X95,0XB2,
0X9D,0X53,0X95,0X12,0X7C,0X50,0X53,0X4C,0X5B,0XEE,0X4B,0X8C,0X4B,0X6B,0X4B,0X6B,
0X5B,0XCE,0X8D,0X54,0X53,0X6E,0X64,0X10,0X53,0XAE,0X5C,0X0F,0X64,0X50,0X53,0XEE,
0X64,0X2F,0X6C,0X30,0X63,0XEF,0X5B,0XCE,0X5B,0XCE,0X6C,0X50,0X5B,0XEE,0X9D,0XD6,
0XAE,0X78,0XCF,0X5C,0XBE,0XFA,0XB6,0XBA,0XCF,0X7C,0XB6,0XB9,0XA6,0X37,0XD7,0X5C,
0XE7,0X9D,0XFF,0XFF,0XF7,0XFF,0XD7,0X1B,0XE7,0X9E,0XCF,0X3C,0XCF,0X3D,0XBE,0XDB,
0XCF,0X5D,0XCF,0X1C,0XC6,0XDB,0XBE,0X9A,0X5B,0X6D,0X63,0XAD,0X3A,0X68,0X4A,0X88,
0X73,0X6C,0X7B,0X4A,0X62,0X45,0X61,0XE1,0XBC,0XA9,0XEE,0X0D,0XC4,0X87,0XDD,0X2A,
0XE5,0XEE,0XC5,0X0A,0X93,0XA3,0X8B,0X62,0X8B,0X85,0X93,0X85,0XAC,0X27,0XA4,0X26,
0XB4,0XA8,0XBD,0X4A,0X9C,0X07,0X9C,0X28,0XEF,0X55,0XFF,0XD9,0XFF,0XFB,0XF7,0XBB,
0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,0XFF,0XFC,0XFF,0XFC,0XFF,0XFC,0XFF,0XFD,0XFF,0XFD,
0XFF,0XDD,0XFF,0XFE,0XFF,0XFC,0XFF,0XDA,0XFF,0XD8,0XA5,0X0B,0XCD,0XEE,0XC5,0X8C,
0XF7,0X9C,0XF7,0X9C,0XC6,0X37,0XF7,0X9C,0XF7,0XBD,0XFF,0XFE,0XFF,0XFE,0XFF,0XBD,
0XFF,0XFE,0XDE,0XFA,0XCE,0X37,0XFF,0XDD,0XFF,0XDD,0XFF,0XFE,0XFF,0XFE,0XFF,0XDD,
0XFF,0XFE,0XD6,0XD9,0XCE,0X97,0XA5,0X52,0XCE,0X97,0XAD,0X73,0X9C,0XF1,0XBE,0X35,
0X84,0X8F,0X84,0X6F,0X6C,0X0D,0X8C,0XF1,0X7C,0X70,0X9D,0XB5,0X9D,0XB6,0XA5,0XF6,
0X74,0XB0,0X6C,0X8F,0X7C,0XB0,0X7C,0XB0,0X7C,0X70,0X84,0X70,0X84,0X71,0X84,0X92,
0X8C,0XF4,0X8C,0XF4,0X7C,0XB3,0X8D,0X34,0X74,0X91,0X95,0X33,0X84,0XB0,0X7C,0X6E,
0X8D,0X30,0X9D,0X93,0X85,0X33,0X8D,0X75,0X8D,0X56,0X84,0X93,0X8C,0X51,0X9C,0X90,
0XC6,0X13,0X9D,0X0F,0X84,0XEE,0X8D,0X71,0X8D,0X93,0X7C,0XF2,0X8D,0X33,0X8C,0XD2,
0X8C,0XF3,0X84,0XD2,0X8C,0XF3,0X95,0X34,0X8D,0X13,0X84,0XD2,0X84,0XB2,0X84,0XB2,
0X8D,0X13,0X8D,0X13,0X8C,0XD2,0X84,0XF2,0X95,0X13,0X8D,0X13,0X8C,0XF3,0X9D,0X75,
0X8D,0X16,0X8C,0XF4,0X8D,0X12,0X8D,0X11,0X84,0XD2,0X8C,0XD1,0X8C,0XEF,0X8C,0XCE,
0X8C,0XEF,0X84,0XD0,0X8D,0X14,0X8C,0XF4,0X8C,0XF1,0X8C,0XCF,0X94,0XB1,0X8C,0X4F,
0X84,0X0B,0XB5,0X71,0XBD,0XB4,0XEF,0X3A,0XEF,0X3B,0XDE,0XD9,0XFF,0XFC,0XF7,0XBA,
0XFF,0XFC,0XFF,0XFD,0XFF,0XFF,0XE7,0X9D,0XD7,0X3C,0XF7,0XFF,0XEF,0XFC,0XDF,0X9A,
0XCE,0XD9,0XDF,0X3B,0XBE,0X78,0X8D,0X12,0X85,0X12,0X74,0XB0,0X64,0X2F,0X5C,0X0E,
0X85,0X13,0XDF,0XDE,0X84,0XF3,0X6C,0X50,0X74,0XB2,0X74,0XD2,0X64,0X71,0X7D,0X13,
0X74,0X92,0X6C,0X71,0X5B,0XEF,0X53,0XAE,0X64,0X50,0X6C,0X91,0X53,0XAD,0X6C,0X91,
0X74,0XD2,0X74,0XB1,0X85,0X34,0X8D,0X95,0X8D,0X54,0X95,0X95,0XA6,0X18,0XAE,0X17,
0XAD,0XD5,0XDF,0X1A,0XCE,0XD9,0XA5,0X94,0XAE,0X17,0X9D,0XD6,0XAE,0X59,0XAE,0X7A,
0XB6,0XBB,0XAE,0X59,0XCF,0X3C,0XCF,0X1A,0X95,0X33,0X84,0X6F,0X19,0X21,0X5B,0X29,
0X52,0XA6,0X7B,0XAA,0X6A,0XA6,0X7A,0XE6,0XDD,0XEE,0XE6,0X0D,0XE6,0X0E,0XDD,0X8D,
0XB4,0XEB,0X94,0X07,0X7B,0X84,0XA4,0XCA,0XAC,0XAB,0X93,0XC8,0X9C,0X07,0XB4,0XA9,
0XD5,0XCC,0XC5,0X8B,0X73,0X02,0XDE,0X72,0XFF,0XFA,0XFF,0XFC,0XFF,0XFE,0XFF,0XDE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFE,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XBF,0XFF,0XDF,0XFF,0XDD,0XFF,0XFC,0XFF,0XF9,0XDE,0XD3,0XCE,0X0F,0XCE,0X0F,
0XF7,0XBD,0XCE,0X57,0X9C,0XF2,0XF7,0XDD,0XFF,0XFE,0XEF,0X7C,0XE7,0X5B,0XFF,0XFE,
0XF7,0XBD,0XD6,0XD9,0XAD,0X73,0XDE,0XFA,0XFF,0XDD,0XF7,0XBC,0XE7,0X3B,0XF7,0XBD,
0XFF,0XFE,0XE7,0X3A,0XD6,0XB8,0XAD,0X73,0XE7,0X3A,0XCE,0X97,0XAD,0X93,0XD6,0XD8,
0X8C,0XB0,0X8C,0XB0,0X73,0XED,0X8C,0XF1,0X8D,0X12,0XB6,0X79,0XB6,0X79,0XBE,0XDA,
0X7C,0XF2,0X6C,0X4F,0X74,0X4F,0X84,0XD1,0X74,0X2F,0X63,0X8D,0X63,0X8D,0X6B,0XEF,
0X7C,0X71,0X64,0X0F,0X5B,0XAD,0X74,0X70,0X8D,0X53,0XC6,0XD8,0XB6,0X35,0X95,0X30,
0X74,0X4D,0X7C,0XAF,0X6C,0X6E,0X6C,0XAF,0X85,0X12,0X7C,0X71,0X84,0X51,0X8C,0X70,
0X84,0X2E,0XC6,0X77,0X9D,0XB4,0X8D,0X54,0X74,0X93,0X74,0X73,0XA5,0XB8,0X95,0X14,
0X9D,0X55,0X95,0X14,0X9D,0X55,0X9D,0X55,0X8C,0XD3,0X95,0X14,0XA5,0X96,0X9D,0X75,
0X9D,0X34,0XAD,0XB6,0XA5,0X75,0X9D,0X34,0XA5,0X96,0X9D,0X55,0X95,0X14,0XAD,0XD8,
0XA5,0X98,0XA5,0X97,0X9D,0X74,0X9D,0X54,0X9D,0X36,0XA5,0X56,0XA5,0X74,0XAD,0XB4,
0XA5,0X74,0X9D,0X54,0X8C,0XF4,0X95,0X34,0X9D,0X72,0XA5,0X91,0X9D,0X30,0XA5,0X4F,
0XF7,0XB7,0XD6,0XD3,0XDE,0XF6,0XF7,0XBA,0XF7,0X9B,0XEF,0X5A,0XFF,0XFC,0XFF,0XDB,
0XFF,0XDC,0XF7,0X5B,0XFF,0XFF,0XFF,0XDF,0XFF,0XBF,0XFF,0X9E,0XFF,0XFE,0XFF,0XFD,
0XB5,0X95,0XDF,0X1B,0XC6,0XB9,0X95,0X74,0X8D,0X73,0X95,0XD5,0X8D,0X53,0X85,0X33,
0X9D,0XD5,0XE7,0XFE,0XA5,0XD6,0X95,0X95,0X85,0X34,0X8D,0X95,0X8D,0X95,0X8D,0XB6,
0X8D,0X75,0X8D,0X54,0X74,0XD2,0X5C,0X0F,0X74,0XF2,0X74,0XD2,0X5C,0X0F,0X5C,0X2F,
0X53,0XEE,0X5C,0X2F,0X5C,0X0F,0X53,0XCE,0X53,0XEE,0X53,0XCE,0X53,0XCE,0X5B,0XEE,
0X74,0X0E,0XB6,0X15,0XA5,0XD5,0X6B,0XEE,0X63,0XEF,0X53,0X8D,0X64,0X30,0X6C,0X92,
0X64,0X50,0X64,0X30,0X53,0X8D,0X64,0X0E,0X53,0X6B,0X74,0X4E,0X74,0X2E,0X63,0XAB,
0X4A,0XE6,0X74,0X0A,0X7B,0XCA,0X7B,0X88,0XCD,0XCE,0XAC,0XC9,0XB4,0XCB,0X9C,0X29,
0X73,0X27,0X8C,0X0A,0X84,0X09,0XA5,0X2E,0XA4,0XF0,0XAC,0XF1,0X8B,0XEB,0X73,0X05,
0XA4,0X89,0X8B,0XA5,0XCD,0XEF,0XDE,0X93,0XFF,0XFB,0XFF,0XFD,0XFF,0XDD,0XFF,0XDE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF7,0X9F,0XFF,0XDE,0XFF,0XFD,0XFF,0XB9,0XFF,0XF9,0XB5,0X6E,0XB5,0X6D,
0XFF,0XFE,0XF7,0XBC,0XC6,0X36,0XEF,0X7B,0XFF,0XDD,0XF7,0X9C,0XEF,0X7C,0XF7,0XDD,
0XFF,0XFE,0XFF,0XFE,0XD6,0X98,0XEF,0X7B,0XFF,0XFE,0XF7,0XBC,0XE7,0X3A,0XFF,0XFE,
0XF7,0XDD,0XEF,0X5B,0XEF,0X9C,0XDE,0XD9,0XFF,0XFD,0XF7,0XDC,0XD6,0XF8,0XE7,0X7A,
0XA5,0X93,0XA5,0X93,0X95,0X12,0XAD,0XF5,0X8C,0XF2,0X84,0XF2,0X63,0XEF,0X6C,0X50,
0X85,0X33,0X85,0X13,0X84,0XF1,0X95,0X33,0X6B,0XEE,0X53,0X0A,0X53,0X0A,0X5B,0X6C,
0X74,0X50,0X8D,0X13,0X95,0X94,0X85,0X12,0X8D,0X52,0X84,0XEF,0XB6,0X34,0X9D,0X92,
0X84,0X6F,0X84,0XD0,0X74,0X6E,0X6C,0X6E,0X7C,0XD0,0X7C,0X90,0X9D,0X13,0XB5,0XB6,
0XA5,0X33,0XF7,0XFF,0XAE,0X38,0X8D,0X76,0X7C,0XD4,0X8D,0X56,0XA5,0XD8,0X8C,0XB3,
0X63,0XAE,0X7C,0X50,0X9D,0X54,0X9D,0X33,0X84,0X91,0X95,0X33,0X9D,0X54,0X74,0X0F,
0X84,0X71,0X9D,0X54,0X94,0XF2,0X84,0XB1,0X9D,0X33,0X8C,0XB1,0X74,0X2F,0X8C,0XD3,
0X95,0X14,0X8C,0XB3,0X95,0X12,0X9D,0X33,0X8C,0X92,0X94,0XB4,0XA5,0X54,0X9D,0X13,
0X8C,0XD2,0XAD,0XB5,0XA5,0X75,0XAD,0XB5,0X9D,0X71,0XB6,0X11,0XC6,0X94,0XFF,0XFA,
0XFF,0XF8,0X94,0X69,0XBD,0XD1,0XF7,0X9A,0XF7,0XBC,0XFF,0XFD,0XDE,0XD8,0XFF,0XFD,
0XFF,0XFD,0XFF,0XDE,0XEE,0XFC,0XF7,0X1E,0XFF,0X7F,0XF6,0XFC,0XFF,0XBD,0XC5,0X74,
0XA5,0X33,0XD6,0XFA,0XBE,0X78,0X8D,0X12,0X8D,0X33,0X95,0XB5,0X8D,0X53,0X85,0X33,
0X8D,0X53,0XCF,0X1B,0XA5,0XF6,0XA6,0X17,0X8D,0X13,0X8D,0X74,0X85,0X34,0X74,0XD2,
0X8D,0X95,0X9E,0X17,0X95,0XD6,0X6C,0XD2,0X8D,0XB5,0X85,0X74,0X85,0X54,0X85,0X95,
0X7D,0X33,0X8D,0XD5,0X85,0X74,0X74,0XF2,0X85,0X54,0X85,0X74,0X7D,0X13,0X85,0X13,
0X9D,0X94,0XEF,0XFD,0XE7,0XDD,0XA5,0XD5,0X95,0X95,0X7C,0XD2,0X7D,0X14,0X85,0X54,
0X8D,0XB6,0X8D,0X95,0X74,0XB1,0X95,0X94,0X4B,0X4B,0X29,0XE5,0X6C,0X0D,0X74,0X4D,
0X5B,0X88,0X63,0XA9,0X8C,0XAD,0X7B,0XE9,0XA4,0XEB,0X8C,0X27,0X73,0X46,0X8B,0XEA,
0X8C,0X2C,0X8C,0X4D,0X84,0X4C,0XE7,0X38,0XFF,0XBD,0XFF,0XFE,0XE6,0XD8,0XDE,0X74,
0XF7,0X34,0XBD,0X4C,0XBD,0X4D,0XFF,0XFA,0XF7,0X79,0XF7,0X9B,0XFF,0XDD,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XDE,0XFF,0XFD,0XF7,0XBA,0XFF,0XFA,0XA5,0X0D,0X94,0X8B,
0XFF,0XDD,0XFF,0XFD,0XD6,0XB8,0XDE,0XF9,0XE7,0X3A,0XE7,0X3A,0XEF,0X9C,0XF7,0XDC,
0XFF,0XFE,0XFF,0XFD,0XF7,0XDD,0XFF,0XFD,0XDE,0XF9,0XDE,0XD9,0XEF,0X7B,0XEF,0X7B,
0XFF,0XFE,0XEF,0X5B,0XFF,0XFE,0XF7,0XBC,0XF7,0XDC,0XF7,0XDC,0XEF,0X7B,0XE7,0X7A,
0X94,0XD0,0X95,0X11,0X9D,0X73,0XC6,0XD9,0X8D,0X13,0X63,0XEE,0X4B,0X0B,0X64,0X0F,
0X8D,0X34,0XAE,0X38,0X95,0X54,0X8C,0XF2,0X53,0X2B,0X42,0X67,0X3A,0X87,0X4B,0X0A,
0X84,0XF1,0XA5,0XF5,0X95,0X93,0X95,0X73,0X8D,0X10,0X6C,0X0C,0XAE,0X13,0XA5,0X72,
0XAD,0X75,0XB5,0XB7,0XA5,0X95,0X9D,0X94,0X84,0XD1,0X74,0X4F,0X9D,0X13,0XB5,0XB7,
0XAD,0X97,0XF7,0XFF,0XE7,0XFF,0XCF,0XBE,0X9E,0X37,0X7D,0X53,0X8D,0X53,0XB6,0X77,
0X95,0X32,0XA5,0XB3,0XB6,0X76,0XAE,0X15,0X95,0X52,0XAD,0XF5,0XB6,0X56,0X95,0X31,
0X95,0X52,0XAE,0X15,0X9D,0X73,0X8D,0X31,0XA5,0XF4,0X9D,0X93,0X8D,0X11,0X9D,0X93,
0XA5,0XB4,0X8C,0XD0,0XA5,0X92,0XB5,0XF3,0X8C,0XB1,0X9D,0X13,0XB5,0XF5,0X95,0X11,
0XC6,0X76,0XB6,0X15,0X95,0X11,0XB5,0XF4,0XDF,0X56,0XFF,0XF9,0XF7,0XF9,0XFF,0XFA,
0XE6,0XB4,0X7B,0X27,0XAC,0XAF,0XFF,0X7C,0XEF,0X1B,0XF7,0X7D,0XC6,0X78,0XF7,0XDE,
0XD6,0XDA,0XA5,0X76,0X94,0XB4,0XC6,0X1A,0XFF,0XDF,0XFF,0X9E,0XEE,0XF9,0X94,0X4E,
0XD6,0X98,0XF7,0XDE,0XD6,0XFA,0X9D,0X94,0XA5,0XD5,0X9D,0XB4,0X8D,0X32,0X95,0X94,
0X5B,0XAC,0XC6,0XDA,0X8D,0X12,0X53,0X6C,0X42,0XEA,0X53,0X8C,0X32,0X88,0X22,0X47,
0X53,0XEE,0X6C,0XB1,0X6C,0XB1,0X53,0XEE,0X75,0X13,0X75,0X12,0X75,0X33,0X7D,0X53,
0X75,0X33,0X6C,0XD1,0X7D,0X54,0X7D,0X74,0X6C,0XB1,0X7D,0X53,0X8D,0XB5,0X6C,0X70,
0XA5,0XD5,0XEF,0XFD,0XEF,0XFE,0XB6,0X57,0XA6,0X16,0X85,0X54,0X8D,0X95,0X85,0X74,
0X8D,0XB6,0X8D,0X95,0X85,0X33,0X8D,0X53,0X74,0XB0,0X63,0XED,0X63,0XED,0XA5,0XD4,
0X9D,0X91,0X8D,0X0F,0XA5,0X72,0X9C,0XCF,0X94,0X6B,0X7B,0XA8,0X6B,0X28,0X9C,0X8E,
0X9C,0XB0,0XAD,0X52,0XB5,0XB2,0XFF,0XFC,0XFF,0XFD,0XFF,0XFE,0XFF,0XBB,0XFF,0XFA,
0XDE,0XB3,0XB5,0X4D,0XBD,0XB0,0XFF,0XFB,0XF7,0X7A,0XFF,0XBD,0XFF,0XFE,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDF,0XFF,0XFF,0XFF,0XDE,0XFF,0XFD,0XFF,0XFD,0XFF,0XFB,0XC6,0X13,0XA4,0XEE,
0XF7,0XBC,0XFF,0XFD,0XCE,0X97,0XD6,0XB8,0XE7,0X3A,0XD6,0X98,0XE7,0X1A,0XFF,0XDD,
0XE7,0X3A,0XFF,0XFD,0XFF,0XFD,0XF7,0XBC,0XD6,0X97,0XCE,0X77,0XDE,0XD8,0XE7,0X1A,
0XEF,0X7B,0XD6,0XB9,0XFF,0XFE,0XFF,0XFE,0XF7,0XBC,0XFF,0XFD,0XF7,0XBC,0XE7,0X7A,
0XC6,0X57,0XB5,0XF5,0XBE,0X57,0XE7,0X9C,0X84,0XD1,0X4A,0XEB,0X32,0X69,0X5B,0XAE,
0X8D,0X14,0XC6,0XFB,0X8D,0X13,0X6B,0XEE,0X3A,0X88,0X3A,0X47,0X3A,0X67,0X4B,0X09,
0X85,0X11,0XC6,0XF9,0XAE,0X36,0XB6,0X97,0X4B,0X09,0X29,0XE4,0X6B,0XCB,0XA5,0X72,
0XC5,0XD7,0XC5,0XD8,0XBD,0XF8,0XC6,0XB9,0X84,0XD0,0X6B,0XCC,0X7B,0XEE,0X84,0X30,
0XB5,0X97,0XE7,0X5F,0XEF,0XFF,0XE7,0XFE,0XE7,0XFC,0XE7,0XFB,0XCE,0XD8,0XD7,0X19,
0XDF,0X59,0XC6,0XB6,0XBE,0X75,0XB6,0X13,0X9D,0X71,0XAD,0XD2,0XC6,0X95,0XC6,0XB6,
0XCE,0XD6,0XD7,0X17,0XC6,0X75,0XBE,0X54,0XD7,0X17,0XD7,0X38,0XCE,0XD6,0XDF,0X38,
0XEF,0X79,0XD6,0X95,0XF7,0X77,0XFF,0XFA,0XDE,0XF7,0XEF,0X59,0XFF,0XFC,0XE7,0X58,
0XEF,0X79,0XFF,0XFA,0XFF,0XDB,0XFF,0XFB,0XFF,0XF9,0XFF,0XF9,0XF7,0XFA,0XFF,0XFA,
0X9B,0XEA,0X72,0X65,0X72,0X88,0XE6,0X58,0XF7,0X3C,0XFF,0XBF,0XFF,0XFF,0XDF,0X3B,
0X4A,0XCA,0X29,0XC7,0X52,0XCC,0X52,0XCC,0XAD,0X35,0XFF,0X9D,0XD6,0X76,0XCE,0X15,
0XF7,0XBC,0XFF,0XFF,0XE7,0X5C,0XB6,0X16,0XB6,0X78,0X84,0XD1,0X7C,0X90,0XA6,0X16,
0X7C,0X70,0XE7,0XDD,0XBE,0X57,0X53,0X2B,0X11,0X64,0X64,0X0E,0X6C,0X4F,0X32,0XA8,
0X7D,0X12,0X8D,0XB5,0X7D,0X12,0X75,0X12,0X9E,0X57,0X96,0X37,0X8D,0XD5,0X75,0X53,
0X7D,0X53,0X85,0X94,0X85,0XB5,0X7D,0X73,0X7D,0X33,0X85,0X53,0X7D,0X33,0X74,0X90,
0XBE,0X98,0XEF,0XFD,0XE7,0XDD,0XAE,0X57,0X9D,0XD5,0X7D,0X13,0X85,0X34,0X6C,0XB2,
0X6C,0X91,0X8D,0XD6,0X8D,0X95,0X7D,0X12,0X32,0X88,0X22,0X06,0X4B,0X2A,0X7C,0XAF,
0X74,0X4D,0X84,0X8E,0X7C,0X0D,0X94,0XB0,0X94,0X8D,0X9C,0X8D,0XD6,0X76,0XFF,0X7B,
0XFF,0XDD,0XFF,0XDD,0XDE,0XF7,0XF7,0XDA,0XFF,0XDC,0XFF,0XDC,0XB5,0XD2,0X73,0X87,
0X52,0X83,0X5A,0X84,0XC5,0XD2,0XEF,0X39,0XF7,0XBC,0XFF,0XFE,0XFF,0XFE,0XF7,0XBD,
0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF7,0XBF,0XFF,0XFF,0XFF,0XDE,0XF7,0XDD,0XFF,0XFD,0XF7,0X9B,0XE7,0X19,0XBD,0XD3,
0XCE,0X56,0XF7,0XDC,0XC6,0X56,0XA5,0X52,0XBD,0XF5,0XC6,0X36,0XC6,0X56,0XB5,0XB4,
0XBE,0X15,0XFF,0XFC,0XE7,0X19,0XD6,0XD8,0XD6,0XB8,0XCE,0X97,0XC6,0X35,0XDE,0XD9,
0XD6,0XB9,0XC6,0X37,0XEF,0X7B,0XFF,0XDD,0XFF,0XDD,0XFF,0XFD,0XE7,0X3A,0XDF,0X19,
0XBE,0X16,0X9D,0X32,0XAD,0XD5,0XDF,0X7C,0X84,0XD2,0X63,0XCE,0X63,0XAE,0X7C,0XB2,
0X8D,0X34,0XD7,0X7D,0X84,0XD2,0X63,0XCD,0X5B,0X6B,0X63,0XAC,0X5B,0X6B,0X6C,0X0D,
0X7C,0XD0,0X8D,0X52,0X7C,0XD1,0XB6,0X77,0X63,0XCC,0X6B,0XCC,0X7C,0X0D,0XC6,0X36,
0XE6,0XDA,0XBD,0XD7,0XAD,0XB7,0XDF,0X7D,0X8C,0XF0,0X84,0X6D,0X84,0X0C,0X7B,0X8C,
0XBD,0X97,0XFF,0X7E,0XFF,0XFE,0XEF,0XBA,0XF7,0XF8,0XFF,0XF9,0XFF,0XDB,0XFF,0X7A,
0XFF,0XFB,0XFF,0XFA,0XFF,0XFA,0XFF,0XFA,0XFF,0XFA,0XFF,0XD9,0XFF,0XD9,0XFF,0XFA,
0XFF,0XFA,0XFF,0XFA,0XFF,0XB9,0XF7,0X78,0XFF,0XFA,0XFF,0XFA,0XFF,0XFA,0XFF,0XFA,
0XFF,0XB8,0XFF,0X57,0XFF,0X97,0XFF,0XD8,0XFF,0X97,0XFF,0XD8,0XFF,0XF9,0XFF,0XD8,
0XFF,0XD8,0XFF,0XD8,0XFF,0XF9,0XFF,0X98,0XFF,0X99,0XFF,0X9A,0XFF,0X5B,0XEE,0X97,
0X8A,0XE6,0X82,0X43,0X61,0X82,0XC4,0XF2,0XFF,0XBE,0XFF,0XBE,0XFF,0XBD,0X5B,0X0A,
0X19,0X43,0X5B,0X2C,0X8C,0X92,0X21,0X46,0X52,0XAA,0XFF,0XBD,0XE6,0XD7,0XDE,0XB7,
0XFF,0XFE,0XFF,0XFF,0XF7,0XBD,0XC6,0X98,0XC6,0X98,0X4B,0X0A,0X53,0X2B,0XA5,0XD5,
0X95,0X33,0XE7,0XBC,0XD7,0X3B,0X6B,0XED,0X00,0X80,0X74,0X4F,0XAE,0X56,0X5B,0XED,
0XAE,0X78,0XB6,0XD9,0X8D,0X74,0X95,0XD5,0XAE,0XB9,0XA6,0X98,0X85,0X95,0X6C,0XF2,
0X75,0X13,0X9E,0X57,0X8D,0XB5,0X7D,0X53,0XA6,0X57,0X9E,0X16,0X85,0X53,0XA6,0X16,
0XDF,0X9C,0XF7,0XFF,0XF7,0XFF,0XC6,0XD9,0XA6,0X17,0X8D,0X54,0X8D,0X95,0X6C,0X91,
0XA6,0X58,0X95,0XB5,0X7C,0XD2,0XC7,0X1B,0X63,0XEE,0X21,0XE6,0X32,0X88,0X42,0X87,
0X8C,0X8E,0X94,0XCE,0X94,0X4E,0X8C,0X0D,0X94,0X0C,0XAC,0XCF,0XFF,0X7B,0XFF,0XDD,
0XFF,0XDD,0XFF,0XBC,0XF7,0X78,0XFF,0XB9,0XCE,0X14,0X83,0XEB,0X6B,0X46,0X73,0X87,
0X73,0X87,0X73,0X68,0X42,0X04,0XF7,0X7B,0XFF,0XDE,0XFF,0XFF,0XFF,0XFE,0XFF,0XFD,
0XFF,0XFE,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF7,0XDE,0XFF,0XFF,0XFF,0XDE,0XF7,0XBD,0XFF,0XFE,0XEF,0X9C,0XEF,0X7B,0XD6,0XB8,
0XDE,0XF9,0XF7,0XBC,0XCE,0X77,0XD6,0XB8,0XFF,0XFD,0XF7,0XDC,0XFF,0XFC,0XFF,0XFD,
0XE7,0X5A,0XFF,0XDC,0XF7,0XBC,0XFF,0XFD,0XF7,0XBC,0XFF,0XFD,0XFF,0XFD,0XF7,0XBC,
0XFF,0XFD,0XEF,0X7C,0XF7,0X9C,0XF7,0X9C,0XFF,0XFE,0XFF,0XFD,0XD6,0XB8,0XDE,0XF9,
0XBE,0X15,0X94,0XF1,0XAD,0XD5,0XEF,0XDD,0X9D,0X95,0X95,0X34,0X95,0X75,0X9D,0X96,
0X9D,0X95,0XEF,0XFF,0X95,0X13,0X7C,0X70,0X95,0X12,0XA5,0XB4,0X8D,0X11,0X9D,0X93,
0X8D,0X32,0X7C,0X90,0XA5,0XB5,0XCF,0X1B,0X9D,0X54,0X9D,0X12,0X94,0X90,0XE7,0X1A,
0XFF,0XFE,0XC6,0X17,0X9D,0X55,0XDF,0X9E,0X8C,0XF1,0X95,0X10,0X94,0X6E,0X83,0XAB,
0XFF,0X7C,0XFF,0X5B,0XE6,0XD5,0XCE,0X10,0XEE,0XD0,0XE6,0X2E,0XF6,0X31,0XE5,0X70,
0XCC,0XEC,0XDD,0X8E,0XDD,0XCE,0XDD,0XAE,0XE5,0XEF,0XEE,0X30,0XE5,0XEF,0XE5,0XCF,
0XE5,0XEF,0XEE,0X10,0XE6,0X10,0XE5,0XEF,0XE6,0X10,0XEE,0X10,0XDD,0XCF,0XD5,0X6D,
0XE5,0XCF,0XEE,0X2F,0XE5,0XAC,0XDD,0X4A,0XED,0XCC,0XDD,0X8C,0XC5,0X2A,0XCD,0X6B,
0XCD,0X4B,0XCD,0X09,0XE5,0X6B,0XD4,0XC9,0XDD,0X0C,0XE5,0X70,0XF6,0X16,0XD5,0X52,
0X82,0X85,0X48,0XE0,0X38,0X80,0X7A,0XC9,0XFF,0X7D,0XFF,0X9D,0XF7,0X9C,0X42,0X47,
0X29,0XC4,0X73,0XEE,0XB5,0XF7,0XB5,0XD7,0XDE,0XBA,0XFF,0XFD,0XBD,0X92,0XC5,0XB2,
0XFF,0XFE,0XF7,0X9D,0XFF,0XFF,0XDF,0X1B,0XD7,0X3B,0X3A,0X88,0X4B,0X2A,0XC6,0XB8,
0X95,0X33,0XF7,0XFE,0XE7,0X7B,0X74,0X0E,0X11,0X22,0X63,0XED,0X95,0X73,0X7C,0XB0,
0XBE,0XB8,0XC6,0XFA,0X8D,0X53,0X95,0XD5,0XA6,0X58,0X9E,0X37,0X85,0X74,0X75,0X12,
0X7D,0X33,0X85,0X74,0X85,0X74,0X85,0X53,0X8D,0X74,0X8D,0X73,0X8D,0X53,0X95,0X74,
0XDF,0X7C,0XEF,0XDE,0XF7,0XFF,0XDF,0X7C,0XC6,0XB9,0XAE,0X17,0XB6,0X79,0X85,0X34,
0X9D,0XF7,0XA6,0X38,0XA6,0X17,0XB6,0XB9,0X74,0X91,0X6C,0X2F,0X32,0X27,0X63,0X8B,
0XB5,0XD2,0XAD,0X4F,0XC5,0XF4,0X7B,0XAB,0X8B,0XEB,0XBD,0X91,0XFF,0X7B,0XFF,0XBD,
0XFF,0XDD,0XF7,0X3A,0XC5,0XB2,0XA4,0XCE,0X94,0X4D,0X94,0X0C,0X9C,0X8C,0X94,0X6B,
0X84,0X2B,0X7B,0XEB,0X73,0X8B,0XCE,0X58,0XF7,0XBE,0XFF,0XFF,0XFF,0XDD,0XFF,0XFD,
0XFF,0XFD,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDE,0XFF,0XFE,0XF7,0XDD,0XF7,0XBD,0XFF,0XFE,0XFF,0XDE,0XEF,0X9D,0XEF,0X7C,
0XCE,0X77,0XFF,0XFD,0XD6,0XD8,0XC6,0X15,0X8C,0X8F,0X94,0XB0,0X73,0XAB,0XB5,0X93,
0XD6,0XB8,0XF7,0XDC,0XEF,0X9B,0XEF,0X5A,0XA5,0X11,0X84,0X4E,0X6B,0X4A,0X7C,0X0D,
0XDE,0XD9,0XE7,0X1A,0XFF,0XDD,0XFF,0XDD,0XFF,0XFE,0XFF,0XFD,0XDE,0XD9,0XBE,0X15,
0X8C,0X8F,0X73,0XED,0XB6,0X16,0XDF,0X7C,0XAD,0XF7,0X5B,0X6D,0X4B,0X0C,0X95,0X75,
0XCF,0X1B,0XEF,0XFE,0X95,0X12,0X6B,0XAD,0X73,0XED,0X6B,0XED,0X7C,0X4F,0X5B,0XAC,
0X6C,0X0F,0X7C,0X91,0XB6,0X38,0XDF,0X3C,0X84,0X51,0X63,0X2C,0X94,0X50,0XFF,0XBE,
0XFF,0XFE,0XEF,0XBE,0XD7,0X7E,0XDF,0XBF,0X8C,0XD3,0X5A,0XEA,0X62,0X67,0XC5,0X30,
0XFF,0XB9,0XBC,0X6A,0XC5,0X09,0XCD,0X07,0XDD,0X67,0XCC,0X64,0XE4,0X67,0XE4,0X46,
0XDC,0X45,0XEC,0XC7,0XEC,0XC7,0XDC,0X65,0XDC,0X45,0XE4,0X86,0XE4,0XA6,0XDC,0X65,
0XE4,0X86,0XE4,0X86,0XE4,0X86,0XE4,0X86,0XE4,0X85,0XDC,0X65,0XDC,0X65,0XD4,0X65,
0XD4,0XA5,0XD4,0XC5,0XDC,0XA4,0XDC,0XC4,0XE5,0X05,0XE5,0X46,0XDD,0X47,0XCD,0X05,
0XCC,0XA3,0XE5,0X24,0XF5,0X23,0XEC,0X82,0XEC,0X84,0XEC,0XA8,0XFD,0XAF,0XB4,0X0A,
0X8B,0X69,0X52,0X04,0X5A,0X48,0X94,0X50,0XF7,0X9E,0XFF,0XFF,0XD7,0X3B,0XAD,0XF5,
0XB6,0X36,0XCE,0XF9,0XF7,0XFE,0XF7,0XBD,0XFF,0XFD,0XEE,0XF8,0X6A,0XA5,0X83,0X89,
0XFF,0XFD,0XFF,0XBE,0XFF,0XFF,0XCE,0X99,0XEF,0XDD,0X63,0X8C,0X53,0X2B,0XBE,0X98,
0XAE,0X16,0XE7,0XBC,0XDF,0X5B,0X84,0X70,0X11,0X02,0X74,0X4E,0XAE,0X36,0X84,0XD0,
0XC6,0XD9,0XCE,0XFA,0X95,0X74,0XA5,0XF6,0XAE,0X58,0XBE,0XFA,0X74,0XB1,0X43,0X2C,
0X6C,0XB1,0X8D,0X74,0X4B,0X8D,0X5B,0XAD,0XA5,0XF6,0X7C,0X91,0X5B,0X8C,0XA5,0XB5,
0XEF,0XDE,0XFF,0XFF,0XFF,0XFF,0XE7,0XBD,0XC6,0X99,0XBE,0X99,0XBE,0X99,0X7C,0XB2,
0X95,0X95,0XBE,0XDA,0XBE,0X99,0XC6,0XDA,0XAD,0XF7,0XA5,0XB6,0X5B,0X8D,0X63,0X6B,
0XAD,0X8F,0X9C,0XEB,0X9C,0XCD,0X9C,0XCE,0X9C,0XAC,0XC5,0XF2,0XFF,0XFD,0XFF,0XDD,
0XFF,0X9D,0XFF,0X9C,0XFF,0X9B,0XD6,0X35,0X8B,0XEE,0XA4,0XB1,0XAD,0X11,0XAD,0X10,
0X73,0XCA,0X7B,0XEC,0XAD,0X54,0XDE,0XDB,0XF7,0X9F,0XFF,0XFF,0XFF,0XFD,0XFF,0XFC,
0XF7,0XBA,0XFF,0XFC,0XFF,0XDE,0XFF,0X9E,0XFF,0XFF,0XF7,0X9F,0XFF,0XDE,0XFF,0XDD,
0XFF,0XDC,0XFF,0XFC,0XFF,0XFD,0XF7,0X9C,0XFF,0XDE,0XFF,0XFF,0XEF,0X7D,0XEF,0X7D,
0XE7,0X5A,0XFF,0XFD,0XB5,0XD4,0XCE,0X97,0XC6,0X15,0X73,0XCC,0X29,0X63,0XAD,0X73,
0XE7,0X3A,0XFF,0XFD,0XFF,0XFC,0XFF,0XFD,0XD6,0XB8,0XA5,0X32,0X52,0XC8,0X3A,0X05,
0XE7,0X1A,0XE7,0X3B,0XFF,0XDD,0XFF,0XDD,0XFF,0XFD,0XFF,0XFE,0XE7,0X3A,0XD6,0X98,
0XC6,0X57,0X9D,0X33,0XBE,0X57,0XE7,0X9D,0XA5,0X95,0X42,0XAA,0X21,0XC7,0X63,0XCF,
0XE7,0X9D,0XF7,0XFF,0X9D,0X74,0X74,0X0E,0X52,0XEA,0X19,0X64,0X29,0XE6,0X21,0X85,
0X9D,0X96,0XAD,0XF7,0XC6,0X9A,0XE7,0X5D,0X52,0X8A,0X5A,0X69,0X5A,0X48,0XF7,0X1B,
0XFF,0XFE,0XEF,0X7C,0XF7,0XFE,0XEF,0X9E,0XDE,0XFC,0XD6,0X59,0X93,0XAD,0XFE,0X75,
0XDD,0X4C,0XBC,0X45,0XD5,0X26,0XD5,0X26,0XE5,0X88,0XE4,0XE6,0XF4,0XE7,0XF4,0XA6,
0XEC,0XA6,0XED,0X08,0XED,0X08,0XE4,0XA7,0XE4,0XA6,0XE4,0XC7,0XEC,0XC7,0XE4,0XA6,
0XEC,0XE8,0XEC,0XE8,0XEC,0XE7,0XEC,0XE7,0XEC,0XC7,0XEC,0XC7,0XEC,0XE7,0XED,0X08,
0XDD,0X28,0XE5,0X48,0XED,0X28,0XED,0X07,0XED,0X27,0XED,0X48,0XDD,0X69,0XDD,0X48,
0XED,0XA8,0XFD,0XC6,0XFD,0X22,0XFD,0X42,0XFC,0XC3,0XFD,0X06,0XDC,0XC8,0XD5,0X2C,
0XB4,0XEE,0XDE,0XB7,0XFF,0XFE,0XFF,0XBE,0XF7,0XBE,0XEF,0XBD,0XF7,0XFC,0XF7,0XFB,
0XEF,0XDA,0XF7,0XDB,0XFF,0XFD,0XFF,0XBC,0XFF,0XDC,0XC5,0X92,0X62,0XA5,0X6A,0XC6,
0XF7,0X3A,0XFF,0XDF,0XFF,0XDE,0XBE,0X17,0XE7,0X9C,0X53,0X2A,0X5B,0X4B,0XCE,0XD9,
0XC6,0XB8,0XF7,0XFE,0XDF,0X5B,0X7C,0X4F,0X10,0XE2,0X7C,0X4F,0XB6,0X56,0X84,0XF1,
0XD7,0X1A,0XD7,0X3B,0X9D,0X74,0XAE,0X17,0XAE,0X37,0XB6,0X99,0X6C,0X50,0X22,0X27,
0X85,0X13,0XA6,0X17,0X3A,0XEA,0X53,0X8D,0XC6,0XFA,0X6C,0X0E,0X3A,0X68,0XD7,0X1B,
0XFF,0XFF,0XFF,0XDE,0XEF,0XBD,0XE7,0X5C,0XC6,0X99,0XCE,0XDA,0XC6,0X99,0X74,0X30,
0XBE,0X99,0XC6,0XDB,0XCF,0X1B,0XDF,0X7D,0X8C,0XF3,0X95,0X55,0X95,0X14,0X9D,0X53,
0X95,0X0D,0X84,0X28,0X8C,0X6C,0XB5,0XB2,0X6B,0XA8,0XB5,0X91,0XFF,0XDC,0XFF,0XBD,
0XFF,0XFF,0XFF,0X9D,0XFF,0XFD,0XFF,0XFD,0XDE,0X79,0XFF,0X7D,0XFF,0XFE,0XF7,0X9B,
0XFF,0XFC,0XEF,0X9A,0XF7,0X9D,0XFF,0XBF,0XFF,0XDF,0XFF,0XDF,0XFF,0XDC,0XFF,0XDA,
0XFF,0XFB,0XFF,0XFC,0XF7,0X9C,0XE7,0X1C,0XF7,0XBF,0XF7,0X9E,0XFF,0XFF,0XFF,0XFD,
0XFF,0XFC,0XF7,0XDA,0XFF,0XFC,0XFF,0XFD,0XFF,0XFE,0XFF,0XFF,0XE7,0X5D,0XF7,0XBF,
0XF7,0XDC,0XF7,0XDC,0XBE,0X15,0XEF,0XBC,0XDF,0X3A,0XAD,0X93,0X6B,0XAC,0XCE,0X77,
0XFF,0XFD,0XFF,0XFD,0XEF,0X9B,0XF7,0XBC,0XEF,0X9B,0XD6,0XB8,0X9D,0X11,0X84,0X2E,
0XEF,0X5C,0XEF,0X5C,0XFF,0XDD,0XFF,0XBD,0XFF,0XDD,0XFF,0XFE,0XEF,0X5B,0XDE,0XF9,
0XC6,0X57,0X9D,0X32,0XAD,0X95,0XE7,0X9C,0XAD,0XD7,0X5B,0X6D,0X32,0X49,0X7C,0X91,
0XDF,0X9D,0XF7,0XFE,0XB5,0XF6,0X94,0XF2,0X63,0X8C,0X29,0XC6,0X4A,0XCA,0X3A,0X49,
0XBE,0X59,0XD7,0X1D,0XD7,0X1C,0XE7,0X3D,0X5A,0X6A,0X5A,0X28,0X8B,0X8D,0XFF,0X9D,
0XFF,0X7B,0XFF,0X9B,0XFF,0X9A,0XFF,0XDC,0XFF,0X9E,0XFF,0X1C,0XBC,0X70,0XFE,0XF6,
0XC4,0X65,0XE5,0XA7,0XE6,0X09,0XDD,0XCA,0XE6,0X0E,0XE5,0XAE,0XF5,0XEC,0XF5,0XCC,
0XDD,0XAD,0XDD,0XAF,0XDD,0XCF,0XDD,0XCF,0XDD,0XCF,0XDD,0XCF,0XDD,0XCF,0XDD,0XCF,
0XDD,0XCF,0XDD,0XCF,0XDD,0XAF,0XD5,0X8E,0XD5,0X8E,0XD5,0X8E,0XDD,0XCF,0XDD,0XF0,
0XCD,0X4E,0XD5,0X6F,0XDD,0X4F,0XDD,0X0F,0XDD,0X0E,0XD5,0X2F,0XCD,0X31,0XCD,0X50,
0XD5,0X4D,0XD4,0XC7,0XFD,0X45,0XFE,0X27,0XFC,0XE4,0XFD,0X68,0XB3,0XE4,0XAC,0X68,
0XC5,0X6E,0XFF,0X98,0XFF,0XFC,0XFF,0X9A,0XFF,0X7A,0XFF,0XFB,0XFF,0XF9,0XFF,0XF8,
0XFF,0XF8,0XF7,0XB7,0XFF,0XFB,0XFF,0X9A,0XFF,0X7A,0X94,0X0C,0X62,0XA5,0X4A,0X03,
0XC5,0XB4,0XFF,0XFF,0XFF,0XDE,0XBE,0X37,0XEF,0XBD,0X4A,0XC9,0X5B,0X6C,0XD7,0X1A,
0XCE,0XF9,0XF7,0XFE,0XDF,0X3B,0X7C,0X2F,0X11,0X02,0X7C,0X90,0XBE,0XB8,0X95,0X53,
0XDF,0X5C,0XE7,0X7C,0X9D,0X74,0XB6,0X37,0XB6,0X58,0XBE,0XBA,0X85,0X14,0X19,0XE7,
0X9D,0XD7,0XB6,0XBA,0X53,0X4C,0X6C,0X30,0XD7,0X3B,0X7C,0X50,0X63,0X8D,0XF7,0XDE,
0XFF,0XDE,0XFF,0XFF,0XFF,0XFF,0XF7,0XBE,0XC6,0X58,0XCE,0XBA,0XCE,0XFA,0X84,0X91,
0XDF,0X7D,0XC6,0XBA,0XCE,0XFB,0XE7,0XBE,0X63,0XAE,0X84,0XB3,0XB6,0X18,0XC6,0X98,
0X6B,0XAA,0X6B,0XA9,0X84,0X8F,0XBE,0X56,0X7C,0X4D,0X9D,0X72,0XE7,0X7C,0XFF,0XFF,
0XEF,0X7D,0XEF,0X7C,0XE7,0X19,0XFF,0XDC,0XFF,0XDE,0XFF,0XFF,0XF7,0XFD,0XF7,0XDC,
0XF7,0XFB,0XF7,0XDC,0XFF,0XDF,0XFF,0XFF,0XFF,0XDF,0XFF,0XDE,0XFF,0XDB,0XFF,0XB9,
0XEF,0X56,0XFF,0XB9,0XFF,0XFD,0XFF,0XDF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFE,0XFF,0XFC,
0XFF,0XD9,0XEF,0X77,0XFF,0XBA,0XFF,0XFC,0XFF,0XDD,0XEF,0X7D,0XDE,0XDB,0XE7,0X3D,
0XFF,0XFE,0XFF,0XFD,0XC6,0X56,0XEF,0X7B,0XD6,0XD9,0XC6,0X36,0X94,0XD1,0XC6,0X56,
0XFF,0XFD,0XF7,0XDC,0XF7,0XBC,0XF7,0XDC,0XF7,0XDD,0XD6,0XB8,0XAD,0X73,0X94,0XB1,
0XF7,0XBD,0XEF,0X7C,0XFF,0XBD,0XF7,0XBD,0XFF,0XDD,0XFF,0XDD,0XE7,0X1A,0XD6,0XB8,
0XCE,0X77,0XA5,0X73,0XA5,0X94,0XEF,0XDE,0XB6,0X17,0X63,0X8E,0X32,0X49,0X84,0XB2,
0XDF,0X9C,0XF7,0XFE,0XBE,0X37,0X95,0X13,0X6B,0XCE,0X42,0X89,0X73,0XF0,0X3A,0X49,
0XBE,0X39,0XDF,0X7E,0XDF,0X3D,0XDE,0XBA,0X83,0X8D,0X72,0XA9,0XFE,0XF9,0XFF,0X79,
0XE5,0X90,0XED,0XD0,0XDD,0XAF,0XE5,0XF2,0XED,0XF6,0XB4,0X0F,0XED,0X93,0XF5,0X8F,
0XC4,0X64,0XE5,0XE9,0XAC,0X86,0X83,0X65,0X7B,0X29,0X6A,0XC8,0X72,0XC5,0X72,0XC4,
0X5A,0X65,0X52,0X46,0X52,0X46,0X5A,0X87,0X5A,0X87,0X52,0X46,0X52,0X26,0X52,0X67,
0X52,0X26,0X52,0X26,0X52,0X26,0X4A,0X05,0X49,0XE5,0X49,0XE5,0X52,0X26,0X52,0X47,
0X4A,0X06,0X49,0XE7,0X51,0XA7,0X59,0X87,0X51,0X86,0X51,0XA7,0X41,0XC9,0X49,0XE8,
0X51,0XC4,0X51,0X60,0XD4,0X63,0XFD,0X65,0XE4,0XA4,0XE5,0X48,0X9B,0XC3,0X72,0XC1,
0XCD,0X6B,0XEE,0X50,0XDD,0XAE,0XE5,0XF0,0XE6,0X10,0XEE,0X30,0XE6,0X0D,0XEE,0X6E,
0XEE,0X6F,0XE6,0X50,0XE6,0X52,0XFF,0X79,0XE6,0X35,0X73,0X09,0X5A,0X25,0X39,0X42,
0X83,0XAD,0XFF,0XDE,0XFF,0XFF,0XCE,0X98,0XEF,0XDD,0X42,0X88,0X53,0X4B,0XD7,0X1A,
0XCE,0XD9,0XF7,0XFE,0XDF,0X3B,0X7C,0X2F,0X11,0X02,0X84,0X90,0XBE,0XB8,0X95,0X53,
0XE7,0X9D,0XE7,0X7D,0X9D,0X54,0XAD,0XF7,0XB6,0X38,0XBE,0XDA,0X9D,0XD7,0X22,0X08,
0X9D,0XD7,0XAE,0X79,0X4B,0X4C,0X74,0X71,0XD7,0X3C,0X7C,0X71,0X73,0XEE,0XEF,0X9D,
0XF7,0X9E,0XFF,0XBE,0XFF,0XFF,0XF7,0XBE,0XC6,0X38,0XCE,0X99,0XD6,0XFA,0X84,0XB1,
0XE7,0XBE,0XC6,0XB9,0XC6,0XBA,0XE7,0XBE,0X5B,0X8E,0X84,0XB3,0XBE,0X5A,0XBE,0X79,
0X8C,0X8F,0X7C,0X2D,0X73,0XEF,0XAD,0XD7,0XEF,0XFD,0XEF,0XFE,0XEF,0XFF,0XEF,0XDF,
0XFF,0XFF,0XF7,0XDC,0XDE,0XF7,0XFF,0XFB,0XFF,0XFE,0XF7,0XFD,0XEF,0XFC,0XF7,0XFC,
0XFF,0XFB,0XFF,0XFD,0XFF,0XBE,0XF7,0X7F,0XFF,0X7F,0XF7,0X7C,0XFF,0XDA,0XFF,0XD8,
0XEF,0X35,0XEF,0X77,0XFF,0XFD,0XF7,0X7D,0XF7,0X9E,0XF7,0X9E,0XFF,0XFD,0XFF,0XFB,
0XF7,0X76,0XEF,0X34,0XF7,0X97,0XFF,0XFB,0XFF,0XDC,0XF7,0X7C,0XE7,0X1C,0XD6,0XDB,
0XF7,0XBD,0XFF,0XFE,0XA5,0X53,0XD6,0XB8,0XE7,0X5B,0XB5,0XB4,0X73,0XCD,0XD6,0XD8,
0XF7,0XDD,0XF7,0XDD,0XFF,0XFD,0XFF,0XFE,0XF7,0XDD,0XCE,0X77,0X9C,0XF1,0X84,0X4F,
0XF7,0XBD,0XEF,0X5B,0XF7,0X9D,0XFF,0XBD,0XFF,0XFE,0XFF,0XDD,0XD6,0XD9,0XC6,0X57,
0XAD,0X94,0X9D,0X12,0X9D,0X53,0XEF,0XBE,0XB6,0X38,0X63,0X8E,0X3A,0X8A,0X8D,0X14,
0XE7,0XBD,0XEF,0XDE,0XB5,0XF6,0X94,0XF2,0X6B,0XAE,0X32,0X08,0X6B,0XAF,0X29,0XA7,
0XBE,0X7A,0XE7,0X7E,0XDF,0X1C,0XDE,0X98,0X7B,0X29,0XAC,0X4C,0XFF,0X58,0XE5,0X4D,
0XDC,0XC7,0XDC,0XA5,0XDD,0X08,0XCC,0XCA,0XCC,0X8C,0XB3,0X89,0XFF,0X77,0XCC,0X69,
0XDD,0X6A,0XD5,0X8C,0X73,0X07,0X39,0XE8,0X3A,0X0E,0X31,0XAD,0X31,0XCA,0X3A,0X09,
0X4A,0X6B,0X4A,0X0A,0X4A,0X0A,0X4A,0X2B,0X4A,0X0A,0X39,0XA9,0X39,0X88,0X39,0XA9,
0X41,0XC9,0X41,0XCA,0X41,0XEA,0X41,0XC9,0X39,0XA9,0X39,0XA9,0X39,0XA9,0X39,0XEA,
0X32,0X0B,0X29,0XEA,0X31,0X8B,0X39,0X6A,0X39,0XAA,0X39,0XEB,0X32,0X0D,0X32,0X0C,
0X42,0X28,0X6A,0XC5,0XF6,0X4B,0XED,0X85,0XED,0X88,0XB4,0X44,0X73,0X42,0X62,0XC0,
0XE5,0XC8,0XF5,0XC7,0XDC,0XA4,0XF5,0X88,0XF5,0X67,0XED,0X05,0XE4,0XC3,0XF5,0X45,
0XED,0X26,0XED,0X69,0XDD,0X0C,0XFE,0XF7,0XB4,0X2E,0X6A,0X27,0X41,0X03,0X28,0XA1,
0X52,0X27,0XDE,0X99,0XFF,0XFE,0XD6,0XB9,0XEF,0XBD,0X3A,0X47,0X4A,0XC9,0XCE,0XDA,
0XD6,0XFA,0XF7,0XFF,0XDF,0X5C,0X7C,0X0F,0X08,0XC2,0X7C,0X50,0XBE,0X78,0X95,0X54,
0XE7,0X9D,0XE7,0X7D,0X9D,0X54,0XA5,0XD6,0XAE,0X18,0XB6,0X79,0X95,0XB7,0X1A,0X08,
0X95,0XB6,0XAE,0X7A,0X43,0X0C,0X7C,0XB2,0XF7,0XFF,0X84,0XB2,0X53,0X0B,0XF7,0XDE,
0XFF,0XFF,0XFF,0XDF,0XF7,0XFF,0XEF,0X9D,0XCE,0XB9,0XD7,0X1B,0XD7,0X1B,0X84,0XB1,
0XDF,0X7C,0XD7,0X3B,0XC6,0XDA,0XDF,0X9E,0X74,0X51,0X8D,0X14,0XB6,0X5A,0XC6,0X9A,
0X7C,0X0F,0X84,0X50,0X8C,0X93,0XBE,0X7A,0XF7,0XFF,0XEF,0XFE,0XEF,0XFF,0XF7,0XFF,
0XE7,0X3A,0XDE,0XF7,0XD6,0XD4,0XFF,0XF9,0XF7,0XFA,0XEF,0XDA,0XE7,0XB9,0XF7,0XFA,
0XF7,0XD9,0XFF,0XFC,0XEF,0X3C,0XF7,0X3E,0XFF,0XDF,0XFF,0XFE,0XFF,0XB9,0XE6,0XD2,
0XDE,0XD1,0XDE,0XD3,0XFF,0XFC,0XFF,0XBD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XE7,0X57,
0XEF,0X33,0XD6,0X4F,0XDE,0XB2,0XFF,0XD9,0XFF,0XBB,0XD6,0X98,0XBD,0XF7,0XB5,0XD6,
0XEF,0X9C,0XF7,0XDD,0XA5,0X32,0XDF,0X1A,0XEF,0X7B,0XBD,0XF6,0X8C,0X90,0XE7,0X3A,
0XFF,0XFE,0XFF,0XFD,0XEF,0X7C,0XF7,0XDD,0XF7,0XBD,0XDE,0XF9,0XB5,0XB4,0XA5,0X53,
0XF7,0XBD,0XEF,0X3B,0XF7,0X9C,0XFF,0XFE,0XFF,0XFE,0XFF,0XDD,0XD6,0X98,0XC6,0X36,
0XBE,0X16,0XB5,0XD5,0XAD,0XB5,0XE7,0X9D,0XB6,0X17,0X53,0X0C,0X32,0X08,0X74,0X30,
0XE7,0X9D,0XE7,0X9D,0XAD,0XB5,0X9D,0X34,0X74,0X10,0X21,0X66,0X6B,0XAF,0X42,0X6B,
0XCE,0XBB,0XE7,0X3D,0XDE,0XFA,0XEE,0XF9,0X6A,0X85,0XFE,0XD4,0XE5,0X8E,0XC4,0X87,
0XDC,0XE5,0XED,0X67,0XDD,0X2A,0XE5,0X6D,0X7A,0X22,0XE5,0X6E,0XFE,0X0E,0XB3,0XC4,
0XD5,0X4B,0X9B,0XC9,0X41,0XC8,0X29,0X6C,0X3A,0X53,0X2A,0X13,0X32,0X71,0X42,0X90,
0X3A,0X0E,0X39,0XCD,0X39,0XCD,0X39,0XEE,0X39,0XEE,0X39,0XCD,0X31,0XAD,0X39,0XCD,
0X39,0XEE,0X39,0XEE,0X39,0XEE,0X39,0XEE,0X39,0XED,0X39,0XCD,0X39,0XCD,0X31,0XCD,
0X22,0X0E,0X19,0XCD,0X21,0X6D,0X31,0X8D,0X31,0XCD,0X32,0X0D,0X22,0X0F,0X22,0X0D,
0X21,0X66,0X83,0XE9,0XDD,0XA9,0XBC,0X41,0XE5,0X48,0X8B,0X02,0X7B,0XA6,0XBD,0XCE,
0XE6,0X0B,0XED,0XC9,0XDC,0XE6,0XFD,0XEA,0XFD,0XEA,0XFD,0XA7,0XFD,0XE7,0XFD,0X85,
0XF5,0X25,0XED,0X48,0XE5,0X0B,0XFE,0X13,0X82,0XC9,0X59,0XC6,0X28,0XA2,0X28,0XE3,
0X41,0XA6,0XA4,0XF2,0XFF,0XFE,0XD6,0XD9,0XDF,0X5B,0X42,0X88,0X42,0X88,0XCF,0X1A,
0XCE,0XD9,0XF7,0XFF,0XDF,0X1B,0X74,0X0F,0X08,0XC2,0X7C,0X71,0XC6,0XDA,0XA5,0XB5,
0XE7,0X9D,0XE7,0X9D,0XA5,0XD6,0XA5,0XF7,0XB6,0X79,0XB6,0XBA,0X95,0XD7,0X32,0XCB,
0X8D,0X96,0XA6,0X39,0X64,0X51,0X85,0X14,0XDF,0XBE,0X9D,0X75,0X74,0X0F,0XD7,0X1B,
0XEF,0XBE,0XF7,0XFF,0XF7,0XDF,0XE7,0X7D,0XC6,0X98,0XBE,0X78,0XC6,0XB9,0X9D,0X74,
0XC6,0XFA,0XCF,0X3B,0XB6,0X79,0XCF,0X1B,0X84,0XF4,0X84,0XD3,0XAE,0X19,0XC6,0XDB,
0X8C,0X70,0X7B,0XCE,0X7B,0XCF,0XC6,0X59,0XC6,0X98,0XF7,0XFD,0XF7,0XFE,0XDF,0X3A,
0X84,0X2C,0XC6,0X32,0XCE,0X71,0XE7,0X13,0XFF,0XFA,0XDF,0X16,0X84,0X6B,0XBE,0X31,
0XFF,0XFA,0XFF,0XFC,0XD6,0X79,0XE6,0XBB,0XFF,0XDF,0XFF,0X9B,0XC5,0XD0,0X7B,0X84,
0X73,0X63,0X94,0X89,0XF7,0X99,0XFF,0XBD,0XF7,0XBE,0XFF,0XFE,0XEF,0XBB,0XDF,0X15,
0X8C,0X46,0X6B,0X01,0X8C,0X27,0XF7,0X56,0XFF,0XDB,0XD6,0X77,0XD6,0X98,0XDF,0X1B,
0XFF,0XFE,0XF7,0XDD,0XA5,0X73,0XE7,0X3B,0XD6,0XD9,0XB5,0XD5,0X9D,0X12,0XC6,0X57,
0XF7,0XFD,0XF7,0XDD,0XE7,0X5B,0XFF,0XFE,0XF7,0XBD,0XDE,0XFA,0X9D,0X32,0X94,0XB1,
0XF7,0XBD,0XEF,0X3C,0XFF,0XBD,0XFF,0XDE,0XFF,0XDD,0XEF,0X5B,0XCE,0X57,0XC6,0X16,
0XAD,0X74,0XA5,0X33,0X94,0XF2,0XC6,0X99,0XB6,0X18,0X5B,0X8E,0X4B,0X0C,0X84,0XD3,
0XE7,0XBD,0XEF,0XDE,0XAD,0XB6,0X9D,0X14,0X73,0XF0,0X10,0XE4,0X5B,0X2E,0X3A,0X2A,
0XCE,0XDB,0XD6,0XDA,0XE7,0X19,0XE6,0XD6,0X9B,0XE9,0XFF,0X75,0XC4,0X87,0XDD,0X48,
0XD5,0X67,0XDD,0XCB,0XC4,0XEF,0XCD,0X53,0X93,0X6A,0XFF,0X96,0XCC,0XE7,0XD5,0X08,
0XCC,0XEC,0X6A,0X47,0X49,0XAC,0X31,0X6F,0X3A,0X52,0X21,0XD0,0X29,0XEF,0X29,0XEE,
0X21,0XAE,0X21,0X8E,0X21,0X8E,0X21,0XAE,0X29,0XCF,0X29,0XEF,0X31,0XF0,0X32,0X10,
0X29,0XEF,0X29,0XCF,0X29,0XCF,0X29,0XCF,0X29,0XEF,0X29,0XEF,0X29,0XCF,0X21,0XAF,
0X22,0X10,0X21,0XD0,0X31,0X8F,0X39,0X8F,0X39,0XCD,0X31,0XEC,0X29,0XEE,0X2A,0X0D,
0X4A,0X69,0XC5,0XB0,0XE5,0X68,0XDC,0XA4,0XC3,0XC5,0X8A,0X83,0XA4,0X2D,0XCE,0X36,
0X94,0X8E,0XA4,0XEF,0XB5,0X50,0XD5,0XD2,0XC4,0XEC,0XB4,0X27,0XFE,0X2B,0XFE,0X6B,
0XED,0X88,0XD5,0X08,0XEE,0X0F,0XBC,0XCD,0X5A,0X66,0X39,0X84,0X18,0XE2,0X29,0X64,
0X39,0XC5,0X63,0X0B,0XF7,0XBD,0XD6,0XF9,0XDF,0X5B,0X53,0X2B,0X3A,0X68,0XCF,0X1A,
0XC6,0X99,0XF7,0XDE,0XDF,0X3C,0X84,0X71,0X19,0X45,0X8C,0XF3,0XCE,0XFB,0X9D,0XB5,
0XE7,0X9D,0XDF,0X7D,0XAE,0X17,0X9D,0XB6,0XAE,0X79,0XB6,0XBA,0X95,0XF7,0X54,0X10,
0X74,0XF4,0X7D,0X55,0X7D,0X14,0X74,0XB2,0X95,0X95,0XA5,0XD6,0X95,0X13,0XAD,0XD6,
0XCE,0X9A,0XEF,0X9E,0XEF,0XDE,0XD7,0X1B,0XAD,0XF6,0X9D,0X74,0XB6,0X57,0XBE,0XB8,
0XCF,0X3B,0XB6,0X98,0X9D,0XB5,0XB6,0XBA,0X95,0XB6,0X74,0X92,0X9D,0XF8,0XC6,0X99,
0X8C,0X0C,0X94,0X2C,0X9C,0X8F,0XD6,0X77,0XB5,0XD2,0XEF,0XB9,0XFF,0XFC,0XF7,0X79,
0X94,0X2B,0XDE,0X51,0XEE,0XB1,0XE6,0X71,0XFF,0XD9,0XE6,0XD6,0X6B,0X67,0XAD,0X4F,
0XFF,0XD9,0XFF,0XDA,0XB5,0X75,0XBD,0X97,0XFF,0X7D,0XFF,0XBC,0XE6,0XD3,0XA4,0XC8,
0X7B,0X83,0X9C,0XC9,0XFF,0XFB,0XFF,0XFD,0XF7,0XBE,0XFF,0XFF,0XFF,0XFD,0XF7,0XD8,
0X9C,0XA7,0X83,0XE3,0XAD,0X0A,0XFF,0XD7,0XFF,0XDA,0XCE,0X35,0XC6,0X16,0XC6,0X17,
0XF7,0XDD,0XFF,0XFE,0X94,0XD1,0XCE,0X98,0XE7,0X5B,0XB5,0XD5,0X8C,0X6F,0XDF,0X1A,
0XFF,0XFE,0XFF,0XFE,0XE7,0X7B,0XFF,0XFE,0XE7,0X7B,0XD6,0XF9,0X9D,0X32,0X9D,0X32,
0XFF,0XDD,0XEF,0X7C,0XFF,0XDD,0XFF,0XDD,0XF7,0X9C,0XDE,0XF9,0XC5,0XF5,0XBE,0X15,
0XBD,0XD5,0XA5,0X53,0X8C,0X90,0XAD,0XD5,0XAD,0XD6,0X53,0X2C,0X42,0XCB,0X74,0X30,
0XDF,0X7C,0XF7,0XFE,0XAD,0XB6,0X8C,0XB3,0X7C,0X11,0X29,0X87,0X63,0XAF,0X32,0X08,
0XD6,0XFB,0XD6,0XB9,0XF7,0X5A,0XCD,0XB2,0XE5,0XF1,0XED,0XED,0XBC,0X65,0XDD,0XAA,
0X7B,0X62,0X41,0XE0,0X51,0XE7,0X41,0X25,0XDD,0XD3,0XF6,0X91,0XBC,0XA5,0XD5,0X69,
0XA3,0XCB,0X38,0XC5,0X41,0X6E,0X31,0X70,0X3A,0X31,0X21,0XAF,0X32,0X10,0X29,0XF0,
0X29,0XF0,0X21,0XD0,0X21,0XAF,0X21,0XAF,0X21,0XD0,0X29,0XF0,0X29,0XF0,0X21,0XCF,
0X2A,0X10,0X29,0XD0,0X21,0XAF,0X29,0XD0,0X29,0XF0,0X32,0X11,0X29,0XF0,0X21,0XB0,
0X21,0XF1,0X21,0XB0,0X31,0X90,0X39,0X8F,0X31,0X8D,0X29,0X6B,0X21,0X8C,0X31,0XCA,
0X62,0XA8,0XDD,0XAF,0XD4,0XC6,0XF5,0X68,0X82,0X20,0X93,0X07,0XBD,0X12,0X94,0X71,
0X29,0XC7,0X19,0X87,0X3A,0X4B,0X6B,0X6E,0X6A,0XE9,0X62,0X22,0XCD,0X09,0XED,0XCA,
0XED,0XAA,0XC4,0XA7,0XFF,0X13,0X8B,0XC9,0X52,0X66,0X19,0X01,0X10,0XE1,0X21,0X43,
0X39,0XA5,0X39,0XA5,0XEF,0X7C,0XD6,0XFA,0XDF,0X3B,0X63,0XAD,0X32,0X27,0XC6,0XB9,
0XCE,0XDB,0XF7,0XFF,0XEF,0XBE,0X95,0X14,0X29,0XE7,0X95,0X34,0XC6,0XBA,0X84,0XF3,
0XE7,0XBD,0XD7,0X5C,0XA5,0XF7,0X84,0XF3,0X95,0XD7,0X9E,0X19,0X85,0X76,0X64,0X92,
0X6C,0X93,0X7D,0X35,0X7D,0X34,0X6C,0X71,0X95,0XB5,0XA5,0XD6,0X84,0XD2,0XC6,0XDA,
0XD7,0X1B,0XF7,0XFF,0XF7,0XFE,0XDF,0X7C,0XC6,0XB9,0XB6,0X57,0XD7,0X5B,0XEF,0XFE,
0XE7,0XFE,0XA6,0X16,0X85,0X33,0XB6,0XDA,0XAE,0X9A,0X7C,0XF4,0X9E,0X39,0XAE,0X79,
0X84,0X2D,0X8C,0X0C,0X83,0XCC,0X9C,0XD0,0XD6,0XD6,0XFF,0XFA,0XFF,0XFB,0XDE,0X95,
0XDE,0X73,0XB5,0X0C,0XE6,0X91,0XD6,0X30,0XAC,0XCD,0XD6,0X53,0XB5,0X70,0XB5,0XB1,
0XF7,0XB8,0XFF,0XFB,0XCE,0X36,0XD6,0X37,0XFF,0XDD,0XFF,0XFB,0XF7,0X35,0XB5,0X6B,
0X94,0X87,0XA4,0XEA,0XFF,0XB9,0XFF,0XDC,0XFF,0XFE,0XFF,0XFE,0XCE,0X96,0X94,0XAC,
0X8C,0X47,0X9C,0XA7,0XCD,0XEE,0XFF,0XF7,0XF7,0X78,0XD6,0X55,0XCE,0X56,0XAD,0X52,
0XFF,0XFE,0XFF,0XFE,0X7C,0X0E,0XD6,0XD9,0XE7,0X5A,0XB5,0XF4,0X8C,0XD0,0XD6,0XF8,
0XFF,0XFD,0XEF,0XDC,0XEF,0XDC,0XFF,0XFD,0XEF,0XDC,0XC6,0X76,0X9D,0X52,0X94,0XD1,
0XF7,0XDD,0XEF,0X3B,0XF7,0X7C,0XFF,0XBD,0XFF,0XBC,0XC5,0XF5,0XAD,0X52,0XB5,0X93,
0X94,0XB0,0X84,0X2D,0X9D,0X11,0XBE,0X16,0XAD,0XB4,0X4A,0XCA,0X42,0X89,0X74,0X4F,
0XCF,0X59,0XF7,0XFD,0XBE,0X17,0X9C,0XF4,0X63,0X0E,0X29,0XA8,0X53,0X6D,0X32,0X68,
0XCE,0XF9,0XE7,0X3A,0XE6,0X57,0XC4,0XCF,0XFE,0XD5,0XCC,0X8A,0XCC,0XA7,0XC5,0X4A,
0X32,0X02,0X32,0X46,0X52,0X29,0X6A,0X69,0XFF,0X36,0XCC,0XC9,0XC4,0XC6,0XD5,0X8D,
0X51,0XA6,0X49,0XAC,0X49,0X90,0X41,0XB1,0X42,0X10,0X32,0X10,0X21,0XD2,0X21,0XD2,
0X21,0XD1,0X32,0X31,0X32,0X11,0X29,0XF0,0X32,0X10,0X29,0XEF,0X29,0XCF,0X3A,0X10,
0X31,0XCF,0X31,0XEF,0X31,0XAF,0X29,0X8E,0X31,0XAF,0X31,0X8F,0X29,0X8F,0X31,0XD0,
0X29,0XF1,0X19,0X4E,0X31,0X8F,0X39,0XB0,0X29,0X6E,0X31,0XAE,0X29,0X8B,0X39,0X66,
0XB4,0X4B,0XED,0XCD,0XD4,0XC7,0XD5,0X08,0X72,0XA2,0X6A,0XE6,0XD6,0X76,0X52,0X68,
0X42,0X29,0X42,0X2C,0X32,0X2E,0X53,0X31,0X63,0X0D,0X7B,0X08,0XFE,0X4E,0XF5,0X27,
0XE4,0X85,0XF5,0X8B,0XE6,0X10,0X62,0XC6,0X4A,0X66,0X29,0X63,0X20,0XA2,0X28,0XE3,
0X4A,0X27,0X52,0X88,0X7C,0X2E,0XC6,0X57,0XAD,0XB5,0X84,0X92,0X4B,0X0C,0XBE,0X7A,
0XD7,0X3D,0XDF,0X9E,0XB6,0X59,0XA5,0XB6,0X3A,0X89,0X8D,0X34,0XC6,0XDA,0XAD,0XF7,
0XEF,0XFF,0XAE,0X38,0X9D,0X96,0X74,0X72,0X95,0X77,0X9D,0XF9,0X85,0X36,0X8D,0X77,
0X85,0X56,0X85,0X55,0XA6,0X58,0X85,0X34,0X95,0XB6,0X95,0XB6,0X9D,0XD6,0XBE,0XB9,
0XCE,0XB8,0XEF,0X9B,0XEF,0XBC,0XBE,0X57,0XBE,0X77,0X9D,0X73,0X95,0X53,0XD7,0X3A,
0XBE,0X98,0X7C,0XB1,0X85,0X12,0X8D,0X54,0X6C,0XB2,0X6C,0XD4,0XA6,0XBC,0XA6,0X7A,
0X9D,0XB6,0XA5,0X94,0XAD,0XD6,0XAD,0XB5,0XA5,0X72,0XB5,0XB3,0X73,0XAA,0XA5,0X0F,
0XC5,0XF2,0XC6,0X11,0XBD,0XD0,0XD6,0XD3,0XAD,0X8F,0X94,0XCD,0X74,0X2A,0X42,0XA4,
0XAD,0X91,0XFF,0XFC,0XC6,0X34,0XD6,0X96,0XFF,0XFA,0XCE,0X52,0XEF,0X54,0XBD,0XEE,
0X84,0X07,0X84,0X28,0XFF,0XD8,0XF7,0XBA,0XF7,0XBB,0XDE,0XF8,0X4A,0X47,0X5A,0XE7,
0X5A,0XE4,0X84,0X27,0XD6,0X71,0XF7,0X96,0XFF,0XD8,0XC6,0X12,0XAD,0X2F,0XA4,0XEE,
0XF7,0X9C,0XFF,0XFE,0X84,0X4F,0XDE,0XF9,0XD6,0XD9,0XAD,0X93,0X94,0XF0,0XD6,0XF8,
0XF7,0XFD,0XFF,0XFD,0XEF,0XDC,0XF7,0XFD,0XCE,0XD8,0X95,0X11,0X84,0X8F,0X8C,0XB0,
0XF7,0X9C,0XF7,0X9C,0XFF,0XDD,0XEF,0X7C,0XE7,0X3A,0XB5,0X94,0XAD,0X52,0XB5,0XB3,
0XA5,0X11,0XB5,0XB3,0X8C,0X8F,0X74,0X0D,0XA5,0X94,0X63,0X6C,0X2A,0X06,0X63,0XAC,
0XD7,0X59,0XEF,0XFC,0XA5,0X33,0X9C,0XD4,0X9C,0XD4,0X63,0X6F,0X64,0X0F,0X4B,0X4B,
0XB6,0X36,0XD6,0XB7,0XA4,0X2E,0XFE,0X14,0XFD,0XF2,0XC4,0X08,0XED,0XCC,0X93,0XC6,
0X29,0XC4,0X42,0XAB,0X41,0X87,0XDD,0X94,0XF5,0XF0,0XC4,0XA8,0XD5,0X8C,0X8B,0XC9,
0X41,0XC9,0X41,0XCE,0X39,0X8F,0X39,0XAF,0X39,0XEF,0X31,0XEF,0X19,0XB0,0X19,0XD1,
0X19,0X8F,0X29,0XF0,0X29,0XD0,0X29,0XCF,0X29,0XF0,0X29,0XCF,0X29,0XCF,0X32,0X10,
0X29,0XAE,0X31,0XCF,0X29,0X8F,0X29,0X6F,0X31,0XB0,0X31,0XB0,0X31,0X90,0X31,0XF1,
0X32,0X10,0X32,0X10,0X31,0XF0,0X3A,0X11,0X3A,0X32,0X21,0X4D,0X31,0X6A,0X5A,0X07,
0XDD,0X4D,0XF5,0XAC,0XDD,0X4A,0XA4,0X06,0X5A,0X64,0X73,0X8B,0XA4,0XF2,0X4A,0X29,
0X42,0X29,0X42,0X6D,0X3A,0X70,0X4A,0XD0,0X5B,0X0C,0X93,0XEA,0XFE,0X0C,0XF5,0X06,
0XEC,0XC7,0XFE,0X2F,0XAC,0X8B,0X52,0X65,0X31,0XE3,0X31,0XA4,0X28,0XC2,0X39,0X04,
0X18,0XC1,0X4A,0X88,0X63,0X6B,0XB5,0XF6,0XBE,0X58,0X8C,0XD2,0X3A,0X8A,0XBE,0X9A,
0XEF,0XFF,0XE7,0XDF,0XBE,0X9A,0XAE,0X18,0X42,0XEB,0X7C,0XB2,0XC6,0XDA,0XB6,0X57,
0XEF,0XFE,0XB6,0X58,0XBE,0X9A,0X95,0X96,0X95,0X77,0XBE,0XBC,0XAE,0X7B,0X64,0X31,
0X64,0X51,0X74,0XB2,0X5C,0X0F,0X74,0XD2,0X85,0X13,0X5B,0XEE,0X63,0XEF,0X6C,0X2F,
0X9D,0X52,0XDF,0X5A,0XE7,0X9B,0XAD,0XF5,0X95,0X52,0X95,0X52,0X95,0X32,0XC6,0XB8,
0XC6,0XD9,0X7C,0X6F,0X8D,0X32,0X8D,0X75,0X8D,0XD6,0X75,0X15,0X6D,0X36,0XAF,0X1D,
0XB6,0XBB,0X9D,0XB6,0X9D,0X95,0XAD,0XD6,0XA5,0X93,0XB5,0XD4,0X94,0XCF,0XA5,0X2F,
0XBD,0XF2,0XD6,0XD5,0XBE,0X32,0X94,0XCD,0XA5,0XB0,0X9D,0X90,0X63,0XC9,0XA5,0XB1,
0XDF,0X39,0XD6,0XD8,0XAD,0X72,0XF7,0XBA,0XDE,0XF7,0X5A,0XE6,0XCE,0X73,0XC6,0X32,
0X9D,0X0D,0XC6,0X53,0XFF,0XFA,0XFF,0XFB,0XFF,0XFC,0XDE,0XF8,0X63,0X2A,0X52,0XC8,
0X5A,0XE6,0XBE,0X11,0XE7,0X36,0XDE,0XF5,0XFF,0XF9,0XD6,0XB3,0X9C,0XCC,0XAD,0X4D,
0XEF,0X5C,0XD6,0XD9,0X4A,0X88,0XBD,0XF5,0XD6,0XD9,0XCE,0X98,0XC6,0X36,0XD6,0XF9,
0XE7,0X5A,0XDF,0X39,0XAD,0XD4,0XDF,0X5A,0XD6,0XF9,0XBE,0X57,0XC6,0X78,0XCE,0X98,
0XE7,0X3B,0XEF,0X7C,0XF7,0XBD,0XDE,0XD9,0XD6,0X78,0XAD,0X53,0XA5,0X11,0XA5,0X31,
0X94,0XD0,0X9D,0X11,0X84,0X8F,0X7C,0X2E,0XA5,0X94,0X84,0XB1,0X6B,0XEE,0X74,0X4F,
0XA5,0X94,0XBE,0X37,0XB5,0XB5,0XD6,0X99,0XB5,0XB7,0X7C,0X31,0X95,0X55,0X84,0XD2,
0XBE,0X36,0XBD,0X93,0XA4,0X0B,0XFE,0XF5,0XD4,0XCB,0XCC,0XAA,0XCD,0X0A,0X6A,0X83,
0X41,0XA6,0X49,0XE9,0X7A,0XAA,0XFF,0X18,0XC4,0X2A,0XCC,0XEC,0XB4,0XED,0X52,0X67,
0X31,0XEA,0X31,0XED,0X29,0XAD,0X29,0XAC,0X32,0X0C,0X32,0X0C,0X21,0XEC,0X21,0XED,
0X29,0XF0,0X32,0X32,0X32,0X31,0X32,0X11,0X32,0X31,0X2A,0X10,0X29,0XF0,0X32,0X31,
0X29,0XD0,0X29,0XF1,0X21,0XB0,0X21,0X90,0X21,0XB1,0X21,0XB1,0X21,0X91,0X21,0XD0,
0X21,0XAD,0X32,0X2F,0X21,0XAF,0X19,0X90,0X32,0X33,0X10,0XCC,0X52,0X2C,0X9B,0XAC,
0XED,0X6E,0XD4,0XA8,0XDD,0X4C,0X72,0XA4,0X62,0XA9,0XBD,0X79,0X5A,0X70,0X49,0XED,
0X42,0X6B,0X4B,0X0D,0X4B,0X10,0X4A,0XCD,0X6B,0X6A,0XC5,0X4B,0XED,0XA8,0XED,0X26,
0XF5,0X6B,0XFE,0X12,0X72,0XA7,0X42,0X05,0X21,0X81,0X31,0XE3,0X28,0XE1,0X31,0X03,
0X20,0XE1,0X4A,0X67,0X4A,0X88,0XAD,0X94,0XD6,0XFA,0XAD,0XD6,0X63,0X8E,0XAE,0X18,
0XE7,0XDF,0XEF,0XFF,0XE7,0XDE,0XD7,0X5C,0X74,0X50,0X7C,0X91,0XB6,0X57,0X9D,0X94,
0XC6,0XD8,0XC6,0XF9,0XBE,0XD9,0X8D,0X34,0X85,0X14,0X95,0XB7,0X95,0XD7,0X64,0X51,
0X6C,0X50,0X53,0XCE,0X1A,0X06,0X64,0X2E,0X74,0XD0,0X5B,0XED,0X43,0X0A,0X5B,0XAC,
0X95,0X32,0XBE,0X56,0XC6,0XB8,0XBE,0X77,0X8D,0X32,0X6C,0X2E,0X6C,0X0D,0XC6,0XB8,
0XA5,0XB4,0X32,0X26,0X5B,0XAC,0X43,0X0A,0X4B,0X6D,0X8D,0XB7,0X96,0X19,0XAE,0XFC,
0X95,0XB6,0X6C,0X30,0X8D,0X13,0XA5,0XB5,0XAD,0XB4,0XBE,0X56,0XA5,0X31,0X8C,0X6D,
0XAD,0X71,0XC6,0X54,0XB5,0XB2,0XA5,0X70,0X9D,0X50,0X9D,0X71,0XBE,0X75,0X7C,0X6E,
0XBE,0X37,0X94,0XF3,0X8C,0XB2,0XFF,0XFF,0XEF,0X7D,0XD6,0XB9,0XBE,0X17,0XBD,0XD6,
0XD6,0XD9,0XE7,0X3A,0XDE,0XF9,0XDF,0X19,0XAD,0XD4,0XAD,0XD4,0XF7,0XFC,0XBE,0X56,
0XDF,0X19,0XEF,0X9B,0XDE,0XF7,0XD6,0XB5,0XFF,0XF9,0XEF,0X96,0XB5,0XEE,0XA5,0X6B,
0XFF,0XFE,0XE7,0X5B,0X5A,0XE9,0XBD,0XF6,0XC6,0X57,0XC6,0X57,0XD6,0XB8,0XE7,0X5B,
0XE7,0X3A,0XE7,0X7B,0XB5,0XD5,0XEF,0X9C,0XCE,0X98,0XBE,0X37,0XDF,0X1B,0XF7,0XBD,
0XEF,0X7C,0XF7,0XBD,0XF7,0XBD,0XD6,0XB8,0XD6,0XD9,0XCE,0X97,0XD6,0XB7,0XCE,0X77,
0XBE,0X35,0XB5,0XF4,0XC6,0X77,0XB6,0X16,0XA5,0X94,0X7C,0X90,0X7C,0X90,0X7C,0X70,
0XBE,0X38,0XD6,0XBA,0X9C,0XD1,0XAD,0X53,0XD6,0XFA,0XB6,0X17,0XAD,0XD8,0XBE,0X7A,
0XBE,0X16,0X94,0X2C,0XEE,0X31,0XED,0XEE,0XAB,0XE6,0XE5,0XEE,0X72,0X83,0X6A,0X85,
0X6A,0X49,0X61,0XC7,0XF5,0XF3,0XFE,0X10,0XBC,0X07,0XDD,0X8F,0X6A,0XA7,0X39,0XC7,
0X32,0X0B,0X32,0X0D,0X29,0XCC,0X29,0XCC,0X3A,0X2D,0X3A,0X2C,0X31,0XEB,0X32,0X0C,
0X29,0XAD,0X31,0XEF,0X29,0XCE,0X29,0XCE,0X29,0XEE,0X21,0XCE,0X21,0X8D,0X21,0XCE,
0X29,0XEF,0X2A,0X10,0X21,0XCF,0X19,0XAF,0X21,0XD0,0X19,0XD1,0X19,0XB0,0X21,0XCF,
0X29,0XCC,0X3A,0X4E,0X2A,0X30,0X19,0XD0,0X32,0X72,0X29,0X8B,0XA4,0X51,0XDD,0X91,
0XB3,0XC6,0XDD,0X0A,0XB4,0X8A,0X6A,0XA6,0X94,0X52,0XBD,0XBC,0X39,0X8E,0X41,0XEF,
0X42,0XAD,0X43,0X0D,0X4A,0XEF,0X52,0XCC,0X8C,0X2A,0XE6,0X4C,0XD5,0X24,0XE5,0X46,
0XFE,0X50,0XCC,0XCF,0X5A,0X26,0X42,0X26,0X31,0XC2,0X39,0XE2,0X31,0X22,0X28,0XE2,
0X31,0X83,0X31,0XC4,0X3A,0X06,0X8C,0X90,0XB5,0XF6,0XAD,0XB5,0X8C,0XD2,0X8C,0XF3,
0XC6,0XBA,0XD7,0X5C,0XA5,0XD6,0X95,0X54,0XA5,0XB5,0XBE,0X98,0XD7,0X5A,0X9D,0X73,
0X9D,0X93,0X95,0X52,0X74,0X6F,0X6C,0X0E,0X8D,0X54,0X7C,0XF3,0X5B,0XEF,0X22,0X27,
0X32,0X89,0X43,0X4B,0X2A,0X46,0X32,0X87,0X32,0XA8,0X74,0X6F,0X22,0X26,0X2A,0X47,
0X63,0XEE,0X9D,0X74,0XB6,0X78,0X95,0X94,0X74,0X70,0X64,0X0D,0X74,0X4E,0X9D,0XB3,
0XB6,0X55,0X29,0XE4,0X7C,0X6F,0X8C,0XF1,0X74,0X70,0X85,0X35,0X7D,0X35,0X95,0XD8,
0X7C,0XD3,0X85,0X13,0X84,0XD1,0XAD,0XF6,0XCE,0XB8,0XC6,0X97,0XDF,0X18,0XA5,0X31,
0X94,0XAE,0X4A,0X65,0X8C,0X6E,0XBE,0X35,0XB6,0X15,0X84,0X6F,0X74,0X0D,0X84,0X90,
0XB6,0X37,0XC6,0X9A,0XD6,0XFC,0XDF,0X3D,0XFF,0XFF,0XDF,0X3D,0XAD,0X97,0XAD,0X96,
0XCE,0XBB,0XE7,0X5C,0XFF,0XFE,0X74,0X2D,0X53,0X28,0X7C,0X8C,0X84,0XED,0XCF,0X16,
0XE7,0X9B,0XEF,0X9C,0XDF,0X5A,0XE7,0X99,0XA5,0X6F,0X84,0XAA,0X8C,0XC9,0XBE,0X8F,
0XFF,0XFE,0XE7,0X1B,0X52,0X88,0XBD,0XF6,0XC6,0X37,0XC6,0X57,0XDE,0XD9,0XE7,0X3A,
0XFF,0XFD,0XFF,0XFE,0XC6,0X37,0XF7,0XDD,0XD6,0XB9,0XBE,0X17,0XCE,0X99,0XCE,0XB9,
0XF7,0XDE,0XF7,0XDD,0XDF,0X1A,0XA5,0X53,0X9C,0XF1,0X8C,0X6F,0X8C,0X6E,0X7C,0X2D,
0X73,0XCC,0X73,0XEC,0X6B,0XAC,0X42,0XA8,0X42,0XA8,0X42,0XC9,0X74,0X4F,0XA5,0XB6,
0XD6,0X9B,0XCE,0X19,0XCE,0X37,0XE7,0X39,0XD6,0XF9,0XBE,0X58,0XC6,0X9A,0XB5,0XF8,
0X9C,0X90,0X93,0XEB,0XFF,0X34,0XC4,0X87,0XBC,0X67,0XCD,0X6C,0X39,0X40,0X6A,0XC8,
0X6A,0X07,0XA3,0X8A,0XFF,0X31,0XCC,0X64,0XDC,0XE7,0XCC,0XCB,0X40,0XE2,0X39,0X46,
0X39,0XEA,0X39,0XEB,0X31,0XAB,0X31,0X8C,0X41,0XAE,0X41,0XAE,0X39,0X6C,0X39,0XAB,
0X31,0XCA,0X3A,0X0B,0X32,0X0B,0X32,0X0B,0X3A,0X2C,0X32,0X2B,0X2A,0X0B,0X32,0X2C,
0X21,0XEC,0X2A,0X0D,0X21,0XEC,0X19,0XCC,0X22,0X2E,0X22,0X2E,0X1A,0X0E,0X2A,0X0E,
0X31,0X6A,0X39,0XAB,0X32,0X4F,0X22,0X0E,0X2A,0X2C,0X42,0X28,0XD5,0X90,0XE5,0X8D,
0X82,0X40,0XF6,0X4F,0X8B,0X88,0X62,0XE9,0XB5,0XD8,0X5B,0X10,0X3A,0X0E,0X31,0XED,
0X32,0X8E,0X2A,0X4D,0X32,0X0D,0X5A,0XEC,0XAC,0XEB,0XEE,0X6B,0XC4,0XE1,0XE5,0XA6,
0XFE,0XD4,0X8B,0X2A,0X62,0X8A,0X4A,0X47,0X42,0X64,0X31,0XC1,0X41,0XA4,0X28,0XE2,
0X29,0X02,0X39,0XC4,0X52,0X88,0X63,0X4B,0X94,0XD1,0X8C,0XD1,0X9D,0X33,0X8C,0XB1,
0X9D,0X74,0XEF,0XDD,0XB6,0X16,0X95,0X32,0XC6,0XB8,0XCE,0XF9,0XD7,0X5A,0XC6,0XB7,
0XCF,0X19,0X95,0X73,0X7C,0XB1,0X7C,0X90,0X84,0XD2,0X7C,0XD2,0X7C,0XF3,0X43,0X2B,
0X22,0X27,0X4B,0X4B,0X43,0X09,0X5B,0XCC,0X5B,0XCD,0X9D,0XB4,0X53,0X6C,0X53,0X6D,
0X53,0X6C,0X64,0X0E,0X95,0X95,0X74,0X90,0X74,0X90,0X63,0XED,0X7C,0XAF,0X95,0X31,
0X63,0XAA,0X32,0X04,0X8C,0XAF,0X9D,0X93,0X8D,0X13,0X7C,0XB2,0X85,0X15,0XBE,0XFC,
0X85,0X34,0X7C,0XF2,0XB6,0X78,0X8D,0X12,0XBE,0X97,0XEF,0XBC,0XD6,0XD8,0XE7,0X5A,
0XB5,0XD4,0X63,0X29,0XB5,0XB4,0XDF,0X3A,0XAD,0XB5,0X63,0XAD,0X6B,0XEE,0X8D,0X12,
0X84,0XD0,0X9D,0X73,0X3A,0X67,0XCE,0XFA,0XE7,0X9D,0XB6,0X18,0X74,0X30,0X95,0X14,
0X9D,0X74,0XEF,0XFD,0XD7,0X58,0X53,0X87,0X4B,0X44,0X43,0X42,0X4B,0XA2,0X7D,0X09,
0XE7,0XBA,0XBE,0X37,0X63,0XAB,0X7C,0X4C,0X53,0X46,0X4B,0X24,0X42,0XE1,0X95,0X8B,
0XFF,0XDD,0XD6,0XB9,0X42,0X06,0XCE,0X57,0XF7,0XBC,0XFF,0XDD,0XFF,0XFE,0XFF,0XFD,
0XFF,0XFD,0XF7,0XDD,0XCE,0X78,0XFF,0XDE,0XF7,0XDE,0XEF,0X7D,0XE7,0X1C,0XCE,0X78,
0XF7,0X9D,0XFF,0XFD,0XFF,0XFE,0XDF,0X1A,0XDE,0XF9,0XC6,0X36,0XBE,0X35,0XBE,0X14,
0XB5,0XD4,0XA5,0X93,0XA5,0XB3,0X84,0XB0,0X84,0X8F,0X95,0X53,0XBE,0X98,0XB5,0XF7,
0XA5,0X14,0XB5,0X95,0XC6,0X35,0XE7,0X99,0XF7,0XFC,0XC6,0XD8,0X8C,0XB2,0X7B,0XF0,
0X8B,0XAD,0XD5,0X91,0XFE,0X50,0XC4,0X66,0XDD,0X6B,0X8B,0X26,0X41,0X85,0X5A,0X49,
0X7A,0XA9,0XFE,0X53,0XED,0XA9,0XB4,0X00,0XED,0X88,0XBC,0X27,0X82,0XC8,0X7A,0XEB,
0X62,0XA9,0X62,0XCA,0X5A,0X89,0X5A,0X4A,0X62,0X4C,0X62,0X0C,0X61,0XE9,0X62,0X28,
0X52,0X07,0X52,0X47,0X52,0X06,0X49,0XE6,0X4A,0X06,0X42,0X06,0X39,0XC5,0X42,0X06,
0X42,0X07,0X3A,0X28,0X31,0XE7,0X31,0XC7,0X32,0X08,0X31,0XE8,0X29,0XA8,0X31,0XA8,
0X51,0XC8,0X41,0X67,0X3A,0X0B,0X29,0XE8,0X21,0XA4,0X5A,0X83,0XDD,0XAB,0XDD,0X28,
0X92,0XC0,0XB4,0X69,0X62,0XA7,0X39,0XA6,0XA5,0X78,0X29,0XEB,0X21,0XCC,0X19,0XCD,
0X22,0X2F,0X21,0XCE,0X29,0X6C,0X73,0X2E,0XD5,0X8E,0XE5,0XC8,0XCD,0X22,0XEE,0X4A,
0XDD,0XF1,0X51,0XE6,0X5A,0X89,0X4A,0X26,0X4A,0X43,0X31,0X60,0X41,0XA4,0X31,0X23,
0X18,0XC1,0X4A,0X26,0X52,0X67,0X39,0XE5,0X8C,0X6F,0X94,0XB0,0X7C,0X2F,0X7C,0X4F,
0XAD,0XD5,0XF7,0XFE,0XA5,0XB4,0X9D,0X52,0XEF,0XBC,0XD7,0X39,0XBE,0X55,0XA5,0X93,
0XA5,0XD4,0XA5,0XD5,0X84,0XD1,0X3A,0XA9,0X42,0XAA,0X4B,0X2C,0X6C,0X10,0X7C,0XB1,
0X5B,0XCE,0X3A,0XA9,0X3A,0XC9,0XBE,0XD9,0X95,0X74,0X2A,0X48,0X4B,0X0C,0XCF,0X3C,
0X74,0X71,0X4B,0X0B,0X6C,0X2F,0X53,0X8C,0X63,0XEE,0X2A,0X46,0X42,0XC7,0X53,0X08,
0X74,0X2C,0X4A,0XC7,0X7C,0X6E,0X6B,0XED,0X3A,0X88,0X4B,0X0C,0X84,0XF4,0X95,0X77,
0X64,0X10,0X64,0X0F,0XD7,0X7C,0X9D,0X94,0XD7,0X3B,0XF7,0XFE,0XBE,0X36,0XE7,0X7B,
0XDF,0X19,0X52,0XE9,0X73,0XED,0XE7,0X7C,0XB5,0XF6,0X53,0X2B,0X8C,0XD3,0XB6,0X57,
0X9D,0X92,0XCF,0X38,0XD7,0X59,0XEF,0XFD,0XF7,0XFF,0XEF,0XFF,0XC6,0XBA,0X6C,0X0F,
0XDF,0X7C,0XE7,0XFC,0XC7,0X16,0X4B,0X65,0X43,0X62,0X54,0X02,0X54,0X41,0X5C,0X43,
0XB6,0X92,0X95,0X70,0X63,0XE9,0X6C,0X4A,0X32,0XA2,0X32,0XA1,0X43,0X02,0X7D,0X09,
0XCE,0X57,0XC6,0X36,0X39,0XC5,0XBE,0X15,0XD6,0XB8,0XBD,0XF5,0XCE,0X97,0XE7,0X19,
0XDE,0XF9,0XEF,0X9B,0XEF,0X5B,0XF7,0XBD,0XE7,0X5C,0XDE,0XFA,0XE7,0X1B,0XDE,0XFB,
0XDE,0XF9,0XD6,0XB8,0XDE,0XB8,0XD6,0X97,0XE7,0X19,0XCE,0X76,0XBE,0X14,0XB5,0XD3,
0XAD,0XB2,0X9D,0X51,0XBE,0X56,0XCE,0XD8,0XDF,0X5A,0XD7,0X3A,0XBE,0X57,0X73,0XED,
0X94,0XB0,0X8C,0X8E,0XCE,0XF5,0XEF,0XF8,0XA5,0XD0,0X6C,0X0B,0X73,0XED,0X8C,0X0F,
0XBC,0XF1,0XFE,0X54,0XDC,0XEB,0XD4,0XCA,0XCC,0XCC,0X59,0XA3,0X51,0XAA,0X51,0XAB,
0XBC,0XD5,0XFE,0XD8,0XCC,0XC9,0XCC,0XC5,0XED,0XAA,0XD4,0XE9,0XE5,0X4E,0XD5,0X2E,
0XDD,0XAE,0XE6,0X0E,0XDD,0XED,0XDD,0XAD,0XDD,0X8F,0XDD,0X6E,0XDD,0X6B,0XE5,0X8B,
0XE5,0X8D,0XED,0X8E,0XE5,0X4D,0XDD,0X0C,0XDD,0X2D,0XD5,0X0C,0XCC,0XEC,0XD5,0X0C,
0XBC,0X8A,0XBC,0XAB,0XB4,0X6A,0XAC,0X4A,0XB4,0X6B,0XAC,0X6B,0XA4,0X0A,0XAC,0X0A,
0XAB,0XCB,0X8B,0X29,0X7B,0XAB,0X73,0X88,0X73,0X63,0XA4,0X44,0XED,0XC7,0XED,0X67,
0X9B,0X23,0X62,0X23,0X49,0XA6,0X52,0X8C,0X73,0XD2,0X2A,0X0C,0X11,0XAC,0X12,0X0F,
0X1A,0X12,0X29,0XF1,0X31,0X2C,0X93,0X90,0XED,0XCF,0XCC,0XE6,0XDD,0X86,0XF6,0XAD,
0X8B,0XA9,0X49,0XC5,0X49,0XE6,0X52,0X26,0X41,0XC1,0X39,0X60,0X41,0X84,0X39,0X44,
0X18,0XC1,0X31,0X83,0X31,0X83,0X29,0X42,0X63,0X2A,0X84,0X4E,0X5A,0XE9,0X5B,0X2A,
0X7C,0X0D,0XDF,0X5A,0X95,0X31,0X8C,0XAF,0XB6,0X35,0XB6,0X14,0XBE,0X35,0XAD,0XD3,
0X95,0X12,0XC6,0XBA,0XAD,0XD7,0X5B,0X6E,0X95,0X55,0X9D,0X96,0X74,0X51,0X95,0X75,
0X74,0X71,0X74,0X50,0X4B,0X0B,0X3A,0X89,0X32,0X69,0X4B,0X0D,0X63,0XF1,0X42,0XAB,
0X32,0X28,0X11,0X44,0X3A,0X88,0X21,0XE6,0X4B,0X4A,0X6C,0X2D,0X8D,0X10,0X7C,0X4C,
0XBE,0X75,0X42,0X65,0X74,0X0C,0X8C,0XF0,0X4B,0X2B,0X53,0X4D,0X85,0X15,0X7C,0XD4,
0X32,0XAB,0X63,0XEF,0X8D,0X14,0XA5,0XF6,0XD7,0X3B,0X8C,0XF2,0XBE,0X57,0XA5,0X73,
0XD6,0XD9,0X73,0XED,0X84,0X6F,0X73,0XCD,0X63,0X8C,0X7C,0X70,0X42,0XA9,0X53,0X2B,
0X9D,0X92,0X5B,0XA9,0X84,0XCF,0XB6,0X36,0XA5,0X94,0XD7,0X5C,0X6B,0XEE,0X3A,0X88,
0X8C,0XF1,0XE7,0XDB,0X8D,0X2E,0X43,0X43,0X85,0X69,0X5C,0X43,0X75,0X45,0X6C,0XE4,
0X6C,0X86,0X6C,0X87,0X95,0X8B,0XA6,0X2E,0X95,0XCC,0X64,0X46,0X5B,0XC5,0X5B,0XE5,
0XDF,0X19,0XCE,0X76,0X29,0X83,0XBD,0XD4,0XDE,0XF8,0XC6,0X15,0XC6,0X56,0XD6,0X97,
0XD6,0X97,0XCE,0X77,0XC6,0X57,0XC6,0X16,0XCE,0X57,0XCE,0X78,0XCE,0X78,0XC6,0X17,
0XE6,0XF9,0XB5,0X73,0X94,0X6E,0X94,0X6E,0XC5,0XF4,0XC6,0X14,0XC6,0X14,0XBE,0X14,
0XB5,0XD3,0XA5,0X51,0X7C,0X0C,0X7C,0X2D,0XD7,0X19,0XBE,0X36,0X84,0X8F,0X8C,0X90,
0XB5,0XB5,0X94,0XB0,0XBE,0X75,0XD7,0X77,0X95,0X90,0X5B,0X69,0X52,0XC8,0X7B,0X6A,
0XFE,0X95,0XE5,0X6F,0XC4,0X27,0XDC,0XCA,0XA3,0X66,0X61,0XA3,0X49,0X47,0X59,0XEB,
0XDD,0XF9,0XB4,0X6F,0XC4,0XEC,0XCD,0X09,0XCC,0XC9,0XE5,0X4C,0XD4,0XCC,0XCC,0X8A,
0XD5,0X09,0XDD,0X69,0XDD,0X6A,0XD5,0X2A,0XDD,0X2B,0XDD,0X0A,0XDD,0X07,0XE5,0X48,
0XDC,0XC8,0XDC,0XCA,0XDC,0X89,0XD4,0X89,0XDC,0XCA,0XE4,0XEA,0XDC,0XEA,0XE5,0X2B,
0XE5,0X0B,0XE5,0X2B,0XDD,0X0B,0XDD,0X0B,0XED,0X6D,0XED,0X8D,0XE5,0X6D,0XE5,0X6D,
0XED,0X6E,0XD5,0X2E,0XCD,0XAF,0XD5,0XEF,0XDD,0XEB,0XE5,0XE9,0XE5,0X26,0XD4,0XC6,
0X7A,0X62,0X72,0X87,0X30,0XC3,0XC5,0X98,0X52,0X6C,0X11,0X08,0X2A,0X8F,0X1A,0X4F,
0X21,0XF0,0X3A,0X31,0X41,0X4A,0XA3,0X8E,0XED,0X8F,0XB3,0XE4,0XDD,0XAA,0XEE,0XB0,
0X41,0XA1,0X5A,0X66,0X39,0X62,0X62,0XA6,0X49,0XC1,0X49,0XC2,0X41,0X64,0X41,0XA5,
0X31,0X43,0X29,0X22,0X39,0XA4,0X4A,0X26,0X39,0XE4,0X73,0XAC,0X52,0XA8,0X5B,0X29,
0X5B,0X29,0XCE,0XB7,0X9D,0X30,0X9D,0X31,0XC6,0X96,0XDF,0X58,0XCE,0XB6,0X74,0X2D,
0X42,0XC9,0X74,0X30,0X95,0X35,0X5B,0X6E,0X74,0X31,0X74,0X51,0X5B,0X6D,0X7C,0X91,
0XAD,0XF6,0X6C,0X4F,0X8D,0X33,0X74,0X71,0X53,0X6D,0X42,0XCC,0X6B,0XF1,0X9D,0X77,
0X95,0X33,0X53,0X0A,0X5B,0X6B,0X6B,0XED,0X84,0XD0,0X95,0X51,0XAD,0XF3,0XAD,0XD2,
0XAD,0XD2,0X5B,0X69,0X84,0X8E,0X6C,0X0D,0X53,0X8C,0X74,0X72,0X85,0X36,0X7C,0XF5,
0XBE,0XDC,0X7C,0XD3,0XC6,0XFC,0XAE,0X38,0XBE,0XB9,0XA5,0XB5,0XC6,0X98,0XE7,0X7B,
0XCE,0XB7,0X6B,0XAC,0X84,0X8F,0X8C,0XB0,0X3A,0X67,0X9D,0X74,0XBE,0X98,0X53,0X2A,
0X8D,0X0F,0X95,0X70,0XCF,0X38,0XC6,0XD7,0XB6,0X36,0XEF,0XDD,0XB6,0X36,0X6B,0XED,
0X8D,0X10,0XB6,0X74,0XB6,0X92,0X6C,0X68,0X64,0X45,0X43,0X80,0X5C,0X63,0X43,0XA0,
0X4B,0X81,0X5B,0XE4,0X5C,0X04,0X64,0X26,0XE7,0XF7,0X8D,0X6D,0X53,0XA6,0X64,0X29,
0XE7,0X59,0XD6,0XB7,0X4A,0X86,0XBD,0XF4,0XF7,0X9A,0XCE,0X75,0XDF,0X18,0XF7,0XBB,
0XFF,0XFC,0XFF,0XDC,0XFF,0XFD,0XE7,0X5B,0XF7,0XDD,0XE7,0X1A,0XBD,0XF6,0XBD,0XD5,
0XFF,0XFD,0XD6,0X35,0X83,0XCB,0X5A,0XA6,0X94,0X8D,0XB5,0X91,0XBD,0XD2,0XA5,0X0F,
0XB5,0XB1,0X73,0XA9,0X5B,0X08,0XB5,0XD3,0XC6,0X56,0X84,0X6E,0X84,0X2E,0XE7,0X1B,
0XA5,0X36,0X84,0X54,0X84,0XB4,0XBE,0XBB,0XD7,0X5D,0X8C,0XF2,0X52,0X87,0XAC,0XAE,
0XF6,0X52,0X9B,0X02,0XD4,0X86,0XFD,0XCB,0XCC,0XA7,0XDD,0X8D,0XC5,0X2E,0X7A,0XE8,
0X72,0X87,0X83,0X09,0X7A,0XC6,0X93,0X68,0X9B,0X89,0X9B,0X48,0XAB,0XC9,0XA3,0X67,
0XAB,0XE8,0XAC,0X09,0XAC,0X0B,0XAB,0XEB,0XB4,0X0D,0XC4,0X6D,0XCC,0XAB,0XD4,0XCB,
0XD4,0XCD,0XCC,0XCD,0XD4,0XEE,0XD4,0XEE,0XCC,0XAD,0XD4,0XEE,0XE5,0X4F,0XE5,0X70,
0XDD,0X2F,0XE5,0X4F,0XE5,0X70,0XE5,0X90,0XED,0XD1,0XF5,0XF2,0XED,0XD0,0XE5,0X4E,
0XD4,0XEC,0XDD,0X6E,0XCD,0X2D,0XCD,0X2D,0XD4,0XEA,0XE5,0X2A,0XDC,0XC9,0XFE,0X52,
0XA3,0XCB,0X20,0X20,0X8B,0X0C,0XA3,0XEF,0X38,0XC3,0X49,0XE9,0X21,0XAA,0X22,0X2D,
0X19,0X8A,0X42,0X0B,0X61,0XE7,0XD4,0XCE,0XF5,0X8E,0X8A,0X60,0XEE,0X2F,0X9C,0X08,
0X73,0X27,0X52,0X44,0X49,0XC2,0X49,0XC1,0X51,0XA1,0X59,0XE2,0X5A,0X05,0X41,0X63,
0X31,0X22,0X21,0X01,0X29,0X42,0X4A,0X25,0X5A,0XC8,0X52,0X66,0X42,0X25,0X4A,0X66,
0X6B,0X8A,0XBE,0X34,0X8C,0XAE,0X9D,0X30,0XBE,0X75,0XC6,0X96,0X3A,0X65,0X21,0XA3,
0X6B,0XCD,0X42,0XAA,0X2A,0X07,0X3A,0X69,0X4A,0XEB,0X4A,0XCA,0X63,0XCD,0X32,0X67,
0X63,0XCC,0XAE,0X56,0X9D,0XB4,0X84,0XF1,0XB6,0X78,0X95,0X55,0X95,0X56,0XAD,0XF8,
0XD7,0X1A,0XCE,0XB8,0XD7,0X19,0XDF,0X5A,0XA5,0XB3,0XAE,0X14,0XC6,0XB6,0XCE,0XD6,
0X95,0X30,0X32,0X04,0X74,0X2D,0X42,0XC8,0X2A,0X48,0X53,0XCF,0X6C,0XB4,0X5C,0X53,
0X64,0X12,0X8D,0X36,0XAE,0X59,0XBE,0XDA,0XCF,0X1B,0XD7,0X5B,0XAD,0XF5,0X6B,0XCC,
0XC6,0X76,0X84,0XAE,0X84,0XAE,0X9D,0X92,0X4A,0XE8,0X84,0XCF,0XA5,0XB3,0X6C,0X0C,
0X95,0X70,0X9D,0X90,0X9D,0XB1,0XAE,0X33,0XC6,0XD6,0XCE,0XF7,0XBE,0X75,0XB6,0X33,
0XC6,0XD5,0X95,0X4E,0X84,0XEC,0X5B,0XC6,0X43,0X22,0X2A,0XC0,0X4B,0XA2,0X3B,0X41,
0X4B,0X64,0X4B,0X45,0X74,0X6A,0XCF,0X36,0X8D,0X0E,0XAE,0X54,0XDF,0XBA,0X74,0X8E,
0XCE,0XB6,0X9D,0X0F,0X52,0XA6,0X7C,0X2C,0XAD,0X92,0X84,0X4D,0X42,0X45,0X94,0XCF,
0XC6,0X55,0XDE,0XF8,0XAD,0X73,0XE7,0X19,0XEF,0X7B,0XC6,0X36,0XDE,0XFA,0XC6,0X36,
0XD6,0X35,0XFF,0XFC,0XC5,0XB3,0X7B,0X8A,0XCE,0X13,0XC5,0XF2,0XC5,0XF2,0XB5,0XB1,
0XAD,0X70,0X8C,0X6D,0XAD,0X71,0XC6,0X35,0XAD,0X52,0X6B,0X6A,0XB5,0XB4,0XAD,0X54,
0X52,0X8C,0X6B,0X71,0X84,0X95,0XA5,0XB9,0XC6,0XBC,0X7C,0X0F,0X83,0XAB,0XFE,0XF5,
0XAB,0XC7,0XBC,0X06,0XD4,0XA7,0XD4,0XA7,0XCC,0X88,0XDD,0X4B,0X7A,0X81,0X82,0XE4,
0XCD,0X2F,0XCD,0X10,0XDD,0XB4,0XBC,0XF1,0XB4,0XB1,0XC5,0X11,0XB4,0X8E,0XB4,0XAD,
0XAC,0X4D,0XAC,0X4E,0X9C,0X0F,0X93,0X8E,0X8B,0X4D,0X8B,0X0B,0X8A,0XE8,0X82,0XA6,
0X82,0XE8,0X7A,0XA8,0X7A,0XC8,0X7A,0XC8,0X7A,0X87,0X7A,0X87,0X82,0XC8,0X82,0XA7,
0X8B,0X09,0X93,0X09,0X93,0X28,0X93,0X08,0X93,0X28,0X9B,0X69,0XA3,0X89,0X9B,0X69,
0X9B,0XC9,0XAC,0X4B,0XC4,0XCC,0XC4,0XAC,0XC4,0X08,0XD4,0X4A,0XBB,0XEA,0XD4,0XF0,
0X8B,0X0B,0X38,0XC2,0X9B,0X4B,0X92,0XE8,0X48,0XA0,0X49,0X41,0X31,0X85,0X21,0XA6,
0X3A,0X26,0X39,0X81,0X82,0XC3,0XF5,0XCE,0XDC,0XCA,0X8A,0X41,0XFE,0XD5,0X83,0X27,
0X4A,0X03,0X39,0XA1,0X51,0XE1,0X62,0X43,0X62,0X02,0X62,0X43,0X62,0X45,0X49,0XE4,
0X41,0XE4,0X29,0X22,0X31,0X62,0X42,0X05,0X4A,0X45,0X4A,0X24,0X42,0X24,0X42,0X24,
0X8C,0X6D,0XD6,0XD7,0X94,0XEF,0XA5,0X51,0XC6,0X75,0XBE,0X34,0X32,0X24,0X32,0X05,
0X84,0X90,0X5B,0X8C,0X3A,0X68,0X42,0XA9,0X53,0X6B,0X4B,0X09,0X5B,0XAB,0X3A,0X86,
0X3A,0XC6,0X3A,0XA6,0XA6,0X13,0X9D,0XD3,0X74,0X8F,0XA6,0X16,0XCF,0X3B,0X9D,0X74,
0XBE,0X35,0XB5,0XD3,0XB5,0XB3,0XBE,0X35,0X84,0X6E,0X53,0X08,0X5B,0X69,0XA5,0XB2,
0X7C,0X4D,0X29,0XE3,0X6C,0X0C,0X4B,0X2A,0X2A,0X89,0X3B,0X4C,0X64,0XD3,0X6C,0XF5,
0X6C,0X73,0X84,0XF5,0X8D,0X55,0X84,0XF3,0X7C,0XB1,0X84,0XF1,0X74,0X2D,0X42,0XA6,
0X9D,0X70,0XAE,0X13,0XC6,0XD5,0XAE,0X12,0X7C,0XAD,0X5B,0X89,0X53,0X68,0X4B,0X27,
0X3A,0XA4,0XB6,0X53,0XA5,0XB1,0X3A,0XA4,0X42,0X84,0X63,0XA8,0X5B,0X87,0X63,0XA7,
0X63,0XE8,0X4B,0X25,0X4B,0X24,0X53,0X86,0X3A,0XE2,0X43,0X23,0X3B,0X02,0X32,0X81,
0X3A,0XC4,0X42,0XC4,0X4B,0X26,0X6C,0X0A,0X4B,0X07,0X84,0XEE,0XB6,0X75,0X9D,0XB2,
0XB5,0XD3,0X9D,0X10,0X63,0X49,0X8C,0X8E,0XB5,0XF3,0X6B,0X69,0X3A,0X24,0XA5,0X51,
0XBE,0X14,0XCE,0X76,0XA5,0X52,0XDE,0XD8,0XE7,0X5A,0XAD,0X73,0XEF,0X7B,0XD6,0X97,
0XB5,0X31,0XFF,0X7A,0XEE,0XD7,0X94,0X0C,0XBD,0X71,0XAD,0X0F,0X9C,0XAD,0XB5,0X91,
0X84,0X2B,0XA5,0X30,0XD6,0XB6,0XAD,0X51,0X5A,0XE8,0XBD,0XF5,0XBD,0XB4,0X84,0X4F,
0X52,0XC9,0X42,0X68,0X7C,0X51,0X74,0X31,0XAD,0XB6,0XB5,0XD4,0XAC,0XCE,0XD5,0X6E,
0XC4,0X68,0XD4,0XA9,0XB3,0XA6,0XDC,0XEC,0XDC,0XED,0XAB,0X87,0X7A,0X01,0XFF,0X55,
0XED,0XAF,0XED,0XB0,0XED,0XF6,0XC4,0XF4,0XB4,0XD4,0XCD,0XF7,0XC5,0XD4,0XCE,0X15,
0XCE,0X36,0XCE,0X38,0XC6,0X1A,0XC6,0X1B,0XD6,0X5B,0XE6,0X9A,0XEE,0XB8,0XEE,0X97,
0XF6,0XD9,0XE6,0X78,0XE6,0X78,0XEE,0XB9,0XE6,0X78,0XE6,0X57,0XE6,0X57,0XE6,0X37,
0XD5,0XB5,0XD5,0X94,0XCD,0X53,0XC5,0X11,0XBC,0XAF,0XB4,0X4E,0XB4,0X4D,0XB4,0X8E,
0XA4,0X6F,0X93,0XED,0X7A,0XA6,0XA3,0XA8,0X9A,0XE5,0X8A,0X43,0X8A,0XA7,0X6A,0X06,
0X59,0XC6,0X72,0X26,0XE5,0X0F,0XEC,0XEC,0XE4,0XA9,0XE5,0X0A,0XD5,0XAE,0XB5,0X4C,
0XAD,0X08,0XB4,0XE5,0XF6,0X49,0XDD,0X05,0XFD,0XCB,0XAB,0X66,0X69,0XE3,0X7A,0XE9,
0X41,0XC3,0X41,0XC1,0X62,0X63,0X7A,0XC4,0X6A,0X63,0X62,0X22,0X5A,0X43,0X49,0XE2,
0X39,0XC3,0X21,0X01,0X29,0X21,0X39,0XC3,0X39,0XA2,0X31,0X82,0X41,0XE3,0X4A,0X45,
0X94,0XAE,0XD6,0XB6,0X8C,0XAE,0X9D,0X10,0XBE,0X55,0XA5,0X72,0X19,0X62,0X29,0XE4,
0X9D,0XB3,0X7C,0X8E,0X3A,0XA7,0X3A,0X86,0X53,0X69,0X42,0XE6,0X5B,0XA8,0X4B,0X26,
0X74,0X6A,0X74,0X6A,0X7C,0XEC,0X8D,0X4E,0XD7,0XB9,0XC7,0X18,0X95,0X92,0XBE,0XB7,
0XBE,0X13,0XCE,0X75,0XBE,0X14,0XB6,0X14,0X7C,0X4D,0X42,0X86,0X32,0X25,0X84,0X8E,
0X8C,0XCF,0X4A,0XE7,0X74,0X6E,0X7C,0XD0,0X6C,0X90,0X54,0X4F,0X75,0X75,0X7D,0X76,
0X85,0X55,0X8D,0X55,0X7C,0XD2,0X5B,0XCD,0X53,0X6B,0X6C,0X4E,0X7C,0XAE,0X6C,0X0A,
0X6C,0X2A,0X74,0X4A,0X5B,0X87,0X42,0XE5,0X53,0X66,0X4B,0X46,0X42,0XE4,0X43,0X25,
0X53,0X87,0X53,0X87,0X53,0X67,0X63,0XE8,0X7C,0X8B,0X74,0X69,0X5B,0XA6,0X53,0X65,
0X6C,0X08,0X84,0XAA,0X7C,0X8A,0X7C,0XAA,0X21,0XE0,0X2A,0X41,0X22,0X00,0X32,0X82,
0X32,0X62,0X22,0X01,0X3A,0XE3,0X64,0X07,0X6C,0X27,0X63,0XE6,0X64,0X26,0XA6,0X4E,
0X8C,0XAF,0X73,0XCC,0X3A,0X05,0X6B,0XAA,0X9D,0X30,0X52,0XE7,0X42,0X45,0X73,0XEB,
0XCE,0X76,0XD6,0XB7,0XDE,0XF8,0XE7,0X19,0XBE,0X15,0XC6,0X36,0XDE,0XD9,0XE7,0X19,
0XB5,0X31,0XC5,0X93,0XFF,0XFC,0XB5,0X30,0X94,0X6D,0XD6,0X75,0XAD,0X50,0XAD,0X70,
0XA5,0X50,0XBE,0X13,0XCE,0X75,0X63,0X49,0X84,0X0D,0XC6,0X36,0X9C,0XB1,0X6B,0X4B,
0X7C,0X0C,0X74,0X0C,0X5B,0X4B,0X94,0XD2,0X7B,0XCD,0XB5,0X51,0X8B,0X46,0X82,0XA2,
0X8A,0XA1,0X8A,0X82,0X82,0X64,0X9B,0X07,0X92,0XC7,0X8A,0X44,0XFF,0X56,0XF5,0X4D,
0XD4,0X69,0XCC,0X4A,0X82,0X89,0X59,0XA8,0X39,0X47,0X31,0X46,0X42,0X06,0X31,0X64,
0X4A,0X8A,0X42,0X4B,0X32,0X0B,0X31,0XEB,0X3A,0X2B,0X52,0X8A,0X62,0XCA,0X6A,0XC9,
0X6B,0X0B,0X5A,0XCA,0X6B,0X2C,0X7B,0XCE,0X83,0XEF,0X8C,0X0F,0X94,0X51,0X9C,0X71,
0XAC,0XD2,0XAD,0X13,0XBD,0X74,0XCD,0XD6,0XCD,0XD6,0XCD,0XB5,0XD5,0XF6,0XDE,0X77,
0XD6,0X78,0XEE,0XF9,0XFE,0XD5,0XFF,0X35,0XFE,0X92,0XFF,0X15,0XED,0XF3,0X83,0X0A,
0X61,0XE6,0X9B,0X6B,0XD4,0X6C,0XD3,0XE7,0XE4,0X25,0XDC,0X65,0XD4,0XE7,0XBC,0XE6,
0XCD,0X65,0XCD,0X43,0XE5,0XC4,0XD4,0XE3,0XE5,0X6A,0X82,0X82,0X49,0X21,0X51,0XE5,
0X49,0XC4,0X49,0XC2,0X6A,0X63,0X7A,0XC5,0X6A,0X43,0X5A,0X02,0X52,0X42,0X42,0X22,
0X39,0XA3,0X29,0X42,0X29,0X21,0X39,0XC4,0X4A,0X45,0X39,0X82,0X31,0XA2,0X5A,0XE7,
0XA5,0X0F,0XD6,0XD7,0X94,0XEF,0XA5,0X72,0XD6,0XF8,0XA5,0X93,0X21,0X83,0X32,0X46,
0XAE,0X14,0X8D,0X10,0X42,0XE7,0X32,0X64,0X53,0X68,0X42,0XE5,0X63,0XE8,0X6C,0X48,
0X6C,0X48,0X53,0XA5,0X5B,0XE7,0X6C,0X69,0X6C,0X8B,0X95,0XB1,0XEF,0XFD,0XDF,0XDA,
0X9D,0X50,0XB5,0XF2,0XAD,0XB2,0XAD,0XB3,0X7C,0X6E,0X53,0X08,0X3A,0X46,0X63,0X8B,
0X63,0XAB,0X5B,0X6A,0X84,0XF0,0X95,0X93,0X64,0X6E,0X22,0XC9,0X64,0XF1,0X8E,0X37,
0X85,0X74,0X8D,0X74,0X85,0X53,0X74,0XD0,0X85,0X10,0XAE,0X54,0XCF,0X57,0XC7,0X35,
0XC6,0XF4,0XB6,0X92,0X9D,0XAE,0X8D,0X6D,0X3A,0XA2,0X32,0XA2,0X2A,0X41,0X2A,0X41,
0X6C,0X2A,0X8D,0X4E,0XAE,0X72,0XB6,0X52,0XB6,0X92,0XD7,0X96,0XD7,0X55,0X9D,0XAE,
0XA5,0XEF,0XE7,0XD7,0XE7,0XD8,0XC6,0XB4,0X42,0XC5,0X3A,0X84,0X19,0XC1,0X21,0XC1,
0X11,0X60,0X43,0X24,0X64,0X27,0X64,0X26,0X74,0XC8,0X74,0XA6,0X5B,0XE2,0X6C,0XA5,
0X42,0X47,0X3A,0X46,0X6B,0XAC,0X6B,0XAC,0X42,0X87,0X19,0X21,0X21,0X42,0X6B,0XAB,
0XB5,0XF4,0XD6,0XD8,0XD6,0XF9,0XD6,0XD8,0X94,0X90,0XEF,0X5B,0XCE,0X78,0XE6,0XFA,
0XE6,0XB8,0X9C,0X4E,0XEE,0XF8,0XEF,0X38,0XB5,0X71,0XCE,0X54,0X8C,0X6D,0X9C,0XEF,
0XCE,0X95,0XC6,0X55,0X7B,0XEC,0X8C,0X8F,0XC6,0X37,0X7C,0X0E,0X7B,0XCE,0X7B,0XED,
0XE7,0X18,0XEF,0X79,0XD6,0XB8,0XD6,0X98,0XFF,0X9C,0X7A,0XE7,0XBC,0X8B,0XED,0XAE,
0XFE,0X10,0XD4,0XED,0X8B,0X07,0X8B,0X08,0X7A,0X66,0XFE,0XB5,0XDC,0X89,0XCB,0XC5,
0XD4,0X07,0XB3,0X47,0X69,0X85,0X51,0X87,0X49,0XC9,0X41,0XA8,0X5A,0X69,0X4A,0X28,
0X52,0X4A,0X42,0X0A,0X31,0XCA,0X29,0XAA,0X29,0XC9,0X39,0XE8,0X49,0XE8,0X4A,0X08,
0X42,0X48,0X32,0X07,0X3A,0X28,0X3A,0X28,0X29,0XC7,0X21,0X66,0X29,0X86,0X29,0XA7,
0X31,0XC8,0X31,0XE8,0X3A,0X29,0X42,0X49,0X39,0XE8,0X29,0X86,0X29,0X65,0X31,0XC5,
0X41,0XE4,0X52,0X01,0XAC,0X27,0XCC,0XE8,0XBC,0X66,0XC4,0XC9,0X72,0XA4,0X49,0XA2,
0X83,0X09,0XC4,0XAF,0XC4,0X0B,0XCB,0XC9,0XD3,0XC6,0XCB,0XC5,0XBB,0XC4,0XBC,0X03,
0XE5,0X67,0XC4,0X82,0XDD,0X24,0XE5,0X88,0XDD,0X6B,0X7A,0XA3,0X51,0XC3,0X49,0XA4,
0X49,0XC4,0X41,0X82,0X62,0X23,0X72,0XA5,0X6A,0X64,0X5A,0X43,0X52,0XA4,0X4A,0X84,
0X42,0X04,0X39,0XE5,0X29,0X22,0X4A,0X46,0X84,0X0C,0X5A,0XA6,0X31,0X61,0X5A,0XE7,
0XA5,0X0F,0XCE,0X75,0X8C,0X8E,0X95,0X10,0XC6,0X97,0X9D,0X32,0X19,0X42,0X29,0XC4,
0XAE,0X35,0X95,0X72,0X43,0X08,0X2A,0X24,0X4B,0X07,0X3A,0XA4,0X5B,0XA7,0X74,0X89,
0X64,0X27,0X5B,0XE6,0X53,0XC5,0X43,0X44,0X32,0XC4,0X43,0X27,0X8D,0X91,0X85,0X10,
0X4B,0X07,0X4A,0XA6,0X52,0XE8,0X6C,0X0C,0X4A,0XE9,0X21,0XC5,0X19,0X83,0X42,0XA8,
0X7C,0X8F,0X74,0X2D,0X5B,0XCB,0X6C,0X6D,0X5C,0X2C,0X2A,0XC7,0X5C,0X8E,0X7D,0X92,
0X85,0X93,0X8D,0XB3,0X95,0XB2,0X8D,0X70,0X8D,0X70,0XA6,0X31,0XAE,0X72,0X9D,0XEF,
0XBE,0XD2,0XB6,0XB1,0XBE,0XD2,0XB6,0XD1,0X43,0X03,0X2A,0X61,0X2A,0X41,0X2A,0X62,
0X6C,0X4B,0X85,0X2F,0X95,0X90,0X9D,0XF1,0XA5,0XF1,0X85,0X0E,0X9D,0XB1,0XF7,0XFB,
0XD7,0X37,0XF7,0XFC,0XDF,0X99,0X9D,0X71,0X4B,0X07,0X53,0X49,0X3A,0XA6,0X19,0XA1,
0X32,0XA3,0X53,0XC6,0X53,0XA5,0X4B,0X63,0X53,0XC3,0X53,0XC2,0X5C,0X23,0X5C,0X22,
0X42,0X47,0X3A,0X06,0X4A,0XC9,0X5B,0X2A,0X32,0X05,0X21,0X63,0X21,0X63,0X29,0XA3,
0XAD,0X93,0XC6,0X56,0XC6,0X57,0XA5,0X53,0XAD,0X94,0XDE,0XFA,0XEF,0X5C,0XCE,0X37,
0XF7,0X5B,0XB5,0X52,0XB5,0X52,0XEF,0X39,0XD6,0XB7,0XAD,0X92,0X94,0XCF,0X9D,0X51,
0XB6,0X14,0X95,0X31,0X6B,0XAC,0XC6,0X57,0X9D,0X33,0X5B,0X2B,0X52,0XCB,0X7B,0XEE,
0XB5,0X51,0XC5,0XB2,0XD6,0X35,0XB5,0X32,0X83,0X4A,0XEE,0X34,0XFE,0XB3,0XC4,0X49,
0XDD,0X2D,0XED,0XF2,0X6A,0X66,0X72,0X86,0XF6,0X75,0XF6,0X11,0XE4,0XC8,0XDC,0X46,
0XCB,0XA5,0X92,0X42,0X72,0X04,0X59,0XC5,0X5A,0X07,0X62,0X68,0X51,0XA4,0X62,0X05,
0X51,0XA5,0X49,0XA5,0X41,0XC6,0X39,0XE7,0X39,0XE6,0X39,0XE6,0X49,0XC6,0X49,0XE6,
0X39,0XC6,0X31,0XC6,0X3A,0X27,0X42,0X28,0X31,0XE7,0X39,0XE8,0X42,0X29,0X42,0X4A,
0X31,0XE9,0X31,0XE9,0X31,0XE9,0X31,0XE9,0X29,0XC8,0X29,0XA7,0X29,0XC8,0X39,0XE7,
0X52,0X45,0X83,0X05,0X82,0X80,0XC4,0X66,0XBC,0X66,0X8B,0X23,0X6A,0XA4,0X49,0XC2,
0X72,0XA7,0XB4,0X2D,0XB3,0XAA,0XD4,0X6C,0XE4,0XCD,0XED,0X0C,0XE4,0XCA,0XED,0X0A,
0XE4,0XA8,0XDC,0X87,0XDC,0XE9,0XCC,0XC9,0XEE,0X30,0X93,0XA8,0X31,0X20,0X62,0X86,
0X62,0X87,0X5A,0X04,0X6A,0X65,0X7A,0XE7,0X6A,0XA5,0X5A,0X84,0X4A,0XA5,0X42,0X44,
0X31,0XC4,0X39,0XA4,0X18,0XC0,0X42,0X05,0X94,0X6E,0X6B,0X49,0X31,0X61,0X4A,0X44,
0XB5,0X92,0XD6,0XB6,0X94,0XCF,0X8C,0XAF,0XB6,0X15,0X94,0XF1,0X29,0XC5,0X32,0X06,
0XBE,0X98,0X95,0X94,0X3A,0XA8,0X01,0X01,0X19,0XA2,0X11,0X60,0X42,0XE5,0X5B,0XA7,
0X5B,0XE7,0X3A,0XC2,0X4B,0X65,0X4B,0X65,0X53,0XC8,0X3B,0X27,0X3A,0XE8,0X6C,0X4D,
0X63,0XCB,0X6C,0X0B,0X6C,0X2D,0X7C,0X8F,0X5B,0X8C,0X5B,0XAD,0X6C,0X2F,0X8C,0XF1,
0XAE,0X15,0XC6,0XD7,0XA5,0XF3,0X95,0X71,0X6C,0X6C,0X33,0X06,0X5C,0X6C,0X7D,0X90,
0X75,0X0E,0X74,0XCD,0X64,0X6B,0X4B,0XA8,0X3B,0X24,0X43,0X65,0X4B,0X85,0X3B,0X02,
0X32,0XA0,0X32,0XC1,0X3A,0XC1,0X3A,0XC1,0X3A,0XE2,0X2A,0X40,0X2A,0X40,0X3A,0XE3,
0X2A,0XA2,0X32,0XE3,0X2A,0XA3,0X2A,0X63,0X32,0X84,0X43,0X05,0X53,0X88,0X64,0X0A,
0X64,0X0A,0X7C,0XAD,0X63,0XEA,0X32,0X44,0X32,0X64,0X6C,0X2B,0X8D,0X2F,0X7C,0XCC,
0X64,0X69,0X53,0XC5,0X32,0XE1,0X4B,0X83,0X3B,0X42,0X2A,0XA0,0X3B,0X20,0X53,0XC3,
0X5B,0X0B,0X63,0X8D,0X5B,0X4C,0X6B,0XCE,0X3A,0X48,0X32,0X06,0X32,0X06,0X42,0X47,
0XC6,0X77,0XC6,0X57,0XCE,0X78,0X9D,0X12,0XE7,0X1B,0XD6,0XB9,0XF7,0XBD,0XCE,0X58,
0XD6,0X58,0XEF,0X5B,0XD6,0X98,0XB5,0XB3,0XC6,0X56,0XC6,0X76,0XDF,0X39,0XA5,0X72,
0XAD,0XD4,0X7C,0X6E,0XBE,0X36,0X9D,0X33,0X74,0X2F,0X4A,0XCA,0X42,0X8A,0X42,0X28,
0X62,0X87,0X51,0XE3,0X49,0X83,0X62,0X46,0XCD,0X30,0XF5,0XD2,0X9A,0XE5,0XCC,0X29,
0XBC,0X0A,0X51,0X60,0X62,0X46,0XF7,0X39,0XFE,0XF6,0XAB,0XC7,0XC3,0XC3,0XE4,0X85,
0XF5,0X49,0XD4,0XC8,0XCD,0X2B,0XBC,0XCB,0XBC,0XEC,0XCD,0X2C,0XC4,0X69,0XD4,0XAA,
0XC4,0X08,0XB4,0X08,0XA4,0X28,0X9C,0X28,0X83,0XA8,0X73,0X27,0X72,0XC7,0X7A,0XC7,
0X62,0X46,0X62,0X47,0X6A,0X88,0X62,0X47,0X51,0XE6,0X51,0XE7,0X52,0X08,0X41,0XA7,
0X49,0XE9,0X49,0XE9,0X41,0XA8,0X39,0X67,0X31,0X67,0X39,0X88,0X39,0XA8,0X41,0XA6,
0X7A,0XE7,0XED,0XD1,0X9A,0XA3,0XA2,0XE4,0XA3,0X68,0X7A,0X86,0X6A,0X87,0X10,0X00,
0X59,0XA2,0X72,0X84,0X6A,0X02,0X69,0XE2,0X6A,0X04,0X7A,0X04,0X8A,0X24,0X9A,0X25,
0XAA,0X67,0XBA,0XEA,0X92,0X67,0XAB,0XCC,0XAC,0X6E,0X5A,0X64,0X62,0XE6,0X4A,0X23,
0X6A,0XC7,0X5A,0X04,0X62,0X25,0X6A,0XA6,0X5A,0X65,0X4A,0X24,0X3A,0X46,0X32,0X05,
0X31,0XC4,0X39,0XC4,0X21,0X01,0X31,0X62,0X5A,0XA7,0X52,0X85,0X31,0XA2,0X39,0XE3,
0X84,0X2C,0XA5,0X30,0X73,0XAB,0X63,0X4A,0X7C,0X2E,0X6B,0XAD,0X21,0X64,0X19,0X65,
0X9D,0X96,0X7C,0XD3,0X32,0X69,0X09,0X03,0X21,0XC5,0X21,0XC4,0X4B,0X07,0X4B,0X26,
0X42,0XE4,0X53,0X66,0X53,0X87,0X22,0X02,0X3A,0XE7,0X32,0XA7,0X3A,0XEA,0X9D,0XD5,
0X9D,0XF3,0XB6,0XB6,0X9D,0XD3,0X84,0XF1,0X74,0X90,0X8D,0X54,0X95,0X75,0XB6,0X57,
0XEF,0XFE,0XF7,0XFD,0XA5,0XD2,0X74,0X6C,0X6C,0X6B,0X4B,0XC8,0X43,0XC7,0X43,0XA7,
0X3B,0X45,0X3B,0X45,0X3B,0X24,0X2A,0XC2,0X2A,0X81,0X3B,0X02,0X4B,0XA4,0X4B,0XA4,
0X64,0X25,0X74,0XE8,0X6C,0X87,0X64,0X47,0X7C,0XC9,0X53,0XA5,0X3B,0X03,0X5C,0X27,
0X5C,0X66,0X54,0X65,0X64,0XC8,0X54,0X46,0X3B,0X23,0X53,0XC6,0X64,0X48,0X3A,0XE3,
0X32,0X82,0X53,0X86,0X53,0X85,0X32,0XA2,0X2A,0X60,0X4B,0X43,0X53,0XA5,0X53,0XC4,
0X4B,0XA3,0X5C,0X45,0X4B,0XC3,0X2A,0XE1,0X22,0X60,0X53,0XE6,0X3B,0X03,0X22,0X61,
0X4A,0X89,0X31,0XE6,0X5B,0X2B,0X4A,0XA9,0X3A,0X06,0X73,0XEE,0X3A,0X06,0X8C,0XB0,
0XDF,0X1A,0XDF,0X1A,0XC6,0X77,0XD6,0XB9,0XDF,0X1A,0XF7,0XBD,0XDE,0XDA,0XEF,0X5C,
0XDE,0XB9,0XD6,0X98,0XE7,0X1A,0XA5,0X32,0XA5,0X52,0X73,0XED,0X8C,0XAF,0X63,0X6A,
0X9D,0X52,0X9D,0X53,0XA5,0X94,0X9D,0X74,0X73,0XEF,0X6B,0XAF,0X5B,0X2D,0X4A,0X28,
0X51,0XE4,0X51,0XA2,0X59,0XC2,0XDD,0XD1,0XFE,0XB4,0XAB,0XA7,0XCC,0X6A,0XC4,0X2A,
0X69,0XA1,0X40,0XE0,0X8B,0X6A,0XDE,0X55,0XB4,0XAE,0XC4,0XCC,0XCC,0X88,0XDC,0XA7,
0XCC,0X67,0XDD,0X4B,0XC4,0XEA,0XBC,0XEA,0XB4,0X89,0XB4,0X27,0XD4,0XA9,0XCC,0X47,
0XDC,0XA8,0XDC,0XC8,0XD5,0X09,0XCD,0X4A,0XC5,0X2B,0XBD,0X0B,0XCD,0X0D,0XD5,0X70,
0XD5,0X70,0XD5,0X91,0XD5,0X92,0XCD,0X31,0XC4,0XD0,0XC4,0XF0,0XBC,0XB0,0XA3,0XED,
0XA3,0XEE,0X9B,0XEE,0X93,0X8D,0X83,0X2B,0X83,0X4B,0X8B,0X6C,0X83,0X4C,0X82,0XE9,
0XD5,0X0F,0XDC,0XED,0XA2,0XE5,0X82,0X03,0X7A,0X67,0X59,0XE6,0X20,0XA2,0X62,0XC9,
0XCD,0XD4,0XE6,0X96,0XF6,0XF7,0XE6,0X55,0XE6,0X77,0XDD,0XD5,0XED,0XB4,0XE4,0XF1,
0XE4,0X50,0XD3,0XAE,0XC3,0XEF,0X9B,0X4C,0X7B,0X09,0X52,0X65,0X39,0XC2,0X31,0XA1,
0X41,0XC3,0X29,0X01,0X31,0X21,0X41,0XC3,0X39,0XA3,0X39,0XE4,0X42,0X67,0X42,0X88,
0X52,0XA8,0X52,0XA8,0X4A,0X45,0X29,0X41,0X20,0XA0,0X31,0X20,0X41,0XC2,0X4A,0X03,
0X5A,0X86,0X7B,0XCB,0X5A,0XE8,0X42,0X67,0X5B,0X2A,0X53,0X0B,0X19,0X64,0X09,0X24,
0X5B,0X8F,0X5B,0XB0,0X3A,0XCB,0X3A,0XAA,0X63,0XED,0X63,0XEC,0X74,0X6D,0X53,0X69,
0X3A,0XA5,0X2A,0X63,0X09,0X60,0X3A,0XC5,0X6C,0X8D,0X32,0XA6,0X53,0XAB,0XAE,0X76,
0XE7,0XFB,0XDF,0XFA,0X9E,0X13,0X8D,0X92,0X9D,0XF4,0X8D,0X72,0X7C,0XD0,0XBE,0XD7,
0XF7,0XFD,0XEF,0XFB,0X84,0XCD,0X32,0X62,0X2A,0X81,0X2A,0XC2,0X2B,0X01,0X2B,0X01,
0X2B,0X01,0X3B,0X83,0X5C,0X47,0X64,0X88,0X5C,0X67,0X6C,0XA8,0X74,0XE9,0X74,0XE8,
0X6C,0XA7,0X74,0XC7,0X43,0X62,0X53,0XE5,0X53,0XA5,0X53,0XC6,0X3A,0XE3,0X5C,0X27,
0X5C,0X86,0X5C,0XC6,0X54,0X65,0X3B,0X82,0X33,0X42,0X3B,0X42,0X4B,0X84,0X53,0XE6,
0X2A,0X80,0X4B,0X64,0X4B,0X43,0X43,0X02,0X2A,0X60,0X3A,0XC0,0X3A,0XE0,0X4B,0X82,
0X3B,0X21,0X43,0X83,0X3B,0X42,0X1A,0X20,0X11,0XE0,0X64,0X49,0X32,0XC4,0X3B,0X05,
0X42,0X46,0X29,0X62,0X21,0X21,0X4A,0X25,0X29,0X63,0X5A,0XC9,0XD6,0XB8,0XD6,0XB8,
0X8C,0XB0,0X9D,0X32,0X8C,0XB0,0XA5,0X52,0XAD,0X94,0XBD,0XD5,0XB5,0XB4,0XCE,0X56,
0XD6,0X56,0XD6,0X96,0XCE,0X56,0XAD,0X52,0X7B,0XEC,0X42,0X05,0X63,0X4A,0X6B,0XCC,
0XBE,0X56,0X9D,0X52,0X9D,0X53,0X94,0XF2,0X73,0XCE,0X73,0X8E,0X4A,0X49,0X5A,0X69,
0X59,0XE5,0X62,0X03,0XE5,0XF0,0XF6,0X70,0XAB,0XE6,0XC4,0XA9,0XBC,0X69,0X59,0X60,
0X61,0X81,0X7A,0X86,0X59,0X63,0X72,0X26,0X82,0XA7,0X72,0X45,0X9B,0X69,0X92,0XE6,
0X9B,0X27,0XA3,0XA8,0XA3,0XA8,0X9B,0X68,0XA3,0XC9,0XC4,0X8C,0XCC,0XCC,0XC4,0X6A,
0XD4,0XEC,0XD4,0XCB,0XD5,0X0B,0XCC,0XEB,0XBC,0XAA,0XC4,0XEB,0XD5,0X2C,0XCD,0X2C,
0XBD,0X2D,0XC5,0X2D,0XBC,0XED,0XBC,0XCD,0XCC,0XED,0XC4,0XCB,0XC4,0XA9,0XCC,0XCA,
0XD5,0X0C,0XD5,0X2D,0XD5,0X0C,0XE5,0X8D,0XCC,0XC9,0XE5,0X6C,0XDD,0X0C,0XDD,0X2D,
0XC4,0X8B,0XC4,0X6B,0X72,0X43,0X59,0XC3,0X39,0X63,0X10,0XA1,0X42,0X69,0X6B,0XCF,
0X6B,0XAE,0X6B,0X8D,0X73,0XAE,0X94,0X50,0XAC,0XF2,0XA4,0X90,0XC5,0X72,0XCD,0X2F,
0XFE,0XD5,0XFE,0X74,0XFE,0X75,0XEE,0X34,0X7B,0X28,0X52,0X45,0X4A,0X86,0X52,0XC8,
0X5B,0X29,0X52,0XE8,0X4A,0XC8,0X52,0XE8,0X63,0X6A,0X73,0XAC,0X6B,0X6C,0X5B,0X0A,
0X5B,0X0A,0X52,0X88,0X62,0XC8,0X73,0X49,0X6A,0XC6,0X59,0XE2,0X51,0XA1,0X59,0XE2,
0X6A,0XC6,0X6B,0X08,0X63,0X29,0X63,0X6B,0X6C,0X2F,0X64,0X0F,0X53,0XAE,0X74,0XB2,
0X74,0X94,0X4B,0X6F,0X4B,0X4C,0X64,0X4E,0X6C,0X6D,0X5B,0XEA,0X64,0X0B,0X84,0XEF,
0X7C,0XCE,0X2A,0X23,0X6C,0X6B,0X8D,0X4E,0X74,0XCB,0X6C,0X8A,0X6C,0X8B,0X74,0XAB,
0X9D,0XCF,0X85,0X0C,0X5B,0XE7,0X4B,0X65,0X64,0X69,0X7D,0X0C,0X7D,0X0C,0X9E,0X10,
0XC7,0X14,0XBE,0XD3,0X85,0X0B,0X53,0XC5,0X64,0X86,0X54,0X44,0X65,0X06,0X65,0X26,
0X6D,0X26,0X75,0X68,0X6D,0X07,0X5C,0X86,0X54,0X25,0X7D,0X4A,0X74,0XE9,0X6C,0XC8,
0X74,0XE8,0X6C,0XA7,0X32,0XE0,0X1A,0X20,0X1A,0X20,0X32,0XE3,0X3B,0X25,0X54,0X08,
0X75,0X6B,0X64,0XE8,0X3B,0XA3,0X43,0XC4,0X33,0X42,0X22,0XA0,0X4B,0XE4,0X5C,0X46,
0X2A,0X80,0X43,0X43,0X64,0X06,0X3A,0XE2,0X4B,0X23,0X21,0XE0,0X32,0X61,0X3A,0XC2,
0X3A,0XC2,0X3A,0XE3,0X3B,0X03,0X32,0XC3,0X2A,0X41,0X5C,0X08,0X4B,0X46,0X43,0X05,
0X4A,0X45,0X52,0XA6,0X52,0X86,0X83,0XEC,0X4A,0X46,0X7B,0X8B,0XD6,0X97,0XDE,0XF8,
0XAD,0X93,0X95,0X11,0X5B,0X2A,0X4A,0X87,0X52,0XC8,0X5A,0XE9,0X5A,0XA8,0X62,0XE9,
0X62,0XE8,0X7B,0XEC,0X8C,0X2D,0X52,0X66,0X52,0X66,0X62,0XE8,0X9C,0XF0,0X94,0XF0,
0X9D,0X31,0X42,0XA8,0X31,0XE5,0X29,0XC5,0X39,0XE6,0X52,0X69,0X39,0X86,0X51,0XE6,
0X7A,0XC8,0XE5,0XB1,0XEE,0X10,0XA3,0X84,0XB4,0X66,0XE5,0XCC,0XAC,0X07,0X9B,0X66,
0X8A,0XE6,0X6A,0X03,0XF5,0XF4,0XFE,0X97,0XDD,0X52,0XF6,0X36,0XD5,0X54,0XCD,0X13,
0XA3,0XEE,0XAC,0X2E,0XA3,0XED,0X93,0X4B,0X8B,0X09,0X8B,0X29,0X82,0XE8,0X7A,0X86,
0X7A,0X86,0X7A,0XA5,0X83,0X07,0X8B,0X47,0X8B,0X26,0X93,0X87,0X9B,0XC8,0X93,0XA7,
0XA4,0X6A,0XAC,0X8C,0XAC,0X6D,0XB4,0X6D,0XCC,0XCD,0XD5,0X0C,0XDD,0X2B,0XE5,0X6C,
0XE5,0X4D,0XDD,0X2C,0XC4,0X68,0XE5,0X4A,0XE5,0X49,0XA3,0X42,0XDC,0X89,0XCC,0X49,
0XCC,0X8B,0XD5,0X4F,0XAC,0X4C,0X28,0XC0,0X18,0XC1,0X3A,0X28,0X95,0X34,0X42,0XCC,
0X08,0XE4,0X29,0XA8,0X42,0X4A,0X18,0XC3,0X31,0X64,0X41,0XA4,0X49,0XC3,0X93,0XA8,
0XD5,0X0D,0XCC,0X8A,0XBC,0X4A,0X82,0XC5,0X8B,0X89,0X52,0X25,0X52,0XC8,0X5B,0X2A,
0X53,0X09,0X42,0XC9,0X3A,0X67,0X32,0X06,0X31,0XE6,0X31,0XA5,0X29,0X24,0X18,0XE2,
0X10,0XC1,0X29,0X42,0X41,0XC3,0X5A,0X45,0X9B,0XEA,0XDD,0XD1,0XC5,0X0E,0X72,0XA5,
0X49,0X81,0X52,0X25,0X4A,0X67,0X7C,0X2F,0X8D,0X34,0X85,0X34,0X7D,0X34,0X7D,0X14,
0X7C,0XD5,0X43,0X4E,0X22,0X47,0X7D,0X0F,0X6C,0XAC,0X85,0X4E,0X64,0X0A,0X64,0X0B,
0X7C,0XAE,0X84,0XEE,0X74,0X6B,0X53,0XA6,0X6C,0X48,0X64,0X27,0X43,0X44,0X5B,0XC6,
0X32,0X81,0X2A,0X40,0X32,0XA1,0X3A,0XE1,0X43,0X83,0X5C,0X47,0X64,0X88,0X74,0XC9,
0X5B,0XE6,0X6C,0X68,0X64,0X06,0X74,0XC8,0X85,0X89,0X75,0X47,0X75,0X86,0X75,0XA6,
0X75,0XA6,0X75,0X87,0X75,0X88,0X5C,0X86,0X33,0X22,0X43,0XA4,0X53,0XE6,0X5C,0X46,
0X6C,0XA7,0X7D,0X29,0X64,0X86,0X5C,0X25,0X32,0XC1,0X09,0XA0,0X09,0X80,0X43,0X86,
0X54,0X27,0X6D,0X0A,0X54,0X46,0X22,0XC0,0X0A,0X00,0X2B,0X01,0X5C,0X46,0X43,0X83,
0X53,0XE5,0X53,0XC5,0X43,0X23,0X43,0X23,0X32,0X61,0X2A,0X21,0X19,0XA0,0X2A,0X21,
0X2A,0X01,0X22,0X01,0X00,0XE0,0X4B,0X46,0X4B,0X25,0X2A,0X22,0X95,0X8F,0X4B,0X46,
0X52,0XC7,0X63,0X49,0X4A,0X45,0X94,0X6E,0X6B,0X4A,0XA4,0XD0,0XC6,0X36,0XBE,0X15,
0XCE,0X97,0XB5,0XF5,0X63,0X6B,0X29,0XC4,0X3A,0X26,0X39,0XE5,0X41,0XE5,0X41,0XE5,
0X31,0X83,0X63,0X29,0X9C,0XAF,0X52,0X66,0X63,0X09,0X73,0X8B,0X94,0X8F,0X6B,0X8B,
0X9D,0X31,0X4A,0XC8,0X4A,0XA8,0X29,0X84,0X39,0XE6,0X62,0XCA,0X39,0X85,0X51,0XE6,
0XB4,0X8F,0XFE,0XD6,0XC4,0XCB,0XAB,0XE6,0XBC,0X87,0X93,0X62,0XAC,0X06,0XD5,0X4D,
0XA3,0XE8,0XFE,0X73,0XFE,0X74,0XCC,0XEE,0XE5,0XB3,0XE5,0X94,0XAB,0XEF,0XBC,0XF4,
0XD5,0XD9,0XD6,0X1A,0XDE,0X5B,0XDE,0X7B,0XE6,0X7B,0XEE,0XDC,0XEE,0XFC,0XEE,0XFB,
0XE6,0X9A,0XDE,0X78,0XDE,0X58,0XCD,0XF5,0XB5,0X52,0XAD,0X11,0XAC,0XF0,0X9C,0X6E,
0X7B,0X4A,0X8B,0X6B,0X8B,0X2C,0X82,0XCB,0X82,0XC9,0X82,0X87,0X7A,0X45,0X82,0X65,
0X82,0X67,0X82,0X87,0XAB,0XCB,0X8A,0XA5,0XA3,0X46,0XBB,0XC9,0XC3,0XEB,0XBB,0XAB,
0XBB,0XCB,0XC4,0X6D,0XB4,0X2D,0X38,0XE1,0X41,0X85,0X7B,0XCE,0X52,0X8A,0X31,0XC8,
0X39,0XE8,0X39,0XA7,0X52,0X29,0X41,0X66,0X51,0XE7,0X49,0X84,0X62,0X25,0XB4,0X8D,
0XCC,0XCC,0XB4,0X08,0X8A,0XE5,0X62,0X02,0X72,0XA7,0X49,0XE4,0X31,0X84,0X31,0XC5,
0X19,0X03,0X19,0X23,0X19,0X23,0X18,0XE3,0X18,0XE4,0X29,0X25,0X31,0X46,0X31,0X66,
0X39,0XC4,0X29,0X82,0X39,0XA2,0X62,0X84,0X93,0XC9,0XBC,0XCD,0XB4,0XAC,0X93,0XA9,
0X49,0X82,0X52,0X25,0X31,0XA4,0X74,0X0F,0X84,0XD3,0X85,0X14,0X95,0X96,0X7D,0X15,
0X95,0XD9,0X53,0XAF,0X32,0XC9,0X64,0X2B,0X95,0XB0,0XAE,0X72,0X85,0X2E,0X42,0XE6,
0X42,0XE7,0X74,0X6C,0X42,0XE4,0X2A,0X40,0X43,0X23,0X43,0X23,0X43,0X43,0X2A,0X80,
0X53,0X84,0X53,0XA4,0X7C,0XC9,0X7D,0X09,0X6C,0XC8,0X85,0X4B,0X85,0X4B,0X74,0XC9,
0X74,0XA9,0X74,0XA9,0X64,0X06,0X85,0X0A,0X75,0X08,0X75,0X27,0X6D,0X05,0X6D,0X45,
0X65,0X05,0X64,0XE5,0X54,0X65,0X3B,0X83,0X54,0X26,0X53,0XE6,0X33,0X02,0X33,0X02,
0X43,0X63,0X7D,0X29,0X74,0XC8,0X5C,0X26,0X5C,0X27,0X5C,0X28,0X3B,0X05,0X3B,0X25,
0X54,0X48,0X6C,0XEA,0X5C,0X88,0X2B,0X02,0X43,0XA4,0X5C,0X67,0X4B,0XC4,0X5C,0X26,
0X53,0XE5,0X43,0X43,0X53,0XA5,0X4B,0X65,0X43,0X04,0X22,0X01,0X32,0X42,0X32,0X43,
0X19,0XA0,0X5B,0XA8,0X42,0XE5,0X11,0X60,0X32,0X63,0X32,0X64,0X84,0XCD,0X22,0X02,
0X31,0XE3,0X5B,0X08,0X42,0X25,0X6B,0X4A,0X41,0XE5,0X52,0XA8,0X84,0X2E,0X94,0XD0,
0X9D,0X32,0XA5,0X73,0X6B,0XAC,0X3A,0X26,0X52,0XC8,0X42,0X26,0X5A,0XA9,0X5A,0XC9,
0X4A,0X46,0X73,0X6B,0XC6,0X36,0X83,0XED,0XA4,0XD0,0X94,0X8F,0XA5,0X52,0X8C,0X6F,
0XAD,0X93,0X7C,0X2D,0X84,0X4E,0XCE,0X77,0XCE,0X36,0X9C,0X90,0XAC,0XD1,0X93,0XCD,
0X93,0X2B,0X8A,0XA8,0XA3,0XCA,0XB4,0X2A,0XC4,0XCA,0XB4,0X68,0X7A,0X82,0X8A,0XE4,
0XFE,0X72,0XFE,0X51,0XAB,0XE7,0XBC,0X6A,0XAC,0X09,0X49,0X20,0X49,0X22,0X49,0X44,
0X39,0X86,0X39,0XA7,0X41,0XC8,0X52,0X49,0X62,0XCC,0X6B,0X2D,0X7B,0X6E,0X83,0XAF,
0XAC,0XD4,0XA4,0XD3,0XAC,0XF4,0XAD,0X35,0XB5,0X35,0XC5,0XD7,0XD6,0X59,0XD6,0X38,
0XEE,0XB9,0XFF,0X1B,0XFE,0XFD,0XFE,0XBC,0XFE,0XBA,0XF6,0X78,0XEE,0X16,0XEE,0X37,
0XD5,0X55,0XD5,0X75,0XB4,0X50,0XBC,0X8F,0XAB,0XEB,0X92,0XC7,0X92,0X88,0X8A,0X47,
0X8A,0X46,0X8A,0X66,0X38,0X80,0X51,0X83,0X7B,0X2A,0X62,0XA8,0X41,0XC6,0X52,0X27,
0X41,0X85,0X51,0XC6,0X51,0XA5,0X49,0X64,0X59,0XA4,0X82,0XE8,0XB4,0XAD,0X6A,0X23,
0XBC,0X8B,0X8A,0XE4,0X7A,0XA4,0X7A,0XC5,0X62,0X45,0X39,0X42,0X29,0X02,0X18,0XC0,
0X21,0X22,0X21,0X43,0X29,0X43,0X29,0X23,0X31,0X43,0X39,0XA5,0X49,0XE7,0X4A,0X06,
0X6B,0X49,0X7B,0XCA,0X9C,0X6C,0XA4,0X6B,0X9C,0X0A,0XA4,0X2A,0XA4,0X2B,0X93,0XCA,
0X8B,0XCB,0X94,0X4E,0X63,0X0B,0X84,0X91,0X95,0X14,0X8D,0X15,0X7C,0XB3,0X7C,0XB4,
0X84,0XF5,0X4B,0X6E,0X3A,0XE9,0X53,0XEA,0XC7,0X15,0XDF,0XF8,0XD7,0XB8,0XB6,0X73,
0X42,0XE6,0X85,0X0E,0X64,0X08,0X53,0XA5,0X5B,0XC5,0X32,0XA0,0X7C,0XC9,0X6C,0X47,
0X53,0X84,0X5B,0XE5,0X74,0X87,0X64,0X67,0X6C,0XA7,0X7D,0X2A,0X7D,0X0A,0X7C,0XEA,
0XBE,0XB2,0XA5,0XEF,0X84,0XEB,0X7C,0XEA,0X74,0XA7,0X7D,0X28,0X75,0X47,0X6D,0X26,
0X6D,0X47,0X7D,0X89,0X7D,0XAB,0X43,0XA4,0X33,0X03,0X22,0X81,0X2A,0XA2,0X3B,0X23,
0X2A,0X80,0X5C,0X26,0X64,0X26,0X64,0X47,0X5C,0X06,0X43,0X65,0X32,0XE5,0X64,0X6B,
0X1A,0X41,0X5C,0X28,0X43,0XA5,0X4B,0XE6,0X4B,0XA5,0X43,0X43,0X7D,0X0A,0X6C,0X88,
0X74,0XA9,0X5B,0XE6,0X6C,0X69,0X53,0X86,0X53,0X66,0X2A,0X42,0X3A,0XA4,0X2A,0X02,
0X5B,0XA9,0X63,0XEA,0X9D,0XB2,0X95,0X70,0X8D,0X30,0X11,0X81,0X21,0XE3,0X74,0X6D,
0X42,0X25,0X4A,0X66,0X31,0XA3,0X4A,0X67,0X52,0XA8,0X5A,0XC9,0X9C,0XF1,0X9D,0X52,
0XB6,0X36,0XC6,0XB8,0X7C,0X6F,0X3A,0X67,0X3A,0X47,0X10,0XA0,0X10,0XA1,0X10,0X60,
0X10,0X80,0X31,0XA4,0XBD,0XD4,0X84,0X0D,0XAD,0X32,0X83,0XED,0XA5,0X31,0X84,0X2E,
0X94,0XCF,0X7C,0X0C,0XC6,0X35,0XCE,0X55,0XAD,0X11,0XEE,0XD8,0XE6,0X97,0X8B,0X4C,
0XAB,0XB0,0XBC,0X12,0XA3,0X8D,0X9B,0X8C,0X9B,0X6A,0X6A,0X03,0X82,0XA6,0XDD,0X70,
0XFF,0X55,0XB4,0X27,0XBC,0X66,0XDD,0XA9,0XAC,0X65,0X8B,0X42,0X8B,0X44,0X7B,0X04,
0X62,0XA3,0X52,0X42,0X41,0XC1,0X41,0XA1,0X39,0X61,0X39,0X21,0X30,0XE1,0X30,0XC2,
0X41,0X45,0X41,0X25,0X41,0X26,0X41,0X07,0X38,0XE7,0X41,0X49,0X49,0X8A,0X49,0X68,
0X49,0X64,0X62,0X26,0X72,0X69,0X7A,0XCB,0X8B,0X4C,0X9B,0XAD,0X9B,0XEE,0XA4,0X4F,
0XC5,0X55,0XCD,0X76,0XB4,0XB1,0XD5,0X94,0XF6,0X75,0XF5,0XF3,0XFE,0X97,0XFE,0XB8,
0XFE,0X34,0X92,0XA5,0X82,0X64,0X69,0XE2,0X8B,0X48,0X62,0X44,0X5A,0X03,0X59,0XE3,
0X51,0X82,0X59,0XC2,0X61,0XA2,0X6A,0X02,0X82,0XC5,0XB4,0X2A,0XDD,0XAF,0X61,0XE0,
0X9B,0X67,0X59,0X80,0X6A,0X43,0X59,0XE2,0X7B,0X08,0X30,0XE0,0X49,0XA3,0X39,0X82,
0X41,0XC2,0X4A,0X22,0X5A,0XA4,0X6B,0X46,0X83,0XE8,0X9C,0X8B,0XA4,0XEC,0XAC,0XED,
0XAD,0X2E,0XA4,0XCC,0XA4,0XAC,0XAC,0XAC,0X9C,0X2A,0X93,0XA8,0X8B,0X88,0X7B,0X49,
0X7B,0X8B,0X84,0X0E,0X5B,0X2C,0X74,0X10,0X9D,0X76,0X8C,0XF4,0X42,0XAC,0X42,0XED,
0XCF,0X3E,0XCF,0X5D,0X9D,0XF4,0XA6,0X53,0XBF,0X14,0XBE,0XF3,0XA6,0X31,0XDF,0XB8,
0X84,0XED,0XA5,0XF1,0X53,0X65,0X3A,0XC2,0X8D,0X6B,0X53,0X83,0X7C,0XC9,0X53,0XA4,
0X43,0X21,0X53,0XC4,0X4B,0X83,0X43,0X42,0X6C,0XA7,0X7D,0X4A,0X7C,0XE9,0X9D,0XCE,
0XD7,0X55,0XC6,0XD4,0XBE,0X72,0X8D,0X0C,0X7C,0XC9,0X74,0XE8,0X7D,0X28,0X64,0XA6,
0X75,0X28,0X5C,0X66,0X6C,0XC9,0X3B,0X23,0X22,0X41,0X2A,0X62,0X32,0XC3,0X19,0XE0,
0X43,0X23,0X43,0X43,0X22,0X40,0X22,0X40,0X2A,0X81,0X32,0XA2,0X22,0X42,0X53,0X88,
0X53,0XE8,0X11,0XA0,0X2A,0XA2,0X43,0X65,0X22,0X61,0X2A,0X81,0X3A,0XE2,0X3A,0XE3,
0X5C,0X07,0X4B,0X65,0X2A,0X41,0X32,0X83,0X22,0X02,0X53,0X88,0X4B,0X27,0X32,0X84,
0X84,0XCF,0X53,0X69,0X32,0X65,0X64,0X0C,0X6C,0X0C,0X19,0XA3,0X63,0XCB,0X7C,0XAE,
0X4A,0XA7,0X5B,0X29,0X6B,0X8B,0X5B,0X09,0X4A,0X67,0X21,0X02,0X94,0XB0,0XBE,0X36,
0XC6,0XB9,0XCE,0XFA,0X6B,0XED,0X3A,0X47,0X42,0X88,0X19,0X22,0X18,0XE2,0X10,0XA1,
0X08,0X40,0X31,0XA4,0XDE,0XB8,0X9C,0XB0,0XB5,0X73,0X73,0X8B,0XB5,0X93,0X73,0XAB,
0XAD,0X72,0X94,0XEF,0X7C,0X0C,0X73,0X89,0X94,0X2C,0X9C,0X4D,0X72,0XC7,0XF6,0X55,
0XED,0XB4,0XDD,0X32,0XFE,0X97,0XED,0XF4,0XEE,0X35,0XD5,0X73,0XA3,0X6C,0XFE,0XF8,
0XD4,0XCD,0XC4,0X69,0XCD,0X08,0XBC,0XA5,0XBC,0XE6,0XD5,0XAA,0XBD,0X0A,0XCD,0X8D,
0XCD,0XAE,0XC5,0X6C,0XBD,0X0B,0XBC,0XCB,0XB4,0XAB,0XAC,0X4A,0XAC,0X0A,0XAB,0XEB,
0X82,0XC7,0X82,0X87,0X7A,0X87,0X7A,0X47,0X69,0XE7,0X61,0XC6,0X61,0XA6,0X51,0X43,
0X49,0X40,0X51,0XA1,0X51,0X83,0X49,0X43,0X49,0X83,0X49,0XA3,0X49,0XC3,0X4A,0X05,
0X41,0X85,0X31,0X24,0X5A,0X46,0X39,0X20,0X8B,0X68,0XA3,0XA9,0XBC,0X0B,0XCC,0X6D,
0XA3,0X07,0X79,0XE2,0X59,0X40,0X93,0X45,0XCD,0X0C,0XAC,0X49,0XDD,0XAF,0XCD,0X0D,
0XE5,0X8F,0XCC,0XCB,0XD4,0XEB,0XC4,0X68,0XC4,0X88,0XB3,0XE6,0X9B,0X64,0X6A,0X00,
0X6A,0X21,0X59,0XC1,0X49,0X60,0X49,0X81,0X93,0XCB,0X6A,0XA7,0X6A,0XA6,0X8B,0XCA,
0X9C,0X2B,0X9C,0X6B,0XA4,0XAB,0XAC,0XEB,0XAD,0X0C,0XAC,0XEB,0X9C,0X8A,0X8C,0X08,
0X9C,0XAB,0X73,0X26,0X5A,0X42,0X62,0X63,0X5A,0X02,0X49,0XA0,0X41,0X81,0X41,0XA2,
0X21,0X01,0X21,0X43,0X19,0X24,0X32,0X28,0X8C,0XD4,0X84,0X93,0X3A,0X6A,0X42,0XAB,
0XA5,0XF8,0XEF,0XFF,0XD7,0XDA,0X8D,0X6E,0X74,0XAA,0X5B,0XE7,0X5B,0XC7,0X8D,0X0D,
0X63,0XE9,0X9D,0X8F,0X8D,0X2C,0X74,0X68,0X8D,0X4A,0X53,0XA3,0X8D,0X6B,0X64,0X26,
0X3B,0X00,0X4B,0X82,0X3B,0X00,0X32,0XE0,0X6C,0XA7,0X74,0XE9,0X74,0X88,0XB6,0X91,
0XF7,0XF9,0XF7,0XF9,0XE7,0X97,0X8D,0X0C,0X74,0X49,0X6C,0X47,0X74,0XC8,0X74,0XC8,
0X64,0XA7,0X75,0X09,0X64,0XA9,0X32,0XC3,0X2A,0X83,0X2A,0X63,0X2A,0X42,0X3A,0XC3,
0X5B,0XC6,0X5B,0XE6,0X32,0X80,0X11,0XC0,0X09,0X60,0X2A,0X41,0X32,0X84,0X4B,0X67,
0X2A,0X42,0X4B,0X46,0X95,0X8F,0X5C,0X08,0X2A,0X62,0X22,0X00,0X19,0XC0,0X7C,0XEC,
0XB6,0XB3,0XA6,0X31,0X7C,0XCC,0X6C,0X2A,0X5B,0XC9,0X7C,0XCD,0X6C,0X2B,0X4B,0X07,
0X74,0X4D,0X53,0X8A,0X2A,0X24,0X7C,0XAF,0X7C,0X8F,0X09,0X21,0X3A,0X86,0X8D,0X10,
0X3A,0X46,0X21,0X83,0X29,0XA4,0X31,0XC4,0X42,0X47,0X29,0X63,0X9D,0X33,0XBE,0X57,
0XD7,0X1A,0XD7,0X5B,0X5B,0X8C,0X3A,0X88,0X42,0XA9,0X21,0X64,0X10,0XC2,0X10,0XA1,
0X18,0XE2,0X31,0X84,0XCE,0X57,0X84,0X0E,0X94,0X70,0X52,0X67,0XCE,0X36,0X8C,0X2D,
0XBD,0XF4,0X9C,0XEF,0X73,0X69,0XC5,0XD2,0X62,0X85,0X72,0XE7,0XFE,0XD6,0XE5,0XF0,
0X93,0X00,0XBC,0X64,0XA4,0X04,0X5A,0X00,0X41,0X00,0X62,0X04,0XAB,0XAE,0XD4,0XB2,
0XC4,0X2F,0XC4,0X4C,0XCC,0XAB,0XDD,0XAD,0XCD,0X4C,0XAC,0X6A,0XBC,0XAE,0XC4,0XAF,
0X93,0X27,0XA3,0X88,0XB3,0XE9,0XC4,0X4A,0XCC,0X6A,0XCC,0X8A,0XD4,0X89,0XD4,0X89,
0XED,0X4B,0XE5,0X29,0XE5,0X49,0XE5,0X49,0XDD,0X07,0XDD,0X27,0XD5,0X07,0XC4,0XA5,
0XB4,0XA6,0XB4,0XC9,0XAC,0X49,0X8B,0X68,0X83,0X46,0X73,0X05,0X62,0XC4,0X52,0X84,
0X42,0X04,0X41,0XE4,0X39,0XA2,0X73,0X05,0XC5,0X4C,0X72,0X81,0XBC,0X29,0XCC,0X8C,
0X79,0XE2,0X69,0X80,0X8A,0XA3,0XB4,0X27,0XC4,0XA9,0XC4,0XC9,0X93,0X23,0XBC,0X48,
0XC4,0X48,0XD4,0XA8,0XDD,0X09,0XBB,0XE4,0XB3,0XC3,0XC4,0X45,0XB4,0X26,0X61,0XA0,
0X49,0X80,0X72,0XE4,0X5A,0X23,0X7A,0XE6,0X9B,0XEC,0XAC,0X6E,0X8B,0X6A,0XB4,0X8F,
0X93,0XAB,0X93,0XAA,0X8B,0X69,0X83,0X28,0X72,0XE7,0X6A,0X86,0X5A,0X04,0X49,0XA2,
0X30,0XE0,0X41,0X80,0X49,0XA0,0X31,0X00,0X30,0XC0,0X41,0X60,0X39,0X40,0X08,0X40,
0X18,0XA0,0X08,0X81,0X08,0XC2,0X21,0XA6,0X6B,0XEF,0X74,0X10,0X84,0X92,0X84,0XB2,
0X95,0X55,0XD7,0X7C,0XE7,0XFB,0X6C,0X4A,0X64,0X07,0X4B,0X23,0X5B,0XC7,0X4A,0XE4,
0X4B,0X26,0X74,0X4A,0X84,0XCA,0X7C,0XC9,0X74,0X87,0X6C,0X25,0X84,0XE9,0X5B,0XE4,
0X43,0X41,0X4B,0X61,0X4B,0X82,0X5C,0X04,0X7D,0X29,0X7D,0X09,0X85,0X2B,0XDF,0XB6,
0XF7,0XFA,0XFF,0XFA,0XEF,0XB8,0XAD,0XD0,0X9D,0X6E,0XA5,0XCE,0X95,0X6C,0X95,0XCD,
0X6C,0XC9,0X53,0XE6,0X43,0X65,0X4B,0XA7,0X6C,0X6B,0X9D,0XD1,0XA6,0X32,0XBE,0XD4,
0XCF,0X34,0X6C,0X68,0X2A,0X20,0X43,0X03,0X4B,0X24,0X42,0XE4,0X2A,0X43,0X32,0X84,
0X63,0XC9,0XF7,0XFB,0XBE,0XB4,0X6C,0X4A,0X4B,0X26,0X2A,0X21,0X4B,0X25,0X43,0X05,
0XA6,0X11,0X9D,0XF0,0X95,0X6F,0X74,0X8C,0X8D,0X2F,0X8D,0X2F,0X84,0XEF,0X6C,0X2C,
0X3A,0XA7,0X2A,0X05,0X11,0X42,0X32,0X45,0X74,0X6E,0X53,0X6A,0X53,0X4A,0XCF,0X39,
0X9D,0X31,0X63,0X8B,0X52,0XE9,0X52,0XC8,0X52,0XC9,0X31,0XA5,0XA5,0X54,0XBE,0X58,
0XBE,0X78,0XCF,0X1A,0X5B,0X6C,0X4B,0X0A,0X53,0X0A,0X29,0XA5,0X10,0XC2,0X18,0XE2,
0X21,0X23,0X21,0X43,0XCE,0X57,0X9C,0XB1,0XA4,0XF2,0X41,0XC5,0XCE,0X16,0X7B,0XEC,
0XAD,0X72,0X7B,0XCA,0XB5,0X91,0XEF,0X17,0X83,0X89,0XDE,0X13,0XE5,0XF2,0XB4,0X48,
0XB4,0X20,0XD5,0X42,0XC5,0X24,0XAC,0XA5,0X9C,0X48,0X8B,0X28,0X72,0X08,0X71,0XC8,
0X71,0X86,0X92,0X88,0X8A,0XC5,0X82,0XA3,0X8B,0X26,0X93,0X68,0X9B,0X8C,0XAB,0XEE,
0XAB,0X6A,0XCC,0X2C,0XE4,0XEE,0XE5,0X0E,0XE4,0XCC,0XDC,0X8A,0XDC,0X67,0XD4,0X45,
0XCC,0X03,0XCC,0X01,0XCC,0X20,0XCC,0X60,0XD4,0X60,0XDC,0XE0,0XDD,0X00,0XCC,0XE1,
0XBC,0XC3,0XC5,0X48,0XCD,0X4B,0XC5,0X2C,0XCD,0X8E,0XCD,0XCF,0XBD,0XAE,0XAD,0X6E,
0XAD,0X70,0XAD,0X91,0XBD,0XB0,0XBD,0X8E,0XCD,0XAC,0X93,0XA4,0X7A,0X60,0X8A,0XA3,
0X7A,0X02,0X50,0XC0,0XB3,0XE7,0XB4,0X06,0XB4,0X05,0XBC,0X46,0XD5,0X09,0XD4,0XC8,
0XE5,0X2A,0XDC,0XC7,0XD4,0X86,0XED,0X68,0XD4,0XA5,0XD4,0XC6,0XFE,0X2D,0X61,0XC0,
0X72,0XC2,0X94,0X09,0XB5,0X0D,0XA4,0X4C,0X9C,0X0C,0X8B,0X6A,0X82,0XE9,0X6A,0X06,
0X6A,0X06,0X61,0XE5,0X51,0X83,0X41,0X01,0X38,0XC1,0X38,0XC1,0X38,0XE2,0X38,0XE1,
0X39,0X20,0X28,0XC0,0X31,0X00,0X39,0X40,0X31,0X00,0X28,0XC0,0X29,0X00,0X31,0X42,
0X19,0X02,0X08,0XC1,0X11,0X03,0X21,0X86,0X3A,0X49,0X29,0XE7,0X8C,0XD2,0X8C,0XD2,
0X8D,0X14,0X85,0X12,0X85,0X0F,0X6C,0X6A,0X5B,0XC5,0X53,0XA5,0X53,0X65,0X3A,0XA3,
0X63,0XA8,0X9D,0X6E,0X74,0X48,0X63,0XC5,0X63,0XE4,0X53,0X82,0X3A,0XA0,0X6C,0X46,
0X6C,0X86,0X64,0X24,0X74,0XC6,0X85,0X49,0X7D,0X49,0X6C,0X87,0X85,0X2B,0XDF,0XB6,
0XE7,0XB8,0XF7,0XFA,0XF7,0XFA,0XF7,0XD9,0XF7,0XF9,0XEF,0XF8,0X9D,0XAE,0X8D,0X6C,
0X74,0XCA,0X2A,0XA1,0XA6,0X31,0XEF,0XFB,0X74,0X8C,0X8D,0X70,0XCF,0X57,0X9D,0XD0,
0XB6,0X72,0X7C,0XCA,0X53,0X85,0X63,0XE6,0X4B,0X45,0X4B,0X25,0X3A,0XC5,0X32,0X84,
0XC6,0XD6,0X6C,0X0A,0X8D,0X0E,0X6C,0X2A,0X32,0X84,0X43,0X05,0X4B,0X26,0X7C,0XAC,
0X84,0XED,0X7C,0XAC,0X42,0XE5,0X53,0X88,0X63,0XEA,0X8D,0X50,0X6C,0X2C,0X64,0X2C,
0X53,0X6A,0X4B,0X29,0X4B,0X29,0X2A,0X25,0X42,0XE8,0X42,0XE8,0X4B,0X29,0X7C,0XAF,



};
//=====  end ==========================================
