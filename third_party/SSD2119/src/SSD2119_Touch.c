/*********************************************************************
 * SSD2119_Touch.c
 * Runs on the EK-TM4C1294XL Connected LaunchPad.
 * Touchpad driver for the SSD2119 interface on a Kentec 320x240x16 
 * BoosterPack. The display assembly should be plugged in via the 
 * BoosterPack 2 interface on the TM4C1294XL.
 * 
 * Touchpad pin assignments:
 * PQ1     X-         
 * PM6     Y-     
 * PD5     Y+    AIN6
 * PD4     X+    AIN7
 * 
 * -------------  -------------
 * | X-  | Y-  |  | Y+  | X+  |  <<<=== Function
 * | R   | B   |  | T   | L   |  <<<=== Edge
 * | PQ1 | PM6 |  | PD5 | PD4 |  <<<=== TM4C1294
 * | --- | --- |  | AI6 | AI7 |  <<<=== Analog In
 * -------------  -------------
 * Ports required: Q, M, D
 * 
 *      YP / Y+ / PD5 / AIN6
 *      ---------------------
 *      |                   |
 *   XP |                   | XN   
 *   X+ |                   | X-  
 *  PD4 |                   | PQ1
 * AIN7 |                   |
 *      |                   |
 *      ---------------------
 *          YN / Y- / PM6
 ********************************************************************/
#include <stdint.h>
#include "official_tm4c1294ncpdt.h"
#include "SSD2119_Touch.h"

#if !defined TOUCH_USE_ADC0 && !defined TOUCH_USE_ADC1
    #error ADC module is not defined for the touchpad.
#elif defined TOUCH_USE_ADC0 && defined TOUCH_USE_ADC1
    #error More than 1 module is defined.
#endif

#define PQ1  (1<<1)   //X-         
#define PM6  (1<<6)   //Y-     
#define PD5  (1<<5)   //Y+    AIN6
#define PD4  (1<<4)   //X+    AIN7

// Private Functions
// - Initializes the ADC to use a specficed channel on SS3
static void ADC_Init(void);

// - Takes a sample from the ADC
static unsigned long ADC_Read(void);

// - Configures the ADC to use a specific channel
static void ADC_SetChannel(unsigned char channelNum);

// **************  Touch_Init ******************************
// - Initializes the GPIO used for the touchpad
// - Port D for ADC, port Q and M for digital output
// *********************************************************
// Input: none
// Output: none
// *********************************************************
void Touch_Init(void){
    unsigned long wait = 0;    
    // Initialize ADC for touchscreen
    ADC_Init();    
    
    // GPIO port D
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
    wait++;
    wait++;
    GPIO_PORTD_DIR_R |= (PD4|PD5);
     
    // GPIO Port M
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;
    wait++;
    wait++;
    GPIO_PORTM_DIR_R |= PM6;

    // GPIO Port Q
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R14;
    wait++;
    wait++;
    GPIO_PORTQ_DIR_R |= PQ1;
}

