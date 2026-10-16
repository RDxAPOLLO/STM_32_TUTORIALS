#include <stdint.h>

/* ============================================================
 * RCC
 * ============================================================ */

#define RCC_BASE        0x40023800UL

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30UL))

/* ============================================================
 * GPIOA
 * ============================================================ */

#define GPIOA_BASE      0x40020000UL

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_OTYPER    (*(volatile uint32_t *)(GPIOA_BASE + 0x04UL))
#define GPIOA_OSPEEDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x08UL))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))
#define GPIOA_IDR       (*(volatile uint32_t *)(GPIOA_BASE + 0x10UL))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))

/* ============================================================
 * Delay
 * ============================================================ */

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 1000000; i++)
    {
        /* crude software delay */
    }
}

/* ============================================================
 * Main
 * ============================================================ */

int main(void)
{
    /* --------------------------------------------------------
     * 1. Enable GPIOA peripheral clock
     *
     * RCC_AHB1ENR bit 0 = GPIOAEN
     * -------------------------------------------------------- */

    RCC_AHB1ENR |= (1U << 0);


    /* --------------------------------------------------------
     * 2. Configure PA5 as General Purpose Output
     *
     * PA5 uses MODER bits [11:10]
     *
     * 00 = Input
     * 01 = General purpose output
     * 10 = Alternate function
     * 11 = Analog
     * -------------------------------------------------------- */

    GPIOA_MODER &= ~(3U << 10);
    GPIOA_MODER |=  (1U << 10);


    /* --------------------------------------------------------
     * 3. Configure output type
     *
     * OTYPER bit 5
     *
     * 0 = Push-pull
     * 1 = Open-drain
     * -------------------------------------------------------- */

    GPIOA_OTYPER &= ~(1U << 5);


    /* --------------------------------------------------------
     * 4. Configure output speed
     *
     * OSPEEDR bits [11:10]
     *
     * 00 = Low speed
     * 01 = Medium speed
     * 10 = High speed
     * 11 = Very high speed
     *
     * Low speed is completely sufficient for an LED.
     * -------------------------------------------------------- */

    GPIOA_OSPEEDR &= ~(3U << 10);


    /* --------------------------------------------------------
     * 5. No pull-up / pull-down
     *
     * PUPDR bits [11:10]
     *
     * 00 = No pull-up/pull-down
     * -------------------------------------------------------- */

    GPIOA_PUPDR &= ~(3U << 10);


    /* --------------------------------------------------------
     * 6. Infinite loop
     * -------------------------------------------------------- */

    while (1)
    {
        /* Toggle PA5 */

        GPIOA_ODR ^= (1U << 5);

        delay();
    }
}
