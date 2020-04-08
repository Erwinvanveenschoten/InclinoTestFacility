/**
 * @file systimer.c
 * @date 2016-08-05
 * This file is generated by DAVE, User modification to this file will be overwritten at the next code generation.
 *
 * @cond
 ***********************************************************************************************************************
 * SYSTIMER v4.1.16 - The SYSTIMER APP uses the SysTick interrupt to call user functions periodically at a specified
 *                   rate or after a given period of time expires.
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
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided with the distribution.
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
 *     - Initial version.<br>
 *
 * 2015-05-19:
 *     - Structure name changed from SYSTIMER_TimerObject to SYSTIMER_OBJECT_t and SYSTIMER_INIT_t to SYSTIMER_t. And
 *       also changed its parameter name.<br>
 *     - Enum name changed from SYSTIMER_TimerStateType, SYSTIMER_TimerType to SYSTIMER_STATE_t and SYSTIMER_MODE_t.
 *       And also changed its parameter name.<br>
 *     - Supported new APIs SYSTIMER_GetTimerState() and  SYSTIMER_GetTickCount().<br>
 *
 * 2015-05-29:
 *     - SYSTIMER_TICK_PERIOD_US macro is used for calculation purpose in SYSTIMER_CreateTimer and
 *       SYSTIMER_RestartTimer APIs<br>
 *
 * 2015-07-31:
 *     - Fixed return value issues for APIs SYSTIMER_StartTimer(), SYSTIMER_StopTimer(), SYSTIMER_RestartTimer(),
 *       and SYSTIMER_DeleteTimer().<br>
 *
 * 2015-08-25:
 *     - Fixed GUI tool tip issues. Updated SYSTIMER.h file for help documentation range mistakes.<br>
 *       Support for XMC4700 / XMC4800 added.
 *     - Removed CPU_CTRL_XMC1_Init / CPU_CTRL_XMC4_Init function call.
 *
 * 2015-09-29:
 *     - Fixed wrong parameter check in the CreateTimer API.
 *       CreateTimer return 0 (indicates error) if the requested timer has a period less than the SysTick timer period
 *
 * 2016-08-05:
 *     - Fixed reentracy problems when using SYSTIMER_StopTimer
 *
 * 2016-12-12:
 *     - Fixed reentracy problems when modifying the global list of timers from ISR routines.
 *     - Added SYSTIMER_CreateTimerFromISR(), SYSTIMER_StartTimerFromISR(), SYSTIMER_StopTimerFromISR(), SYSTIMER_RestartTimerFromISR(), SYSTIMER_DeleteTimerFromISR()
 *
 * @endcond
 *
 */
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

/* Included to access APP data structure, functions & enumerations */
#include "systimer.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

#define HW_TIMER_ADDITIONAL_CNT (1U)

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/
/* SYSTIMER_OBJECT structure acts as the timer control block */

typedef struct SYSTIMER_OBJECT
{
  struct SYSTIMER_OBJECT *next; /**< pointer to next timer control block */
  struct SYSTIMER_OBJECT *prev; /**< Pointer to previous timer control block */
  SYSTIMER_CALLBACK_t callback; /**< Callback function pointer */
  SYSTIMER_MODE_t mode; /**< timer Type (single shot or periodic) */
  SYSTIMER_STATE_t state; /**< timer state */
  void *args; /**< Parameter to callback function */
  uint32_t id; /**< timer ID */
  uint32_t count; /**< timer count value */
  uint32_t reload; /**< timer Reload count value */
  bool delete_swtmr; /**< To delete the timer */
} SYSTIMER_OBJECT_t;

/** Table which save timer control block. */
SYSTIMER_OBJECT_t g_timer_tbl[SYSTIMER_CFG_MAX_TMR];

/* The header of the timer Control list. */
SYSTIMER_OBJECT_t *g_timer_list = NULL;

/* Timer ID tracker */
uint32_t g_timer_tracker = 0U;

/* SysTick counter */
volatile uint32_t g_systick_count = 0U;

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/

