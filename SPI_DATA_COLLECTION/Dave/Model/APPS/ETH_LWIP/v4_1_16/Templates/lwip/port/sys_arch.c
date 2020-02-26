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

#include "ETH_LWIP/lwip/include/lwip/opt.h"

#if !NO_SYS

#include <string.h>
#include "CMSIS_RTOS/cmsis_rtos.h"

#include "ETH_LWIP/lwip/include/lwip/sys.h"
#include "ETH_LWIP/lwip/include/lwip/stats.h"
#include "ETH_LWIP/lwip/include/lwip/debug.h"

#if defined ( __CC_ARM )
int32_t errno;
#endif

#if !defined(CMSIS_RTOS_CONFIG_IMPL_FREERTOS)
#if (CMSIS_RTOS_PRIVCNT < 2)
#error "ETH_LWIP: requires at least 2 threads with user-provided stack size when running in an OS environment"
#endif

#if (CMSIS_RTOS_PRIVSTKSIZE < ETH_LWIP_MIN_PRIV_STACK_SIZE)
#error "ETH_LWIP: requires at least ETH_LWIP_MIN_PRIV_STACK_SIZE bytes of stack size for threads with user-provided stack size"
#endif
#endif

#if defined(CMSIS_RTOS_CONFIG_IMPL_RTX)
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  memset(sem, 0, sizeof(sys_sem_t));
  sem->def.semaphore = sem->cb;

  sem->id = osSemaphoreCreate(&sem->def, count);

  if (sem->id == NULL)
  {
    SYS_STATS_INC(sem.err);
    return ERR_MEM;
  }
  else
  {
    SYS_STATS_INC_USED(sem);
    return ERR_OK;
  }
}

void sys_sem_free(sys_sem_t *sem)
{
  if (sem->id != NULL)
  {
    osSemaphoreDelete(sem->id);
    SYS_STATS_DEC(sem.used);
  }

}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  int32_t  count;
  uint32_t tick_start;
  uint32_t tick_stop;
  uint32_t tick_elapsed;

  tick_start = osKernelSysTick();

  if (timeout == 0)
  {
    count = osSemaphoreWait(sem->id, osWaitForever);
  }
  else
  {
    count = osSemaphoreWait(sem->id, timeout);
  }

  tick_stop = osKernelSysTick();

  if (count < 1)
  {
    return SYS_ARCH_TIMEOUT;
  }

  /* Take care of wrap-around. */
  if (tick_stop >= tick_start)
  {
    tick_elapsed = tick_stop - tick_start;
  }
  else
  {
    tick_elapsed = (UINT32_MAX - tick_start) + tick_stop;
  }

  return (u32_t)(1000U * ((float)tick_elapsed / osKernelSysTickFrequency));
}

void sys_sem_signal(sys_sem_t *sem)
{
  if (sem->id != NULL)
  {
    osSemaphoreRelease(sem->id);
  }
}

err_t sys_mutex_new(sys_mutex_t *mutex)
{
  memset(mutex, 0, sizeof(sys_mutex_t));
  mutex->def.mutex = mutex->cb;

  mutex->id = osMutexCreate(&mutex->def);

  if (mutex->id == NULL)
  {
    SYS_STATS_INC(mutex.err);
    return ERR_MEM;
  }
  else
  {
    SYS_STATS_INC_USED(mutex);
    return ERR_OK;
  }
}

void sys_mutex_free(sys_mutex_t *mutex)
{
  if (mutex->id != NULL)
  {
    osMutexDelete(mutex->id);
    SYS_STATS_DEC(mutex.used);    
  }
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
  if (mutex->id != NULL)
  {
    osMutexWait(mutex->id, osWaitForever);
  }
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
  if (mutex->id != NULL)
  {
    osMutexRelease(mutex->id);
  }
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
  if (size > SYS_MBOX_SIZE) {
    return ERR_MEM;
  }

  memset(mbox, 0, sizeof(sys_mbox_t));
  mbox->def.pool = mbox->q;
  mbox->def.queue_sz = size;

  mbox->id = osMessageCreate(&mbox->def, NULL);

  if (mbox->id == NULL)
  {
    SYS_STATS_INC(mbox.err);
    return ERR_MEM;
  }
  else
  {
    SYS_STATS_INC_USED(mbox);
    return ERR_OK;
  }
}

