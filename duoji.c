#include "duoji.h"
#include "gpio.h"
#include "tim.h"
#include "wheel.h"
#include "cmsis_os.h"
#include "can.h"
#include "pid.h"
#include "xunji.h"
#include "JY62.h"
//云台 m:4750 l:5370 r:3720
extern int shaomiao;
extern int a[2];//物料上层所用记忆数组
//测试数组
//物料台平放1200  起来：1700 颜色下层：-5500  1870  放物料2006：-2400
//-4100  7200
uint8_t s[6]={0};
uint8_t e[6]={0};
int set =0;
int zuoyi_time=0;
int youyi_time=0;
int bihe=870;
int dakai=1900;
int dakai_cu=1200;
int jingzhi_motor;
int first_elevator_take_high=-1200;
int second_elevator_take_high=-4500;
int set_wuliao_high=-4000;
int set_first_floor_high=4800;
int set_second_floor_high=4300;

int weizhi=0;
int weizhi_cu=-1;
extern int duoji_work[2]; //a
extern int guiwei_work[3];  //b
extern int qidong_work[3];  //c
extern int shunxu_wei;

extern float current_angel;
int o=0;
void duoji(void const * argument)
{

    pwm_init();
  for(;;)
  {		

 if(a[0]==1)//原料区抓取判断
	{
		
	  zhuaqu_yuan();
	  osDelay(1);
	}
	
	 if(a[0]==3)//放物料在粗加工区
	{	
		shunxu_wei=0;
	  fangzhi_cu_1();	
	  a[0]=4;				//作为抓取的标记符号
		osDelay(1);
	}
	if(a[0]==4)//抓取粗加工物料
	{
		for(int z=0;z<10;z++)
    {
		pid_elevator_setting_sudu(0);
		wuliaotai(1280);
    }
		for(int z=0;z<100;z++)
    {
		zhuazi(1700);
		  osDelay(1);
		}
		shunxu_wei=0;
		zhuaqu_cu(0);
		a[0]=5;//等待状态
		osDelay(1);
	}
	
	if(a[0] ==6) //放物料在成品区
	{
		wuliaotai(1240);
		fangzhi_cu_3(); 
		a[0]=8; 		//第一层工作结束，变为0初始化状态
		osDelay(1);
	}

	if(a[0] ==8) 
	{
		  for(int i=0;i<1000;i++)
      {
				pid_motor_setting(-1000,-1000,-1000,-1000);
			}
			
   	 for(int i=0;i<3000;i++)
      {
			pid_elevator_setting(-7400);
			openmv(900)	;
			pid_motor_setting_sudu(0,0,0,0);
			osDelay(1);
			} 
			a[0]=9;
			osDelay(1);
	}
	
	//第二层
	
	if(a[1]==1)  //原料区第二层抓取
	{
		zhuaqu_yuan_2();
		osDelay(1);
	}
	
	if(a[1]==3)  //放第二层物料在粗加工区
	{
		fangzhi_cu_2();
    a[1]=4;	
		osDelay(1);
	}
	
	if(a[1]==4)  //抓取粗加工后的物料
	{
		for(int z=0;z<100;z++)
    {
			zhuazi(1700);
		osDelay(1);
		}
		zhuaqu_cu(3);
		a[1]=5;
		osDelay(1);
	}
	
	//成品区
  if(a[1]==6)
	{
		fangzhi_cheng_2();
		a[1]=8; 
		osDelay(1);
	}
	else 
	{
		osDelay(1);
	}

	osDelay(1);		
}

  /* USER CODE END duoji */
}



//设置爪子舵机的函数
void zhuazi(int16_t pwm3)
{
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,pwm3);
}
//设置超控opmenmv的舵机函数

void openmv(int16_t pwm4)
{
__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,pwm4);
}
void wuliaotai(int16_t pwm2)
{
__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,pwm2);
}


