#ifndef __DUOJI_H__
#define __DUOJI_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
void pwm_init(void);
void zhuazi(int16_t pwm3);
void openmv(int16_t pwm4);
void fangzhi(int yanse);
void guiwei(int weizhi);
void fangzhi_cu_2(void);
void fangzhi_cu_1(void);
void fangzhi_cu_3(void);
void zhuaqu_yuan(void);
void zhuaqu_yuan_2(void);
void zhuaqu_cu(int cengci);
void guiwei_cu(int weizhi,int dakai);
void fangzhi_cheng_2(void);
void wuliaotai(int16_t pwm2);
void fangzhi_ercheng(int yanse);
void guiwei_ercheng(int weizhi);

	#ifdef __cplusplus
}
#endif

#endif /* __DUOJI_H__ */	
