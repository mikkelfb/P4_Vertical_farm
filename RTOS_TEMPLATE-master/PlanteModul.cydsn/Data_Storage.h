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
#ifndef Data_Storage_H 
    #define Data_Storage_H 
    #define Data_Storage_Test 1 //set this to 0 to disable Debug/test

    
    
    
    // This function initializes all of the data storage tasks.
    void vInintDataStorage(); 
    
    
    
    // collects data from different controller tasks and put them in a new struct.
    _Bool vDataQueueing();
    
      
    /*
    This task will check for data/messages from the controller tasks and from the central.
    Data from the controller tasks will be stored, and messages from the central will red and respond by sending sensor values back to the central.   
    */
    void vTaskDataStorage();
    
    
    


    
    
    #if Data_Storage_Test == 0
       /* --- TEST TASK/FUNCTIONS/VARIABLES --- */

    // Test task for data storage    
    void vTaskTestData();




    #endif
#endif
/* [] END OF FILE */