// ************** ADC_Init *********************************
// - Initializes the ADC to use a specficed channel on SS3
// *********************************************************
// Input: none
// Output: none
// *********************************************************
static void ADC_Init(void){
    long wait = 0;
    
    #if defined TOUCH_USE_ADC0
    // Set bit 0 in SYSCTL_RCGCADC_R to enable ADC0
    SYSCTL_RCGCADC_R |= 0x01;
    wait++;
    wait++;
    // Power up the PLL
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while (SYSCTL_PLLSTAT_R != 0x1); // wait for PLL to lock
    // Enable PIOSC in the CS bit field in the ADCCC registeR
    ADC0_CC_R = 0x1;
    // ALTCLK field should be programmed to 0x0
    SYSCTL_ALTCLKCFG_R = 0x0;
    // Set ADC sample to 125KS/s
    ADC0_PC_R = 0x01;
    // ADC sample averaging control
    ADC0_SAC_R = 0x3;
    // Disable all sequencers for configuration
    ADC0_ACTSS_R &= ~0x000F;
    // Set ADC0 SS3 to highest priority
    ADC0_SSPRI_R = 0x0123;    
    // Set bits 12-15 to 0x00 to enable software trigger on SS3
    ADC0_EMUX_R &= ~0xF000;
    // Set sample channel for sequencer 3
    ADC0_SSMUX3_R &= 0xFFF0;    
    ADC0_SSMUX3_R += 6;
    // TS0 = 0, IE0 = 1, END0 = 1, D0 = 0
    ADC0_SSCTL3_R = 0x006;
    // Disable ADC interrupts on SS3 by clearing bit 3
    ADC0_IM_R &= ~0x0008;
    // Re-enable sample sequencer 3
    ADC0_ACTSS_R |= 0x0008;

    #elif defined TOUCH_USE_ADC1
    // Set bit 1 in SYSCTL_RCGCADC_R to enable ADC0
    SYSCTL_RCGCADC_R |= (0x1<<1);
    wait++;
    wait++;
    // Power up the PLL
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while (SYSCTL_PLLSTAT_R != 0x1); // wait for PLL to lock
    // Enable PIOSC in the CS bit field in the ADCCC registeR
    ADC1_CC_R = 0x1;
    // ALTCLK field should be programmed to 0x0
    SYSCTL_ALTCLKCFG_R = 0x0;
    // Set ADC sample to 125KS/s
    ADC1_PC_R = 0x01;
    // ADC sample averaging control
    ADC1_SAC_R = 0x3;
    // Disable all sequencers for configuration
    ADC1_ACTSS_R &= ~0x000F;
    // Set ADC0 SS3 to highest priority
    ADC1_SSPRI_R = 0x0123;    
    // Set bits 12-15 to 0x00 to enable software trigger on SS3
    ADC1_EMUX_R &= ~0xF000;
    // Set sample channel for sequencer 3
    ADC1_SSMUX3_R &= 0xFFF0;    
    ADC1_SSMUX3_R += 6;
    // TS0 = 0, IE0 = 1, END0 = 1, D0 = 0
    ADC1_SSCTL3_R = 0x006;
    // Disable ADC interrupts on SS3 by clearing bit 3
    ADC1_IM_R &= ~0x0008;
    // Re-enable sample sequencer 3
    ADC1_ACTSS_R |= 0x0008;
    #endif
}

// ************** ADC_Read *********************************
// - Takes a sample from the ADC
// *********************************************************
// Input: none
// Output: sampled value from the ADC
// *********************************************************
unsigned long ADC_Read(void){
    unsigned long result;
    
    #if defined TOUCH_USE_ADC0

    // Set bit 3 to trigger sample start
    ADC0_PSSI_R = 0x008; 
    // Wait for SS3 RIS bit to be set to 1
    while ((ADC0_RIS_R & 0x08) == 0); 
    // Read 12-bit result from ADC from FIFO
    result = ADC0_SSFIFO3_R&0xFFF;
    // Clear SS3 RIS bit to 0 to acknowledge completion
    ADC0_ISC_R = 0x0008;

    #elif defined TOUCH_USE_ADC1

    // Set bit 3 to trigger sample start
    ADC1_PSSI_R = 0x008; 
    // Wait for SS3 RIS bit to be set to 1
    while ((ADC1_RIS_R & 0x08) == 0); 
    // Read 12-bit result from ADC from FIFO
    result = ADC1_SSFIFO3_R&0xFFF;
    // Clear SS3 RIS bit to 0 to acknowledge completion
    ADC1_ISC_R = 0x0008;

    #endif

    return result;
}

// ************** ADC_SetChannel ***************************
// - Configures the ADC to use a specific channel
// *********************************************************
// Input: none
// Output: none
// *********************************************************
void ADC_SetChannel(unsigned char channelNum){
    #if defined TOUCH_USE_ADC0
    
    // Disable all sequencers for configuration
    ADC0_ACTSS_R &= ~0xF;
    // Set sample channel for sequencer 3
    ADC0_SSMUX3_R &= ~0xF;
    ADC0_SSMUX3_R += channelNum;
    // Re-enable sample sequencer 3
    ADC0_ACTSS_R |= 0x8;

    #elif defined TOUCH_USE_ADC1

    // Disable all sequencers for configuration
    ADC1_ACTSS_R &= ~0xF;
    // Set sample channel for sequencer 3
    ADC1_SSMUX3_R &= ~0xF;
    ADC1_SSMUX3_R += channelNum;
    // Re-enable sample sequencer 3
    ADC1_ACTSS_R |= 0x8;

    #endif
}

