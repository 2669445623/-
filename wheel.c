/**
  ******************************************************************************
  * @file    wheel.c
  * @brief   这个文件集成了关于车辆控制，PID算法的代码
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
//pid_yuntai_setting(2000);+为逆时针

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "wheel.h"
#include "pid.h"
#include "JY62.h"
#include "xunji.h"
#include "stdio.h"
#include "usart.h"
#include "tim.h"
#include "duoji.h"
#include "string.h"
#include "esp8266.h"
static CAN_TxHeaderTypeDef chassis_tx_message_1;
static    uint8_t            chassis_can_1_send_data[8];
static CAN_TxHeaderTypeDef chassis_tx_message;//用于底座
static uint8_t   chassis_can_send_data[8];//用于底座
static PID_TypeDef motor_pid_in[4],motor_pid_out[4],in,out,ingan,outgan;
CAN_RxHeaderTypeDef rx_header;
motor_measure_t motor_chassis[7]= {0};//用于丝杆和底座
motor_measure_t_2 motor_chassis_b = {0};//用于云台
uint8_t rx_data[8]={0};
int16_t max_speed =0;  //取最大速度值

extern int f_m_number;  //头方向计数
extern int l_m_number ;  //侧方向计数
extern int l_l2_number ;
extern int r_m_number ;  
extern int r_l2_number ;
extern int b_m_number;
extern int xjq1;
extern int r_r2_number;
extern int success;
int pingfang=900;
int shuzhi=1600;
int  a[2]={0};
extern int jingzhi_motor;
extern int moshi;
extern uint8_t openmvdate[11];
extern uint8_t esp8266date[12];
extern uint8_t e[6];
int m;
extern uint8_t s[6];

int duoji_work[2]={0};
int guiwei_work[3]={0};
int qidong_work[3]={-1};

int tiqian_run=0;
extern int weizhi;
extern int weizhi_cu;

int shunxu[6] ={0};
int shunxu_wei=0;

extern  struct
{
		float Angle_x;  //角度
		float Angle_y;  //俯仰角
		float Angle_z;  
		
		float Gyro_x;   //角速度
	  float Gyro_y;
		float Gyro_z;
		
		float Acc_x;    //加速度
	  float Acc_y;
		float Acc_z;
	}JY62_record;
float current_angel;
int time_limit=0;
int time=0;
	int saomiao=0;
	int chenggong=0;
void chasis(void const * argument)
{

	can_filter_init();
	pid_main_init();
	ESP8266_Init ();
  for(;;)
  {                                                     			 

		
		
//保护函数
   if(moshi==1)
		{	
		wheelhandle();
	   m_max_speed();
		 if(max_speed>=5000)//过速保护
		 {
			while(1)
			{
				pid_motor_setting(0,0,0,0);
				osDelay(1);
			}		
		}

		if((__fabs(motor_chassis[4].given_current)>100)&&(motor_chassis[4].ecd_error!=0))//保护丝杆
		{
			pid_elevator_setting_sudu(0);
		}

		
//搬运代码	
//后退2格	
    else if(l_m_number<2)
		{
		 openmv(1600)	;	
		houtui(3000);
		 xjzuo(1);//循迹左	
		}
		
//在左移6格
		else if(l_m_number==2&&f_m_number<6)
		{	
			if(f_m_number!=2)
			{
				 if(f_m_number==5)
				{
				zuoyi(1500);
				xjqian(1);
				}
				else if(f_m_number!=5)
				{
				zuoyi(3000);
				xjqian(1);
				}
			}
			else if(f_m_number==2)
			{
				 
				 for(int i=0;i<260;i++)
					{   
						zuoyi(1000);
						xjqian(1);
						osDelay(1);
					}
					HAL_GPIO_WritePin(deng_GPIO_Port,deng_Pin,GPIO_PIN_RESET);//kai
					HAL_GPIO_WritePin(openmv_moshi_GPIO_Port,openmv_moshi_Pin,GPIO_PIN_SET);
					if(saomiao==0)
					{
						for(int i=0;i<1300;i++)
					 {
						set_angel(current_angel);
						pid_yuntai_setting(4700);
					  pid_elevator_setting(-4800);
					  osDelay(1);
					 }
						for(int i=0;i<10;i++)
					 {
						pid_motor_setting_sudu(0,0,0,0);
						pid_elevator_setting_sudu(0);
						osDelay(1);
					 }
					 while(HAL_GPIO_ReadPin(b_m_GPIO_Port,b_m_Pin)!=1)
					 {
						 pid_motor_setting(1000,1000,1000,1000);
						 osDelay(1);
					 }
					}
				while(success!=1)
				{		
						pid_motor_setting_sudu(0,0,0,0);
						saomiao=1;
						if(s[2]!=0&&s[1]!=0)
						{
						success=1;					
						}
						if(time>4000)
						{
							success=1;
							for(int i=0;i<6;i++)
							{
								if(i<3)
								{
							   s[i]=i+1;
								}
								else if(i>2)
								{
									s[i]=i-2;
								}
						  }
						}
						else 
						{
						memcpy(&s,&openmvdate[0],6);
						pid_yuntai_setting(4700);
						openmv(1600);//摄像头对前		
//						pid_motor_setting_sudu(0,0,0,0);						
						osDelay(1);
						}
						time++;
				}
				HAL_GPIO_WritePin(deng_GPIO_Port,deng_Pin,GPIO_PIN_SET);//guan
				HAL_GPIO_WritePin(openmv_moshi_GPIO_Port,openmv_moshi_Pin,GPIO_PIN_RESET);
				if(saomiao==1&&chenggong==0)
				{
					while(HAL_GPIO_ReadPin(r_m_GPIO_Port,r_m_Pin)!=1)
					 {
						 pid_motor_setting(-1000,-1000,-1000,-1000);
						 osDelay(1);
					 }
					 for(int i=0;i<10;i++)
					 {
						 pid_motor_setting_sudu(0,0,0,0);
						 osDelay(1);
					 }
					 chenggong=1;
					 saomiao=3;
				}
				
			}
			 if(f_m_number==5)
			{
				zuoyi(1500);
				xjqian(1);
			}
		}
		
//前进一格进行搬运
		else if(f_m_number==6)
		{
		
		    HAL_GPIO_WritePin(deng_GPIO_Port,deng_Pin,GPIO_PIN_RESET);//kai
			HAL_GPIO_WritePin(openmv_moshi_GPIO_Port,openmv_moshi_Pin,GPIO_PIN_SET);
     for(int i=0;i<2500;i++)
      {
				pid_motor_setting(0,0,0,0);
				pid_elevator_setting(-500);
				osDelay(1);
			}
      for(int i=0;i<1500;i++)
      {
			openmv(1600);
				pid_elevator_setting_sudu(0);
			pid_yuntai_setting(4400);
			memcpy(&e[0],&openmvdate[6],3);
			osDelay(1);
			}   
			for(int i=0;i<3000;i++)
      {	
				pid_elevator_setting(-6000);			
				openmv(1470);
				pid_yuntai_setting(4400);
			 osDelay(1);
      }
			for(int i=0;i<2000;i++)
      {					
			openmv(1470);
			memcpy(&e[3],&openmvdate[6],3);
			osDelay(1);
			}
			for(int i=0;i<3000;i++)
      {	
				pid_elevator_setting(-500);
				zhuazi(1900);
				pid_yuntai_setting(4750);
				openmv(900)	;
				osDelay(1);
			}
			HAL_GPIO_WritePin(deng_GPIO_Port,deng_Pin,GPIO_PIN_SET);//guan
			HAL_GPIO_WritePin(openmv_moshi_GPIO_Port,openmv_moshi_Pin,GPIO_PIN_RESET);
			if(e[0]==0)
			{
					for(int i=0;i<6;i++)
							{
								if(i<3)
								{
							   e[i]=i+1;
								}
								else if(i>2)
								{
									e[i]=i-2;
								}
						  }
			}
			for(int i=0;i<3;i++)
      {
				for(int j=0;j<3;j++)
        {
					if(s[i] == e[j])
					{
						shunxu[i]=j;
					}
        }
      }
			for(int i=3;i<6;i++)
      {
				for(int j=3;j<6;j++)
        {
					if(s[i] == e[j])
					{
						shunxu[i]=j;
					}
        }
      }
		    jinyige_2();//前进一格
			 for(int z=0;z<5;z++)
			{
				pid_motor_setting_sudu(0,0,0,0);
				osDelay(1);
			}
			  wuliaotai(1245);
//			 for(int z=0;z<5;z++)
//			{
//				pid_motor_setting_sudu(0,0,0,0);
//				osDelay(1);
//			}
		   a[0]=1;//待定数据（发送数据给duoji.c文件执行搬运任务）
			f_m_number=f_m_number+1;
		}
		
		
		else if(a[0]==2&&f_m_number==7)//执行完搬运物料
		{
			houtui(3000);
			xjzuo(1);//后退到粗加工
			if(l_m_number==5)//后退3格，循迹加3
			{
				f_m_number=f_m_number+1;
				pid_motor_setting_sudu(0,0,0,0);
				osDelay(1);
			}
		}
		
//转弯
		else if(f_m_number==8)
		{
			for(int z=0;z<50;z++)
			{
				zhuazi(1200);
				pid_motor_setting_sudu(0,0,0,0);
				osDelay(1);			
			}
			for(int i=0;i<1500;i++)
			{
				pid_motor_setting(-1000,1000,1000,-1000);//先让循迹传感器转出黑线防止计数出现错误
				pid_elevator_setting(-500);
				zhuazi(1100);
				osDelay(1);
			}
			//识别到黑线停止转弯
			while(f_m_number!=9)
			{
			pid_elevator_setting_sudu(0);
			pid_motor_setting(-1000,1000,1000,-1000);
			xjqian(1);
			osDelay(1);
		  }
	  }
		
		//前进一格
		else if(f_m_number==9)
		{
		  jinyige();
			for(int i=0;i<10;i++)//防止过冲
				{
					pid_elevator_setting_sudu(0);
					pid_motor_setting_sudu(0,0,0,0);
					pid_yuntai_setting(4750);
					osDelay(1);
				}
				wuliaotai(1240);
		  f_m_number=f_m_number+1;
			a[0]=3;//开启放置（转到duoji任务）
		}
		
		
		//后退前进到达半成品区
		else if(a[0]==5&&f_m_number==10)
		{
			houtui(3000);
			xjzuo(1);
			if(l_m_number==8)
			{
				for(int i=0;i<1000;i++)
				{
					pid_motor_setting(-1000,1000,1000,-1000);
					osDelay(1);
				}
				while(f_m_number!=11)
				{
					pid_motor_setting(-1000,1000,1000,-1000);
					osDelay(1);
					xjqian(1);
				}
			}	
		}
		
    else if(f_m_number==11)
		{
			qianjin(3000);
			xjzuo(1);
			if(l_m_number==10)
			{
				f_m_number++;
			}    		
		}
		
		else if(f_m_number==12)
		{
			jinyige();
			for(int i=0;i<300;i++)//防止过冲
				{
					pid_elevator_setting_sudu(0);
					pid_motor_setting(0,0,0,0);
					zhuazi(1100);
					pid_yuntai_setting(4750);
					osDelay(1);
				}
		  f_m_number=f_m_number+1;
			a[0]=6;//开启放置（转到duoji任务）
		}
		
		//开始第二次搬运，开始回到第二次搬运起点
    if(a[0]==9&&f_m_number==13)  //此 a[0] 代表整个第一层工作完成，归为0（初始化）
	 {
		
		 houtui(3000);
		  xjzuo(1);
		 if(l_m_number==15)//?多了一次
		 {
				 for(int i=0;i<1500;i++)
				{
					pid_motor_setting(1000,-1000,-1000,1000);//先让循迹传感器转出黑线防止计数出现错误
					osDelay(1);
				}
				while(f_m_number<14)
		   {				 
			  pid_motor_setting(1000,-1000,-1000,1000);
        osDelay(1);
				xjqian(1);
		   }
		 }
	 }
	 
	 
	 //转弯
   else if(f_m_number==14)
	 {
		 xjzuo(1);
		 qianjin(3000);
		 if(l_m_number==18)//测试得到，转过来循迹多加了1
		 {
				for(int i=0;i<1500;i++)
				{
					pid_motor_setting(1000,-1000,-1000,1000);//先让循迹传感器转出黑线防止计数出现错误
					osDelay(1);
				}
				while(f_m_number<15)
		   {				 
			  pid_motor_setting(1000,-1000,-1000,1000);
        osDelay(1);
				xjqian(1);
		   }
		 }
	 }
	 
	 
	 //到达搬运地点前一格
	 else if(f_m_number==15)
	 {
		   for(int i=0;i<500;i++)
			{
				pid_motor_setting_sudu(0,0,0,0);
				zhuazi(1700);
				osDelay(1);
			}
			jinyige_2();
			 for(int i=0;i<200;i++)
      {
				pid_motor_setting_sudu(0,0,0,0);
				
				pid_elevator_setting_sudu(0);
				osDelay(1);
			}
		  f_m_number++;
		  a[1]=1;  //开始材料区的第二层抓取
	 }
	 
	 //后退到粗加工区
	 else if(a[1]==2&&f_m_number==16)//等候车体转移到粗加工区
		{
			xjzuo(1);
			houtui(3000);
			if(l_m_number==21)
			{
				f_m_number=f_m_number+1;
				pid_motor_setting_sudu(0,0,0,0);
				osDelay(1);
			}
		}
	 
	 else if(f_m_number==17)
	 {
		for(int i=0;i<1500;i++)
			{
				pid_motor_setting(-1000,1000,1000,-1000);//先让循迹传感器转出黑线防止计数出现错误
				osDelay(1);
			}
			//识别到黑线停止转弯
			while(f_m_number!=18)
			{
			pid_motor_setting(-1000,1000,1000,-1000);
			osDelay(1);
			xjqian(1);
		  }

	 }
	 
	 else if(f_m_number==18)
	 {
		  
			jinyige();
		  current_angel=JY62_record.Angle_z;
			for(int i=0;i<400;i++)//防止过冲
				{
					pid_motor_setting(0,0,0,0);
					pid_yuntai_setting(4750);
					zhuazi(1100);
					osDelay(1);
				}
		  f_m_number=f_m_number+1;
			a[1]=3;//开启放置（转到duoji任务）
	 }
	 
	 //第二次搬运到达半成品区              
	 
	 else if(a[1]==5&&f_m_number==19)
	 {
			houtui(3000);
				xjzuo(1);
				if(l_m_number==24)
				{
					for(int i=0;i<1000;i++)
					{
						pid_motor_setting(-1000,1000,1000,-1000);
						osDelay(1);
					}
					while(f_m_number!=20)
					{
						pid_motor_setting(-1000,1000,1000,-1000);
						osDelay(1);
						xjqian(1);
					}
				}	
			
	 }
	 else if(f_m_number==20)
	 {
		 	qianjin(3000);
			xjzuo(1);
			if(l_m_number==26)
			{
				f_m_number++;
			} 
	 }
	 
	 else if(f_m_number==21)
	 {
		 jinyige();
		 current_angel=JY62_record.Angle_z;
			for(int i=0;i<1000;i++)//防止过冲
				{
					pid_motor_setting(0,0,0,0);
					pid_yuntai_setting(4750);
					zhuazi(1200);
					osDelay(1);
				}
		  f_m_number=f_m_number+1;
			a[1]=6;//开启放置（转到duoji任务）
	 }
	 else if(a[1]==8&&f_m_number==22)
	 {
		pid_motor_setting(-2000,2000,-2000,2000);
    xjyou_l2(1);
    if(r_l2_number==5)
		{
			for(int i;i<1000;i++)
			{
			zuoyi(3000);
				osDelay(1);
			}
			f_m_number++;
		}			
	 }
   else if(b_m_number!=0)
	 {
		 pid_motor_setting(0,0,0,0);
		 osDelay(1);
	 }
	 else if(f_m_number==23)
	 {
		 pid_motor_setting(1000,1000,1000,1000);
		 xjhou(1);
	   osDelay(1);
	 }
	 else 
	 {
		 osDelay(1);
	 }
	  }
		osDelay(1);
 
	}
}
  /* USER CODE END chasis */

