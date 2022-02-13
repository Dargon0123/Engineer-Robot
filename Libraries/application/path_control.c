#include "path_control.h"                                                //用于绝对值处理
extern uint8 Fit_Middle[ROW];         //后面进行偏差计算  摄像头提取中线数组
extern uint8 Receive_P;
extern uint8 Receive_D;
extern uint8 Receive_Pwm;
extern uint16 Total_Rad;     //拟定速度全为正值，后期调整刹车速度可以为负值
extern float Velocity;
extern uint8 Stop_All_Flag;

extern uint8 Diamond_Shape_Flag ; //菱形、斑马线识别标志位区
extern uint8 Zebra_Crossing_Flag ;

extern uint8 Straight_Road_Select_Flag ;
extern uint8 Zhi_Flag_1;
extern uint8 Wan_Flag_1;

extern uint8 Road_Turn_Right_Flag ;  //内——>外标志位
extern uint8 Road_Turn_Left_Flag ;   //外——>内标志位
extern uint8 Road_Justice_Flag ;   //0：内道 1：外道  
 
extern uint16 KS103_Distance;   //超声波检测距离
extern uint8 Zebra_Crossing_Flag_Count;

//全局变量用于观测值的变化
extern int pwm_view;
int Stero_pwm_view;
int Target_view;
float I_view,D_view;

//舵机PID参数
PID_arg_t Steer_arg;
static PID_val_t Steer_val;
//电机PID参数
PID_arg_t Straight_arg;
static PID_val_t Straight_val;

PID_arg_t Curve_arg;
PID_val_t Curve_val;


/*--------------------------------PID 参数区----------------*/
/**************************************************************************
函数功能：PID 参数区
入口参数：无
返回值  ：
函数说明：调整PID参数 舵机、电机、弯道
**************************************************************************/
void PID_Init(void)
{
  //舵机的PID参数
  Steer_arg.kp =7.6;  //7.6由于两边直道有误差，导致前头不稳定，目前最适合p 7 D 20 可以稳定 04-16日以前参数
  Steer_arg.ki =0;
  Steer_arg.kd =20;
  
  //直道的PID参数
  Straight_arg.kp =5.0;  //0.22 0.8 3.5 值 可以稳定 4.0稳定
  Straight_arg.ki =0.02;  //0.01
  Straight_arg.kd =0;
  
  //弯道的PID参数
  Curve_arg.kp =0;
  Curve_arg.ki =0;
  Curve_arg.kd =0;
}


/**************************************************************************
函数功能：舵机控制车模方向
入口参数：无
返回值  ：
函数说明：用一组PID进行计算
**************************************************************************/
void Road_Control(void)
{
  //计算实际偏差 
  int8 i;
  int16 Stero_pwm;
  float Target =0;
  static int Target_Last =75;
  
//  Array_Rank(Fit_Middle,3);
//  Array_Filter(Fit_Middle,3);   //进行数组滤波处理
  
  for(i =3;i >0;i--)    // 相应计算3行元素
  {
    Target += i *Fit_Middle[i+52];    
  }
  Target /=6;            //使用加权平均
//  if(Zhi_Flag_1 ==0)
//    Target =Fit_Middle[55];
  if( (ABS(Target -Target_Last)) > 30) Target =Target_Last;
  Stero_pwm =PID_Position_Calcualte(75,(float)Target,&Steer_arg,&Steer_val); //直道控制    
  Stero_pwm +=1520;   //理论值 1520
  //以下用于观测变量值
  
  Stero_pwm_view =Stero_pwm;
  Target_view =(int)Target;
  Stero_pwm =LIMIT(Stero_pwm,1200,1800); //用于限制舵机转角范围
  
  Target_Last =(int)Target;
  Set_Steer_Pwm(Stero_pwm);  //舵机
//  Pwm_Test(); //电机测试
}

