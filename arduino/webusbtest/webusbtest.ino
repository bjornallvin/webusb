#include <WebUSB.h>
#include <Bounce2.h>

/**
 * Creating an instance of WebUSBSerial will add an additional USB interface to
 * the device that is marked as vendor-specific (rather than USB CDC-ACM) and
 * is therefore accessible to the browser.
 *
 * The URL here provides a hint to the browser about what page the user should
 * navigate to to interact with the device.
 */
WebUSB WebUSBSerial(1 /* http:// */, "localhost:3000");

#define Serial WebUSBSerial

const int ledPin = 13;
const int buttonPin = 3;

Bounce debouncer = Bounce(); 
int buttonValue = 0;
int oldButtonState = 1;
int buttonState;
int firstChange = true;
boolean stabilized = false;

void setup() {
    pinMode(buttonPin,INPUT);
    debouncer.attach(buttonPin);
    debouncer.interval(25);
    // Activate internal pull-up
    digitalWrite(buttonPin,HIGH);

    //debouncer.update();
    //oldButtonState = debouncer.read();
    pinMode(ledPin, OUTPUT);
    while (!Serial) {
        ;
    }
    Serial.begin(9600);
    Serial.println("Sketch begins.");
    Serial.flush();

    /*debouncer.update();
    oldButtonState = buttonState = debouncer.read();

    Serial.print("buttonState: ");
    Serial.println(buttonState);Serial.flush();
    Serial.print("oldButtonState: ");
    Serial.println(oldButtonState);Serial.flush();
*/
   
}

void loop() {
  if (Serial && Serial.available()) {
    int byte = Serial.read();
    Serial.write(byte);
    if (byte == 'H') {
      Serial.write("Turning LED on.");
      digitalWrite(ledPin, HIGH);
      buttonValue = 1;
    } else if (byte == 'L') {
      Serial.write("Turning LED off.");
      buttonValue = 0;
      digitalWrite(ledPin, LOW);
    }
    Serial.flush();
  }
  
    debouncer.update();
    buttonState = debouncer.read();
    
    if (oldButtonState == 1 && buttonState == 1 && !stabilized){
        Serial.println("Button stabilized");
        Serial.flush();
        stabilized = true;
    }
    if (stabilized){

        if (buttonState != oldButtonState) {

            Serial.print("buttonState: ");
            Serial.println(buttonState);Serial.flush();
            Serial.print("oldButtonState: ");
            Serial.println(oldButtonState);Serial.flush();


            oldButtonState = buttonState;
            if (buttonState == 0){
                //Serial.println("\r\nButton pressed down");
                //Serial.flush();
            }
            else{
                buttonValue = !buttonValue;
                Serial.print("TURNED:");
                Serial.println(buttonValue);
                Serial.flush();
                digitalWrite(ledPin,buttonValue);
            }
        }
    }
  
}