void wheelhandle(void)
{
	if(weizhi_cu==1)  //粗上位
	{
		if(s[shunxu_wei]==1)
		{
			while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
			{
				pid_motor_setting(500,500,500,500);
				osDelay(1);
			}
			for(int z=0;z<1300;z++)
            {
				pid_motor_setting(0,0,0,0);
				osDelay(1);
            }
		}
		else if(s[shunxu_wei]==2)
		{
			for(int z=0;z<1070;z++)
            {
				pid_motor_setting(1050,-1050,1000,-1000);
				osDelay(1);
            }
			while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
			{
				pid_motor_setting(500,500,500,500);
				osDelay(1);
			}
			for(int z=0;z<500;z++)
            {
				pid_motor_setting(0,0,0,0);
				osDelay(1);
            }
		}
		else if(s[shunxu_wei]==3)
		{
			for(int z=0;z<1070;z++)
            {
				pid_motor_setting(-1050,1050,-1000,1000);
				osDelay(1);
            }
			while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
			{
				pid_motor_setting(500,500,500,500);
				osDelay(1);
			}
			for(int z=0;z<500;z++)
            {
				pid_motor_setting(0,0,0,0);
				osDelay(1);
            }
		}
		shunxu_wei++;
		weizhi_cu=0;
		osDelay(1);
	}
	if(guiwei_work[1]==1)  //粗归位
	{
		if(weizhi==3)
		{
			for(int z=0;z<1200;z++)
            {
				pid_motor_setting(-500,-500,-500,-500);
				osDelay(1);
            }
			for(int z=0;z<1070;z++)
            {
				pid_motor_setting(1050,-1050,1000,-1000);
				osDelay(1);
            }
			for(int z=0;z<500;z++)
            {
				pid_motor_setting(0,0,0,0);
				osDelay(1);
            }
		}
		else if(weizhi==2)
		{
			for(int z=0;z<1200;z++)
            {
				pid_motor_setting(-500,-500,-500,-500);
				osDelay(1);
            }
			for(int z=0;z<1070;z++)
            {
				pid_motor_setting(-1050,1050,-1000,1000);
				osDelay(1);
            }
			for(int z=0;z<500;z++)
            {
				pid_motor_setting(0,0,0,0);
				osDelay(1);
            }
		}
		else if(weizhi==1)
		{
			for(int z=0;z<1200;z++)
            {
				pid_motor_setting(-500,-500,-500,-500);
				osDelay(1);
            }
			for(int z=0;z<500;z++)
            {
				pid_motor_setting(0,0,0,0);
				osDelay(1);
            }
		}
		if(weizhi_cu==0)
		{
			weizhi_cu=1;
		}
		if(shunxu_wei==-1)
		{
			tiqian_run=1;
		}
		guiwei_work[1]=0;
	}
}

