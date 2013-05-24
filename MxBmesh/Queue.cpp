// Do not remove the include below
#include "Queue.h"

QUEUE::QUEUE() {
	// TODO Auto-generated constructor stub
	front=rear=0;
}
void QUEUE::init_queue()
{
	front=rear=0;

}
byte QUEUE::peerqueue()
{
	byte t=RFQUENEMAX;
	if(front==rear)//�����ǿ�
	{
		return RFQUENEMAX;
	}
	t=front;

	return t;
}
byte QUEUE::inqueue()
{
	byte t=RFQUENEMAX;
	if((rear+1)%RFQUENEMAX==front)//������
	{
		return RFQUENEMAX ;
	}
	else//����
	{
		t=rear;
		rear=(rear+1)%RFQUENEMAX;

	}

	return t;
}
byte QUEUE::dequeue()
{
	byte t=RFQUENEMAX;
	if(front==rear)//�����ǿ�
	{
		return RFQUENEMAX;
	}
	t=front;
	front=(front+1) % RFQUENEMAX;

	return t;
}
void QUEUE::undodequeue() //used for no ack get
{

	front=(front+RFQUENEMAX-1) % RFQUENEMAX;

	return ;
}
