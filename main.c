//Include Libraries
//TEST KEIL
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/timer.c"
// Define variables
#define PORT_LED GPIO_PORTF_BASE
#define LED      GPIO_PIN_2
uint32_t value;
// Timer1 ISR
void Timer1_ISRHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
    if (GPIOPinRead(PORT_LED, LED)==0)
        GPIOPinWrite(PORT_LED,LED,4);
    else
        GPIOPinWrite(PORT_LED,LED,0);

}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_20|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //Enable peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    //Configure peripherals
    //Configure LED
    GPIOPinTypeGPIOOutput(PORT_LED, LED);
    //Configure Timer
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PERIODIC);
    TimerPrescaleSet(TIMER1_BASE, TIMER_B, 200);
    //Calculate Prescaler
    // 10*10^6/Pre = Value
    TimerLoadSet(TIMER1_BASE, TIMER_B, SysCtlClockGet()-1);//50000-1);
    TimerEnable(TIMER1_BASE, TIMER_B);
    //Interrupt configurations
    IntMasterEnable();
    TimerIntRegister(TIMER1_BASE, TIMER_B, Timer1_ISRHandler);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
    while(1)
    {
    }
}