int Target =0,Target_Last =0;
extern uint8 Huan_Dao_Ok;
uint8 Stop_Flag =1;
uint8 Decelerate_Flag =0;
uint8 Detect_Ok =0;  //红外检测 Ok标志位
uint8 Voice_Detect_Ok =0;
/**************************************************************************
函数功能：电机速度控制 (稳定速度)   （需现场修改编码器的圈数）？？？？？？？？
入口参数：无                           内弯道 40R 外弯道  50R
返回值  ：
函数说明：各阶段赛道采取不同速度控制   内到外  420~470圈数
**************************************************************************/
void Motor_Control(void)
{
//  static uint8 Decelerate_Falg =0;
  static int pwm =0;
  static uint16 count =0;
//  static uint8 Stop_Flag =1;
  //内道
  if(Road_Justice_Flag ==0)
  {
    //正常区
    if(Decelerate_Flag ==0 )
    {    
      //起点----终点直道
      if(Straight_Road_Select_Flag ==1)
      { //第一圈
        if(Road_Turn_Right_Flag ==1 && Road_Turn_Left_Flag ==1) 
          Target =600;  //原值 900 850
        //第二圈 (换道速度)
        else if( (Road_Turn_Right_Flag ==3 && Road_Turn_Left_Flag ==2) || (Road_Turn_Right_Flag ==2 && Road_Turn_Left_Flag ==3))
        {
          if(Huan_Dao_Ok ==1) Target =1000;  //1100
          else Target =500;                 //500
        }
        //第三圈
        else
          Target =900;     //1200
      }
      //普通直道
      else if( (Zhi_Flag_1 ==1) && (Wan_Flag_1 ==0) && (Straight_Road_Select_Flag ==0))
      {
        Target =850;  //原值 850
      }
      //弯道
      else if( (Zhi_Flag_1 ==0) && (Wan_Flag_1 ==1) && (Straight_Road_Select_Flag ==0))
      {
        Target =700; //原值 700  750
      }
    }
    
    Target_Last =Target;   //记录上次目标值 
     //红外检测、超声波检测  
    if(Straight_Road_Select_Flag ==1 && Total_Rad >310 && Zebra_Crossing_Flag_Count ==1)  
      Hongwai_Control();  //限制第二圈 换道区
//    if(Zhi_Flag_1 ==1 && Total_Rad >310 && Detect_Ok ==0) 
//      KS103_Disatance_Control();
    //减速区
    if(Stop_Flag ==1)
    {
      //菱形 //先用固定值代替 
      if(Diamond_Shape_Flag ==1 && Total_Rad >450 && Total_Rad <550 )  //原值 360-420 测得值 440
      { Decelerate_Flag =1;}
      else if(Diamond_Shape_Flag >=2 && Total_Rad >450 && Total_Rad <550 ) {Decelerate_Flag =2;}
      
      if(Decelerate_Flag ==1) Target =500;     //600 
      else if(Decelerate_Flag ==2)   Target =200;  // 200
      
      if(Zebra_Crossing_Flag_Count ==1)   //仅限刹车时间，检测
      {
        Target_Last =Target;   //记录上次目标值 
        //红外检测、超声波检测  
        if(Straight_Road_Select_Flag ==1 && Total_Rad >310 && Zebra_Crossing_Flag_Count ==1)  
          Hongwai_Control();  //限制第二圈 换道区
//        if(Zhi_Flag_1 ==1 && Total_Rad >310 && Detect_Ok ==0) 
//          KS103_Disatance_Control();
      }
//      
      
      //斑马线停车
      if(Zebra_Crossing_Flag ==1 && Total_Rad >410 )
      {
        Target =0;        
        if(Velocity <100)
        {
          count ++; 
          Decelerate_Flag =0;
          if(count >850) 
          {
            count =0;
            Target =1000;
            Stop_Flag =0;        
          }
        }
      }
    }
     
  if( Stop_All_Flag >=3 && (Total_Rad >610) && Zhi_Flag_1 ==0)   //停车标志位       （后面加弯道判断标志）
  {
    Target =0;
  }
  }
  //外道
  else
  {
    //正常区
    if(Decelerate_Flag ==0 )
    {    
      //起点----终点直道
      if(Straight_Road_Select_Flag ==1)
      { //第一圈
        if(Road_Turn_Right_Flag ==1 && Road_Turn_Left_Flag ==1) 
          Target =600;    //1000
        //第二圈 (换道速度)
        else if( (Road_Turn_Right_Flag ==3 && Road_Turn_Left_Flag ==2) || (Road_Turn_Right_Flag ==2 && Road_Turn_Left_Flag ==3))
        {
          if(Huan_Dao_Ok ==1) Target =1000;  //1100
          else Target =500;                  //500
        }
        //第三圈
        else 
          Target =900;                      //1200
      }
      //普通直道
      else if( (Zhi_Flag_1 ==1) && (Wan_Flag_1 ==0) && (Straight_Road_Select_Flag ==0))
      {
        Target =900;   //900
      }
      //弯道
      else if( (Zhi_Flag_1 ==0) && (Wan_Flag_1 ==1) && (Straight_Road_Select_Flag ==0))
      {
        Target =750;   //800
      }
    }
    
    //红外检测、超声波检测
      Target_Last =Target;   //记录上次目标值 (专为减道区 使用检测装置)
      if( (Straight_Road_Select_Flag ==1 || Zhi_Flag_1 ==0) && Total_Rad >280 && Zebra_Crossing_Flag_Count >=1)   
        Hongwai_Control();  //限制第二圈 换道区
//      if(Zhi_Flag_1 ==1 && Total_Rad >280 && Detect_Ok ==0) 
//        KS103_Disatance_Control();
    //减速区        
    if(Stop_Flag ==1)
    {
      //菱形 //先用固定值代替 
      if(Diamond_Shape_Flag ==1 && Total_Rad >420 && Total_Rad <500 ) //测试 原值 400-460
      {Decelerate_Flag =1;} 
      else if(Diamond_Shape_Flag ==2 && Total_Rad >420 && Total_Rad <500 ) {Decelerate_Flag =2;} 
      
      if(Decelerate_Flag ==1) Target =500;    
      else if(Decelerate_Flag ==2)   Target =200;
      
      
      if(Zebra_Crossing_Flag_Count ==1)   //仅限刹车时间
      {
        Target_Last =Target;   //记录上次目标值 
        //红外检测、超声波检测  
        if(Straight_Road_Select_Flag ==1 && Total_Rad >2280 && Zebra_Crossing_Flag_Count ==1)  
          Hongwai_Control();  //限制第二圈 换道区
//        if(Zhi_Flag_1 ==1 && Total_Rad >280 && Detect_Ok ==0) 
//          KS103_Disatance_Control();
      }
//      
      //斑马线停车
      if(Zebra_Crossing_Flag ==1 && Total_Rad >420 )
      {
        Target =0;        
        if(Velocity <100)
        {
          count ++; 
          Decelerate_Flag =0;
          if(count >850) 
          {
            count =0;
            Target =1000;
            Stop_Flag =0;        
          }
        }
      }
    }
    
    
  if( Stop_All_Flag >=3 && (Total_Rad >650) && Zhi_Flag_1 ==0)   //停车标志位       （后面加弯道判断标志）
  {
    Target =0;
  }
  }
  
  
  
 if( (Target -Velocity >= 400) && (Zhi_Flag_1 ==1) && (Detect_Ok ==0) &&(Voice_Detect_Ok ==0))   pwm =800; //800
 else  
 {
   if(Velocity <10)   {Straight_val.integral =0; }  //速度零，停车，积分项清零
   pwm = -( PID_Position_Calcualte(Velocity,Target,&Straight_arg,&Straight_val) ); 
 }
// Target_Last =Target;   //记录上次目标值
 if(pwm >=0)      
  {
    pwm =LIMIT(pwm,0,800);  //800
    Set_Pwm(pwm,0);
  } 
  if(pwm <0)
  {
    pwm =ABS(pwm);
    pwm =LIMIT(pwm,0,800);
    Set_Pwm(0,pwm);
  }
  pwm_view =pwm;
}