void sys_mbox_free(sys_mbox_t *mbox)
{
  if (mbox->id != NULL)
  {
    osMessageGet(mbox->id, 0);

    mbox->id = NULL;
    SYS_STATS_DEC(mbox.used);
  }    
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
  if (mbox->id != NULL)
  {
    osMessagePut(mbox->id, (uint32_t)msg, osWaitForever);
  }
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
  osStatus status;

  if (mbox->id != NULL)
  {
    status = osMessagePut(mbox->id, (uint32_t)msg, 0);
    if (status != osOK)
    {
      return ERR_MEM;
    }
    return ERR_OK;
  }
  else
  {
    return ERR_ARG;
  }
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
  osEvent  event;
  uint32_t tick_start;
  uint32_t tick_stop;
  uint32_t tick_elapsed;

  tick_start = osKernelSysTick();

  if (timeout == 0)
  {
    event = osMessageGet(mbox->id, osWaitForever);
  } else
  {
    event = osMessageGet(mbox->id, timeout);
  }

  if (event.status != osEventMessage)
  {
    return SYS_ARCH_TIMEOUT;
  }

  if (msg != NULL)
  {
    *msg = event.value.p;
  }

  tick_stop = osKernelSysTick();

  /* Take care of wrap-around. */
  if (tick_stop >= tick_start)
  {
    tick_elapsed = tick_stop - tick_start;
  }
  else
  {
    tick_elapsed = (UINT32_MAX - tick_start) + tick_stop;
  }

  return (u32_t)(1000U * ((float)tick_elapsed / osKernelSysTickFrequency));
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  osEvent event;

  if (mbox->id != NULL)
  {
    event = osMessageGet(mbox->id, 0);
    if (event.status != osEventMessage) 
    {
      return SYS_MBOX_EMPTY;
    }

    if (msg != NULL) 
    {
      *msg = event.value.p;
    }

    return ERR_OK;
  }
  else
  {
    return ERR_ARG;
  }
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{
  LWIP_UNUSED_ARG(name);

  osThreadId threadId;
  osThreadDef_t threadDef;

  threadDef.pthread = (os_pthread)function;
  threadDef.tpriority = (osPriority)prio;
  threadDef.instances = 1;
  threadDef.stacksize = stacksize;

  threadId = osThreadCreate(&threadDef, arg);
  LWIP_PLATFORM_ASSERT(threadId != NULL);

  return threadId;
}

#if SYS_LIGHTWEIGHT_PROT

static sys_mutex_t lwip_mutex;

sys_prot_t sys_arch_protect(void)
{
  if (osKernelRunning ())
  sys_mutex_lock(&lwip_mutex);
  return 0;
}

void sys_arch_unprotect(sys_prot_t pval)
{
  LWIP_UNUSED_ARG(pval);
  if (osKernelRunning ())
  sys_mutex_unlock(&lwip_mutex);
}

#endif /* SYS_LIGHTWEIGHT_PROT */

extern uint32_t os_time; // Time keping in RTOS RTX systick handler

uint32_t sys_now(void)
{
#if CMSIS_RTOS_RTX_TICK == 1000
  return os_time;
#else
  return (os_time * (CMSIS_RTOS_RTX_TICK / 1000));
#endif  
}

void sys_init(void)
{
#if SYS_LIGHTWEIGHT_PROT
  sys_mutex_new(&lwip_mutex);
#endif
}
#else /* defined(CMSIS_RTOS_CONFIG_IMPL_RTX) */

/** Set this to 1 to use a mutex for SYS_ARCH_PROTECT() critical regions.
 * Default is 0 and locks interrupts/scheduler for SYS_ARCH_PROTECT().
 */
#ifndef LWIP_SYS_ARCH_PROTECT_USES_MUTEX
#define LWIP_SYS_ARCH_PROTECT_USES_MUTEX     0
#endif

/** Set this to 1 to include a sanity check that SYS_ARCH_PROTECT() and
 * SYS_ARCH_UNPROTECT() are called matching.
 */
#ifndef LWIP_SYS_ARCH_PROTECT_SANITY_CHECK
#define LWIP_SYS_ARCH_PROTECT_SANITY_CHECK   0
#endif

/** Set this to 1 to let sys_mbox_free check that queues are empty when freed */
#ifndef LWIP_CHECK_QUEUE_EMPTY_ON_FREE
#define LWIP_CHECK_QUEUE_EMPTY_ON_FREE       0
#endif


#if SYS_LIGHTWEIGHT_PROT && LWIP_SYS_ARCH_PROTECT_USES_MUTEX
static osMutexId_t sys_arch_protect_mutex;
#endif

#if SYS_LIGHTWEIGHT_PROT && LWIP_SYS_ARCH_PROTECT_SANITY_CHECK
static sys_prot_t sys_arch_protect_nesting;
#endif

void sys_init(void)
{
#if SYS_LIGHTWEIGHT_PROT && LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  /* initialize sys_arch_protect global mutex */
  static const osMutexAttr_t attr = {
    NULL, osMutexPrioInherit | osMutexRecursive | osMutexRobust, NULL, 0
  };

  sys_arch_protect_mutex = osMutexNew(&attr);
  LWIP_ASSERT("failed to create sys_arch_protect mutex", sys_arch_protect_mutex != NULL);
#endif /* SYS_LIGHTWEIGHT_PROT && LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX */
}

u32_t sys_now(void)
{
  return (osKernelGetTickCount() * 1000) / osKernelGetTickFreq();
}

u32_t sys_jiffies(void)
{
  return osKernelGetTickCount();
}

#if SYS_LIGHTWEIGHT_PROT

sys_prot_t sys_arch_protect(void)
{
#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  BaseType_t ret;
  LWIP_ASSERT("sys_arch_protect_mutex != NULL", sys_arch_protect_mutex != NULL);

  ret = osMutexAcquire(sys_arch_protect_mutex, osWaitForever);
  LWIP_ASSERT("sys_arch_protect failed to take the mutex", ret == osOK);
#else /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */
  osKernelLock();
#endif /* LWIP_FREERTOS_SYS_ARCH_PROTECT_USES_MUTEX */
#if LWIP_SYS_ARCH_PROTECT_SANITY_CHECK
  {
    /* every nested call to sys_arch_protect() returns an increased number */
    sys_prot_t ret = sys_arch_protect_nesting;
    sys_arch_protect_nesting++;
    LWIP_ASSERT("sys_arch_protect overflow", sys_arch_protect_nesting > ret);
    return ret;
  }
#else
  return 1;
#endif
}

void sys_arch_unprotect(sys_prot_t pval)
{
#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  osStatus_t ret;
#endif
#if LWIP_SYS_ARCH_PROTECT_SANITY_CHECK
  LWIP_ASSERT("unexpected sys_arch_protect_nesting", sys_arch_protect_nesting > 0);
  sys_arch_protect_nesting--;
  LWIP_ASSERT("unexpected sys_arch_protect_nesting", sys_arch_protect_nesting == pval);
#endif

#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  LWIP_ASSERT("sys_arch_protect_mutex != NULL", sys_arch_protect_mutex != NULL);

  ret = osMutexRelease(sys_arch_protect_mutex);
  LWIP_ASSERT("sys_arch_unprotect failed to give the mutex", ret == osOK);
#else /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */
  osKernelUnlock();
#endif /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */
  LWIP_UNUSED_ARG(pval);
}

#endif /* SYS_LIGHTWEIGHT_PROT */

void sys_arch_msleep(u32_t delay_ms)
{
  uint32_t delay_ticks = delay_ms * (osKernelGetTickFreq() / 1000);
  osDelay(delay_ticks);
}


#if !LWIP_COMPAT_MUTEX
/* Mutexes */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
  LWIP_ASSERT("mutex != NULL", mutex != NULL);

  // CMSIS-FreeRTOS  Version 10.2.0 Limitations
  // Robust mutex objects are not supported (osMutexNew returns NULL when osMutexRobust attribute is specified).
  // Priority inherit protocol is used as default mutex behavior (osMutexNew creates priority inherit mutex object by default and ignores osMutexPrioInherit attribute when specified).
  static const osMutexAttr_t attr = {
    NULL, osMutexPrioInherit | osMutexRecursive, NULL, 0
  };

  mutex->mut = osMutexNew(&attr);
  if (mutex->mut == NULL) {
    SYS_STATS_INC(mutex.err);
    return ERR_MEM;
  }
  SYS_STATS_INC_USED(mutex);
  return ERR_OK;
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
  osStatus_t ret;
  LWIP_ASSERT("mutex != NULL", mutex != NULL);
  LWIP_ASSERT("mutex->mut != NULL", mutex->mut != NULL);

  ret = osMutexAcquire(mutex->mut, osWaitForever);
  LWIP_ASSERT("failed to take the mutex", ret == osOK);
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
  osStatus_t ret;
  LWIP_ASSERT("mutex != NULL", mutex != NULL);
  LWIP_ASSERT("mutex->mut != NULL", mutex->mut != NULL);

  ret = osMutexRelease(mutex->mut);
  LWIP_ASSERT("failed to give the mutex", ret == osOK);
}

