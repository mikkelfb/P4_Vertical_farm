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

#ifndef NEWPARAM_H 
    #define NEWPARAM_H 
    #define NEWPARAMTEST 1 //set this to 0 to disable Debug/test
    #include "project.h"
 
    void vNewparamInit();
    void vTaskNewParam();
    
    #if NEWPARAMTEST == 1
        /* --- TEST TASK/FUNCTIONS/VARIABLES --- */
        
        /*
        Task for testing existing implementations.

        DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
        for your current implementation/testing
    
        All test task/functions, must start with (type)Test(Task/Function/Var)(Name)
        */
        
        void vComsFromOtherTask();        
        void vInitTestParam();
        
    #endif
#endif