/**************************************************************************
函数功能：电机速度控制 (提速)
入口参数：无
返回值  ：
函数说明：各阶段赛道采取不同速度控制
**************************************************************************/
void Motor_Control_Fast(void)
{
//  static uint8 Decelerate_Falg =0;
  static int pwm =0;
  static uint16 count =0;
//  static uint8 Stop_Flag =1;
  //内道
  if(Road_Justice_Flag ==0)
  {
    //正常区
    if(Decelerate_Flag ==0 )
    {    
      //起点----终点直道
      if(Straight_Road_Select_Flag ==1)
      { //第一圈
        if(Road_Turn_Right_Flag ==1 && Road_Turn_Left_Flag ==1) 
          Target =1000;  //原值 900
        //第二圈 (换道速度)
        else if( (Road_Turn_Right_Flag ==3 && Road_Turn_Left_Flag ==2) || (Road_Turn_Right_Flag ==2 && Road_Turn_Left_Flag ==3))
        {
          if(Huan_Dao_Ok ==1) Target =1100;
          else Target =500;
        }
        //第三圈
        else
          Target =1300;  
      }
      //普通直道
      else if( (Zhi_Flag_1 ==1) && (Wan_Flag_1 ==0) && (Straight_Road_Select_Flag ==0))
      {
        Target =900;  //原值 850
      }
      //弯道
      else if( (Zhi_Flag_1 ==0) && (Wan_Flag_1 ==1) && (Straight_Road_Select_Flag ==0))
      {
        Target =800; //原值 700
      }
    }
    
   Target_Last =Target;   //记录上次目标值   
  //红外检测、超声波检测
  if( (Straight_Road_Select_Flag ==1 || Zhi_Flag_1 ==0)&& Total_Rad >200 && Zebra_Crossing_Flag_Count >=1)  
    Hongwai_Control();  //限制第二圈 换道区 弯道区 第二圈、第三圈都要检测
  if(Zhi_Flag_1 ==1 && Total_Rad >240 && Detect_Ok ==0)     
    KS103_Disatance_Control();
  
  //减速区
    if(Stop_Flag ==1)
    {
      //菱形 //先用固定值代替 
      if(Diamond_Shape_Flag ==1 && Total_Rad >400 && Total_Rad <460 )  //原值 360-420
      { Decelerate_Flag =1;}
      else if(Diamond_Shape_Flag >=2 && Total_Rad >400 && Total_Rad <460 ) {Decelerate_Flag =2;}
      
      if(Decelerate_Flag ==1) Target =600;    
      else if(Decelerate_Flag ==2)   Target =200;
      
      //红外检测、超声波检测
      Target_Last =Target;   //记录上次目标值 (专为减道区 使用检测装置)
      if( (Straight_Road_Select_Flag ==1 || Zhi_Flag_1 ==0) && Total_Rad >200 && Zebra_Crossing_Flag_Count >=1)   
        Hongwai_Control();  //限制第二圈 换道区
      if(Zhi_Flag_1 ==1 && Total_Rad >240 && Detect_Ok ==0) 
        KS103_Disatance_Control();
      
      //斑马线停车
      if(Zebra_Crossing_Flag ==1 && Total_Rad >410 )
      {
        Target =0;        
        if(Velocity <100)
        {
          count ++; 
          Decelerate_Flag =0;
          if(count >840) 
          {
            count =0;
            Target =1000;
            Stop_Flag =0;        
          }
        }
      }
    }
  if( Stop_All_Flag >=3 && (Total_Rad >600))   //停车标志位       （后面加弯道判断标志）
  {
    Target =0;
  }
  }
  //外道
  else
  {
    //正常区
    if(Decelerate_Flag ==0 )
    {    
      //起点----终点直道
      if(Straight_Road_Select_Flag ==1)
      { //第一圈
        if(Road_Turn_Right_Flag ==1 && Road_Turn_Left_Flag ==1) 
          Target =1100;
        //第二圈 (换道速度)
        else if( (Road_Turn_Right_Flag ==3 && Road_Turn_Left_Flag ==2) || (Road_Turn_Right_Flag ==2 && Road_Turn_Left_Flag ==3))
        {
          if(Huan_Dao_Ok ==1) Target =1100;
          else Target =500;
        }
        //第三圈
        else
          Target =1300;  
      }
      //普通直道
      else if( (Zhi_Flag_1 ==1) && (Wan_Flag_1 ==0) && (Straight_Road_Select_Flag ==0))
      {
        Target =1000;
      }
      //弯道
      else if( (Zhi_Flag_1 ==0) && (Wan_Flag_1 ==1) && (Straight_Road_Select_Flag ==0))
      {
        Target =900;
      }
    }
    
   Target_Last =Target;   //记录上次目标值  
//   Hongwai_Control(); 
  //红外检测、超声波检测   
  if( (Straight_Road_Select_Flag ==1 || Zhi_Flag_1 ==0) && Total_Rad >200 && Zebra_Crossing_Flag_Count >=1)   
    Hongwai_Control();  //限制第二圈 换道区
  if(Zhi_Flag_1 ==1 && Total_Rad >240 && Detect_Ok ==0) 
    KS103_Disatance_Control();
 
  //减速区        
    if(Stop_Flag ==1)
    {
      //菱形 //先用固定值代替 
      if(Diamond_Shape_Flag ==1 && Total_Rad >380 && Total_Rad <440 ) //测试 原值 400-460
      {Decelerate_Flag =1;} 
      else if(Diamond_Shape_Flag ==2 && Total_Rad >380 && Total_Rad <440 ) {Decelerate_Flag =2;} 
      
      if(Decelerate_Flag ==1) Target =600;    
      else if(Decelerate_Flag ==2)   Target =200;
      
      //红外检测、超声波检测            (专为减道区 使用检测装置)
      Target_Last =Target;   //记录上次目标值
      if( (Straight_Road_Select_Flag ==1 || Zhi_Flag_1 ==0) && Total_Rad >200 && Zebra_Crossing_Flag_Count >=1)   
        Hongwai_Control();  //限制第二圈 换道区
      if(Zhi_Flag_1 ==1 && Total_Rad >240 && Detect_Ok ==0) 
        KS103_Disatance_Control();
      
      //斑马线停车
      if(Zebra_Crossing_Flag ==1 && Total_Rad >400 )
      {
        Target =0;        
        if(Velocity <100)
        {
          count ++; 
          Decelerate_Flag =0;
          if(count >1000) 
          {
            count =0;
            Target =1000;
            Stop_Flag =0;        
          }
        }
      }
    }
  if( Stop_All_Flag >=3 && (Total_Rad >580))   //停车标志位       （后面加弯道判断标志）
  {
    Target =0;
  }
  }
  
 if(Target -Velocity >= 450 && Zhi_Flag_1 ==1)   pwm =800;  
 else
 {
   if(Velocity <10)   {Straight_val.integral =0; }  //速度零，停车，积分项清零
   pwm = -( PID_Position_Calcualte(Velocity,Target,&Straight_arg,&Straight_val) ); 
 }
// Target_Last =Target;   //记录上次目标值
 if(pwm >=0)
  {
    pwm =LIMIT(pwm,50,900);
    Set_Pwm(pwm,0);
  } 
  if(pwm <0)
  {
    pwm =ABS(pwm);
    pwm =LIMIT(pwm,50,900);
    Set_Pwm(0,pwm);
  }
  pwm_view =pwm;
}


