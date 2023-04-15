#include "xunji.h"
#include "stdint.h"
#include "gpio.h"
#include "cmsis_os.h"
#include  "wheel.h"
#include  "main.h"
int r_road=0;
int r_road_l2=0;
int r_road_l1=0;
int r_road_m=0;
int r_road_r1=0;
int r_road_r2=0;

int l_side=0;
int l_side_l2=0;
int l_side_l1=0;
int l_side_m=0;
int r_side_r1=0;
int r_side_r2=0;

int back_side=0;
int back_side_l2=0;
int back_side_l1=0;
int back_side_m=0;
int back_side_r1=0;
int back_side_r2=0;

int head_side=0;
int head_road_l2=0;
int head_road_l1=0;
int head_road_m=0;
int back_road_r1=0;
int back_road_r2=0;

int f_m_number=0;  //计数
int f_r2_number=0;
int l_m_number =0;  
int l_l2_number =0;
int l_r2_number =0;
int r_m_number =0;  
int r_l2_number =0;
int r_r2_number=0;
int b_m_number=0;
int b_r2_number=0;

int xjq1=0;
int xjq2=0;
int xjh1=0;
int xjh2=0;
int xjl1=0;
int xjl2=0;
int xjr1=0;
int xjr1_l2=0;
int xjr2=0;
int xjr2_l2=0;
int xjr1_r2=0;
int xjr2_r2=0;
int xjbm1=0;
int xjbm2=0;
int motor=3000;



////头部计数
//void head_number_view()
//{ 
//	head_road_m = HAL_GPIO_ReadPin(f_m_GPIO_Port,f_m_Pin);	
//	if(head_road_m==1)
//		f_m_number++;
//}

////左面中间计数
//void l_number_view_m()
//{ 
//	l_side_m = HAL_GPIO_ReadPin(l_m_GPIO_Port,l_m_Pin);
//	if(l_side_m==1)
//	l_m_number++;
//}

////左侧左二计数
//void l_number_view_l2()
//{ 
//	l_side_l2 = HAL_GPIO_ReadPin(l_l2_GPIO_Port,l_l2_Pin);
//	if(l_side_l2==1)
//	l_l2_number++;
//}

////右侧中间计数
//void r_number_m()
//{ 
//	r_m_number = HAL_GPIO_ReadPin(r_m_GPIO_Port,r_m_Pin);
//	if(r_m_number==1)
//  r_m_number++;
//}



void xjqian(int t)
{  
	
  xjq1=xjq2;
	xjq2=HAL_GPIO_ReadPin(f_m_GPIO_Port,f_m_Pin);
	if(xjq2>xjq1)
	{
		f_m_number++;
		osDelay(t);
	}

}



void xjhou(int t)
{ 

	xjbm1=xjbm2;

  xjbm2=HAL_GPIO_ReadPin(b_m_GPIO_Port,b_m_Pin);
	if(xjbm2>xjbm1)
	{
		b_m_number++;
		osDelay(t);
	}
}



void xjyou(int t)
{
	
	xjr1=xjr2;

  xjr2=HAL_GPIO_ReadPin(r_m_GPIO_Port,r_m_Pin);
	if(xjr2>xjr1)
	{
		r_m_number++;
		osDelay(t);
	}
}

void xjyou_r2(int t)
{
	
	xjr1_r2=xjr2_r2;
  xjr2_r2=HAL_GPIO_ReadPin(r_r2_GPIO_Port,r_r2_Pin);
	if(xjr2_r2>xjr1_r2)
	{
		r_r2_number++;
		osDelay(t);
	}
}

void xjyou_l2(int t)
{
	
	xjr1_l2=xjr2_l2;
  xjr2_l2=HAL_GPIO_ReadPin(r_l2_GPIO_Port,r_l2_Pin);
	if(xjr2_l2>xjr1_l2)
	{
		r_l2_number++;
		osDelay(t);
	}
}



void xjzuo(int t)
{
//  uint32_t PreviousWakeTime =osKernelSysTick();
	xjl1=xjl2;
//	osDelayUntil(&PreviousWakeTime,1);
  xjl2=HAL_GPIO_ReadPin(l_m_GPIO_Port,l_m_Pin);
	
	if(xjl2>xjl1)
	{
		l_m_number++;	
		osDelay(t);	
	 
  }
}



//前进函数
void qianjin(int speed)
{
  
	//待加入
//	if(HAL_GPIO_ReadPin(l_r2_GPIO_Port,l_r2_Pin)==1)//减速
//	{
//		while(HAL_GPIO_ReadPin(l_m_GPIO_Port,l_m_Pin)!=1)
//		{
//	  pid_motor_setting(1000,1000,1000,1000);
//		}		
//	}
	
	if(HAL_GPIO_ReadPin(f_r1_GPIO_Port,f_r1_Pin)==1)//纠正
	{
	pid_motor_setting(speed+300,speed-300,speed-300,speed+300);
	
	}
	if(HAL_GPIO_ReadPin(f_l1_GPIO_Port,f_l1_Pin)==1)
	{
	pid_motor_setting(speed-300,speed+300,speed+300,speed-300);
	}
	else if(HAL_GPIO_ReadPin(f_l1_GPIO_Port,f_l1_Pin)==0&&HAL_GPIO_ReadPin(f_r1_GPIO_Port,f_r1_Pin)==0)//正常行驶
	{
		pid_motor_setting(speed,speed,speed,speed);
	}
}






