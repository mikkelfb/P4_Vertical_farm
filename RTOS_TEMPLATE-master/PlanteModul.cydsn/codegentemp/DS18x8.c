/* ============================================================================
 * DS18x8: MAXIM DS18B20 Digital Thermometer component (v 0.0)
 * Read temperature simultaaneously from up to 8 DS18B20 sensors 
 *
 * based on original OneWire code by Evg. Pavlov <pavloven>
 * http://mylab.wmsite.ru/moi-razrab/cypress-psoc/temperature-logger/
 * http://mylab.wmsite.ru/moi-razrab/cypress-psoc/psoc-user-module-library/
 * ============================================================================
 * PROVIDED AS-IS, NO WARRANTY OF ANY KIND, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * FREE TO SHARE, USE AND MODIFY UNDER TERMS: CREATIVE COMMONS - SHARE ALIKE
 * ============================================================================
*/


#include <DS18x8.h>
#include <project.h>


#define GetBit(Value, BitNo) ((Value >> BitNo) & (uint8)0x01)   //macro to read a bit,  returns 0 or 1

uint8 BusPresence; // 8-bit bus state: 1-sensor present, 0-sensor read failure
  

//==============================================================================
//                  ISR Timer data ready to read
//==============================================================================

//CY_ISR_PROTO(DS18x8_IRQDataReady); //ISR proto
CY_ISR(DS18x8_IRQDataReady) // ISR Timer data ready to read
{
    DS18x8_DataReady = 1;
}


//==============================================================================
//                              Read Bit
//==============================================================================

uint8 R_BitBus(void) //  Read a single bit off the 1-WIRE bus
{  
    CyDelayUs(Tinact);  // Inactive before next signal. =20us
    
    DS18x8_ControlReg_SEL_Write(SET_ALL_OUT);
    DS18x8_ControlReg_DRV_Write(LOW);  //Drive LOW selected pin

    CyDelayUs(TMtr1);   // read 1us, write 1-15us
    DS18x8_ControlReg_DRV_Write(HIGH);  //Drive HIGH selected pin
    
	CyDelayUs(5);       // check in 5 usec
    
    //prepare read
    DS18x8_ControlReg_SEL_Write(SET_ALL_INP);   // all outputs disable
    DS18x8_ControlReg_DRV_Write(LOW); // Drive LOW 
    
    uint8 result = DS18x8_StatusReg_BUS_Read();
    return (result);
}

//==============================================================================
//                              Write Bit
//==============================================================================

void W_Bit(uint8 payload) // Write a single bit to the 1-WIRE bus. 
{
	CyDelayUs(Tinact);  //Inactive before next signal.
    DS18x8_ControlReg_DRV_Write(LOW); // Drive LOW enabled outputs

    if(payload==0)	CyDelayUs(TMtr0); //If its a 0 to write, TMtr0 = 50us
	else     		CyDelayUs(TMtr1); //If its a 1 to write, TMtr1 = 10us
        
    DS18x8_ControlReg_DRV_Write(HIGH); // Drive HIGH enabled outputs
}
 
//==============================================================================
//                              Write a byte
//==============================================================================

void Write8(uint8 payload) //Write a byte to the slave.
{
	uint8 BitPayload,  shiftcount;
    
    DS18x8_ControlReg_SEL_Write(SET_ALL_OUT); // enable all outputs [11]
    
	for (shiftcount=0; shiftcount<=7;shiftcount++) 
    {
        BitPayload = (payload >> shiftcount) & 0x01; //starting with LSB
        W_Bit(BitPayload);
    }
}  


//==============================================================================
//                              Bus Reset
//==============================================================================
   
