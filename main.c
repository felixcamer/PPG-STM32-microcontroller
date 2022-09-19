#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "max3010x.h"
//#include "stm32f4_i2c.h"



 
/*for max30100*/
 int32_t IRcw=0;
 int32_t REDcw=0,REDac_1,REDac_2 = 0,div_;

  Filter_Data redf = {0}; 

	u8 databuf[1]={0};
	
	

	
	
void init_port(){

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������  GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
}
  


int main(void)
{
	int j=0;
	u8 WriteBuf[3]={1,2,3};
	u8*n;
	*n=12;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	uart_init(115200);
	delay_init(168);//84
	
  //init_port();
	IIC_Init();
  MAX30100_Init();

	
	//I2C WITH STM LIBRARY
	
	
  while(1){
		
    //TIME =0;
		Heart_Rate_SpO2();
		TIME =0;
		if(max30100_recive_flag==1){
			
			
			
			my_counter++;
		if(my_counter==1){
		     TIME = 1;
			   my_counter=0;
		}
		 max30100_recive_flag = 0;
				//max30100_recive_flag=0;
		   for(j = 0; j < 15; j++)
				{
								int32_t REDac= DCRemove(Raw_Data[j][0] ,&IRcw);

							 //int32_t REDac=-Raw_Data[j][0];
								REDac = LowPassButterworthFilter(REDac,&redf);
										//printf("$ %d;", my_ecg_ppg_dat_dsp.point[0] );//16500
								
											 printf("$ %d;",-REDac);
					             //delay_ms(10);
										
										
				}
		
		
		
	   delay_ms(50);
			
	}
}


}