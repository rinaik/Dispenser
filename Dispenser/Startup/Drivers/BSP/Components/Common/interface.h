/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INTERFACE_H
#define __INTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif

// Using SPI5, put chip select at PF6

#define L6470_nCS_GPIO_PORT GPIOF      // PORT F
#define L6470_nCS_GPIO_PIN GPIO_PIN_6  // PIN 6

#ifdef __cplusplus
}
#endif

#endif /* __INTERFACE_H */
