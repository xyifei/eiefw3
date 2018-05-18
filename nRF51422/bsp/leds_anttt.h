/******************************************************************************
File: leds_anttt.h                                                               

Description:
Header file for leds_anttt.c

DISCLAIMER: THIS CODE IS PROVIDED WITHOUT ANY WARRANTY OR GUARANTEES.  USERS MAY
USE THIS CODE FOR DEVELOPMENT AND EXAMPLE PURPOSES ONLY.  ENGENUICS TECHNOLOGIES
INCORPORATED IS NOT RESPONSIBLE FOR ANY ERRORS, OMISSIONS, OR DAMAGES THAT COULD
RESULT FROM USING THIS FIRMWARE IN WHOLE OR IN PART.

******************************************************************************/

#ifndef __LEDS_H
#define __LEDS_H

#include "configuration.h"

/******************************************************************************
Type Definitions
******************************************************************************/
typedef enum {BLUE = 0, GREEN, YELLOW, RED} eLedTypes;

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Function Declarations
******************************************************************************/
/* Public Functions */
void LedOn(eLedTypes);
void LedOff(eLedTypes);
void LedToggle(eLedTypes);
bool LedCheckOn(eLedTypes);

/* Protected Functions */

/* Private Functions */


/******************************************************************************
* State Machine Function Prototypes
******************************************************************************/


#endif /* __LEDS_H */