//暂停操作
void clamdown()
{
	int i=10;
	while(i--)
	{
		pid_motor_setting(1000,1000,1000,1000);
		osDelay(1);
	}
}

//防止车轮发癫
void m_max_speed()
{
		for(int i=0;i<=3;i++)
    {
			if(__fabs(motor_chassis[i].speed_rpm) >max_speed)
			{
				max_speed =__fabs(motor_chassis[i].speed_rpm);
			}
    }
}

//前进一格的函数
void jinyige()
{
	for(int z=0;z<400;z++)
  {
		pid_elevator_setting_sudu(0);
		qianjin(500);
		osDelay(1);
  }
	while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
		 {
			qianjin(500);
			if(HAL_GPIO_ReadPin(f_l2_GPIO_Port,f_l2_Pin)!=1)
			{
				current_angel=JY62_record.Angle_z;
			}	
			osDelay(1);
		 }
}
void jinyige_2()
{
	for(int z=0;z<100;z++)
  {
		pid_elevator_setting_sudu(0);
		qianjin(1000);
		osDelay(1);
  }
	while(HAL_GPIO_ReadPin(f_l2_GPIO_Port,f_l2_Pin)!=1)
		 {
      qianjin(1000);
			if(HAL_GPIO_ReadPin(f_l2_GPIO_Port,f_l2_Pin)!=1)
			{
				current_angel=JY62_record.Angle_z;
			}	
			osDelay(1);
		 }
	
}

