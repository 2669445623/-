#include "main.h"
#include "wheel.h"
#include "usart.h"
#include "string.h"
#include "JY62.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "duoji.h"
static signed char ucRxBuffer[250] = {0};
static unsigned char ucRxCnt = 0;	

uint8_t ucData;
uint8_t  ucData_2;
uint8_t stcAcc[8];
uint8_t stcGyro[8];
uint8_t stcAngle[8];            //mpu�������ݻ���e
extern uint8_t s[6];
extern uint8_t e[6];

static signed char mvRxBuffer[250];
static unsigned char mvRxCnt = 0;	 
uint8_t mvucbuf;     //��������
uint8_t openmvdate[11]={0};

static signed char esp8266_buffer[250];
static unsigned char esp8266_cnt = 0;	 
uint8_t esp8266_buf;     //�ֻ�����
uint8_t esp8266date[12]={0};



int YAWCMD[3] = {0XFF,0XAA,0X52};  //z����JY62��ʼ�����ݵı���
int  ACCCMD[3] = {0XFF,0XAA,0X67};
int  level_set[3] = {0XFF,0XAA,0X67};
int SLEEPCMD[3] = {0XFF,0XAA,0X60};
int UARTMODECMD[3] = {0XFF,0XAA,0X61};
int IICMODECMD[3] = {0XFF,0XAA,0X62};

int x,y,z;
int success=0;
int jiaodu=0;
int xiuzheng_moshi=1;
extern int angel;                     
 struct
{
		float Angle_x;  //�Ƕ�
		float Angle_y;  //������
		float Angle_z;  
		
		float Gyro_x;   //���ٶ�
	  float Gyro_y;
		float Gyro_z;
		
		float Acc_x;    //���ٶ�
	  float Acc_y;
		float Acc_z;
	}JY62;
	
 struct
{
		float Angle_x;  //�Ƕ�
		float Angle_y;  //������
		float Angle_z;  
		
		float Gyro_x;   //���ٶ�
	  float Gyro_y;
		float Gyro_z;
		
		float Acc_x;    //���ٶ�
	  float Acc_y;
		float Acc_z;
	}JY62_record;	
	
 struct 
	{
		float Angle_x_error;  //�Ƕ����
		float Angle_y_error;  //������
		float Angle_z_error;  
		
		float Gyro_x_error;   //���ٶ����
	  float Gyro_y_error;
		float Gyro_z_error;
		
		float Acc_x_error;    //���ٶ����
	  float Acc_y_error;
		float Acc_z_error;
	}JY62_error;                   //mpu�������У��
	
struct
	{
		float Angle_x_buf[20];
		float Angle_y_buf[20];
		float Angle_z_buf[20];
		float Gyro_x_buf[20];
	  float Gyro_y_buf[20];
		float Gyro_z_buf[20];
		float Acc_x_buf[20];
	  float Acc_y_buf[20];
		float Acc_z_buf[20];
		int i;
	}JY62_buf;                  //mpu���ݻ��棬�����˲�
	
//	void Start_mpu_Task(void const * argument)
	
void JY62_1(void const * argument)
{

	UART_Start_Receive_DMA(&huart1, &ucData, 1);  //����DMA��ʼ��
    HAL_UART_Receive_DMA(&huart1, &ucData, 1);//����JY62����
	UART_Start_Receive_IT(&huart2, &mvucbuf, 1);//����openmv����
	UART_Start_Receive_IT(&huart3, &esp8266_buf, 1);//����esp8266����
	JY62initial();//����JY62���ݳ�ʼ������
  for(;;)
  {	
	get_JY62();//��ȡJY62��̬������������

    osDelay(1);
  }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//���ڽ����жϴ�����
{
	if(huart->Instance == USART1)//����ΪJY62������
 {
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) 
  {
	return;
	}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ����
		{
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
		}
//		if(JY62_buf.i ==20)
//			{
//				JY62_buf.i=0;
//			}
			
      JY62_buf.Angle_x_buf[JY62_buf.i] = (float)(stcAngle[1]<<8|stcAngle[0])/32768*180 ;
			
			
		  JY62_buf.Angle_y_buf[JY62_buf.i] = (float)(stcAngle[3]<<8|stcAngle[2])/32768*180;
			
			
		  JY62.Angle_z = (float)(stcAngle[5]<<8|stcAngle[4])/32768*180;
			
		  JY62_buf.Gyro_x_buf[JY62_buf.i] = (float)(stcGyro[1]<<8|stcGyro[0])/32768*2000;
			
			
	  	JY62_buf.Gyro_y_buf[JY62_buf.i] = (float)(stcGyro[3]<<8|stcGyro[2])/32768*2000;
			
			
	  	JY62_buf.Gyro_z_buf[JY62_buf.i] = (float)(stcGyro[5]<<8|stcGyro[4])/32768*2000;
			
			
	  	JY62_buf.Acc_x_buf[JY62_buf.i] = (float)((stcAcc[1]<<8)|stcAcc[0])/32768*16*9.8f;
			
			
		  JY62_buf.Acc_y_buf[JY62_buf.i] = (float)((stcAcc[3]<<8)|stcAcc[2])/32768*16*9.8f;
		
			
	  	JY62_buf.Acc_z_buf[JY62_buf.i] = (float)((stcAcc[5]<<8)|stcAcc[4])/32768*16*9.8f;
			
//			JY62_handle();
//			JY62_buf.i ++;
		ucRxCnt=0;//��ջ�����
	}
}
 
 	if(huart->Instance == USART2)//����Ϊopenmv����
	{
		HAL_UART_Receive_IT(&huart2, &mvucbuf, 1);
		mvRxBuffer[mvRxCnt++]=mvucbuf;
		if (mvRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
		{
			mvRxCnt=0;
			return;
		}
		if (mvRxCnt<12) 
		{
			return;
		}//���ݲ���11�����򷵻�
		else
		{
			memcpy(&openmvdate,&mvRxBuffer[1],11);			
			mvRxCnt=0;
		}
	}
	
	if(huart->Instance == USART3)//����Ϊesp8266����
	{
		HAL_UART_Receive_IT(&huart3, &esp8266_buf, 1);
		esp8266_buffer[esp8266_cnt++]=esp8266_buf;
		if (esp8266_buffer[0]!=0x0D) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
		{
			esp8266_cnt=0;
			return;
		}
		if (esp8266_cnt<13) 
		{
			return;
		}//���ݲ���10�����򷵻�
		else
		{
			memcpy(&esp8266date,&esp8266_buffer[0],12);			
			esp8266_cnt=0;		
		}
	}
		
}

