#include "bsp.h"

main_process_t  gpro_t;

uint8_t wifi_power_off_error_flag ;

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
/**********************************************
*
*Functin Name: void receive_copy_cmd(uint8_t cmd)
*Function : 
*Input Ref:  NO
*Return Ref: NO

***********************************************/
void receive_copy_cmd(uint8_t cmd)
{
    switch(cmd){

	case copy_power_off:
		run_t.wifi_power_on_flag = RUN_NULL; //divisive app power on and key power on
	    gpro_t.main_process_step=0;
	    gpro_t.gPower_On = power_off;
		wifi_power_off_error_flag = 0;
		
	

	break;


	case copy_power_on:
		
		gpro_t.main_process_step=0;
		run_t.wifi_power_on_flag = RUN_NULL; //divisive app pow
		gpro_t.gPower_On = power_on;
		wifi_power_off_error_flag = 0;
	   


	break;

	case copy_wifi_det_power_off:
		if(gpro_t.gPower_On == power_off){


		}
		else{
	        run_t.wifi_power_on_flag = RUN_NULL; //divisive app power on and key power on
		    gpro_t.main_process_step=0;
		    gpro_t.gPower_On = power_off;
			wifi_power_off_error_flag = 1;

        }


	break;

	}

}



