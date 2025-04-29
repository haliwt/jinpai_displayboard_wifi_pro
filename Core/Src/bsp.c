#include "bsp.h"

main_process_t  gpro_t;

uint8_t wifi_power_off_error_flag ;

static void debug_usart_send_data(uint8_t *pdata,uint16_t len);


/**********************************************
*
*Functin Name: void displaybaord_process_handler(void)
*Function : 
*Input Ref:  NO
*Return Ref: NO

***********************************************/
void displaybaord_process_handler(void)
{

	switch(gpro_t.gPower_On){

	case power_on:

		power_on_handler();
	break;

	case power_off:

		power_off_handler();

	break;

	}

}
/****************************************************************************
	*
	*Functin Name: void receive_copy_cmd(uint8_t cmd)
	*Function : 
	*Input Ref:  NO
	*Return Ref: NO
	*
******************************************************************************/
void receive_copy_cmd(uint8_t cmd)
{
    switch(cmd){

	case copy_power_off:
		run_t.wifi_power_on_flag = RUN_NULL; //divisive app power on and key power on
	    gpro_t.main_process_step=0;
	    gpro_t.gPower_On = power_off;
		wifi_power_off_error_flag = 0;
		gpro_t.g_debug_state[1]=0x02;
	

	break;


	case copy_power_on:
		
		gpro_t.main_process_step=0;
		run_t.wifi_power_on_flag = RUN_NULL; //divisive app pow
		gpro_t.gPower_On = power_on;
		wifi_power_off_error_flag = 0;
		gpro_t.g_debug_state[0]=0x01;
	   


	break;

	case copy_wifi_det_power_off:
		if(gpro_t.gPower_On == power_off){


		}
		else{
	        run_t.wifi_power_on_flag = RUN_NULL; //divisive app power on and key power on
		    gpro_t.main_process_step=0;
		    gpro_t.gPower_On = power_off;
			wifi_power_off_error_flag = 1;
            gpro_t.g_debug_state[1]=0xff;
        }


	break;

	}

}

/****************************************************************************
	*
	*Functin Name: void debug_usart_send_data(uint8_t data)
	*Function : 
	*Input Ref:  NO
	*Return Ref: NO
	*
******************************************************************************/
static void debug_usart_send_data(uint8_t *pdata,uint16_t len)
{
  
  HAL_UART_Transmit_DMA(&huart2, pdata,len);

}

void debug_usart2_send_dat_handler(void)
{
    if(gpro_t.gPower_On==power_on){

      if(gpro_t.gTimer_debug_counter > 3){
	  	gpro_t.gTimer_debug_counter=0;
	   debug_usart_send_data(&gpro_t.g_debug_state[0],1);
	   HAL_Delay(5);

      }


	}
	else{
	     if(gpro_t.gTimer_debug_counter > 3 && wifi_power_off_error_flag == 0){
		   gpro_t.gTimer_debug_counter=0;
		  	debug_usart_send_data(&gpro_t.g_debug_state[1],1);
		      HAL_Delay(5);

		   
	
		 }
		 else if(wifi_power_off_error_flag == 1){

		      if(gpro_t.gTimer_debug_counter > 3){
				gpro_t.gTimer_debug_counter=0;
			   debug_usart_send_data(&gpro_t.g_debug_state[1],1);
			 
		      HAL_Delay(5);
			  }


		 }


	}


}