uint8 BusReset() //Pulldown the required "Bus Reset". 
{
    static struct DS18x8_TSensor Sensor_off = {T_OFF, 0};
    uint8 i;
    
    // reset all sensor Temerature and presence
    for (i=0; i<8; i++) {DS18x8_Sensor[i] = Sensor_off;}
    
    
    uint8 BusPin=0;    //bus state

    DS18x8_ControlReg_DRV_Write(LOW);         // Drive LOW enabled outputs
    DS18x8_ControlReg_SEL_Write(SET_ALL_OUT); // enable all outputs [11111111]
    
 
	CyDelayUs(RST_MAX); // send RESET pulse, 480usec min
    DS18x8_ControlReg_DRV_Write(HIGH); // Drive HIGH enabled outputs
    
    CyDelayUs(5);       // check bus shorted
    
    //prepare to read
    DS18x8_ControlReg_SEL_Write(SET_ALL_INP); // set all as inputs [0000000]
    DS18x8_ControlReg_DRV_Write(LOW);         // Drive LOW enabled outputs
       
    CyDelayUs(55);      // check PRESENCE in 15-60usec  (60-240 мкСек)
    
    BusPin = DS18x8_StatusReg_BUS_Read();   // Read state: bit=0 if present 1 if unplugged
    
    CyDelayUs(200);     // wait for DS18 to transmit PRESENCE  (60-240 usec) 
    
    return((~BusPin) & 255u);     // Return bus state: 1-present, 0-absent  (originally 0-present, 1-fault)
}


//==============================================================================
//                         start temperature conversion
//==============================================================================

uint8 DS18x8_SendTemperatureRequest() // start temperature conversion 
{   
    BusPresence = BusReset(); // Reset ( > 480 usec ) 
    
    if (BusPresence == 0) {
        DS18x8_DataReady = 1; //proceed directly to reporting
        return (0);                     // no sensors found
    }
    
    //DS18x8_ControlReg_SEL_Write(ALL); // enable all outputs [11111111]
    Write8(0xCC);       // send command Skip_ROM  (0xCCÍ)  
    Write8(0x44);       // send command for temperature conversion  (0x44Í)

    DS18x8_Trigger_Write(1);   // trigger Async delayed reading
    
    return (BusPresence);
}


//==============================================================================
// Read sensor Temperature
// data is ready - read sensor's Temperature LSB & MSB 
//==============================================================================

uint8 DS18x8_ReadTemperature() 
{
    uint8 Bus;              // read state of bus  
    uint8 mask;             // sensor presence mask
    uint8 i, shiftcount;
    struct DS18x8_TSensor s[8]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; //array of sensors {Temperature, presence}, must reset to "0"
    
    if (!DS18x8_DataReady) return  (0);   // sensor not ready for temperature reading
    DS18x8_DataReady = 0;                 // reset flag
    
  
    if (BusPresence == 0) return (0); // no sensors found on temperature request, exit 
    
    
    BusPresence = BusReset();  // Reset  ( > 480 usec )
    if (BusPresence == 0) return (0); // no sensors found 
    
    
    mask = BusPresence;
        
    for (i=0; i<DS18x8_NumSensors; i++) {s[i].present = GetBit(mask, i);} //
    
    
    //DS18x8_ControlReg_SEL_Write(SET_ALL_OUT); // enable all for outputs [1111] - can use as selection mask
    Write8(0xCC);   // send command Skip_ROM  (0xCC)    
    Write8(0xBE);   // send command to read buffer memory (0xBE)
    
    // read first two bytes of the memory DS18B20
    for (shiftcount=0; shiftcount<=11; shiftcount++) // read bytes 0 to 11
    {
        Bus = R_BitBus(); //read all sensors in parallel
        
        for (i=0; i<DS18x8_NumSensors; i++) //
        {
            if (s[i].present) //ignore channels without sensor
                s[i].Temperature |= GetBit(Bus, i) << shiftcount; // staff Temperature begining with LSB //todo: SetBit
        }
    }
    
    for (i=0; i<8; i++) //8-is maximum number of sensors in current implementation
    {
        if (s[i].present)   
            {if (s[i].Temperature >= 2048) s[i].Temperature -= 4096;} // roll over for negative temperatures
        else
            {s[i].Temperature = T_OFF;} // set some "defunct" value if sensor absent 
        
        DS18x8_Sensor[i] = s[i]; //record Temperature and presence
    }
    
    return(mask); //return BusPresence
}


