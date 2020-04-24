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

#ifndef AIR_H 
    #define AIR_H 
    #define AIRTEST 1 //set this to 0 to disable Debug/test
   
#include "project.h"


        
    void vAirInit(); // creates tasks and initializes air
    

    // this function initializes the SCD30 sensor and sets the interval between measurements
    void vInitSCD30(uint16_t interval); 
    
    
    // This task is reading temp, RH and Co2 from the SCD30 sensor
    void vTaskGetMeasSCD30();
    
    
    // ----- Test tasks here ----- //
    
    void vAirTestTaskInit(); // init for testtasks Air 
    
    void vTaskTestSCD30(); // test task for the SCD30 sensor
    
    
#endif
