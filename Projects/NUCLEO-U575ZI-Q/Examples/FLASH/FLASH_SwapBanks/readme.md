## <b>FLASH_SwapBanks Example Description</b>



### <b> Example Description </b>

This guide takes you through the configuration steps to program the internal Flash memory banks 1 and 2, and to swap between them using the FLASH HAL API.

This project is composed of two sub-projects:

  -  FLASH_SwapBanks_Bank1.
  -  FLASH_SwapBanks_Bank2.

The LEDs on the NUCLEO-U575ZI-Q board can be used to monitor the swap status.

Pressing the USER push button initiates the swap between the two banks:

  - If program is executing from bank1 is selected, LED1 toggles.
  - If program is executing from bank2 is selected, LED2 toggles.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :

##### <b>note</b>
      Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

##### <b>note </b>
       The example need to ensure that the SysTick time base is always set to 1 millisecond
       to have correct HAL operation.

##### <b>note</b>
       After executing this example , the SWAP_BANK option byte will be set ,
       Use STM32CubeProgrammer to change this option byte back to "Not swapped" if needed.


### <b> Keywords </b>

Memory, FLASH, Swap, Bank, Dual bank, Program, Sector, LED

### <b> Directory contents </b>

  - FLASH/FLASH_SwapBanks/Inc/stm32u5xx_hal_conf.h        HAL Configuration file
  - FLASH/FLASH_SwapBanks/Inc/stm32u5xx_nucleo_conf.h     STM32U5xx_Nuleo board configuration file
  - FLASH/FLASH_SwapBanks/Inc/stm32u5xx_it.h              Header for stm32u5xx_it.c
  - FLASH/FLASH_SwapBanks/Inc/main.h                      Header for main.c module
  - FLASH/FLASH_SwapBanks/Src/stm32u5xx_it.c              Interrupt handlers
  - FLASH/FLASH_SwapBanks/Src/main.c                      Main program
  - FLASH/FLASH_SwapBanks/Src/system_stm32u5xx.c          STM32U5xx system clock configuration file

### <b> Hardware and Software environment </b>

  - This example runs on STM32U575 devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b> How to use it ? </b>

In order to make the program work, you must do the following :

  - Open your preferred toolchain.
  - Rebuild all files.
  - Load the binary file generated with FLASH_SwapBanks_Bank2 sub-project into
    the BANK2 at address 0x08100000 using STM32CubeProgrammer.
  - Set the FLASH_SwapBanks_Bank1 sub-project as Active Project and run it, this project will be loaded
    in the bank1 of the flash at the address 0x08000000


<b>STM32CubeIDE</b>
In order to program the FLASH_SwapBanks_Bank1 and FLASH_SwapBanks_Bank2 sub-projects into 0x8000000 and 0x8100000 using STM32CubeIDE, you must do the following steps:
 - Rebuild all files
 - Set the FLASH_SwapBanks_Bank1 sub-project and select “Debug configuration”
 - Double click on “STM32 Cortex-M C/C++ Application”
 - Select  “Startup” >  “Add” >
 - Select the FLASH_SwapBanks_Bank2 sub-project
     - Uncheck Perform build option
     - In Use download offset(hex): Add 0x100000 to actually load the generated (.elf) file into 0x8100000
 - Click Debug/Run to debug/run the example
   STM32CubeIDE loses debug connection when the application toggles the SWAP_BANK option bit. Example can be compiled and executed on target, but debug does not work.
   Target is not responding, retrying...
   Target is not responding, retrying...
   Shutting down...