//6,0.00025,0.00001
//轮子和丝杆pid参数总体设定(0~3为底座电机，4位丝杆，in，out为云台)
void pid_main_init()
{
	  pid_init(&motor_pid_in[0]);
    motor_pid_in[0].f_param_init(&motor_pid_in[0],PID_Speed,2000,220,10,0,8000,0,6,1,0.00001);
	 	pid_init(&motor_pid_in[1]);
    motor_pid_in[1].f_param_init(&motor_pid_in[1],PID_Speed,2000,220,10,0,8000,0,6,1,0.00001);
	  pid_init(&motor_pid_in[2]);
    motor_pid_in[2].f_param_init(&motor_pid_in[2],PID_Speed,2000,220,10,0,8000,0,6,1,0.00001);
	 	pid_init(&motor_pid_in[3]);
    motor_pid_in[3].f_param_init(&motor_pid_in[3],PID_Speed,2000,220,10,0,8000,0,6,1,0.00001);
	
	  pid_init(&motor_pid_out[0]);
    motor_pid_out[0].f_param_init(&motor_pid_out[0],PID_Speed,2000,2000,30,0,8000,0,0.5,0.001,0);
	 	pid_init(&motor_pid_out[1]);
    motor_pid_out[1].f_param_init(&motor_pid_out[1],PID_Speed,2000,2000,30,0,8000,0,0.5,0.001,0);
	  pid_init(&motor_pid_out[2]);
    motor_pid_out[2].f_param_init(&motor_pid_out[2],PID_Speed,2000,2000,30,0,8000,0,0.5,0.001,0);
	 	pid_init(&motor_pid_out[3]);
    motor_pid_out[3].f_param_init(&motor_pid_out[3],PID_Speed,2000,2000,30,0,8000,0,0.5,0.001,0);
	
		pid_init(&out);
    out.f_param_init(&out,PID_Position,10000,10000,1,0,8000,0,3,0,0);//10,0,0
		pid_init(&in);
    in.f_param_init(&in,PID_Speed,10000,10000,1,0,8000,0,0.5,0,0);//5,0,0
	 
	  pid_init(&outgan);
    outgan.f_param_init(&outgan,PID_Position,20000,2000,30,0,8000,0,50,0.001,0);
		pid_init(&ingan);
    ingan.f_param_init(&ingan,PID_Speed,20000,2200,200,0,8000,0,4,0.0025,2);
		for(int i=0;i<200;i++)
		{
			pid_yuntai_setting(1200);//
			osDelay(1);
		}
    current_angel=JY62_record.Angle_z;	
		HAL_GPIO_WritePin(deng_GPIO_Port,deng_Pin,GPIO_PIN_SET);//guan
		HAL_GPIO_WritePin(openmv_moshi_GPIO_Port,openmv_moshi_Pin,GPIO_PIN_RESET);
}



