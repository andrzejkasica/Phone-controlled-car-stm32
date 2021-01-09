#include "uart.h"

void USART_INIT(void)
{
	// enable USART2 clock, bit 17 on APB1ENR
    RCC->APB1ENR |= (1 << 17);
	
	// set pin modes as alternate mode 7 (pins 2 and 3)
    // USART2 TX and RX pins are PA2 and PA3 respectively
    GPIOA->MODER &= ~(0xFU << 4); // Reset bits 4:5 for PA2 and 6:7 for PA3
    GPIOA->MODER |=  (0xAU << 4); // Set   bits 4:5 for PA2 and 6:7 for PA3 to alternate mode (10)
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6);  // Bits (5:4)= 1:1 and Bits (7:6)= 1:1 --> High Speed for PIN PA2 and PA3
	
	// choose AF7 for USART2 in Alternate Function registers AF7
		GPIOA->AFR[0] |= (7 << 8); // for pin A2
    GPIOA->AFR[0] |= (7 << 12); // for pin A3
	
	 // enable usart2 - UE, bit 13
	 USART2->CR1 = 0x00;
 	 USART2->CR1 = (1 << 13);
	
	
	  USART2->CR1 &= ~(1 << 12);
		 
		 // 546 14
		 // 104 3
		 // 312 8
	  USART2->BRR |= (104 << 4) | (3 << 0); //clk 16 baudrate 9600
		
		

    // USART2 parity control, bit 9
    USART2->CR1 |= (0 << 9); // 0 - no parity

    // USART2 TX enable, TE bit 3
    USART2->CR1 |= (1 << 3);

    // USART2 RX enable, RE bit 2
    USART2->CR1 |= (1 << 2);


    NVIC_SetPriority(USART2_IRQn, 1); // Priority level 1
    NVIC_EnableIRQ(USART2_IRQn);

    // now that everything is ready,
    // enable tx interrupt and let it push out
    USART2->CR1 |= (1 << 7);
		USART2->CR1 |= (1 << 5);
}

void USART2_SendChar(uint8_t data)
{
	USART2->DR = data;
	
	while (!(USART2->SR & (1<<6)));
}

void USART2_SendString(char *string)
{
	while (* string) USART2_SendChar(*string++);
}
	
uint8_t USART2_GetChar()
{	
	while (!(USART2->SR & (1<<5)));
	
	return USART2->DR;
}
