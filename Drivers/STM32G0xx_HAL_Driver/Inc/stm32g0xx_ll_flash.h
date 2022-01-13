/**
  ******************************************************************************
  * @file    stm32g0xx_ll_flash.h
  * @brief   Header file of FLASH LL module.--->STM32G0xx
	* @Version  : 1.2
	* @Date     : 2018-07-15
	* @Author   : GHG
	* ---------------------------------- Revision History: -------------------------
	* <version>           <date>          < author >              <desc>
	* Revision 1.0        2018-07-07      xxxGHGxxxx              MDK-ARM 5.21a
	*
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32G0xx_LL_FLASH_H
#define __STM32G0xx_LL_FLASH_H


/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"

/** @addtogroup STM32G0xx_LL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/** 
  * @brief  LL Status structures definition  
  */  
typedef enum 
{
  LL_OK       = 0x00U,
  LL_ERROR    = 0x01U,
  LL_BUSY     = 0x02U,
  LL_TIMEOUT  = 0x03U
} LL_StatusTypeDef;

/** 
  * @brief  LL Lock structures definition  
  */
typedef enum 
{
  LL_UNLOCKED = 0x00U,
  LL_LOCKED   = 0x01U  
} LL_LockTypeDef;

/**
  * @brief  FLASH Procedure structure definition
  */
typedef enum 
{
  FLASH_PROC_NONE              = 0, 
  FLASH_PROC_PAGEERASE         = 1,
  FLASH_PROC_PROGRAM           = 2,
} FLASH_ProcedureTypeDef;
/**
  * @}
  */


#define LL_MAX_DELAY      0xFFFFFFFFU

#define __LL_LOCK(__HANDLE__)                                                  \
																	do{                                          \
																			if((__HANDLE__)->Lock == LL_LOCKED)      \
																			{                                        \
																				 return LL_BUSY;                       \
																			}                                        \
																			else                                     \
																			{                                        \
																				 (__HANDLE__)->Lock = LL_LOCKED;       \
																			}                                        \
																		}while (0U)

#define __LL_UNLOCK(__HANDLE__)                                                \
                                  do{                                          \
                                      (__HANDLE__)->Lock = LL_UNLOCKED;        \
                                    }while (0U)


																															
/** @defgroup FLASH_Type_Erase FLASH Erase Type
  * @{
  */
#define FLASH_TYPEERASE_PAGES           FLASH_CR_PER    /*!< Pages erase only */
#define FLASH_TYPEERASE_MASS            FLASH_CR_MER1   /*!< Flash mass erase activation */
/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Program Type
  * @{
  */
#define FLASH_TYPEPROGRAM_DOUBLEWORD    FLASH_CR_PG     /*!< Program a double-word (64-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_FAST          FLASH_CR_FSTPG  /*!< Fast program a 32 row double-word (64-bit) at a specified address.
                                                             And another 32 row double-word (64-bit) will be programmed */
/**
  * @}
  */


/**
* @brief  FLASH handle Structure definition
*/
typedef struct
{
  LL_LockTypeDef    Lock;              /* FLASH locking object */
  uint32_t          ErrorCode;         /* FLASH error code */
  uint32_t          ProcedureOnGoing;  /* Internal variable to indicate which procedure is ongoing or not in IT context */
  uint32_t          Address;           /* Internal variable to save address selected for program in IT context */
  uint32_t          Page;              /* Internal variable to define the current page which is erasing in IT context */
  uint32_t          NbPagesToErase;    /* Internal variable to save the remaining pages to erase in IT context */
} FLASH_ProcessTypeDef;

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< Mass erase or page erase.
                             This parameter can be a value of @ref FLASH_Type_Erase */
  uint32_t Page;        /*!< Initial Flash page to erase when page erase is enabled
                             This parameter must be a value between 0 and (max number of pages - 1) */
  uint32_t NbPages;     /*!< Number of pages to be erased.
                             This parameter must be a value between 1 and (max number of pages - value of initial page)*/
} FLASH_EraseInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */  

/** @defgroup FLASH_Error FLASH Error
  * @{
  */
#define LL_FLASH_ERROR_NONE            0x00000000u
#define LL_FLASH_ERROR_OP              FLASH_FLAG_OPERR
#define LL_FLASH_ERROR_PROG            FLASH_FLAG_PROGERR
#define LL_FLASH_ERROR_WRP             FLASH_FLAG_WRPERR
#define LL_FLASH_ERROR_PGA             FLASH_FLAG_PGAERR
#define LL_FLASH_ERROR_SIZ             FLASH_FLAG_SIZERR
#define LL_FLASH_ERROR_PGS             FLASH_FLAG_PGSERR
#define LL_FLASH_ERROR_MIS             FLASH_FLAG_MISERR
#define LL_FLASH_ERROR_FAST            FLASH_FLAG_FASTERR
#if defined(FLASH_PCROP_SUPPORT)
#define FLASH_FLAG_RDERR               FLASH_SR_RDERR    /*!< FLASH PCROP read error flag */
#endif /* FLASH_PCROP_SUPPORT */
#define LL_FLASH_ERROR_OPTV            FLASH_FLAG_OPTVERR
#define LL_FLASH_ERROR_ECCD            FLASH_FLAG_ECCD
/**
  * @}
  */

