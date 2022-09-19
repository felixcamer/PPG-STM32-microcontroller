#ifndef __MAX3010X__
#define __MAX3010X__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "sys.h" 
#include "myiic.h"
#include "delay.h"
#include "I2C.h"

#define TIME PFout(5)	// 
//Interrupt Status
#define MAX30100_REG_Interrupt_Status				0x00
#define Interrupt_Clear											0x00
#define FIFO_Almost_Full_Flag								(1<<7)

//Interrupt Enable
#define MAX30100_REG_Interrupt_Enable				0x01
#define Interrupt_Enable										(1<<7)


//FIFO
#define MAX30100_REG_FIFO_write_pointer				0x02
#define MAX30100_REG_FIFO_Overflow_Counter		0x03
#define MAX30100_REG_FIFO_Read_Pointer				0x04
#define MAX30100_REG_FIFO_Data								0x05

#define MAX30100_REG_Part_ID								0xFF

/*raw ppg data */
extern uint16_t Raw_Data[15][2];
extern u8 max30100_recive_flag;
extern float sp02_;
extern u16 my_counter;
extern u8 PR_;
#define MEAN_FILTER_SIZE        15  
typedef struct{
   int32_t value[2];
}Filter_Data;


//Mode Configuration
#define MAX30100_REG_Mode_Configuration			0x06
typedef enum Mode {
    MAX30100_MODE_HRONLY    = 0x02,
    MAX30100_MODE_SPO2_HR   = 0x03
} Mode;


//SpO2 Configuration
#define MAX30100_REG_SpO2_Configuration			0x07
#define MAX30100_SPC_SPO2_HI_RES_EN					(1 << 6)


typedef enum LEDPulseWidth {
    MAX30100_SPC_PW_200US_13BITS    = 0x00,
    MAX30100_SPC_PW_400US_14BITS    = 0x01,
    MAX30100_SPC_PW_800US_15BITS    = 0x02,
    MAX30100_SPC_PW_1600US_16BITS   = 0x03
} LEDPulseWidth;


typedef enum SamplingRate {
  MAX30100_SAMPRATE_50HZ      = 0x00,
	MAX30100_SAMPRATE_100HZ     = 0x01,
	MAX30100_SAMPRATE_167HZ     = 0x02,
	MAX30100_SAMPRATE_200HZ     = 0x03,
	MAX30100_SAMPRATE_400HZ     = 0x04,
	MAX30100_SAMPRATE_600HZ     = 0x05,
	MAX30100_SAMPRATE_800HZ     = 0x06,
	MAX30100_SAMPRATE_1000HZ    = 0x07
} SamplingRate;


//LED Configuration
#define MAX30100_REG_LED_Configuration			0x09
typedef enum LEDCurrent {
	MAX30100_LED_CURR_0MA      = 0x00,
	MAX30100_LED_CURR_4_4MA    = 0x01,
	MAX30100_LED_CURR_7_6MA    = 0x02,
	MAX30100_LED_CURR_11MA     = 0x03,
	MAX30100_LED_CURR_14_2MA   = 0x04,
	MAX30100_LED_CURR_17_4MA   = 0x05,
	MAX30100_LED_CURR_20_8MA   = 0x06,
	MAX30100_LED_CURR_24MA     = 0x07,
	MAX30100_LED_CURR_27_1MA   = 0x08,
	MAX30100_LED_CURR_30_6MA   = 0x09,
	MAX30100_LED_CURR_33_8MA   = 0x0a,
	MAX30100_LED_CURR_37MA     = 0x0b,
	MAX30100_LED_CURR_40_2MA   = 0x0c,
	MAX30100_LED_CURR_43_6MA   = 0x0d,
	MAX30100_LED_CURR_46_8MA   = 0x0e,
	MAX30100_LED_CURR_50MA     = 0x0f
} LEDCurrent;

//Temperature
extern float Temp_Integer;
extern float Temp_Fraction;
extern float Temp_Measure;

/* Pulse detection parameters */
#define PULSE_MIN_THRESHOLD         20//300 is good for finger, but for wrist you need like 20, and there is shitloads of noise
#define PULSE_MAX_THRESHOLD         50
#define PULSE_GO_DOWN_THRESHOLD     1
#define PULSE_BPM_SAMPLE_SIZE       10 //Moving average size



