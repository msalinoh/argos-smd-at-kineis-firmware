This is package is releasing Kinéis SW library with a demo app on a STM32WL55xx-based HW reference
design.

By default this package is built for KRD_MP HW. Refer to "How to build" section later for details.


Two applications tasks are available in KNS_APP files (kns_app.h):
* A standalone app (\ref KNS_APP_stdalone) is sending one message each 60s.
* a GUI app (\ref KNS_APP_gui) is dedicated to communicate with the Kinéis Device Interface (KDI)
through AT commands (\ref mgr_at_cmd_page) over an UART link (9600 bauds, 8, N, 1).

Those application needs some configurations from user such as credentials (ID, ADDR, Secret key)
and radio configuration (UL frequency and modulation). Refer to "getting started" section below for
details.



You can choose the application through the compilation flag in main.c (comment / uncomment
USE_STDALONE_APP / USE_GUI_APP). In main.c, the expected app is then registered in OS through a
"register" API. Queues are also created and declared in main function.

\note The Kinéis stack task (\ref KNS_MAC_task) is also registered (cf kns_mac.h).



The OS embedded in this package is the Kineis baremetal OS, which is a very minimalist OS.
Behind the "queue create" APIs and "task register" APIs, all is actually statically declared in
the kns_q_conf.c, kns_q_conf.h, kns_os_conf.h files (cf \ref Kineis/Extdep/Conf folder).



In case of GUI APP, the format of an AT command is like "<pattern>=?\r\n" (e.g. "AT+PING=?\r\n") to
get information or "<pattern>=<params>\r\n" (e.g. "AT+TX=<user_data>\r\n") to set information. <br>
The list of supported AT commands is available through \ref cas_atcmd_list_array.



By the end, those applications shows how to use the Kinéis stack library which currently exposes the
`MAC` layer for:

* TX, RX, TRX actions and notifications (kns_mac_evt.h)
* configuration service (kns_cfg.h).

# Doc

Documentation folder is containing doxygen auto-generated documentation. Read html documentation by
opening `Kineis/doc/html/index.html` file in a web browser. 

# Getting started

## Credentials

This kineis stack needs integrator/users credentials (ID, address, secret key) to run properly. 

\attention Please contact Kinéis to get your own credentials file.

\attention Credentials are delivered by Kineis after certificication of your device.  
\attention Once credentials are delivered by Kineis, it becomes your responsability.

It is up to the integrator to store it as per its own security strategy. The kineis stack is just
requesting them through APIs called MCU-NVM wrapper (mcu_nvm.c):

* "get device ID" and "get device address" are said not to be critical from a security point of
view. Thus it remains visible when the API is called
* "AES init" and "AES encrypt" are used to build the frame which is transmitted over the air.
\note Here Kineis stack never knows the secret key.

In this package, the credentials are just hardcoded in the code, meaning clearly stored in
RAM/FLASH without any protections. One can change the code of these wrappers to fit its security
strategy (e.g. set the secret key directly in some AES HW module or secure element). 

## Radio configuration

This FW package needs some radio configuration (transmission frequency range, transmission
modulation, ...) to run properly.

\attention Please contact Kinéis to get your own radio configuration binary file when you will
deploy your devices.

It is up to you to handle how to store those informations in the devices. The kineis stack is just
requesting them through one API. 

In this package, the configuration binary file is just hardcoded in the code as a table of bytes.
You will get several examples of it. you can comment, uncomment the one you want.

\note In case of GUI APP, it is also possible to change the radio configuration through the AT
command "AT+RCONF" (\ref bMGR_AT_CMD_RCONF_cmd)

All Kineis stack needs is to be able to get the radio configuration at initialisation.
In case user application wants to change the radio configuration on the fly, Kineis stack must
stopped and restarted.

# Package content

All Kinéis related informations are located in `Kineis` folder. Typical source tree looks like
below.
* **Extdep** folder contains external dependencies of the stack. So far, it is about:
    * **MGR_LOG** The logging manager (mgr_log.h) which may be used by the library to log important
  informations during execution.
  \note So far, this logger is also used by the application software described later below.
    * **Mcu** contains low level hardware drivers (called wrappers) used by the Kinéis SW library.
  This is about AES (mcu_aes.c), credentials (mcu_nvm.c), extra drivers such as PA (mcu_misc.c).
  User is expected to implement its own version of thoses wrappers as per his platform needs.
  By default, this package is delivering a simple implementation of each wrapper. <br>
    * **Conf** contains static configurations used by the entire project. As mentionned earlier,
  you will find here all things concerning OS task and queue definitions. <br>
