#include "photo_handle.h"
#include "math.h"

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>用于观看变量定义区域>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 
uint8 Right_White_Flag_view;
uint8 Right_White_view;
uint8 Left_White_Flag_view;
uint8 Left_White_view; 
uint8 Mid_Line_view;
uint8 Road_Select_Flag_view;
uint8 Road_In_Out_Flag_view;
uint8 Diamond_view;
uint8 Zebra_view;

uint8 L69,L68,R69,R68;


uint8 Middle_White_Line_Flag_view;
uint8 Straight_Road_Flag_view;
uint8 Zebra_Crossing_Flag_view;
uint8 White_View;
uint8 White_view_In;  //观看内道白区数字变化
uint8 White_view_Out; //观看外道白区数字变化

uint8 Black_D_view;
uint8 Black_Z_view;

uint8 Road_Inside_Flag_view;
uint8 Road_Outside_Flag_view;

//后边进行删除，只进行测测变量
uint8 Left_In_view;
uint8 Left_Out_view;

uint8 middle_centre_v;
uint8 left_centre_v;
uint8 right_centre_v;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>全局变量定义区>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 

uint8 imagebin[ROW][COL];  //存放二值化数组 
uint8 Threshold =100;    //拟定义静态阈值 100
uint8 Fit_Middle[ROW];  //后面进行偏差计算
extern uint8 image_threshold;  //图像阈值
extern uint16 Total_Rad,Velocity;
uint8 Road_Turn_Right_Flag =0;  //内——>外标志位  //更改一下
uint8 Road_Turn_Left_Flag =0;   //外——>内标志位
uint8 Road_Justice_Flag ;   //0：内道 1：外道  
uint8 Stop_All_Flag =0;  //停车标志位

uint8 Straight_Road_Select_Flag =0; // 0 普通直道 1 起点直道

uint8 Zhi_Flag_1;
uint8 Wan_Flag_1;

uint8 Diamond_Shape_Flag =0; //菱形、斑马线识别标志位区
uint8 Zebra_Crossing_Flag =0;

uint8 Left_Jump_Dis_view;
uint8 Huan_Dao_Ok =1;  //1表示换道结束
//uint8 Zhi_Xing_Over =1;      //观看变量值

void image_binaryzation(void)  //初步进行二值化 
{
  uint8 i,j;
  for(i=ROW-1;i >0;i--)
  {
    for(j=0;j<COL;j++)
    {
      if(image[i][j] >=image_threshold) imagebin[i][j] =0xff;    //额定值，晚上60   前道65  晚上最佳值：2019-03-17 实验值 75
      else imagebin[i][j] =0x00;
    }
    
  }
}
uint8 start_x =0,end_x =149; 

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row) //例程大津法求阈值
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = (60 *150 /4);//((width -40) * (height -25));
//    int k ,t,pixelSum_wan =(30 *120 + 20 *150);
    static uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    uint8 pixelmin =0xff;
    uint8 pixelmax =0x00;    
    static uint8 Finish_Ok =0;
        
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数 
//    if(Zhi_Flag_1 ==0 && Straight_Road_Select_Flag ==0) 
//    {
//      for(i = 30; i <60; i++)
//      {
//        for (j = 0; j < 120; j++)
//        {
//            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
//            if(data[i * width + j] >= pixelmax)  pixelmax =data[i * width + j];
//            else if( data[i * width + j] <=pixelmin) pixelmin =data[i * width + j];
//        }
//      }
//      for(i =60; i<height; i++)
//      {
//        for(j =0; j <width; j++)
//        {
//          pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
//          if(data[i * width + j] >= pixelmax)  pixelmax =data[i * width + j];
//          else if( data[i * width + j] <=pixelmin) pixelmin =data[i * width + j];
//        }
//      }
//      //计算每个像素在整幅图像中的比例  
//      for (i = pixelmin; i < pixelmax; i++)
//      {
//          pixelPro[i] = (float) pixelCount[i] / pixelSum_wan ;
//      }
//    }
    //统计灰度级中每个像素在整幅图像中的个数 
//    else
//    {
//      for (i = 60; i >20; i--)
//      {
//        for (j = 0; j < 150; j++)
//        {
//            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
//            if(data[i * width + j] >= pixelmax)  pixelmax =data[i * width + j];
//            else if( data[i * width + j] <=pixelmin) pixelmin =data[i * width + j];
//        }
//      }
      for(i =20; i<height; i+=2)  //前30行不扫描
      {
        for(j =0; j <width; j+=2)
        {
          pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
          if(data[i * width + j] >= pixelmax)  pixelmax =data[i * width + j];
          else if( data[i * width + j] <=pixelmin) pixelmin =data[i * width + j];
        }
      }
       //计算每个像素在整幅图像中的比例  
      for (i = pixelmin; i < pixelmax; i++)
      {
          pixelPro[i] = (float) pixelCount[i] / pixelSum ;
      }