typedef enum PulseStateMachine{
   PULSE_IDLE,
   PULSE_TRACE_UP,
   PULSE_TRACE_DOWN
}PulseStateMachine;



/*********************************/
//全局变量ID SpO2 Heart_Rate
//extern uint8_t	ID;
extern uint16_t Heart_Rate;
extern float SpO2;

#define MAX30100_REG_Temp_Integer						0x16
#define MAX30100_REG_Temp_Fraction					0x17
#define MAX30100_MC_TEMP_EN									(1 << 3)

//Procession
#define START_INDEX    12   //滤出低频干扰


void Heart_Rate_SpO2(void);
void MAX30100_Init();
void setMode(Mode mode);
void setSamplingRate(SamplingRate samplingRate);
void setLedsPulseWidth(LEDPulseWidth ledPulseWidth);
void setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
void setHighresModeEnabled(uint8_t enabled);
void MAX30100_FIFO_Read(uint8_t Register_Address,uint16_t  Word_Data[][2],uint8_t count);
uint8_t Read_One_Byte(uint8_t Register_Address);
uint8_t Write_One_Byte(uint8_t Register_Address,  uint8_t Word_Data);
//void MAX30100_FIFO_Read(uint8_t Adrr, uint16_t Word_Data[][2], uint8_t count);
u8 detectPulse(uint32_t sensor_value);
void MAX30100_Test_Fun(void);
int32_t DCRemove(int32_t value,int32_t *cw);
int32_t MeanDiff(int32_t M);
int32_t LowPassButterworthFilter(int32_t value,Filter_Data *filter_data);
uint16_t  PR_Algorithm(double* ecg,int *RR,int fs,int dataSize);


#define MAX30100_ADDRESS 0xAE
#define MAX30100_INT_STATUS 0x00
#define MAX30100_INT_ENABLE     0x01
#define MAX30100_FIFO_WR_PTR    0x02 
#define MAX30100_OVRFLOW_CTR    0x03 
#define MAX30100_FIFO_RD_PTR    0x04  
#define MAX30100_FIFO_DATA      0x05 
#define MAX30100_MODE_CONFIG    0x06 
#define MAX30100_SPO2_CONFIG    0x07 
#define MAX30100_LED_CONFIG     0x09 
#define MAX30100_TEMP_INTG      0x16 
#define MAX30100_TEMP_FRAC      0x17  
#define MAX30100_REV_ID         0xFE  
#define MAX30100_PART_ID        0xFF 
#define POR_PART_ID             0x11
typedef enum{
   i0,    // No current
   i4,    // 4.4mA
   i8,    // 7.6mA
   i11,   // 11.0mA
   i14,   // 14.2mA
   i17,   // 17.4mA
   i21,   // 20.8mA
   i24,	 // 24mA
   i27,   // 27.1mA
   i31,   // 30.6mA
   i34,   // 33.8mA
   i37,   // 37.0mA
   i40,   // 40.2mA
   i44,   // 43.6mA
   i47,   // 46.8mA
   i50    // 50.0mA
}ledCurrent;
//extern int32_t IRcw;
//extern int32_t REDcw;
extern uint32_t IR;
extern uint32_t RED;
typedef enum{ // This is the same for both LEDs
   pw200,    // 200us pulse
   pw400,    // 400us pulse
   pw800,    // 800us pulse
   pw1600    // 1600us pulse
}pulseWidth;

typedef enum{
   so2,	// SO2 interrupt
   hr,	// Heart-rate interrupt
   temp,	// Temperature interrupt
   full,	// FIFO full interrupt
}interruptSource;

// 0x00~0xFF = 0mA~51mA
static const uint8_t MAX_CURRENT = i50;
static const uint8_t FIX_IR_CURRENT = i21;
typedef enum{
   MAX3010x_MODE_HR_ONLY = 0x02,
   MAX3010x_MODE_SPO2_HR = 0x03,
   MAX3010x_MODE_MULTI_LED = 0x07
}Mode_;

typedef enum{
   sr50,    // 50 samples per second
   sr100,   // 100 samples per second
   sr167,   // 167 samples per second
   sr200,   // 200 samples per second
   sr400,   // 400 samples per second
   sr600,   // 600 samples per second
   sr800,   // 800 samples per second
   sr1000   // 1000 samples per second
}sampleRate;
#endif
