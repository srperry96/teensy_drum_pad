# Teensy Drum Pad
Development repo for a teensy (+ audio board) based synthesizer and drum pad.

Demo video here: https://www.youtube.com/watch?v=pE9f0ItpdG8

![Teensy Drum Pad Full View](/Docs/drumpad_pic1.jpg)

## Hardware
- Teensy + Audio Board
- Adafruit Neotrellis
- 2.4" TFT LCD Screen (ILI9341)
- 4 x Potentiometers
- Microphone (MAX4466 Breakout Board) - not implemented yet...

![Electronics Internal View](/Docs/drumpad_pic3_inner.jpg)

## Enclosure
### CAD
![Enclosure CAD](/Docs/CAD_full_view.PNG)
CAD for the V1 basic enclosure.

## Menu Navigation
The neotrellis buttons are used as a keypad for navigating through modes. Menu is accessed by holding any button for a period of time.
![Menu Navigation Photo](/Docs/drumpad_pic2.jpg)

### Modes
Implemented modes so far are drum pad and synthesizer mode.
#### Drum Pad
Plays WAV file samples from a MicroSD card. Multiple kits could be implemented by adding more samples to the card.

#### Synthesizer
- 2x Oscillators 
- Multiple waveshapes - Sin, Square, Triangle, Sawtooth, Reverse Sawtooth
- Knobs to control:
    - Volume
    - Low Pass Filter
    - Detune (oscillator 2 frequency is altered)
    - Overdrive
- Change octaves up/down
