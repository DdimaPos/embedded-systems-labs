#include "dht11.h"
#include <DHT.h>
#include <freertos/FreeRTOS.h>

static DHT *dhtSensor = nullptr;

void dht11Init(uint8_t pin) {
  // The DHT11 data line is open-drain and requires a pull-up to VCC.
  // Enabling the ESP32's internal pull-up (≈45 kΩ) ensures the line is
  // not floating even if the module's built-in resistor is weak or absent.
  pinMode(pin, INPUT_PULLUP);

  dhtSensor = new DHT(pin, DHT11);
  dhtSensor->begin();
  delay(2000); // DHT11 needs ≥1 s after power-on before first read

  // Diagnostic: if the data line reads LOW here the bus is being held down
  // (short to GND, wrong pin, or damaged sensor).
  int idle = digitalRead(pin);
  printf("[dht11] init on GPIO%d  |  idle line = %s (expect HIGH)\n",
         pin, idle == HIGH ? "HIGH OK" : "LOW – check wiring!");
}

bool dht11Read(float *temperature, float *humidity) {
  if (!dhtSensor) {
    printf("[dht11] ERROR: sensor not initialised\n");
    return false;
  }

  portDISABLE_INTERRUPTS();
  float t = dhtSensor->readTemperature();
  float h = dhtSensor->readHumidity();
  portENABLE_INTERRUPTS();

  if (isnan(t) || isnan(h)) {
    printf("[dht11] read failed (NaN) on GPIO%d\n", DHT11_PIN);
    return false;
  }

  *temperature = t;
  *humidity    = h;
  return true;
}
