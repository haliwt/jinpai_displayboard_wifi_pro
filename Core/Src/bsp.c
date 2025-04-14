#include "bsp.h"

main_process_t  gpro_t;



void receive_copy_cmd(uint8_t cmd)
{
    switch(cmd){

	case copy_power_off:
		run_t.wifi_power_on_flag = RUN_POWER_OFF_NULL; //divisive app power on and key power on
	    run_t.power_key_interrupt_flag=0;
	    gpro_t.gPower_On = power_off;
		gpro_t.main_process_step=0;
		
	

	break;


	case copy_power_on:
		run_t.gTimer_set_temp_times=0; //conflict with send temperatur value
		run_t.power_key_interrupt_flag=0;
		
		run_t.wifi_power_on_flag = RUN_POWER_OFF_NULL; //divisive app pow
		gpro_t.gPower_On = power_on;
		gpro_t.main_process_step=0;
	   


	break;

	}

}



