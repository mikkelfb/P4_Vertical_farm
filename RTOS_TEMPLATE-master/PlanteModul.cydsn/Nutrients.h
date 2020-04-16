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

#ifndef NUTRIENTS.H
#define NUTRIENTS.H
    
    
    
    /*
    Param:	Out:
	        In:
    This function initializes all of the nutrient tasks.
    */
    void vNutrientsInit();
    
    
    /*
    Param:	Out:
	        In: *pvParameters (Initialize task parameter)
    Thus task controls the nutrients peristaltic pumps. 
    */
    void vTaskNutrientPump();
    
    
    
    
    
#endif