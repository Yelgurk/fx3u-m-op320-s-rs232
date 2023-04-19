## PLC FX3U + HMI OP320 + Arduino


[TOC]

###About project
This bundle is an equipment managment solution based Arduino framework. FX3U is the master in this project, and OP320 acts as the slave using modbus protocol + rs232 interface.

The main task was to write a firmware for the existing in stock PLC FX3U, the functionality of which should contain the following:
- full pasteurization cycle program;
- control of heating, mixing separately;
- liquid dispensing with the blowgun;
- linking the pasteurization program to the alarm (self-start on time).
- other features displaying status.

The above tasks were separated and implemented as the following simple commands:
- control of the relay connected to the (onward crct) dispensing pump (fluid dispensing);
- crct mixer;
- crct water jacket pump;
- crct heating elements;
- reading a digital/analog signal from (onward rs) a STOP button;
- rs water jacket sensor;
- rs blowgun sensor;
- rs battery charge;
- rs connection to 380 volt (relay in the switchboard);
- real-time clock;
- alarm;
- saving to eeprom;
- FX3U and OP320 communication by modbus.

The task was given two weeks to complete + no experience in developing for controllers before.

### Firmware
The project includes 6 main files (except main.cpp):
- ee24c64 .hpp/.cpp;
- baseNode .hpp/.cpp;
- dataBase .hpp/.cpp;
- global .hpp;
- ioMonitor .hpp/.cpp;
- plcCore .hpp/.cpp;

#####1. EE24C64 (based on [SlowSoftI2CMaster](https://github.com/felias-fogg/SlowSoftI2CMaster))
This class implements functionality to initialize, read and write to the eeprom.

#####2. BaseNode
This class is an independent unit, which stores some actual value, HEX-address to display the value on the HMI OP320 panel, HEX-address to save the value to EEPROM and functions to work with OP320 via modbus and save to EEPROM accordingly.

#####3. DataBase <s>, real SQL core, enjoy it</s>
Just an intermediate class where the OP320 variables are initialized (output information to the user), get-set variables involved in the basic algorithms of the controller, command variables (commands from OP320 when user presses the buttons).

#####4. Global
A class initializing EE24C64 and modbus (about modbus will follow) as static instances to address OP320 and read/write to flash from any part of the code.

#####5. IOMonitor
Acts as a dispatcher for writing and reading signals. Implemented a small correspondent functionality with the physical FX3U controller to enable/disable relays and read signals accordingly.

#####6. PLCCore (include [STM32duino RTC](https://github.com/stm32duino/STM32RTC), [TaskManagerIO](https://github.com/davetcc/TaskManagerIO))
This class defines the basic algorithms for the behavior of the controller when external signals are received, the OP320 controller commands. The functions are implemented:
- setting the clock at startup;
- changing the clock;
- setting of alarm clock;
- setting and saving default parameters, if at startup they are not found in EEPROM;
- Displaying information on OP320;
- reading and executing commands from OP320;
- applying new settings (reading from OP320 and saving to EEPROM);
- running the blowgun;
- starting mixer;
- start of heaters;
- pasteurization algorithm;
- the process of holding the temperature of the barrel contents;
- rewriting and changing the pointer to a new eeprom sector when 60k write cycles are reached.

#####<s>7.</s> Main (include [ModbusMaster](https://github.com/4-20ma/ModbusMaster))
<s>You already know what main is.</s> This is where all libraries are included, all necessary variables are initialized, modbus, serial (for modbus) is configured, the called functions are set and the task manager is started.

#####Report
Before I started working with the firmware I had to separately solder a lir1220 type battery holder.
![](https://github.com/Yelgurk/fx3u-m-op320-s-rs232/blob/main/readme_gif/PLC_preparing.gif)

After preparing the controllers you can start flashing.
Start with the OP320 panel. Open the OP Series Edit Tool (in my case version 8.0, like the controller itself), open my project and flash the panel.
![](https://github.com/Yelgurk/fx3u-m-op320-s-rs232/blob/main/readme_gif/OP320_demo.gif)

When you have flashed and made sure that the HMI panel works, then proceed to the final part - the PLC firmware.
The project is built with Platformio in VS Code. The controller is flashed via SWD with ST-link. Everything you need to configure is in platformio.ini. Set it up, flash it, test it.
![](https://github.com/Yelgurk/fx3u-m-op320-s-rs232/blob/main/readme_gif/OP320_PLC.gif)

###Issues
Before summarizing, it is necessary to confess and share the obvious problems of the project.
Before that, I want to note that for more than 2 years, since I graduated from college, I have not been involved in programming at all. The reason for that was compulsory military service in my country. This is the first program in 3 years, which was written in a very tight schedule and not on my specifics, because in general I am a .NET developer and I've never touched embedded development before - my first experience.
Enough excuses, forward to the bugs. To infinity and beyond!
The main problems:
- OP320 is Slave, which greatly complicates the exchange of data, because you have to do for each byte of information separate polling from FX3U;
- this version of the firmware was in development when the controller had no battery, which makes RTC library almost useless;
- due to tight deadlines and forgotten (and in embedded, not even available) experience, the code turned out to be unreadable, with crutches and a lot of abstractions that could be abandoned, which became obvious to me only at the end of the development.

###Summarizing
For the most part, the project performs its basic functions and has already been run-in on one of the equipment. Regarding the project some feedback have been received, which will be taken into consideration and implemented in future. This project is uploaded more for demonstration purposes.
This repository will not be supported, because for these PLC + HMI will be written new firmware as OP320 Master and FX3U Slave, which completely changes the concept. For this reason, I do not think it is logical to create new branches.

Thank you for your attention