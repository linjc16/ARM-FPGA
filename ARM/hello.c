//*****************************************************************************
//
// hello.c - Simple hello world example.
//
// Copyright (c) 2011-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 9453 of the EK-LM4F232 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "driverlib/qei.h"
#include "inc/lm4f232h5qd.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "stdio.h"
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Hello World (hello)</h1>
//!
//! A very simple ``hello world'' example.  It simply displays ``Hello World!''
//! on the display and is a starting point for more complicated applications.
//! This example uses calls to the Stellaris Graphics Library graphics
//! primitives functions to update the display.  For a similar example using
//! widgets, please see ``hello_widget''.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// Print "Hello World!" to the display.
//
//*****************************************************************************
#define ADC0_MAX_VALUE  0x290
#define PWM_PERIOD 500

void select_key(int *key_cnt, int *direction)
{
    if (!(ROM_GPIOPinRead(GPIO_PORTM_BASE, GPIO_PIN_4) && GPIO_PIN_4))
    {
        *key_cnt = 0;
    }
    else if (++(*key_cnt) == 1)
    {
          *direction = !(*direction);
    }
}

int
main(void)
{
	//-----------------�������--------------------
    tContext sContext;//����һ����ͼ����
    tRectangle sRect;//����һ�����νṹ��
    unsigned long ulADC0_Value;
    int direction = 0;
    int width;
    int rpm = 100;
    int key_cnt = 0;
    // ʹ��FPU
    FPUEnable();
    FPULazyStackingEnable();

    // ����ϵͳʱ��Ϊ50MHz (400/2/4=50)
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //--------------------------ADC---------------------------------------
    // ��ʼ��ADC0/PE3
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    // ����ADC�ο���ѹΪ�ⲿ3V
    ROM_ADCReferenceSet(ADC0_BASE, ADC_REF_EXT_3V);
    // ����ADC�ɼ�����
    ROM_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 | ADC_CTL_END | ADC_CTL_IE);
    // ʹ��ADC�ɼ�����
    ROM_ADCSequenceEnable(ADC0_BASE, 0);
    ROM_ADCIntClear(ADC0_BASE, 0);
    ROM_ADCIntEnable(ADC0_BASE, 0);

    //------------------------key--------------------------------
    // ʹ������PORTM
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    // ����PG2Ϊ����ڣ�PM4Ϊ����ڣ�������������
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_4);
    ROM_GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_4,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //-----------------------PWM------------------------------------
    // ʹ��PWMģ�� */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    // ʹ��PWM2��PWM3�������GPIO
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

    // ��PH0/PH1ѡ��ΪPWM��������
    ROM_GPIOPinConfigure(GPIO_PH0_M0PWM0);
    ROM_GPIOPinConfigure(GPIO_PH1_M0PWM1);

    // ��PH0/PH1������PWM���ܵ�����
    ROM_GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);
    ROM_GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_1);

    // PWMʱ�����ã�����Ƶ
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    // ����PWM������1���Ӽ�����
    ROM_PWMGenConfigure(PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN
                        | PWM_GEN_MODE_NO_SYNC);
    // ����PWM������1������
    ROM_PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, PWM_PERIOD);
    // ����PWM2/PWM3�����������
    ROM_PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 0);
    ROM_PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 0);
    // ʹ��PWM2��PWM3�����
    ROM_PWMOutputState(PWM_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true);
    // ʹ��PWM������1
    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    //---------------------------QEI--------------------------------------
    //ʹ��QEIģ��
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    //ʹ��QEI����GPIO
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //��PF0��PF1ѡ��ΪQEI��������
    ROM_GPIOPinConfigure(GPIO_PF0_PHA0);
    ROM_GPIOPinConfigure(GPIO_PF1_PHB0);
    //��PF0��PF1������QEI���ܵ�����
    ROM_GPIOPinTypeQEI(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //QEI���� //ʹ��A,Bͨ����4�����ؼ����ٶ�
    QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B
                             | QEI_CONFIG_RESET_IDX | QEI_CONFIG_QUADRATURE
                             | QEI_CONFIG_NO_SWAP ), 333);
    //ת�ٲ��������ã������ٶȼ������
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, 5000000);
    // ʹ���������������ٶȼ�⹦��
    QEIVelocityEnable(QEI0_BASE);
    //ʹ������������
    QEIEnable(QEI0_BASE);
    //--------------------��ʾ-----------------
    CFAL96x64x16Init();
    GrContextInit(&sContext, &g_sCFAL96x64x16);
    int cnt = 0;
  while(1)
  {

    //--------------------------����ת��---------------------------------
      //---------- ADC ------------
      // �����ɼ�
      ADCProcessorTrigger(ADC0_BASE, 0);
      // �ȴ��ɼ�����
      while(!ADCIntStatus(ADC0_BASE, 0, false)) ;
      // ��ȡ�ɼ����
      ADCSequenceDataGet(ADC0_BASE, 0, &ulADC0_Value);

      select_key(&key_cnt, &direction);

      //----------PWM------------------------
      width = (int) (1.0 * ulADC0_Value / ADC0_MAX_VALUE * PWM_PERIOD);
      if (direction)
      {
          ROM_PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, width);
          ROM_PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 0);
      }
      else
      {
          ROM_PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 0);
          ROM_PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, width);
      }


      //-----------------QEI--------------------
      rpm = QEIVelocityGet(QEI0_BASE);

      char str[7];
      str[0] = rpm/1000 + 48;
      str[1] = (rpm % 1000) /100 + 48;
      str[2] = (rpm % 100 ) / 10 + 48;
      str[3] = rpm % 10+ 48;
      str[4] = 'r';
      str[5] = '/';
      str[6] = 'm';

   	//------------------------��ʾ------------------------------------


    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();

    //
    // Initialize the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);



    cnt = (cnt + 1) % 10000;
    if(!cnt)
    {
    //
    // Fill the top 24 rows of the screen with blue to create the banner.
    //
    sRect.sXMin = 0;
    sRect.sYMin = 0;
    sRect.sXMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.sYMax = 23;
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &sRect);

    //
    // Put a white box around the banner.
    //
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectDraw(&sContext, &sRect);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextFontSet(&sContext, g_pFontCm12);
    GrStringDrawCentered(&sContext, str, 7,
                                   GrContextDpyWidthGet(&sContext) / 2, 10, 0);

    //
    // Say hello using the Computer Modern 40 point font.
    //
    GrContextFontSet(&sContext, g_pFontCm12/*g_pFontFixed6x8*/);
    GrStringDrawCentered(&sContext, "Speed", -1,
                         GrContextDpyWidthGet(&sContext) / 2,
                         ((GrContextDpyHeightGet(&sContext) - 24) / 2) + 24,
                         0);

    //
    // Flush any cached drawing operations.
    //
    GrFlush(&sContext);
    //
    // We are finished. Hang around doing nothing.
    //
    }
    }
}
