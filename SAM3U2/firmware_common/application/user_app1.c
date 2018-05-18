/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */
extern u8 G_au8DebugScanfBuffer[];                        /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */

SspConfigurationType Spi_Config;
SspPeripheralType* AntttTaskSsp;
static u8 au8RXD[128]="";
static u8* pu8RxBuffer=&au8RXD[0];
static u8 u8RXD=0x00;
static bool bTimeToPrint = FALSE;
static bool bJudge = TRUE;
static bool bPlayerAOrPlayerB = TRUE;
static u8 au8GameScreen[][51] = \
	{
		"012345678901234567890123456789012345678901234567\n\r",
		"1               |               |               \n\r",
		"2               |               |               \n\r",
		"3               |               |               \n\r",
		"4       1       |       2       |       3       \n\r",
		"5               |               |               \n\r",
		"6               |               |               \n\r",
		"7---------------|---------------|---------------\n\r",
		"8               |               |               \n\r",
		"9               |               |               \n\r",
		"0               |               |               \n\r",
		"1       4       |       5       |       6       \n\r",
		"2               |               |               \n\r",
		"3               |               |               \n\r",
		"4---------------|---------------|---------------\n\r",
		"5               |               |               \n\r",
		"6               |               |               \n\r",
		"7               |               |               \n\r",
		"8       7       |       8       |       9       \n\r",
		"9               |               |               \n\r",
		"0               |               |               \n\r"
	};
static u8 au8GameEnd[][51] = \
{
		"012345678901234567890123456789012345678901234567\n\r",
		"1                                               \n\r",
		"2        @@@@@@@     @      @     @@@@@@@       \n\r",
		"3        @           @@@    @     @      @      \n\r",
		"4        @@@@@@@     @  @   @     @      @      \n\r",
		"5        @           @   @  @     @      @      \n\r",
		"6        @@@@@@@     @    @@@     @@@@@@@       \n\r",
		"7                                               \n\r",
		"8                                               \n\r",
		"9                                               \n\r",
		"0                                               \n\r",
		"1                                               \n\r",
		"2                                               \n\r",
		"3                                               \n\r",
		"4                                               \n\r",
		"5                                               \n\r",
		"6                                               \n\r",
		"7                                               \n\r",
		"8                                               \n\r",
		"9                                               \n\r",
		"0                                               \n\r"
};

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

void SlaveRxFlowCallback(void)
{
   u8RXD = 0xFF & AT91C_BASE_US2->US_RHR;
}