void houtui(int speed)
{
  
	//待加入
//	if(HAL_GPIO_ReadPin(r_r2_GPIO_Port,r_r2_Pin)==1)//减速
//	{
//		while(HAL_GPIO_ReadPin(r_m_GPIO_Port,r_m_Pin)!=1)
//		{
//	  pid_motor_setting(-1000,-1000,-1000,-1000);
//		}	
//	}

	if(HAL_GPIO_ReadPin(b_r1_GPIO_Port,b_r1_Pin)==1)//纠正
	{
	pid_motor_setting(-(speed-1000),-(speed+1000),-(speed+1000),-(speed-1000));
	
	}
	if(HAL_GPIO_ReadPin(b_l1_GPIO_Port,b_l1_Pin)==1)
	{
	pid_motor_setting(-(speed+1000),-(speed-1000),-(speed-1000),-(speed+1000));
	}
	else if(HAL_GPIO_ReadPin(b_l1_GPIO_Port,b_l1_Pin)==0&&HAL_GPIO_ReadPin(b_r1_GPIO_Port,b_r1_Pin)==0)//正常行驶
	{
		pid_motor_setting(-speed,-speed,-speed,-speed);
	}
}





void zuoyi(int speed)
{
  
	
//	if(HAL_GPIO_ReadPin(b_r2_GPIO_Port,b_r2_Pin)==1&&b_r2_number==1)//减速
//	{
//		while(b_r2_number==1)
//		{
//	  pid_motor_setting(-1000,1000,-1000,1000);
//		}
//		b_r2_number=0;
//	}
	if(f_m_number==5&&HAL_GPIO_ReadPin(f_l2_GPIO_Port,f_l2_Pin)==1)
	{
		speed=1000;
	}
	else 
	{
		speed=3000;
	}
	if(HAL_GPIO_ReadPin(l_r1_GPIO_Port,l_r1_Pin)==1)//纠正
	{
	pid_motor_setting(-(speed-1000),speed-1000,-(speed+1000),speed+1000);	
	}
	if(HAL_GPIO_ReadPin(l_l1_GPIO_Port,l_l1_Pin)==1)
	{
	pid_motor_setting(-(speed+1000),speed+1000,-(speed-1000),speed-1000);
	}
	else if(HAL_GPIO_ReadPin(l_l1_GPIO_Port,l_l1_Pin)==0&&HAL_GPIO_ReadPin(l_r1_GPIO_Port,l_r1_Pin)==0)//正常行驶
	{
		pid_motor_setting(-speed,speed,-speed,speed);
	}
	
}


void zuoyi_home(int speed)
{
  
	
//	if(HAL_GPIO_ReadPin(b_r2_GPIO_Port,b_r2_Pin)==1&&b_r2_number==1)//减速
//	{
//		while(b_r2_number==1)
//		{
//	  pid_motor_setting(-1000,1000,-1000,1000);
//		}
//		b_r2_number=0;
//	}
	
	if(HAL_GPIO_ReadPin(l_r2_GPIO_Port,l_r2_Pin)==1)//纠正
	{
	pid_motor_setting(-(speed-1000),speed-1000,-(speed+1000),speed+1000);	
	}
	if(HAL_GPIO_ReadPin(l_r1_GPIO_Port,l_r1_Pin)==1)
	{
	pid_motor_setting(-(speed+1000),speed+1000,-(speed-1000),speed-1000);
	}
	else if(HAL_GPIO_ReadPin(l_r1_GPIO_Port,l_r1_Pin)==0&&HAL_GPIO_ReadPin(l_r2_GPIO_Port,l_r2_Pin)==0)//正常行驶
	{
		pid_motor_setting(-speed,speed,-speed,speed);
	}
	
}




void youyi(int speed)
{
  
	
//	if(HAL_GPIO_ReadPin(f_r2_GPIO_Port,b_r2_Pin)==1&&f_r2_number==1)//减速
//	{
//		while(f_r2_number==1)
//		{
//	  pid_motor_setting(1000,-1000,1000,-1000);
//		}
//		b_r2_number=0;
//	}
	
	if(HAL_GPIO_ReadPin(r_r1_GPIO_Port,r_r1_Pin)==1)//纠正
	{
	pid_motor_setting((speed+400),-(speed+400),(speed-400),-(speed-400));	
	}
	if(HAL_GPIO_ReadPin(r_l1_GPIO_Port,r_l1_Pin)==1)
	{
	pid_motor_setting((speed-400),-(speed-400),(speed+400),-(speed+400));
	}
	else if(HAL_GPIO_ReadPin(r_l1_GPIO_Port,r_l1_Pin)==0&&HAL_GPIO_ReadPin(r_r1_GPIO_Port,r_r1_Pin)==0)//正常行驶
	{
		pid_motor_setting(speed,-speed,speed,-speed);
	}
}



