#ifndef DHT11_DRIVER_H
#define DHT11_DRIVER_H

#include <Arduino.h>
#include <stdbool.h>

#define DHT11_PIN 4

void dht11Init(uint8_t pin);
bool dht11Read(float *temperature, float *humidity);

#endif
