# STM32F446ZE Software LED Brightness Control

## Objective

Control LED brightness using two push buttons on the STM32F446ZE without using the hardware PWM peripheral.

- PA5 → Increase brightness
- PB0 → Decrease brightness
- PE9 → LED
- Brightness range → 0 to 10
- Total period → 10 ms
- Frequency → 100 Hz
- No HAL_TIM_PWM_Start()

## GPIO Configuration

### PA5 - Increase Button

- Mode: GPIO Input
- Pull: Pull-up
- Released: GPIO_PIN_SET
- Pressed: GPIO_PIN_RESET

### PB0 - Decrease Button

- Mode: GPIO Input
- Pull: Pull-up
- Released: GPIO_PIN_SET
- Pressed: GPIO_PIN_RESET

### PE9 - LED

- Mode: GPIO Output
- Output type: Push-Pull
- Pull: No Pull
- Speed: Low

## Software PWM Concept

The LED is manually switched ON and OFF:

    LED ON
    delay(on_time)

    LED OFF
    delay(off_time)

The total period is maintained at:

    on_time + off_time = 10 ms

Therefore:

    Frequency = 1 / 0.010
              = 100 Hz

## Brightness Control

A separate brightness variable is used:

    uint32_t brightness = 5;

Brightness is limited to:

    0 to 10

The timing values are calculated as:

    on_time = brightness;
    off_time = 10 - brightness;

This guarantees:

    on_time + off_time = 10 ms

## Brightness Levels

| Brightness | ON Time | OFF Time | Duty Cycle |
|------------|---------|----------|------------|
| 0          | 0 ms    | 10 ms    | 0%         |
| 1          | 1 ms    | 9 ms     | 10%        |
| 2          | 2 ms    | 8 ms     | 20%        |
| 3          | 3 ms    | 7 ms     | 30%        |
| 4          | 4 ms    | 6 ms     | 40%        |
| 5          | 5 ms    | 5 ms     | 50%        |
| 6          | 6 ms    | 4 ms     | 60%        |
| 7          | 7 ms    | 3 ms     | 70%        |
| 8          | 8 ms    | 2 ms     | 80%        |
| 9          | 9 ms    | 1 ms     | 90%        |
| 10         | 10 ms   | 0 ms     | 100%       |

## Increase Brightness

PA5 is used to increase the brightness.

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
    {
        if (brightness < 10)
        {
            brightness++;
        }
    }

The sequence is:

    0 → 1 → 2 → ... → 9 → 10

The upper limit prevents the brightness value from exceeding 10.

## Decrease Brightness

PB0 is used to decrease the brightness.

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
        if (brightness > 0)
        {
            brightness--;
        }
    }

The sequence is:

    10 → 9 → 8 → ... → 1 → 0

The lower limit prevents the brightness value from going below 0.

## Why Limits Are Required

The variables are declared as uint32_t.

An unsigned integer cannot represent negative values.

For example:

    uint32_t value = 0;
    value--;

The result does not become -1.

It wraps around to:

    4294967295

Therefore, the brightness value must be restricted to the range:

    0 ≤ brightness ≤ 10

## Button Release Logic

After detecting a button press, the program waits until the button is released:

    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
    }

This prevents one long button press from continuously changing the brightness.

The same method is used for PB0.

## LED Control

LED ON:

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);

LED OFF:

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);

The timing is:

    if (on_time > 0)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_Delay(on_time);
    }

    if (off_time > 0)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_Delay(off_time);
    }

## Final Code

    uint32_t brightness = 5;
    uint32_t on_time;
    uint32_t off_time;

    while (1)
    {
        /* INCREASE BRIGHTNESS */
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
        {
            if (brightness < 10)
            {
                brightness++;
            }

            while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)
            {
                HAL_Delay(10);
            }
        }

        /* DECREASE BRIGHTNESS */
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            if (brightness > 0)
            {
                brightness--;
            }

            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
            {
                HAL_Delay(10);
            }
        }

        /* CALCULATE ON/OFF TIME */
        on_time = brightness;
        off_time = 10 - brightness;

        /* LED ON */
        if (on_time > 0)
        {
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
            HAL_Delay(on_time);
        }

        /* LED OFF */
        if (off_time > 0)
        {
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
            HAL_Delay(off_time);
        }
    }

## Working Principle

Increase button:

    PA5 pressed
        ↓
    brightness++
        ↓
    ON time increases
        ↓
    OFF time decreases
        ↓
    LED appears brighter

Decrease button:

    PB0 pressed
        ↓
    brightness--
        ↓
    ON time decreases
        ↓
    OFF time increases
        ↓
    LED appears dimmer

## Hardware PWM vs Software Method

### Hardware PWM

The STM32 timer peripheral generates the PWM waveform.

Example:

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

### Software Method Used Here

The CPU directly controls the GPIO:

    GPIO SET
    ↓
    HAL_Delay(on_time)
    ↓
    GPIO RESET
    ↓
    HAL_Delay(off_time)

No hardware PWM peripheral is used.

## Key Concepts

- GPIO input and output
- Pull-up resistors
- Button state detection
- GPIO control
- Software timing
- ON/OFF time manipulation
- Duty cycle
- PWM frequency
- Unsigned integer limits
- Button release handling
- Software-generated PWM