// ************** ADC_ReadY ********************************
// - 
// *********************************************************
// Input: none
// Output: Y value
// *********************************************************
unsigned long Touch_ReadY(void){
    // Configure PD5 (Y+/Top/AIN6) for ADC
    GPIO_PORTD_AFSEL_R  |=  PD5;       // AFSEL = 1
    GPIO_PORTD_AMSEL_R  &= ~PD5;       // AMSEL = 0
    GPIO_PORTD_DEN_R    &= ~PD5;       // DEN = 0
    GPIO_PORTD_DIR_R    &= ~PD5;       // DIR = 0

    // Configure PM6 (Y-/Bottom) for digital LOW
    GPIO_PORTM_AFSEL_R  &= ~PM6;       // AFSEL = 0
    GPIO_PORTM_AMSEL_R  |=  PM6;       // AMSEL = 1
    GPIO_PORTM_DEN_R    |=  PM6;       // DEN = 1
    GPIO_PORTM_DIR_R    |=  PM6;       // DIR = 1
    GPIO_PORTM_DATA_R   &= ~PM6;       // DATA = 0

    // Configure PD4 (X+/Left/AIN7) for digital HIGH      
    GPIO_PORTD_AFSEL_R  &= ~PD4;       // AFSEL = 0
    GPIO_PORTD_AMSEL_R  |=  PD4;       // AMSEL = 1
    GPIO_PORTD_DEN_R    |=  PD4;       // DEN = 1               
    GPIO_PORTD_DIR_R    |=  PD4;       // DIR = 1               
    GPIO_PORTD_DATA_R   |=  PD4;       // DATA = 1          
                                                      
    // Configure PQ1 (X-/Right) for digital HIGH         
    GPIO_PORTQ_AFSEL_R  &= ~PQ1;       // ASFEL = 0
    GPIO_PORTQ_AMSEL_R  |=  PQ1;       // AMSEL = 1
    GPIO_PORTQ_DEN_R    |=  PQ1;       // DEN = 1
    GPIO_PORTQ_DIR_R    |=  PQ1;       // DIR = 1
    GPIO_PORTQ_DATA_R   |=  PQ1;       // DATA = 1    

    // Configure ADC to read from AIN6 (Y+/Top/PD5)
    ADC_SetChannel(6);
    
    // Discard
    ADC_Read();

    // Keep
    unsigned long result = ADC_Read();

    return result;
}

// ************** ADC_ReadX ********************************
// - 
// *********************************************************
// Input: none
// Output: X value
// *********************************************************
unsigned long Touch_ReadX(void){
    // Configure PD4 (X+/Left/AIN7) for ADC
    GPIO_PORTD_AFSEL_R  |=  PD4;       // AFSEL = 1
    GPIO_PORTD_AMSEL_R  &= ~PD4;       // AMSEL = 0
    GPIO_PORTD_DEN_R    &= ~PD4;       // DEN = 0
    GPIO_PORTD_DIR_R    &= ~PD4;       // DIR = 0

    // Configure PQ1 (X-/Right) for digital LOW
    GPIO_PORTQ_AFSEL_R  &= ~PQ1;       // AFSEL = 0
    GPIO_PORTQ_AMSEL_R  |=  PQ1;       // AMSEL = 1
    GPIO_PORTQ_DEN_R    |=  PQ1;       // DEN = 1
    GPIO_PORTQ_DIR_R    |=  PQ1;       // DIR = 1
    GPIO_PORTQ_DATA_R   &= ~PQ1;       // DATA = 0
    
    // Configure PD5 (Y+/Top/AIN6) for digital HIGH
    GPIO_PORTD_AFSEL_R  &= ~PD5;       // ASFEL = 0
    GPIO_PORTD_AMSEL_R  |=  PD5;       // AMSEL = 1
    GPIO_PORTD_DEN_R    |=  PD5;       // DEN = 1
    GPIO_PORTD_DIR_R    |=  PD5;       // DIR = 1
    GPIO_PORTD_DATA_R   |=  PD5;       // DATA = 1
    
    // Configure PM6 (Y-/Bottom) for digital HIGH
    GPIO_PORTM_AFSEL_R  &= ~PM6;       // ASFEL = 0
    GPIO_PORTM_AMSEL_R  |=  PM6;       // AMSEL = 1
    GPIO_PORTM_DEN_R    |=  PM6;       // DEN = 1
    GPIO_PORTM_DIR_R    |=  PM6;       // DIR = 1
    GPIO_PORTM_DATA_R   |=  PM6;       // DATA = 1

    // Configure ADC to read from AIN7 (X+/Left/PD4)
    ADC_SetChannel(7);
    
    // Discard
    ADC_Read();

    // Keep
    unsigned long result = ADC_Read();
    
    return result;
}
