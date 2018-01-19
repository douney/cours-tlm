/********************************************************************
 * Copyright (C) 2009, 2012 by Verimag                              *
 * Initial author: Matthieu Moy                                     *
 ********************************************************************/

/*!
  \file hal.h
  \brief Harwdare Abstraction Layer : implementation for MicroBlaze
  ISS.


*/
#ifndef HAL_H
#define HAL_H

#include <stdint.h>


/* Dummy implementation of abort(): invalid instruction */
#define abort() 

/* TODO: implement HAL primitives for cross-compilation */
#define hal_read32(a) *((uint32_t*)(a))
#define hal_write32(a, d) *((uint32_t*)(a)) = d
#define hal_wait_for_irq() while(irq_received == 0){} irq_received = 0
#define hal_cpu_relax()

void microblaze_enable_interrupts(void) {
	__asm("ori     r3, r0, 2\n"
	      "mts     rmsr, r3");
}

/* TODO: printf is disabled, for now ... */
#define printf(str) xprintf(str)

void xprintf(char *str) {
	int i = 0;
	while(str[i] != '\0') {
		hal_write32(UART_BASEADDR + UART_FIFO_WRITE, (uint32_t) str[i]);
		i++;
	}
}

#endif /* HAL_H */
