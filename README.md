# MamaMatrix
CP210x USB to UART Bridge VCP Drivers - Silicon Labs
https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads


# Part list
- RGB LED matrix 32x64 P5 (P5 means that the LED pitch is 5mm)
- USB-C cable or 5V power supply (3A minimum)
- PicoDriver 

#### Commercial driver alternative
- [Teensy 4.0 or 4.0 development board](https://www.pjrc.com/teensy/) 
- [SmartLed shield](https://docs.pixelmatix.com/SmartMatrix/) (not strictly necessary but handy to quickly connect the microcontroller)

# Software requirements
[Visual Studio Code](https://code.visualstudio.com)

[PlatformIO](https://platformio.org) (install via VS Code plugin manager)

An UART driver might be necessary for Windows systems:   
[Silicon Labs CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)

Library dependency
[ESP32 HUB75 MatrixPanel-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA)



# Examples
![Text matrix](https://raw.githubusercontent.com/ertdfgcvb/MamaMatrix/master/pics/a6_text_matrix.gif)

__a1_single_pixel__\
Smallest example program that runs directly on the microcontroller.
A few LEDs are activated “manually”.

__a2_single_pixel_animated__\
Another simple example with some moving LEDs.

__a3_serial_rgb_slave__\
A slave program that forwards incoming pixel data from the serial port to the LED panels.\
The code is unoptimized but runs smooth at 60fps on a single matrix and around 30fps on 4 chained matrices.\
The following Processing examples encode some pixels from the canvas (or a render target).

__p1_serial_rgb_send_canvas__\
A simple example that grabs all the pixels from the Processing canvas and sends them to the serial port.

__p2_serial_rgb_send_webcam__\
Same as above but with a live webcam.

__p3_serial_rgb_preview__\
A slightly more structured example with a better (bigger) preview.\
The slave is always configured as a stack of matrices. The master program can be configured to slice the canvas according to the desired configuration.

__p4_serial_rgb_multi_anims__\
A demonstration running several scenes from a single Processing sketch.







