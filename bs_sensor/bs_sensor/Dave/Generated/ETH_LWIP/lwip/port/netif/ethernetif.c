/*********************************************************************************************************************
* DAVE APP Name : ETH_LWIP       APP Version: 4.1.6
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/**
 * @file ethernetif.c
 * @date 2017-09-27
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * ETH_LWIP v4.1.6 - Initializes LWIP stack.
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
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
 * 2017-09-27:
 *     - Initialize PHY brefore reseting MAC
 *
 * @endcond
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "../../include/lwip/opt.h"

#include "../../include/lwip/def.h"
#include "../../include/lwip/sys.h"
#include "../../include/lwip/mem.h"
#include "../../include/lwip/pbuf.h"
#include "../../include/lwip/stats.h"
#include "../../include/lwip/snmp.h"
#include "../../include/netif/etharp.h"
#include "../../../eth_lwip.h"

#include "xmc_gpio.h"
#include "xmc_eth_mac.h"
#include "xmc_eth_phy.h"
#include <string.h>
#define ETH_LWIP_0_CRS  XMC_GPIO_PORT5, 4U
#define ETH_LWIP_0_CD  XMC_GPIO_PORT5, 5U
#define ETH_LWIP_0_RDV  XMC_GPIO_PORT5, 2U
#define ETH_LWIP_0_RXER  XMC_GPIO_PORT5, 3U
#define ETH_LWIP_0_RXD0  XMC_GPIO_PORT5, 0U
#define ETH_LWIP_0_RXD1  XMC_GPIO_PORT2, 3U
#define ETH_LWIP_0_RXD2  XMC_GPIO_PORT5, 8U
#define ETH_LWIP_0_RXD3  XMC_GPIO_PORT5, 9U
#define ETH_LWIP_0_TX_CLK  XMC_GPIO_PORT5, 10U
#define ETH_LWIP_0_RX_CLK  XMC_GPIO_PORT6, 5U
#define ETH_LWIP_0_MDIO  XMC_GPIO_PORT1, 11U
#define ETH_LWIP_0_TXEN  XMC_GPIO_PORT1, 12U
#define ETH_LWIP_0_TXER  XMC_GPIO_PORT6, 2U
#define ETH_LWIP_0_TXD0  XMC_GPIO_PORT1, 13U
#define ETH_LWIP_0_TXD1  XMC_GPIO_PORT1, 14U
#define ETH_LWIP_0_TXD2  XMC_GPIO_PORT6, 0U
#define ETH_LWIP_0_TXD3  XMC_GPIO_PORT6, 1U
#define ETH_LWIP_0_MDC  XMC_GPIO_PORT1, 10U
#define ETH_LWIP_0_PIN_LIST_SIZE 18U




#define ETH_LWIP_0_PHY_ADDR   (0)

#define ETH_LWIP_0_NUM_RX_BUF (4U)
#define ETH_LWIP_0_NUM_TX_BUF (4U)

/* MAC ADDRESS*/

/* MAC address*/
#define MAC_ADDR0 (0x00U)
#define MAC_ADDR1 (0x03U)
#define MAC_ADDR2 (0x19U)
#define MAC_ADDR3 (0x45U)
#define MAC_ADDR4 (0x00U)
#define MAC_ADDR5 (0x00U)

#define MAC_ADDR    ((uint64_t)MAC_ADDR0 | \
                     ((uint64_t)MAC_ADDR1 << 8) | \
                     ((uint64_t)MAC_ADDR2 << 16) | \
                     ((uint64_t)MAC_ADDR3 << 24) | \
                     ((uint64_t)MAC_ADDR4 << 32) | \
                     ((uint64_t)MAC_ADDR5 << 40))

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/*Maximum retry iterations for phy auto-negotiation*/
#define ETH_LWIP_PHY_MAX_RETRIES  0xfffffU