//设定云台角度 
void pid_yuntai_setting(int16_t position)
{
  out.target = position;  
  out.f_cal_pid_2(&out,motor_chassis_b.ecd);  
//  in.target =  out.output;  
//  in.f_cal_pid(&in,motor_chassis_b.speed_rpm);  
  CAN_anter_chassis(out.output);  	
}

void pid_yuntai_setting_sudu(int16_t speed)
{
  in.target =  speed;  
  in.f_cal_pid(&in,motor_chassis_b.speed_rpm);  
  CAN_anter_chassis(-in.output);  	
}

//设定升降电机
void pid_elevator_setting(int16_t elevator1)
{
	  outgan.target = elevator1; 	
		outgan.f_cal_pid(&outgan,motor_chassis[6].out_really_angel);
	  ingan.target=outgan.output;
	  ingan.f_cal_pid(&ingan,motor_chassis[6].speed_rpm);
		CAN_sigan_chassis(ingan.output);
}

void pid_elevator_setting_sudu(int16_t sudu)
{  
	CAN_sigan_chassis(0);
	
}
void pid_elevator_setting_sudu_remote(int16_t sudu)
{ 
  ingan.target=sudu;
	ingan.f_cal_pid(&ingan,motor_chassis[6].speed_rpm);
	CAN_sigan_chassis(ingan.output);
}
void pid_motor_setting_sudu(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	CAN_cmd_chassis(0,0,0,0);	
}