/**************************************************************************
函数功能：车灯控制   (需修改，编码器的圈数)？？？？？？？？？？？？？？
入口参数：无
返回值  ：
**************************************************************************/
void LED_Control(void)
{  
  //刹车灯 Decelerate_Falg 先为 0 后停车 注意！！！  //加声波检测标志位  //原值；580
  if( (((Decelerate_Flag !=0) || ((Total_Rad >640)) )&& (Velocity !=0)) || Voice_Detect_Ok ==1 ) LED_Open(Brake_LED);
  else LED_Close(Brake_LED);
  
  //左转向灯    (对应定时器计数 设置时间闪烁频率)？？？？
  if( (Zhi_Flag_1 ==0) || ((Road_Turn_Right_Flag ==2 && Road_Turn_Left_Flag ==3) && (Huan_Dao_Ok ==0)))
    LED_Turn(Turn_L_LED);  
  //右转向灯
  else if((Road_Turn_Right_Flag ==3) && (Road_Turn_Left_Flag ==2) && (Huan_Dao_Ok ==0) )
    LED_Turn(Turn_R_LED);
  else
  {
    LED_Close(Turn_L_LED);
    LED_Close(Turn_R_LED);
  }
  //前车灯
  if( (Velocity !=0) || (Stop_Flag ==1) ) LED_Open(Front_LED);
  else 
  {
    LED_Close(Front_LED);
    LED_Close(Brake_LED);
    LED_Close(Turn_L_LED);
    LED_Close(Turn_R_LED);
  }
}



