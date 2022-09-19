#include "max3010x.h"


uint8_t	ID = 0;
float Temp_Integer = 0;
float Temp_Fraction = 0;
float Temp_Measure = 0;
uint16_t g_fft_index=0;
float SpO2 = 0;
u8 PR_ = 0;
uint16_t Heart_Rate=0;
uint16_t Raw_Data[15][2] = {0};
u8 max30100_recive_flag = 0;
float sp02_ = 50.0;
 u16 my_counter=0;

uint32_t IR = 0;
uint32_t RED = 0;
#define max10300_WR_address 0xAE
#define I2C_WR	0		/* Write controle bite */
#define I2C_RD	1		/* Read controle bite*/

 

/********************************/
/*MAX30100 FILTERS*/
#define MEAN_FILTER_SIZE        15  
int32_t msum = 0;
int32_t mvalues[MEAN_FILTER_SIZE];
int32_t mindex = 0;
int32_t mcount = 0;



//This function is to write one byte in to the slave register
 uint8_t Write_One_Byte(uint8_t Register_Address,  uint8_t Word_Data)
{


	//Start the transaction
	IIC_Start();

	//send the address of the slave
	IIC_Send_Byte(max10300_WR_address | I2C_WR);	

	//waite Ack
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}

	//send the register address
	IIC_Send_Byte(Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}
	
	// Write the word data
	IIC_Send_Byte(Word_Data);

	//wait Ack.
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}

	//Stop to finish the transaction
	IIC_Stop();
	return 1;	//return 1

cmd_fail:    //goto instruction 
	IIC_Stop();
	return 0;
}


//Read one byte 
uint8_t Read_One_Byte(uint8_t Register_Address)
{
	uint8_t data;
	IIC_Start();
	IIC_Send_Byte(max10300_WR_address | I2C_WR);	

	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}

	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}
	
	IIC_Start();

	
	IIC_Send_Byte(max10300_WR_address | I2C_RD);	
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}

	{
		data = IIC_Read_Byte(0);	

		IIC_NAck();	
	}
	
	IIC_Stop();
	return data;	

cmd_fail: 
	IIC_Stop();
	return 0;
}



void MAX30100_FIFO_Read(uint8_t Register_Address,uint16_t  Word_Data[][2],uint8_t count)
{
	uint8_t i=0;
	uint8_t no = count;
	uint8_t data1, data2;
	
	IIC_Start();
	IIC_Send_Byte(max10300_WR_address | I2C_WR);	

	
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}
	IIC_Send_Byte((uint8_t)Register_Address);
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}
	IIC_Start();	
	IIC_Send_Byte(max10300_WR_address | I2C_RD);	
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	
	}

	
	while (no)
	{
		data1 = IIC_Read_Byte(1);	
		data2 = IIC_Read_Byte(1);
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //
		
		
		data1 = IIC_Read_Byte(1);	
		data2 = IIC_Read_Byte(1);
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 
    //printf("%d\r\n",Word_Data[i][1]);
		no--;	
		i++;
	}
	
	//TIME=!TIME;
	IIC_NAck();
	IIC_Stop();

cmd_fail: 
	IIC_Stop();
}


void MAX30100_Init()
{
	uint8_t temp1;
	uint8_t temp2;
	u8 sr1,sr2;
	ID = Read_One_Byte(MAX30100_REG_Part_ID);
	printf("\r\n The MAX ID is 0x%x\n",ID);
	if(ID == 0x11)   //is ok
	{
		
		//Write_One_Byte(MAX30100_REG_Mode_Configuration, MAX30100_MC_TEMP_EN);
		//temp1 = Read_One_Byte(MAX30100_REG_Temp_Integer);
		//printf("%d\n",temp1);
		
		
		//Temp_Integer = 1 * temp1;
		//printf("%f\n",Temp_Integer);
		
		//temp2 = Read_One_Byte(MAX30100_REG_Temp_Fraction);
		//printf("0x%x\n",temp2);
		//Temp_Fraction = 0.0625 * temp2;
		//printf("%f\n",Temp_Fraction);
		//Temp_Measure = Temp_Integer + Temp_Fraction;
		//printf("Temp: %f\n",Temp_Measure);
		
		
		//SPO2_HR
		setMode(MAX30100_MODE_SPO2_HR);
		
		//enable interruption
	  Write_One_Byte(MAX30100_REG_Interrupt_Enable, Interrupt_Enable);
		//clear interruption
		Write_One_Byte(MAX30100_REG_Interrupt_Status, Interrupt_Clear);
		//set 11ma for the leds
		setLedsCurrent(MAX30100_LED_CURR_11MA, MAX30100_LED_CURR_11MA);
		
		
		
		//set the sample rate 
		 //sr1 = Read_One_Byte(MAX30100_REG_SpO2_Configuration);
	  //printf("\r\n SR1: 0x%x \r\n",sr1);
		setSamplingRate(MAX30100_SAMPRATE_100HZ);
		// sr2 = Read_One_Byte(MAX30100_REG_SpO2_Configuration);
	  //printf("\r\n SR2: 0x%x \r\n",sr2);
		
		
		
		
		//set the pulse width
		
		//MAX30100_SPC_PW_1600US_16BITS
		setLedsPulseWidth( MAX30100_SPC_PW_1600US_16BITS);
		//SpO2High Resolution Enable£¬16bits and current 1.6ms
		setHighresModeEnabled(1);
		//initialization of FIFO
		Write_One_Byte(MAX30100_REG_FIFO_write_pointer, 0x00);
		Write_One_Byte(MAX30100_REG_FIFO_Overflow_Counter, 0x00);
		Write_One_Byte(MAX30100_REG_FIFO_Read_Pointer, 0xff);
		//waitting  write pointer eq read pointer to interrupts INTERRUPT_REG_A_FULL		
	} 
}


