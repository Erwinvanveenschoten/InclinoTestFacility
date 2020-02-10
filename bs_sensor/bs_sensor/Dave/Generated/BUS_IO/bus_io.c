/**
 * @file BUS_io.c
 * @date 2016-04-06
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * BUS_IO v4.0.2 - BUS_IO APP is used to configure/control several GPIO pins as one entity.
 *
 * Copyright (c) 2016, Infineon Technologies AG
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
 * 2016-04-01
 *     - Initial version.<br>
 * 2016-04-06
 *     - BUS_IO_Write() API performance improved.<br>
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "bus_io.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/

 /**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/**
* @brief Get BUS_IO APP version
* @return DAVE_APP_VERSION_t APP version information (major, minor and patch number)
*/

DAVE_APP_VERSION_t BUS_IO_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t)BUS_IO_MAJOR_VERSION;
  version.minor = (uint8_t)BUS_IO_MINOR_VERSION;
  version.patch = (uint8_t)BUS_IO_PATCH_VERSION;

  return (version);
}

/**
* @brief Function to initialize the port pin as per UI settings.
* @param handle_ptr Pointer pointing to APP data structure.
* @return BUS_IO_STATUS_t BUS_IO APP status.
*/

BUS_IO_STATUS_t BUS_IO_Init(BUS_IO_t *const handle_ptr)
{
  uint8_t i;

  XMC_ASSERT("BUS_IO_Init: handle_ptr null pointer", handle_ptr != NULL);

  if ((bool)false == handle_ptr->initialized)
  {
    /* Iterate the list of pins initializing the pin according to the gpio_config */
    for (i = 0U; i < handle_ptr->number_of_pins; i++)
    {
      /* Initializes input / output characteristics */
      XMC_GPIO_Init(handle_ptr->pin_array[i].gpio_port, handle_ptr->pin_array[i].gpio_pin, &handle_ptr->gpio_config);
    }
    handle_ptr->initialized = (bool)true;
  }
  return (BUS_IO_STATUS_OK);
}

/* Function to set the mode of the bus */
void BUS_IO_SetMode(BUS_IO_t *const handle_ptr, const BUS_IO_MODE_t mode)
{
  uint8_t i;

  XMC_ASSERT("BUS_IO_SetMode: handle_ptr null pointer", handle_ptr != NULL);

  /* Iterate the list of pins setting the pin according to the selected mode */
  for (i = 0U; i < handle_ptr->number_of_pins; i++)
  {
    /* Setting of a mode of a bus */
    XMC_GPIO_SetMode(handle_ptr->pin_array[i].gpio_port, handle_ptr->pin_array[i].gpio_pin, mode);
  }
}

/* Function to read the bus pins state */
uint16_t BUS_IO_Read(BUS_IO_t *const handle_ptr)
{
  uint8_t i;
  uint16_t count = 0U;

  XMC_ASSERT("BUS_IO_Read: handle_ptr null pointer", handle_ptr != NULL);

  /* Iterate the list of pins reading the pin state and packing the result according the pin position in the list, from LSB to MSB */
  for (i = 0U; i < handle_ptr->number_of_pins; i++)
  {
    /* Reading a bus pin's state */
    count |= (uint16_t)(XMC_GPIO_GetInput(handle_ptr->pin_array[i].gpio_port, handle_ptr->pin_array[i].gpio_pin) << i);
  }
  return(count);
}

/* Function to set the pins bus state */
void BUS_IO_Write(BUS_IO_t *const handle_ptr, const uint16_t data)
{
  uint8_t i;
  const BUS_IO_PORT_PIN_t *bus_io_port_pin;

  XMC_ASSERT("BUS_IO_Write: handle_ptr null pointer", handle_ptr != NULL);

  bus_io_port_pin = handle_ptr->pin_array;
  /* Iterate the list of pins setting the pin state according the pin position in the list, , from LSB to MSB */
  for (i = 0U; i < handle_ptr->number_of_pins; i++)
  {
    if (data & (uint16_t)((uint16_t)1 << i))
    {
      XMC_GPIO_SetOutputLevel(bus_io_port_pin->gpio_port, bus_io_port_pin->gpio_pin, XMC_GPIO_OUTPUT_LEVEL_HIGH);
    }
    else
    {
      XMC_GPIO_SetOutputLevel(bus_io_port_pin->gpio_port, bus_io_port_pin->gpio_pin, XMC_GPIO_OUTPUT_LEVEL_LOW);
    }
   bus_io_port_pin++;
  }
}

/* Function to Toggles the pins bus state */
void BUS_IO_Toggle(BUS_IO_t *const handle_ptr)
{
  uint8_t i;

  XMC_ASSERT("BUS_IO_Toggle: handle_ptr null pointer", handle_ptr != NULL);

  /* Iterate the list of pins toggling the pin state */
  for (i = 0U; i < handle_ptr->number_of_pins; i++)
  {
    /* Toggles the pins bus state */
    XMC_GPIO_ToggleOutput(handle_ptr->pin_array[i].gpio_port, handle_ptr->pin_array[i].gpio_pin);
  }
}