/**************************************************************************
函数功能：光电管检测控制
入口参数：无
返回值  ：无
函数调用：执行时放在 超声波检测后面。在换道区使用光电管。
**************************************************************************/
void Hongwai_Control(void)
{  
  switch (Road_Justice_Flag)
  {
  case 0:
      {
        switch (Zhi_Flag_1)
        {
          case 0:
          {
            if( Hongwai_Status(Hongwai_2) ==0 ) 
              {Target =50;Detect_Ok =1 ;} 
//            else if( Hongwai_Status(Hongwai_3) +Hongwai_Status(Hongwai_4) <2)
//              {Target =100;Detect_Ok =1 ;}   
        //    else if( Hongwai_Status(Hongwai_5) +Hongwai_Status(Hongwai_6) <2)
        //      {Target =100;Detect_Ok =1 ;} 
            else
              {Target =Target_Last;Detect_Ok =0;}
            break;
          }
          case 1:
          {
            if( Hongwai_Status(Hongwai_1) +Hongwai_Status(Hongwai_2) <2 ) 
              {Target =0;Detect_Ok =1 ;} 
            else if( Hongwai_Status(Hongwai_3) +Hongwai_Status(Hongwai_4) <2)
              {Target =100;Detect_Ok =1 ;}   
        //    else if( Hongwai_Status(Hongwai_5) +Hongwai_Status(Hongwai_6) <2)
        //      {Target =100;Detect_Ok =1 ;} 
            else
              {Target =Target_Last;Detect_Ok =0;}
            break;
          }          
        }
        break;
      }
           
  case 1:
      {
        switch (Zhi_Flag_1)
        {
          case 0:
            {
              if( Hongwai_Status(Hongwai_2) ==0 ) 
                {Target =0;Detect_Ok =1 ;} 
//              else if( Hongwai_Status(Hongwai_3) +Hongwai_Status(Hongwai_4) <2)
//                {Target =100;Detect_Ok =1 ;}   
          //    else if( Hongwai_Status(Hongwai_5) +Hongwai_Status(Hongwai_6) <2)
          //      {Target =100;Detect_Ok =1 ;} 
              else
                {Target =Target_Last;Detect_Ok =0;}
              break;
             }
          case 1:
            {
              if( Hongwai_Status(Hongwai_1) +Hongwai_Status(Hongwai_2) <2 ) 
                {Target =0;Detect_Ok =1 ;} 
              else if( Hongwai_Status(Hongwai_3) +Hongwai_Status(Hongwai_4) <2)
                {Target =100;Detect_Ok =1 ;}   
          //    else if( Hongwai_Status(Hongwai_5) +Hongwai_Status(Hongwai_6) <2)
          //      {Target =100;Detect_Ok =1 ;} 
              else
                {Target =Target_Last;Detect_Ok =0;}
              break;
            }
        }
        break;
      }
   }  
}