* **App** folder contains:
    * The **main** file (kns_app.c) for the applications mentionned earlier. The GUI app relies on
    an AT command manager.
    * **Kineis_os** contains the Kineis baremetal OS used to register queues and tasks. <br>
    * **Managers/MGR_AT_CMD** contains the AT command manager used by the GUI application. <br>
    It is about parsing a ASCII data stream coming from UART driver. <br>
    Entry point of this manager is mgr_at_cmd.h. <br>
    mgr_at_cmd_list.h is the entry files referencing the
    AT cmds supported by this firmware. Then, the other mgr_at_cmd_list files refers to subsets
    of AT cmds per functionnalities (such as general, user_data). General subset is to get ID or
    firmware version. user data subset is to transmit data over the air.
    * **Libs/STRUTIL** and **Libs/USERDATA** are pure sw libraries (no HW dependencies) used in AT
    cmd manager.
    * **Mcu** contains low level hardware drivers (called wrappers) used by the components above.<br>
    Typically, it contains the wrapper used to handle a terminal/console to post/receive AT cmds
    (mcu_at_console.h).
    \note So far, this at_cmd console wrapper is derived from STM32_HAL_UART drivers.
    It is a bit changed to handle a continuous ACII data stream. It may be optimized as your
    convenience or based on STM LL drivers instead. Each time a character is received on UART, it
    is invoking a stream-parsing callback (so far \ref MGR_AT_CMD_parseStreamCb in this package).
* **version.txt** file contains references to the software version of the package

```
.
├── App
│   ├── Kineis_os
│   │   ├── KNS_OS
│   │   │   ├── Inc
│   │   │   │   └── kns_os.h
│   │   │   └── Src
│   │   │       └── kns_os.c
│   │   └── KNS_Q
│   │       ├── Inc
│   │       │   └── kns_q.h
│   ├── kns_app.c
│   ├── kns_app.h
│   ├── Libs
│   │   ├── STRUTIL
│   │   │   ├── Inc
│   │   │   │   └── strutil_lib.h
│   │   │   └── Src
│   │   │       └── strutil_lib.c
│   │   └── USERDATA
│   │       ├── Inc
│   │       │   └── user_data.h
│   │       └── Src
│   │           └── user_data.c
│   ├── Managers
│   │   ├── MGR_AT_CMD
│   │   │   ├── Inc
│   │   │   │   ├── mgr_at_cmd_common.h
│   │   │   │   ├── mgr_at_cmd.h
│   │   │   │   ├── mgr_at_cmd_list_general.h
│   │   │   │   ├── mgr_at_cmd_list.h
│   │   │   │   ├── mgr_at_cmd_list_mac.h
│   │   │   │   ├── mgr_at_cmd_list_previpass.h
│   │   │   │   └── mgr_at_cmd_list_user_data.h
│   │   │   └── Src
│   │   │       ├── mgr_at_cmd.c
│   │   │       ├── mgr_at_cmd_common.c
│   │   │       ├── mgr_at_cmd_list.c
│   │   │       ├── mgr_at_cmd_list_general.c
│   │   │       ├── mgr_at_cmd_list_mac.c
│   │   │       ├── mgr_at_cmd_list_previpass.c
│   │   │       └── mgr_at_cmd_list_user_data.c
│   │   └── MGR_LPM
│   │       ├── Inc
│   │       │   └── mgr_lpm.h
│   │       └── Src
│   │           └── mgr_lpm.c
│   ├── Mcu
│   │   ├── Inc
│   │   │   └── mcu_at_console.h
│   │   └── Src
│   │       └── mcu_at_console.c
│   └── README.md
├── doc
│   └── html
│       ├── index.html
├── doxy_warn_log_file.txt
├── Extdep
│   ├── Conf
│   │   ├── kineis_sw_conf.h
│   │   ├── kns_os_conf.h
│   │   ├── kns_q_conf.c
│   │   ├── kns_q_conf.h
│   │   └── mgr_log_conf.h
│   ├── Mcu
│   │   ├── Inc
│   │   │   ├── aes.h
│   │   │   ├── mcu_aes.h
│   │   │   ├── mcu_misc.h
│   │   │   └── mcu_nvm.h
│   │   └── Src
│   │       ├── aes.c
│   │       ├── mcu_aes.c
│   │       ├── mcu_misc.c
│   │       └── mcu_nvm.c
│   └── MGR_LOG
│       ├── Inc
│       │   └── mgr_log.h
│       └── Src
│           └── mgr_log.c
├── Lib
│   ├── build_info.c
│   ├── build_info.h
│   ├── kns_cfg.h
│   ├── kns_mac_evt.h
│   ├── kns_mac.h
│   ├── kns_srvc_common.h
│   ├── kns_types.h
│   └── libkineis.a
├── README.md
└── version.txt

```

