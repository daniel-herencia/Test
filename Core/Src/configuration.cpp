/*
 * configuration.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: guifre
 */
#include "configuration.h"
#include "main.h"
#include "main.c"
#include "definitions.h"

static const uint8_t GYRO_ADDR = 0x68 << 1; //gyroscope address, 0x68 or 0x69 depending on the SA0 pin
static const uint8_t MAG_ADDR = 0x30 << 1; //magnetometer address


int checkbatteries(BatteryLevels batterylevel){

	double actual_level, percentage;
	actual_level = reading_EPS; //reading_EPS is still to be defined, will be acquireVoltage()?, acquirecurrent()?
	percentage = (actual_level/MAX_BAT_LEVEL)*100;
	batterylevel.fields.totalbattery = (int) percentage;
	return batterylevel.fields.totalbattery;

}


void deployment(){
	while(system_state() && !deployment_state){
		/*Give high voltage to the resistor to burn the wire, TBD TIME AND VOLTAGE*/
	}
	deployment_state = true; /*Must be stored in FLASH memory in order to keep it if the system is rebooted*/
}


bool payloadconfig(){
	/*POWER ON?*/
	uint8_t reset[4] = {0x56, 0x00, 0x26, 0x00};
	uint8_t supposedAckReset[4] = {0x76, 0x00, 0x26, 0x00};
	uint8_t ackReset[4];
	uint8_t setCompressibility[9] = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, 0xFF}; //0xFF means maximum compressed*/
	uint8_t supposedAckSetCompressibility[5] = {0x76, 0x00, 0x31, 0x00, 0x00};
	uint8_t ackSetCompressibility[5];

	sleep_for(std::chrono::milliseconds(DELAY_CAMERA)); /*Delay 2.5s*/
	HAL_UART_Transmit(&huart1, reset, 4, 1000); 		/*Transmit reset command*/
	HAL_UART_Receive(&huart1, ackReset, 4, 1000);		/*Receive ack reset*/
	if(ackReset == supposedAckReset){
		HAL_UART_Transmit(&huart1, setCompressibility, 9, 1000); 	/*Transmit compressibility of image*/
		HAL_UART_Receive(&huart1, ackSetCompressibility, 4, 1000);	/*Receive ack compressibility*/
		if(ackSetCompressibility == supposedAckSetCompressibility){
			return true;
		}
	}
}


bool check_position() {
	//check the PQ position.
	//Region of contact with GS => comms_state = true
}

/* Function that simulates the "INIT" state
 * Wait for a while until we start acting on the pocketqube once it is deployed
 */
void init(){
	if(!system_state()) currentState = CONTINGENCY;
	else {
		if(!detumble_state) detumble();
		if(!deployment_state)	deployment();
		//Just in the PocketQube with the RF antenna
		if(!deploymentRF_state) deploymentRF();
		currentState = IDLE;
	}
}

void initsensors() {
	//todo mirar com inicialitzar sensors (magneto, giros, sensors temp...)
	HAL_StatusTypeDef ret;
	//GYROSCOPE CONFIGURATION
	ret = HAL_I2C_Master_Transmit(&hi2c1, GYRO_ADDR, (uint8_t*)0x1A, 1, 1000); //write in the register 0x1A
	//wait for ACK?
	if (ret != HAL_OK) {

	} else {
		HAL_I2C_Master_Transmit(&hi2c1, GYRO_ADDR, /*data to register 0x1A*/, 1, 1000);
	}
	ret = HAL_I2C_Master_Transmit(&hi2c1, GYRO_ADDR, (uint8_t*)0x1B, 1, 1000); //write in the register 0x1B
	//wait for ACK?
	if (ret != HAL_OK) {

	} else {
		HAL_I2C_Master_Transmit(&hi2c1, GYRO_ADDR, /*data to register 0x1B*/, 1, HAL_MAX_DELAY);
	}

	//MAGNETOMETER CONFIGURATION mirar a quin registre s'ha d'escriure
	ret = HAL_I2C_Master_Transmit(&hi2c1, MAG_ADDR, /**/, 1, HAL_MAX_DELAY);
	//wait for ACK?


}

bool system_state(){
	if(checkbatteries() < NOMINAL) return false;

	//checkbatteries();
	if (!checktemperature()) return false;
	//EPS_state(); ???
	//illumination_state(); ???
	return true;
}

bool checktemperature(){
	Temperatures temperatures = acquireTemp();
	int i, cont = 0;
	for (i=1; i<=7; i++){  //number of sensors not defined yet

		switch(i) {

		case 1:
			if (temperatures.fields.tempbatt <= TEMP_MIN) heater(1);
			else heater(0);
		break;

		case 2:
			if (temperatures.fields.temp1 > TEMP_MAX) cont ++;
		break;

		case 3:
			if (temperatures.fields.temp2 > TEMP_MAX) cont ++;
		break;

		case 4:
			if (temperatures.fields.temp3 > TEMP_MAX) cont ++;
		break;

		case 5:
			if (temperatures.fields.temp4 > TEMP_MAX) cont ++;
		break;

		case 6:
			if (temperatures.fields.temp5 > TEMP_MAX) cont ++;
		break;

		case 7:
			if (temperatures.fields.temp6 > TEMP_MAX) cont ++;
		break;

		default: break;
		//more cases should come as much as the final number of sensors
		}

	if (cont > 3) return false;
	else return true;
	}
}

void heater(int state){

}

bool checkmemory(){

}

void writeFlash(){
	HAL_FLASHEx_DATAEEPROM_Unlock();

	//todo flash memory distribution
}