/** @defgroup FLASH_Flags FLASH Flags Definition
  * @{
  */
#define FLASH_FLAG_EOP                  FLASH_SR_EOP      /*!< FLASH End of operation flag */
#define FLASH_FLAG_OPERR                FLASH_SR_OPERR    /*!< FLASH Operation error flag */
#define FLASH_FLAG_PROGERR              FLASH_SR_PROGERR  /*!< FLASH Programming error flag */
#define FLASH_FLAG_WRPERR               FLASH_SR_WRPERR   /*!< FLASH Write protection error flag */
#define FLASH_FLAG_PGAERR               FLASH_SR_PGAERR   /*!< FLASH Programming alignment error flag */
#define FLASH_FLAG_SIZERR               FLASH_SR_SIZERR   /*!< FLASH Size error flag  */
#define FLASH_FLAG_PGSERR               FLASH_SR_PGSERR   /*!< FLASH Programming sequence error flag */
#define FLASH_FLAG_MISERR               FLASH_SR_MISERR   /*!< FLASH Fast programming data miss error flag */
#define FLASH_FLAG_FASTERR              FLASH_SR_FASTERR  /*!< FLASH Fast programming error flag */
#if defined(FLASH_PCROP_SUPPORT)
#define HAL_FLASH_ERROR_RD              FLASH_FLAG_RDERR
#endif
#define FLASH_FLAG_OPTVERR              FLASH_SR_OPTVERR  /*!< FLASH Option validity error flag */
#define FLASH_FLAG_BSY                  FLASH_SR_BSY1     /*!< FLASH Operation Busy flag */
#define FLASH_FLAG_CFGBSY               FLASH_SR_CFGBSY   /*!< FLASH Configuration Busy flag */
#define FLASH_FLAG_ECCC                 FLASH_ECCR_ECCC   /*!< FLASH ECC correction */
#define FLASH_FLAG_ECCD                 FLASH_ECCR_ECCD   /*!< FLASH ECC detection */
/**
  * @}
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup FLASH_Private_Constants FLASH Private Constants
  * @{
  */
#define FLASH_SIZE_DATA_REGISTER        FLASHSIZE_BASE

#define FLASH_SIZE                      (((*((uint32_t *)FLASH_SIZE_DATA_REGISTER)) & (0x00FFu)) << 10u)

#define FLASH_BANK_SIZE                 (FLASH_SIZE)

#define FLASH_PAGE_SIZE                 0x800u

#define FLASH_PAGE_NB                   64u

#define FLASH_TIMEOUT_VALUE             2000u /* 1 s */

#define FLASH_TYPENONE                  0x00u

#if defined(FLASH_PCROP_SUPPORT)
#define FLASH_FLAG_SR_ERROR             (FLASH_FLAG_OPERR  | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR |  \
                                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR |   \
                                         FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR | FLASH_FLAG_RDERR |   \
                                         FLASH_FLAG_OPTVERR)     /*!< All SR error flags */
#else
#define FLASH_FLAG_SR_ERROR             (FLASH_FLAG_OPERR  | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR |  \
                                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR |   \
                                         FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR |   \
                                         FLASH_FLAG_OPTVERR)     /*!< All SR error flags */
#endif
/**
  * @}
  */



/** @defgroup FLASH_Keys FLASH Keys 
  * @{
  */
#define FLASH_KEY1                      0x45670123u   /*!< Flash key1 */
#define FLASH_KEY2                      0xCDEF89ABu   /*!< Flash key2: used with FLASH_KEY1
                                                           to unlock the FLASH registers access */
/**
  * @}
  */

/* CMSIS_Legacy */ 


/**
  * @brief  Enable the specified FLASH interrupt.
  * @param  __INTERRUPT__ FLASH interrupt
  *         This parameter can be any combination of the following values:
  *     @arg FLASH_IT_EOP: End of FLASH Operation Interrupt
  *     @arg FLASH_IT_OPERR: Error Interrupt
  *     @arg FLASH_IT_RDERR: PCROP Read Error Interrupt(*)
  *     @arg FLASH_IT_ECCC: ECC Correction Interrupt
  * @note (*) availability depends on devices
  * @retval none
  */