//    }
    
//    start_x =LIMIT( start_x,0,110 );
//    end_x   =LIMIT( end_x,40,149 );
//    for(i =50; i>=10;i-=10)  //相隔10行改变扫描范围
//    {
//      if( i ==50) k=0;
//      else if(i ==40) k =1;
//      else if(i ==30) k =2;
//      else if(i ==20) k =3; 
//      else if(i ==10) k =4;
////      else if(i ==0)  k =5;
//      for(t =0;t <10;t++)
//      {        
//        if( start_x <3 && Straight_Road_Select_Flag ==0 )
//        {
//          for(j =0; j<(end_x -k*10);j+=2)
//          {
//            pixelCount[(int)data[(i+t) * width + j]]++;  //将像素值作为计数数组的下标
//            if(data[(i+t) * width + j] >= pixelmax)  pixelmax =data[(i+t) * width + j];
//            else if( data[(i+t) * width + j] <=pixelmin) pixelmin =data[(i+t) * width + j];
//          }
//        }
//        else
//        {
//          for(j =(start_x +k*5); j<(end_x -k*5);j+=2)
//          {
//            pixelCount[(int)data[(i+t) * width + j]]++;  //将像素值作为计数数组的下标
//            if(data[(i+t) * width + j] >= pixelmax)  pixelmax =data[(i+t) * width + j];
//            else if( data[(i+t) * width + j] <=pixelmin) pixelmin =data[(i+t) * width + j];
//          }
//        }
//      }
//    }
    
    
   

    //遍历灰度级[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    if(Finish_Ok ==0)
    {
      Finish_Ok =1;
      for (i = 0; i < 255; i++)     // i作为阈值  GrayScale
      {
          w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
          for (j = 0; j < GrayScale; j++)
          {
              if (j <= i)   //背景部分  
              {
                  w0 += pixelPro[j];
                  u0tmp += j * pixelPro[j];
              }
              else   //前景部分  
              {
                  w1 += pixelPro[j];
                  u1tmp += j * pixelPro[j];
              }
          }
          u0 = u0tmp / w0;
          u1 = u1tmp / w1;
          u = u0tmp + u1tmp;
          deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
          if (deltaTmp > deltaMax)
          {
              deltaMax = deltaTmp;
              threshold = i;
          }
      }
    }
    if( Finish_Ok ==1)
    {
      for (i = threshold -10; i < threshold +10; i++)     // i作为阈值  GrayScale
      {
          w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
          for (j = 0; j < GrayScale; j++)
          {
              if (j <= i)   //背景部分  
              {
                  w0 += pixelPro[j];
                  u0tmp += j * pixelPro[j];
              }
              else   //前景部分  
              {
                  w1 += pixelPro[j];
                  u1tmp += j * pixelPro[j];
              }
          }
          u0 = u0tmp / w0;
          u1 = u1tmp / w1;
          u = u0tmp + u1tmp;
          deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
          if (deltaTmp > deltaMax)
          {
              deltaMax = deltaTmp;
              threshold = i;
          }
      }
    }
    return threshold;
}
uint8 Zebra_Crossing_Flag_Count =0;

