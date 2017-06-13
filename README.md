# Helium for Arduino #

[![Build Status](https://travis-ci.com/helium/helium-arduino.svg?token=aKFDFjxw98usjj6jrpst&branch=master)](https://travis-ci.com/helium/helium-arduino)

This code repository exposes an [Arduino](https://www.arduino.cc/)
library for
the [Helium Atom](https://www.helium.com/products/atom-xbee-module)
module. The Helium Atom makes it easy to securely connect IoT devices
and applications to back-end IoT services.


# Getting Started #


## Hardware ##

To get started with Helium for Arduino you will need the following hardware:

* A Helium Atom. The easiest way to get started is to get
  the
  [Helium Starter Kit](https://www.helium.com/products/helium-starter-kit) which
  includes a gateway (Element), and an XBee version of the Helium
  Atom.

* An Arduino board of any supported type, such as
  the
  [Arduino Uno](https://www.arduino.cc/en/main/arduinoBoardUno). The
  supported boards are listed below.

* An
  [Arduino XBee Shield](https://www.helium.com/products/atom-arduino-adapters) to
  adapt the XBee to the Arduino shield pinout.



## Software ##

You will need the following software installed:

* The [Arduino IDE](https://www.arduino.cc/en/Main/Software).

* The
  [Helium Arduino Library](https://github.com/helium/helium-arduino). You
  can install this library through either of the following steps:

    * The Library Manager by clicking on _Sketch > Include Library_,
      searching for Helium and clicking the _Install_ button.

    * By downloading a release _Helium.zip_ file from
      the
      [helium-arduino releases](https://github.com/helium/helium-arduino/releases) page. Once
      downloaded launch the Arduino IDE, click _Sketch > Include
      Library, Add .Zip Library_, and selecting the _Helium.zip_
      file. Note that you will need to re-start the Arduino IDE after
      installing the zip library.


## Board Setup ##

Ensure that the Helium Element is powered on and
connected. The main LED on the Element should be green or blue to
indicate it is powered on and connected to the network.

Plug the Adapter into the Arduino board and the Helium XBee Atom into
the Adapter.

Depending on the Arduino board you will have to configure the
D<SUB>in</SUB> and D<SUB>out</SUB> jumpers on the Adapter to get the
Arduino to talk to the Helium Atom using the right pins.

The following boards D<SUB>in</SUB> and D<SUB>out</SUB> pins are
known. Other setups will also work but we've confirmed the following
work.

Board             | D<SUB>in</SUB> | D<SUB>out</SUB>
------------------| -------------: | --------------:
Arduino Uno       | 8              | 9
Arduino M0 Pro    | 0              | 1
Arduino Zero      | 0              | 1

## Example Setup ##

The example sketches can be found in the Arduino IDE under _File > Examples > Helium_.

With one of the examples opened, the Element powered on, and the
configured board connected to a USB port click the _Run_ button.

The IDE should upload the example sketch to the board and you should
see the Atom power on, connect and perform the task coded into the
loaded example sketch.

Note that some of the demo sketches will require you to have a
configured Channel in the Helium Dashboard in order to channel data
from the device all the way to the end IoT service.

Please read the comments in the example sketch and in the Channel
configuration sections in the dashboard for more information on how to
set up and configure channels.

## Getting Help ##

That should be all you need to get going with Helium and Arduino. The
Arduino community has a vast library of supported hardware extensions,
all with libraries and example sketches to show their use. Tying these
together with the Helium wireless network will make it a breeze to
build a connected IoT device.

If you have any questions or ideas about how to use this code - or any
part of Helium - head over to
the [Helium Community Slack](http://chat.helium.com/). We're standing
by to help.

# Contributing

Want to contribute to helium-arduino? That's awesome!

Please
see
[CONTRIBUTING.md](https://github.com/helium/helium-arduino/CONTRIBUTING.md) in
this repository for details.
