#pragma once

#if defined(TARGET_BLUEPILL)
    #define DPIN_LED        LED_BUILTIN
    #define LED_INVERTED    1
    #define APIN_VBAT       A0
    #define USART_INPUTS    { USART2 }  // UART2 RX=PA3 TX=PA2
    #define USART_OUTPUT    USART3  // UART2 RX=PA3 TX=PA2
#endif

#if !defined(VBAT_R1) || !defined(VBAT_R2)
    // Resistor divider used on VBAT input, R1+R2 must be less than 3178
    #define VBAT_R1         820
    #define VBAT_R2         120
#endif