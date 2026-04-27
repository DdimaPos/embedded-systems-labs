#include "SerialIO.h"
#include <Arduino.h>

void SerialIoInit(void)
{
    Serial.begin(SERIAL_BAUD);
    delay(2000);   /* wait for USB-serial enumeration on the host side */
}

/* Non-blocking read: returns the next byte or -1 if nothing is available.
 *
 * WHY NOT getchar()/scanf():
 *   Arduino's Serial.begin() installs its own buffered UART driver that is
 *   separate from the ESP-IDF VFS UART driver. The VFS functions
 *   (esp_vfs_dev_uart_use_nonblocking, etc.) require uart_driver_install()
 *   to have been called first — something Serial.begin() does NOT do.
 *   Calling VFS setup after Serial.begin() therefore has no effect on stdin,
 *   and getchar() always returns EOF.
 *
 *   This function wraps Serial.available()/Serial.read() inside the ECAL
 *   driver so that no service or main program ever calls Serial.* directly.
 */
int SerialIO_ReadChar(void)
{
    if (Serial.available() > 0) {
        return Serial.read();
    }
    return -1;
}
