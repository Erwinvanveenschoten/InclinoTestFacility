/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
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
#ifndef SYS_ARCH_H
#define SYS_ARCH_H

#if !NO_SYS

#include "CMSIS_RTOS/cmsis_rtos.h"

#if defined(CMSIS_RTOS_CONFIG_IMPL_RTX)

typedef struct sys_sem
{
  osSemaphoreId id;
  osSemaphoreDef_t def;
  uint32_t cb[2];
} sys_sem_t;

#define sys_sem_valid(sem) ((sem)->id != NULL)
#define sys_sem_set_invalid(sem) ((sem)->id = NULL)

typedef struct sys_mutex
{
  osMutexId id;
  osMutexDef_t def;
  uint32_t cb[4];
} sys_mutex_t;

#define sys_mutex_valid(mutex) ((mutex)->id != NULL)
#define sys_mutex_set_invalid(mutex) ((mutex)->id = NULL)

#define sys_msleep(ms) osDelay(ms)

#define SYS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SYS_MBOX_SIZE SYS_MAX(DEFAULT_RAW_RECVMBOX_SIZE, SYS_MAX(DEFAULT_UDP_RECVMBOX_SIZE, SYS_MAX(DEFAULT_TCP_RECVMBOX_SIZE, SYS_MAX(DEFAULT_ACCEPTMBOX_SIZE, TCPIP_MBOX_SIZE))))

typedef struct sys_mbox
{
  osMessageQId id;
  osMessageQDef_t def;
  uint32_t q[4 + SYS_MBOX_SIZE];
} sys_mbox_t;

#define sys_mbox_valid(mbox) ((mbox)->id != NULL)
#define sys_mbox_set_invalid(mbox) ((mbox)->id = NULL)

typedef osThreadId sys_thread_t;

#define sys_thread_valid_val(thread)   (thread != NULL)
#define sys_thread_valid(thread)       (((thread) != NULL) && sys_thread_valid_val(*(thread)))

typedef uint32_t sys_prot_t;

#else /* CMSIS_RTOS_CONFIG_IMPL_RTX */
#if defined(CMSIS_RTOS_CONFIG_IMPL_FREERTOS)
#include "CMSIS_RTOS/FREERTOS/cmsis_os2.h"
#else
#include "CMSIS_RTOS_RTX5/Include/cmsis_os2.h"
#endif

#include "ETH_LWIP/lwip/include/lwip/opt.h"
#include "ETH_LWIP/lwip/include/lwip/arch.h"

/** This is returned by _fromisr() sys functions to tell the outermost function
 * that a higher priority task was woken and the scheduler needs to be invoked.
 */
#define ERR_NEED_SCHED 123

void sys_arch_msleep(u32_t delay_ms);
#define sys_msleep(ms) sys_arch_msleep(ms)

#if SYS_LIGHTWEIGHT_PROT
typedef int32_t sys_prot_t;
#endif /* SYS_LIGHTWEIGHT_PROT */

#if !LWIP_COMPAT_MUTEX
struct _sys_mut {
  void *mut;
};
typedef struct _sys_mut sys_mutex_t;
#define sys_mutex_valid_val(mutex)   ((mutex).mut != NULL)
#define sys_mutex_valid(mutex)       (((mutex) != NULL) && sys_mutex_valid_val(*(mutex)))
#define sys_mutex_set_invalid(mutex) ((mutex)->mut = NULL)
#endif /* !LWIP_COMPAT_MUTEX */


struct _sys_sem {
  void *sem;
};
typedef struct _sys_sem sys_sem_t;
#define sys_sem_valid_val(sema)   ((sema).sem != NULL)
#define sys_sem_valid(sema)       (((sema) != NULL) && sys_sem_valid_val(*(sema)))
#define sys_sem_set_invalid(sema) ((sema)->sem = NULL)

struct _sys_mbox {
  void *mbx;
};
typedef struct _sys_mbox sys_mbox_t;
#define sys_mbox_valid_val(mbox)   ((mbox).mbx != NULL)
#define sys_mbox_valid(mbox)       (((mbox) != NULL) && sys_mbox_valid_val(*(mbox)))
#define sys_mbox_set_invalid(mbox) ((mbox)->mbx = NULL)


struct _sys_thread {
  void *thread_handle;
};
typedef struct _sys_thread sys_thread_t;
#define sys_thread_valid_val(thread)   ((thread).thread_handle != NULL)
#define sys_thread_valid(thread)       (((thread) != NULL) && sys_thread_valid_val(*(thread)))

#endif /* !CMSIS_RTOS_CONFIG_IMPL_RTX */
#endif /* !NO_SYS */

#endif /* SYS_ARCH_H */
