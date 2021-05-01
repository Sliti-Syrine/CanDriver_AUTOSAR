#include "CanDriver.h"
#define  NULL_PTR  ((void *)0)
#define MODE_CHANGED             1U
#define MODE_NOT_CHANGED         0U



void Can_Init(const Can_ConfigType* config)
 {
     const Can_IdType ID;
     const Controller_Type* controller;
     const Can_HwType* canHwConfig;
     const Can_HwHandleType Hoh;
     Can_DriverStateType Driver_state;
     uint8_t ctrl_Id;
     uint8_t Ch_Id;


     if ( config == NULL_PTR )
     {
         Det_Report_Error(CAN_E_PARAM_POINTER);
         break;
     }
     //driver state
     if ( Driver_state == CAN_UNINIT )
     {
         Det_ReportError(CAN_E_TRANSITION);
         break;
     }
     Driver_state = CAN_READY;

     //CAN controller specific settings
     controller ->state = CANIF_CS_STOPPED;
     controller ->ControllerId = ctrl_Id;
     controller ->Channel_Id = Ch_Id;

     //settings for the CAN HW unit
     canHwConfig ->CanIdID = ID ;
     canHwConfig ->CAN_Hoh = Hoh;

     //register settings
     CAN_CTRL_BASE_ADR -> CAN_MCR |= 0x1; //sets the INRQ bit in the CAN_MCR
     CAN_CTRL_BASE_ADR->  CAN_MSR |=0x1;  //sets the INAK bit in the CAN_MSR

     /*------------------BIT TIMIMG-------------------*/

     CAN_CTRL_BASE_ADR-> CAN_FMR |= 0x1; //set the FINIT Filter init mode

 }


 /*-------------------------Can_SetBaudrate-----------------------*/

 Std_ReturnType Can_SetBaudrate(uint8_t Controller, uint16_t BaudRateConfigID)
{
	uint8_t         Can_DevErrorType;
	uint8_t         ErrorStatus = E_OK;
	Can_DriverStateType Driver_state;
	tCANBitClkParms      Bit_Time_Parameters;
	Std_ReturnType Status;

	if (Driver_state != CAN_READY) //the driver is not yet initialized
	{
		Det_Report_Error(CAN_E_UNINIT);
		return E_NOT_OK;
	}

	if (Controller >= 1) //the parameter Controller is out of range
	{
	    Det_Report_Error(CAN_E_PARAM_CONTROLLER);
		return E_NOT_OK;
	}

	if (BaudRateConfigID >= MAX_BAUDRATES) //BaudRateConfigID has an invalid value
	{
		Det_Report_Error(CAN_E_PARAM_BAUDRATE);
		return E_NOT_OK;
	}

	if (state == E_OK)

	{
	    /*------------------BIT TIMIMG-------------------*/

		if (Can_GetControllerMode(Controller) == CAN_CS_STOPPED || Can_GetControllerMode(Controller) == CAN_CS_SLEEP)
		{
			CANDisable(RCC_APB1_ENABLE);
		}

		ErrorStatus = E_OK;
	}
	else
	{
		ErrorStatus = E_NOT_OK;
	}
	return ErrorStatus;
}


/*-----------------------------Can_GetControllerMode-----------------------*/


Std_ReturnType Can_GetControllerMode(uint8_t Controller, Can_ControllerStateType* ControllerModePtr)
{
	Std_ReturnType Status ;
	Can_DriverStateType Driver_state;
	uint8_t CanDevolpmentErrorType;

	/*  validating inputs  */
	if (Controller ==1)
	{
		if (ControllerModePtr != NULL_PTR)
		{
			/*  check that the can has been initialized before  */
			if (Driver_state == CAN_READY)
			{
				*ControllerModePtr = Can_GetControllerMode(Controller);
			}
			else
			{
			    Det_Report_Error(CAN_E_UNINIT); //the driver is not yet initialized
			    Status = E_NOT_OK;
			}
		}
		else
		{
		    Det_Report_Error(CAN_E_PARAM_POINTER);
			Status = E_NOT_OK;
		}
	}
	else
	{
	    Det_Report_Error(CAN_E_PARAM_CONTROLLER);
		Status = E_NOT_OK;
	}

	return Status;
}


/*-----------------------------Can_SetControllerMode-----------------------*/