/*
 * This function is called to insert a timer into the timer list.
 */
static void SYSTIMER_lInsertTimerList(uint32_t tbl_index);

/*
 * This function is called to remove a timer from the timer list.
 */
static void SYSTIMER_lRemoveTimerList(uint32_t tbl_index);

/*
 * Handler function  called from SysTick event handler.
 */
static void SYSTIMER_lTimerHandler(void);

/*
 * SysTick handler which is the main interrupt service routine to service the
 * system timer's configured
 */
void SysTick_Handler(void);

/**********************************************************************************************************************
* API IMPLEMENTATION
**********************************************************************************************************************/
/*
 * This function is called to insert a timer into the timer list.
 */
static void SYSTIMER_lInsertTimerList(uint32_t tbl_index)
{
  SYSTIMER_OBJECT_t *object_ptr;
  int32_t delta_ticks;
  int32_t timer_count;
  bool found_flag = false;
   /* Get timer time */
  timer_count = (int32_t)g_timer_tbl[tbl_index].count;
  /* Check if Timer list is NULL */
  if (NULL == g_timer_list)
  {
    /* Set this as first Timer */
    g_timer_list = &g_timer_tbl[tbl_index];
  }
  /* If not, find the correct place, and insert the specified timer */
  else
  {
    object_ptr = g_timer_list;
    /* Get timer tick */
    delta_ticks = timer_count;
    /* Find correct place for inserting the timer */
    while ((NULL != object_ptr) && (false == found_flag))
    {
      /* Get timer Count Difference */
      delta_ticks -= (int32_t)object_ptr->count;
      /* Check for delta ticks < 0 */
      if (delta_ticks <= 0)
      {
        /* Check If head item */
        if (NULL != object_ptr->prev)
        {
          /* If Insert to list */
          object_ptr->prev->next = &g_timer_tbl[tbl_index];
          g_timer_tbl[tbl_index].prev = object_ptr->prev;
          g_timer_tbl[tbl_index].next = object_ptr;
          object_ptr->prev = &g_timer_tbl[tbl_index];
        }
        else
        {
          /* Set Timer as first item */
          g_timer_tbl[tbl_index].next = g_timer_list;
          g_timer_list->prev = &g_timer_tbl[tbl_index];
          g_timer_list = &g_timer_tbl[tbl_index];
        }
        g_timer_tbl[tbl_index].count = g_timer_tbl[tbl_index].next->count + (uint32_t)delta_ticks;
        g_timer_tbl[tbl_index].next->count  -= g_timer_tbl[tbl_index].count;
        found_flag = true;
      }
      /* Check for last item in list */
      else
      {
        if ((delta_ticks > 0) && (NULL == object_ptr->next))
        {
          /* Yes, insert into */
          g_timer_tbl[tbl_index].prev = object_ptr;
          object_ptr->next = &g_timer_tbl[tbl_index];
          g_timer_tbl[tbl_index].count = (uint32_t)delta_ticks;
          found_flag = true;
        }
      }
      /* Get the next item in timer list */
      object_ptr = object_ptr->next;
    }
  }
}

/*
 * This function is called to remove a timer from the timer list. 
 */
static void SYSTIMER_lRemoveTimerList(uint32_t tbl_index)
{
  SYSTIMER_OBJECT_t *object_ptr;
  object_ptr = &g_timer_tbl[tbl_index];
  /* Check whether only one timer available */
  if ((NULL == object_ptr->prev) && (NULL == object_ptr->next ))
  {
    /* set timer list as NULL */ 
    g_timer_list = NULL;                  
  }
  /* Check if the first item in timer list */
  else if (NULL == object_ptr->prev)
  {
    /* Remove timer from list, and reset timer list */
    g_timer_list  = object_ptr->next;
    g_timer_list->prev = NULL;
    g_timer_list->count += object_ptr->count;
    object_ptr->next    = NULL;
  }
  /* Check if the last item in timer list */
  else if (NULL == object_ptr->next)
  {
    /* Remove timer from list */
    object_ptr->prev->next = NULL;
    object_ptr->prev = NULL;
  }
  else                       
  {
    /* Remove timer from list */
    object_ptr->prev->next  =  object_ptr->next;
    object_ptr->next->prev  =  object_ptr->prev;
    object_ptr->next->count += object_ptr->count;
    object_ptr->next = NULL;
    object_ptr->prev = NULL;
  }
}

