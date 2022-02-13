#ifndef _ANO_LINK_H
#define _ANO_LINK_H

#include "headfile.h"
typedef struct
{
    uint8 msg_id;
    uint8 msg_data;
    uint8 send_check;
    uint8 send_version;
    uint8 send_status;
    uint8 send_senser;
    uint8 send_senser2;
    uint8 send_pid1;
    uint8 send_pid2;
    uint8 send_pid3;
    uint8 send_pid4;
    uint8 send_pid5;
    uint8 send_pid6;
    uint8 send_rcdata;
    uint8 send_offset;
    uint8 send_motopwm;
    uint8 send_power;
    uint8 send_user;
    uint8 send_speed;
    uint8 send_location;
    uint8 send_vef;
    uint16 send_parame;
} dt_flag_t;

extern float Motor_P,Motor_I,Motor_D;	//roll方向上的pid数据
extern float Steer_P,Steer_I,Steer_D;	//pitch方向上的pid数据
extern float yaw_p,yaw_i,yaw_d;
extern dt_flag_t f;
//void usart1_send(unsigned char *DataToSend,u8 data_num)
void usart3_send(unsigned char *DataToSend,uint8 data_num);
void ANO_DT_Send_Check(uint8 head, uint8 check_sum);
void ANO_DT_Send_Actual(uint16 a_x,uint16 a_y,uint16 a_z,uint16 g_x,uint16 g_y,uint16 g_z,uint16 m_x,uint16 m_y,uint16 m_z);
void ANO_DT_Send_PID(uint8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);
void ANO_DT_Send_SetPoint(float angle_rol, float angle_pit, float angle_yaw, uint32 alt, uint8 fly_model, uint8 armed);
void ANO_DT_Data_Receive_Prepare(uint8 data);
void ANO_DT_Data_Receive_Anl(uint8 *data_buf,uint8 num);

#endif