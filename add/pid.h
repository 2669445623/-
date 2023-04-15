#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"



 typedef enum
  {
      PID_Position,//两种模式
	    PID_Speed
	
  }PID_ID;


   typedef struct _PID_TypeDef
 {
	PID_ID id;
	
	float target;							//目标值
	float lastNoneZeroTarget;
	float kp;                    //系数
	float ki;
	float kd;
	
	float   measure;					//测量值
	float   last_measure;			//上次测量值
	float   err;							//误差
	float   last_err;      		//上次误差
	float   opposite_target;      //云台电机目标值的对立值
	
	float pout;           
	float iout;
	float dout;
	
	float output;						//本次输出
	float last_output;			//上次输出
	
	float MaxOutput;				//输出限幅
	float IntegralLimit;		//积分限幅
	float DeadBand;			  //死区（绝对值）
	float ControlPeriod;		//控制周期
	float  Max_Err;					//最大误差
	
	uint32_t thistime;
	uint32_t lasttime;
	uint8_t dtime;	
	
	void (*f_param_init)
	(
		struct _PID_TypeDef *pid,  //PID参数初始化
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
				   
	void (*f_pid_reset)(struct _PID_TypeDef *pid, float kp,float ki, float kd);		//pid三个参数修改
	float (*f_cal_pid)(struct _PID_TypeDef *pid, float measure);   //pid计算
	float (*f_cal_pid_2)(struct _PID_TypeDef *pid, float measure);  //云台角度计算
  }PID_TypeDef;
 
void pid_init(PID_TypeDef* pid);
	

//获取数据并进行移位的函数
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
    uint16_t ecd;//角度
    int16_t speed_rpm;//速度
	  int really_ecd;//输出轴的当前角度
    int16_t given_current;//提供给电流的值
    uint8_t temperate;//当前工作温度
    int16_t last_ecd;//上次输出的角度
	  int ecd_error;//当前速度与前一次速度的误差
	  int cyclinder_number;//转子所转过的圈数
	  int out_angel;//总输出的角度
	  int out_really_angel;//输出轴的角度
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
