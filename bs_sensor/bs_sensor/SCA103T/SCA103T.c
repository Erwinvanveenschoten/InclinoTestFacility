/*
 * SCA103T.c
 *
 *  Created on: Feb 27, 2020
 *      Author: erwin
 */

#include "Dave.h"
#include "SCA103T.h"
#include "data_config.h"
#include "stdio.h"
#include "UDP.h"

static MESSAGE_t SCA103T_buffer[SCA103_BUFFERSIZE];
static uint8_t ADC_count = 0;

static void print_buffer( void );
static void send_buffer( void );

void SCA103T_ADC0_ISR(void)
{
	// read the results of the conversion
#if SCA103T_6_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_6_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_A);

	SCA103T_buffer[SCA103T_6_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_6_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_6_ID;
#endif
#if SCA103T_6_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_6_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_B);
#endif
#if SCA103T_7_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_7_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_C);

	SCA103T_buffer[SCA103T_7_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_7_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_7_ID;
#endif
#if SCA103T_5_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_5_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_D);

	SCA103T_buffer[SCA103T_5_INCLA_BUFFER_INDEX].data_id = INCL_A;
	SCA103T_buffer[SCA103T_5_INCLA_BUFFER_INDEX].ic_id	 = SCA103T_5_ID;
#endif
#if SCA103T_5_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_5_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_E);

	SCA103T_buffer[SCA103T_5_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_5_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_5_ID;
#endif
#if SCA103T_4_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_4_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_F);

	SCA103T_buffer[SCA103T_4_INCLA_BUFFER_INDEX].data_id = INCL_A;
	SCA103T_buffer[SCA103T_4_INCLA_BUFFER_INDEX].ic_id	 = SCA103T_4_ID;
#endif
#if SCA103T_7_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_7_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_G);

	SCA103T_buffer[SCA103T_7_INCLA_BUFFER_INDEX].data_id = INCL_A;
	SCA103T_buffer[SCA103T_7_INCLA_BUFFER_INDEX].ic_id	 = SCA103T_7_ID;
#endif
#if SCA103T_7_ENA && VREF2_ENA
	SCA103T_buffer[SCA103T_VREF_1_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_H);

	SCA103T_buffer[SCA103T_VREF_1_BUFFER_INDEX].data_id = VREF;
	SCA103T_buffer[SCA103T_VREF_1_BUFFER_INDEX].ic_id	 = SCA103T_7_ID;
#endif
	ADC_count++;
	if ( ADC_count == ( NROF_ADC_APPS - 1 ) )
	{
		ADC_count = 0;
		// Send buffer
		print_buffer();
		send_buffer();
	}
}

void SCA103T_ADC1_ISR(void)
{
	// read the results of the conversion
#if SEVEN_V_MES
	//SCA103T_buffer[].data = ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_A);
#endif
#if SCA103T_3_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_3_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_B);

	SCA103T_buffer[SCA103T_3_INCLA_BUFFER_INDEX].data_id = INCL_A;
	SCA103T_buffer[SCA103T_3_INCLA_BUFFER_INDEX].ic_id	 = SCA103T_3_ID;

#endif
#if SCA103T_4_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_4_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_C);

	SCA103T_buffer[SCA103T_4_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_4_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_4_ID;
#endif
#if SCA103T_0_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_0_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_D);

	SCA103T_buffer[SCA103T_0_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_0_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_0_ID;
#endif
#if SCA103T_0_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_0_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_E);

	SCA103T_buffer[SCA103T_0_INCLA_BUFFER_INDEX].data_id = INCL_A;
	SCA103T_buffer[SCA103T_0_INCLA_BUFFER_INDEX].ic_id	 = SCA103T_0_ID;
#endif
#if SCA103T_0_ENA && VREF1_ENA
	SCA103T_buffer[SCA103T_VREF_0_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_F);

	SCA103T_buffer[SCA103T_VREF_0_BUFFER_INDEX].data_id 	= VREF;
		SCA103T_buffer[SCA103T_VREF_0_BUFFER_INDEX].ic_id	= SCA103T_0_ID;
#endif
#if SCA103T_1_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_1_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_G);

	SCA103T_buffer[SCA103T_1_INCLB_BUFFER_INDEX].data_id 	= INCL_B;
	SCA103T_buffer[SCA103T_1_INCLB_BUFFER_INDEX].ic_id		= SCA103T_1_ID;
#endif
#if SCA103T_1_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_1_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_1_Channel_H);

	SCA103T_buffer[SCA103T_1_INCLA_BUFFER_INDEX].data_id 	= INCL_A;
	SCA103T_buffer[SCA103T_1_INCLA_BUFFER_INDEX].ic_id		= SCA103T_1_ID;
#endif
	ADC_count++;
	if ( ADC_count == ( NROF_ADC_APPS - 1 ) )
	{
		ADC_count = 0;
		// Send buffer
		send_buffer();

		print_buffer();
	}
}

void SCA103T_ADC2_ISR(void)
{
	// read the results of the conversion
#if SCA103T_3_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_3_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_2_Channel_A);

	SCA103T_buffer[SCA103T_3_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_3_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_3_ID;
#endif
#if SCA103T_2_ENA && INCL_A_ENA
	SCA103T_buffer[SCA103T_2_INCLA_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_2_Channel_B);

	SCA103T_buffer[SCA103T_2_INCLA_BUFFER_INDEX].data_id = INCL_A;
	SCA103T_buffer[SCA103T_2_INCLA_BUFFER_INDEX].ic_id	 = SCA103T_2_ID;
#endif
#if SCA103T_2_ENA && INCL_B_ENA
	SCA103T_buffer[SCA103T_2_INCLB_BUFFER_INDEX].data =
			ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_2_Channel_C);

	SCA103T_buffer[SCA103T_2_INCLB_BUFFER_INDEX].data_id = INCL_B;
	SCA103T_buffer[SCA103T_2_INCLB_BUFFER_INDEX].ic_id	 = SCA103T_2_ID;
#endif
	ADC_count++;
	if ( ADC_count == ( NROF_ADC_APPS - 1 ) )
	{
		ADC_count = 0;
		// Send buffer
		send_buffer();
		print_buffer();
	}
}

static void print_buffer( void )
{
	for ( int i = 0; i < SCA103_BUFFERSIZE; i++ )
	{
		printf("\n\n********************\n\r");
		printf("Data id: %d\n\r", SCA103T_buffer[i].data_id);
		printf("IC   id: %d\n\r", SCA103T_buffer[i].ic_id);
		printf("Data   : %04X\n\r", (uint)SCA103T_buffer[i].data);
	}
}

static void send_buffer( void )
{
	for ( int i = 0; i < SCA103_BUFFERSIZE; i++ )
	{
		udp_printStruct((void *)&SCA103T_buffer[i], sizeof(SCA103T_buffer[i]));
	}
}