void SlaveTxFlowCallback(void)
{
    
}
/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{   
    Spi_Config.SspPeripheral = USART2;
    Spi_Config.pCsGpioAddress = AT91C_BASE_PIOB;
    Spi_Config.u32CsPin = PB_22_ANT_USPI2_CS;
    Spi_Config.eBitOrder = MSB_FIRST;
    Spi_Config.eSspMode = SPI_SLAVE_FLOW_CONTROL;
    Spi_Config.fnSlaveRxFlowCallback = &SlaveRxFlowCallback;
    Spi_Config.fnSlaveTxFlowCallback = &SlaveTxFlowCallback;
    Spi_Config.pu8RxBufferAddress = &au8RXD[0];
    Spi_Config.ppu8RxNextByte = &pu8RxBuffer;
    Spi_Config.u16RxBufferSize = 128;
  
    AntttTaskSsp = SspRequest(&Spi_Config);
    
    PrintScreen();
  /* If good initialization, set state to Idle */
  if(AntttTaskSsp != NULL)
  {
    LedOn(GREEN);
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LedOn(RED);
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
static void PrintScreen(void)
{
	for(u8 i = sizeof(au8GameScreen) / 51, *pu8Point = au8GameScreen[0]; i; i--, pu8Point += 51)
	{
		DebugPrintf(pu8Point);
	}
}

static void Judgement(void)
{ 
  	if(bJudge ==1)
	{
		if((au8GameScreen[4][8] == au8GameScreen[4][24] && au8GameScreen[4][8] == au8GameScreen[4][40])
			||(au8GameScreen[11][8] == au8GameScreen[11][24] && au8GameScreen[11][8] == au8GameScreen[11][40])
			||(au8GameScreen[18][8] == au8GameScreen[18][24] && au8GameScreen[18][8] == au8GameScreen[18][40])
			||(au8GameScreen[4][8] == au8GameScreen[11][8] && au8GameScreen[4][8] == au8GameScreen[18][8])
			||(au8GameScreen[4][24] == au8GameScreen[11][24] && au8GameScreen[4][24] == au8GameScreen[18][24])
			||(au8GameScreen[4][40] == au8GameScreen[11][40] && au8GameScreen[4][40] == au8GameScreen[18][40])
			||(au8GameScreen[4][8] == au8GameScreen[11][24] && au8GameScreen[4][8]== au8GameScreen[18][40])
			||(au8GameScreen[4][40] == au8GameScreen[11][24] && au8GameScreen[4][40]== au8GameScreen[18][8]))
		{
			for(u8 i = sizeof(au8GameEnd) / 51, *pu8Point = au8GameEnd[0]; i; i--, pu8Point += 51)
			{
				DebugPrintf(pu8Point);
			}
            
			bJudge = FALSE;
		}
	}
}

static void PlayerA(void)
{
	static u8 au8Buffer[5];
    static bool bRepeat = FALSE;

    if(DebugScanf(au8Buffer))
    {
        switch(au8Buffer[0]) 
        {
            case '1':
            {
                if((au8GameScreen[4][8] == 'O')||(au8GameScreen[4][8] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[4][8] = 'X';
                }
                
                break;
            }
            case '2':
            {
                if((au8GameScreen[4][24] == 'O')||(au8GameScreen[4][24] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[4][24] = 'X';
                }
                break;
            }
            case '3':
            {
                if((au8GameScreen[4][40] == 'O')||(au8GameScreen[4][40] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[4][40] = 'X';
                }
                
                break;
            }
            case '4':
            {
                if((au8GameScreen[11][8] == 'O')||(au8GameScreen[11][8] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[11][8] = 'X';
                }
                
                break;
            }
            case '5':
            {
                if((au8GameScreen[11][24] == 'O')||(au8GameScreen[11][24] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[11][24] = 'X';
                }
                
                break;
            }
            case '6':
            {
                if((au8GameScreen[11][40] == 'O')||(au8GameScreen[11][40] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[11][40] = 'X';
                }
                
                break;
            }
            case '7':
            {
                if((au8GameScreen[18][8] == 'O')||(au8GameScreen[18][8] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[18][8] = 'X';
                }
                
                break;
            }
            case '8':
            {
                if((au8GameScreen[18][24] == 'O')||(au8GameScreen[18][24] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[18][24] = 'X';
                }
                
                break;
            }
            case '9':
            {
                if((au8GameScreen[18][40] == 'O')||(au8GameScreen[18][40] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[18][40] = 'X';
                }

                break;
            }
            default:
            {
                break; 
            }
        }
        
        if(bRepeat == TRUE)
        {
            DebugPrintf("Wrong input");
        }
        else
        {
            bTimeToPrint = TRUE;
            bPlayerAOrPlayerB = FALSE; 
        }
    }
	
}

static void PlayerB(void)
{
    static u8 u8New=0x00;
    static bool bRepeat = FALSE;
    
    if(u8New != u8RXD)
    {
        u8New = u8RXD;

        switch(u8RXD) 
        {
            case 0x11:
            {
                if((au8GameScreen[4][8] == 'O')||(au8GameScreen[4][8] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[4][8] = 'O';
                }
                
                break;
            }
            case 0x12:
            {
                if((au8GameScreen[4][24] == 'O')||(au8GameScreen[4][24] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[4][24] = 'O';
                }
                break;
            }
            case 0x13:
            {
                if((au8GameScreen[4][40] == 'O')||(au8GameScreen[4][40] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[4][40] = 'O';
                }
                
                break;
            }
            case 0x21:
            {
                if((au8GameScreen[11][8] == 'O')||(au8GameScreen[11][8] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[11][8] = 'O';
                }
                
                break;
            }
            case 0x22:
            {
                if((au8GameScreen[11][24] == 'O')||(au8GameScreen[11][24] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[11][24] = 'O';
                }
                
                break;
            }
            case 0x23:
            {
                if((au8GameScreen[11][40] == 'O')||(au8GameScreen[11][40] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[11][40] = 'O';
                }
                
                break;
            }
            case 0x31:
            {
                if((au8GameScreen[18][8] == 'O')||(au8GameScreen[18][8] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[18][8] = 'O';
                }
                
                break;
            }
            case 0x32:
            {
                if((au8GameScreen[18][24] == 'O')||(au8GameScreen[18][24] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[18][24] = 'O';
                }
                
                break;
            }
            case 0x33:
            {
                if((au8GameScreen[18][40] == 'O')||(au8GameScreen[18][40] == 'X'))
                {
                    bRepeat = TRUE;
                }
                else
                {
                    bRepeat =FALSE;
                    au8GameScreen[18][40] = 'O';
                }

                break;
            }
            default:
            {
                break; 
            } 
        }
        
        if(bRepeat)
        {
            DebugPrintf("Wrong input");
        }
        else
        {
            bTimeToPrint = TRUE;
            bPlayerAOrPlayerB = TRUE;
        }
    }

}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
    if(bPlayerAOrPlayerB)
    {
        PlayerA();
    }
    else
    {
        PlayerB();
    }
    
  	if(bTimeToPrint)
  	{     
  		PrintScreen();
		bTimeToPrint = FALSE;
        
        if(bPlayerAOrPlayerB)
        {
            DebugPrintf("Your turn:");
        }
        else
        {
            DebugPrintf("BLE turn:");
        }
  	}
    
  	Judgement();
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
