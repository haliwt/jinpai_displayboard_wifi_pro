#include "bsp.h"

volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
uint8_t outputBuf[8];
volatile uint8_t transferSize;
volatile uint8_t state;
uint8_t copy_mainboard_cmd; //WT.EDIT 2025.04.22
uint8_t inputBuf[MAX_BUFFER_SIZE];

uint16_t Error_Counter;

uint8_t wifi_link_counter;

static void handleState3(void);
static void handleState4(void);
static void handleState5(void);
static void handleWifiInfo(void);


void SendData_Copy_Cmd(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='Y'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		
	    transferSize=3;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}

/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_PowerOnOff(uint8_t index)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D //Touch Key
	outputBuf[1]='K'; //58
	outputBuf[2]='P'; //4C	// 'P' power
	outputBuf[3]=index; //53	// 'S' select light command, 'C' close all light command
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
void SendData_Buzzer(void)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D
	outputBuf[1]='K'; //58
	outputBuf[2]='Z'; //4C	// buzzer
	outputBuf[3]='Z'; //53	// buzzer
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send 
 * 
*********************************************************/
void SendData_Set_Wifi(uint8_t hdata)
{
	    outputBuf[0]='T'; //
		outputBuf[1]='K'; //58
		outputBuf[2]='W'; //W ->WIFI
		outputBuf[3]=hdata; 
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Set_Command(uint8_t hdata)

{
	    outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='C'; //"C" ->command
		outputBuf[3]=hdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Temp_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='M'; //"T"->temperature
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Time_Data(uint8_t tdata)
 * Function:send to set  timer timing value
 * 
*********************************************************/
void SendData_Time_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='T'; //"T"->timer timing
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Works_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='O'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendData_Remaining_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='R'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}

/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) // Motor Board receive data (filter)
    {
        switch (state)
        {
        case 0: // #0
            if (inputBuf[0] == 'M') // hex :4D - "M" - fixed mainboard
                state = 1;
            break;

        case 1: // #1
            switch (inputBuf[0])
            {
            case 'A':
                state = 2;
                break;
            case 'R':
                run_t.wifi_orderByMainboard_label = WIFI_REF_DATA;
                state = 3;
                break;
            case 'Y': // O -> "copy"
                run_t.wifi_orderByMainboard_label = MAIN_BOARD_COPY_CMD;
                state = 3;
                break;
            default:
                state = 0;
                break;
            }
            break;

        case 2: // #2
            if (strchr("DWPCTRB", inputBuf[0])) // 'D' -> data, 'W' -> wifi
            {
                switch (inputBuf[0])
                {
                case 'D':
                    run_t.wifi_orderByMainboard_label = PANEL_DATA;
                    break;
                case 'W':
                    run_t.wifi_orderByMainboard_label = WIFI_INFO;
                    break;
                case 'P':
                    run_t.wifi_orderByMainboard_label = WIFI_TEMP;
                    break;
                case 'C':
                    run_t.wifi_orderByMainboard_label = WIFI_CMD;
                    break;
                case 'B':
                    run_t.wifi_orderByMainboard_label = WIFI_BEIJING_TIME;
                    break;
                case 'T':
                    run_t.wifi_orderByMainboard_label = WIFI_SET_TIMING;
                    break;
                }
                state = 3;
            }
            else
            {
                state = 0;
            }
            break;

        case 3:
            handleState3();
            break;

        case 4:
            handleState4();
            break;

        case 5:
            handleState5();
            break;

        default:
            break;
        }

        __HAL_UART_CLEAR_OREFLAG(&huart1);
        HAL_UART_Receive_IT(&huart1, inputBuf, 1); // UART receive data interrupt 1 byte
    }
}

static void handleState3(void)
{
    switch (run_t.wifi_orderByMainboard_label)
    {
    case PANEL_DATA:
        run_t.gReal_humtemp[0] = inputBuf[0]; // Humidity value
        state = 4;
        break;

    case WIFI_INFO:
        handleWifiInfo();
        break;

    case WIFI_TEMP:
        run_t.wifi_set_temperature = inputBuf[0];
        state = 0;
        run_t.decodeFlag = 1;
        break;

    case WIFI_CMD:
        wifi_link_counter = 0;
        run_t.wifiCmd[0] = inputBuf[0];
        state = 0;
        run_t.decodeFlag = 1;
        break;

    case WIFI_BEIJING_TIME:
        run_t.dispTime_hours = inputBuf[0];
        state = 4;
        break;

    case WIFI_SET_TIMING:
        run_t.dispTime_hours = inputBuf[0];
        run_t.gTimer_key_timing = 0;
        state = 0;
        run_t.decodeFlag = 1;
        break;

    case WIFI_REF_DATA:
        run_t.gDry = inputBuf[0];
        state = 4;
        break;

    case MAIN_BOARD_COPY_CMD:
        copy_mainboard_cmd = inputBuf[0];
        receive_copy_cmd(inputBuf[0]);
        state = 0;
        break;
    }
}

