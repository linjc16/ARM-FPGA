/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "LCD1602_Qsys.h"
#include "system.h"
unsigned char LCD_Data1[8] = {"Hello"};
int main()
{
  //printf("Hello from Nios II!\n");
  LCD_Disp(1,0,LCD_Data1,5);
  while(1)
  {
  }

  return 0;
}
