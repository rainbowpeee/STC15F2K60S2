#include <string.h>

//////////////////////////////////
//�˿ڶ���
/////////////////////////////////////////////////////////////////////
//MFRC522
sbit     MF522_RST  =    P1^4;//4                   //RC500Ƭѡ
sbit     MF522_NSS  =    P1^0;//2
sbit     MF522_SCK  =    P1^1;//1
sbit     MF522_SI   =    P1^2;//0
sbit		 MF522_SO   =    P1^3;//3

//����
sbit     KEY1  =    P3^2;
sbit     KEY2  =    P3^3;
sbit     KEY3  =    P3^4;
sbit	 KEY4  =    P3^5;

#define Menu 0x00
#define Rfid_Button1 0x0e
#define Rfid_Button2 0x0d
#define Rfid_Button3 0x0b
#define Rfid_Button4 0x07
  

//��
/////////////////////////////////////////////////////////////////////
//����ԭ��
/////////////////////////////////////////////////////////////////////
void InitializeSystem();  

/*************��������****************/

#define MAIN_Fosc                   11059200L       //��ʱ��,����Ƶ�ʲ�ͬ��ֱ���������޸�Ƶ��
#define serial_one_read_max         32              //���建�����ĳ���
#define serial_one_baud_rate        9600L           //������,ֻ���������޸Ĳ�����
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //�ö�ʱ��1�������ʷ����������㶨ʱ��1����װֵ    
#define u8 unsigned char