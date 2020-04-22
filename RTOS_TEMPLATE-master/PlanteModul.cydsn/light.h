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

#ifndef LIGHT_H 
    #define LIGHT_H 
    #define LIGHTTEST 1 //set this to 0 to disable Debug/test
    
    
    /*
    This function initializes all of the light tasks.
    @Param[out]:
	@Param[in]
    */
    void vLightInit();
    
    
    /*
    This task controls the measurement of light with the light sensor.
    @Param[out]:
	@Param[in]
    */
    void vTaskLightMeasure();
    
    /*
    This task controls the light sensor and the LED light.
    @Param[out]:
	@Param[in]
    */
    void vTaskLightController();
    
    
    #if LIGHTTEST == 1
        /* --- TEST TASK/FUNCTIONS/VARIABLES --- */
        
        /*
        Task for testing existing implementations.

        DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
        for your current implementation/testing
    
        All test task/functions, must start with (type)Test(Task/Function/Var)(Name)
        */
        
        /*
        This function initializes all of the light test tasks.
        @Param[out]:
	    @Param[in]
        */
        void vTestLightTaskInit();
        
        
        void vTestLightTask();
    #endif
#endif