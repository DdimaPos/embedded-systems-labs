#pragma once
#include <stdint.h>

/* --- Wiring --- */
#define RELAY_PIN  19   /* Active-HIGH 1-channel relay module */

void    RelayDriver_Init(void);
void    RelayDriver_On(void);
void    RelayDriver_Off(void);
void    RelayDriver_Toggle(void);
uint8_t RelayDriver_GetState(void);   /* 0=off, 1=on */