//==============================================================================
// Convert DS18B20 encoded temperature to float value (degC)
//==============================================================================

float DS18x8_GetTemperatureAsFloat (uint8 index) // 
{    
    int16 Val = DS18x8_Sensor[index].Temperature;
    return ( (float) Val * 0.0625f);
}


//==============================================================================
// Convert DS18B20 temperature (degC) as *100 value rounded to integer
// e.g. -38.0625 -> "-3806" 
//==============================================================================

int16 DS18x8_GetTemperatureAsInt100 (uint8 index) 
{
    int16 Val = DS18x8_Sensor[index].Temperature;
    return ((Val * 100u + 8u) >> 4); //rounding
}


//==============================================================================
// C++ version 0.4 char* style "itoa":
// Written by Lukás Chmela
// Released under GPLv3.
// http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
// here: modified for radix 10 only 
//==============================================================================

char* itoa10(int value, char* result)
{
    const uint8 base = 10u;
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "9876543210123456789" [9 + tmp_value - value * base];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}



//==============================================================================
// Convert DS18B20 temperature (degC) as string
// e.g. -38.0625 <-> "-38.0625"
//==============================================================================

char* DS18x8_GetTemperatureAsString (uint8 index) 
{
    char TempStr[13]={};    // buffer string for current temperature //warning
    char* result = TempStr; //
    char buf[8];            // buffer
    CYBIT sign;             // Temp sign
    int grad;               // integer portion w/o sign
    uint8 frac;             // fractional portion
    
    static const char *str[] = {".00", ".06", ".13", ".19", ".25", ".31", ".38", ".44", ".50", ".56", ".63", ".69", ".75", ".81", ".88", ".94"};// %.2
     
    // exit and return pre-defined string if no sensor present
    if (!DS18x8_Sensor[index].present) return (Str_OFF);

        
    int16 Val = DS18x8_Sensor[index].Temperature;
    sign = (Val < 0);                       // remember sign prior to shift     
    if (sign) grad = -Val; else grad = Val; //grad = abs(Temp); no need for stdlib.h
    frac = grad & 15;                       //index for fractional part
    grad = grad >> 4;                       //get integer portion
    
    if (sign) strcat(TempStr, itoa10(-grad, buf)); 
    else      strcat(TempStr, itoa10( grad, buf));
    
    strcat(TempStr, str[frac]); //add fractional part
    
    return (result);
}

//==============================================================================
// Initialize and Start component
//==============================================================================

void DS18x8_Start() 
{
    DS18x8_isr_DataReady_StartEx(DS18x8_IRQDataReady); //set interrupt
    
    //configure and start 1-shot timer for 750ms data conversion delay
    DS18x8_TimerDelay_WriteCounter(T_conv);   // set time for conversion 750ms
    DS18x8_TimerDelay_Start();                // Initialize TimerDelay
    
    DS18x8_DataReady = 0;                     // semaphor flag
}

//==============================================================================
// Disable component
//==============================================================================

void DS18x8_Stop() 
{
    DS18x8_TimerDelay_Stop(); // disable timer
}

//==============================================================================
// Return 8-bit bus state: 1-sensor present, 0-sensor read failure
//==============================================================================

uint8 DS18x8_BusState()
{
    return( BusPresence );   
}

//==============================================================================
// Function to check sensors presence
// Return 8-bit bus state: 1-sensor present, 0-sensor read failure
//==============================================================================

uint8 DS18x8_CheckPresence()
{  
    BusPresence = BusReset(); // Reset ( > 480 usec ) 
    return( BusPresence );   
}




/* [] END OF FILE */


  