void sys_mutex_free(sys_mutex_t *mutex)
{
  LWIP_ASSERT("mutex != NULL", mutex != NULL);
  LWIP_ASSERT("mutex->mut != NULL", mutex->mut != NULL);

  SYS_STATS_DEC(mutex.used);
  osMutexDelete(mutex->mut);
  mutex->mut = NULL;
}

#endif /* !LWIP_COMPAT_MUTEX */


/* Semaphores */
err_t sys_sem_new(sys_sem_t *sem, u8_t initial_count)
{
  LWIP_ASSERT("sem != NULL", sem != NULL);
  LWIP_ASSERT("initial_count invalid (not 0 or 1)",
    (initial_count == 0) || (initial_count == 1));

  sem->sem = osSemaphoreNew(1, initial_count, NULL);
  if(sem->sem == NULL) {
    SYS_STATS_INC(sem.err);
    return ERR_MEM;
  }
  SYS_STATS_INC_USED(sem);

  if(initial_count == 1) {
	osStatus_t ret = osSemaphoreRelease(sem->sem);
    LWIP_ASSERT("sys_sem_new: initial give failed", ret == osOK);
  }
  return ERR_OK;
}

void sys_sem_signal(sys_sem_t *sem)
{
  osStatus_t ret;
  LWIP_ASSERT("sem != NULL", sem != NULL);
  LWIP_ASSERT("sem->sem != NULL", sem->sem != NULL);

  ret = osSemaphoreRelease(sem->sem);
  /* queue full is OK, this is a signal only... */
  LWIP_ASSERT("sys_sem_signal: sane return value", (ret == osOK));
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout_ms)
{
  osStatus_t ret;
  LWIP_ASSERT("sem != NULL", sem != NULL);
  LWIP_ASSERT("sem->sem != NULL", sem->sem != NULL);

  if(!timeout_ms) {
    /* wait infinite */
    ret = osSemaphoreAcquire(sem->sem, osWaitForever);
    LWIP_ASSERT("taking semaphore failed", ret == osOK);
  } else {
	uint32_t timeout_ticks = timeout_ms * (osKernelGetTickFreq() / 1000);
    ret = osSemaphoreAcquire(sem->sem, timeout_ticks);
    if (ret == osErrorTimeout) {
      /* timed out */
      return SYS_ARCH_TIMEOUT;
    }
    LWIP_ASSERT("taking semaphore failed", ret == osOK);
  }

  /* Old versions of lwIP required us to return the time waited.
     This is not the case any more. Just returning != SYS_ARCH_TIMEOUT
     here is enough. */
  return 1;
}


