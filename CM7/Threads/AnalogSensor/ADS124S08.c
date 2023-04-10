/*****************************************************/

#include "ADS124S08.h"
#include <stdio.h>
/*
 * Reads a single register contents from the specified address
 *
 * \param regnum identifies which address to read
 *
 */
extern SPI_HandleTypeDef hspi4;

static void spi_wr_data(SPI_HandleTypeDef *hspi, uint8_t *tx_data, uint8_t *rx_data)
{
	HAL_SPI_TransmitReceive(&hspi4, tx_data, rx_data, 1, 100);
}

uint8_t regRead(uint8_t regnum)
{
    uint8_t ulDataTx[1];
    uint8_t ulDataRx[1];

	ulDataTx[0] = 0x00;
	ulDataRx[0] = 0x00;

	ulDataTx[0] = REGRD_OPCODE_MASK + (regnum & 0x1f);
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);

	ulDataTx[0] = 0x00;
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);

	ulDataTx[0] = 0x00;
	ulDataRx[0] = 0x00;

	spi_wr_data(&hspi4, ulDataTx, ulDataRx);

	return ulDataRx[0];
}
/*
 * Writes a single of register with the specified data
 *
 * \param regnum addr_mask 8-bit mask of the register to which we start writing
 * \param data to be written
 *
 */
void WriteReg(uint8_t regnum, uint8_t data)
{
	uint8_t ulDataTx[1],ulDataRx[1];
	ulDataRx[0] = 0;
	ulDataTx[0] = 0;

	ulDataTx[0] = REGWR_OPCODE_MASK + (regnum & 0x1f);   	// Register address
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);

	ulDataTx[0] = 0x00;
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);             	// Lend data

	ulDataTx[0] = data;
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);				// data

}

/*
 * Sends a command to the ADS124S08
 *
 * \param op_code is the command being issued
 *
 */
void sendCommand(uint8_t op_code)
{
	uint8_t command[1];
	uint8_t tem[1];
	tem[0] = 0;

	command[0] = op_code;
	spi_wr_data(&hspi4, command, tem);
}

/*
 *
 * Read the last conversion result
 *
 */
uint32_t dataRead()
{
	uint32_t result = 0;
	uint8_t ulDataTx[1],ulDataRx[1];
	ulDataTx[0] = 0x00;
	ulDataRx[0] = 0x00;

	// according to datasheet chapter 9.5.4.2 Read Data by RDATA Command
	sendCommand(RDATA_OPCODE_MASK);

	ulDataTx[0] = 0x00;
	ulDataRx[0] = 0x00;
	// get the conversion data (3 bytes)
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);
	result = ulDataRx[0];


	ulDataTx[0] = 0x00;
	ulDataRx[0] = 0x00;
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);
	result = (result << 8) + ulDataRx[0];

	ulDataTx[0] = 0x00;
	ulDataRx[0] = 0x00;
	spi_wr_data(&hspi4, ulDataTx, ulDataRx);
	result = (result << 8) + ulDataRx[0];

	return (result);
}

/*
 * Initializes device for use in the ADS124S08 EVM.
 * return True if device is in correct hardware defaults and is connected
 *
 */

uint8_t InitDevice_ADC()
{
	osDelay(10);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);		/*	PA11 ----> NCC	*/
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_9, GPIO_PIN_SET);			/*	PD12 ----> ADC_RESET	*/
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_10, GPIO_PIN_RESET);		/*	PD13 ----> ADC_START	*/

	sendCommand(STOP_OPCODE_MASK);

	uint8_t registers[NUM_REGISTERS];

	/* Default register settings */
	registers[ID_ADDR_MASK] 		= 0x08;
	registers[STATUS_ADDR_MASK]		= 0x00;
	registers[INPMUX_ADDR_MASK]		= 0x4C; //Set MUX to AIN4 & AINCOM
	registers[PGA_ADDR_MASK] 		= 0x00; //Disable PGA
	registers[DATARATE_ADDR_MASK] 	= 0x14; //Set Datarate:disable G-Chop,internal CLK,Continous Conversion, Low-Latency Filter, Datarate:20SPS
	registers[REF_ADDR_MASK] 		= 0x00; //disable Ref-Monitor, disable positive reference buffer,dis. neg RefBuf,select ref input pins,dis. inter
	registers[IDACMAG_ADDR_MASK] 	= 0x00;
	registers[IDACMUX_ADDR_MASK] 	= 0xFF;	//Disconnected IDAC1 and IDAC2
	registers[VBIAS_ADDR_MASK] 		= 0x00;
	registers[SYS_ADDR_MASK]		= 0x00;
	registers[OFCAL0_ADDR_MASK] 	= 0x00;
	registers[OFCAL1_ADDR_MASK] 	= 0x00;
	registers[OFCAL2_ADDR_MASK] 	= 0x00;
	registers[FSCAL0_ADDR_MASK] 	= 0x00;
	registers[FSCAL1_ADDR_MASK] 	= 0x00;
	registers[FSCAL2_ADDR_MASK] 	= 0x40;
	registers[GPIODAT_ADDR_MASK] 	= 0xF0;
	registers[GPIOCON_ADDR_MASK]	= 0x00;
	osDelay(10);

	if( regRead(0x1) == 0x80 )
	{
		WriteReg(0x1, 0);
	}

	for( int i = 0x02; i < 18 ; i++)
	{

		WriteReg(i, registers[i]);
	}

	osDelay(10);
	sendCommand(START_OPCODE_MASK);
	return 1;

}

double Getdata_ADC(uint8_t chanel_an)
{
	uint32_t temp_an;
	double data_an;

	sendCommand(STOP_OPCODE_MASK);
	WriteReg(INPMUX_ADDR_MASK, chanel_an);
	sendCommand(START_OPCODE_MASK);
	osDelay(100);

	temp_an = dataRead();

	//printf("temp_an = %lu\n\r", (unsigned long)temp_an);
	data_an = (double) temp_an/DEVIDER_DATA;
	return data_an;
}