void setMode(Mode mode)
{

	Write_One_Byte(MAX30100_REG_Mode_Configuration, mode);
//	printf("%d\r\n",Write_One_Byte(MAX30100_REG_MODE_CONFIGURATION, mode));
}


/*Set the sample rate function*/
void setSamplingRate(SamplingRate samplingRate)
{

	uint8_t previous; 
	previous=Read_One_Byte(MAX30100_REG_SpO2_Configuration);
	Write_One_Byte(MAX30100_REG_SpO2_Configuration, (previous&0xe3) | (samplingRate << 2)); 
	          
}


/*Set Leds Pulse function*/
void setLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
	uint8_t previous;  
	previous=Read_One_Byte(MAX30100_REG_SpO2_Configuration);
	Write_One_Byte(MAX30100_REG_SpO2_Configuration, (previous & 0xfc) | ledPulseWidth);
}



/*Set current function*/
void setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
	Write_One_Byte(MAX30100_REG_LED_Configuration, redLedCurrent << 4 | irLedCurrent);
}


/*SpO2High Resolution Enable function */
void setHighresModeEnabled(uint8_t enabled)
{

	 uint8_t previous; 
	 previous=Read_One_Byte(MAX30100_REG_SpO2_Configuration);
	 if (enabled) 
	{
	 Write_One_Byte(MAX30100_REG_SpO2_Configuration, previous | MAX30100_SPC_SPO2_HI_RES_EN);
   } 
	else
	{
     Write_One_Byte(MAX30100_REG_SpO2_Configuration, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
   }
}


void Heart_Rate_SpO2(void)
{
	int i;
	//for(i = 0; i < 1; i++ )//50
	//{
	   //if (max30100_recive_flag ==1)
		   // my_counter++;
		MAX30100_Test_Fun();
	//}
	
}

void MAX30100_Test_Fun(void)
{
	uint16_t temp;
	
	temp = Read_One_Byte(MAX30100_REG_Interrupt_Status);
	
	if(temp & FIFO_Almost_Full_Flag)
	{     
		
		  TIME=!TIME;
		
		    Write_One_Byte(MAX30100_REG_Interrupt_Status, Interrupt_Clear);
		    MAX30100_FIFO_Read(MAX30100_REG_FIFO_Data, Raw_Data, 15);
	      max30100_recive_flag =1;
		    
  }
	}


int32_t DCRemove(int32_t value,int32_t *cw)
{
   int32_t oldcw = *cw;
   *cw = value + 0.95 * *cw;
   return *cw - oldcw;
}


int32_t LowPassButterworthFilter(int32_t value,Filter_Data *filter_data)
{  
   filter_data->value[0] = filter_data->value[1];
   filter_data->value[1] = (2.452372752527856026e-1 * value) + (0.50952544949442879485 * filter_data->value[0]);
   return filter_data->value[0] + filter_data->value[1];
}




        /**--------------Find the maximum between i-a and i+b -------------------------------------/
         * Name : extreme
         * arg  :
         *       samples    : an array of type float . Contains the ecg samples
         *       i          : an integer . The index of the current sample
         *       a          : and integer. the value to subtract from the current index.
         *       b          : and integer. the value to add from the current index.
         *return:
         *                 : return the max value of that particular interval
 -----------------------------------------------------------------------------------------*/
    	 int extreme(double *samples,int i,int a, int b) {

    		double max =0;
    		int index = 0;
				 int j=0;
    		for(j = i-a ; j<= i+b ; j++) {

    			if(samples[j] > max) {
    				max = samples[j];
    				index = j;
    				}

    		}

    	    return index;
    	}


	
			

	uint16_t  PR_Algorithm(double* ecg,int *RR,int fs,int dataSize)
{

	      int I[100] ={0};         //Hold the index
    		int k = 26;   // QRS complex interval can be change for PPG
    		int Index_prev = 0,Index=0;
				float P=0;
				int delta = 45;
				int i=0,j=0,l=0;
				int PRI=0;

       //---------------Check the first 3s data---------------------------------------------------------
    		
							//TIM_Cmd(TIM3,ENABLE);

    				   while(i<=dataSize-k){
                  
									P= (ecg[i]-ecg[i-k])*(-ecg[i]+ecg[i+k]);
								 
								 
									if( P < 0.0 ){  //the slopes have opposite signs
																	
												 Index = extreme(ecg,i,k,k); //find the index
										    
												 if(Index > (Index_prev + delta) && i >= Index){
                               
															I[j] = Index;
													 
    				    			        if(j>=1) {
    				    				       PRI +=(I[l+1]-I[l]);
    				    				       l = l+1;
															}
													 
															 j++;
															 i=Index+k+delta;
															 Index_prev=Index;
									        }
								
						      }
									
							  i = i + 1;
									
}
							 
//int PRR1 = PRI/(l-1);
	//int PRR = 60*fs/(PRI/(l-1));
	//printf("%d\r\n",(int)(PRI/(l-1)));
  return   60*fs/(PRI/(l-1));
//return   PRR;
}
void Init_I2C1()
{
   GPIO_InitTypeDef GPIO_InitStructure;
   I2C_InitTypeDef I2C_InitStruct;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

   I2C_InitStruct.I2C_ClockSpeed = 100000;
   I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
   I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
   I2C_InitStruct.I2C_OwnAddress1 = 0x00;
   I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
   I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_Init(I2C1, &I2C_InitStruct);

   I2C_Cmd(I2C1, ENABLE);

}