void sys_sem_free(sys_sem_t *sem)
{
  LWIP_ASSERT("sem != NULL", sem != NULL);
  LWIP_ASSERT("sem->sem != NULL", sem->sem != NULL);

  SYS_STATS_DEC(sem.used);
  osSemaphoreDelete(sem->sem);
  sem->sem = NULL;
}


/* Mailbox */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("size > 0", size > 0);

  mbox->mbx = osMessageQueueNew((uint32_t)size, sizeof(void *), NULL);
  if(mbox->mbx == NULL) {
    SYS_STATS_INC(mbox.err);
    return ERR_MEM;
  }
  SYS_STATS_INC_USED(mbox);
  return ERR_OK;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
  osStatus_t ret;
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mbx != NULL", mbox->mbx != NULL);

  ret = osMessageQueuePut(mbox->mbx, &msg, 0, osWaitForever);
  LWIP_ASSERT("mbox post failed", ret == osOK);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
  osStatus_t ret;
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mbx != NULL", mbox->mbx != NULL);

  ret = osMessageQueuePut(mbox->mbx, &msg, 0, 0);
  if (ret == osOK) {
    return ERR_OK;
  } else {
    LWIP_ASSERT("mbox trypost failed", ret == osErrorResource);
    SYS_STATS_INC(mbox.err);
    return ERR_MEM;
  }
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
  osStatus_t ret;
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mbx != NULL", mbox->mbx != NULL);

  ret = osMessageQueuePut(mbox->mbx, &msg, 0, 0);
  if (ret == osOK) {
    return ERR_OK;
  } else {
    LWIP_ASSERT("mbox trypost failed", ret == osErrorResource);
    SYS_STATS_INC(mbox.err);
    return ERR_MEM;
  }
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout_ms)
{
  osStatus_t ret;
  void *msg_dummy;
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mbx != NULL", mbox->mbx != NULL);

  if (!msg) {
    msg = &msg_dummy;
  }

  if (!timeout_ms) {
    /* wait infinite */
    ret = osMessageQueueGet(mbox->mbx, &(*msg), 0, osWaitForever);
    LWIP_ASSERT("mbox fetch failed", ret == osOK);
  } else {
    uint32_t timeout_ticks = timeout_ms * (osKernelGetTickFreq() / 1000);
    ret = osMessageQueueGet(mbox->mbx, &(*msg), 0, timeout_ticks);
    if (ret == osErrorTimeout) {
      /* timed out */
      *msg = NULL;
      return SYS_ARCH_TIMEOUT;
    }
    LWIP_ASSERT("mbox fetch failed", ret == osOK);
  }

  /* Old versions of lwIP required us to return the time waited.
     This is not the case any more. Just returning != SYS_ARCH_TIMEOUT
     here is enough. */
  return 1;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  osStatus_t ret;
  void *msg_dummy;
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mbx != NULL", mbox->mbx != NULL);

  if (!msg) {
    msg = &msg_dummy;
  }

  ret = osMessageQueueGet(mbox->mbx, &(*msg), 0, 0);
  if (ret == osErrorResource) {
    *msg = NULL;
    return SYS_MBOX_EMPTY;
  }
  LWIP_ASSERT("mbox fetch failed", ret == pdTRUE);

  /* Old versions of lwIP required us to return the time waited.
     This is not the case any more. Just returning != SYS_ARCH_TIMEOUT
     here is enough. */
  return 1;

}

