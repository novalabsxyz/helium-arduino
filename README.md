# Helium for Arduino #

![Build Status](https://travis-ci.org/helium/helium-arduino.svg?branch=master)

This code repository exposes an [Arduino](https://www.arduino.cc/)
library for
the [Helium Atom](https://www.helium.com/products/atom-xbee-module)
module. The Helium Atom makes it easy to securely connect IoT devices
and applications to back-end IoT services.


## Getting Started ##


See
a
[getting started guide](https://www.helium.com/dev/hardware-libraries/arduino) on
the Helium site.


## Supported Boards ##

Board             | D<SUB>in</SUB> | D<SUB>out</SUB> | Notes                                           |
------------------| -------------: | --------------: | ------------------------------------------------|
Arduino Uno       | 9              | 8               |                                                 |
Arduino M0 Pro    | 0              | 1               |                                                 |
Arduino Zero      | 0              | 1               |                                                 |
Arduino Due       | 9              | 8               | Wire up RX3 (15) to pin 8 on board, TX3 to pin9 |
Arduino Mega      | 10             | 11              |                                                 |

## Example Setup ##

The example sketches can be found in the Arduino IDE under _File >
Examples > Helium_.

## Getting Help ##

If you have any questions or ideas about how to use this code - or any
part of Helium - head over to
the [Helium Community Slack](http://chat.helium.com/). We're standing
by to help.

# Contributing

Want to contribute to helium-arduino? That's awesome!

Please
see
[CONTRIBUTING.md](https://github.com/helium/helium-arduino/blob/master/CONTRIBUTING.md) in
this repository for details.
