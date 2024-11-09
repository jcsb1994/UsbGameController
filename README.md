# UsbGameController

This repo contains source code and CAD files (step 3D files and PCB gerber files) for assembling a 3D-printed HID joystick/keyboard based on the Arduino framework.

- [UsbGameController](#usbgamecontroller)
  - [Hardware setup](#hardware-setup)
    - [Materials](#materials)
  - [Software setup](#software-setup)
    - [Requirements](#requirements)
    - [Build](#build)


## Hardware setup

The gerber files are provided so that they can be sent to any PCB manufacture as-is.

**Note:** The PCB was made with Altium Circuit Maker Pro, which is being deprecated. The CircuitMaker project files are not in the repo. The PCB will need to be designed again in KiCad.

The step files are provided so that they can be exported as STL and sliced and 3D-printed, or edited on any CAD.

![Alt text](GameController_Steps\Render.PNG)

### Materials
The BOM is available at `GameController_PCB\BOM.xlsx`. Note that resistor values are not specified, use standard pull-up and led resistance values.

The main PCB mounts a 24-pin Arduino micro, which is based on the Atmega32u4, as it supports USB HID.

## Software setup

### Requirements

- VS Code
- PlatformIO extension
  
### Build
The project was built with PlatformIO on VS Code. 

to build, open the subfolder `GameController_Firmware` and build the project there using the PIO API. Simply plug in the Arduino micro to flash the program.