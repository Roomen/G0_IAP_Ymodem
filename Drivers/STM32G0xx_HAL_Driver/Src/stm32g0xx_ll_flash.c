/**
  ******************************************************************************
  * @file    stm32l0xx_ll_flash.c
  * @brief   FLASH LL module driver.--->STM32G0xx
	* @Version  : 1.2
	* @Date     : 2018-07-15
	* @Author   : GHG
	* ---------------------------------- Revision History: -------------------------
	* <version>           <date>          < author >              <desc>
	* Revision 1.0        2018-07-07      xxxGHGxxxx              MDK-ARM 5.21a
	* 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_flash.h"
//#include "stm32g0xx_ll_bus.h"


/** @addtogroup STM32L0xx_LL_Driver
  * @{
  */


/* Private variables ---------------------------------------------------------*/
/** @defgroup FLASH_Private_Variables FLASH Private Variables
  * @{
  */
/* Variables used for Erase pages under interruption*/
//FLASH_ProcessTypeDef pFlash;
FLASH_ProcessTypeDef pFlash  = {.Lock = LL_UNLOCKED, \
                                .ErrorCode = LL_FLASH_ERROR_NONE, \
                                .ProcedureOnGoing = FLASH_TYPENONE, \
                                .Address = 0U, \
                                .Page = 0U, \
                                .NbPagesToErase = 0U};
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup FLASH_Private_Functions FLASH Private Functions
  * @{
  */
static void FLASH_MassErase(void);

static void FLASH_Program_Fast(uint32_t Address, uint32_t DataAddress);
static void FLASH_Program_DoubleWord(uint32_t Address, uint64_t Data);


/**
  * @brief  Program double word or fast program of a row at a specified address.
  * @param  TypeProgram Indicate the way to program at a specified address.
  *                      This parameter can be a value of @ref FLASH_Type_Program
  * @param  Address specifies the address to be programmed.
  * @param  Data specifies the data to be programmed
  *               This parameter is the data for the double word program and the address where
  *               are stored the data for the row fast program
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
LL_StatusTypeDef LL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
	LL_StatusTypeDef status;
	
//	/* Check the parameters */
//  assert_param(IS_FLASH_TYPEPROGRAM(TypeProgram));
	
	/* Process Locked */
	__LL_LOCK(&pFlash);
	
	/* Reset error code */
  pFlash.ErrorCode = LL_FLASH_ERROR_NONE;
	
	/* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
	
	if(status == LL_OK)
  {
    if(TypeProgram == FLASH_TYPEPROGRAM_DOUBLEWORD)
    {
//      /* Check the parameters */
//      assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));

      /* Program double-word (64-bit) at a specified address */
      FLASH_Program_DoubleWord(Address, Data);
    }
    else
    {
//      /* Check the parameters */
//      assert_param(IS_FLASH_FAST_PROGRAM_ADDRESS(Address));

      /* Fast program a 32 row double-word (64-bit) at a specified address */
      FLASH_Program_Fast(Address, (uint32_t)Data);
    }

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

    /* If the program operation is completed, disable the PG or FSTPG Bit */
		CLEAR_BIT(FLASH->CR, TypeProgram);
	}

  /* Process Unlocked */
  __LL_UNLOCK(&pFlash);
	
	return status;
}

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory pages.
  * @param[in]  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * @param[out]  PageError   pointer to variable that contains the configuration
  *         information on faulty page in case of error (0xFFFFFFFF means that all
  *         the pages have been correctly erased)
  * @retval HAL Status
  */
LL_StatusTypeDef LL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError)
{
  LL_StatusTypeDef status;
  uint32_t index;
  
//	/* Check the parameters */
//  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));
	
  /* Process Locked */
  __LL_LOCK(&pFlash);
	
	/* Reset error code */
  pFlash.ErrorCode = LL_FLASH_ERROR_NONE;
	
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == LL_OK)
  {
		if (pEraseInit->TypeErase == FLASH_TYPEERASE_MASS)
    {
      /* Mass erase to be done */
      FLASH_MassErase();
			
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    }
		else
    {
      /*Initialization of PageError variable*/
      *PageError = 0xFFFFFFFFu;
			
      for (index = pEraseInit->Page; index < (pEraseInit->Page + pEraseInit->NbPages); index++)
      {
        /* Start erase page */
        FLASH_PageErase(index);
				
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
				
        if (status != LL_OK)
        {
          /* In case of error, stop erase procedure and return the faulty address */
          *PageError = index;
          break;
        }
      }
			
      /* If operation is completed or interrupted, disable the Page Erase Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
    }
  }
	
  /* Process Unlocked */
  __LL_UNLOCK(&pFlash);
	
  return status;
}

/**
  * @brief  Unlock the FLASH control register access.
  * @retval HAL Status
  */
LL_StatusTypeDef LL_FLASH_Unlock(void)
{
	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
	{
		/* Authorize the FLASH Registers access */
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);
		
//		/* verify Flash is unlock */
//    if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00u)
//    {
//      return LL_ERROR;
//    }
	}
	else
  {
    return LL_ERROR;
  }
	
	return LL_OK; 
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval HAL Status
  */
LL_StatusTypeDef LL_FLASH_Lock(void)
{
	/* Set the LOCK Bit to lock the FLASH Registers access */
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);
	
//	/* verify Flash is lock */
//  if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00u)
//  {
//    return LL_OK;
//  }
  
  return LL_OK;  
}

