/*
 * sim.c
 *
 *  Created on: Dec 28, 2022
 *      Author: minht
 */

#include "sim4g_process.h"
#include <stdlib.h>
#include <string.h>
/* FTP variables */
char APNCommand[100] = {0};
char FTPCommand[100] = {0};
char FTPServer[50] = "27.72.56.216";
char FTPPort[6] = "2221";
char Username[32] = "tantm";
char Pass[32] = "Tan1234";

/* Sim variables */
char Sim_response[500] = {0};
char Sim_Rxdata[2] = {0};

/* File variables */
char fileName[20] = "dicom.txt";
char filePath[50] = "/TEST/";

extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;

void display(void* data)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)data,(uint16_t)strlen(data),1000);
}

void deleteBuffer(char* buf)
{
	int len = strlen(buf);
	for(int i = 0; i < len; i++)
	{
		buf[i] = 0;
	}
}

uint8_t Sim_sendCommand(char* command, char* response, uint32_t timeout)
{
	uint8_t answer = 0, count = 0;
	deleteBuffer(Sim_response);
	uint32_t time = HAL_GetTick();
	uint32_t time1 = HAL_GetTick();
	HAL_UART_Transmit(&huart8, (uint8_t *)command, (uint16_t)strlen(command), 1000);
	HAL_UART_Transmit(&huart8,(uint8_t *)"\r\n",(uint16_t)strlen("\r\n"),1000);
	do
	{
		while(HAL_UART_Receive(&huart8, (uint8_t *)Sim_Rxdata, 1, 1000) != HAL_OK)
		{
			if(HAL_GetTick() - time > timeout)
			{
				return 0;
			}
		}
                time = HAL_GetTick();
		Sim_response[count++] = Sim_Rxdata[0];
		while((HAL_GetTick() - time < timeout))
		{
			if(HAL_UART_Receive(&huart8, (uint8_t *)Sim_Rxdata, 1, 1000) == HAL_OK)
			{
				Sim_response[count++] = Sim_Rxdata[0];
				time1 = HAL_GetTick();
			}
			else
			{
				if(HAL_GetTick() - time1 > 100)
				{
					if(strstr(Sim_response,response) != NULL)
					{
						answer = 1;
					}
					else
					{
						answer = 0;
					}
					break;
				}
			}
		}
	}
	while(answer == 0);
	display(Sim_response);
	return answer;
}

void Sim_configGPRSnFTP(void)
{
	if(Sim_sendCommand("AT","OK",5000))
	{
		HAL_Delay(10);
		if(Sim_sendCommand("AT+SAPBR=1,1","OK",5000))
		{
			HAL_Delay(10);
			if(Sim_sendCommand("AT+FTPCID=1","OK",5000))		//Set FTP bearer profile identifier
			{
				HAL_Delay(10);
				if(Sim_sendCommand("AT+FTPPUTOPT=\"APPE\"","OK",5000))		//Set FTP bearer profile identifier
				{
					deleteBuffer(APNCommand);
					strcpy(FTPCommand,"AT+FTPSERV=\"");	//Set FTP server address
					strcat(FTPCommand,FTPServer);
					strcat(FTPCommand,"\"");
					if(Sim_sendCommand(FTPCommand,"OK",5000))
					{
						deleteBuffer(FTPCommand);
						HAL_Delay(10);
						strcpy(FTPCommand,"AT+FTPPORT=");	//Set FTP server port
						strcat(FTPCommand,FTPPort);
						if(Sim_sendCommand(FTPCommand,"OK",5000))
						{
							deleteBuffer(FTPCommand);
							HAL_Delay(10);
							strcpy(FTPCommand,"AT+FTPUN=\"");	//Set FTP username
							strcat(FTPCommand,Username);
							strcat(FTPCommand,"\"");
							if(Sim_sendCommand(FTPCommand,"OK",5000))
							{
								deleteBuffer(FTPCommand);
								HAL_Delay(10);
								strcpy(FTPCommand,"AT+FTPPW=\"");	//Set FTP password
								strcat(FTPCommand,Pass);
								strcat(FTPCommand,"\"");
								if(Sim_sendCommand(FTPCommand,"OK",5000))
								{
									deleteBuffer(FTPCommand);
									HAL_Delay(10);
									display("Configuration done!\r\n");
								}
							}
						}
					}
				}
			}
		}
	}
}

void Sim_uploadFileFromFTP(char* data)
{
	deleteBuffer(FTPCommand);
	HAL_Delay(10);
	strcpy(FTPCommand,"AT+FTPPUTNAME=\"");	//Set upload file name
	strcat(FTPCommand,fileName);
	strcat(FTPCommand,"\"");
	if(Sim_sendCommand(FTPCommand,"OK",5000))
	{
		deleteBuffer(FTPCommand);
		HAL_Delay(10);
		strcpy(FTPCommand,"AT+FTPPUTPATH=\"");	//Set upload file path
		strcat(FTPCommand,filePath);
		strcat(FTPCommand,"\"");
		if(Sim_sendCommand(FTPCommand,"OK",5000))
		{
			if(Sim_sendCommand("AT+FTPPUT=1","+FTPPUT: 1,1,",30000))
			{
				Sim_sendCommand("AT+FTPPUT=2,10","+FTPPUT: 2,10",30000);
				Sim_sendCommand(data,"+FTPPUT: 1,1",30000);
				Sim_sendCommand("AT+FTPPUT=2,0","+FTPPUT: 1,0",30000);
			}
		}
	}
}

void Sim_downloadFileFromFTP(void)
{
	uint8_t eof = 0;
	deleteBuffer(FTPCommand);
	HAL_Delay(10);
	strcpy(FTPCommand,"AT+FTPGETNAME=\"");	//Get file name
	strcat(FTPCommand,fileName);
	strcat(FTPCommand,"\"");
	if(Sim_sendCommand(FTPCommand,"OK",5000))
	{
		deleteBuffer(FTPCommand);
		HAL_Delay(10);
		strcpy(FTPCommand,"AT+FTPGETPATH=\"");	//Get file path
		strcat(FTPCommand,filePath);
		strcat(FTPCommand,"\"");
		if(Sim_sendCommand(FTPCommand,"OK",5000))
		{
			deleteBuffer(FTPCommand);
			HAL_Delay(10);
			if(Sim_sendCommand("AT+FTPGET=1","+FTPGET: 1,1",30000))
			{
				eof = 0;
				while(eof == 0)
				{
					Sim_sendCommand("AT+FTPGET=2,20","+FTPGET: 2,",30000);
					if(strstr(Sim_response,"+FTPGET: 2,0") != NULL)
					{
						eof = 1;
					}
				}
			}
		}
	}
}
