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
#ifndef AIR_H 
    #define AIR_H 
    #define AIRTEST 1 //set this to 0 to disable Debug/test

    // initializeing Air
    void vAirInit();
    
    
    //  controller task for air conditions. calculating mean values, sending values, and taking new parameters.
     void vTaskAirController();
    
    
    
    
    #if AIRTEST == 1
         /* --- TEST TASK/FUNCTIONS/VARIABLES --- */
        
        /*
        Task for testing existing implementations.

        DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
        for your current implementation/testing 
        */
        
        
        // init for test task
        void vTestTaskAirInit();
        
        // test task for air controller 
        void vTestTaskAirController();
        
        
    
    
    #endif
#endif


/* [] END OF FILE */
