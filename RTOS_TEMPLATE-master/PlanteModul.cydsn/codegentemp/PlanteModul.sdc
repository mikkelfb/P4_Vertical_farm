# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\mag94\Desktop\UNIstuff\4. Semester\P4_Vertical_farm\RTOS_TEMPLATE-master\PlanteModul.cydsn\PlanteModul.cyprj
# Date: Tue, 21 Apr 2020 07:45:36 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {ADC_PH_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 15 31} [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 1875 3751} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {DS18x8_clock_delay} -source [get_pins {ClockBlock/clk_sync}] -edges {1 24001 48001} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_DS18} -source [get_pins {ClockBlock/clk_sync}] -edges {1 24000001 48000001} [list [get_pins {ClockBlock/dclk_glb_2}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\mag94\Desktop\UNIstuff\4. Semester\P4_Vertical_farm\RTOS_TEMPLATE-master\PlanteModul.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\mag94\Desktop\UNIstuff\4. Semester\P4_Vertical_farm\RTOS_TEMPLATE-master\PlanteModul.cydsn\PlanteModul.cyprj
# Date: Tue, 21 Apr 2020 07:45:25 GMT
