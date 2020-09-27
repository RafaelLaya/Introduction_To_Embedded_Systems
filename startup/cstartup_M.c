/**************************************************
 *
 * This file contains an interrupt vector for Cortex-M written in C.
 * The actual interrupt functions must be provided by the application developer.
 *
 * Copyright 2007-2017 IAR Systems AB.
 *
 * $Revision: 112610 $
 * 
 * User added the rest of the handlers (those with IRQ number higher than SysTick_Handler)
 **************************************************/

#pragma language=extended
#pragma segment="CSTACK"

extern void __iar_program_start( void );

extern void NMI_Handler( void );
extern void HardFault_Handler( void );
extern void MemManage_Handler( void );
extern void BusFault_Handler( void );
extern void UsageFault_Handler( void );
extern void SVC_Handler( void );
extern void DebugMon_Handler( void );
extern void PendSV_Handler( void );
extern void SysTick_Handler( void );
extern void GPIOA_Handler( void );
extern void GPIOB_Handler( void );
extern void GPIOC_Handler( void );
extern void GPIOD_Handler( void );
extern void GPIOE_Handler( void );
extern void GPIOF_Handler( void );
extern void GPIOG_Handler( void );
extern void GPIOH_Handler( void );
extern void GPIOJ_Handler( void );
extern void GPIOK_Handler( void );
extern void GPIOL_Handler( void );
extern void GPIOM_Handler( void );
extern void GPION_Handler( void );
extern void GPIOP0_Handler( void );
extern void GPIOP1_Handler( void );
extern void GPIOP2_Handler( void );
extern void GPIOP3_Handler( void );
extern void GPIOP4_Handler( void );
extern void GPIOP5_Handler( void );
extern void GPIOP6_Handler( void );
extern void GPIOP7_Handler( void );
extern void GPIOQ0_Handler( void );
extern void GPIOQ1_Handler( void );
extern void GPIOQ2_Handler( void );
extern void GPIOQ3_Handler( void );
extern void GPIOQ4_Handler( void );
extern void GPIOQ5_Handler( void );
extern void GPIOQ6_Handler( void );
extern void GPIOQ7_Handler( void );
extern void Timer0A_Handler( void );
extern void Timer0B_Handler( void );
extern void Timer1A_Handler( void );
extern void Timer1B_Handler( void );
extern void Timer2A_Handler( void );
extern void Timer2B_Handler( void );
extern void Timer3A_Handler( void );
extern void Timer3B_Handler( void );
extern void Timer4A_Handler( void );
extern void Timer4B_Handler( void );
extern void Timer5A_Handler( void );
extern void Timer5B_Handler( void );
extern void Timer6A_Handler( void );
extern void Timer6B_Handler( void );
extern void Timer7A_Handler( void );
extern void Timer7B_Handler( void );
extern void UART0_Handler( void );
extern void UART1_Handler( void );
extern void UART2_Handler( void );
extern void UART3_Handler( void );
extern void UART4_Handler( void );
extern void UART5_Handler( void );
extern void UART6_Handler( void );
extern void UART7_Handler( void );
extern void I2C0_Handler( void );
extern void I2C1_Handler( void );
extern void I2C2_Handler( void );
extern void I2C3_Handler( void );
extern void I2C4_Handler( void );
extern void I2C5_Handler( void );
extern void I2C6_Handler( void );
extern void I2C7_Handler( void );
extern void I2C8_Handler( void );
extern void I2C9_Handler( void );
extern void PWM_Generator0_Handler( void );
extern void PWM_Generator1_Handler( void );
extern void PWM_Generator2_Handler( void );
extern void PWM_Generator3_Handler( void );
extern void SSI0_Handler( void );
extern void SSI1_Handler( void );
extern void SSI2_Handler( void );
extern void SSI3_Handler( void );
extern void PWM_Fault_Handler( void );
extern void ADC0Sequence0_Handler( void );
extern void ADC0Sequence1_Handler( void );
extern void ADC0Sequence2_Handler( void );
extern void ADC0Sequence3_Handler( void );
extern void ADC1Sequence0_Handler( void );
extern void ADC1Sequence1_Handler( void );
extern void ADC1Sequence2_Handler( void );
extern void ADC1Sequence3_Handler( void );
extern void WatchdogTimers0_1_Handler( void );
extern void AnalogComparator0_Handler( void );
extern void AnalogComparator1_Handler( void );
extern void AnalogComparator2_Handler( void );
extern void QEI0_Handler( void );
extern void FlashMemoryControl_Handler( void );
extern void SystemControl_Handler( void );
extern void CAN0_Handler( void );
extern void CAN1_Handler( void );
extern void Ethernet_Handler( void );
extern void HIB_Handler( void );
extern void USB_Handler( void );
extern void DMA0_Software_Handler( void );
extern void DMA0_Error_Handler( void );
extern void EPI0_Handler( void );
extern void FPU_Imprecise_Handler( void );
extern void Tamper_Handler( void );

typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

// The vector table is normally located at address 0.
// When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
// If you need to define interrupt service routines,
// make a copy of this file and include it in your project.
// The name "__vector_table" has special meaning for C-SPY, which
// is where to find the SP start value.
// If vector table is not located at address 0, the user has to initialize
// the  NVIC vector table register (VTOR) before using interrupts.


