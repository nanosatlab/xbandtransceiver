# **Description**

This folder includes the [LMX2572](https://www.ti.com/lit/ds/symlink/lmx2572.pdf?ts=1637912320049&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLMX2572) PLL evaluation board related files. It has been designed in orther to test signal reference generation from a TCXO of 25 MHz that is attached to the developement board. If another clock reference is desired, it is necessary to desolder the 25 MHz TCXO reference and instead the OSCin pin can be used.

# **How to use the test board**

To use the LMX2572 evaluation board you will need a Nucleo-L476RG developement board and the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). 

## Connection diagram

The LMX2572 uses SPI in order to be controlled. The code has been written using the hspi1 peripheral of the Nucleo-L476RG board with the GPIOA9 pin as the CSB:



