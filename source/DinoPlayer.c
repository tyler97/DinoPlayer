/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    DinoPlayer.c
 * @brief   Application entry point.
 */
#include "init.h"
/* TODO: insert other include files here. */
#include "dino.h"
#include <stdio.h>
#include "Scheduler/gsc_sch_core.h"
#include "Scheduler/gsc_sch_core_tick_isr.h"

/* TODO: insert other definitions and declarations here. */

/* Variables for TPM */
tpm_config_t tpmInfo;
tpm_chnl_pwm_signal_param_t tpmParam;

/* Variables for ADC16 */
adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;

/* Variables for PIT */
pit_config_t My_PIT;

volatile unsigned int sys_tick_counter = 0;

void InitAll(void);
uint32_t ReadLDR(void);
void PressDown(void);
void Release(void);
void DinoPlay(void);
/*
 * @brief   Application entry point.
 */

static uint8_t count = 0;
int main(void) {

    SysTick_Config(48000000U/1000U);

    gsc_sch_core_Init();

    gsc_sch_core_exec();

    return 0 ;

}

void DinoPlay(void)
{

	uint32_t ldr = ReadLDR();
	if(ldr >= 900)
	{
		Release();
		count=0;
	}
	else
	{
		count++;
		if(count >= 3 )
		{
    		PressDown();
    		uint8_t flag = 1;
    		PIT_StartTimer(PIT,kPIT_Chnl_0);
    		while(flag)
    		{
    			uint32_t time = 200U - COUNT_TO_MSEC(PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_0) , PIT_CLK_SRC_HZ_HP);
    			if(time >= 80U)
    			{
    				flag = 0;
    				PIT_StopTimer(PIT, kPIT_Chnl_0);
    				count = 0;
    			}
    		}
		}
	}
}

void PressDown(void)
{
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, 3U);
}

void Release(void)
{
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, 2U);
}

uint32_t ReadLDR(void)
{

	uint32_t adc = 0;

	ADC16_SetChannelConfig(ADC0, ADC_CHANNEL_GROUP, &adc16ChannelConfigStruct);

	while (0U == (kADC16_ChannelConversionDoneFlag &
				  ADC16_GetChannelStatusFlags(ADC0, ADC_CHANNEL_GROUP)))
	{
		//wait...
	}
	adc = ADC16_GetChannelConversionValue(ADC0, ADC_CHANNEL_GROUP);

	return adc;

}

void InitAll(void)
{

	InitBoard();
	InitADC(&adc16ConfigStruct, &adc16ChannelConfigStruct);
	InitTPM(&tpmInfo, &tpmParam);
	InitPIT(&My_PIT);

}

void SysTick_Handler(void)
{

 	sys_tick_counter++;
 	gsc_sch_core_tick_isr();

}