#define __LL_FLASH_ENABLE_IT(__INTERRUPT__)    do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { SET_BIT(FLASH->ECCR, FLASH_ECCR_ECCCIE); }\
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { SET_BIT(FLASH->CR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }\
                                                   } while(0U)

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param  __INTERRUPT__ FLASH interrupt
  *         This parameter can be any combination of the following values:
  *     @arg FLASH_IT_EOP: End of FLASH Operation Interrupt
  *     @arg FLASH_IT_OPERR: Error Interrupt
  *     @arg FLASH_IT_RDERR: PCROP Read Error Interrupt(*)
  *     @arg FLASH_IT_ECCC: ECC Correction Interrupt
  * @note (*) availability depends on devices
  * @retval none
  */
#define __LL_FLASH_DISABLE_IT(__INTERRUPT__)   do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { CLEAR_BIT(FLASH->ECCR, FLASH_ECCR_ECCCIE); }\
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { CLEAR_BIT(FLASH->CR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }\
                                                   } while(0U)

/**
  * @brief  Check whether the specified FLASH flag is set or not.
  * @param  __FLAG__ specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_OPERR: FLASH Operation error flag
  *     @arg FLASH_FLAG_PROGERR: FLASH Programming error flag
  *     @arg FLASH_FLAG_WRPERR: FLASH Write protection error flag
  *     @arg FLASH_FLAG_PGAERR: FLASH Programming alignment error flag
  *     @arg FLASH_FLAG_SIZERR: FLASH Size error flag
  *     @arg FLASH_FLAG_PGSERR: FLASH Programming sequence error flag
  *     @arg FLASH_FLAG_MISERR: FLASH Fast programming data miss error flag
  *     @arg FLASH_FLAG_FASTERR: FLASH Fast programming error flag
  *     @arg FLASH_FLAG_RDERR: FLASH PCROP read  error flag(*)
  *     @arg FLASH_FLAG_OPTVERR: FLASH Option validity error flag
  *     @arg FLASH_FLAG_BSY: FLASH write/erase operations in progress flag
  *     @arg FLASH_FLAG_CFGBSY: FLASH configuration is busy : program or erase setting are used.
  *     @arg FLASH_FLAG_ECCC: FLASH one ECC error has been detected and corrected
  *     @arg FLASH_FLAG_ECCD: FLASH two ECC errors have been detected
  * @note (*) availability depends on devices
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
#define __LL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)) != 0U) ? \
                                                 (READ_BIT(FLASH->ECCR, (__FLAG__)) == (__FLAG__))  : \
                                                 (READ_BIT(FLASH->SR,   (__FLAG__)) == (__FLAG__)))

/**
  * @brief  Clear the FLASH's pending flags.
  * @param  __FLAG__ specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_OPERR: FLASH Operation error flag
  *     @arg FLASH_FLAG_PROGERR: FLASH Programming error flag
  *     @arg FLASH_FLAG_WRPERR: FLASH Write protection error flag
  *     @arg FLASH_FLAG_PGAERR: FLASH Programming alignment error flag
  *     @arg FLASH_FLAG_SIZERR: FLASH Size error flag
  *     @arg FLASH_FLAG_PGSERR: FLASH Programming sequence error flag
  *     @arg FLASH_FLAG_MISERR: FLASH Fast programming data miss error flag
  *     @arg FLASH_FLAG_FASTERR: FLASH Fast programming error flag
  *     @arg FLASH_FLAG_RDERR: FLASH PCROP read  error flag
  *     @arg FLASH_FLAG_OPTVERR: FLASH Option validity error flag
  *     @arg FLASH_FLAG_ECCC: FLASH one ECC error has been detected and corrected
  *     @arg FLASH_FLAG_ECCD: FLASH two ECC errors have been detected
  *     @arg FLASH_FLAG_ALL_ERRORS: FLASH All errors flags
  * @retval None
  */
#define __LL_FLASH_CLEAR_FLAG(__FLAG__)        do { if(((__FLAG__) & (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)) != 0U) { SET_BIT(FLASH->ECCR, ((__FLAG__) & (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD))); }\
                                                     if(((__FLAG__) & ~(FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)) != 0U) { WRITE_REG(FLASH->SR, ((__FLAG__) & ~(FLASH_FLAG_ECCC | FLASH_FLAG_ECCD))); }\
                                                   } while(0U)

/**
  * @}
  */


/** @addtogroup FLASH_Exported_Functions_Group1
  * @{
  */
/* IO operation functions *****************************************************/
LL_StatusTypeDef LL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
LL_StatusTypeDef LL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);

/* FLASH IRQ handler function */

/* Callbacks in non blocking modes */ 


/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions ***********************************************/
LL_StatusTypeDef LL_FLASH_Unlock(void);
LL_StatusTypeDef LL_FLASH_Lock(void);
void FLASH_PageErase(uint32_t PageAddress);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/

/**
  * @}
  */

/**
  * @}
  */

/* Private function -------------------------------------------------*/
/** @addtogroup FLASH_Private_Functions
 * @{
 */
LL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout);

/**
  * @}
  */


#endif /* __STM32G0xx_LL_FLASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
