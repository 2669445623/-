#ifndef __WHEEL_H__
#define __WHEEL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void pid_main_init(void);//轮子pid参数总体设定
void CAN_cmd_chassis(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);//CAN数据发送以及接收的集成
void CAN_anter_chassis(int16_t motor1);//云台数据发送和接收的集合
void pid_motor_setting(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);//设定电机转速（内含pid计算以及CAN发送接收）
void pid_yuntai_setting(int16_t position); //设定云台角度
void CAN_sigan_chassis(int16_t position);//设置丝杆的位置	
void pid_elevator_setting(int16_t elevator1);
void m_max_speed(void);
void jinyige(void);
void pid_elevator_setting_sudu(int16_t sudu);
void pid_elevator_setting_sudu_remote(int16_t sudu);
void jinyige_slow(void);
void pid_motor_setting_sudu(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
void pid_motor_setting_weizhi(int position);
void jinyige_2(void);
void pid_yuntai_setting_sudu(int16_t speed);
void wheelhandle(void);
#ifdef __cplusplus
}
#endif

#endif /* __WHEEL_H__ */
