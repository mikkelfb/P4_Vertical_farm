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

#include "project.h"
#ifndef COMMUNICATION_H 
    #define COMMUNICATION_H 
    #define COMMUNICATIONTEST 1 //set this to 0 to disable Debug/test
    
    
    /*
    This function initializes all of the light tasks.
    @Param[out]:
	@Param[in]
    */
    void vTaskComsInit();
    
    void vTaskRecieveFromFPGA();
    
    void vTaskSendDataRequest();
    
    void vTaskSendNewParams();
    
    void vTaskSendToFPGA();
    
    void vBitShifterUART(uint16 command);
    
    uint16_t vBitShiftForMessage(uint8 command[2]);
    
    void vTaskAlarmHandler();
    
    #if COMMUNICATIONTEST == 1
        /* --- TEST TASK/FUNCTIONS/VARIABLES --- */
        
        /*
        Task for testing existing implementations.

        DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
        for your current implementation/testing
    
        All test task/functions, must start with (type)Test(Task/Function/Var)(Name)
        */
        
        void vTestTaskComsInit();
        
        void vTaskTestComAlarm();
        
    #endif
#endif