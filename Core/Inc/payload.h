/*
 * payload.h
 *
 *  Created on: Apr 20, 2021
 *      Author: guifre
 */

#ifndef INC_PAYLOAD_H_
#define INC_PAYLOAD_H_

#include "main.h"

/*configuration.system_state() must be 'true'
 *batterylevel must be at Nominal level
 * configuration.checkmemory()
 * Start preparing the payload to detect RF if needed (camera is already configured)
 * takes the photo and store it in memory*/
void payloadinit(void);

/*（1）capture a image command
*（2）read image data length command
*（3）read image data command
*（4）stop capture command*/
bool takePhoto(void);

/*Includes payloadinit() and takePhoto(), then stores it in memory*/
void receive(void);

#endif /* INC_PAYLOAD_H_ */
