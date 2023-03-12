/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*
 * ADS124S08.h
 *
 */

#include "stm32h7xx_hal.h"

#define NUM_REGISTERS 	17

/*
 * Address masks used for register addressing with
 * either a REGRD of REGWR mask
 *
 */
#define ID_ADDR_MASK		0x00
#define STATUS_ADDR_MASK	0x01
#define INPMUX_ADDR_MASK	0x02
#define PGA_ADDR_MASK		0x03
#define DATARATE_ADDR_MASK	0x04
#define REF_ADDR_MASK		0x05
#define IDACMAG_ADDR_MASK	0x06
#define IDACMUX_ADDR_MASK	0x07
#define VBIAS_ADDR_MASK		0x08
#define SYS_ADDR_MASK		0x09
#define OFCAL0_ADDR_MASK	0x0A
#define OFCAL1_ADDR_MASK	0x0B
#define OFCAL2_ADDR_MASK	0x0C
#define FSCAL0_ADDR_MASK	0x0D
#define FSCAL1_ADDR_MASK	0x0E
#define FSCAL2_ADDR_MASK	0x0F
#define GPIODAT_ADDR_MASK	0x10
#define GPIOCON_ADDR_MASK	0x11

/* Opcode masks */
#define NOP_OPCODE_MASK		0x00
#define WAKE_OPCODE_MASK	0x02
#define SLEEP_OPCODE_MASK	0x04
#define RESET_OPCODE_MASK	0x06
#define START_OPCODE_MASK	0x08
#define STOP_OPCODE_MASK	0x0A
#define SFOCAL_OPCODE_MASK	0x19
#define SYOCAL_OPCODE_MASK	0x16
#define SYGCAL_OPCODE_MASK	0x17
#define RDATA_OPCODE_MASK	0x12
#define REGRD_OPCODE_MASK	0x20
#define REGWR_OPCODE_MASK	0x40

#define CHANNEL_0_EN		0x0C
#define CHANNEL_1_EN		0x1C
#define CHANNEL_2_EN		0x2C
#define CHANNEL_3_EN		0x3C
#define CHANNEL_4_EN		0x4C
#define CHANNEL_5_EN		0x5C
#define CHANNEL_6_EN		0x6C
#define CHANNEL_7_EN		0x7C
#define CHANNEL_8_EN		0x8C
#define CHANNEL_9_EN		0x9C
#define CHANNEL_10_EN		0xAC
#define CHANNEL_11_EN		0xBC


#define DEVIDER_DATA		1677722
#define N_UPDATE_DATA		9
#define SIZE_NAME			8

uint8_t regRead(uint8_t regnum);
void WriteReg(uint8_t regnum, uint8_t data);
uint8_t InitDevice_ADC();
double Getdata_ADC(uint8_t chanel_an);
