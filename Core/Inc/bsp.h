#ifndef __BSP_H
#define __BSP_h
#include "main.h"

#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "dma.h"

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

#define  DEBUG_SIZE   3
typedef enum _power_onoff_state{

    power_off,
	power_on


}power_onoff_state;


typedef enum power_special_off_state{

  power_off_run = 1,
}power_off_special;


typedef enum{

  copy_power_off = 0x1,
  copy_power_on = 0x02,
  copy_wifi_power_off=3,
  copy_wifi_power_on=4,
  copy_wifi_timer_power_on=5,
  copy_wifi_det_power_off =6
  


}copy_cmd_t;


typedef struct COMP_CMD_T{

   uint8_t g_copy_cmd ;
   uint8_t gPower_On;
   uint8_t main_process_step;
   uint8_t g_debug_state[DEBUG_SIZE];

   uint8_t gTimer_debug_counter;



}main_process_t;


extern main_process_t  gpro_t;


void displaybaord_process_handler(void);


void receive_copy_cmd(uint8_t cmd);



void debug_usart2_send_dat_handler(void);

#endif 

