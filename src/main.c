#include "stm32f4xx.h"


// Quick and dirty delay
static void delay (unsigned int time) {
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 2000; j++);
}

void blink() {
    // Turn on the GPIOA peripheral
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    while (1) {
        // Reset the state of pin 13 to output low
        GPIOA->BSRR = GPIO_BSRR_BR5;

        delay(500);

        // Set the state of pin 13 to output high
        GPIOA->BSRR = GPIO_BSRR_BS5;

        delay(500);
    }
}

void blink2 (void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER5)) | (GPIO_MODER_MODER5_1);
    GPIOA->AFR[0] |= 0x1 << GPIO_AFRL_AFSEL5_Pos;

    TIM2->ARR = 50000; // 500ms ON + 500ms OFF
    uint32_t iter = 0;
    TIM2->CCR1 = iter;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->EGR |= TIM_EGR_UG;

    while(1){
        TIM2->CCR1 = iter;
        if (iter < (TIM2->ARR)) {
            iter += 1;
            // 1/16 MHz = 62.5 ns  ~ x16 ~ 1mks

            for (volatile unsigned short j = 0; j < 16; j++); // ~ 1us
        } else {
            while (iter != 0) {
                iter -=  1;
                TIM2->CCR1 = iter;
                for (volatile unsigned short j = 0; j < 16; j++);  // ~ 1us
            }
        }
    }
}


void blink3() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER5)) | (GPIO_MODER_MODER5_1);
    GPIOA->AFR[0] |= 0x1 << GPIO_AFRL_AFSEL5_Pos;
    TIM2->PSC = 16000-1; // 1000Hz
    TIM2->ARR = 2000-1;
    uint32_t iter = 1000-1;
    TIM2->CCR1 = iter;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->EGR |= TIM_EGR_UG;
    while(1){ }
}

int main (void) {

    blink2();
    return 0;
}
