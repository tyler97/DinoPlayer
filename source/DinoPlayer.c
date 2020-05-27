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
/* TODO: insert other definitions and declarations here. */
static int LDR_PORT = 0;
static int SERVO_PORT = 0;

/* Variables for TPM */
tpm_config_t tpmInfo;
tpm_chnl_pwm_signal_param_t tpmParam;

/* Variables for ADC16 */
adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;

void InitAll(void);
void ReadLDR(void);
/*
 * @brief   Application entry point.
 */
int main(void) {

	InitAll();

    while(1) {

    	Release();

    	ReadLDR();
    }
    return 0 ;
}

void PressDown(void)
{
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, 3U);
}

void Release(void)
{
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, 2U);
}

void ReadLDR(void)
{

	uint32_t adc = 0;

	ADC16_SetChannelConfig(ADC0, ADC_CHANNEL_GROUP, &adc16ChannelConfigStruct);

	while (0U == (kADC16_ChannelConversionDoneFlag &
				  ADC16_GetChannelStatusFlags(ADC0, ADC_CHANNEL_GROUP)))
	{
		//wait...
	}
	adc = ADC16_GetChannelConversionValue(ADC0, ADC_CHANNEL_GROUP);
	//printf("%d\n", adc);

	//TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1, kTPM_CenterAlignedPwm, adc * 10);

}
void InitAll(void)
{

	InitBoard();
	InitADC(&adc16ConfigStruct, &adc16ChannelConfigStruct);
	InitTPM(&tpmInfo, &tpmParam);

}
