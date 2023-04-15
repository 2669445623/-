#ifndef __WHEEL_H__
#define __WHEEL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void pid_main_init(void);//����pid���������趨
void CAN_cmd_chassis(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);//CAN���ݷ����Լ����յļ���
void CAN_anter_chassis(int16_t motor1);//��̨���ݷ��ͺͽ��յļ���
void pid_motor_setting(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);//�趨���ת�٣��ں�pid�����Լ�CAN���ͽ��գ�
void pid_yuntai_setting(int16_t position); //�趨��̨�Ƕ�
void CAN_sigan_chassis(int16_t position);//����˿�˵�λ��	
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
