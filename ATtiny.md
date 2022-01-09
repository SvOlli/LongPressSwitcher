 ATtiny
========

As of now, the ATtiny configurations can be compiled only using the
Arduino IDE. This will most probably change, once I've learned how to
configure arduino-mk to use boards added using the "Board Manager".

So far, I've tested to two different extensions for the "Board Manager":
[ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) and
[attiny](https://github.com/damellis/attiny). Look at their pages for
informations on how to include them in your Arduino IDE.

If your already familiar with that procedure and just need the additional
Board Manager URLs, these are:
* ATTinyCore: https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
* attiny: http://drazzy.com/package_drazzy.com_index.json

As with the Arduino based setup, I recommend to use a setup for flashing
without a bootloader, using an ISP (In-System Programmer). This can be
done using an Arduino programmed with a sketch from "File" -> "Examples"
-> "11.ArduinoISP". I'm using a very cheap "USBASP" programmer.
