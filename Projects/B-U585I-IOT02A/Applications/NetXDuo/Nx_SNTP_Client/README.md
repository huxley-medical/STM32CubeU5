## <b>Nx_SNTP_Client Application Description</b>

This application provides an example of Azure RTOS NetX/NetXDuo stack usage. 

It shows how to develop a NetX SNTP client and connect with an STNP server to get a time update.

The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all NetX resources are created.

 + A <i>NX_PACKET_POOL</i>is allocated

 + A <i>NX_IP</i> instance using that pool is initialized

 + The <i>ARP</i>, <i>ICMP</i>, <i>UDP</i> and <i>TCP</i> protocols are enabled for the <i>NX_IP</i> instance

 + An <i>Sntp Client is created.</i>

 + A <i>DHCP client is created.</i>

The application then creates 2 threads with the same priorities:

 + **AppMainThread** (priority 10, PreemtionThreashold 10) : created with the <i>TX_AUTO_START</i> flag to start automatically.

 + **AppSNTPThread** (priority 10, PreemtionThreashold 10) : created with the <i>TX_DONT_START</i> flag to be started later.

The **AppMainThread** starts and perform the following actions:

  + Starts the DHCP client

  + Waits for the IP address resolution

  + Resumes the **AppSNTPThread**

The **AppSNTPThread**, once started:

  + creates a dns_client with USER_DNS_ADDRESS used as DNS server.

  + initialize SntpClient in Unicast mode and set SNTP_SERVER_NAME predefined in app_netxduo.h
  
  + run SntpClient
  
  + once a valid time update received, time will be displayed on the Hyperterminal and set to RTC
  
  + RTC time will be displayed each second on the Hyperterminal.

####  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal

 + The time update sent by the sntp server is printed on the HyerTerminal

 + Each second, time from RTC is printed on the HyerTerminal and the green LED is toggling.

#### <b>Error behaviors</b>

+ The red LED is toggling to indicate any error that has occurred while the green LED is turned OFF.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

### <b>Notes</b>

default NX_SNTP_CLIENT_MAX_ROOT_DISPERSION and NX_SNTP_CLIENT_MIN_SERVER_STRATUM values in "nx_user.h" may not work for some SNTP servers, they should be tuned for example :

#define NX_SNTP_CLIENT_MAX_ROOT_DISPERSION    500000

#define NX_SNTP_CLIENT_MIN_SERVER_STRATUM     5

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.

 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.S" file too.

 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.

 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).

 - Using dynamic memory allocation requires to apply some changes to the linker file.

   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function, using the "first_unused_memory" argument.
   This requires changes in the linker files to expose this memory location.

    + For EWARM add the following section into the .icf file:
     ```
     place in RAM_region    { last section FREE_MEM };
     ```
    + For MDK-ARM:
    ```
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_initialize_low_level.S to match the memory region being used
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    + For STM32CubeIDE add the following section into the .ld file:
    ``` 
    ._threadx_heap :
      {
         . = ALIGN(8);
         __RAM_segment_used_end__ = .;
         . = . + 64K;
         . = ALIGN(8);
       } >RAM_D1 AT> RAM_D1
    ```

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, WIFI, SNTP, DNS, RTC, MXCHIP, UART

### <b>Hardware and Software environment</b>

 - To use the EMW3080B MXCHIP Wi-Fi module functionality, 2 software components are required:
   1. The module driver running on the STM32 device
   2. The module firmware running on the EMW3080B Wi-Fi module

 - This application uses an updated version of the EMW3080B MXCHIP Wi-Fi module driver V2.3.4.

 - The B-U585I-IOT02A Discovery board Revision D is delivered with the EMW3080B MXCHIP Wi-Fi module firmware V2.1.11;
   to upgrade your board with the required version V2.3.4, please visit [X-WIFI-EMW3080B](https://www.st.com/en/development-tools/x-wifi-emw3080b.html),
   using the `EMW3080update_B-U585I-IOT02A-RevC_V2.3.4_SPI.bin` file under the V2.3.4/SPI folder.

 - Please note that module firmware version V2.1.11 is not backwards compatible with the driver V2.3.4 (the V2.1.11 module firmware is compatible with the driver versions from V2.1.11 to V2.1.13).
 - The module driver is available under [/Drivers/BSP/Components/mx_wifi](../../../../../Drivers/BSP/Components/mx_wifi/), and its version is indicated in the [Release_Notes.html](../../../../../Drivers/BSP/Components/mx_wifi/Release_Notes.html) file.

 - Be aware that some STM32U5 SW packages (for examples X-CUBE-AZURE and X-CUBE-AWS) may continue to use older version of the EMW3080B MXCHIP module firmware;
   thanks to refer to the release notes of each SW package to know the recommended module firmware's version which can be retrieved from this page
   [X-WIFI-EMW3080B](https://www.st.com/en/development-tools/x-wifi-emw3080b.html).

 - This application has been tested with B-U585I-IOT02A (MB1551-U585AI) boards Revision: Rev D01 and can be easily tailored to any other supported device and development board.

 - This application uses USART1 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain

 - On <code> Core/Inc/mx_wifi_conf.h </code> , Edit your Wifi Settings (WIFI_SSID, WIFI_PASSWORD)
 
 - Edit the file app_netxduo.h : define the USER_DNS_ADDRESS and define the SNTP server name to connect to.

 - Rebuild all files and load your image into target memory

 - Run the application