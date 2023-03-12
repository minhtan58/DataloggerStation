/*
 * network_handler.h
 *
 *  Created on: Oct 20, 2022
 *      Author: minht
 */

#ifndef NETWORK_NETWORK_HANDLER_H_
#define NETWORK_NETWORK_HANDLER_H_

void Network_Task(void const *argument);
void lwip_sntp_recv_callback(uint32_t time);

#endif /* NETWORK_NETWORK_HANDLER_H_ */
