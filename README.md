## Project Overview

This project demonstrates how to establish a reliable UART communication link between two PIC16F877A microcontrollers. The system is designed such that:

- **Microcontroller A** transmits a detailed paragraph about the MAX6952 display driver.
- **Microcontroller B** receives the text, counts the occurrences of the characters **‘D’**, **‘E’**, and **‘F’**, and echoes the original text.
- After processing, Microcontroller B sends the counts back to Microcontroller A.
- **Microcontroller A** then uses a bit‐banged interface on PORTB to drive a MAX6952-controlled LED dot matrix display, showing the counts in the format **D=x, E=y, F=z**.

### Key Features

- **UART Communication:** Configured on pins RC6 (TX) and RC7 (RX) for both microcontrollers.
- **Data Processing:** Counts specific characters (‘D’, ‘E’, and ‘F’) during text transmission.
- **Display Integration:** Bit‐banged control of the MAX6952 to display the counts on an LED dot matrix.
- **Simulation:** Validated using Proteus Simulation Software.
- **Embedded Code:** Complete source code for both microcontrollers is provided.

For a comprehensive description of the project’s design, methodology, detailed circuit diagrams, and complete code listings, please refer to the [report.docx](report.docx) file included in this repository.

### Requirements

- **Development Tools:** MPLAB X IDE and XC8 Compiler.
- **Simulation:** Proteus Simulation Tool.
- **Hardware:** PIC16F877A microcontrollers and MAX6952 display driver.

### References

1. [PIC16F877A Datasheet](https://www.microchip.com/en-us/product/PIC16F877A)
2. [MPLAB XC8 C Compiler User’s Guide](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers)
3. [MAX6952 Display Driver Details](https://www.analog.com/en/products/max6952.html)
