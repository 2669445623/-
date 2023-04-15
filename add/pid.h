#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"



 typedef enum
  {
      PID_Position,//����ģʽ
	    PID_Speed
	
  }PID_ID;


   typedef struct _PID_TypeDef
 {
	PID_ID id;
	
	float target;							//Ŀ��ֵ
	float lastNoneZeroTarget;
	float kp;                    //ϵ��
	float ki;
	float kd;
	
	float   measure;					//����ֵ
	float   last_measure;			//�ϴβ���ֵ
	float   err;							//���
	float   last_err;      		//�ϴ����
	float   opposite_target;      //��̨���Ŀ��ֵ�Ķ���ֵ
	
	float pout;           
	float iout;
	float dout;
	
	float output;						//�������
	float last_output;			//�ϴ����
	
	float MaxOutput;				//����޷�
	float IntegralLimit;		//�����޷�
	float DeadBand;			  //����������ֵ��
	float ControlPeriod;		//��������
	float  Max_Err;					//������
	
	uint32_t thistime;
	uint32_t lasttime;
	uint8_t dtime;	
	
	void (*f_param_init)
	(
		struct _PID_TypeDef *pid,  //PID������ʼ��
		PID_ID id,
		uint16_t maxOutput,
		uint16_t integralLimit,
		float deadband,
		uint16_t controlPeriod,
		int16_t max_err,     
		int16_t  target,
		float kp,
		float ki,
		float kd
	);
				   
	void (*f_pid_reset)(struct _PID_TypeDef *pid, float kp,float ki, float kd);		//pid���������޸�
	float (*f_cal_pid)(struct _PID_TypeDef *pid, float measure);   //pid����
	float (*f_cal_pid_2)(struct _PID_TypeDef *pid, float measure);  //��̨�Ƕȼ���
  }PID_TypeDef;
 
void pid_init(PID_TypeDef* pid);
	

//��ȡ���ݲ�������λ�ĺ���
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }
		
		typedef struct
  {
    uint16_t ecd;//�Ƕ�
    int16_t speed_rpm;//�ٶ�
	  int really_ecd;//�����ĵ�ǰ�Ƕ�
    int16_t given_current;//�ṩ��������ֵ
    uint8_t temperate;//��ǰ�����¶�
    int16_t last_ecd;//�ϴ�����ĽǶ�
	  int ecd_error;//��ǰ�ٶ���ǰһ���ٶȵ����
	  int cyclinder_number;//ת����ת����Ȧ��
	  int out_angel;//������ĽǶ�
	  int out_really_angel;//�����ĽǶ�
  } motor_measure_t;
		
	typedef struct
  {
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
  } motor_measure_t_2;
	
int delay_ms(uint16_t time);



	
#ifdef __cplusplus
}
#endif

#endif /* __PID_H__ */	