//默认图像尺寸 ROW：80  COLUMN：150
void Get_Center_Line(void)              //中线简单提取
{ 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>函数使用局部变量定义区域>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  
  uint8 i,j;
  uint8 Left_White_Flag =0 ;   
  uint8 Right_White_Flag =0 ; 
  uint8 Middle_Line;
  static uint8 Middle_Line_Last;
    
  static uint8 Road_Inside_Flag =0;    //判断车模内外道的标志位 
  static uint8 Road_Outside_Flag =0;
  uint8 Black_Turn_White_Flag_D =0;   //判断白线个数
  uint8 Black_Turn_White_Flag_Z =0;  
  //定义边线的宽度  
  volatile uint8 line_width =7;  //原值：7
  
  //定义直道时候白线的标志位
  uint8 Find_White_Range_Flag =0;
  uint8 Find_White_Range_Flag_In =0;
  uint8 Find_White_Range_Flag_Out =0;
  
  /////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  /////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  //更改定义区域；
  static uint8 Find_Standard_Flag =1;
  uint8 Left_Standard_Array[10];
  uint8 Right_Standard_Array[10];
    
  static uint8 Right_Array[ROW];    //左右边界数组
  static uint8 Left_Array[ROW];
  uint8 White_Point;
//  uint8 wide_falg_r =0;
//  uint8 wide_falg_l =0;
  int centre =0;
  uint8 centre_cha;
  uint8 wide =0;
  uint8 left_ok =0; 
  uint8 right_ok =0;
  uint8 middle_ok =0;
  uint8 size=0;
  uint8 m;
  uint8 a;
  
  static uint8 count_D=0,count_Z =1,count1 =0;  //用于时间计数 
  static uint8 Zhi_Xing_Over =1;
  uint8 Black_Turn_White_Flag_C =0;
  m =1;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 
  if(Find_Standard_Flag ==1)
    {
      for(i =55;i >53;i--)  //提取两行基本元素
      {
        for(j =40;j >0;j--)        
          if( (image[i][j+1] <image_threshold) && (image[i][j] >=image_threshold) && (image[i][j-1] >=image_threshold) )
          { 
            White_Point =j;
            while(image[i][j] >=image_threshold) j--;    //左边界从内向外巡白线
            Left_Standard_Array[i-53] =(White_Point +j)/2;
            break;
          }
        
        for(j =110;j <149;j++)
          if( (image[i][j-1] <image_threshold) && (image[i][j]>=image_threshold) && (image[i][j+1] >=image_threshold) )
          { 
            White_Point =j;
            while(image[i][j] >=image_threshold) j++;   //右边界从内向外巡白线
            Right_Standard_Array[i-53] =(White_Point +j)/2;
            break;
          }            
      }
      Left_In_view =Right_Array[55]  =Right_Standard_Array[2];
      Left_Out_view =Left_Array[55]   =Left_Standard_Array[2];
      Right_Array[54]  =Right_Standard_Array[1];
      Left_Array[54]   =Left_Standard_Array[1];      
    }
  Find_Standard_Flag =0;          //用于找到基准数组值，函数运行一次
  
  
  
  //中间巡白色区 
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if(Straight_Road_Select_Flag ==0 && Zhi_Flag_1 ==1)  //正常直道区域 
  {
    Zhi_Xing_Over =0;
    for(i =75;i >15;i-=5)     //查看并提取8行元素 到19
    {
      for(j =50;j <90;j++)
      { 
        if( (image[i][j] >=image_threshold) && (image[i][j+1] >=image_threshold) ) 
        {Find_White_Range_Flag ++; break;}              
      } 
      if(Find_White_Range_Flag >=1) break;  //可注释     
    }
    if(Find_White_Range_Flag >=1)      
    {Straight_Road_Select_Flag =0; }      //中间有白区
    else 
    {
      if(Zebra_Crossing_Flag_Count ==0)  //第一圈
       {Straight_Road_Select_Flag =1 ;Road_Turn_Right_Flag =1; Road_Turn_Left_Flag =1;} 
      else if(Zebra_Crossing_Flag_Count ==1)  //第二圈
      {
        if(Road_Justice_Flag ==0)  //处于内道
         {Straight_Road_Select_Flag =1 ;Road_Turn_Right_Flag =3; Road_Turn_Left_Flag =2;}  
        else                       //处于外道
        {Straight_Road_Select_Flag =1 ;Road_Turn_Right_Flag =2; Road_Turn_Left_Flag =3;} 
      }
      else
       {Straight_Road_Select_Flag =1 ;Road_Turn_Right_Flag ++; Road_Turn_Left_Flag ++;} 
    }
    
    White_View =Find_White_Range_Flag;    
    Find_White_Range_Flag =0;             //标志位清零
  } 
  //增加直道判断
  if(Straight_Road_Select_Flag ==1 )   
  {
    for(j =50;j <90;j++)  
    {
      if((image[79][j] >=image_threshold) && (image[79][j+3] >=image_threshold))  //应隔2个像素点判断
      {
        Find_White_Range_Flag ++;
        break; 
      }      
    }
    if(Find_White_Range_Flag ==1) 
    { //{Zhi_Xing_Over =1;}
      count1 ++;
      if(count1 ==4) {Zhi_Xing_Over =1;}  //防止突变点 大约4场图像时间
      if(count1 >20) count1 =20;
    }
    else
    {count1 =0;}
      
//    if((Huan_Dao_Ok ==0))           //可增加huandao_ok标志位 判断换道完成之后是否加速
//      {Zhi_Xing_Over =0;}
    
    White_View =Find_White_Range_Flag;
    Find_White_Range_Flag =0;
  }    
  //起点直道  
  if( (Straight_Road_Select_Flag ==1) && (Huan_Dao_Ok ==1) && (Zhi_Xing_Over ==1) )    //&& (Road_Inside_Flag ==1)
  {    
    //外侧直道 
    if( (Road_Outside_Flag ==1) && (Road_Inside_Flag ==0) )          //？？？？？？？？？？？？？？？？problem   Not eually one
    {
      for(i =70;i >30;i-=5)   //查看并提取4行元素   太少？？？
      {
        for(j =50;j >1;j--)
        {
          if( (image[i][j] >=image_threshold) && (image[i][j-2] >=image_threshold) )
          {Find_White_Range_Flag_Out ++; break;} 
        }
      }
      if(Find_White_Range_Flag_Out >=7)  
      {
        if(Zebra_Crossing_Flag_Count ==0) 
         {Straight_Road_Select_Flag =0;Road_Outside_Flag =0;Road_Turn_Left_Flag =2;Road_Turn_Right_Flag =1;Stop_All_Flag =1;} 
        else if(Zebra_Crossing_Flag_Count ==1)
        {Straight_Road_Select_Flag =0;Road_Outside_Flag =0;Road_Turn_Left_Flag =3;Stop_All_Flag ++; }          
        else
         {Straight_Road_Select_Flag =0;Road_Outside_Flag =0;Road_Turn_Left_Flag ++;Stop_All_Flag ++; } 
      }
//      {Straight_Road_Select_Flag =0;Road_Outside_Flag =0;Road_Turn_Left_Flag ++;Stop_All_Flag ++; }      //中间有白区
      else
       {Straight_Road_Select_Flag =1; }    
     
      
      White_view_Out =Find_White_Range_Flag_Out;
      Find_White_Range_Flag_Out =0;                //标志位清零
    }
    //内侧直道 
    else if( (Road_Inside_Flag ==1) && (Road_Outside_Flag ==0))
    {
      for(i =70;i >30;i-=5)   //查看并提取4行元素   
      {
        for(j =100;j <148;j++)
        {
          if( (image[i][j] >=image_threshold) && (image[i][j+2] >=image_threshold) )
            {Find_White_Range_Flag_In ++; break;} 
        }
      }
      if(Find_White_Range_Flag_In >=7) 
      {
        if(Zebra_Crossing_Flag_Count ==0) 
         {Straight_Road_Select_Flag =0;Road_Inside_Flag =0;Road_Turn_Right_Flag =2;Road_Turn_Left_Flag =1;Stop_All_Flag =1 ;} 
        else if(Zebra_Crossing_Flag_Count ==1)
         {Straight_Road_Select_Flag =0;Road_Inside_Flag =0;Road_Turn_Right_Flag =3;Stop_All_Flag ++; }
        else 
        {Straight_Road_Select_Flag =0;Road_Inside_Flag =0;Road_Turn_Right_Flag ++;Stop_All_Flag ++; }          
      }
//        {Straight_Road_Select_Flag =0;Road_Inside_Flag =0; Road_Turn_Right_Flag ++;Stop_All_Flag ++;}      //中间有白区
      else
        {Straight_Road_Select_Flag =1; }
      
      White_view_In =Find_White_Range_Flag_In;
      Find_White_Range_Flag_In =0;                //标志位清零
    }
    //判断出错
    else
     {Straight_Road_Select_Flag =1;}
  }
  
  Road_Inside_Flag_view =Road_Inside_Flag;   //变量观测
  Road_Outside_Flag_view =Road_Outside_Flag;
  Straight_Road_Flag_view =Straight_Road_Select_Flag; 
  
  //菱形、斑马线识别
  if(Straight_Road_Select_Flag ==0  && Zhi_Flag_1 ==1)  //正常直道区域 
  {
    for(j =20;j <130;j++) 
    {
      if(j >start_x+15 && j <end_x-15)   //外道菱形识别过多？？？？？ 5 值  
      {  
        if( (image[55][j] <image_threshold) && (image[55][j+1] >=image_threshold) )   //后面再加判断出斑马线的标志 65        
          Black_Turn_White_Flag_D ++; 
      }
      if( (image[30][j] <image_threshold) && (image[30][j+1] >=image_threshold)  ) 
      {
        Black_Turn_White_Flag_Z ++;
        Black_Turn_White_Flag_C ++;
      }
      
    } 
    
    if( (Black_Turn_White_Flag_D ==2) )  
    {
      count_D ++;
      if( (count_D ==3) )     
      {Diamond_Shape_Flag ++;}
      
      if(count_D >=20) count_D =20;       
    }    
    else 
      {count_D =0;}
    
    
    if( ( Black_Turn_White_Flag_Z >5) ) 
    {
      count_Z ++;
      if(count_Z ==4)
      {
        Zebra_Crossing_Flag =1;
        Diamond_Shape_Flag =0;
      }
      if(count_Z >20) count_Z =20;
    }
    else
    {
      Zebra_Crossing_Flag =0;
      count_Z =1;
//      count2 ++;
//      if(count2 ==5)
//      {
//        Zebra_Crossing_Flag =0;
//        count_Z =0;
//      }
//      if(count2 >20) count2 =20;      
    }
    
    if(Black_Turn_White_Flag_C >5)  //外道斑马线，白天光线识别问题？？？？？？多记录次数
    {
      count1 ++;
      if(count1 ==5)
      {
        Zebra_Crossing_Flag_Count ++; 
      }
      if(count1 >20) count1 =20;
    }
    else if(Black_Turn_White_Flag_C <3 )   //只看量边界个数
      count1 =0;
     
//      {Zebra_Crossing_Flag =1;  Diamond_Shape_Flag =0;}    
//    else if(Black_Turn_White_Flag_Z <4 && Black_Turn_White_Flag_D ==1)     //设置车出斑马线的标志位
//      {Zebra_Crossing_Flag =0;}
  }
  
  Black_Z_view =Black_Turn_White_Flag_Z;  //变量观测
  Black_D_view =Black_Turn_White_Flag_D;
  Zebra_Crossing_Flag_view =Zebra_Crossing_Flag;
  
///////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  
  for(i =55;i >50;i--)     //实验提取20行数据
  {
    for(j =1;j <3;j++)
    {
      if( (Zebra_Crossing_Flag ==1) && (Road_Justice_Flag ==0) && (j ==2)) break; //内道斑马线 右边界不扫描
      if( (Zebra_Crossing_Flag ==1) && (Road_Justice_Flag ==1) && (j ==1)) j=2;   //外道斑马线 左边界不扫描
      if(i ==55)
      {
        switch(j)
        {
        case 1: centre =Left_Array[i]; break;
        case 2: centre =Right_Array[i]; break;
        }
      }
      else
      {
        switch(j)
        {
        case 1: centre =Left_Array[i+1]; break;
        case 2: centre =Right_Array[i+1]; break;
        }
      }
      if( (centre <0) || (centre >149) ) {middle_ok = left_ok = right_ok =0; }
      else 
      {
        if(image[i][centre] >=image_threshold)    //正常区域          
        {
          wide++;
          while(image[i][centre-m] >=image_threshold)  //向左扫描
          {
            if(centre -m <0) break;
            if(wide >line_width) { break;}  //限副扫描范围  wide =0;          
            m++;
            wide++;
            if(m >4) break;    //限制一直往左边扫描
          }
          size =centre -m +1;  //记录左边界点
//          if(wide_falg_l ==1 ) {size +=wide/2;wide =0;  }  
          
          m =1;
          while(image[i][centre+m] >=image_threshold)  //向右扫描
          {
            if(centre+m >149) break;
            if(wide >line_width) { wide =0;break;}  // wide =0; 正常跑加后面的值
            m++;
            wide++;
          } 
//          if( (centre <3) && (wide_falg_r ==1) ) wide =0;   //限制复杂点
//          if( (wide_falg_r ==1) && (wide_falg_l ==1) ) wide =0;  //限制十字
          
          if(wide >0  )                        //得出白线的宽度，进行判断
          {
            middle_ok =1;
            centre_cha =0;
            centre  =size +wide/2;  //本行扫描边线值
            middle_centre_v =centre; 
            if(i <55)
            {
              switch(j)
              {
              case 1: centre_cha =fs(centre,Left_Array[i+1]); break;  //求上下行差值
              case 2: centre_cha =fs(centre,Right_Array[i+1]); break;
              }
            }
            if(centre_cha <4)  //正确测试值为：4
            {
              switch(j)
              {
              case 1: centre =Left_Array[i] =size +wide/2; break;
              case 2: centre =Right_Array[i] =size +wide/2; break;
              }
            }
            else
            {
              switch(j)
              {
              case 1: centre =Left_Array[i] =Left_Array[i+1]; break;
              case 2: centre =Right_Array[i] =Right_Array[i+1]; break;
              }
            }
          }
          else {middle_ok =0; wide =0; m=1;}
         
        }
        
        else               //非正常区域，中心点不是白区
        {
          for(m =1;m <7;m++)    //向外扫描,向左  
          {
            if(centre-m<0) break;
            else
            {
              if(image[i][centre-m] >=image_threshold)
              {
                size =centre -m ;  //记录右边界
                a =0;
                while(image[i][size -a] >=image_threshold)
                {
                  if(size -a <0) break;
                  if(wide >line_width) {wide =0; break;} //
                  a++;
                  wide++;
                }
                if(wide >0)
                {
                  centre_cha =0;
                  centre =size -wide/2;
                  left_centre_v =centre;
                  if(i <55)
                  {
                    switch(j)
                    {
                    case 1: centre_cha =fs(centre,Left_Array[i+1]); break;
                    case 2: centre_cha =fs(centre,Right_Array[i+1]); break;
                    }
                  }
                  if(centre_cha <5)
                  {
                    switch(j)
                    {
                    case 1: centre =Left_Array[i] =size -wide/2; left_ok =1;break;
                    case 2: centre =Right_Array[i] =size -wide/2;left_ok =1;break;
                    }
                  }
                  else
                  {
                    switch(j)
                    {
                    case 1: centre =Left_Array[i] =Left_Array[i+1]; break;
                    case 2: centre =Right_Array[i] =Right_Array[i+1]; break;
                    }
                  }
               }
               else {left_ok =0;wide =0;}
               if(left_ok ==1)  break;
              }
             }
          }
        
        for(m =1;m <5;m++)    //向外扫描  向右
        {
          if(left_ok ==1)  break;
          if(centre +m>149) break;
          else
          {
            if(image[i][centre+m] >=image_threshold)
            {
              size =centre +m ;  //记录左边界
              a =0;
              while(image[i][size+a] >=image_threshold)
              {
                if(size +a>149) break;
                if(wide >4) {wide =0; break;}
                a++;
                wide++;
              }
              if(wide >0)
                {
                  right_ok =1;                  
                  centre_cha =0;
                  centre =size +wide/2;
                  right_centre_v =centre;
                  if(i <55)
                  {
                    switch(j)
                    {
                    case 1: centre_cha =fs(centre,Left_Array[i+1]); break;
                    case 2: centre_cha =fs(centre,Right_Array[i+1]); break;
                    }
                  }
                  if(centre_cha <5)
                  {
                    switch(j)
                    {
                    case 1: centre =Left_Array[i] =size +wide/2; break;
                    case 2: centre =Right_Array[i] =size +wide/2;break;
                    }
                  }
                  else
                  {
                    switch(j)
                    {
                    case 1: centre =Left_Array[i] =Left_Array[i+1]; break;
                    case 2: centre =Right_Array[i] =Right_Array[i+1]; break;
                    }
                  }
               }
              else {wide =0;right_ok =0; }
              if( right_ok ==1) break; 
              
            }
          }
        }
        
        
      }
      
      if( (right_ok ==1) || (left_ok ==1) || (middle_ok ==1) )
      {
        switch(j)
        {
        case 1: Left_White_Flag =1; break;
        case 2: Right_White_Flag =1;break;
        }
      }
      else
      {
        switch(j)
        {
        case 1: Left_White_Flag =0; break;
        case 2: Right_White_Flag =0;break;
        }
      }
        
      
    }
    wide =0;
    m =1;
    left_ok =right_ok = middle_ok =0; //标志位清零
  }
  
    //外道特殊弯道判断
    if(Left_Array[55] <5) 
    {
      Zhi_Flag_1 =0;
      Wan_Flag_1 =1;
    }
                    
    //进行道路检测
    //直道判断
    if(Right_White_Flag ==1 && Left_White_Flag ==1 )
    {
      Huan_Dao_Ok =1;
      if(Left_Array[55] >5)  
      {
        Zhi_Flag_1 =1;
        Wan_Flag_1 =0;
      }     
      if(Zebra_Crossing_Flag ==1)
      {
        if(Road_Justice_Flag ==0) Middle_Line =(  Left_Array[55] +60); 
        else Middle_Line =( Right_Array[55] -60);
      }           
      else
        Middle_Line =(Right_Array[i] +Left_Array[i])/2;
      Middle_Line_Last =Middle_Line;    //记录中值      
    }
    //外道 左弯道判断 外侧斑马线  
    else if(Right_White_Flag ==1 && Left_White_Flag ==0 )      
    {
         
      if( (Road_Outside_Flag ==1) && (Straight_Road_Select_Flag ==1) )      //起点直道
      { 
        Zhi_Flag_1 =1;
        Wan_Flag_1 =0;
        if( Road_Turn_Right_Flag ==3 )      //第二圈换道之后、第三圈 && Road_Turn_Right_Flag <5
        {
          Middle_Line =Right_Array[55] -60;  //判断右换道之后  
          if(Right_Array[55] >125)           //防止溢出
            Left_Array[55] =Right_Array[55] -120;
        }
        else
          Middle_Line =(Left_Array[i] +Right_Array[i])/2;
//          Middle_Line =(Right_Array[70] -60);
      }
      else if(Zebra_Crossing_Flag ==1)                    //斑马线的处理
      {
        Zhi_Flag_1 =1;
        Wan_Flag_1 =0;
        Middle_Line =(Right_Array[55] -60);    //或者用左边界数组进行不补线
        if(Right_Array[55] >125)           //防止溢出
            Left_Array[55] =Right_Array[55] -120;
      }
      else 
      {
        if( i==55)
          Middle_Line =(Left_Array[i] +Right_Array[i])/2; //左弯道仅靠右边线来矫正值 
        else
          Middle_Line =(Left_Array[i+1] +Right_Array[i])/2;  
      } 
      Middle_Line_Last =Middle_Line;  //记录中值      
    }
                       
    //内道 右弯道判断 内侧斑马线  
    else if( (Right_White_Flag ==0) && (Left_White_Flag ==1) )
    {        
      if( (Road_Inside_Flag ==1) && (Straight_Road_Select_Flag ==1) )      //进入起点直道判断
      {
        Zhi_Flag_1 =1;
        Wan_Flag_1 =0;
        if( Road_Turn_Left_Flag ==3  ) 
        {
//          Huan_Dao_Ok =1;
          Middle_Line =Left_Array[55] +60;  //判断向左换道之后 
          if(Left_Array[55]  <25)
            Right_Array[55] =Left_Array[55]  +120;  //记录向左换道之后，补右边界
        }
        else
          Middle_Line =(Left_Array[i] +Right_Array[i])/2; 
      }
      else if(Zebra_Crossing_Flag ==1) 
      {
        Zhi_Flag_1 =1;
        Wan_Flag_1 =0;
        Middle_Line =( Left_Array[55] +60 );   //或者用左边界数组进行不补线 
        if(Left_Array[55]  <22)
            Right_Array[55] =Left_Array[55]  +120;  //补右边界
      }
      else
      {
        if( i ==55)
         Middle_Line =(Left_Array[i] +Right_Array[i])/2;
        else
          Middle_Line =(Left_Array[i] +Right_Array[i+1])/2;        
      }
      Middle_Line_Last =Middle_Line;  //记录中值
    }
    
    //两边都未寻到
    else if(Right_White_Flag ==0 && Left_White_Flag ==0 )
    { 
      Zhi_Flag_1 =1;
      Wan_Flag_1 =0;
      if(Zebra_Crossing_Flag ==1)
      {
        if(Road_Justice_Flag ==0) Middle_Line =(  Left_Array[55] +60 ); 
        else Middle_Line =(  Right_Array[55] -60 );
      }
      else if( (Road_Turn_Right_Flag ==3) && (Straight_Road_Select_Flag ==1) )    //判断换右道中线
      {
        if(Road_Justice_Flag ==1) Middle_Line =78; //换道第二次偏移
        else Middle_Line =Middle_Line_Last;
      }
      else if((Road_Turn_Left_Flag ==3) && (Straight_Road_Select_Flag ==1) )     //判断换左道中线
      {
        if(Road_Justice_Flag ==0) Middle_Line =72; //换道第二次偏移 
        else Middle_Line =Middle_Line_Last; 
      }
      else                                                          //？？？？？？？？？？
      {
         if( i==55)
          {Middle_Line =(Left_Array[i] +Right_Array[i])/2;}
         else
           Middle_Line =(Left_Array[i+1] +Right_Array[i+1])/2;
      }
      
      Middle_Line_Last =Middle_Line;  //记录中值
    }
    
    //进行换道转弯
    if( (Straight_Road_Select_Flag ==1) )
    {
      if(  (Road_Turn_Right_Flag ==3) && (Road_Turn_Left_Flag ==2) && (Road_Inside_Flag ==1))  //内道换外道
      {
        Middle_Line =80;  // 对应PWM 180可以用078
        Huan_Dao_Ok =0;
      }
      else if( (Road_Turn_Left_Flag ==3) && (Road_Turn_Right_Flag ==2) && (Road_Outside_Flag ==1)) //外道换内道
      {
        Middle_Line =70;  //白天值，72
        Huan_Dao_Ok =0;
      }
      Middle_Line_Last =Middle_Line;  //记录中值
    }
       
    
    //用于显示变量值，后续可以进行删除  
    
    Right_White_Flag_view =Right_White_Flag;
    Right_White_view =Right_Array[55];
    Left_White_Flag_view =Left_White_Flag;
    Left_White_view = Left_Array[55];    
    start_x =Left_Array[55];
    end_x   =Right_Array[55];
    start_x =LIMIT(start_x,1,120);
    end_x =LIMIT(end_x,20,149);
    
     L69 =Left_Array[54];
     L68 =Left_Array[53];
     R69 =Right_Array[54];
     R68 =Right_Array[53];
    //中线显示
    Mid_Line_view =Middle_Line;
    Fit_Middle[i] =Middle_Line;        
    imagebin[i][Middle_Line] =0xff;    //显示所巡中线
    imagebin[i][75] =0xff;             //显示赛道标准中线    
  }
  
    
  //起点直道标志位判断 （判断的为i =50行 标志位）
  if( Straight_Road_Select_Flag ==1 )
  {
    if(Left_White_Flag ==1 && Right_White_Flag ==0)  
    {
      Road_Inside_Flag =1;
      Road_Outside_Flag =0;
      Road_Justice_Flag =0;
      
    } //Road_Outside_Flag =0;
    else if(Left_White_Flag ==0 && Right_White_Flag ==1)  
    {
      Road_Outside_Flag =1;
      Road_Inside_Flag =0;
      Road_Justice_Flag=1;
    }
  }
       
}

