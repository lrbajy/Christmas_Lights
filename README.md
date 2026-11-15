# Christmas Lights

A simple ESP32 music project that plays Christmas songs through a buzzer, flashes LEDs with the music, and lets you control everything from a web browser.

## What You Need

To use this project, you will need:

* 1 ESP32 board
  Examples: ESP32 DevKit, ESP32 Dev Module, or a compatible board
* 1 passive buzzer
  A passive buzzer is required so the board can play different notes
* 6 LEDs
  Any color works
* 6 resistors (220Ω to 330Ω)
  One for each LED(Optional)
* 1 breadboard or expansion board
  Useful for quick wiring without soldering
* Jumper wires
  Male-to-male jumper wires are usually enough
* USB cable for your ESP32
  Used to upload the sketch and power the board
* A phone or computer with a web browser
  Used to control the player

## Wiring

Basic wiring is very simple:

* Connect the positive leg of the buzzer to GPIO 18
* Connect the negative leg of the buzzer to GND

For the LEDs, connect the long leg (anode) of each LED through a resistor to its pin, and the short leg (cathode) to GND:

* LED 1: GPIO 13
* LED 2: GPIO 14
* LED 3: GPIO 25
* LED 4: GPIO 26
* LED 5: GPIO 27
* LED 6: GPIO 32

Each LED needs its own resistor. If your board uses different pins, change `BUZZER` and `LED_PINS` at the top of the code.

## Breadboard Setup

1. Place the buzzer on the breadboard
2. Connect one side of the buzzer to GPIO 18 and the other side to GND
3. Place the 6 LEDs on the breadboard
4. Connect each LED to its GPIO pin through a resistor
5. Connect the short leg of every LED to GND
6. Plug the ESP32 into your computer with a USB cable

## How to Use

1. Open the project in the Arduino IDE with ESP32 board support installed
2. Connect your ESP32 board
3. Select the correct board and port
4. Upload the sketch
5. The 6 LEDs will blink once at startup as a wiring test
6. On your phone, join the Wi-Fi network `Xmas-Player` (password `12345678`)
7. Open `http://192.168.4.1` in your browser
8. Pick a song, then play, pause, skip, or change the speed

Want to use your home Wi-Fi instead? Set `USE_AP` to `false` in the code, enter your Wi-Fi name and password, then open the Serial Monitor (115200 baud) to see the IP address. Only 2.4 GHz Wi-Fi works.

## Songs

* Jingle Bells
* We Wish You a Merry Christmas
* Silent Night
* Joy to the World
* Deck the Halls

## LED Modes

* Pitch: each note lights one LED, from low notes to high notes
* Chase: the light hops from LED 1 to LED 6, one step per note
* Bar: lights every LED up to the pitch of the note, like a level meter
* Off: all LEDs stay dark

## Notes

* Make sure your buzzer is connected to the correct pin
* If you hear nothing, check:
   * the wiring
   * the board/port selection
   * whether the buzzer is passive and suitable for tone output
* If you can't open the web page, check:
   * your phone is connected to the `Xmas-Player` network
   * you typed `http://` before the address
   * mobile data is turned off
* If an LED stays off, check its direction and resistor
* The melodies were written by hand from the standard tunes, so some notes may sound slightly off
* Some buzzers are much quieter than others, so low volume does not always mean something is broken

## Project Goal

This project is mainly a fun hardware-based ESP32 music example.
The important part is having the basic components ready:

* ESP32
* passive buzzer
* 6 LEDs with resistors
* breadboard
* jumper wires
* USB cable
