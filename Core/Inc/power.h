#ifndef __POWER_H_
#define __POWER_H_
#include "main.h"

typedef enum {

    RUN_POWER_ON=0,
	UPDATE_DATA=1,
    POWER_OFF_PROCESS=3,
    RUN_WIFI_NORMAL_POWER_ON,
    RUN_WIFI_TIMER_POWER_ON,
    RUN_NULL,
    RUN_POWER_OFF_NULL,

}power_on_state_t;

extern void (*single_ai_fun)(uint8_t cmd);
extern void (*single_add_fun)(void);
extern void (*sendAi_usart_fun)(uint8_t senddat);
extern void (*dispose_key)(uint8_t dsdat);


void Scan_KeyMode(void);
void Single_RunCmd(void);


void RunReference_Fun(void);

void Single_AI_RunCmd(void (*singlaieHandler)(uint8_t dat));

void Single_Add_RunCmd(void(*addHandler)(void));

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void));

void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat));

void SplitDispose_Key_RunCmd(void(*keyHandler)(uint8_t dat));

void power_on_handler(void);

void power_off_handler(void);



#endif 

