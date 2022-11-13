/*
 * Interface and controller for scale and bagging system.
 * V1 will weigh and control water input through solenoid
*/


#include "HX711.h" //This library http://librarymanager/All#Avia_HX711

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

const int buttonPin = 6;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


HX711 scale;
float calibration_factor = 213; //200930; //-7050 worked for my 440lb max scale setup



int user_int;
int user_water = 220; //default for grain jars
int dry_weight = 200; //default for grain jars


void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

//  pinMode(greenPin, OUTPUT);
//  pinMode(bluePin, OUTPUT);

  Serial.println("Remove all weight from scale");
  Serial.print("Calibration factor: ");
  Serial.println(calibration_factor);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

    menu();

}

void loop() {
  // if there's any serial available, read it:
  user_int = 0;
  scale.set_scale(calibration_factor);

  while (Serial.available() > 0) {
    menu_choice();
//    get_int();
    // look for the next valid integer in the incoming serial stream:
//    char user = Serial.parseInt();


//    Serial.print(user);
    // look for the newline. That's the end of your sentence:
//    if (isDigit(user)) {
//
//      // print the three numbers in one string as hexadecimal:
////      Serial.print("Is a number: ");
////      Serial.println(user);
//    } else {
//      Serial.print("Is a char: ");
//      Serial.println(char(user));
//
//    }
  }
}

void menu_choice(){
  char user_char = Serial.read();
    switch(user_char){
      case 't':  //Tare the scale
        scale.tare();
        Serial.println("\n------------------------------------------------------\n");
        Serial.println("Taring the scale...");
        Serial.print("Reading: ");
        Serial.print(scale.get_units(2), 1);
        Serial.println(" g");
        break;
      case 'c':
        calibration();
        break;
      case 'e':
        get_user_water();
        break;
      case 'd':
        get_dry_weight();
        break;
      case 'p':
        print_settings();
        break;
      case 'w':
        weighing_menu_choice();
        break;
      case '\n':
        break;
      default:
        Serial.println("\nError! Incorrect input...");
        menu();
        break;
      }
}

int calibration(){
  Serial.println("Entering calibration mode...");
  Serial.println("a to increase: z to decrease...");
  while(true){
    scale.set_scale(calibration_factor);
    just_weight_mode(true);
    if (Serial.available())
    {
      char temp = Serial.read();
      if (temp == '+' || temp == 'a'){
        calibration_factor += 1;
      }
      else if (temp == '-' || temp == 'z'){
        calibration_factor -= 1;
      }
     else if (temp == 't' ){
        scale.tare();
      }
    }
  }
}


void just_weight_mode(int cal){
  char input = 'a';
//  Serial.println("Just weight mode");
//  Serial.println(input);
  while(true){
    if(Serial.available() > 1){break;}
    if(cal){
      Serial.print("Calibration factor:");
      Serial.print(calibration_factor);
      Serial.print(" -- ");

      }
//    input = Serial.read();
//    Serial.println(input);
//    Serial.print(" -- Reading: ");
//    Serial.print(scale.get_units(), 3);
//    Serial.print(" g");
    Serial.print("Average: ");
//    Serial.println(scale.read_average());
    Serial.print(scale.get_units(2), 1);
    Serial.println(" g");

  }
}

void weighing_menu_choice(){
  Serial.println("\n------------------------------------------------------\n");
  Serial.println("Enter a menu choice: ");
  Serial.println("(w)ater dispensing mode...");
  Serial.println("(d)ry dispensing mode...");
  Serial.println("(j)ust weight mode...");
//  Serial.println(Serial.available());
  Serial.read(); //I think this is neccesary to pull \n off input
  while (Serial.available() <= 1){} // wait until input
  char user_char = Serial.read();
  switch(user_char){
      case 'w':  //Enter water dispensing mode
        Serial.println("Water dispense mode...");
        break;
      case 'd':
        dry_dispense_mode();
        break;
      case 'j':
        Serial.println("Just weight mode...");
        just_weight_mode(false);
        break;
      case '\n':
        break;
      default:
        Serial.println("Error! Incorrect input...");
        break;
      }
}
void menu(){
  Serial.println("\n------------------------------------------------------\n");
  Serial.println("Enter command: ");
  Serial.println("(t)are the scale...");
  Serial.println("(c)alibration mode...");
  Serial.println("(e)nter water amount...");
  Serial.println("(d)ry weight input...");
  Serial.println("(p)rint out current values...");
  Serial.println("(w)eighing mode submenu...");

  print_settings();
  }



