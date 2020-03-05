/*********************************************************************************************************************
* DAVE APP Name : UART       APP Version: 4.1.10
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015-2016, Infineon Technologies AG
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
 * 2015-02-16:
 *     - Initial version for DAVEv4.
 *
 * 2015-06-20:
 *     - Service request value moved from runtime structure to config structure.
 *
 * 2015-07-06:
 *     - Structure name changed from UART_DYNAMIC_t to UART_RUNTIME_t
 *
 * 2015-07-30:
 *     - Added DMA and Direct mode configuration
 * @endcond
 *
 */
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "uart.h"

/***********************************************************************************************************************
 * EXTERN DECLARATIONS
 ***********************************************************************************************************************/

/*
 * Function implements the data transmission. It is called from the transmit interrupt service handler.
 * Function pushes data to the output block and releases control. It is called again when the previous data is
 * transmitted. When transmit FIFO is used, the function sets the trigger limit based on the size of data to be
 * transmitted.
 */
extern void UART_lTransmitHandler(const UART_t * const handle);
/*
 * Function implements the data reception. It is called from the receive interrupt service handler.
 * Function reads data from the receive block and updates the user's buffer. It is called again when the data is
 * received again. When receive FIFO is used, the function sets the trigger limit based on the size of data to be
 * received.
 */
extern void UART_lReceiveHandler(const UART_t * const handle);
/*
 * Function monitors the configured protocol interrupt flags. It is called from the protocol interrupt
 * service handler.
 * Function reads the status of the USIC channel and checks for configured flags in the APP UI.
 * If any callback function is provided in the APP UI, it will be called when the selected flag is set.
 */
extern void UART_lProtocolHandler(const UART_t * const handle);


/**********************************************************************************************************************
 * DATA STRUCTURES
 **********************************************************************************************************************/
UART_STATUS_t UART_0_init(void);

/*USIC channel configuration*/
const XMC_UART_CH_CONFIG_t UART_0_channel_config =
{
  .baudrate      = 19200U,
  .data_bits     = 8U,
  .frame_length  = 8U,
  .stop_bits     = 1U,
  .oversampling  = 16U,
  .parity_mode   = XMC_USIC_CH_PARITY_MODE_NONE
};
/*Transmit pin configuration*/
const XMC_GPIO_CONFIG_t UART_0_tx_pin_config   = 
{ 
  .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2, 
  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
};

/*Transmit pin configuration used for initializing*/
const UART_TX_CONFIG_t UART_0_tx_pin = 
{
  .port = (XMC_GPIO_PORT_t *)PORT3_BASE,
  .config = &UART_0_tx_pin_config,
  .pin = 13U
};

/*UART APP configuration structure*/
const UART_CONFIG_t UART_0_config = 
{
  .channel_config   = &UART_0_channel_config,


  .fptr_uart_config = UART_0_init,
  .tx_cbhandler = NULL,
  .rx_cbhandler = NULL,  
  .sync_error_cbhandler = NULL,  
  .rx_noise_error_cbhandler = NULL,  
  .format_error_bit0_cbhandler = NULL,  
  .format_error_bit1_cbhandler = NULL,  
  .collision_error_cbhandler = NULL,
  .tx_pin_config    = &UART_0_tx_pin,
  .mode             = UART_MODE_FULLDUPLEX,
  .transmit_mode = UART_TRANSFER_MODE_INTERRUPT,
  .receive_mode = UART_TRANSFER_MODE_INTERRUPT,
  .tx_fifo_size     = XMC_USIC_CH_FIFO_SIZE_16WORDS,
  .rx_fifo_size     = XMC_USIC_CH_FIFO_SIZE_16WORDS,
  .tx_sr   = 0x4U,
};

/*Runtime handler*/
UART_RUNTIME_t UART_0_runtime = 
{
  .tx_busy = false,  
  .rx_busy = false,
};

/*APP handle structure*/
UART_t UART_0 = 
{
  .channel = XMC_UART0_CH1,
  .config  = &UART_0_config,
  .runtime = &UART_0_runtime
};

/*Receive pin configuration*/
const XMC_GPIO_CONFIG_t UART_0_rx_pin_config   = {
  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
};
/**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/
/*Channel initialization function*/
UART_STATUS_t UART_0_init()
{
  UART_STATUS_t status = UART_STATUS_SUCCESS;
  /*Configure Receive pin*/
  XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT6_BASE, 3U, &UART_0_rx_pin_config);
  /* Initialize USIC channel in UART mode*/
  XMC_UART_CH_Init(XMC_UART0_CH1, &UART_0_channel_config);
  /*Set input source path*/
  XMC_USIC_CH_SetInputSource(XMC_UART0_CH1, XMC_USIC_CH_INPUT_DX0, 2U);
  /*Configure transmit FIFO*/
  XMC_USIC_CH_TXFIFO_Configure(XMC_UART0_CH1,
        48U,
        XMC_USIC_CH_FIFO_SIZE_16WORDS,
        1U);
  /*Configure receive FIFO*/
  XMC_USIC_CH_RXFIFO_Configure(XMC_UART0_CH1,
        32U,
        XMC_USIC_CH_FIFO_SIZE_16WORDS,
        0U);
  /* Start UART */
  XMC_UART_CH_Start(XMC_UART0_CH1);

  /* Initialize UART TX pin */
  XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT3_BASE, 13U, &UART_0_tx_pin_config);

  /*Set service request for UART protocol events*/
  XMC_USIC_CH_SetInterruptNodePointer(XMC_UART0_CH1, XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL,
     0U);
  /*Set service request for tx FIFO transmit interrupt*/
  XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(XMC_UART0_CH1, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
      4U);
  /*Set service request for rx FIFO receive interrupt*/
  XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_UART0_CH1, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
       0x5U);
  XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_UART0_CH1, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE,
       0x5U);
  /*Set priority and enable NVIC node for transmit interrupt*/
  NVIC_SetPriority((IRQn_Type)88, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                        63U, 0U));
  NVIC_EnableIRQ((IRQn_Type)88);
  /*Set priority and enable NVIC node for receive interrupt*/
  NVIC_SetPriority((IRQn_Type)89, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                      63U, 0U));
  NVIC_EnableIRQ((IRQn_Type)89);
  return status;
}
/*Interrupt handlers*/
/*Transmit ISR*/
void UART_0_TX_HANDLER()
{
  UART_lTransmitHandler(&UART_0);
}

/*Receive ISR*/
void UART_0_RX_HANDLER()
{
  UART_lReceiveHandler(&UART_0);
}

/*CODE_BLOCK_END*/

