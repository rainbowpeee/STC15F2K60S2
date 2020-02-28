#include <STC15F2K60S2.H>
#include "main.h"
#include "LCD12864.h"
#include "mfrc522.h"	

unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 		 //��Ƭ��Կ  
unsigned char idata MLastSelectedSnr[4]; 										//��ƬUID���Cache
unsigned char idata RevBuffer[16];  											//��ȡ���ݴ��
unsigned char data SerBuffer[16];												//�������ݴ��

struct User{				  //�����û�����
	int Apparel_Orders;
	int Price;
	int Inventory;
	int Shipments;
}User_Data;


void InitializeSystem()				 //��ʼ������
{
	P0 = 0xFF; P1 = 0xFF; P3 = 0xFF; 

			
	PcdReset();		  //RC522��ʼ��
	PcdAntennaOff(); //�ر�����
	PcdAntennaOn();  //������
	M500PcdConfigISOType( 'A' ); //���ÿ�����ΪA
	delay_1ms(10);	
	
}


void LCD12864_Display_num(unsigned char x,unsigned char y,unsigned int z){		 //LCD��ָ��λ����ʾ��λ����

	unsigned char display_code[5];

	display_code[0] = z/1000+48;
	display_code[1] = z/100%10+48;
	display_code[2] = z/10%10+48;
	display_code[3] = z%10+48;
	display_code[4] = '\0';
	lcd_pos(x,y);		
	write_str(display_code);
}


void Rx_Data_Parsing(unsigned char *Rx_Data){		 //�������ݽ���
	User_Data.Apparel_Orders = Rx_Data[0];
	User_Data.Apparel_Orders = User_Data.Apparel_Orders <<8;
	User_Data.Apparel_Orders |= Rx_Data[1];

	User_Data.Price = Rx_Data[2];
	User_Data.Price = User_Data.Price <<8;
	User_Data.Price |= Rx_Data[3];

	User_Data.Inventory = Rx_Data[4];
	User_Data.Inventory = User_Data.Inventory <<8;
	User_Data.Inventory |= Rx_Data[5];

	User_Data.Shipments = Rx_Data[6];
	User_Data.Shipments = User_Data.Shipments <<8;
	User_Data.Shipments |= Rx_Data[7];
}

void Tx_Data_Parsing(){		   //�������ݽ���

	SerBuffer[0] =   (User_Data.Apparel_Orders >>8) & 0x00ff;
	SerBuffer[1] =   User_Data.Apparel_Orders & 0x00ff;

	SerBuffer[2] =   (User_Data.Price >>8) & 0x00ff;
	SerBuffer[3] =   User_Data.Price & 0x00ff;

	SerBuffer[4] =   (User_Data.Inventory >>8) & 0x00ff;
	SerBuffer[5] =   User_Data.Inventory & 0x00ff;

	SerBuffer[6] =   (User_Data.Shipments >>8) & 0x00ff;
	SerBuffer[7] =   User_Data.Shipments & 0x00ff;

}