void dry_dispense_mode(){
  Serial.println("Dry dispense mode...");
  Serial.print("Dry weight: ");
  Serial.println(dry_weight);
  Serial.println("Press button to start...");

  int duration = 3000; // 3000ms duration
  int temp_weight = 0;
  
//  int buttonState = digitalRead(buttonPin);
//  Serial.println(buttonState);
  while(true){
//    int lastButtonState = debounce();
    debounce();
//    Serial.print(lastButtonState);
//    Serial.print(" -- ");
//    Serial.println(buttonState);

    if(lastButtonState != buttonState){ 
      Serial.println("Button pressed");
      break;  // if button was pressed then released then break
      }
    }//Wait until button is pressed
//  Serial.println("exited pre press loop");
//  Serial.println(debounce());
//  if(buttonState){
////    Serial.println("entered if statement");
//
//    while(!debounce()){ //Wait until button is released
////      Serial.println("Button pressed...");
//    }
    while(temp_weight <= dry_weight){
//      lastButtonState = debounce();
//      Serial.print(lastButtonState);
//      Serial.print(" -- ");
//      Serial.println(buttonState);
      debounce();
      if(lastButtonState || buttonState){
        Serial.println("Emergency pause...");
//        Serial.print(lastButtonState);
//        Serial.print(" -- ");
//        Serial.println(buttonState);
        buttonState = 0;
        buttonState = lastButtonState;
        while(true){ // Pause until button press again
//          Serial.println("running loop to check if resuming");
          debounce();
//          Serial.print(lastButtonState);
//          Serial.print(" -- ");
//          Serial.println(buttonState);
//          Serial.println(lastButtonState);
//          buttonState = digitalRead(buttonPin);
          if(lastButtonState || buttonState){
            Serial.println("Resuming...");
            break;
            } //stop the pause
          }
      }
      delay(200);
      Serial.print(temp_weight);
      Serial.println('g');
      temp_weight += 10;
      }
}



int debounce(){

    // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
//  digitalWrite(ledPin, ledState);
  

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

  return !ledState;
  
}


void get_dry_weight() {
  Serial.println("Enter a dry weight(g): ");
  dry_weight = get_int();
  Serial.print("Dry weight set to: ");
  Serial.print(dry_weight);
  Serial.println('g');

}

int get_user_water(){
  Serial.println("Enter an amount of water to dispense(mL): ");
  user_water = get_int();
  Serial.print("Set water amount to: ");
  Serial.print(user_water);
  Serial.println("mL");
}

void print_settings(){
  Serial.println("\n------------------------------------------------------\n");
  Serial.print("Dry weight: ");
  Serial.print(dry_weight);
  Serial.println("g");

  Serial.print("Water amount: ");
  Serial.print(user_water);
  Serial.println("mL");

  }

int get_int(){
    while (Serial.available() <= 1)
      {
//        Serial.print(Serial.read());
      }
    int user = Serial.parseInt();
    return user;
//    Serial.print("Is a number: ");
//      Serial.println(user);
    
//    int user = Serial.read();
//    if(user >= '0' && user <='9'){
//      user = user - '0';
//      Serial.print("Is a number: ");
//      Serial.println(user);
//      user += Serial.parseInt();
//      Serial.print("Is a number: ");
//      Serial.println(user);
      
}