Std_ReturnType Can_SetControllerMode(uint8_t Controller, Can_StateTransitionType Transition)
{
	uint8_t        HwObject;
	uint8_t        Can_DevErrorType;
	Std_ReturnType  Status;
	Can_DriverStateType Driver_state;
	uint8_t state_transition_flag[2] = { MODE_NOT_CHANGED, MODE_NOT_CHANGED }


	/*  Check for the initialization of the Driver   */
	if (Driver_state == CAN_UNINIT)
	{
		if (Controller !=1)
		{
			/*   [SWS_Can_00409]  When the function Can_SetControllerMode (CAN_CS_STARTED) is entered and the
			CAN controller is not in state STOPPED it shall detect a invalid state transition*/
			if (((Transition == CAN_CS_STARTED) && (Can_GetControllerMode(Controller) == CAN_CS_STOPPED)) || ((Transition == CAN_CS_SLEEP) && (Can_GETControllerMode(Controller) == CAN_CS_STOPPED)))
			{
				state_transition_flag = MODE_CHANGED;

				switch (Transition)
				{

				case CAN_CS_STARTED:
					/*[SWS_Can_00196]The function Can_SetControllerMode shall enable interrupts that are needed in the new state.*/

					/* [SWS_Can_00261] The function Can_SetControllerMode(CAN_CS_STARTED) shall set the hardware registers in a way
					that makes the CAN controller participating on the network.*/

					/*[SWS_Can_00204] The Can module shall track all individual enabling and disabling of interrupts in other functions (i.e. Can_SetControllerMode)
					, so that the correct interrupt enable state can be restored.*/

					/* [SWS_Can_00425] Enabling of CAN interrupts shall not be executed, when CAN interrupts have been disabled by function Can_DisableControllerInterrupts.*/
					if (OUT_CRITICAL_SECTION() && InterruptStatus[Controller] != INTERRUPT_ENABLE)
					{

						CANEnable(CanController[Controller].CanControllerBaseAddress);
						InterruptStatus[Controller] = INTERRUPT_ENABLE;
					}
					/* if the interrupt has been enabled before hand */
					else
					{
						CANEnable(CanControllerBaseAddress);

						/*  Changing the controller mode  */
						Can_ControllerMode = CAN_CS_STARTED;
						break;

				case CAN_CS_STOPPED:

					/* if the interrupt has been disabled before hand */
					if (IN_CRITICAL_SECTION())
					{
						CANDisable(CanControllerBaseAddress);
					}
					/*  if this is the first time to disabled the interrupt  */
					else
					{
						InterruptStatus[Controller] = INTERRUPT_DISABLE;
						CANDisable(CanControllerBaseAddress);

					}

					Can_ControllerMode = CAN_CS_STOPPED;
					break;



					/*[SWS_Can_00258] When the CAN hardware does not support sleep mode and is triggered to transition into SLEEP state,
					the Can module shall emulate a logical SLEEP state from which it returns only, when it is triggered by software to transition into STOPPED state.*/
					/*[SWS_Can_00404] The CAN hardware shall remain in state STOPPED, while the logical SLEEP state is active.*/
					/*[SWS_Can_00290] If the CAN HW does not support a sleep mode,
					the function Can_SetControllerMode(CAN_CS_SLEEP) shall set the CAN controller to the logical sleep mode.*/
					/* [SWS_Can_00197] The function Can_SetControllerMode shall disable interrupts that are not allowed in the new state. */
				case CAN_CS_SLEEP:
					/* if the interrupt has been disabled before hand */
					if (IN_CRITICAL_SECTION())
					{

						CANDisable(CanControllerBaseAddress);

					}
					/*  if this is the first time to disabled the interrupt  */
					else
					{
						InterruptStatus = INTERRUPT_DISABLE;
						CANDisable(CanControllerBaseAddress);

					}



					Can_ControllerMode = CAN_CS_SLEEP;
					break;
				default:
					break;
					}
				}
				/*   if the input parameter Controller is not valid, then change the error type and set the status to E_NOT_OK that indicate that the operation did not take place   */
			else
			{
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
				CanDevolpmentErrorType = CAN_E_TRANSITION;
				Det_ReportError(CAN_MODULE_ID, Controller, CAN_SET_CONTROLLER_MODE_SID, CanDevolpmentErrorType);
#endif

				Status = E_NOT_OK;

			}
			}
			/*   if the input parameter Controller is not valid, then change the error type and set the status to E_NOT_OK that indicate that the operation did not take place   */
			else
			{
				Status = E_NOT_OK;

#if CAN_DEV_ERROR_DETECT == E_NOT_OK
				Can_DevErrorType = 0x04 //CAN_E_PARAM_CONTROLLER;

#endif

			}
		}


		else
		{

#if CAN_DEV_ERROR_DETECT == E_NOT_OK
			CanDevolpmentErrorType = 0x04 //CAN_E_PARAM_CONTROLLER;

#endif
				Status = E_NOT_OK;
		}

		/*[SWS_Can_00262] The function Can_SetControllerMode(CAN_CS_STARTED) shall wait for
		limited time until the CAN controller is fully operational. Compare to SWS_Can_00398.*/

		/*[SWS_Can_00264] The function Can_SetControllerMode(CAN_CS_STOPPED) shall wait for
		a limited time until the CAN controller is really switched off. Compare to SWS_Can_00398.*/
	}
	return Status;
}


/*-----------------------------Can_Write-----------------------*/


