
/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2014-06-16:
 *     - Initial version<br>
 * 2015-08-28:
 *     - Added CLOCK_XMC1_Init conditionally
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "DAVE.h"

/***********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/*******************************************************************************
 * @brief This function initializes the APPs Init Functions.
 *
 * @param[in]  None
 *
 * @return  DAVE_STATUS_t <BR>
 ******************************************************************************/
DAVE_STATUS_t DAVE_Init(void)
{
  DAVE_STATUS_t init_status;
  
  init_status = DAVE_STATUS_SUCCESS;
     /** @Initialization of APPs Init Functions */
     init_status = (DAVE_STATUS_t)CLOCK_XMC4_Init(&CLOCK_XMC4_0);

  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of ETH_LWIP APP instance ETH_LWIP_0 */
	 init_status = (DAVE_STATUS_t)ETH_LWIP_Init(&ETH_LWIP_0); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of SPI_MASTER APP instance SPI_MASTER_0 */
	 init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_MASTER_0); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of ADC_MEASUREMENT_ADV APP instance ADC_MEASUREMENT_ADV_0 */
	 init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_ADV_Init(&ADC_MEASUREMENT_ADV_0); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of ADC_MEASUREMENT_ADV APP instance ADC_MEASUREMENT_ADV_1 */
	 init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_ADV_Init(&ADC_MEASUREMENT_ADV_1); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of ADC_MEASUREMENT_ADV APP instance ADC_MEASUREMENT_ADV_2 */
	 init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_ADV_Init(&ADC_MEASUREMENT_ADV_2); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of SPI_MASTER APP instance SPI_MASTER_1 */
	 init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_MASTER_1); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of SPI_MASTER APP instance SPI_MASTER_2 */
	 init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_MASTER_2); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of UART APP instance UART_0 */
	 init_status = (DAVE_STATUS_t)UART_Init(&UART_0); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of BUS_IO APP instance IO_GA_8 */
	 init_status = (DAVE_STATUS_t)BUS_IO_Init(&IO_GA_8); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of BUS_IO APP instance IO_GP */
	 init_status = (DAVE_STATUS_t)BUS_IO_Init(&IO_GP); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance TICK_TIMER */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&TICK_TIMER); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance TICK_TIMER_INTERRUPT */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&TICK_TIMER_INTERRUPT); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance ADC_MEASUREMENT_ADV_0_RESULT */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ADC_MEASUREMENT_ADV_0_RESULT); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance ADC_MEASUREMENT_ADV_1_RESULT */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ADC_MEASUREMENT_ADV_1_RESULT); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance ADC_MEASUREMENT_ADV_2_RESULT */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ADC_MEASUREMENT_ADV_2_RESULT); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance ADC_TIMER */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&ADC_TIMER); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance TIME_MEASUREMENT */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&TIME_MEASUREMENT); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance DELAY_TIMER */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&DELAY_TIMER); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of BUS_IO APP instance GP_LED_BUS */
	 init_status = (DAVE_STATUS_t)BUS_IO_Init(&GP_LED_BUS); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance DELAY_TIMER_INTERRUPT */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&DELAY_TIMER_INTERRUPT); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance MS5611_TIMER */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&MS5611_TIMER); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance MS5611_TIMER_ISR */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&MS5611_TIMER_ISR); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of TIMER APP instance TEMP_UPDATE_TIMER */
	 init_status = (DAVE_STATUS_t)TIMER_Init(&TEMP_UPDATE_TIMER); 
   } 
  if (init_status == DAVE_STATUS_SUCCESS)
  {
	 /**  Initialization of INTERRUPT APP instance TEMP_UPDATE_INTERRUPT */
	 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&TEMP_UPDATE_INTERRUPT); 
   }  
  return init_status;
} /**  End of function DAVE_Init */

