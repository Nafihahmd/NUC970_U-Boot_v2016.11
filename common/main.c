/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define	DEBUG	*/

#include <common.h>
#include <autoboot.h>
#include <cli.h>
#include <console.h>
#include <version.h>

DECLARE_GLOBAL_DATA_PTR;

// Include GPIO headers
#include <asm/gpio.h>
//#include <linux/delay.h>
#define LED1 GPIO_PG3
#define LED2 GPIO_PB2
#define LED3 GPIO_PG5

void set_user_led_red(void) {
    struct udevice *gpio_dev;
    int ret;
	
	puts("All LEDs turn RED\n");

    // Request and configure PG3 as output (RED LED ON)
    ret = gpio_request( GPIO_PG3, "LED 1");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
    gpio_direction_output(LED1, 1); // Set initial state Low
    // Request and configure PB2 as output (RED LED ON)
    ret = gpio_request( GPIO_PB2, "LED 2");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
    gpio_direction_output(LED2, 1); // Set initial state Low
    // Request and configure PG5 as output (RED LED ON)
    ret = gpio_request( GPIO_PG5, "LED 3");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
    gpio_direction_output(LED3, 1); // Set initial state Low
	/* LED Blink Logic */
	/*
	udelay(500000);                     // 500ms delay
	
	// Turn LEDs Green 
	gpio_set_value(LED1, 0); 
	gpio_set_value(LED2, 0); 
	gpio_set_value(LED3, 0); 
	udelay(500000); 
	
	// Turn LEDs OFF
	gpio_direction_input(LED1);
	gpio_direction_input(LED2);
	gpio_direction_input(LED3);
	*/
	
}

void set_user_led_green(void) {
    struct udevice *gpio_dev;
    int ret;
	
	puts("All LEDs GREEN\n");

    // Request and configure PG3 as output (RED LED ON)
    ret = gpio_request( GPIO_PG3, "LED 1");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
    gpio_direction_output(LED1, 0); // Set initial state Low
    // Request and configure PB2 as output (RED LED ON)
    ret = gpio_request( GPIO_PB2, "LED 2");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
    gpio_direction_output(LED2, 0); // Set initial state Low
    // Request and configure PG5 as output (RED LED ON)
    ret = gpio_request( GPIO_PG5, "LED 3");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
    gpio_direction_output(LED3, 0); // Set initial state Low
}

void set_user_led_off(void) {
    struct udevice *gpio_dev;
    int ret;
	
	puts("LEDs tunred OFF\n");

    // Request and configure PB2 as output (RED LED ON)
    ret = gpio_request( GPIO_PB2, "LED 2");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
	gpio_direction_input(LED2);
    // Request and configure PG5 as output (RED LED ON)
    ret = gpio_request( GPIO_PG5, "LED 3");
    if (ret) {
        printf("GPIO request failed: %d\n", ret);
        return;
    }
	gpio_direction_input(LED3);
}


/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

static void run_preboot_environment_command(void)
{
#ifdef CONFIG_PREBOOT
	char *p;

	p = getenv("preboot");
	if (p != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

		run_command_list(p, -1, 0);

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */
}

/* We come here after U-Boot is initialised and ready to process commands */
void main_loop(void)
{
	const char *s;

	bootstage_mark_name(BOOTSTAGE_ID_MAIN_LOOP, "main_loop");

#ifdef CONFIG_VERSION_VARIABLE
	setenv("ver", version_string);  /* set version variable */
#endif /* CONFIG_VERSION_VARIABLE */

	//set_user_led_red();	//LED red
	cli_init();

	run_preboot_environment_command();
	//set_user_led_green();

#if defined(CONFIG_UPDATE_TFTP)
	update_tftp(0UL, NULL, NULL);
#endif /* CONFIG_UPDATE_TFTP */

	s = bootdelay_process();
	if (cli_process_fdt(&s))
		cli_secure_boot_cmd(s);

	//set_user_led_off();
	autoboot_command(s);
	cli_loop();
	panic("No CLI available");
}
