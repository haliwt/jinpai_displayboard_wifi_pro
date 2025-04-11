#ifndef __BSP_H
#define __BSP_h
#include "main.h"

#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "smg.h"
#include "cmd_link.h"
#include "run.h"
#include "key.h"
#include "led.h"
#include "single_mode.h"
#include "display.h"
#include "interrupt_manager.h"


typedef enum{

  copy_power_off = 0x1,
  copy_power_on = 0x02,
  copy_wifi_power_off=3,
  copy_wifi_power_on=4,
  copy_wifi_timer_power_on=5,
  


}copy_cmd_t;


typedef struct COMP_CMD_T{

   uint8_t g_copy_cmd ;



}main_process_t;


extern main_process_t  gpro_t;




void receive_copy_cmd(uint8_t cmd);



#endif 

