/*
 * inclino_sens_board_config.h
 *
 *  Created on: Feb 17, 2020
 *      Author: erwin
 */

#ifndef INCLINO_SENS_BOARD_CONFIG_H_
#define INCLINO_SENS_BOARD_CONFIG_H_

/***********************************************************
 * ISB selection configuration
 ***********************************************************/
#define ISB_0_ENABLED 1
#define	ISB_1_ENABLED 0
#define	ISB_2_ENABLED 0
#define ISB_3_ENABLED 0
#define ISB_4_ENABLED 0
#define	ISB_5_ENABLED 0
#define	ISB_6_ENABLED 0
#define	ISB_7_ENABLED 0

/***********************************************************
 * Number of ISBs and IC configuration
 ***********************************************************/
#define NROF_ISB 		( 	ISB_0_ENABLED + \
							ISB_1_ENABLED + \
							ISB_2_ENABLED +	\
							ISB_3_ENABLED +	\
							ISB_4_ENABLED +	\
							ISB_5_ENABLED +	\
							ISB_6_ENABLED + \
							ISB_7_ENABLED )

#endif /* INCLINO_SENS_BOARD_CONFIG_H_ */
