# **Description**

This folder includes the [LMX2572](https://www.ti.com/lit/ds/symlink/lmx2572.pdf?ts=1637912320049&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLMX2572) PLL evaluation board related files. It has been designed in orther to test signal reference generation from a TCXO of 25 MHz that is attached to the evaluation board. If another clock reference is desired, it is necessary to desolder the 25 MHz TCXO reference and instead the OSCin pin can be used.

# **How to use the test board**

To use the LMX2572 evaluation board you will need a Nucleo-L476RG developement board and the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). It can be programmed either using the oficial developement program [TICS Pro](https://www.ti.com/tool/TICSPRO-SW) from TexasInstruments or using the custom created library.

## Connections

The LMX2572 evaluation board is powered by a single 3.3 V line and can be connected either to the Nucleo-L476RG supply or to an external power supply. It is important to consider the grounding connections. If the computer connected to the Nucleo board is connected to the power line, an additional connection should be added between the ground of the LMX2572 evaluation board and the Nucleo board to make sure that the ground planes are at the same level. If not done, malfunction of the LMX2572 evaluation board may occur.

Nominal current consumption when not powered is around 34 mA, and when powered around 80-90 mA.   

If the RF output is connected to an SA, consider putting at least a 30 dB attenuator in between.

The LMX2572 uses SPI in order to be controlled. The code has been written using the hspi1 peripheral of the Nucleo-L476RG board with the GPIOA9 pin as the CSB:

![image](https://user-images.githubusercontent.com/33161309/143574974-b1bc2b03-1288-45e9-a0ff-e7ff3ddaa5c0.png)

Connections on the Nucleo-L476RG for dummies are (me): green - D5, blak - D11, orange - D12, yellow - D13

## Programming with TICS PRO

You can program the LMX2572 evaluation board using the Texas Instruments GUI following this procedure:
  1. Configure the LMX2572 as deisred using TICS PRO software (note the limitations of the evaluation board)
  2. Export the Register Map as a .txt
  3. Copy the .txt content into an Excel document, it should create two columns: one with the register name and the other with the content 
  4. Add a third column full of commas (,) until the last register not included
  5. Copy the second and third column and replace the contents of lmx2572_configs.h
  6. Compile code and execute

## Programming with custom library 
**This part is not recomended to be used yet because is still in developement phase.**

