/**
 * @file xmc_eth_phy_dp83848.c
 * @date 2015-12-15
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.18 - XMC Peripheral Driver Library
 *
 * Copyright (c) 2015-2018, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-06-20:
 *     - Initial <br>
 *
 * 2015-12-15:
 *     - Added Reset and exit power down
 *     - Reset function called in Init function
 *
 * @endcond
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
#if defined(XMC_ETH_PHY_DP83848C)
#include <xmc_eth_phy.h>

/*******************************************************************************
 * MACROS
 *******************************************************************************/

/* Basic Registers */
#define REG_BMCR                (0x00U)        /* Basic Mode Control Register       */
#define REG_BMSR                (0x01U)        /* Basic Mode Status Register        */
#define REG_PHYIDR1             (0x02U)        /* PHY Identifier 1                  */
#define REG_PHYIDR2             (0x03U)        /* PHY Identifier 2                  */
#define REG_ANAR                (0x04U)        /* Auto-Negotiation Advertisement    */
#define REG_ANLPAR              (0x05U)        /* Auto-Neg. Link Partner Abitily    */
#define REG_ANER                (0x06U)        /* Auto-Neg. Expansion Register      */
#define REG_ANNPTR              (0x07U)        /* Auto-Neg. Next Page TX            */
#define REG_RBR                 (0x17U)        /* RMII and Bypass Register          */

/* Extended Registers */
#define REG_PHYSTS              (0x10U)        /* Status Register                   */

/* Basic Mode Control Register */
#define BMCR_RESET              (0x8000U)      /* Software Reset                    */
#define BMCR_LOOPBACK           (0x4000U)      /* Loopback mode                     */
#define BMCR_SPEED_SEL          (0x2000U)      /* Speed Select (1=100Mb/s)          */
#define BMCR_ANEG_EN            (0x1000U)      /* Auto Negotiation Enable           */
#define BMCR_POWER_DOWN         (0x0800U)      /* Power Down                        */
#define BMCR_ISOLATE            (0x0400U)      /* Isolate Media interface           */
#define BMCR_REST_ANEG          (0x0200U)      /* Restart Auto Negotiation          */
#define BMCR_DUPLEX             (0x0100U)      /* Duplex Mode (1=Full duplex)       */
#define BMCR_COL_TEST           (0x0080U)      /* Collision Test                    */

/* Basic Mode Status Register */
#define BMSR_100B_T4            (0x8000U)      /* 100BASE-T4 Capable                */
#define BMSR_100B_TX_FD         (0x4000U)      /* 100BASE-TX Full Duplex Capable    */
#define BMSR_100B_TX_HD         (0x2000U)      /* 100BASE-TX Half Duplex Capable    */
#define BMSR_10B_T_FD           (0x1000U)      /* 10BASE-T Full Duplex Capable      */
#define BMSR_10B_T_HD           (0x0800U)      /* 10BASE-T Half Duplex Capable      */
#define BMSR_MF_PRE_SUP         (0x0040U)      /* Preamble suppression Capable      */
#define BMSR_ANEG_COMPL         (0x0020U)      /* Auto Negotiation Complete         */
#define BMSR_REM_FAULT          (0x0010U)      /* Remote Fault                      */
#define BMSR_ANEG_ABIL          (0x0008U)      /* Auto Negotiation Ability          */
#define BMSR_LINK_STAT          (0x0004U)      /* Link Status (1=established)       */
#define BMSR_JABBER_DET         (0x0002U)      /* Jaber Detect                      */
#define BMSR_EXT_CAPAB          (0x0001U)      /* Extended Capability               */

/* RMII and Bypass Register */
#define RBR_RMII_MODE         (0x0020U)      /* Reduced MII Mode                  */

/* PHY Identifier Registers */
#define PHY_ID1             0x2000      /* DP83848C Device Identifier MSB    */
#define PHY_ID2             0x5C90      /* DP83848C Device Identifier LSB    */

/* PHY Status Register */
#define PHYSTS_MDI_X        0x4000      /* MDI-X mode enabled by Auto-Negot. */
#define PHYSTS_REC_ERR      0x2000      /* Receive Error Latch               */
#define PHYSTS_POL_STAT     0x1000      /* Polarity Status                   */
#define PHYSTS_FC_SENSE     0x0800      /* False Carrier Sense Latch         */
#define PHYSTS_SIG_DET      0x0400      /* 100Base-TX Signal Detect          */
#define PHYSTS_DES_LOCK     0x0200      /* 100Base-TX Descrambler Lock       */
#define PHYSTS_PAGE_REC     0x0100      /* Link Code Word Page Received      */
#define PHYSTS_MII_INT      0x0080      /* MII Interrupt Pending             */
#define PHYSTS_REM_FAULT    0x0040      /* Remote Fault                      */
#define PHYSTS_JABBER_DET   0x0020      /* Jabber Detect                     */
#define PHYSTS_ANEG_COMPL   0x0010      /* Auto Negotiation Complete         */
#define PHYSTS_LOOPBACK     0x0008      /* Loopback Status                   */
#define PHYSTS_DUPLEX       0x0004      /* Duplex Status (1=Full duplex)     */
#define PHYSTS_SPEED        0x0002      /* Speed10 Status (1=10MBit/s)       */
#define PHYSTS_LINK_STAT    0x0001      /* Link Status (1=established)       */

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

 /* Check if the device identifier is valid */
