#include "Ano_Link.h"

float Motor_P,Motor_I,Motor_D;	//roll�����ϵ�pid����
float Steer_P,Steer_I,Steer_D;	//pitch�����ϵ�pid����
float yaw_p,yaw_i,yaw_d;
/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ����������ǣ�����int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з��� 
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;					//��Ҫ�������ݵı�־
uint8 data_to_send[50];	////�������ݻ���
uint8 checkdata_to_send,checksum_to_send;

/////////////////////////////////////////////////////////////////////////////////////
uint8 Tx1Buffer[256];
uint8 Tx1Counter=0;
uint8 count1=0; 
void usart3_send(unsigned char *DataToSend,uint8 data_num)
{

    uint8 i;
	for(i=0;i<data_num;i++)
	{
		Tx1Buffer[count1++] = *(DataToSend+i);
	}
//        for(int i=0;i<=count1;i++)
//        {
//           uart_putchar(uart3,Tx1Buffer[i]);
//        }

}
//Send_Data
void ANO_DT_Send_Data(uint8 *dataToSend , uint8 length)
{

	usart3_send(data_to_send,length);

}

void ANO_DT_Send_Check(uint8 head, uint8 check_sum)
{	
	uint8 sum = 0;
	uint8 i;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=7;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	

	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	ANO_DT_Send_Data(data_to_send, 7);
    f.send_check = 0;
}


void ANO_DT_Send_SetPoint(float angle_rol, float angle_pit, float angle_yaw, uint32 alt, uint8 fly_model, uint8 armed)
{
	uint8 _cnt=0;
	vuint16 _temp;
	vuint32 _temp2 = alt;
	uint8 i;
	uint8 sum = 0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=12;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
		
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_Actual(uint16 a_x,uint16 a_y,uint16 a_z,uint16 g_x,uint16 g_y,uint16 g_z,uint16 m_x,uint16 m_y,uint16 m_z)
{
	uint8 _cnt=0;
	vuint16 _temp;
	uint8 sum = 0;
	uint8 i;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=18;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
/////////////////////////////////////////
	_temp = 0;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	data_to_send[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
//	ANO_DT_Send_Data(data_to_send, _cnt);
        uart_putbuff (uart2, &data_to_send[0],8);//sizeof(data_to_send)  ���� ��Ӧ ����2
}
void ANO_DT_Send_PID(uint8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	
	uint8 _cnt=0;
	vuint16 _temp;
	uint8 sum = 0;
	uint8 i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = (int)p1_p ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p1_i  ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p1_d  ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p2_p  ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p2_i  ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p2_d ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p3_p  ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p3_i  ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)p3_d ;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}




/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
//��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
//�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������
void ANO_DT_Data_Receive_Prepare(uint8 data)
{
	static uint8 RxBuffer[50];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
//У��ͨ��������ݽ��н�����ʵ����Ӧ����
//�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
uint16 flash_save_en_cnt = 0;
//void ANO_DT_Data_Receive_Anl(uint8 *data_buf,uint8 num)
//{
//	uint8 sum = 0;
//	uint8 i;
//	for( i=0;i<(num-1);i++)
//		sum += *(data_buf+i);
//	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
//	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		///�ж�֡ͷ
//	
//	
//	if(*(data_buf+2)==0X10)								//PID1
//       {
//    
//			Steer_Kp= ( (vuint16)(*(data_buf+4)<<8)|*(data_buf+5) );
//			Steer_Ki= ( (vuint16)(*(data_buf+6)<<8)|*(data_buf+7) );
//			Steer_Kd= ( (vuint16)(*(data_buf+8)<<8)|*(data_buf+9) );
//			Motor_Duty= ( (vuint16)(*(data_buf+10)<<8)|*(data_buf+11) );
//                        if(Motor_Duty!=0)  Speed_Shift=1;
//		        Steer_I= ( (vuint16)(*(data_buf+12)<<8)|*(data_buf+13) );
//			Steer_D= ( (vuint16)(*(data_buf+14)<<8)|*(data_buf+15) );
//			yaw_p= ( (vuint16)(*(data_buf+16)<<8)|*(data_buf+17) );
//			yaw_i= ( (vuint16)(*(data_buf+18)<<8)|*(data_buf+19) );
//			yaw_d= ( (vuint16)(*(data_buf+20)<<8)|*(data_buf+21) );
//		
//		if(f.send_check == 0)
//		{
//			
//			f.send_check = 1;
//			checkdata_to_send = *(data_buf+2);
//			checksum_to_send = sum;
////			if(f.send_check)
////	{
////		f.send_check = 0;
//		ANO_DT_Send_Check(*(data_buf+2),sum);
////	}
//		}			
//		//data_save();
//
//    }
//    
//    
//}