void handleWifiInfo(void)
{
    switch (inputBuf[0])
    {
    case 0x01:
        wifi_link_counter = 0;
        run_t.wifi_link_cloud_flag = WIFI_CLOUD_SUCCESS;
        state = 0;
        run_t.decodeFlag = 1;
        break;

    case 0x00:
        wifi_link_counter++;
        if (wifi_link_counter > 100)
        {
            run_t.wifi_link_cloud_flag = WIFI_CLOUD_FAIL;
        }
        state = 0;
        break;

    case 0x52: // link tencent cloud receive return data flag
        run_t.wifi_led_fast_blink_flag = 1;
        state = 0;
        run_t.decodeFlag = 0;
        break;
    }
}

static void handleState4(void)
{
    if (run_t.wifi_orderByMainboard_label == WIFI_BEIJING_TIME)
    {
        run_t.dispTime_minutes = inputBuf[0];
        state = 5;
    }
    else if (run_t.wifi_orderByMainboard_label == PANEL_DATA)
    {
        run_t.gReal_humtemp[1] = inputBuf[0]; // Temperature value
        state = 0;
        run_t.decodeFlag = 1;
    }
    else if (run_t.wifi_orderByMainboard_label == WIFI_REF_DATA)
    {
        run_t.gPlasma = inputBuf[0];
        state = 5;
    }
}

static void handleState5(void)
{
    if (run_t.wifi_orderByMainboard_label == WIFI_BEIJING_TIME)
    {
        run_t.dispTime_seconds = inputBuf[0];
        run_t.send_app_timer_total_minutes_value = run_t.dispTime_seconds * 60;
        run_t.decodeFlag = 1;
        state = 0;
    }
    else if (run_t.wifi_orderByMainboard_label == WIFI_REF_DATA)
    {
        run_t.gUltrasonic = inputBuf[0];
        ULTRASONIC_LED_OnOff(run_t.gUltrasonic);
        state = 0;
    }
}
        
/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}
	
}
/**
  * @brief  UART错误回调函数，处理USART1通信错误
  * @param  huart: UART句柄指针
  */

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) 
{
    

	if (huart->Instance == USART1) {
       
  
	    /* 1. 清除所有可能出现的错误标志 */
	    // 使用单条语句清除多个标志（更高效）
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

	    /* 2. 读取状态和数据寄存器（清空残留数据）*/
	    // 使用UNUSED宏避免编译器警告（如果不需要实际值）
	    //UNUSED(uint32_t temp_isr = huart->Instance->ISR);  // 读取ISR会清除部分标志
	    //UNUSED(uint32_t temp_rdr = huart->Instance->RDR);  // 清空接收寄存器
	      /* 2. 清空寄存器（简洁写法）*/
		    (void)huart->Instance->ISR;  // 清除状态标志
		    (void)huart->Instance->RDR;  // 清空接收数据

	    /* 3. 重启接收（带错误检查）*/
	    if (HAL_UART_GetState(huart) == HAL_UART_STATE_READY) {
	        HAL_UART_Receive_IT(huart, inputBuf, 1);  // 重新启动单字节中断接收
	    } else {
	        // 可选：硬件复位USART（严重错误时）
	        __HAL_UART_DISABLE(huart);
	        __HAL_UART_ENABLE(huart);
	        HAL_UART_Receive_IT(huart, inputBuf, 1);
	    }

	    /* 4. 可选：记录错误日志或触发报警 */
	    Error_Counter++;  // 全局错误计数器
    }
	else if (huart->Instance == USART2){

		 /* 1. 清除所有可能出现的错误标志 */
	    // 使用单条语句清除多个标志（更高效）
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

	    /* 2. 读取状态和数据寄存器（清空残留数据）*/
	    // 使用UNUSED宏避免编译器警告（如果不需要实际值）
	    //UNUSED(uint32_t temp_isr = huart->Instance->ISR);  // 读取ISR会清除部分标志
	    //UNUSED(uint32_t temp_rdr = huart->Instance->RDR);  // 清空接收寄存器
		  /* 2. 清空寄存器（简洁写法）*/
    (void)huart->Instance->ISR;  // 清除状态标志
    (void)huart->Instance->RDR;  // 清空接收数据

//		  /* 3. 重启接收（带错误检查）*/
//	    if (HAL_UART_GetState(huart) == HAL_UART_STATE_READY) {
//	          // 重新启动单字节中断接收
//	    }

	}
}