# How to build

This project is built with makefiles.

## From STM32CubeIDE

Install STM32CubeIDE and open project present in the Kinéis package.
Toolchain (GCC, GNU Make) should direcly come up with your installation.

Present package was tested with following STM32CubeIDE on windows 10:
```
STM32CubeIDE
Version: 1.10.1
Build: 12716_20220707_0928 (UTC)
OS: Windows 10, v.10.0, x86_64 / win32
Java vendor: Eclipse Adoptium
Java runtime version: 11.0.14.1+1
Java version: 11.0.14.1
```

Present package was tested with following STM32CubeIDE on Linux:
```
STM32CubeIDE
Version: 1.10.1
Build: 12716_20220707_0928 (UTC)
OS: Linux, v.5.15.0-52-generic, x86_64 / gtk 3.24.20
Java vendor: Eclipse Adoptium
Java runtime version: 11.0.14.1+1
Java version: 11.0.14.1
```

Once STM32CubeIDE is properly installed:

* Go to following menu: `File > Open Projects from File System...`
* In popped-up windows
    * Click on `Archive...` button in a way to select Kinéis .tar .gz archive of
    this package (should be named something like `krd_fw_package_.*.tar.gz`.
    * Click on `Open`
    * Let STM32CubeIDE parse the package once `Folder` sub-window shows items
    * In this `Folder` sub-windows, select only the found `Eclipse project`
    * Click on `Finish`. `krd_fw` project should open and be available in `Project Explorer` panel
* In `Project Explorer` panel, you can select the `krd_fw` project. As the project is already built
in the package, you need to clean it before build if you want to rebuild it

### Build option for KRD_MP

Right click on project and select `Clean Project` to clean all. In build console, you should see
something like:
```
make clean 
rm -fR build
```

Right click on project and select `Build Project` to build. you should see something like:
```
make all 
mkdir build
arm-none-eabi-gcc ...
```

* Binary file `\*.elf` file should be present in `build` folder of the project and ready to be
flashed in KRD_MP HW.

### Build option for KRD_LP

As HW setting are different between KRD_MP and KRD_LP (mcu_misc.c), a specific compilation
flag is used to discrminate both. In the build menu of you IDE, modify build command as following:
```
make all KRD_BOARD=KRD_FW_LP
```

## From terminal

This project was tested and validated with following toolchain
* GNU make         : GNU Make 4.2.1_st_20200221-0903
* GCC arm-none-eabi: gcc version 9.3.1 20200408 (release) (GNU Tools for STM32 9-2020-q2-update.20201001-1621) 

In a way to build same binary from STM32CubeIDE or console whenever you are in windows or linux, it
is recommended to point on same toolchain coming-up with STM32CubeIDE installation. You may need to
update your PATH variable to point on this toolchain.

* Linux: Depending on you IDE/toolchain version, add something like following in your ~/.bashrc
```
export PATH=~/st/stm32cubeide_1.4.0/plugins/com.st.stm32cube.ide.mcu.externaltools.make.linux64_2.0.0.202105311346/tools/bin/:$PATH
export PATH=~/st/stm32cubeide_1.4.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.9-2020-q2-update.linux64_2.0.0.202105311346/tools/bin/:$PATH
```
* Windows: Update System PATH variable as usual.

### Build option for KRD_MP

Once toolchain is correctly available:
* Go to root directory of the package
* Run following command to build for default HW
```
make clean
make
```
or
```
make clean
make KRD_BOARD=KRD_FW_MP
```

* Binary file `*.elf` file should be present in `build` folder of the project and ready to be
flashed in KRD_MP HW.

### Build option for KRD_LP

As HW setting are different between KRD_MP and KRD_LP, a specific compilation flag is used to
discrminate both. Once toolchain is correctly available:
* Go to root directory of the package
* Run following command to build for default HW
```
make clean
make KRD_BOARD=KRD_FW_LP
```

* Binary file `*.elf` file should be present in `build` folder of the project and ready to be
flashed in KRD_LP HW.