void JY62_handle(void)
{

	int i =0;
	float Angle_x_buf=0;
	float Angle_y_buf=0;
	float Angle_z_buf=0;
	
	float Gyro_x_buf=0;
	float Gyro_y_buf=0;
	float Gyro_z_buf=0;
	
	float Acc_x_buf=0;
	float Acc_y_buf=0;
	float Acc_z_buf=0;
	
	for(i = 0;i<20;i++)
	{
		Angle_x_buf += JY62_buf.Angle_x_buf[i];
		Angle_y_buf += JY62_buf.Angle_y_buf[i];
		Angle_z_buf += JY62_buf.Angle_z_buf[i];
		
		Gyro_x_buf += JY62_buf.Gyro_x_buf[i];
		Gyro_y_buf += JY62_buf.Gyro_y_buf[i];
		Gyro_z_buf += JY62_buf.Gyro_z_buf[i];
		
		Acc_x_buf += JY62_buf.Acc_x_buf[i];
		Acc_y_buf += JY62_buf.Acc_y_buf[i];
		Acc_z_buf += JY62_buf.Acc_z_buf[i];
	}
	JY62.Angle_x = Angle_x_buf/20 - JY62_error.Angle_x_error;
	JY62.Angle_y = Angle_y_buf/20 - JY62_error.Angle_y_error;
	JY62.Angle_z = Angle_z_buf/20 - JY62_error.Angle_z_error;
	x=JY62.Angle_x;
	y=JY62.Angle_y;
	z=JY62.Angle_z;
	
	JY62.Gyro_x = Gyro_x_buf/20 - JY62_error.Gyro_x_error;
	JY62.Gyro_y = Gyro_y_buf/20 - JY62_error.Gyro_y_error;
	JY62.Gyro_z = Gyro_z_buf/20 - JY62_error.Gyro_z_error;
	
	JY62.Acc_x = Acc_x_buf/20 - JY62_error.Acc_x_error;
	JY62.Acc_y = Acc_y_buf/20 - JY62_error.Acc_y_error;
	JY62.Acc_z = Acc_z_buf/20 - JY62_error.Acc_z_error;

	

}

void get_JY62(void)
{
//	JY62_record.Angle_x=stcAngle.Angle_x;
	JY62_record.Angle_y=JY62.Angle_y;
	JY62_record.Angle_z=JY62.Angle_z;
}

void  JY62initial()
{
	HAL_UART_Transmit(&huart1,(uint8_t *)level_set,sizeof(level_set),100);
	osDelay(1);
	HAL_UART_Transmit(&huart1,(uint8_t *)ACCCMD,sizeof(ACCCMD),100);
	osDelay(1);
	 HAL_UART_Transmit(&huart1,(uint8_t *)YAWCMD,sizeof(YAWCMD),100);
	osDelay(1);
	for(int i=0;i<6;i++)
	{
  s[i]=0;
	}
	for(int i=0;i<6;i++)
	{
  e[i]=0;
	}
}

void error_init()
{
	
	JY62_error.Angle_x_error = 0;
	JY62_error.Angle_y_error = 4.05;//���޸�
	JY62_error.Angle_z_error = 0;
	JY62_error.Gyro_x_error = 0;
	JY62_error.Gyro_y_error = 0;
	JY62_error.Gyro_z_error = 0;
	JY62_error.Acc_x_error = 0;
	JY62_error.Acc_y_error = 0;
	JY62_error.Acc_z_error = 0;
	
}

void set_angel(double a)
{
	if(__fabs(a-JY62_record.Angle_z)<0.5)
	{
		pid_motor_setting(0,0,0,0);
		osDelay(1);
	}
	else if(((a-JY62_record.Angle_z)>=0.5&&(a-JY62_record.Angle_z)<=180.0)||a-JY62_record.Angle_z<=-180.0)
	{
		pid_motor_setting(-300,300,300,-300);
		osDelay(1);
	}
	else if(((a-JY62_record.Angle_z)<=-0.5&(a-JY62_record.Angle_z)>=-180.0)||(a-JY62_record.Angle_z)>=180.0)
	{
		pid_motor_setting(300,-300,-300,300);
		
		
		osDelay(1);
	}

}