//设定电机转速（内含pid计算以及CAN发送）
void pid_motor_setting(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{  
		//负值是正转
		motor_pid_in[0].target = motor1; 	
		motor_pid_in[0].f_cal_pid(&motor_pid_in[0],motor_chassis[0].speed_rpm);
	  motor_pid_in[1].target = motor2; 	
		motor_pid_in[1].f_cal_pid(&motor_pid_in[1],-motor_chassis[1].speed_rpm);
		motor_pid_in[2].target = motor3; 	
		motor_pid_in[2].f_cal_pid(&motor_pid_in[2],-motor_chassis[2].speed_rpm);
		motor_pid_in[3].target = motor4; 	
		motor_pid_in[3].f_cal_pid(&motor_pid_in[3],motor_chassis[3].speed_rpm);
	
		CAN_cmd_chassis(motor_pid_in[0].output,motor_pid_in[1].output,motor_pid_in[2].output,motor_pid_in[3].output);	
}

void pid_motor_setting_weizhi(int position)
{
	  motor_pid_out[0].target=position;
	  motor_pid_out[0].f_cal_pid(&motor_pid_out[0],motor_chassis[0].out_really_angel);
		motor_pid_in[0].target = motor_pid_out[0].output; 	
		motor_pid_in[0].f_cal_pid(&motor_pid_in[0],motor_chassis[0].speed_rpm);
	
	  motor_pid_out[1].target=-position;
	  motor_pid_out[1].f_cal_pid(&motor_pid_out[1],-motor_chassis[1].out_really_angel);
		motor_pid_in[1].target = motor_pid_out[1].output; 	
		motor_pid_in[1].f_cal_pid(&motor_pid_in[1],-motor_chassis[1].speed_rpm);
	
		motor_pid_out[2].target=-position;
	  motor_pid_out[2].f_cal_pid(&motor_pid_out[2],-motor_chassis[2].out_really_angel);
		motor_pid_in[2].target = motor_pid_out[2].output; 	
		motor_pid_in[2].f_cal_pid(&motor_pid_in[2],-motor_chassis[2].speed_rpm);
	
		motor_pid_out[3].target=position;
	  motor_pid_out[3].f_cal_pid(&motor_pid_out[3],motor_chassis[3].out_really_angel);
		motor_pid_in[3].target = motor_pid_out[3].output; 	
		motor_pid_in[3].f_cal_pid(&motor_pid_in[3],motor_chassis[3].speed_rpm);
	
		CAN_cmd_chassis(motor_pid_in[0].output,motor_pid_in[1].output,motor_pid_in[2].output,motor_pid_in[3].output);	
}


//CAN数据发送以及接收的集成（底盘电机）
void CAN_cmd_chassis(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
    uint32_t send_mail_box=0;
    chassis_tx_message.StdId = 0X200;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = -motor2 >> 8;
    chassis_can_send_data[3] = -motor2;
    chassis_can_send_data[4] = -motor3 >> 8;
    chassis_can_send_data[5] = -motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

//丝杆2006发送函数
void CAN_sigan_chassis(int16_t position)
{   
	 
    uint32_t send_mail_box=2;
    chassis_tx_message_1.StdId = 0x1FF;
    chassis_tx_message_1.IDE = CAN_ID_STD;
    chassis_tx_message_1.RTR = CAN_RTR_DATA;
    chassis_tx_message_1.DLC = 0x08;
    chassis_can_1_send_data[0] =0;
    chassis_can_1_send_data[1] =0;
    chassis_can_1_send_data[2] =0;
    chassis_can_1_send_data[3] =0;
    chassis_can_1_send_data[4] = position >> 8;
    chassis_can_1_send_data[5] = position;
    chassis_can_1_send_data[6] = 0;
    chassis_can_1_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message_1, chassis_can_1_send_data, &send_mail_box);
}

//云台数据发送和接收的集合
void CAN_anter_chassis(int16_t motor1)
{  
	CAN_TxHeaderTypeDef tx_header;
  uint8_t            tx_data[8];
  uint32_t send_mail_box=3;
  tx_header.StdId = 0x2FF;
  tx_header.IDE   = CAN_ID_STD;
  tx_header.RTR   = CAN_RTR_DATA;
  tx_header.DLC   = 8;

  tx_data[0] = (motor1>>8);
  tx_data[1] = (motor1);
  tx_data[2] = 0;
  tx_data[3] = 0;
  tx_data[4] = 0;
  tx_data[5] = 0;
  tx_data[6] = 0;
  tx_data[7] = 0;
  HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,&send_mail_box); 

}


