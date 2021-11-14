/*
 * comms.h
 *
 *  Created on: Apr 20, 2021
 *      Author: guifre
 */

#ifndef INC_COMMS_H_
#define INC_COMMS_H_

#include "main.h"

/*Telecommand received from comms
/*unpacking() -> redirect to the function we need to execute*/
void telecommand(void);

void configcomms(void);

/*send the housekeeping (system_state) to the GS*/
void sendtelemetry();

/*Includes the packing of the data and telemetry(SPG4) we want to transmit to GS*/
void transmitpck();

/*Includes the unpacking and executes the telecomands received from GS*/
void receivepck(void);

/*The size of each packet is 59Bytes (51Bytes of data)*/
/*Each packet must contain the following fields: ([telecomand],[size of data], [data])*/
void packing();
void unpacking();

#endif /* INC_COMMS_H_ */