uint8 fs(int x,uint8 y)
{
   uint8 shu;
   if(x>y)
       shu=x-y;
   else
       shu=y-x;
   return shu;
}
extern int Stero_pwm_view;
extern int pwm_view;
extern int Target;
extern uint16 KS103_Distance;   //超声波检测距离
extern uint8 Velacity_Mode;     //速度模式选择
//屏上显示字体
void OLED_Display(void)
{
  OLED_P6x8Str(0,0,"L_F"); //第一行，左右标志位
  OLED_P6x8Str(60,0,"R_F"); 
  
  OLED_P6x8Str(0,1,"L_V"); //第二行，左右坐标值
  OLED_P6x8Str(60,1,"R_V");
  
//  OLED_P6x8Str(0,2,"I_F"); //第三行，直行内外道标志
//  OLED_P6x8Str(60,2,"O_F");
  OLED_P6x8Str(0,2,"Z_F"); //第三行，直道、弯道道标志
  OLED_P6x8Str(60,2,"W_F");
  
  OLED_P6x8Str(0,3,"S_F"); //第四行，直道、斑马线标志
  OLED_P6x8Str(60,3,"Z_F");
  
  OLED_P6x8Str(0,4,"TLF");//第五行，左、右换道标志位
  OLED_P6x8Str(60,4,"TRF");
  
  OLED_P6x8Str(0,5,"J_F");  //第六行，综合判断内外道标志位
  OLED_P6x8Str(60,5,"MOTO");  //第六行，电机PWM 值大小
  
  OLED_P6x8Str(0,6,"T_V");  //第七行，显示图像阈值
  OLED_P6x8Str(60,6,"PWM");  //第七行，舵机PWM
  
  OLED_P6x8Str(0,7,"Rad");  //第八行，圈数、速度
  OLED_P6x8Str(60,7,"V");  
  
  OLED_Print_Num1(20,0,Left_White_Flag_view);  
  OLED_Print_Num1(80,0,Right_White_Flag_view);
  
  OLED_Print_Num1(20,1,Left_White_view);
  OLED_Print_Num1(80,1,Right_White_view);
  
//  OLED_Print_Num1(20,2,Road_Inside_Flag_view); //第三行，直行内外道标志
//  OLED_Print_Num1(80,2,Road_Outside_Flag_view);
  
  OLED_Print_Num1(20,2,Zhi_Flag_1);   //第三行，直道、弯道道标志
  OLED_Print_Num1(80,2,Wan_Flag_1);
  
  OLED_Print_Num1(20,3,Straight_Road_Flag_view);
  OLED_Print_Num1(80,3,Zebra_Crossing_Flag_view);
  
  OLED_Print_Num1(20,4,Road_Turn_Left_Flag);
  OLED_Print_Num1(80,4,Road_Turn_Right_Flag);
  
  OLED_Print_Num1(20,5,Road_Justice_Flag);
  OLED_Print_Num1(80,5,Target);
  
  OLED_Print_Num1(20,6,KS103_Distance); //image_threshold KS103_Distance  Velacity_Mode
  OLED_Print_Num1(80,6,Stero_pwm_view); //Stero_pwm_view Stop_All_Flag
  
  OLED_Print_Num1(20,7,Total_Rad); //Total_Rad,Black_D_view Zhi_Xing_Over White_View
  OLED_Print_Num1(80,7,Diamond_Shape_Flag); //Stop_All_Flag Zhi_Xing_Over  Diamond_Shape_Flag
  
  //Zebra_Crossing_Flag_Count
}