// CAN数据接收回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
	  switch(rx_header.StdId){
		case 0x201:
		case 0x202:
		case 0x203:
		case 0x204:
		case 0x207:
		{
			static uint8_t i;
			i = rx_header.StdId - 0x201;
			get_motor_measure(&motor_chassis[i], rx_data);
			motor_chassis[i].really_ecd=(motor_chassis[i].ecd/8191.0)*360.0;
			motor_chassis[i].ecd_error=motor_chassis[i].ecd-motor_chassis[i].last_ecd;
			
			if(motor_chassis[i].speed_rpm>0)
			{
				if(motor_chassis[i].ecd_error < -1000)
				{
					motor_chassis[i].cyclinder_number++;
				}
				
			}
		  else if(motor_chassis[i].speed_rpm<0)
			{
				if(motor_chassis[i].ecd_error > 1000)
				{
					motor_chassis[i].cyclinder_number--;
				}
				
			}
			motor_chassis[i].out_angel=motor_chassis[i].ecd+motor_chassis[i].cyclinder_number*8191;
			motor_chassis[i].out_really_angel=motor_chassis[i].out_angel/(8191.0*36.0)*360.0;		
			break;
		}
		case 0x209://云台地址
		{
    	get_motor_measure(&motor_chassis_b,rx_data);
			break;
		}
		
		
	}
}



