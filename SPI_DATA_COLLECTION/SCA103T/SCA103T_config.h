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

#define NROF_SCA103T 	( 	SCA103T_0_ENA + \
							SCA103T_1_ENA + \
							SCA103T_2_ENA +	\
							SCA103T_3_ENA +	\
							SCA103T_4_ENA +	\
							SCA103T_5_ENA +	\
							SCA103T_6_ENA + \
							SCA103T_7_ENA )

// SCA103T ADV_MEASUREMENT_CHANNEL
#if SCA103T_0_ENA

#endif

#if SCA103T_1_ENA

#endif

#if SCA103T_2_ENA

#endif

#if SCA103T_3_ENA

#endif

#if SCA103T_4_ENA

#endif

#if SCA103T_5_ENA

#endif

#if SCA103T_6_ENA

#endif

#if SCA103T_7_ENA

#endif

#endif /* SCA103T_CONFIG_H_ */