//关于图像无线传输函数
void nrf_send_array(uint8 *dat, uint16 length)
{
    uint16 i;
    uint16 send_num;
    
    length += 4;//加上4个字节的帧头
    //计算需本次要发送的数据个数，每次发送的数据不能超过31个。
    if(31 < length)   
    {
        send_num = 31;
        length -= 31;
    }
    else
    {
        send_num = length;
        length = 0;
    }
    NRF_TX_Buff[0] = send_num;
    NRF_TX_Buff[1] = 0X00;
    NRF_TX_Buff[2] = 0XFF;
    NRF_TX_Buff[3] = 0X01;
    NRF_TX_Buff[4] = 0X01;
    for(i=5; i<32; i++)
    {
        NRF_TX_Buff[i] = *dat;
        dat++;
    }
    NRF_Send_Packet(NRF_TX_Buff);
    systick_delay_ms(2);

    while(length)
    {
        //计算需本次要发送的数据个数，每次发送的数据不能超过31个。
        if(31 < length)   
        {
            send_num = 31;
            length -= 31;
        }
        else
        {
            send_num = length;
            length = 0;
        }
        
        NRF_TX_Buff[0] = send_num;
        for(i=1; i<32; i++)
        {
            NRF_TX_Buff[i] = *dat;
            dat++;
        }
        
        NRF_Send_Packet(NRF_TX_Buff);
        systick_delay_ms(2);
    }
}













