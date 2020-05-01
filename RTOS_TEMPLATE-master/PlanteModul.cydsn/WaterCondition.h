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

#ifndef WATERCONDITION_H
    #define WATERCONDITION_H
    #define WATERCONDITION_TEST 1

    /*
    This function initializes all of the nutrient tasks.
    @Param[out]:
	@Param[in]
    */
    void initWaterCondition();
    
    /*
    This task is for reading waterflow sensor, test if there is water in the pipes
    @Param[out]: Generate semaphore for waterConditionController
	@Param[in]: ADC waterstate
    */
    void vTaskFlowWater();
    
    /*
    This task is for setting the waterpump output pin state, 
    only runs if there is a new state in queue
    @Param[out]: sets Pin_WaterPump
	@Param[in]: takes in a queue with state for output 
    */
    void vTaskWaterPump();
    
    /*
    This task is for controlling waterCondition wakes up when FlowWater task gives semaphorer so that the waterstate is updated
    Generates input for datalager task
    @Param[out]: Package for datalager
	@Param[in]:
    */
    void vTaskWaterConditionController();
    
    #if WATERCONDITION_TEST == 1
        /* --- TEST TASK/FUNCTIONS/VARIABLES --- */
        
        /*
        Task for testing existing implementations.

        DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
        for your current implementation/testing
    
        All test task/functions, must start with (type)Test(Task/Function/Var)(Name)
        */
    
    #endif
    
    
    
#endif
