#include "pid.h"

#define ABS(x)		((x>0)? x: -x) 
int plinus = 1;//�����ж���̨����

static void pid_param_init
(
	PID_TypeDef * pid, 
	PID_ID   id,
	uint16_t maxout,
	uint16_t intergral_limit,
	float deadband,
	uint16_t period,
	int16_t  max_err,
	int16_t  target,

	float 	kp, 
	float 	ki, 
	float 	kd)
{
	pid->id = id;		
	
	pid->ControlPeriod = period;             //û�õ�
	pid->DeadBand = deadband;
	pid->IntegralLimit = intergral_limit;
	pid->MaxOutput = maxout;
	pid->Max_Err = max_err;
	pid->target = target;
	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
	pid->output = 0;
}

/*��;���Ĳ����趨--------------------------------------------------------------*/
static void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}



static float pid_calculate(PID_TypeDef* pid, float measure)//, int16_t target)
{
//	uint32_t time,lasttime;


	pid->lasttime = pid->thistime;
	pid->thistime = HAL_GetTick();
	pid->dtime = pid->thistime-pid->lasttime;
	pid->measure = measure;
  //	pid->target = target;
		
	pid->last_err  = pid->err;
	pid->last_output = pid->output;
	
	pid->err = pid->target - measure;
	
	//�Ƿ��������
	if((ABS(pid->err) > pid->DeadBand))
	{
		pid->pout = pid->kp * pid->err;
		pid->iout += (pid->ki * pid->err);
		

		pid->dout =  pid->kd * (pid->err - pid->last_err); 
		
		//�����Ƿ񳬳�����
		if(pid->iout > pid->IntegralLimit)
			pid->iout = pid->IntegralLimit;
		if(pid->iout < - pid->IntegralLimit)
			pid->iout = - pid->IntegralLimit;
		
		//pid�����
		pid->output = pid->pout + pid->iout + pid->dout;
		

		//pid->output = pid->output*0.7f + pid->last_output*0.3f;  //�˲���
		if(pid->output>pid->MaxOutput)         
		{
			pid->output = pid->MaxOutput;
		}
		if(pid->output < -(pid->MaxOutput))
		{
			pid->output = -(pid->MaxOutput);
		}
	
	}
  if(pid->err > pid->last_err){
		return pid->last_output;
		}
	else{
	return pid->output;
	}

}

static float pid_calculate_2(PID_TypeDef* pid, float measure)//��̨ר��pid�Ƕȿ���
{
//	uint32_t time,lasttime;
	pid->lasttime = pid->thistime;
	pid->thistime = HAL_GetTick();
	pid->dtime = pid->thistime-pid->lasttime;
	pid->last_measure =pid->measure;
	pid->measure = measure;
  //	pid->target = target;
	
	//��̨תһȦΪ8191
	//�˴�����ɼ�����ת�ĽǶ�
//	if((pid->target - 4096)>0)
//	 {
//		pid->opposite_target = pid->target - 4096;
//	}
//	
//	else if((pid->target - 4096)<0)
//	{
//		pid->opposite_target = 8192+(pid->target - 4096);
//	}
	
	pid->last_err  = pid->err;
	pid->last_output = pid->output;
	pid->err = pid->target - measure;
	
//	if(pid->err >4096)
//	{
//		pid->err = 8192 - pid->err;
//	}
		if(measure>7000)
		{
			pid->err =pid->target +(8192- measure);
		}

//  if(pid->opposite_target >4096)
//	{
//		if(pid->measure >pid->target && pid->measure < pid->opposite_target)
//			{
//			plinus = -1;
//		}
//		else
//			{
//			plinus = 1;
//		}
//	}
//	
//	else
//	{
//		if(pid->measure <pid->target && pid->measure > pid->opposite_target)
//		{
//			plinus = 1;
//		}
//		else
//		{
//			plinus = -1;
//		}
//	}
//		
	
	//�Ƿ��������
	if((ABS(pid->err) > pid->DeadBand))
	{
		pid->pout = pid->kp * pid->err;
		pid->iout += (pid->ki * pid->err);
		

		pid->dout =  pid->kd * (pid->err - pid->last_err); 
		
		//�����Ƿ񳬳�����
		if(pid->iout > pid->IntegralLimit)
			pid->iout = pid->IntegralLimit;
		if(pid->iout < - pid->IntegralLimit)
			pid->iout = - pid->IntegralLimit;
		
		//pid�����
		pid->output = (pid->pout + pid->iout + pid->dout) ;
		

		//pid->output = pid->output*0.7f + pid->last_output*0.3f;  //�˲���
		if(pid->output>pid->MaxOutput)         
		{
			pid->output = pid->MaxOutput;
		}
		if(pid->output < -(pid->MaxOutput))
		{
			pid->output = -(pid->MaxOutput);
		}
		if(ABS(pid ->err-pid->last_err) < 20){
			
	  	if(ABS(pid ->err) < 10){
				pid->output  = 0;
			}
			}
		if(ABS(pid ->measure - pid->target >8000 &&  pid ->measure - pid->target<-8000))  //��ֹ��0��8191ͻ�䵼����̨��ת
		{
			pid->output =0;
		}
	}
  if(pid->measure <600)
	{
		pid->output = pid->output;
	}
	//*0.5f
	return pid->output;

}



/*pid�ṹ���ʼ����ÿһ��pid������Ҫ����һ��-----------------------------------------------------*/
void pid_init(PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
	pid->f_cal_pid_2 = pid_calculate_2;
}

//delay����
int delay_ms(uint16_t nms)
{    
  int32_t temp; 

  SysTick->LOAD = 8000*nms; 

  SysTick->VAL=0X00; 

  SysTick->CTRL=0X01;

  do 

  { 

       temp=SysTick->CTRL; 

  }

     while((temp&0x01)&&(!(temp&(1<<16))));

     

     SysTick->CTRL=0x00; 

     SysTick->VAL =0X00; 
	
	
	return 1;

}