#if defined(__ICCARM__)
#pragma data_alignment=4
static XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_rx_desc[ETH_LWIP_0_NUM_RX_BUF];
#pragma data_alignment=4
static XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_tx_desc[ETH_LWIP_0_NUM_TX_BUF];
#pragma data_alignment=4
static uint8_t ETH_LWIP_0_rx_buf[ETH_LWIP_0_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE];
#pragma data_alignment=4
static uint8_t ETH_LWIP_0_tx_buf[ETH_LWIP_0_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE];
#elif defined(__CC_ARM)
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_rx_desc[ETH_LWIP_0_NUM_RX_BUF] __attribute__((section ("RW_IRAM1")));
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_tx_desc[ETH_LWIP_0_NUM_TX_BUF] __attribute__((section ("RW_IRAM1")));
static __attribute__((aligned(4))) uint8_t ETH_LWIP_0_rx_buf[ETH_LWIP_0_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("RW_IRAM1")));
static __attribute__((aligned(4))) uint8_t ETH_LWIP_0_tx_buf[ETH_LWIP_0_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("RW_IRAM1")));
#elif defined(__GNUC__)
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_rx_desc[ETH_LWIP_0_NUM_RX_BUF] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_tx_desc[ETH_LWIP_0_NUM_TX_BUF] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t ETH_LWIP_0_rx_buf[ETH_LWIP_0_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t ETH_LWIP_0_tx_buf[ETH_LWIP_0_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
#else
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_rx_desc[ETH_LWIP_0_NUM_RX_BUF];
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_LWIP_0_tx_desc[ETH_LWIP_0_NUM_TX_BUF];
static __attribute__((aligned(4))) uint8_t ETH_LWIP_0_rx_buf[ETH_LWIP_0_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE];
static __attribute__((aligned(4))) uint8_t ETH_LWIP_0_tx_buf[ETH_LWIP_0_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE];
#endif


const XMC_ETH_PHY_CONFIG_t eth_phy_config =
{
  .interface = XMC_ETH_LINK_INTERFACE_MII,
  .enable_auto_negotiate = true
};

XMC_ETH_MAC_t eth_mac =
{
  .regs = ETH0,
  .address = MAC_ADDR,
  .rx_desc = ETH_LWIP_0_rx_desc,
  .tx_desc = ETH_LWIP_0_tx_desc,
  .rx_buf = &ETH_LWIP_0_rx_buf[0][0],
  .tx_buf = &ETH_LWIP_0_tx_buf[0][0],
  .num_rx_buf = ETH_LWIP_0_NUM_RX_BUF,
  .num_tx_buf = ETH_LWIP_0_NUM_TX_BUF
};

struct netif xnetif = 
{
  /* set MAC hardware address length */
  .hwaddr_len = (u8_t)ETHARP_HWADDR_LEN,

  /* set MAC hardware address */
  .hwaddr =  {(u8_t)MAC_ADDR0, (u8_t)MAC_ADDR1,
              (u8_t)MAC_ADDR2, (u8_t)MAC_ADDR3,
              (u8_t)MAC_ADDR4, (u8_t)MAC_ADDR5},

  /* maximum transfer unit */
  .mtu = 1500U,

  .name = {IFNAME0, IFNAME1},
};

/*Local function declarations*/
static void low_level_init(struct netif *netif);
static err_t low_level_output(struct netif *netif, struct pbuf *p);
static struct pbuf * low_level_input(void);
static void ethernetif_input(void *arg);
err_t ethernetif_init(struct netif *netif);
static void ethernetif_link_callback(struct netif *netif);
static void ethernetif_link_status(void *args);

static void ethernetif_link_callback(struct netif *netif)
{
  XMC_ETH_LINK_SPEED_t speed;
  XMC_ETH_LINK_DUPLEX_t duplex;
  bool phy_autoneg_state;
  uint32_t retries = 0U;

  if (netif_is_link_up(netif))
  {

    /* If autonegotiation is enabled */
    do {
      phy_autoneg_state = XMC_ETH_PHY_IsAutonegotiationCompleted(&eth_mac, ETH_LWIP_0_PHY_ADDR);
      retries++;
    } while ((phy_autoneg_state == false) && (retries < ETH_LWIP_PHY_MAX_RETRIES));
    
    if(phy_autoneg_state == false)
    {
      ETH_LWIP_Error(ETH_LWIP_ERROR_PHY_TIMEOUT);
    }
	
    speed = XMC_ETH_PHY_GetLinkSpeed(&eth_mac, ETH_LWIP_0_PHY_ADDR);
    duplex = XMC_ETH_PHY_GetLinkDuplex(&eth_mac, ETH_LWIP_0_PHY_ADDR);
	
    XMC_ETH_MAC_SetLink(&eth_mac, speed, duplex);
    /* Enable ethernet interrupts */
    XMC_ETH_MAC_EnableEvent(&eth_mac, (uint32_t)XMC_ETH_MAC_EVENT_RECEIVE);

    NVIC_SetPriority((IRQn_Type)108, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 62U, 0U));
    NVIC_ClearPendingIRQ((IRQn_Type)108);
    NVIC_EnableIRQ((IRQn_Type)108);
    XMC_ETH_MAC_EnableTx(&eth_mac);
    XMC_ETH_MAC_EnableRx(&eth_mac);

    netif_set_up(&xnetif);

#if LWIP_DHCP == 1
    /* Start DHCP query */
    dhcp_start(&xnetif);
#elif LWIP_AUTOIP == 1
    /* Start AUTOIP probing */
    autoip_start(&xnetif);
#endif

  }
  else
  {
    /* Enable ethernet interrupts */
    XMC_ETH_MAC_DisableEvent(&eth_mac, (uint32_t)XMC_ETH_MAC_EVENT_RECEIVE);
    NVIC_DisableIRQ((IRQn_Type)108);

    XMC_ETH_MAC_DisableTx(&eth_mac);
    XMC_ETH_MAC_DisableRx(&eth_mac);

    netif_set_down(&xnetif);

#if LWIP_DHCP == 1
    /* Stop DHCP query */
    dhcp_stop(&xnetif);
#elif LWIP_AUTOIP == 1
    /* Stop AUTOIP probing */
    autoip_stop(&xnetif);
#endif

  }
}

static void ethernetif_link_status(void *args)
{
  XMC_UNUSED_ARG(args);


  if (XMC_ETH_PHY_GetLinkStatus(&eth_mac, ETH_LWIP_0_PHY_ADDR) == XMC_ETH_LINK_STATUS_DOWN)
  {
    if (netif_is_link_up(&xnetif))
    {
      netif_set_link_down(&xnetif);
    }
  }
  else
  {
    if (!netif_is_link_up(&xnetif))
    {
      netif_set_link_up(&xnetif);
    }
  }

  sys_timeout(1000U, ethernetif_link_status, NULL);

}



/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
  XMC_UNUSED_ARG(netif);

  XMC_ETH_MAC_PORT_CTRL_t port_control;
  XMC_GPIO_CONFIG_t gpio_config;

  gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
  gpio_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
  XMC_GPIO_Init(ETH_LWIP_0_CRS, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_CD, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RDV, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RXER, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RXD0, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RXD1, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RXD2, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RXD3, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_TX_CLK, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_RX_CLK, &gpio_config);
  XMC_GPIO_Init(ETH_LWIP_0_MDIO, &gpio_config);
  
  gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE;
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_MDC, &gpio_config);
  XMC_GPIO_SetHardwareControl(ETH_LWIP_0_MDIO, XMC_GPIO_HWCTRL_PERIPHERAL1);

  port_control.mode = XMC_ETH_MAC_PORT_CTRL_MODE_MII;
  port_control.rxd0 = (XMC_ETH_MAC_PORT_CTRL_RXD0_t)3U;
  port_control.rxd1 = (XMC_ETH_MAC_PORT_CTRL_RXD1_t)0U;
  port_control.rxd2 = (XMC_ETH_MAC_PORT_CTRL_RXD2_t)0U;
  port_control.rxd3 = (XMC_ETH_MAC_PORT_CTRL_RXD3_t)0U;
  port_control.crs = (XMC_ETH_MAC_PORT_CTRL_CRS_t)3U;
  port_control.clk_tx = (XMC_ETH_MAC_PORT_CTRL_RXER_t)0U;
  port_control.col = (XMC_ETH_MAC_PORT_CTRL_COL_t)3U;
  port_control.clk_rmii = (XMC_ETH_MAC_PORT_CTRL_CLK_RMII_t)3U;
  port_control.crs_dv = (XMC_ETH_MAC_PORT_CTRL_CRS_DV_t)3U;
  port_control.rxer = (XMC_ETH_MAC_PORT_CTRL_RXER_t)3U;
  port_control.mdio = (XMC_ETH_MAC_PORT_CTRL_MDIO_t)2U;
  XMC_ETH_MAC_SetPortControl(&eth_mac, port_control);

  XMC_ETH_MAC_Enable(&eth_mac);
  XMC_ETH_MAC_SetManagmentClockDivider(&eth_mac);

  int32_t status;
  if((status = XMC_ETH_PHY_Init(&eth_mac, ETH_LWIP_0_PHY_ADDR, &eth_phy_config)) != XMC_ETH_PHY_STATUS_OK)
  {
    ETH_LWIP_Error((ETH_LWIP_ERROR_t)status);
  }

  XMC_ETH_MAC_InitEx(&eth_mac);

  XMC_ETH_MAC_SetAddress(&eth_mac, eth_mac.address);
  XMC_ETH_MAC_DisableJumboFrame(&eth_mac);

  XMC_ETH_MAC_EnableReceptionBroadcastFrames(&eth_mac);
  
  gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE;
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_TXEN, &gpio_config);
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_TXER, &gpio_config);
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_TXD0, &gpio_config);
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_TXD1, &gpio_config);
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_TXD2, &gpio_config);
  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;
  XMC_GPIO_Init(ETH_LWIP_0_TXD3, &gpio_config);

}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  XMC_UNUSED_ARG(netif);

  struct pbuf *q;
  uint32_t framelen = 0U;
  uint8_t *buffer;
  
  if (p->tot_len > (u16_t)XMC_ETH_MAC_BUF_SIZE) {
    return ERR_BUF;
  }

  if (XMC_ETH_MAC_IsTxDescriptorOwnedByDma(&eth_mac))
  {
    XMC_ETH_MAC_ResumeTx(&eth_mac);

    return ERR_BUF;
  }
  else
  {
    buffer = XMC_ETH_MAC_GetTxBuffer(&eth_mac);

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE);    /* Drop the padding word */
#endif

    for(q = p; q != NULL; q = q->next)
    {
      /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
      MEMCPY(buffer, q->payload, q->len);
      framelen += (uint32_t)q->len;
      buffer += q->len;
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE);    /* Reclaim the padding word */
#endif

    XMC_ETH_MAC_SetTxBufferSize(&eth_mac, framelen);

    XMC_ETH_MAC_ReturnTxDescriptor(&eth_mac);
    XMC_ETH_MAC_ResumeTx(&eth_mac);

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
  }
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf * low_level_input(void)
{
  struct pbuf *p = NULL;
  struct pbuf *q;
  uint32_t len;
  uint8_t *buffer;

  if (XMC_ETH_MAC_IsRxDescriptorOwnedByDma(&eth_mac) == false)
  {
    len = XMC_ETH_MAC_GetRxFrameSize(&eth_mac);
  
    if ((len > 0U) && (len <= (uint32_t)XMC_ETH_MAC_BUF_SIZE))
    {
#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE;    /* allow room for Ethernet padding */
#endif
  
      /* We allocate a pbuf chain of pbufs from the pool. */
      p = pbuf_alloc(PBUF_RAW, (u16_t)len, PBUF_POOL);
    
      if (p != NULL)
      {
#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE);  /* drop the padding word */
#endif
  
        buffer = XMC_ETH_MAC_GetRxBuffer(&eth_mac);
  
        len = 0U;
        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
        for (q = p; q != NULL; q = q->next)
        {
          /* Read enough bytes to fill this pbuf in the chain. The
           * available data in the pbuf is given by the q->len
           * variable.
           * This does not necessarily have to be a memcpy, you can also preallocate
           * pbufs for a DMA-enabled MAC and after receiving truncate it to the
           * actually received size. In this case, ensure the tot_len member of the
           * pbuf is the sum of the chained pbuf len members.
           */
           MEMCPY(q->payload, &buffer[len], q->len);
           len += q->len;
        }
#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE);    /* Reclaim the padding word */
#endif
        LINK_STATS_INC(link.recv);

        XMC_ETH_MAC_ReturnRxDescriptor(&eth_mac);  
      }
    }
    else
    {
      /* Discard frame */
      XMC_ETH_MAC_ReturnRxDescriptor(&eth_mac);
      LINK_STATS_INC(link.memerr);
      LINK_STATS_INC(link.drop);
    }
  }

  XMC_ETH_MAC_ResumeRx(&eth_mac);

  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void ethernetif_input(void *arg)
{
  struct pbuf *p = NULL;
  struct eth_hdr *ethhdr;
  struct netif *netif = (struct netif *)arg;


    NVIC_DisableIRQ((IRQn_Type)108);

    p = low_level_input();

    while (p != NULL)
    {
   	  ethhdr = p->payload;
   	  switch (htons(ethhdr->type))
   	  {
   	    case ETHTYPE_IP:
   	    case ETHTYPE_ARP:
   	      /* full packet send to tcpip_thread to process */
          if (netif->input( p, netif) != ERR_OK)
          {
            pbuf_free(p);
          }

          break;

   	    default:
   	      pbuf_free(p);
   	      break;
   	  }
      
      p = low_level_input();
    }

    NVIC_ClearPendingIRQ((IRQn_Type)108);
    NVIC_EnableIRQ((IRQn_Type)108);



}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif* netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));


  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  /* device capabilities */
  xnetif.flags = NETIF_FLAG_ETHARP;

