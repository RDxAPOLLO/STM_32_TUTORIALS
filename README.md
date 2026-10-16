# STM32 Tutorials

A collection of hands-on STM32 projects covering embedded C programming, GPIO, UART communication, I²C peripherals, interrupts, and PWM generation. This repository documents my learning and practical experiments with STM32 microcontrollers using STM32CubeIDE and the STM32 HAL library.

## 📚 Projects

| Project | Description |
|---|---|
| `hello_world` | Basic STM32 project setup |
| `running_led` | Running LED sequence |
| `led_blink` | LED blinking using GPIO |
| `disco_led_blink` | LED control on an STM32 Discovery board |
| `LED_BLINK_DISCO` | LED blinking experiment on a Discovery board |
| `push_button_switch` | Push-button input and GPIO control |
| `CHECK` | Experimental STM32 project |
| `PWM_GENERATION` | PWM signal generation |
| `PWM_GENERATION_DISCO` | PWM generation on a Discovery board |
| `PWM_WITHOUT_FUNCTION` | PWM implementation without a custom function |
| `PWM_FUNCTION` | PWM implementation using a function |
| `PWM_WITH_SWITCH` | PWM control using a switch |
| `pwm_2_switch` | PWM and switch experiment |
| `uart_2_deivce` | UART communication between two devices |
| `UART_HELLO_WORLD` | Basic UART transmission |
| `UART_` | UART experiment |
| `uart_input` | UART input handling |
| `2_uart_user_input` | UART user-input experiment |
| `UART_RECIEVE_POLLING` | UART reception using polling |
| `TWO_UART` | Working with two UART peripherals |
| `uart_int` | UART communication using interrupts |
| `interrupt_with_flag` | Interrupt handling using a flag |
| `i2c_oled` | OLED display communication over I²C |
| `I2C_WRITE` | I²C write operation |
| `i2c_scanner` | I²C device scanning |

*Project descriptions are based on folder names and may be refined as each project is documented.*

## 🛠️ Tools and Technologies

- **Language:** Embedded C
- **IDE:** STM32CubeIDE
- **Library:** STM32 Hardware Abstraction Layer (HAL)
- **Configuration:** STM32CubeMX / `.ioc` project files
- **Debugging:** ST-LINK
- **Protocols:** UART, I²C
- **Peripherals:** GPIO, timers, PWM, LEDs, push buttons, OLED displays
- **Concepts:** Polling, interrupts, flags, serial communication

## 🚀 Getting Started

1. Install STM32CubeIDE.
2. Clone this repository:

   ```bash
   git clone https://github.com/RDxAPOLLO/STM_32_TUTORIALS.git
   ```

3. Open STM32CubeIDE.
4. Import the required project into the workspace.
5. Open its `.ioc` file, if available, to inspect the peripheral configuration.
6. Build the project and flash it to compatible STM32 hardware.

**Note:** Each project may target a different STM32 board or microcontroller. Check its `.ioc` file, linker script, and project settings before building or flashing.

## 📁 Repository Structure

Each project is maintained in its own directory and may contain:

- `Core/` — application source code and headers
- `Drivers/` — STM32 HAL drivers
- `.ioc` — STM32CubeMX configuration
- Linker script (`.ld`) — memory layout
- IDE project and debug configuration files

The exact contents vary by project.

## 🎯 Goals

- Practise embedded C programming on STM32.
- Understand GPIO, timers, and PWM.
- Learn UART communication using polling and interrupts.
- Interface peripherals over I²C.
- Build a reusable collection of practical embedded systems examples.

## 📌 Status

This repository is a work in progress. Projects and documentation will be updated as new experiments are completed.

## 📄 License

No license has been specified yet. Unless a license is added, the repository should not be assumed to grant permission to reuse, modify, or distribute its contents.

---

**Repository:** [STM_32_TUTORIALS](https://github.com/RDxAPOLLO/STM_32_TUTORIALS)
