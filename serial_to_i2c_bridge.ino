// Serial-to-i2c bridge
// Copyright Christian Fobel <christian at fobel dot net>
//
// Forward all broadcast messages from i2c bus to serial-stream.
//
// Note that the lowest bit of `TWAR` must be set to 1 to receive _broadcast_
// messages.
#include <Wire.h>


uint8_t buf[BUFFER_LENGTH];
int bytes_read = 0;
bool success = false;
bool sending = false;

#define ADDRESS 0x3A
#define BROADCAST_ADDRESS 0x00

void setup() {
    /* Join i2c bus.
     *
     * Note, we need to set _some_ address, otherwise our `receivedEvent`
     * callback will not be called at all _(event though we are enabling
     * broadcast reception)_.  In other words, if we don't set any address, we
     * will not be able to receive _any_ messages, including broadcast
     * messages. */
    Wire.begin(ADDRESS);
    // Set i2c clock-rate to 400kHz.
    TWBR = 12;
    // Enable broadcasts to be received.
    TWAR = (ADDRESS << 1) | 1;
    // Register receive event.
    Wire.onReceive(receiveEvent);
    // Start serial for forwarding.
    Serial.begin(115200);
}


void loop() {
    if (0 < bytes_read) {
        Serial.write(buf, bytes_read);
        bytes_read = 0;
    }
    if(0 < Serial.available()) {
        /* Forward all bytes received on the local serial-stream to the i2c
         * bus. */

        /* __Important__ Note that `receiveEvent` does not get triggered during
         * between `begintransmission` and `endTransmission`.  This means that
         * messages forwarded from the local serial-stream will not be echoed
         * back to the local serial-stream.  This might actually be beneficial,
         * but is certainly worth noting. */
        Wire.beginTransmission(BROADCAST_ADDRESS);
        while (0 < Serial.available()) {
            char byte = Serial.read();
            //Serial.write(byte);
            Wire.write(byte);
        }
        Wire.endTransmission();
    }
    delay(10);
}


void receiveEvent(int howMany) {
    /* Record all bytes received on the i2c bus to a buffer.  The contents of
     * this buffer will be forwarded to the local serial-stream. */
    int i;
    if (0 < Wire.available()) {
        bytes_read = Wire.available();
        for (i = 0; i < bytes_read; i++) {
            buf[i] = Wire.read();
        }
        buf[i] = 0;
    }
}
