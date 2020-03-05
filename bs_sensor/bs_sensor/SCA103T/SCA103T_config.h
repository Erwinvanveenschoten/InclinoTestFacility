/*
 * SCA103T_config.h
 *
 *  Created on: Feb 17, 2020
 *      Author: erwin
 */

#ifndef SCA103T_CONFIG_H_
#define SCA103T_CONFIG_H_

#include "inclino_sens_board_config.h"

/***********************************************************
 * SCA103T configuration
 ***********************************************************/


// Configure which SCA103T is enabled for testing purposes
#define SCA103T_0_ENA ISB_0_ENABLED
#define SCA103T_1_ENA ISB_1_ENABLED
#define SCA103T_2_ENA ISB_2_ENABLED
#define SCA103T_3_ENA ISB_3_ENABLED
#define SCA103T_4_ENA ISB_4_ENABLED
#define SCA103T_5_ENA ISB_5_ENABLED
#define SCA103T_6_ENA ISB_6_ENABLED
#define SCA103T_7_ENA ISB_7_ENABLED

#define INCL_B_ENA  1
#define INCL_A_ENA 	1
#define VREF1_ENA 	0
#define VREF2_ENA 	0

#define NROF_SCA103T 	( 	SCA103T_0_ENA + \
							SCA103T_1_ENA + \
							SCA103T_2_ENA +	\
							SCA103T_3_ENA +	\
							SCA103T_4_ENA +	\
							SCA103T_5_ENA +	\
							SCA103T_6_ENA + \
							SCA103T_7_ENA )

#define SCA103_BUFFERSIZE 	NROF_SCA103T 					* \
							(INCL_B_ENA+INCL_A_ENA) 		+ \
							(VREF1_ENA && SCA103T_0_ENA)	+ \
							(VREF2_ENA && SCA103T_7_ENA)

#define NROF_ADC_APPS 3

#if SCA103T_0_ENA && INCL_A_ENA
#define SCA103T_0_INCLA_BUFFER_INDEX -1 + SCA103T_0_ENA
#endif
#if SCA103T_0_ENA && INCL_B_ENA
#define SCA103T_0_INCLB_BUFFER_INDEX -1 + SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_1_ENA && INCL_A_ENA
#define SCA103T_1_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA
#endif
#if SCA103T_1_ENA && INCL_B_ENA
#define SCA103T_1_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_2_ENA && INCL_A_ENA
#define SCA103T_2_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA
#endif
#if SCA103T_2_ENA && INCL_B_ENA
#define SCA103T_2_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_3_ENA && INCL_A_ENA
#define SCA103T_3_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA
#endif
#if SCA103T_3_ENA && INCL_B_ENA
#define SCA103T_3_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_4_ENA && INCL_A_ENA
#define SCA103T_4_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA
#endif
#if SCA103T_4_ENA && INCL_B_ENA
#define SCA103T_4_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_5_ENA && INCL_A_ENA
#define SCA103T_5_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA
#endif
#if SCA103T_5_ENA && INCL_B_ENA
#define SCA103T_5_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_6_ENA && INCL_A_ENA
#define SCA103T_6_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_6_ENA
#endif
#if SCA103T_6_ENA && INCL_B_ENA
#define SCA103T_6_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_6_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_7_ENA && INCL_A_ENA
#define SCA103T_7_INCLA_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_6_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_7_ENA
#endif
#if SCA103T_7_ENA && INCL_B_ENA
#define SCA103T_7_INCLB_BUFFER_INDEX -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_6_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_7_ENA * (INCL_B_ENA + INCL_A_ENA)
#endif
#if SCA103T_0_ENA && VREF1_ENA
#define SCA103T_VREF_0_BUFFER_INDEX  -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_6_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_7_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											VREF1_ENA
#endif
#if SCA103T_7_ENA && VREF2_ENA
#define SCA103T_VREF_1_BUFFER_INDEX  -1 + 	SCA103T_0_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_1_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_2_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_3_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_4_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_5_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_6_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											SCA103T_7_ENA * (INCL_B_ENA + INCL_A_ENA) + \
											VREF1_ENA + \
											VREF2_ENA
#endif
#endif /* SCA103T_CONFIG_H_ */
