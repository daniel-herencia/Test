/*
 * payload.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: guifre
 */

#include "payload.h"
#include "definitions.h"
#include "main.c"

void payloadinit(){
	system_state();
}

void receive(){
	payloadinit();
	takePhoto(); 	/*Store it in memory from definitions.Image.bufferImage*/
}

bool takePhoto(){
	Image image; /*Declaration of an Union Image type*/
	uint8_t capture[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
	uint8_t supposedAckCapture[5] = {0x76, 0x00, 0x36, 0x00, 0x00};
	uint8_t ackCapture[5];
	uint8_t readImageDataLength[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
	/*supposed ack of readImageDataLength = 76 00 34 00 04 00 00 XX YY 	where XX YY -> image length
	 * XX -> high byte, YY -> low byte*/
	uint8_t ackReadImageDataLength[8];
	uint16_t dataLength; /*FALTA PASSAR A DECIMAL*/
	uint8_t readImageData[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00,
	                            0x00, 0x00, 0x00, 0x00, 0x00, 0x0A};
	uint8_t imageData[14 + dataLength +14];
	uint8_t stopCapture[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
	uint8_t supposedAckStopCapture[5] = {0x76, 0x00, 0x36, 0x00, 0x00};
	uint8_t ackStopCapture[5];

	HAL_UART_Transmit(&huart1, capture, 5, 1000); 		/*Transmit capture command*/
	HAL_UART_Receive(&huart1, ackCapture, 5, 1000);		/*Receive ack capture*/
	if(ackCapture == supposedAckCapture){
		HAL_UART_Transmit(&huart1, readImageDataLength, 5, 1000); 		/*Transmit readImageDataLength command*/
		HAL_UART_Receive(&huart1, ackReadImageDataLength, 8, 1000);
		readImageData[12] = ackReadImageDataLength[7];		/*Copy the high length bit*/ /*SHIFT 8 POSICIONS*/
		readImageData[13] = ackReadImageDataLength[8];		/*Copy the low length bit*/
		dataLength = readImageData[12]<<8;
		dataLength = readImageData[13] | dataLength;
		HAL_UART_Transmit(&huart1, readImageData, 16, 1000); 	/*Transmit read image command*/
		/*Supposed to receive = 76 00 32 00 00 FF D8 。。。。。。FF D9 76 00 32 00 00    where "...." is the data*/
		HAL_UART_Receive(&huart1, imageData, dataLength, 1000);
		for(uint8_t i=5; i<(5+dataLength); i++){				/*MIRAR DE FER MEMCOPY PER COPIAR TOT EL BUFFER*/
			image.fields.bufferImage[i-5] = imageData[i];		/*Store the data in the array of Union Image*/
		}
		HAL_UART_Transmit(&huart1, stopCapture, 5, 1000);
		HAL_UART_Receive(&huart1, ackStopCapture, 5, 1000);
		if(ackStopCapture == supposedAckStopCapture){
			return true;
		}
	/*Erase last image and stored in memory and store the new image.fields.bufferImage in the FLASH Memory*/
	HAL_FLASHEx_Erase(pEraseInit, SectorError); /*ACABAR DE MIRAR COM FUNCIONA*/
	HAL_FLASH_Program_IT(TypeProgram, Address, Data);


	}
}