#if ETH_BROADCAST_EN == 1
  xnetif.flags |= NETIF_FLAG_BROADCAST;
#endif


  sys_timeout(1000U, ethernetif_link_status, NULL);

  netif_set_link_callback(netif, ethernetif_link_callback);


  return ERR_OK;
}

__WEAK void ETH_LWIP_UserIRQ(void)
{
}
   
void IRQ_Hdlr_108(void)
{
  XMC_ETH_MAC_ClearEventStatus(&eth_mac, XMC_ETH_MAC_EVENT_RECEIVE);
    ethernetif_input(&xnetif);
  
  ETH_LWIP_UserIRQ(); 
}

void ETH_LWIP_Poll(void)
{
    ethernetif_input(&xnetif);
}

/*Weak function to be called incase of error*/
__WEAK void ETH_LWIP_Error (ETH_LWIP_ERROR_t error_code)
{
  switch (error_code)
  {
    case ETH_LWIP_ERROR_PHY_DEVICE_ID:
       /* Wrong PHY address configured in the ETH_LWIP APP Network Interface.
        * Because the connect PHY does not match the configuration or the PHYADR is wrong*/
       break;

   case ETH_LWIP_ERROR_PHY_TIMEOUT:
      /* PHY did not respond.*/
      break;

   case ETH_LWIP_ERROR_PHY_ERROR:
     /*PHY register update failed*/
     break;

   default:
     break;
  }

  for (;;);
}