/*
 * Handler function called from SysTick event handler.
 */
static void SYSTIMER_lTimerHandler(void)
{
  SYSTIMER_OBJECT_t *object_ptr;
  /* Get first item of timer list */
  object_ptr = g_timer_list;
  while ((NULL != object_ptr) && (0U == object_ptr->count))
  {
    if (true == object_ptr->delete_swtmr)
    {
      /* Yes, remove this timer from timer list */
      SYSTIMER_lRemoveTimerList((uint32_t)object_ptr->id);
      /* Set timer status as SYSTIMER_STATE_NOT_INITIALIZED */
      object_ptr->state = SYSTIMER_STATE_NOT_INITIALIZED;
      /* Release resource which are hold by this timer */
      g_timer_tracker &= ~(1U << object_ptr->id);
    }
    /* Check whether timer is a one shot timer */
    else if (SYSTIMER_MODE_ONE_SHOT == object_ptr->mode)
    {
      if (SYSTIMER_STATE_RUNNING == object_ptr->state)
      {
        /* Yes, remove this timer from timer list */
        SYSTIMER_lRemoveTimerList((uint32_t)object_ptr->id);
        /* Set timer status as SYSTIMER_STATE_STOPPED */
        object_ptr->state = SYSTIMER_STATE_STOPPED;
        /* Call timer callback function */
        (object_ptr->callback)(object_ptr->args);
      }
    }
    /* Check whether timer is periodic timer */
    else if (SYSTIMER_MODE_PERIODIC == object_ptr->mode)
    {
      if (SYSTIMER_STATE_RUNNING == object_ptr->state)
      {
        /* Yes, remove this timer from timer list */
        SYSTIMER_lRemoveTimerList((uint32_t)object_ptr->id);
        /* Reset timer tick */
        object_ptr->count = object_ptr->reload;
        /* Insert timer into timer list */
        SYSTIMER_lInsertTimerList((uint32_t)object_ptr->id);
        /* Call timer callback function */
        (object_ptr->callback)(object_ptr->args);
      }
    }
    else
    {
      break;
    }
    /* Get first item of timer list */
    object_ptr = g_timer_list;
  }
}

/*
 *  SysTick Event Handler.
 */
void SysTick_Handler(void)
{
  SYSTIMER_OBJECT_t *object_ptr;
  object_ptr = g_timer_list;
  g_systick_count++;

  if (NULL != object_ptr)
  {
    if (object_ptr->count > 1UL)
    {
      object_ptr->count--;
    }
    else
    {
      object_ptr->count = 0U;
      SYSTIMER_lTimerHandler();
    }
  }
}

/** @ingroup Simple_System_Timer_App PublicFunc
 * @{
 */

/*
 * Function to retrieve the version of the SYSTIMER APP.
 */
DAVE_APP_VERSION_t SYSTIMER_GetAppVersion()
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t)SYSTIMER_MAJOR_VERSION;
  version.minor = (uint8_t)SYSTIMER_MINOR_VERSION;
  version.patch = (uint8_t)SYSTIMER_PATCH_VERSION;

  return (version);
}

/*
 * Initialization function which initializes the SYSTIMER APP, configures SysTick timer and SysTick exception.
 */
