#include "stdint.h"
#define CAN_E_TRANSITION
#define CAN_E_UNINIT
typedef uint16_t Can_HwHandleType;
typedef uint32_t Can_IdType;
typedef uint32_t PduIdType;

typedef struct Can_ConfigType
{

}Can_ConfigType;

//holding information about the controller
typedef struct Controller_Type
{
    CanIf_ControllerModeType state;
    uint8_t ControllerId; //ControllerId of the the corresponding controller
    uint8_t Channel_Id; //id of a configured channel
    PduIdType swPduHandle;
}Controller_Type;

/*data structure which clearly provides an Hardware Object Handle including its corresponding CAN Controller and therefore CanDrv as well as the specific CanId.*/
typedef struct Can_HwType
{
    Can_IdType CanId;
    Can_HwHandleType CAN_Hoh; //ID of the corresponding Hardware Object Range
    Controller_Type controller;

}Can_HwType;

 typedef struct Can_PduType
 {
     PduIdType swPduHandle;
     uint8_t length;
     Can_IdType id;
     uint8_t* sdu; //SduData
 }Can_PduType;

 typedef enum
 {
    CAN_OK,
	CAN_NOT_OK,
	CAN_BUSY
 }Can_ReturnType;


 typedef enum
 {
     CAN_ERRORSTATE_ACTIVE, //The CAN controller takes fully part in communication
     CAN_ERRORSTATE_PASSIVE, //The CAN controller takes part in communication, but does not send active error frames
     CAN_ERRORSTATE_BUSOFF //The CAN controller does not take part in communication
 }Can_ErrorStateType;


typedef enum
{
    CAN_CS_UNINIT,
    CAN_CS_STARTED,
    CAN_CS_STOPPED,
    CAN_CS_SLEEP
}Can_ControllerStateType;
typedef enum
{
    CAN_UNINIT,
    CAN_READY

}Can_DriverStateType;