static int32_t XMC_ETH_PHY_IsDeviceIdValid(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  uint16_t phy_id1;
  uint16_t phy_id2;
  XMC_ETH_PHY_STATUS_t status;

  /* Check Device Identification. */
  if ((XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_PHYIDR1, &phy_id1) == XMC_ETH_MAC_STATUS_OK) &&
      (XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_PHYIDR2, &phy_id2) == XMC_ETH_MAC_STATUS_OK))
  {
    if ((phy_id1 == PHY_ID1) && ((phy_id2 & (uint16_t)0xfff0) == PHY_ID2))
    {
      status = XMC_ETH_PHY_STATUS_OK;
    }
    else
    {
      status = XMC_ETH_PHY_STATUS_ERROR_DEVICE_ID;
    }
  }
  else
  {
    status = XMC_ETH_PHY_STATUS_ERROR_TIMEOUT;
  }

  return (int32_t)status;
}

/* PHY initialize */
int32_t XMC_ETH_PHY_Init(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr, const XMC_ETH_PHY_CONFIG_t *const config)
{
  int32_t status;
  uint16_t reg_val;

  status = XMC_ETH_PHY_IsDeviceIdValid(eth_mac, phy_addr);

  if (status == (int32_t)XMC_ETH_PHY_STATUS_OK)
  {
    status = XMC_ETH_PHY_Reset(eth_mac, phy_addr);
    
    if (status == (int32_t)XMC_ETH_PHY_STATUS_OK)
    {      
      reg_val = 0U;
      if (config->speed == XMC_ETH_LINK_SPEED_100M)
      {
        reg_val |= BMCR_SPEED_SEL;
      }

      if (config->duplex == XMC_ETH_LINK_DUPLEX_FULL)
      {
        reg_val |= BMCR_DUPLEX;
      }

      if (config->enable_auto_negotiate == true)
      {
        reg_val |= BMCR_ANEG_EN;
      }

      if (config->enable_loop_back == true)
      {
        reg_val |= BMCR_LOOPBACK;
      }

      status = (int32_t)XMC_ETH_MAC_WritePhy(eth_mac, phy_addr, REG_BMCR, reg_val);
      
      if (status == (int32_t)XMC_ETH_PHY_STATUS_OK)
      {
        /* Configure interface mode */  
        switch (config->interface) 
        {
          case XMC_ETH_LINK_INTERFACE_MII:
            reg_val = 0x0001;
            break;
          case XMC_ETH_LINK_INTERFACE_RMII:
            reg_val = RBR_RMII_MODE | 0x0001;
            break;
        }
        status = (int32_t)XMC_ETH_MAC_WritePhy(eth_mac, phy_addr, REG_RBR, reg_val);
      }      
    }
  }

  return status;
}

/* Reset */
int32_t XMC_ETH_PHY_Reset(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  int32_t status;
  uint16_t reg_bmcr;

  /* Reset PHY*/
  status = (int32_t)XMC_ETH_MAC_WritePhy(eth_mac, phy_addr, REG_BMCR, BMCR_RESET);
  if (status == (int32_t)XMC_ETH_PHY_STATUS_OK)
  {      
    /* Wait for the reset to complete */
    do 
    {
      status = XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_BMCR, &reg_bmcr);
    } while ((reg_bmcr & BMCR_RESET) != 0);
  }
    
  return status;
}

/* Initiate power down */
int32_t XMC_ETH_PHY_PowerDown(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  int32_t status;
  uint16_t reg_bmcr;

  status = XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_BMCR, &reg_bmcr);
  if (status == (int32_t)XMC_ETH_PHY_STATUS_OK)
  {      
    reg_bmcr |= BMCR_POWER_DOWN;
    status = (int32_t)XMC_ETH_MAC_WritePhy(eth_mac, phy_addr, REG_BMCR, reg_bmcr);
  }
  return status;
}

/* Exit power down */
int32_t XMC_ETH_PHY_ExitPowerDown(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  int32_t status;
  uint16_t reg_bmcr;

  status = XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_BMCR, &reg_bmcr);  
  if (status == (int32_t)XMC_ETH_PHY_STATUS_OK)
  {      
    reg_bmcr &= ~BMCR_POWER_DOWN;
    status = (int32_t)XMC_ETH_MAC_WritePhy(eth_mac, phy_addr, REG_BMCR, reg_bmcr);
  }
  return status;
}

/* Get link status */
XMC_ETH_LINK_STATUS_t XMC_ETH_PHY_GetLinkStatus(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  uint16_t val;

  XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_BMSR, &val);

  return (XMC_ETH_LINK_STATUS_t)((val & BMSR_LINK_STAT) ? XMC_ETH_LINK_STATUS_UP : XMC_ETH_LINK_STATUS_DOWN);
}

/* Get link speed */
XMC_ETH_LINK_SPEED_t XMC_ETH_PHY_GetLinkSpeed(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  uint16_t val;

  XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_PHYSTS, &val);

  return (XMC_ETH_LINK_SPEED_t)((val & PHYSTS_SPEED) ? XMC_ETH_LINK_SPEED_10M   : XMC_ETH_LINK_SPEED_100M);
}

/* Get link duplex settings */
XMC_ETH_LINK_DUPLEX_t XMC_ETH_PHY_GetLinkDuplex(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  uint16_t val;

  XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_PHYSTS, &val);

  return (XMC_ETH_LINK_DUPLEX_t)((val & PHYSTS_DUPLEX) ? XMC_ETH_LINK_DUPLEX_FULL : XMC_ETH_LINK_DUPLEX_HALF);
}

bool XMC_ETH_PHY_IsAutonegotiationCompleted(XMC_ETH_MAC_t *const eth_mac, uint8_t phy_addr)
{
  uint16_t val;

  XMC_ETH_MAC_ReadPhy(eth_mac, phy_addr, REG_BMSR, &val);
  return ((val & BMSR_ANEG_COMPL) == BMSR_ANEG_COMPL);
}

#endif // defined(XMC_ETH_PHY_DP83848C)
