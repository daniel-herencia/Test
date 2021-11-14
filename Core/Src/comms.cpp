/*
 * comms.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: guifre
 */

#include "comms.h"

void telecommand(void){
//Hem de definir que volem fer aquí. //Enviar telecomands a la OBC??
	//todo pensar com tractarem cada cas/parlar amb payload/comms


};

void configcomms(void){
	/*	i) Inicialize power in the antenna and the transceiver
	 *
	 *
	 *
	 *
	 *
	 */

};

void sendtelemetry(){
//Enviar un paquet de telemetria
	/* Hem de pensar amb una estructura:
	 *Agafem telemetria
	 *La segmentem
	 *Enviem
	 */
};

void transmitpck(){
	/*Agafem un paquet
	 * l'enviem
	 */
}


void receivepck(){
	/*Agafem el paquet rebut
	 * El desegmentem
	 * Llegim instrucció
	 */
}

void packing(){
	//Seria l'equivalent a segmentejar usat en Tx
}

void unpacking(){
	//Seria l'invers a segmentejar usat en Rx
}

