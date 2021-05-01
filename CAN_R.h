#define CAN1_BASEADRESS 0x40006400
#define CAN_CTRL_BASE_ADR ((Can_REG)(CAN1_BASEADRESS))
#define RCC_BASE_ADDRESS     0x40023800
#define RCC_APB1_ENABLE      0x40023840
#define HW_ACCESS(type,adresse) (*(volatile type*)adresse)

void CANEnable(uint32_t RCC_APB1_ENABLE )
{
	HWACCESS(uint16_t,RCC_APB1_ENABLE) |= 0x0A000000 ; //enable bxCAN1
};
void CANDisable(uint32_t RCC_APB1_ENABLE )
{
	HWACCESS(uint16_t,RCC_APB1_ENABLE) &= 0xF0FFFFFF; //disable bxCAN1
};

/*CAN REGISTERS*/

/* 3 mailboxes.Each mailbox consist of four registers
------------             -------------                 -------------
CAN_RI0R                  CAN_RI1R                     CAN_RI1R
------------             -------------                --------------
CAN_RDT0R                 CAN_RDT1R                    CAN_RDT1R
------------             -------------                --------------
CAN_RL0R                  CAN_RL1R                     CAN_RL1R
------------             -------------                --------------
CAN_RH0R                  CAN_RH1R                      CAN_RH1R
____________             _____________                ______________
   FIFO0                    FIFO1                        FIFO1        */


typedef volatile struct
{
    volatile uint32_t CAN_MCR;   //0x000
    volatile uint32_t CAN_MSR;   //0x004
    volatile uint32_t CAN_TSR;   //0x008
    volatile uint32_t CAN_RF0R;  //0x00C
    volatile uint32_t CAN_RF1R;  //0x010
    volatile uint32_t CAN_IER;   //0x014
    volatile uint32_t CAN_ESR;   //0x018
    volatile uint32_t CAN_BTR;   //0x01C
    volatile uint32_t RESERVED1[88]; //0x020 -- 0x17F
    volatile uint32_t CAN_TI0R;   //0x180
    volatile uint32_t CAN_TDT0R;  //0x184
    volatile uint32_t CAN_TDL0R;  //0x188
    volatile uint32_t CAN_TDH0R;  //0x18C
    volatile uint32_t CAN_TI1R;   //0x190
    volatile uint32_t CAN_TDT1R;  //0x194
    volatile uint32_t CAN_TDL1R;  //0x198
    volatile uint32_t CAN_TDH1R;  //0x19C
    volatile uint32_t CAN_TI2R;   //0x1A0
    volatile uint32_t CAN_TDT2R;  //0x1A4
    volatile uint32_t CAN_TDL2R;  //0x1A8
    volatile uint32_t CAN_TDH2R;  //0x1AC
    volatile uint32_t CAN_RI0R;   //0x1B0
    volatile uint32_t CAN_RDT0R;  //0x1B4
    volatile uint32_t CAN_RDL0R;  //0x1B8
    volatile uint32_t CAN_RDH0R;  //0x1BC
    volatile uint32_t CAN_RI1R;   //0x1C0
    volatile uint32_t CAN_RDT1R;   //0x1C4
    volatile uint32_t CAN_RDL1R;   //0x1C8
    volatile uint32_t RESERVED2[12]; //0x1D0 -- 0x1FF
    volatile uint32_t CAN_FMR;       //0x200
    volatile uint32_t CAN_FM1R;       //0x204
    volatile uint32_t RESERVED3;      //0x208
    volatile uint32_t CAN_FS1R;       //0x20C
    volatile uint32_t RESERVED4;      //0x210
    volatile uint32_t CAN_FFA1R;       //0x214
    volatile uint32_t RESERVED5;      //0x218
    volatile uint32_t CAN_FA1R;       //0x21C,
    volatile uint32_t RESERVED6;      //0x220
    volatile uint32_t RESERVED7[6]; //0x224 -- 0x23F
/*Filter bank i register x (CAN_FiRx) (i=0..27, x=1, 2)*/
    volatile uint32_t CAN_F0R1;       //0x240
    volatile uint32_t CAN_F0R2;       //0x244
    volatile uint32_t CAN_F1R1;       //0x248
    volatile uint32_t CAN_F1R2;       //0x248

}CAN_REG;

