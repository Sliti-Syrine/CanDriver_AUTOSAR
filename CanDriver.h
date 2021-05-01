#include "Can_types.h"
#include "CAN_R.h"
#define  NULL_PTR  ((void *)0)
#define MODE_CHANGED             1U
#define MODE_NOT_CHANGED         0U
#define ENTER_CRITICAL_SECTION(CONTROLLER)				 (int i++)//not completed yet

#define EXIT_CRITICAL_SECTION(CONTROLLER)				(int i--)//not completed yet

#define IN_CRITICAL_SECTION()							(int i=!0)//not completed yet
#define OUT_CRITICAL_SECTION()							(int i=0)//not completed yet



void Can_Init(const Can_ConfigType* config);
Std_ReturnType Can_GetControllerMode(uint8_t Controller, Can_ControllerStateType* ControllerModePtr);
Std_ReturnType Can_SetBaudrate(uint8_t Controller, uint16_t BaudRateConfigID);
Std_ReturnType Can_SetControllerMode(uint8_t Controller, Can_StateTransitionType Transition);
