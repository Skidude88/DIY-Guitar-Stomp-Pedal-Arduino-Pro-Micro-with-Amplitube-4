/* Based on the following tutorial:
   This examples shows how to make a simple seven keys MIDI keyboard with volume control

   Created: 4/10/2015
   Author: Arturo Guadalupi <a.guadalupi@arduino.cc>
   
   http://www.arduino.cc/en/Tutorial/MidiDevice
*/

#include "MIDIUSB.h"
#define NUM_BUTTONS  5

const uint8_t button1_pin = 15; // Program Preset : Program Change (PC) Pin15
const uint8_t button2_pin = 18; // Program Preset : Program Change (PC) Pin18
const uint8_t button3_pin = 19; // Program Preset : Program Change (PC) Pin19
const uint8_t button4_pin = 20; // Previous Preset : Control Change (CC) Pin20
const uint8_t button5_pin = 21; // Next Preset : Control Change (CC) Pin21

const uint8_t buttons[NUM_BUTTONS] = {button1_pin, button2_pin, button3_pin, button4_pin, button5_pin};

const uint8_t button1_val = 1;  //Program preset 1
const uint8_t button2_val = 2;  //Program preset 2
const uint8_t button3_val = 3;  //Program preset 3
const uint8_t button4_val = 9;  //Previous Program preset
const uint8_t button5_val = 10; //Next Program preset

const uint8_t btn_values[NUM_BUTTONS] = {button1_val, button2_val, button3_val, button4_val, button5_val};

uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;

byte midiCh = 0; //midi ch0 : amplitube ch1


void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
}

void loop() {
  readButtons();
  outputMIDI();

}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}


// First parameter is the event type (0x0C = program change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the program change number (0-127).

void programChange(byte channel, byte program) {
  midiEventPacket_t event = {0x0C, 0xC0 | channel, program, 0};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      delay(50);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void outputMIDI()
{
  
  for (int i = 0; i < NUM_BUTTONS ; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);

        if(buttons[i] < 20){
          //Pins 15 - 19 Select Program Preset
          programChange(midiCh,btn_values[i]);
        } else {
          // Pins 20 - 21 Next\Prev Preset
          controlChange(midiCh,btn_values[i],0);     
        }
      }
      else
      {
        bitWrite(previousButtons, i , 0);
      }
    }
  }
}