/**
  * @brief  Erase the specified FLASH memory page.
  * @param  Page FLASH page to erase
  *         This parameter must be a value between 0 and (max number of pages in Flash - 1)
  * @retval None
  */
void FLASH_PageErase(uint32_t Page)
{
  uint32_t tmp;

  /* Get configuration register, then clear page number */
  tmp = (FLASH->CR & ~FLASH_CR_PNB);

  /* Set page number, Page Erase bit & Start bit */
  FLASH->CR = (tmp | (FLASH_CR_STRT | (Page <<  FLASH_CR_PNB_Pos) | FLASH_CR_PER));
}

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout
  * @retval HAL_StatusTypeDef HAL Status
  */
LL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout)
{
	__IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
	((void)tmp);
	
  uint32_t error;
  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */
     
//  uint32_t tickstart = HAL_GetTick();
     
  while(__LL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0x00u) 
  {
    if (Timeout != LL_MAX_DELAY)
    {
			if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
			{
				Timeout--;
				
				if(Timeout == 0U)//if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
				{
					return LL_TIMEOUT;
				}
			}
    }
  }
	
	/* check flash errors. Only ECC correction can be checked here as ECCD
      generates NMI */
  error = (FLASH->SR & FLASH_FLAG_SR_ERROR);
  error |= (FLASH->ECCR & FLASH_FLAG_ECCC);
  if (error != 0x00u)
  {
    /*Save the error code*/
    pFlash.ErrorCode |= error;

    /* clear error flags */
    FLASH->SR = FLASH_FLAG_SR_ERROR;
    FLASH->ECCR |= FLASH_FLAG_ECCC;

    return LL_ERROR;
  }
	
	 /* Wait for control register to be written */
//  timeout = HAL_GetTick() + Timeout;
	
	while(__LL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0x00u) 
  {
    if (Timeout != LL_MAX_DELAY)
    {
			if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
			{
				Timeout--;
				
				if(Timeout == 0U)//if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
				{
					return LL_TIMEOUT;
				}
			}
    }
  }

  /* There is no error flag set */
  return LL_OK;
}

/**
  * @brief  Mass erase of FLASH memory.
  * @retval None
  */
static void FLASH_MassErase(void)
{
  /* Set the Mass Erase Bit, then Start bit */
  FLASH->CR |= (FLASH_CR_STRT | FLASH_CR_MER1);

}

/**
  * @brief  Program double-word (64-bit) at a specified address.
  * @param  Address specifies the address to be programmed.
  * @param  Data specifies the data to be programmed.
  * @retval None
  */
static void FLASH_Program_DoubleWord(uint32_t Address, uint64_t Data)
{
  /* Set PG bit */
  SET_BIT(FLASH->CR, FLASH_CR_PG);

  /* Program the double word */
  *(uint32_t *)Address = (uint32_t)Data;
  *(uint32_t *)(Address + 4u) = (uint32_t)(Data >> 32u);
}

/**
  * @brief  Fast program a 32 row double-word (64-bit) at a specified address.
  * @param  Address specifies the address to be programmed.
  * @param  DataAddress specifies the address where the data are stored.
  * @retval None
  */
static void FLASH_Program_Fast(uint32_t Address, uint32_t DataAddress)
{
  uint8_t index = 0;
  uint32_t dest = Address;
  uint32_t src = DataAddress;
  uint32_t primask_bit;

  /* Set FSTPG bit */
  SET_BIT(FLASH->CR, FLASH_CR_FSTPG);

  /* Enter critical section: row programming should not be longer than 7 ms */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Fast Program : 64 words */
  while (index < 64u)
  {
    *(uint32_t *)dest = *(uint32_t *)src;
    src += 4u;
    dest += 4u;
    index++;
  }

  /* wait for BSY1 in order to be sure that flash operation is ended befoire
     allowing prefetch in flash. Timeout does not return status, as it will
     be anyway done later */
  while((FLASH->SR & FLASH_SR_BSY1) != 0x00u)
  {
  }

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}


/**
  * @}
  */

/**
  * @}
  */


  /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
