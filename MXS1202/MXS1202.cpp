#include "MXS1202.h"
#include "Wire.h"
#include <util/delay.h>
#define CONTROLCHIPADDRESS  0x27
#if	 defined(isant2400cc) || defined(mx231cc)
#define ONE_WIRE_BUS2 13 //PD5
#elif defined(isant900cb)
#define ONE_WIRE_BUS2 31 //PB7
#elif defined(iduinorfa1)
#define ONE_WIRE_BUS2 23 //PD5
#endif

#define  COM_OUT pinMode(ONE_WIRE_BUS2,OUTPUT)
#define  COM_CLR digitalWrite(ONE_WIRE_BUS2,LOW)
#define  COM_IN pinMode(ONE_WIRE_BUS2,INPUT)
#define  COM_SET digitalWrite(ONE_WIRE_BUS2,HIGH)
#define  COM_R digitalRead(ONE_WIRE_BUS2)

uint16_t    MXS1202::U8FLAG=0,MXS1202::k=0;
uint8_t    MXS1202::U8count=0,MXS1202::U8temp=0;
uint8_t    MXS1202::U8T_data_H=0,MXS1202::U8T_data_L=0,MXS1202::U8RH_data_H=0,MXS1202::U8RH_data_L=0,MXS1202::U8checkdata=0;
uint8_t    MXS1202::U8comdata=0;

void  MXS1202::COM(void)
{

	uint8_t i;
	for(i=0;i<8;i++)
	{

		U8FLAG=2;
		while((!(COM_R))&&U8FLAG++);
		if(U8FLAG==1)break;
		_delay_us(20);
		U8temp=0;
		if(COM_R)U8temp=1;
		U8FLAG=2;
		while((COM_R)&&U8FLAG++);
		//��ʱ������forѭ��
		if(U8FLAG==1)break;
		//�ж�����λ��0����1

		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1

		U8comdata<<=1;
		U8comdata|=U8temp;        //0
	}//rof

}
MXS1202::MXS1202()
{

}
void MXS1202::StartSensor()
{

	Wire.begin(); // set up SEABO I2C support
	Wire.beginTransmission(CONTROLCHIPADDRESS); // join I2C
	Wire.write(0x03);
	Wire.write((byte) 0x00);
	Wire.endTransmission(); // leave I2C bus
	Wire.beginTransmission(CONTROLCHIPADDRESS); // join I2C
	Wire.write(0x01);
	Wire.write((byte) 0x80); //0x80 enable5V  0x8 disable UART1
	Wire.endTransmission(); // leave I2C bus
#if	 defined(isant2400cc) || defined(mx231cc)
	pinMode(28, OUTPUT); //A4
	digitalWrite(28, HIGH);
#elif	 defined(isant900cb)
#endif




}
void MXS1202::StopSensor()
{
	Wire.begin(); // set up SEABO I2C support
	Wire.beginTransmission(CONTROLCHIPADDRESS); // join I2C
	Wire.write(0x03);
	Wire.write((byte) 0x00);
	Wire.endTransmission(); // leave I2C bus
	Wire.beginTransmission(CONTROLCHIPADDRESS); // join I2C
	Wire.write(0x01);
	Wire.write((byte) 0x00); //disable5V
	Wire.endTransmission(); // leave I2C bus

}

int MXS1202::getTempCandHumidity(float *tempc,float *humidity)
{
	uint8_t    U8T_data_H_temp=0,U8T_data_L_temp=0,U8RH_data_H_temp=0,U8RH_data_L_temp=0,U8checkdata_temp=0;

	//��������18ms
	COM_OUT;
	COM_CLR;
	_delay_ms(18);
	COM_IN;
	COM_SET;

	//������������������ ������ʱ20us
	_delay_us(20);
	//������Ϊ���� �жϴӻ���Ӧ�ź�
	//P2_0=1;

	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	// if(!P2_0)		 //T !
	if(COM_R)U8FLAG=1;
	while(!(COM_R))
	{
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
		//while((!P2_0)&&U8FLAG++);
		while((!(COM_R))&&U8FLAG++);
		if(U8FLAG==1)break;
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		//while((P2_0)&&U8FLAG++);
		while((COM_R)&&U8FLAG++);
		if(U8FLAG==1)break;
		//���ݽ���״̬
		COM();
		if(U8FLAG==1)break;
		U8T_data_H_temp=U8comdata;
		COM();
		if(U8FLAG==1)break;
		U8T_data_L_temp=U8comdata;
		COM();
		if(U8FLAG==1)break;
		U8RH_data_H_temp=U8comdata;
		COM();
		if(U8FLAG==1)break;
		U8RH_data_L_temp=U8comdata;
		COM();
		if(U8FLAG==1)break;
		U8checkdata_temp=U8comdata;
		break;
	}//while
	//P2_0=1;
	COM_IN;
	COM_SET;
	//����У��

	U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	COM_IN;
	COM_SET;
	if(U8temp==U8checkdata_temp)
	{
		U8RH_data_H=U8T_data_H_temp;
		U8RH_data_L=U8T_data_L_temp;
		U8T_data_H=U8RH_data_H_temp;
		U8T_data_L=U8RH_data_L_temp;
		U8checkdata=U8checkdata_temp;
		*tempc=  (float)((U8T_data_H & 0x7f)*256+U8T_data_L)/10;
		if (U8T_data_H & 0x80) *tempc=0-*tempc;
		*humidity= ((float)(U8RH_data_H*256+U8RH_data_L))/10;
		return 1;

	}//fi
	else
		return 0;



}