SYSTIMER_STATUS_t SYSTIMER_Init(SYSTIMER_t *handle)
{
  SYSTIMER_STATUS_t status = SYSTIMER_STATUS_SUCCESS;

  XMC_ASSERT("SYSTIMER_Init: SYSTIMER APP handle pointer uninitialized", (handle != NULL));

  /* Check APP initialization status to ensure whether SYSTIMER_Init called or not, initialize SYSTIMER if
   * SYSTIMER_Init called first time.
   */
  if (false == handle->init_status)
  {
    /* Initialize the header of the list */
    g_timer_list = NULL;
    /* Initialize SysTick timer */
    status = (SYSTIMER_STATUS_t)SysTick_Config((uint32_t)(SYSTIMER_SYSTICK_CLOCK * SYSTIMER_TICK_PERIOD));

    if (SYSTIMER_STATUS_FAILURE == status)
    {
      XMC_DEBUG("SYSTIMER_Init: Timer reload value out of range");
    }
    else
    {
#if (UC_FAMILY == XMC4)
      /* setting of First SW Timer period is always and subpriority value for XMC4000 devices */
      NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(
      NVIC_GetPriorityGrouping(), SYSTIMER_PRIORITY, SYSTIMER_SUBPRIORITY));
#elif (UC_FAMILY == XMC1)
      /* setting of priority value for XMC1000 devices */
      NVIC_SetPriority(SysTick_IRQn, SYSTIMER_PRIORITY);
#endif      
      g_timer_tracker = 0U;
      /* Update the Initialization status of the SYSTIMER APP instance */
      handle->init_status = true;
      status = SYSTIMER_STATUS_SUCCESS;
    }
  }

  return (status);
}

/*
 *  API for creating a new software Timer instance.
 */
uint32_t SYSTIMER_CreateTimer
(
  uint32_t period,
  SYSTIMER_MODE_t mode,
  SYSTIMER_CALLBACK_t callback,
  void  *args
)
{
  uint32_t id = 0U;
  uint32_t count = 0U;
  uint32_t period_ratio = 0U;

  XMC_ASSERT("SYSTIMER_CreateTimer: Timer creation failure due to invalid period value",
            ((period >= SYSTIMER_TICK_PERIOD_US) && (period > 0U) && (period <= 0xFFFFFFFFU)));
  XMC_ASSERT("SYSTIMER_CreateTimer: Timer creation failure due to invalid timer mode",
            ((SYSTIMER_MODE_ONE_SHOT == mode) || (SYSTIMER_MODE_PERIODIC == mode)));
  XMC_ASSERT("SYSTIMER_CreateTimer: Can not create software without user callback", (NULL != callback));
  
  if (period < SYSTIMER_TICK_PERIOD_US)
  {
    id = 0U;
  }
  else
  {
    for (count = 0U; count < SYSTIMER_CFG_MAX_TMR; count++)
    {
      /* Check for free timer ID */
      if (0U == (g_timer_tracker & (1U << count)))
      {
        /* If yes, assign ID to this timer */
        g_timer_tracker |= (1U << count);
        /* Initialize the timer as per input values */
        g_timer_tbl[count].id     = count;
        g_timer_tbl[count].mode   = mode;
        g_timer_tbl[count].state  = SYSTIMER_STATE_STOPPED;
        period_ratio = (uint32_t)(period / SYSTIMER_TICK_PERIOD_US);
        g_timer_tbl[count].count  = (period_ratio + HW_TIMER_ADDITIONAL_CNT);
        g_timer_tbl[count].reload  = period_ratio;
        g_timer_tbl[count].callback = callback;
        g_timer_tbl[count].args = args;
        g_timer_tbl[count].prev   = NULL;
        g_timer_tbl[count].next   = NULL;
        id = count + 1U;
        break;
      }
    }

  }

  return (id);
}  

/*
 *  API to start the software timer.
 */