void fangzhi(int yanse)
{
	if(yanse==1)//红放第1个
	{
		for(int z=0;z<10;z++)
		{
		pid_motor_setting_sudu(0,0,0,0);		
		osDelay(1);			
		}
		for(int z=0;z<1300;z++)
		{
		pid_yuntai_setting(1145);			
       osDelay(1);			
		}
		for(int z=0;z<500;z++)
		{
			pid_yuntai_setting(1145);
			openmv(1000)	;
			osDelay(1);
		}
		for(int z=0;z<800;z++)
		{		
		pid_yuntai_setting(1145);
		pid_elevator_setting(-1400);		//待定
    osDelay(1);			
		}
		for(int z=0;z<1000;z++)  //放篮子上
		{
			pid_elevator_setting_sudu(0);
			zhuazi(dakai_cu);
			osDelay(1);					 
		}
		for(int z=0;z<500;z++)
    {	
			pid_elevator_setting(-800);//回到高的位置
			osDelay(1);
			}			
		set=1;
	}
	
	else if(yanse==2)//绿放第2个
	{
		for(int z=0;z<10;z++)
		{
		pid_motor_setting_sudu(0,0,0,0);		
    osDelay(1);			
		}
		for(int z=0;z<1300;z++)
		{
		pid_yuntai_setting(660);			
    osDelay(1);			
		}
		for(int z=0;z<500;z++)
		{
			pid_yuntai_setting(660);
			openmv(1000)	;
			osDelay(1);
		}
		for(int z=0;z<800;z++)
		{		
			pid_yuntai_setting(660);
		pid_elevator_setting(-1400);		
    osDelay(1);			
		}
		for(int z=0;z<1000;z++)  //放篮子上
		{
			pid_elevator_setting_sudu(0);
			zhuazi(dakai_cu);
			osDelay(1);					 
		}
		for(int z=0;z<500;z++)
    {	
			pid_elevator_setting(-800);//回到高的位置
			osDelay(1);
			}			
	
		set=1;
	}
	
	else if(yanse==3)//蓝放第3个
	{
				for(int z=0;z<10;z++)
		{
		pid_motor_setting_sudu(0,0,0,0);		
    osDelay(1);			
		}
		for(int z=0;z<1300;z++)
		{
		pid_yuntai_setting(100);			
        osDelay(1);			
		}
		for(int z=0;z<800;z++)
		{
			pid_yuntai_setting(100);
			openmv(940)	;
			osDelay(1);
		}
		for(int z=0;z<800;z++)
		{		
		pid_yuntai_setting(100);
		pid_elevator_setting(-1400);		
    osDelay(1);			
		}
		for(int z=0;z<1000;z++)  //放篮子上
		{
			pid_elevator_setting_sudu(0);
			zhuazi(dakai_cu);
			osDelay(1);					 
		}
		for(int z=0;z<1000;z++)
    {	
			pid_elevator_setting(-500);//回到高的位置
			osDelay(1);
			}		
			set=1;
	}
}

void fangzhi_ercheng(int yanse)
{
	if(yanse==1)//红放第1个
	{
		for(int z=0;z<10;z++)
		{
		pid_motor_setting_sudu(0,0,0,0);		
    osDelay(1);			
		}
		for(int z=0;z<2500;z++)
		{		
		openmv(1600)	;
		pid_elevator_setting(-1800);		
		osDelay(1);			
		}
			for(int z=0;z<2000;z++)
		{	
			pid_elevator_setting_sudu(0);
			pid_yuntai_setting(1145);
			osDelay(1);
		}
			for(int z=0;z<1000;z++)
		{	
			pid_yuntai_setting(1145);
			openmv(1000)	;
			osDelay(1);
		}
		for(int z=0;z<1000;z++)  //放篮子上
		{
			zhuazi(1100);
			osDelay(1);					 
		}	
//		for(int z=0;z<800;z++)
//		{
//		 pid_motor_setting(500,500,500,500);//qianjin
//			osDelay(1);
//		}		
		
		set=1;
	}
	
	else if(yanse==2)//绿放第2个
	{
		for(int z=0;z<10;z++)
		{
		pid_motor_setting_sudu(0,0,0,0);		
    osDelay(1);			
		}
		for(int z=0;z<2500;z++)
		{	
		openmv(1600)	;			
		pid_elevator_setting(-1500);		
    osDelay(1);			
		}
			for(int z=0;z<2000;z++)
		{	
			pid_elevator_setting_sudu(0);
			pid_yuntai_setting(660);
			osDelay(1);
		}
			for(int z=0;z<1000;z++)
		{		
      pid_yuntai_setting(660);			
			openmv(1000)	;
			osDelay(1);
		}
		for(int z=0;z<1000;z++)  //放篮子上
		{

			zhuazi(1100);
			osDelay(1);					 
		}
			
	
		set=1;
	}
	
	else if(yanse==3)//蓝放第3个
	{
				for(int z=0;z<10;z++)
		{
		pid_motor_setting_sudu(0,0,0,0);		
    osDelay(1);			
		}
	
		for(int z=0;z<2500;z++)
		{		
			openmv(1600)	;
		pid_elevator_setting(-1500);		
		osDelay(1);			
		}
			for(int z=0;z<2000;z++)
		{	
			pid_elevator_setting_sudu(0);
			pid_yuntai_setting(100);
			osDelay(1);
		}
			for(int z=0;z<1000;z++)
		{	
			pid_yuntai_setting(100);
			openmv(1000)	;
			osDelay(1);
		}
		for(int z=0;z<1000;z++)  //放篮子上
		{
			zhuazi(1100);
			osDelay(1);					 
		}
			set=1;
	}
}

