/*!*********************************************************************************************************************
@file user_app1.h                                                                
@brief Header file for user_app1

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1"
6. Add/update any special configurations required in co with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.hnfiguration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

*************************************************************
#define __USER_APP1_H*********************************************************/

#ifndef __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
#define RX_READY ( (AT91C_BASE_US2->US_IMR & AT91C_US_RXRDY) && \
                   (AT91C_BASE_US2->US_CSR & AT91C_US_RXRDY)       )

#define TX_READY ( (AT91C_BASE_US2->US_IMR & AT91C_US_TXRDY) && \
                   (AT91C_BASE_US2->US_CSR & AT91C_US_TXRDY)       )

#define SET_MRDY() ( AT91C_BASE_PIOB->PIO_SODR = PB_23_ANT_MRDY );
#define CLR_MRDY() ( AT91C_BASE_PIOB->PIO_CODR = PB_23_ANT_MRDY );
#define SET_SRDY() ( AT91C_BASE_PIOB->PIO_SODR = PB_24_ANT_SRDY );
#define CLR_SRDY() ( AT91C_BASE_PIOB->PIO_CODR = PB_24_ANT_SRDY );

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);
void SlaveRxFlowCallback(void);
void SlaveTxFlowCallback(void);

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
static void PrintScreen(void);
static void PlayerA(void);
static void PlayerB(void);
static void Judgement(void);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_Error(void);         



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


#endif /* __USER_APP1_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