SYSTIMER_STATUS_t SYSTIMER_StartTimer(uint32_t id)
{
  SYSTIMER_STATUS_t status;
  status = SYSTIMER_STATUS_FAILURE;

  XMC_ASSERT("SYSTIMER_StartTimer: Failure in timer restart operation due to invalid timer ID",
            ((id <= SYSTIMER_CFG_MAX_TMR) && (id > 0U)));
  XMC_ASSERT("SYSTIMER_StartTimer: Error during start of software timer", (0U != (g_timer_tracker & (1U << (id - 1U)))));
  
  /* Check if timer is running */
  if (SYSTIMER_STATE_STOPPED == g_timer_tbl[id - 1U].state)
  {
    g_timer_tbl[id - 1U].count = (g_timer_tbl[id - 1U].reload + HW_TIMER_ADDITIONAL_CNT);
    /* set timer status as SYSTIMER_STATE_RUNNING */
    g_timer_tbl[id - 1U].state = SYSTIMER_STATE_RUNNING;
    /* Insert this timer into timer list */
    SYSTIMER_lInsertTimerList((id - 1U));
    status = SYSTIMER_STATUS_SUCCESS;
  }

  return (status);
}

/*
 *  API to stop the software timer.
 */
SYSTIMER_STATUS_t SYSTIMER_StopTimer(uint32_t id)
{
  SYSTIMER_STATUS_t status;
  status = SYSTIMER_STATUS_SUCCESS;

  XMC_ASSERT("SYSTIMER_StopTimer: Failure in timer restart operation due to invalid timer ID",
            ((id <= SYSTIMER_CFG_MAX_TMR) && (id > 0U)));
  XMC_ASSERT("SYSTIMER_StopTimer: Error during stop of software timer", (0U != (g_timer_tracker & (1U << (id - 1U)))));

  if (SYSTIMER_STATE_NOT_INITIALIZED == g_timer_tbl[id - 1U].state)
  {
    status = SYSTIMER_STATUS_FAILURE;
  }
  else
  {
    /* Check whether Timer is in Stop state */
    if (SYSTIMER_STATE_RUNNING == g_timer_tbl[id - 1U].state)
    {
        /* Set timer status as SYSTIMER_STATE_STOPPED */
        g_timer_tbl[id - 1U].state = SYSTIMER_STATE_STOPPED;

        /* remove Timer from node list */
        SYSTIMER_lRemoveTimerList(id - 1U);

    }
  }

  return (status);
}

/*
 *  API to reinitialize the time interval and to start the timer.
 */
SYSTIMER_STATUS_t SYSTIMER_RestartTimer(uint32_t id, uint32_t microsec)
{
  uint32_t period_ratio = 0U;
  SYSTIMER_STATUS_t status;
  status = SYSTIMER_STATUS_SUCCESS;

  XMC_ASSERT("SYSTIMER_RestartTimer: Failure in timer restart operation due to invalid timer ID",
            ((id <= SYSTIMER_CFG_MAX_TMR) && (id > 0U)));
  XMC_ASSERT("SYSTIMER_RestartTimer: Error during restart of software timer", (0U != (g_timer_tracker & (1U << (id - 1U)))));
  XMC_ASSERT("SYSTIMER_RestartTimer: Can not restart timer due to invalid period value",
            (microsec >= SYSTIMER_TICK_PERIOD_US) && (microsec > 0U));


  if (SYSTIMER_STATE_NOT_INITIALIZED == g_timer_tbl[id - 1U].state)
  {
      status = SYSTIMER_STATUS_FAILURE;
  }
  else
  {
    /* check whether timer is in run state */
    if( SYSTIMER_STATE_STOPPED != g_timer_tbl[id - 1U].state)
    {
         /* Stop the timer */
         status = SYSTIMER_StopTimer(id);
    }
    if (SYSTIMER_STATUS_SUCCESS == status)
    {
      period_ratio = (uint32_t)(microsec / SYSTIMER_TICK_PERIOD_US);
      g_timer_tbl[id - 1U].reload = period_ratio;
      /* Start the timer */
      status = SYSTIMER_StartTimer(id);
    }
  }

  return (status);
}

/*
 *  Function to delete the Timer instance.
 */