/**************************************************************************
函数功能：超声波检测距离控制
入口参数：无
返回值  ：检测距离1 m以内
**************************************************************************/
void KS103_Disatance_Control(void)
{
  static uint8 Brake_Rank =0; 
  
  //switch版本减速
  if(KS103_Distance <=100 && KS103_Distance >90)    {Brake_Rank =1;Voice_Detect_Ok =1 ;}
//  else if(KS103_Distance <=120 && KS103_Distance >90){Brake_Rank =2;Voice_Detect_Ok =1 ;}
  else if(KS103_Distance <=90 && KS103_Distance >60) {Brake_Rank =3;Voice_Detect_Ok =1 ;}
  else if(KS103_Distance <=60 && KS103_Distance >30) {Brake_Rank =4;Voice_Detect_Ok =1 ;}
  else if(KS103_Distance <=30 && KS103_Distance >20) {Brake_Rank =5;Voice_Detect_Ok =1 ;}
  else if(KS103_Distance <=20 && KS103_Distance >10) {Brake_Rank =6;Voice_Detect_Ok =1 ;}
  else if(KS103_Distance <=10 && KS103_Distance >5)  {Brake_Rank =7;Voice_Detect_Ok =1 ;}
  else if(KS103_Distance <5) {Brake_Rank =8;Voice_Detect_Ok =1 ;}
  else {Brake_Rank =0; Voice_Detect_Ok =0 ;}
  
  switch (Brake_Rank)
  {
    case 0:Target =Target_Last;break;
    case 1:Target =(int)1.5 *KS103_Distance; break;
//    case 2:Target =(int)1.5 *KS103_Distance; break;
    case 3:Target =(int)1.5 *KS103_Distance; break;
    case 4:Target =(int)1.5 *KS103_Distance;; break;
    case 5:Target =30; break;
    case 6:Target =20;  break;
    case 7:Target =10;  break;
    case 8:Target =0;  break;
    
    default: break;
  }
}

