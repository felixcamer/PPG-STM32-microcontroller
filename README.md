# PPG-STM32-microcontroller
The bellow video is a short demonstration of a Photoplethysmography (PPG) data acquisition device built by me:

https://user-images.githubusercontent.com/22806623/190244605-d413236d-70bc-4878-825f-9dd9f621895f.mp4


As shown below, the system is composed of a MAX30100 sensor, an STM32F103 MCU, a Bluetooth module and a LCD.
The MAX30100 communicates with the MCU through an I2C protocol, the BLE communicate with MCU througth UART protocol.
The segnal acquired by the sensor was passed througth a two digitals filters; a smooth filter and a low-pass filter of cut-off frequency of 10Hz.

As you can see, the system can compute the heart rate and the oxygen level. Algorithms were first written in MATLAB to verify their accuracy and then transferred to the MCU.

![flowchart](https://user-images.githubusercontent.com/22806623/190251729-bbd664c1-b079-40fe-b04c-85ea1584f9f9.png)

The following picture shows the PCB of the system. The board also has the electrocardiogram analog front end ic (ads1292r):

![Board](https://user-images.githubusercontent.com/22806623/190245347-27a93313-1051-4f4b-a200-3ace11ae77a5.png)