//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
//		osDelay(100);
//if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0)
//{
//	osDelay(10);
//	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0)
//		printf("按下\n");
//	  uint32_t test[64]={0};
//		uint8_t p[500];
//		printf("renwu");
//		osThreadList((uint8_t *)&p[500]);
//		printf("%s\r\n",p);
//		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0)
//		{
//		osDelay(10);
//		}
//}
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
//		k=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
//		osDelay(1);








//    if(l_m_number==0)
//		{						  
//			osThreadResume(xunji3Handle);
//			houtui();	
//			osThreadSuspend(xunji3Handle);
//		}
//		if(l_m_number>=2)
//		{
//			zuoyi();
//		}
//		
//    if(l_m_number==2)
//		{				
//			zuoyi();		
//			osThreadResume(xunji1Handle);			
//		}
//		
//		if(f_m_number==6)
//		{						
//     pid_motor_setting(0,0,0,0);
//			osDelay(10);

/*			
			qianjin();		
			osThreadResume(xunji4Handle);      		
			  while(r_m_number!=1)
			  {							  
			  osThreadSuspend(xunji4Handle);
		
			  houtui();	      				
			  osThreadResume(xunji3Handle);
				osDelay(1);
			  }
			  r_m_number=0;
				f_m_number=7;
		 }
		
		 if(l_m_number==5)
		{
			 osThreadSuspend(xunji3Handle);
			 for(int i;i<100;i++)
				{
					pid_motor_setting(-1000,1000,1000,-1000);
					osDelay(1);
				}
 			 osThreadResume(xunji1Handle);
       while(f_m_number!=8)
			 {
				pid_motor_setting(-1000,1000,1000,-1000);
				osDelay(1);
			 }
			 f_m_number=8;
			 osThreadSuspend(xunji1Handle);
		  }
		
		 if(f_m_number==8)
		 {			
			qianjin();
			
			osThreadResume(xunji4Handle);    
			 while(r_m_number!=1)
			  {
			   qianjin();
				 osDelay(1);
			   osThreadSuspend(xunji4Handle);		   			   
		   	}
				r_m_number=0;
				f_m_number=9;
		 }
		 
		 if(f_m_number==9)	
			{
				pid_motor_setting(0,0,0,0);
				
			}
		      				
			}
			*/
/*
  pid_motor_setting(3000,3000,3000,3000);
  osDelay(10);
*/



//int shaomiao=0;//用于判断二维码是否成功
//int fputc(int ch, FILE *f)
//	{
//uint8_t temp[1] = {ch};
//HAL_UART_Transmit(&huart1, temp, 1, 2);//huart1需要根据你的配置修改
//return ch;
//}
