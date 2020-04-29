/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */

#include "FreeRTOS.h"
#include "Nutrients.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

//Declare handlers here:

//

void vNewparamInit(){
    
    
    
    
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
}



void vTestHandlers(){
    
}