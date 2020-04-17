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

#ifndef NUTRIENTS_H 
    #define NUTRIENTS_H 
    #define NUTRIENTSTEST 1 //set this to 0 to disable Debug/test
    
    

    
    /*
    This function initializes all of the nutrient tasks.
    @Param[out]:
	@Param[in]
    */
    void vNutrientsInit();
    
    
    /*
    This task controls the nutrients peristaltic pumps.
    @Param[in] *pvParameters Initialize task parameter
    */
    void vTaskNutrientPump();
    
    
    
    
    /* --- Functions and variables for pH --- */

    

    
    
    /*
    This task is for making measurements of the PH sensor.
    @Param[out]: iPHValue the measure pH value.
    */
    void vTaskMeasurePH();
    
    /*
    This task is for calculating the pH value from a measurement, 
    uses the calibrated values fNeutralVoltage, fAcidVoltage
    @Param[out]: float the calculated pHvalue from the voltage. 
    @Param[in]: iPHVoltage the measured pH voltage. 
    */
    float fCalculatePHValue(float fPHVoltage);
    
    
    #if NUTRIENTSTEST == 1
        /* --- TEST TASK/FUNCTIONS/VARIABLES --- */
        
        /*
        Task for testing existing implementations.

        DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
        for your current implementation/testing
    
        All test task/functions, must start with (type)Test(Task/Function/Var)(Name)
        */
    
        /*
        This function initializes all of the nutrient test tasks.
        @Param[out]:
	    @Param[in]
        */
        void vTestTaskInit();
        
        /*
        This function tests the vTaskNutreientPump by sending a 
        Queue signal every 500ms alternating between 1 and 0.
        
        @Param[out]: Sends a queue signal to xQueueNutrientPump every 500 ms shifting between 1 and 0
	    @Param[in]: 
        */
        void vTestTaskNutrientPump();
        
        /*
        This function tests the vTaskMeasurePH by sending a 
        Queue signal every 500ms alternating between 1 and 0.
        @Param[out]: 
	    @Param[in]: 
        */
        void vTestTaskUARTDataTransmit();
    #endif
#endif