void guiwei(int weizhi)
{
  if(weizhi==0)
	{
			for(int z=0;z<1000;z++)
     {
			 openmv(900);
			 pid_elevator_setting(-800);//回到高的位置
			 zhuazi(dakai);
			 pid_yuntai_setting(4750);
		   osDelay(1);
      }
		 for(int z=0;z<1070;z++)
     {
		     zhuazi(dakai);
			 pid_elevator_setting_sudu(0);
			 pid_motor_setting(1050,-1050,1000,-1000);	//youyi
		   osDelay(1);
      }
		 for(int z=0;z<10;z++)
    {	
		  pid_elevator_setting_sudu(0);    
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
	}
	
	else if(weizhi==2)
	{
				for(int z=0;z<1000;z++)
     {
			 openmv(900);
			 pid_elevator_setting(-800);//回到高的位置
			 zhuazi(dakai);
			 pid_yuntai_setting(4750);
		   osDelay(1);
      }
		 for(int z=0;z<1070;z++)
     {
		  zhuazi(dakai);
			 pid_elevator_setting_sudu(0);
			 pid_motor_setting(-1050,1050,-1000,1000);	//youyi
		   osDelay(1);
      }
		 for(int z=0;z<10;z++)
    {	
		  pid_elevator_setting_sudu(0);    
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
  }
	
	else if(weizhi==1)
	{
				for(int z=0;z<1000;z++)
     {
			openmv(900);
			 pid_elevator_setting(-800);//回到高的位置
			 zhuazi(dakai);
			 pid_yuntai_setting(4750);
		   osDelay(1);
      }
		 for(int z=0;z<10;z++)
    {	
		  zhuazi(dakai);
		  pid_elevator_setting_sudu(0);    
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
		 for(int z=0;z<10;z++)
    {	  
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
	}
}

void guiwei_ercheng(int weizhi)
{
  if(weizhi==0)
	{
			for(int z=0;z<1400;z++)
     {
			 openmv(900);
			 zhuazi(dakai);
		  pid_elevator_setting(-800);//回到高的位置
			 pid_yuntai_setting(4750);
		   osDelay(1);
      }
		 for(int z=0;z<2300;z++)
     {
		    zhuazi(dakai);
			 pid_elevator_setting_sudu(0);
			 pid_motor_setting(550,-550,500,-500);	//youyi
		   osDelay(1);
      }
		 for(int z=0;z<100;z++)
    {	
		  pid_elevator_setting_sudu(0);    
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
	}
	
	else if(weizhi==2)
	{
				for(int z=0;z<1400;z++)
     {
			 openmv(900);
		  pid_elevator_setting(-800);//回到高的位置
			 zhuazi(dakai);
			 pid_yuntai_setting(4750);
		   osDelay(1);
      }
		 for(int z=0;z<2300;z++)
     {
		 zhuazi(dakai);
			 pid_elevator_setting_sudu(0);
			 pid_motor_setting(-550,550,-500,500);	//youyi
		   osDelay(1);
      }
		 for(int z=0;z<100;z++)
    {	
		  pid_elevator_setting_sudu(0);    
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
  }
	
	else if(weizhi==1)
	{
				for(int z=0;z<1400;z++)
     {
			 openmv(900);
		  pid_elevator_setting(-800);//回到高的位置
			 zhuazi(dakai);
			 pid_yuntai_setting(4750);
		   osDelay(1);
      }
		 for(int z=0;z<100;z++)
    {	
		zhuazi(dakai);
		  pid_elevator_setting_sudu(0);    
			pid_motor_setting_sudu(0,0,0,0);
		  osDelay(1);
     }
	}
}
//第二层抓取
void zhuaqu_yuan_2()
{
	for(int i=3;i<6;i++)
	 {
		 for(int j=3;j<6;j++)
	  {
     	if(s[i]==e[j])//s[i]为抓取顺序，e【j】为摆放颜色顺序
     	{ 
			  set=0;
     	 switch(j-3)
     	 {
     	 	  case 0:
					{
					for(int z=0;z<2500;z++)
					{
					zhuazi(1700);
					pid_elevator_setting(-7700);
					osDelay(1);
					}
					for(int z=0;z<2300;z++)
					{
						pid_elevator_setting_sudu(0);
						pid_motor_setting(-550,550,-500,500);	//zuoyi
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//执行搬运
						for(int z=0;z<2000;z++)
					{		
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(4750);
						osDelay(1);
					}
				
					for(int z=0;z<1500;z++)
					{
						set_angel(current_angel);   //调整角度
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
					  zhuazi(bihe);
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
					{
					  pid_motor_setting(-700,-700,-700,-700);//houtui
						osDelay(1);
					}
     	 	  break;
				 }
					
				
					
			  case 1:
				{
						for(int z=0;z<2500;z++)
					{
					zhuazi(1700);
					pid_elevator_setting(-7700);
					osDelay(1);
					}
			   while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_elevator_setting_sudu(0);
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//执行搬运
						for(int z=0;z<2000;z++)
					{		
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(4750);
						osDelay(1);
					}
				
					for(int z=0;z<1500;z++)
					{
						pid_elevator_setting_sudu(0);
						set_angel(current_angel);   //调整角度
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
					  zhuazi(bihe);
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
					{
					  pid_motor_setting(-700,-700,-700,-700);//houtui
						osDelay(1);
					}
					break;
					}
					
					
			  case 2:
				{
					for(int z=0;z<2500;z++)
					{
					zhuazi(1700);
					pid_elevator_setting(-7700);
					osDelay(1);
					}
					for(int z=0;z<2300;z++)
					{
						pid_elevator_setting_sudu(0);
						pid_motor_setting(550,-550,500,-500);	//youyi
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//					
					
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//执行搬运
						for(int z=0;z<2000;z++)
					{		
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(4750);
						osDelay(1);
					}
					
					for(int z=0;z<1500;z++)
					{
						pid_elevator_setting_sudu(0);
						set_angel(current_angel);   //调整角度
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
					  pid_motor_setting_sudu(0,0,0,0);
					  zhuazi(bihe);
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
					{
					  pid_motor_setting(-700,-700,-700,-700);//houtui
						osDelay(1);
					}
					break;	
				}				
		  }
			 while(set==0)//执行完跳出
				{
					fangzhi_ercheng(e[j]);//放置判断函数
					osDelay(1);						
				}
			guiwei_ercheng(j-3);
			osDelay(1);
		 }
       
		}
	 }
	 a[1]=2;  //传回wheel里执行出发去粗加工区
}

void zhuaqu_yuan()
{
	for(int i=0;i<3;i++)
	 {
		 for(int j=0;j<3;j++)
	  {
     	if(s[i]==e[j])//s[i]为抓取顺序，e【j】为摆放颜色顺序
     	{ 
				set=0;
     	 switch(j)
     	 {

     	 	  case 0:
					{

						//先后退防止打到物料，然后左移，前进
//					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
						for(int z=0;z<10;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						osDelay(1);
					}
					for(int z=0;z<1070;z++)
					{
						zhuazi(1900);
						pid_motor_setting(-1050,1050,-1000,1000);	//zuoyi
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(800,800,800,800);
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//执行搬运
						for(int z=0;z<5;z++)
					{		
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(4750);
						osDelay(1);
					}
						for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(4750);
						set_angel(current_angel);   //调整角度
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
					set_angel(current_angel);
					pid_yuntai_setting(4750);
					pid_elevator_setting(-700);
					osDelay(1);
					}
					for(int z=0;z<500;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						pid_elevator_setting_sudu(0);
					  zhuazi(bihe);
						osDelay(1);
					}
					for(int z=0;z<1200;z++)
					{
						openmv(1600)	;
					  pid_motor_setting(-500,-500,-500,-500);//houtui
						osDelay(1);
					}
     	 	  break;
				 }
					
				
					
			  case 1:
				{

					//后退一点防止打到物料
//					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)==1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
						for(int z=0;z<10;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						osDelay(1);
					}
			   while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						zhuazi(1900);
						pid_motor_setting(800,800,800,800);
						osDelay(1);
					}
						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//执行搬运
						for(int z=0;z<5;z++)
					{		
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(4750);
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(4750);
						set_angel(current_angel);   //调整角度
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
					set_angel(current_angel);
					pid_yuntai_setting(4750);
					pid_elevator_setting(-700);
					osDelay(1);
					}
					for(int z=0;z<500;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						pid_elevator_setting_sudu(0);
					  zhuazi(bihe);
						osDelay(1);
					}
					for(int z=0;z<1200;z++)
					{
						openmv(1600)	;
					  pid_motor_setting(-500,-500,-500,-500);//houtui
						osDelay(1);
					}
					
					break;
					}
					
					
			  case 2:
				{

//					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
						for(int z=0;z<10;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						osDelay(1);
					}
					for(int z=0;z<1070;z++)
					{
						zhuazi(1900);
						pid_elevator_setting_sudu(0);
						pid_motor_setting(1050,-1050,1000,-1000);	//youyi
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(800,800,800,800);
						osDelay(1);
					}
//					
					
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
					{
						pid_motor_setting(300,300,300,300);
						osDelay(1);
					}
//执行搬运
						for(int z=0;z<5;z++)
					{		
						
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(4750);
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(4750);
						set_angel(current_angel);   //调整角度
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
					set_angel(current_angel); 
					pid_yuntai_setting(4750);
					pid_elevator_setting(-700);
					osDelay(1);
					}
					for(int z=0;z<500;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						pid_elevator_setting_sudu(0);
					  zhuazi(bihe);
						osDelay(1);
					}
					for(int z=0;z<1200;z++)
					{
						openmv(1600)	;
					  pid_motor_setting(-500,-500,-500,-500);//houtui
						osDelay(1);
					}
					break;	
				}				
		  }
			 while(set==0)//执行完跳出
				{
					fangzhi(e[j]);//放置判断函数
          osDelay(1);						
				}
			guiwei(j);
			osDelay(1);
		 }     
		}
	 }
	 a[0]=2;  //传回wheel里执行出发去粗加工区
}



void fangzhi_cu_1()  
{
	for(int i=0;i<3;i++)
	{
		if(weizhi_cu==-1)
		{
			weizhi_cu=1;
		}
		switch(s[i])
		{
			//红色
			case 1:
			{
				
				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}			
				for(int z=0;z<1300;z++)
				{
					pid_yuntai_setting(1145);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)//2000
				{
					pid_yuntai_setting(1145);
					pid_elevator_setting(-2400);
					osDelay(1);
				}
				for(int z=0;z<500;z++)
				{	
					pid_yuntai_setting(1145);								
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
					for(int z=0;z<500;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
					osDelay(1);
				}
				for(int z=0;z<1500;z++)
				{
					pid_elevator_setting_sudu(0);
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				while(weizhi_cu==1)
				{
					osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					openmv(930)	;
					pid_yuntai_setting(4750);
					pid_elevator_setting(-6400);//-4800		
					osDelay(1);
					}
				for(int z=0;z<700;z++)//500
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
				for(int z=0;z<500;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
					}
				for(int z=0;z<2300;z++)
        {
					openmv(930);
					zhuazi(1100);
					pid_elevator_setting(-800);
					osDelay(1);
          }
				for(int z=0;z<5;z++)
        {
					pid_elevator_setting_sudu(0);
					osDelay(1);
          }
					break;
			}
			
			case 2:
			{
				//绿色
					for(int z=0;z<10;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
					}
				for(int z=0;z<1500;z++)
				{	
					pid_yuntai_setting(660);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
        {
					pid_yuntai_setting(660);
					pid_elevator_setting(-2400);			
					osDelay(1);
        }
				for(int z=0;z<500;z++)
				{
					pid_yuntai_setting(660);
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
				for(int z=0;z<500;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<1500;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				while(weizhi_cu==1)
				{
					osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					openmv(930)	;
					pid_yuntai_setting(4750);
					pid_elevator_setting(-6400);//-4800
					osDelay(1);
				}
					for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
				for(int z=0;z<500;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
				}
					for(int z=0;z<2300;z++)
        {
					openmv(930);
					zhuazi(1100);
					pid_elevator_setting(-800);
					osDelay(1);
         }
				for(int z=0;z<5;z++)
        {
					pid_elevator_setting_sudu(0);
					osDelay(1);
         }
				break;
			}
			
			
			case 3:
			{
				//蓝色			

				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
					for(int z=0;z<1300;z++)
					{
						pid_yuntai_setting(100);
					  osDelay(1);			
					}
					for(int z=0;z<1200;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting(-2400);		
						osDelay(1);
					}
					for(int z=0;z<700;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting_sudu(0);
						zhuazi(bihe);
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_elevator_setting(-800);
						osDelay(1);
					}
						for(int z=0;z<500;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<1500;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				while(weizhi_cu==1)
				{
					osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					openmv(900)	;
					pid_yuntai_setting(4750);
					pid_elevator_setting(-6400);//-4800
					osDelay(1);
				}
					for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
					for(int z=0;z<1000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						zhuazi(dakai);
						osDelay(1);
					}
					for(int z=0;z<2300;z++)
        {
					openmv(930);
					zhuazi(1100);
					pid_elevator_setting(-800);
					osDelay(1);
         }
				for(int z=0;z<5;z++)
        {
					pid_elevator_setting_sudu(0);
					osDelay(1);
         }
					break;
				}
		}
		if(i==2)
		{
			weizhi_cu=-1;
		}
		weizhi=s[i];
		guiwei_work[1]=1;
		//guiwei_cu(s[i],1100);
    osDelay(1);
	}	
}
void fangzhi_cu_3()  
{
	for(int i=0;i<3;i++)
	{
		switch(s[i])
		{
			//红色
			case 1:
			{
				
				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
	      while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
				{
						pid_motor_setting(-500,-500,-500,-500);
						osDelay(1);
				}		
				for(int z=0;z<2200;z++)
				{
				pid_motor_setting(0,0,0,0);
					pid_yuntai_setting(1145);
					osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(1145);
					pid_elevator_setting(-2400);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{	
					pid_yuntai_setting(1145);					
          pid_motor_setting_sudu(0,0,0,0);					
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
					for(int z=0;z<500;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
					osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_elevator_setting_sudu(0);
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<2500;z++)
				{
					openmv(930)	;
					pid_yuntai_setting(4750);
					pid_elevator_setting(-4800);		
					osDelay(1);
					}
				for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
					}
			
					break;
			}
			
			case 2:
			{
				//绿色
			
					for(int z=0;z<10;z++)
					{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
					}
					for(int z=0;z<1070;z++)
					{
						pid_motor_setting(1050,-1050,1000,-1000);	//youyi
						osDelay(1);
					}
						for(int z=0;z<300;z++)
				{
					set_angel(current_angel);
					osDelay(1);
				}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(500,500,500,500);
						osDelay(1);
					}
				for(int z=0;z<2000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					osDelay(1);
					pid_yuntai_setting(660);
				}
				for(int z=0;z<2000;z++)
        {
					pid_yuntai_setting(660);
					pid_elevator_setting(-2400);			
					osDelay(1);
        }
				for(int z=0;z<1000;z++)
				{
					pid_yuntai_setting(660);
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<2500;z++)
				{
					openmv(930)	;
					pid_yuntai_setting(4750);
					pid_elevator_setting(-4800);
					osDelay(1);
					}
					for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
				}
	
				break;
			}
			
			
			case 3:
			{
				//蓝色			
				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
					for(int z=0;z<1070;z++)
					{
						pid_motor_setting(-1050,1050,-1000,1000);	//youyi
						osDelay(1);
						
					}
					
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(-500,-500,-500,-500);
						osDelay(1);
					}
//						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}
					for(int z=0;z<2000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(100);
					  osDelay(1);			
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting(-2400);		
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting_sudu(0);
						zhuazi(bihe);
						osDelay(1);
					}
				
					for(int z=0;z<1500;z++)
					{
						pid_elevator_setting(-800);
						osDelay(1);
					}
						for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<2500;z++)
				{
					openmv(900)	;
					pid_yuntai_setting(4750);
					pid_elevator_setting(-4800);
					osDelay(1);
					}
					for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
//				for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
					for(int z=0;z<1000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						zhuazi(dakai);
						osDelay(1);
					}	
					break;
				}
		}
		guiwei_cu(s[i],1100);
    osDelay(1);
	}	
}
void fangzhi_cu_2()
{
	for(int i=3;i<6;i++)
	{
		switch(s[i])
		{
			//红色
			case 1:
			{
				
				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
	          while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
				{
						pid_motor_setting(-500,-500,-500,-500);
						osDelay(1);
				}
//				while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}				
				for(int z=0;z<2200;z++)
				{
				pid_motor_setting(0,0,0,0);
					pid_yuntai_setting(1145);
					osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(1145);
					pid_elevator_setting(-2400);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{	
					pid_yuntai_setting(1145);					
          pid_motor_setting_sudu(0,0,0,0);					
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
					for(int z=0;z<500;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
					osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_elevator_setting_sudu(0);
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<2500;z++)
				{
					pid_yuntai_setting(4750);
					openmv(930)	;
					pid_elevator_setting(-6400);		
					osDelay(1);
					}
				for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
//		
//				for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
				for(int z=0;z<1000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
					}
			
					break;
			}
			
			case 2:
			{
				//绿色
			
//					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
					for(int z=0;z<10;z++)
					{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
					}
					for(int z=0;z<2300;z++)
					{
						pid_motor_setting(550,-550,500,-500);	//youyi
						osDelay(1);
					}
						for(int z=0;z<300;z++)
				{
					set_angel(current_angel);
					osDelay(1);
				}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(500,500,500,500);
						osDelay(1);
					}
//						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}
				for(int z=0;z<2000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					osDelay(1);
					pid_yuntai_setting(660);
				}
				for(int z=0;z<2000;z++)
        {
					pid_yuntai_setting(660);
					pid_elevator_setting(-2400);			
					osDelay(1);
        }
				for(int z=0;z<1000;z++)
				{
					pid_yuntai_setting(660);
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<2500;z++)
				{
					pid_yuntai_setting(4750);
					openmv(930)	;
					pid_elevator_setting(-6400);
					osDelay(1);
					}
					for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
//				for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
				for(int z=0;z<1000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
				}
	
				break;
			}
			
			
			case 3:
			{
				//蓝色			

				 //先后退防止打到物料，然后左移，前进
//					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
					for(int z=0;z<2300;z++)
					{
						pid_motor_setting(-550,550,-500,500);	//youyi
						osDelay(1);
						
					}
					
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(-500,-500,-500,-500);
						osDelay(1);
					}
//						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}
					for(int z=0;z<2000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(100);
					  osDelay(1);			
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting(-2400);		
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting_sudu(0);
						zhuazi(bihe);
						osDelay(1);
					}
				
					for(int z=0;z<1500;z++)
					{
						pid_elevator_setting(-800);
						osDelay(1);
					}
						for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<2500;z++)
				{
					pid_yuntai_setting(4750);
					openmv(900)	;
					pid_elevator_setting(-6400);
					osDelay(1);
					}
					for(int z=0;z<700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
					osDelay(1);
				}
//				for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
					for(int z=0;z<1000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						zhuazi(dakai);
						osDelay(1);
					}	
					break;
				}
		}
		guiwei_cu(s[i],1100);
        osDelay(1);
	}	
}
void zhuaqu_cu(int cengci)  //粗加工区的抓取（依照抓取颜色次序进行）
{
	for(int i=cengci;i<cengci+3;i++)
	{
		if(weizhi_cu == -1)
		{
			weizhi_cu =1;
		}
		switch(s[i])
		{//红色
			case 1:
			{
				 for(int z=20;z>0;z--)
				{
					pid_elevator_setting_sudu(0);
					pid_motor_setting_sudu(0,0,0,0);
					osDelay(1);
				}
				for(int z=0;z<1500;z++)
				{
				pid_yuntai_setting(4750);
				osDelay(1);
				}
				while(weizhi_cu ==1)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
				}
				while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
				{
					pid_motor_setting(500,500,500,500);
						osDelay(1);
				}
				while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
				{
					pid_motor_setting(-500,-500,-500,-500);
					osDelay(1);
				}
				for(int z=0;z<100;z++)
				{
					pid_motor_setting(500,500,500,500);
					osDelay(1);
				}
				for(int z=0;z<700;z++)
				{
				zhuazi(dakai);
				openmv(900);
				set_angel(current_angel);
				osDelay(1);
				}
				for(int z=0;z<2200;z++)
				{
				pid_motor_setting_sudu(0,0,0,0);
				pid_elevator_setting(-6850);
				osDelay(1);
				}
				for(int z=0;z<500;z++)
				{
				pid_elevator_setting_sudu(0);
				pid_motor_setting_sudu(0,0,0,0);
				zhuazi(bihe);
				osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
				pid_elevator_setting(-900);
				openmv(1600);
				osDelay(1);
				}
				weizhi=s[i];
				if(i==cengci+2)
				{
					shunxu_wei=-1;
				}
				for(int z=0;z<2500;z++)
        {
					pid_elevator_setting_sudu(0);
					pid_yuntai_setting(1145);
					osDelay(1);
        }
				guiwei_work[1]=1;
				for(int z=0;z<700;z++)
        {
					pid_yuntai_setting(1145);
				 openmv(900);
				osDelay(1);
				}
//				for(int z=0;z<1000;z++)
//        {
//					pid_yuntai_setting(1145);
//					openmv(900);
//					pid_elevator_setting(-1400);
//					osDelay(1);
//        }
				for(int z=0;z<600;z++)
				{
					pid_elevator_setting_sudu(0);
					zhuazi(1100);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_elevator_setting(-500);
					osDelay(1);
				}
				break;
			}
			
			case 2:
			{//绿色
		
		    for(int z=20;z>0;z--)
				{
					pid_elevator_setting_sudu(0);
					pid_motor_setting_sudu(0,0,0,0);
					osDelay(1);
				}
				
				for(int z=1500;z>0;z--)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
				}
				while(weizhi_cu ==1)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
				}
				 while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
				{
					pid_motor_setting(500,500,500,500);
						osDelay(1);
				}
				while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
				{
					pid_motor_setting(-500,-500,-500,-500);
					osDelay(1);
				}
				for(int z=0;z<100;z++)
				{
					pid_motor_setting(500,500,500,500);
					osDelay(1);
				}
					for(int z=0;z<1000;z++)
				{
					zhuazi(dakai);
					openmv(900);
					pid_elevator_setting_sudu(0);
				set_angel(current_angel);
				osDelay(1);
				}
				
				for(int z=0;z<2200;z++)
				{
				pid_motor_setting_sudu(0,0,0,0);
				pid_elevator_setting(-6850);
				osDelay(1);
				}
				for(int z=0;z<500;z++)
				{
				pid_motor_setting_sudu(0,0,0,0);
				zhuazi(bihe);
				osDelay(1);
				}
				for(int z=2500;z>0;z--)
				{
					openmv(1600)	;
					pid_elevator_setting(-900);
					osDelay(1);		
				}
				weizhi=s[i];
				if(i==cengci+2)
				{
					shunxu_wei=-1;
				}
				for(int z=0;z<2500;z++)
        {
					pid_elevator_setting_sudu(0);
					pid_yuntai_setting(660);
					osDelay(1);
        }
				guiwei_work[1]=1;
				for(int z=0;z<700;z++)
        {
					pid_yuntai_setting(660);
				openmv(900);
				osDelay(1);
				}
//				for(int z=0;z<1500;z++)
//        {
//					pid_yuntai_setting(660);
//					openmv(900);
//					pid_elevator_setting(-1400);
//					osDelay(1);
//        }
				for(int z=0;z<600;z++)
				{
					pid_elevator_setting_sudu(0);
					zhuazi(1100);
					osDelay(1);
				}
					for(int z=0;z<1000;z++)
				{
					pid_elevator_setting(-700);
					osDelay(1);
				}
				break;
			}
			//蓝色
			case 3:
			{
				  for(int z=20;z>0;z--)
				{
					pid_elevator_setting_sudu(0);
					pid_motor_setting_sudu(0,0,0,0);
					osDelay(1);
				}
				for(int z=1500;z>0;z--)
				{
//					openmv(900);
					pid_yuntai_setting(4750);
					osDelay(1);
				}
				while(weizhi_cu ==1)
				{
					pid_yuntai_setting(4700);
					osDelay(1);
				}
				 while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
				{
					pid_motor_setting(500,500,500,500);
						osDelay(1);
				}
				while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
				{
					pid_motor_setting(-500,-500,-500,-500);
					osDelay(1);
				}
				for(int z=0;z<100;z++)
				{
					pid_motor_setting(500,500,500,500);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					zhuazi(dakai);
					pid_elevator_setting_sudu(0);
				  set_angel(current_angel);
					osDelay(1);
				}
				for(int z=0;z<2200;z++)
				{
				pid_motor_setting(0,0,0,0);
				pid_elevator_setting(-6850);
				osDelay(1);
				}
				for(int z=0;z<500;z++)
				{
				pid_motor_setting(0,0,0,0);
				zhuazi(bihe);
				osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
				openmv(1600)	;
				pid_elevator_setting(-900);
				osDelay(1);
				}
				weizhi=s[i];
				if(i==cengci+2)
				{
					shunxu_wei=-1;
				}
				for(int z=0;z<2500;z++)
        {
					pid_elevator_setting_sudu(0);
					pid_yuntai_setting(100);
					osDelay(1);
        }
				guiwei_work[1]=1;
				for(int z=0;z<700;z++)
        {
				pid_yuntai_setting(100);
				openmv(900);
				osDelay(1);
				}
				for(int z=0;z<600;z++)
				{
					pid_elevator_setting_sudu(0);
					zhuazi(1100);
					osDelay(1);
				}
					for(int z=0;z<1000;z++)
				{
					pid_elevator_setting(-700);
					osDelay(1);
				}
				break;
			}
		}
		if(i == 2||i ==5)
		{
			weizhi_cu =-1;
		}
	}

}

void guiwei_cu(int weizhi,int dakai1)  //在粗加工区抓取完成后，回到红色中心位置
{
	if(weizhi==3) //蓝色（左边） 
	{
		for(int z=0;z<1070;z++)
    {
			pid_motor_setting(1050,-1050,1000,-1000);	//youyi	//youyi	
			openmv(930);
			pid_elevator_setting(-800);//回到高的位置
			osDelay(1);
			}	
     for(int z=0;z<500;z++)
     {		
			 pid_elevator_setting_sudu(0);		 
			 pid_motor_setting_sudu(0,0,0,0);	
			 zhuazi(dakai1);
			 osDelay(1);
      }			
	}
	else if(weizhi==2)  //绿色 （右边）
	{
		for(int z=0;z<1070;z++)
		{
			openmv(930);
			pid_motor_setting(-1050,1050,-1000,1000);	//youyi	//zouyi		
			pid_elevator_setting(-800);//回到高的位置	
			osDelay(1);			
		}
	  for(int z=0;z<500;z++)
    {	
			pid_motor_setting_sudu(0,0,0,0);	
			pid_elevator_setting_sudu(0);
			zhuazi(dakai1);
		  osDelay(1);
    }
  }
	else if(weizhi==1)  //红色（中间）
	{
		 for(int z=0;z<2300;z++)
    {	
			openmv(930);
			pid_elevator_setting(-800);//回到高的位置	 			
			pid_motor_setting_sudu(0,0,0,0);	
			osDelay(1);	
     }	
		 for(int z=0;z<500;z++)
    {		
		  pid_elevator_setting_sudu(0);;//回到高的位置	    
			zhuazi(dakai1);
		  osDelay(1);
     }	
	}
}


void fangzhi_cheng_2()  
{
	for(int i=3;i<6;i++)
	{
		switch(s[i])
		{
			//红色
			case 1:
			{
				
				for(int z=0;z<10;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_elevator_setting_sudu(0);
					osDelay(1);
				}
	      while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(-550,-550,-500,-500);
						osDelay(1);
					}
//						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}				
				for(int z=0;z<2000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					pid_yuntai_setting(1145);
					osDelay(1);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(1145);
					pid_elevator_setting(-2400);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{							
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
					for(int z=0;z<500;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
					osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<1700;z++)
				{
					openmv(900)	;
					pid_elevator_setting(-3900);		
					osDelay(1);
					}
				for(int z=0;z<1700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
				}
//					for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
				for(int z=0;z<1000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
					}
			
					break;
			}
			
			case 2:
			{
				//绿色
			
//					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
					for(int z=0;z<2300;z++)
					{
						pid_motor_setting(550,-550,500,-500);	//youyi
						osDelay(1);
					}
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(500,500,500,500);
						osDelay(1);
					}
//						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}
				for(int z=0;z<2000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					osDelay(1);
					pid_yuntai_setting(660);
				}
				for(int z=0;z<2000;z++)
				{
					pid_yuntai_setting(660);
					pid_elevator_setting(-2400);			
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					zhuazi(bihe);
					osDelay(1);
				}
				
				for(int z=0;z<1200;z++)
				{
					pid_elevator_setting(-800);
					osDelay(1);
				}
				for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<1700;z++)
				{
					pid_yuntai_setting(4750);
					openmv(900)	;
					pid_elevator_setting(-3900);
					osDelay(1);
					}
					for(int z=0;z<1700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
				}
//					for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
				for(int z=0;z<1000;z++)
				{
					pid_motor_setting_sudu(0,0,0,0);
					zhuazi(dakai);
					osDelay(1);
				}
	
				break;
			}
			
			
			case 3:
			{
				//蓝色			

				 //先后退防止打到物料，然后左移，前进
//					while(HAL_GPIO_ReadPin(b_l2_GPIO_Port,b_l2_Pin)!=1)
//					{
//						pid_elevator_setting_sudu(0);
//						pid_motor_setting(-500,-500,-500,-500);//houtui
//						osDelay(1);
//					}
					for(int z=0;z<2300;z++)
					{
						pid_motor_setting(-550,550,-500,500);	//youyi
						osDelay(1);
						
					}
					
					while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=1)
					{
						pid_motor_setting(-500,-500,-500,-500);
						osDelay(1);
					}
//						while(HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin)!=0)
//					{
//						pid_motor_setting(300,300,300,300);
//						osDelay(1);
//					}
					for(int z=0;z<1700;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						pid_yuntai_setting(100);
					  osDelay(1);			
					}
					for(int z=0;z<1000;z++)
					{
						pid_yuntai_setting(100);
						pid_elevator_setting(-2400);		
						osDelay(1);
					}
					for(int z=0;z<1000;z++)
					{
						pid_elevator_setting_sudu(0);
						zhuazi(bihe);
						osDelay(1);
					}
				
					for(int z=0;z<1200;z++)
					{
						pid_elevator_setting(-800);
						osDelay(1);
					}
						for(int z=0;z<1000;z++)
				{
					pid_elevator_setting_sudu(0);
					openmv(1600)	;
						osDelay(1);
				}
				for(int z=0;z<2500;z++)
				{
					pid_yuntai_setting(4750);
					osDelay(1);
					}
				for(int z=0;z<1700;z++)
				{
					pid_yuntai_setting(4750);
					openmv(900)	;
					pid_elevator_setting(-3900);
					osDelay(1);
					}
					for(int z=0;z<1700;z++)
				{
					pid_elevator_setting_sudu(0);
					set_angel(current_angel);
				}
//					for(int z=0;z<100;z++)
//				{
//					pid_motor_setting(500,500,500,500);
//					osDelay(1);
//				}
					for(int z=0;z<1000;z++)
					{
						pid_motor_setting_sudu(0,0,0,0);
						zhuazi(dakai);
						osDelay(1);
					}	
					break;
				}
		}
		guiwei_cu(s[i],1100);
    osDelay(1);
	}	
}

//pwn初始化开启
void pwm_init()
{
	 HAL_TIM_Base_Start(&htim1);
     HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	 HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	 HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	 HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	 openmv(900)	;
	 zhuazi(1700);
	 wuliaotai(1700);
//     wuliaotai(1280);
}

//1600正对物料  1000正对地面   2500全展开 1700关