#pragma location = ".intvec"
const intvec_elem __vector_table[] =
{
  { .__ptr = __sfe( "CSTACK" ) },
  __iar_program_start,

  NMI_Handler,
  HardFault_Handler,
  MemManage_Handler,
  BusFault_Handler,
  UsageFault_Handler,
  0,
  0,
  0,
  0,
  SVC_Handler,
  DebugMon_Handler,
  0,
  PendSV_Handler,
  SysTick_Handler,
  GPIOA_Handler,
  GPIOB_Handler,
  GPIOC_Handler,
  GPIOD_Handler,
  GPIOE_Handler,
  UART0_Handler,
  UART1_Handler,
  SSI0_Handler,
  I2C0_Handler,
  PWM_Fault_Handler,
  PWM_Generator0_Handler,
  PWM_Generator1_Handler,
  PWM_Generator2_Handler,
  QEI0_Handler,
  ADC0Sequence0_Handler,
  ADC0Sequence1_Handler,
  ADC0Sequence2_Handler,
  ADC0Sequence3_Handler,
  WatchdogTimers0_1_Handler,
  Timer0A_Handler,
  Timer0B_Handler,
  Timer1A_Handler,
  Timer1B_Handler,
  Timer2A_Handler,
  Timer2B_Handler,
  AnalogComparator0_Handler,
  AnalogComparator1_Handler,
  AnalogComparator2_Handler,
  SystemControl_Handler,
  FlashMemoryControl_Handler,
  GPIOF_Handler,
  GPIOG_Handler,
  GPIOH_Handler,
  UART2_Handler,
  SSI1_Handler,
  Timer3A_Handler,
  Timer3B_Handler,
  I2C1_Handler,
  CAN0_Handler,
  CAN1_Handler,
  Ethernet_Handler,
  HIB_Handler,
  USB_Handler,
  PWM_Generator3_Handler,
  DMA0_Software_Handler,
  DMA0_Error_Handler,
  ADC1Sequence1_Handler,
  ADC1Sequence2_Handler,
  ADC1Sequence3_Handler,
  EPI0_Handler,
  GPIOJ_Handler,
  GPIOK_Handler,
  GPIOL_Handler,
  SSI2_Handler,
  SSI3_Handler,
  UART3_Handler,
  UART4_Handler,
  UART5_Handler,
  UART6_Handler,
  UART7_Handler,
  I2C2_Handler,
  I2C3_Handler,
  Timer4A_Handler,
  Timer4B_Handler,
  Timer5A_Handler,
  Timer5B_Handler,
  FPU_Imprecise_Handler,
  0,
  0,
  I2C4_Handler,
  I2C5_Handler,
  GPIOM_Handler,
  GPION_Handler,
  0,
  Tamper_Handler,
  GPIOP0_Handler,
  GPIOP1_Handler,
  GPIOP2_Handler,
  GPIOP3_Handler,
  GPIOP4_Handler,
  GPIOP5_Handler,
  GPIOP6_Handler,
  GPIOP7_Handler,
  GPIOQ0_Handler,
  GPIOQ1_Handler,
  GPIOQ2_Handler,
  GPIOQ3_Handler,
  GPIOQ4_Handler,
  GPIOQ5_Handler,
  GPIOQ7_Handler,
  0,
  0,
  0,
  0,
  0,
  0,
  Timer6A_Handler,
  Timer6B_Handler,
  Timer7A_Handler,
  Timer7B_Handler,
  I2C6_Handler,
  I2C7_Handler,
  0,
  0,
  0,
  0,
  0,
  I2C8_Handler,
  I2C9_Handler,
  0,
  0,
  0
};

#pragma call_graph_root = "interrupt"
__weak void NMI_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void HardFault_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void MemManage_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void BusFault_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UsageFault_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SVC_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void DebugMon_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void PendSV_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SysTick_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOA_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOB_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOC_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOD_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOE_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SSI0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void PWM_Fault_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void PWM_Generator0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void PWM_Generator1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void PWM_Generator2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void QEI0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC0Sequence0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC0Sequence1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC0Sequence2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC0Sequence3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void WatchdogTimers0_1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer0A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer0B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer1A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer1B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer2A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer2B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void AnalogComparator0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void AnalogComparator1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void AnalogComparator2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SystemControl_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void FlashMemoryControl_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOF_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOG_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOH_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SSI1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer3A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer3B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void CAN0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void CAN1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Ethernet_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void HIB_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void USB_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void PWM_Generator3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void DMA_Software_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void DMA_Error_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC1Sequence0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC1Sequence1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC1Sequence2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void ADC1Sequence3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void EPI0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOJ_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOK_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOL_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SSI2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void SSI3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART4_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART5_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART6_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void UART7_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer4A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer4B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer5A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer5B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void FPU_Imprecise_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C4_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C5_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOM_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPION_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Tamper_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP4_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP5_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP6_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOP7_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ0_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ1_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ2_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ3_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ4_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ5_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ6_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void GPIOQ7_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer6A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer6B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer7A_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void Timer7B_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C6_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C7_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C8_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void I2C9_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void DMA0_Software_Handler( void ) { while (1) {} }

#pragma call_graph_root = "interrupt"
__weak void DMA0_Error_Handler( void ) { while (1) {} }

void __cmain( void );
__weak void __iar_init_core( void );
__weak void __iar_init_vfp( void );

#pragma required=__vector_table
void __iar_program_start( void )
{
  __iar_init_core();
  __iar_init_vfp();
  __cmain();
}
