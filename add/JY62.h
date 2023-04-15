#ifndef __JY62_H__
#define __JY62_H__

#ifdef __cplusplus
extern "C" {
#endif


 
void JY62_handle(void);  //mpu数据处理
void  JY62initial(void);
void error_init(void);
void get_JY62(void);
void set_angel(double a);
#ifdef __cplusplus
}
#endif

#endif /* __JY62_H__ */	
