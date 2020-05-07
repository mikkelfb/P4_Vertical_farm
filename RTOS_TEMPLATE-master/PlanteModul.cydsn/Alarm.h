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


#ifndef ALARM_H
    #define ALARM_H
    #define ALARMTEST 1
    
    void vAlarmInit();
    void vTaskAlarmHandling();
    
    #if ALARMTEST == 1
        void vTestTaskAlarmInit();
        void vTaskTestAlarm();
    #endif
    
#endif
/* [] END OF FILE */