//位置式PID
int16 PID_Position_Calcualte(float feedback,
                              float expect,
                             PID_arg_t *pid_arg,
                             PID_val_t *pid_val)
{
  static int16 out;
  pid_val->bias =feedback -expect;      //求偏差
  pid_val->bias =0.8f *pid_val->bias;   //进行简单过滤处理 
  pid_val->bias +=0.2f *pid_val->last_bias;
  pid_val->integral +=pid_val->bias;
  pid_val->integral =LIMIT(pid_val->integral,-10000,10000);
  pid_val->differential =pid_val->bias -pid_val->last_bias; //求微分项
  D_view =pid_val->differential;
  out =(int)(pid_arg->kp * pid_val->bias + pid_arg->ki * pid_val->integral + pid_arg->kd *pid_val->differential);
  
  pid_val->last_bias  =pid_val->bias;
  return out;
}

//增量式PID
int16 PID_Speed_Calcualte(uint8 feedback,
                         int expect,
                         PID_arg_t *pid_arg,
                         PID_val_t *pid_val)
{
  static int16 out;
  pid_val->bias =expect -feedback;      //求偏差
  pid_val->bias =0.8f *pid_val->bias;   //进行简单过滤处理 
  pid_val->bias +=0.2f *pid_val->last_bias;
  pid_val->integral =pid_val->bias;
  pid_val->differential =((pid_val->bias +pid_val->last_last_bias) -2 *pid_val->last_bias); //求微分项
  D_view =pid_val->differential;
  out =(int)(pid_arg->kp * (pid_val->bias -pid_val->last_bias) //Kp
             + pid_arg->ki * pid_val->integral                //Ki
             + pid_arg->kd *pid_val->differential);            //Kd
  
  pid_val->last_last_bias =pid_val->last_bias;
  pid_val->last_bias  =pid_val->bias;  
  return out;
}


//选择法进行排序
void Array_Rank(uint8 str[],uint8 len)     //len长度数组从大到小进行排序
{
  uint8 i,j,k,temp;
  for(i =0;i <(len-1);i++)
  {
    k =i;
    for(j =(i+1);j <len;j++)
         if(str[j] >str[k])
           k =j;
    if(k !=i)
    {temp =str[i];str[i] =str[k];str[k] =temp;}
    
  }
 
}

void Array_Filter(uint8 str[],uint8 len)   //len长度数组
{
  uint8 i;
  
  for(i=0;i <(len-1);i++)
    if( ABS( (str[i]-str[i+1]) ) > 10) str[i] =str[2];  
}