SYSTIMER_STATUS_t SYSTIMER_DeleteTimer(uint32_t id)
{
  SYSTIMER_STATUS_t status;
  status = SYSTIMER_STATUS_SUCCESS;

  XMC_ASSERT("SYSTIMER_DeleteTimer: Failure in timer restart operation due to invalid timer ID",
            ((id <= SYSTIMER_CFG_MAX_TMR) && (id > 0U)));
  XMC_ASSERT("SYSTIMER_DeleteTimer: Error during deletion of software timer", (0U != (g_timer_tracker & (1U << (id - 1U)))));

  /* Check whether Timer is in delete state */
  if (SYSTIMER_STATE_NOT_INITIALIZED == g_timer_tbl[id - 1U].state)
  {
      status = SYSTIMER_STATUS_FAILURE;
  }
  else
  {
    if (SYSTIMER_STATE_STOPPED == g_timer_tbl[id - 1U].state)
    {
      /* Set timer status as SYSTIMER_STATE_NOT_INITIALIZED */
      g_timer_tbl[id - 1U].state = SYSTIMER_STATE_NOT_INITIALIZED;
      /* Release resource which are hold by this timer */
      g_timer_tracker &= ~(1U << (id - 1U));
    }
    else
    {
      /* Yes, remove this timer from timer list during ISR execution */
      g_timer_tbl[id - 1U].delete_swtmr = true;
    }
  }

  return (status);
}

/*
 *  API to get the current SysTick time in microsecond.
 */
uint32_t SYSTIMER_GetTime(void)
{
  return (g_systick_count * SYSTIMER_TICK_PERIOD_US);
}

/*
 *  API to get the SysTick count.
 */
uint32_t SYSTIMER_GetTickCount(void)
{
  return (g_systick_count);
}

/*
 *  API to get the current state of software timer.
 */
SYSTIMER_STATE_t SYSTIMER_GetTimerState(uint32_t id)
{
  return (g_timer_tbl[id - 1U].state);
}

__attribute__((always_inline)) __STATIC_INLINE uint32_t critical_section_enter(void)
{
  uint32_t status;
  status = __get_PRIMASK();
  __disable_irq ();
  return status;
}

__attribute__((always_inline)) __STATIC_INLINE void critical_section_exit(uint32_t status)
{
  __set_PRIMASK(status);
}

void SYSTIMER_Start(void)
{
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SYSTIMER_Stop(void)
{
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

uint32_t SYSTIMER_CreateTimerFromISR
(
  uint32_t period,
  SYSTIMER_MODE_t mode,
  SYSTIMER_CALLBACK_t callback,
  void  *args
)
{
  uint32_t id;

  uint32_t ics;
  ics = critical_section_enter();

  id = SYSTIMER_CreateTimer(period, mode, callback, args);

  critical_section_exit(ics);

  return (id);
}

SYSTIMER_STATUS_t SYSTIMER_StartTimerFromISR(uint32_t id)
{
  SYSTIMER_STATUS_t status;

  uint32_t ics;
  ics = critical_section_enter();

  status = SYSTIMER_StartTimer(id);

  critical_section_exit(ics);

  return (status);
}

SYSTIMER_STATUS_t SYSTIMER_StopTimerFromISR(uint32_t id)
{
  SYSTIMER_STATUS_t status;

  uint32_t ics;
  ics = critical_section_enter();

  status = SYSTIMER_StopTimer(id);

  critical_section_exit(ics);

  return (status);
}

SYSTIMER_STATUS_t SYSTIMER_RestartTimerFromISR(uint32_t id, uint32_t microsec)
{
  SYSTIMER_STATUS_t status;

  uint32_t ics;
  ics = critical_section_enter();

  status = SYSTIMER_RestartTimer(id, microsec);

  critical_section_exit(ics);

  return (status);
}

SYSTIMER_STATUS_t SYSTIMER_DeleteTimerFromISR(uint32_t id)
{
  SYSTIMER_STATUS_t status;

  uint32_t ics;
  ics = critical_section_enter();

  status = SYSTIMER_DeleteTimer(id);

  critical_section_exit(ics);

  return (status);
}
