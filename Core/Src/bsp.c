#include "bsp.h"

main_process_t  gpro_t;



void receive_copy_cmd(uint8_t cmd)
{
    switch(cmd){

	case copy_power_off:
		run_t.wifi_power_on_flag = RUN_POWER_OFF_NULL; //divisive app power on and key power on
		run_t.gRunCommand_label =RUN_POWER_OFF;
		run_t.power_on_run_update_data_flag=0;
		run_t.power_on_recoder_times++ ;
		run_t.power_key_interrupt_flag=0;
		run_t.key_power_on_flag= 0;

	break;


	case copy_power_on:
		run_t.gTimer_set_temp_times=0; //conflict with send temperatur value
		run_t.gRunCommand_label =RUN_POWER_ON;
		run_t.power_on_run_update_data_flag=0;
		run_t.power_key_interrupt_flag=0;
		run_t.power_on_recoder_times++ ;
		run_t.key_power_on_flag = 1;
		run_t.wifi_power_on_flag = RUN_POWER_OFF_NULL; //divisive app pow


	break;

	}

}