void sys_mbox_free(sys_mbox_t *mbox)
{
  LWIP_ASSERT("mbox != NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mbx != NULL", mbox->mbx != NULL);

#if LWIP_CHECK_QUEUE_EMPTY_ON_FREE
  {
	uint32_t msgs_waiting = osMessageQueueGetCount(mbox->mbx);
    LWIP_ASSERT("mbox quence not empty", msgs_waiting == 0);

    if (msgs_waiting != 0) {
      SYS_STATS_INC(mbox.err);
    }
  }
#endif

  osMessageQueueDelete(mbox->mbx);

  SYS_STATS_DEC(mbox.used);

}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{
  osThreadAttr_t attr;
  osThreadId_t   id;
  sys_thread_t lwip_thread;

  LWIP_ASSERT("invalid stacksize", stacksize > 0);

  memset (&attr, 0, sizeof(attr));
  attr.name       = name;
  attr.stack_size = (uint32_t)stacksize;
  attr.priority = prio;

  id = osThreadNew(function, arg, &attr);
  LWIP_ASSERT("task creation failed", id != NULL);

  lwip_thread.thread_handle = id;
  return lwip_thread;
}

#endif
#else /* !NO_SYS */


#include <SYSTIMER/systimer.h>
#include "include/arch/cc.h"
#include "../include/lwip/sys.h"

uint32_t sys_now(void)
{
  return (u32_t)(SYSTIMER_GetTickCount() * SYSTIMER_TICK_PERIOD * 1000);
}

void sys_init()
{
}

#endif