unsigned char key_scan(){			  //��ȡ������ֵ 
	unsigned char key_value = 0x00;
	key_value = key_value | KEY4;
	key_value = key_value << 1;

	key_value = key_value | KEY3;
	key_value = key_value << 1;

	key_value = key_value | KEY2;
	key_value = key_value << 1;

	key_value = key_value | KEY1;

	return key_value;
}
void main()
{   
	
	unsigned char count_for = 0;
	unsigned char key_state = 1;
	bit set_flag = 0;
    InitializeSystem( );
	lcd_init(); 
	lcd_pos(0,2);				//ָ����ʾλ��
	write_str("��ӭʹ��");	   	//��ʾ����
	lcd_pos(1,0);			
	write_str("��װ�ִ�����ϵͳ");
	lcd_pos(3,0);			
	write_str("¼��");
	lcd_pos(3,6);				
	write_str("��ȡ");
	User_Data.Apparel_Orders = 0;
	User_Data.Price	= 0;
	User_Data.Inventory = 0;
	User_Data.Shipments = 0; 

    while(1)
    {	
		switch(key_scan()){

		 	case Rfid_Button1 : 
				while(1){
					lcd_init(); 
					lcd_pos(0,0);			
					write_str("��װ��Ŀ");
					lcd_pos(1,0);			
					write_str("����:   ");
					lcd_pos(2,0);			
					write_str("���:   ");
					lcd_pos(3,0);		
					write_str("������: ");
					
					while(1){
						delay_1ms(1000);			   //����������Ż���ʾˢ��
						set_flag = ~set_flag;		   //���ñ�ʶ�ж�λ
						if(Rfid_Button2 == key_scan())	   //�������ý���д
						{
							break;
						}
						LCD12864_Display_num(0,5,User_Data.Apparel_Orders);
						LCD12864_Display_num(1,5,User_Data.Price);
						LCD12864_Display_num(2,5,User_Data.Inventory);
						LCD12864_Display_num(3,5,User_Data.Shipments);

						switch(key_state){
							case 1 :if(Rfid_Button3 == key_scan())			  //�жϼӼ�
										User_Data.Apparel_Orders++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Apparel_Orders--;
									if(User_Data.Apparel_Orders < 0)		   //����Խ���ж�
										User_Data.Apparel_Orders = 0;
									else if(User_Data.Apparel_Orders >9999)
										User_Data.Apparel_Orders =9999;
									if(set_flag){					//��ʾ���ñ�־
										lcd_pos(0,4);				//ָ����ʾλ��
										write_str(":_");
									}else{							//������ñ�־
										lcd_pos(0,4);				//ָ����ʾλ��
										write_str(": ");
									}
									lcd_pos(1,4);				//�������λ���ñ�־
									write_str("  ");
									lcd_pos(2,4);				
									write_str("  ");
									lcd_pos(3,4);			
									write_str("  ");
									LCD12864_Display_num(0,5,User_Data.Apparel_Orders);
						  		break;
							case 2 :if(Rfid_Button3 == key_scan())				//�жϼӼ�
										User_Data.Price++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Price--;
									if(User_Data.Price < 0)						//����Խ���ж�
										User_Data.Price = 0;
									else if(User_Data.Price >9999)
										User_Data.Price =9999;
									if(set_flag){
										lcd_pos(1,4);			
										write_str(" _");
									}else{
										lcd_pos(1,4);		
										write_str("  ");
									}
									lcd_pos(0,4);				//�������λ���ñ�־
									write_str(": ");
									lcd_pos(2,4);			
									write_str("  ");
									lcd_pos(3,4);			
									write_str("  ");
									LCD12864_Display_num(1,5,User_Data.Price);
						  		break;
							case 3 :if(Rfid_Button3 == key_scan())			 //�жϼӼ�
										User_Data.Inventory++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Inventory--;
									if(User_Data.Inventory < 0)				//����Խ���ж�
										User_Data.Inventory = 0;
									else if(User_Data.Inventory >9999)
										User_Data.Inventory =9999;
									if(set_flag){
										lcd_pos(2,4);			
										write_str(" _");
									}else{
										lcd_pos(2,4);	
										write_str("  ");
									}
									lcd_pos(0,4);				//�������λ���ñ�־
									write_str(": ");
									lcd_pos(1,4);			
									write_str("  ");
									lcd_pos(3,4);			
									write_str("  ");
									LCD12864_Display_num(2,5,User_Data.Inventory);
						  		break;
							case 4 :if(Rfid_Button3 == key_scan())	  //�жϼӼ�
										User_Data.Shipments++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Shipments--;
									if(User_Data.Shipments < 0)
										User_Data.Shipments = 0;
									else if(User_Data.Shipments >9999)
										User_Data.Shipments =9999;
									if(set_flag){
										lcd_pos(3,4);		
										write_str(" _");
									}else{
										lcd_pos(3,4);		
										write_str("  ");
									}
									lcd_pos(0,4);			//�������λ���ñ�־
									write_str(": ");
									lcd_pos(1,4);		
									write_str("  ");
									lcd_pos(2,4);			
									write_str("  ");
									LCD12864_Display_num(3,5,User_Data.Shipments);
							  	break;
							default : break;
							}
							if(Rfid_Button1 == key_scan())	   //��������
							{
								key_state = key_state % 4;
								key_state++;
							}		
						}
						
						lcd_init(); 	
						lcd_pos(0,0);			
						write_str("�뽫��Ƭ�����ڶ�");
						lcd_pos(1,0);			
						write_str("������");
						lcd_pos(2,0);			
						write_str("׼��д��....");
	
//						while(0 != PcdHalt())��
						while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));	    //��ȡ��Ƭ����	
				 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));			   // ��ȡ��ƬUID
						while(0 != PcdSelect(MLastSelectedSnr));				  //ѡ�п�Ƭ
						while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	   //������Կ
						Tx_Data_Parsing();	//����Ҫ���͵�����
						while(0 != PcdWrite(4,&SerBuffer));							 //ָ��λ��д������

		   				lcd_init(); 			//��ʼ�˵�
						lcd_pos(0,0);	
						write_str("д��ɹ�");
						delay_1ms(15000);
						lcd_init(); 
						lcd_pos(0,2);		
						write_str("��ӭʹ��");
						lcd_pos(1,0);		
						write_str("��װ�ִ�����ϵͳ");
						lcd_pos(3,0);			
						write_str("¼��");
						lcd_pos(3,6);		
						write_str("��ȡ");
						break;
					}
				break;

			case Rfid_Button2 : 
		   			lcd_init(); 	
					lcd_pos(0,0);			
					write_str("�뽫��Ƭ�����ڶ�");
					lcd_pos(1,0);			
					write_str("������");
					lcd_pos(2,0);			
					write_str("׼����ȡ....");

//					while(0 != PcdHalt());
					while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));		 //��ȡ��Ƭ����
			 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));				 //��ȡ��ƬUID
					while(0 != PcdSelect(MLastSelectedSnr));						//ѡ�п�Ƭ
					while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	//������Կ
					while(0 != PcdRead(4,&RevBuffer));							//��ȡָ��λ�õ�����
					lcd_init(); 	
					lcd_pos(0,0);				//ָ����ʾλ��
					write_str("�����ɹ���");
					delay_1ms(5000);
					Rx_Data_Parsing(&RevBuffer);
					lcd_init(); 
					lcd_pos(0,0);			
					write_str("��װ��Ŀ :");
					lcd_pos(1,0);			
					write_str("����     :");
					lcd_pos(2,0);			
					write_str("���     :");
					lcd_pos(3,0);			
					write_str("������   :");
					LCD12864_Display_num(0,5,User_Data.Apparel_Orders);		//��ʾ��ȡ����
					LCD12864_Display_num(1,5,User_Data.Price);
					LCD12864_Display_num(2,5,User_Data.Inventory);
					LCD12864_Display_num(3,5,User_Data.Shipments);
					delay_1ms(15000);
					lcd_init(); 					//��ʼ�˵�
					lcd_pos(0,2);				
					write_str("��ӭʹ��");
					lcd_pos(1,0);			
					write_str("��װ�ִ�����ϵͳ");
					lcd_pos(3,0);			
					write_str("¼��");
					lcd_pos(3,6);			
					write_str("��ȡ");
					break;

			default : break;
		}
	}

}