
#include <atom.h>
#include <uart.h>
#include <atomtimer.h>
#include <stm8s.h>
#include <stm8s_gpio.h>
#include <pwm.h>
#include <key.h>
#include <eeprom.h>
#include <gui.h>
// #include <oled.h>
#include "oled_ssd1306.h"
#include "atomport-private.h"
#include "hw_setup.h"
/* Constants */

/*
 * Idle thread stack size
 *
 * This needs to be large enough to handle any interrupt handlers
 * and callbacks called by interrupt handlers (e.g. user-created
 * timer callbacks) as well as the saving of all context when
 * switching away from this thread.
 *
 * In this case, the idle stack is allocated on the BSS via the
 * idle_thread_stack[] byte array.
 */
#define IDLE_STACK_SIZE_BYTES 128

/*
 * Main thread stack size
 *
 * Note that this is not a required OS kernel thread - you will replace
 * this with your own application thread.
 *
 * In this case the Main thread is responsible for calling out to the
 * test routines. Once a test routine has finished, the test status is
 * printed out on the UART and the thread remains running in a loop
 * flashing a LED.
 *
 * The Main thread stack generally needs to be larger than the idle
 * thread stack, as not only does it need to store interrupt handler
 * stack saves and context switch saves, but the application main thread
 * will generally be carrying out more nested function calls and require
 * stack for application code local variables etc.
 *
 * With all OS tests implemented to date on the STM8, the Main thread
 * stack has not exceeded 256 bytes. To allow all tests to run we set
 * a minimum main thread stack size of 204 bytes. This may increase in
 * future as the codebase changes but for the time being is enough to
 * cope with all of the automated tests.
 */
#define MAIN_STACK_SIZE_BYTES 128
#define SCAN_STACK_SIZE_BYTES 128

/*
 * Startup code stack
 *
 * Some stack space is required at initial startup for running the main()
 * routine. This stack space is only temporarily required at first bootup
 * and is no longer required as soon as the OS is started. By default
 * Cosmic sets this to the top of RAM and it grows down from there.
 *
 * Because we only need this temporarily you may reuse the area once the
 * OS is started, and are free to use some area other than the top of RAM.
 * For convenience we just use the default region here.
 */

/* Linker-provided startup stack location (usually top of RAM) */
extern int _stack;

/* Local data */

/* Application threads' TCBs */
static ATOM_TCB main_tcb;
/* Application threads' TCBs */
static ATOM_TCB at_thread;

/* Main thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t main_thread_stack[MAIN_STACK_SIZE_BYTES];

NEAR static uint8_t at_thread_stack[SCAN_STACK_SIZE_BYTES];
/* Idle thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];

/* Forward declarations */
static void system_status(uint32_t param);
static void at_thread_func(uint32_t param);

void idel_hook_system_status();

/**
 * \b main
 *
 * Program entry point.
 *
 * Sets up the STM8 hardware resources (system tick timer interrupt) necessary
 * for the OS to be started. Creates an application thread and starts the OS.
 *
 * If the compiler supports it, stack space can be saved by preventing
 * the function from saving registers on entry. This is because we
 * are called directly by the C startup assembler, and know that we will
 * never return from here. The NO_REG_SAVE macro is used to denote such
 * functions in a compiler-agnostic way, though not all compilers support it.
 *
 */
NO_REG_SAVE void main(void)
{
    int8_t status;

    /* STM8 configuration */
    MCU_Config();

    eeprom_init();

    keyInit();
    // IIC_Init();
    /* SSD1306 configuration */
#ifdef SSD1306_I2C_CONTROL
    SSD1306_I2cInit();
#endif
#ifdef SSD1306_SPI_CONTROL
    SSD1306_SpiInit();
#endif

    PWM_Init();

    // OLED_Init(); //初始化OLED
    // OLED_Clear();

    GUI_Init();
    /**
     * Note: to protect OS structures and data during initialisation,
     * interrupts must remain disabled until the first thread
     * has been restored. They are reenabled at the very end of
     * the first thread restore, at which point it is safe for a
     * reschedule to take place.
     */

    /* Initialise the OS before creating our threads */
    status = atomOSInit(&idle_thread_stack[IDLE_STACK_SIZE_BYTES - 1], IDLE_STACK_SIZE_BYTES, TRUE);
    if (status == ATOM_OK)
    {
        /* Enable the system tick timer */
        archInitSystemTickTimer();
        /* Create an application thread */
        status = atomThreadCreate(&main_tcb,
                                  16, system_status, 0,
                                  &main_thread_stack[MAIN_STACK_SIZE_BYTES - 1],
                                  MAIN_STACK_SIZE_BYTES,
                                  FALSE);
        status = atomThreadCreate(&at_thread,
                                  32, at_thread_func, 0,
                                  &at_thread_stack[SCAN_STACK_SIZE_BYTES - 1],
                                  SCAN_STACK_SIZE_BYTES,
                                  FALSE);
        if (status == ATOM_OK)
        {
            /**
             * First application thread successfully created. It is
             * now possible to start the OS. Execution will not return
             * from atomOSStart(), which will restore the context of
             * our application thread and start executing it.
             *
             * Note that interrupts are still disabled at this point.
             * They will be enabled as we restore and execute our first
             * thread in archFirstThreadRestore().
             */
            atomOSStart();
        }
    }

    /* There was an error starting the OS if we reach here */
    while (1)
        ;
}

/**
 * \b main_thread_func
 *
 * Entry point for main application thread.
 *
 * This is the first thread that will be executed when the OS is started.
 *
 * @param[in] param Unused (optional thread entry parameter)
 *
 * @return None
 */
static void system_status(uint32_t param)
{
    /* Compiler warnings */
    param = param;
    /* Initialise UART (115200bps) */
    // if (uart_init(115200) != 0)
    // {
    //     /* Error initialising UART */
    // }
    // /* Put a message out on the UART */
    // printf("Sys Init Over\n");
    /* Test finished, flash slowly for pass, fast for fail */
    while (1)
    {

        /* Sleep then toggle LED again */
        atomTimerDelay(500);
        LED_SYS_REVERSE;
        // uint8_t duty_cycle = eeprom_read_data8(PWM_CYCLE_ADD);
        // uint16_t pwm1_period = eeprom_read_data16(PWM_PERIOD_ADD);
        // printf("duty_cycle${%d}\n", duty_cycle);
        // printf("pwm1_period${%d}\n", pwm1_period);
    }
}

static void at_thread_func(uint32_t param)
{
    while (1)
    {
        atomTimerDelay(1);
        GUI_Refresh();
    }
}
