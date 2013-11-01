Overview
========

This project implements a [serial][1]-to-[i2c][2] bridge.

[1]: http://arduino.cc/en/reference/serial
[2]: http://arduino.cc/en/reference/Wire


Hardware configuration
======================

The figure below shows an example system configuration.  On the left, there is
a `PC host` device, which has a serial port or USB-to-serial adapter.  The `PC
host` is then connected to an Arduino-compatible device that has been flashed
with the compiled firmware from this project _(i.e.,
`serial_to_i2c_bridge.hex`)_.  This Arduino-device is labelled as
`Serial-to-i2c-bridge` in the figure.  Other Arduino-compatible devices
_(labelled `Device 1`-`Device N` in the figure) may then be connected to the same
i2c bus as the `Serial-to-i2c-bridge`.

In this configuration, any bytes written to the serial connection by the `PC
host` are read by the `Serial-to-i2c-bridge` and are immediately written to the
i2c bus as a _broadcast message_.  Furthermore, the byte contents of any
_broadcast message_ written to the i2c bus by any device _other than the
`Serial-to-i2c-bridge` device_ are read by the `Serial-to-i2c-bridge` device
and are immediately written through the serial connection to the `PC host`.

__N.B.__ To test the bridge:

  * Flash the `serial_to_i2c_bridge.hex` file to a second device connected to the i2c bus.
  * Open a serial terminal on the serial port connected to the second device,
    using a baud-rate of 115200.

If things are connected correctly, any bytes written to the first
`Serial-to-i2c-bridge` device should be displayed in the serial prompt
connected to the second device.

                                              i2c bus
     ____________        _____________          ||         ____________
     |          |        | Serial-to  |         ||         |          |
     | PC host  |<======>| i2c-bridge |<=======>||<=======>| Device 1 |
     |__________|   ^    |____________|         ||         |__________|
                    |                           ||
                  Serial                        ||         ____________
                connection                      ||         |          |
                                                ||<=======>| Device 2 |
                                                ||         |__________|
                                                ||              .
                                                ||              .
                                                ||              .
                                                ||         ____________
                                                ||         |          |
                                                ||<=======>| Device N |
                                                ||         |__________|


Build instructions
==================

To compile and flash to an `Uno`-compatible Arduino device, run:

    scons ARDUINO_BOARD=uno ARDUINO_PORT=<serial port name, e.g., "COM1", > upload

To compile and flash to a `Diecimila`-compatible or a `Duemilanove`-compatible
Arduino device, run:

    scons ARDUINO_BOARD=atmega328 ARDUINO_PORT=<serial port name, e.g., "COM1", > upload


Credits
=======

Copyright 2013 Christian Fobel
