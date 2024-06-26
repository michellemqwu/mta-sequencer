#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

//include midi to frequency lib by littlescale
#include <mtof.h>
//include the bounce library for more stable button readings
#include <Bounce.h>
#define HWSERIAL Serial3

// GUItool: begin automatically generated code
AudioSynthWaveformDc dc1;                  //xy=245.5,394
AudioSynthWaveformModulated waveformMod1;  //xy=466.5,357
AudioEffectEnvelope envelope1;             //xy=494.5,417
AudioFilterStateVariable filter1;          //xy=671.5,335
AudioEffectEnvelope envelope2;             //xy=849.5,312
AudioOutputPT8211 pt8211_1;                //xy=1000,304
AudioConnection patchCord1(dc1, envelope1);
AudioConnection patchCord2(waveformMod1, 0, filter1, 0);
AudioConnection patchCord3(envelope1, 0, filter1, 1);
AudioConnection patchCord4(filter1, 0, envelope2, 0);
AudioConnection patchCord5(envelope2, 0, pt8211_1, 0);
AudioConnection patchCord6(envelope2, 0, pt8211_1, 1);
// GUItool: end automatically generated code

int rootScaled[7][8] = {};
int rootNote = 70;

int stepCounter = 1;
int stepLEDPin;
float potReading;

bool mtaData = true;

void setup() {
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);
  HWSERIAL.begin(9600);

  AudioMemory(100);
  //sgtl5000_1.enable();
  //sgtl5000_1.volume(.6);

  dc1.amplitude(1.0);
  waveformMod1.frequency(55);
  waveformMod1.amplitude(0.8);
  waveformMod1.begin(WAVEFORM_SINE);

  //this adjusts the range of the frequency sweep in the filter. Make it smaller
  //if you ever get static at a polar end.
  filter1.octaveControl(3.5);
  filter1.resonance(5);

  Serial3.setRX(15);
  Serial3.setTX(14);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (HWSERIAL.available() > 0) {
    mtaData = true;
    String incomingByte;
    incomingByte = Serial3.readString();
    Serial.println(incomingByte);
    int an = incomingByte.substring(0, 2).toInt();
    int as = incomingByte.substring(3, 5).toInt();
    int cn = incomingByte.substring(6, 8).toInt();
    int cs = incomingByte.substring(9, 11).toInt();
    int fn = incomingByte.substring(12, 14).toInt();
    int fs = incomingByte.substring(15, 17).toInt();
    int rn = incomingByte.substring(18, 20).toInt();
    int rs = incomingByte.substring(21, 23).toInt();
    playSequence(32, 0, an);
    playSequence(33, 0, as);
    playSequence(34, 0, cn);
    playSequence(35, 0, cs);
    playSequence(36, 0, fn);
    playSequence(37, 0, fs);
    playSequence(31, 0, rn);
    playSequence(30, 0, rs);
  }

  // mtaData = false;
  // switch (stepCounter) {
  //   case 1:
  //     stepLEDPin = 32;
  //     potReading = analogRead(A15);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 2:
  //     stepLEDPin = 33;
  //     potReading = analogRead(A17);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 3:
  //     stepLEDPin = 34;
  //     potReading = analogRead(A16);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 4:
  //     stepLEDPin = 35;
  //     potReading = analogRead(A14);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 5:
  //     stepLEDPin = 36;
  //     potReading = analogRead(A13);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 6:
  //     stepLEDPin = 37;
  //     potReading = analogRead(A12);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 7:
  //     stepLEDPin = 31;
  //     potReading = analogRead(A11);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  //   case 8:
  //     stepLEDPin = 30;
  //     potReading = analogRead(A10);
  //     playSequence(stepLEDPin, potReading, 0);
  //     break;
  // }

  // if (stepCounter < 8) {
  //   stepCounter++;
  // } else {
  //   stepCounter = 1;
  // }
}

void playSequence(int stepLEDPin, float potReading, int mtaInput) {
  digitalWrite(stepLEDPin, HIGH);
  if (mtaData) {
    if (mtaInput == 59) {
      waveformMod1.frequency(2000);
    } else {
      waveformMod1.frequency(map(mtaInput, 0, 20, 55, 1500));
    }
  } else {
    waveformMod1.frequency(map(potReading, 0, 1023, 55, 880));
  }

  int tempo = map(analogRead(A14), 0, 1023, 1000, 100);
  int filterAttackTime = map(analogRead(A15), 0, 1023, 1, 170);
  int envelopeReleaseTime = map(analogRead(A17), 0, 1023, 1, 220);
  float filterResonance = map((float)analogRead(A16), 0, 1023, 0, 5);

  //set all of the parameters for the note
  envelope1.attack(0);
  envelope1.decay(0);
  envelope1.sustain(1);
  envelope1.release(envelopeReleaseTime);

  envelope2.attack(filterAttackTime);
  envelope2.decay(1000);
  envelope2.sustain(1);
  envelope2.release(envelopeReleaseTime);

  filter1.resonance(filterResonance);

  //trigger the note
  envelope1.noteOn();
  envelope2.noteOn();

  //add a short delay before shutting off the note
  delay(10);
  envelope2.noteOff();
  envelope1.noteOff();
  digitalWrite(stepLEDPin, LOW);
  //delay by the knob value to control the overall speed of the sequence
  delay(tempo);
}
