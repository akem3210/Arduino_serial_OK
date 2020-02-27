// TODO: add events handler/manager so we can seemlesly 
// update hardware(motors based on curve speed) or LED blink etc,
// and receive commands at the same time.
#include <Stepper.h>

// LED attached to the board
int led13 = 13;

// Stepper
const int stepsPerRevolution = 200;
Stepper myStepperA(stepsPerRevolution, 4,5,6,7); 
Stepper myStepperB(stepsPerRevolution, 8,9,10,11); 

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication
  Serial.begin(115200); // default 9600
  Serial.flush();
  
  // Attached LED
  pinMode(led13, OUTPUT);
  // Setpper speed
  myStepperA.setSpeed(60);
  myStepperB.setSpeed(60);
}

// ACTIONS /////////////////////////////////////////////////////////////////////////////////////
void led_on() {
  digitalWrite(led13, HIGH);
}

void led_off() {
  digitalWrite(led13, LOW);
}

void stepperA_speed(int wanted_speed){
  myStepperA.setSpeed(wanted_speed);
}

void stepperA_move(int wanted_steps){
  myStepperA.step(wanted_steps);
}

void stepperB_speed(int wanted_speed){
  myStepperB.setSpeed(wanted_speed);
}

void stepperB_move(int wanted_steps){
  myStepperB.step(wanted_steps);
}

// STRING //////////////////////////////////////////////////////////////////////////////////////
// Return (0 terminated) size of char[] string, 
#define STR_MAX_SIZE 1024

int mystr_len(char s[]) {
  int size = 0;
  while(size < STR_MAX_SIZE) {
    if(s[size] == 0) {
      break;
    }
    size += 1;
  }
  return size;
}

// Return -1 if the two strings are equals, 
// or the position of the first difference
// position may be 0 terminating char on smaller one.
int mystr_cmp(char s1[], char s2[]) {
  int i;
  int s1l = mystr_len(s1),
      s2l = mystr_len(s2);
  // It doesn't really matter if s1l or s2l here
  for(i=0;i<s1l;i++) {
    if(s1[i] != s2[i]) {
      return i;
    }
  }
  if(s1l != s2l){
    return i + 1;
  }
  return -1;
}

// Return -1 if char not found, 
// or the position of the first occurence of the character c in s1
int mystr_find_char(char s1[], char c){
  int i = 0;
  char p = -1;
  while(i < STR_MAX_SIZE) {
    if(s1[i] == 0) {
      break;
    }
    if(s1[i] == c) {
      p = i;
      break;
    }
    i += 1;
  }
  return p;
}

// Return -1 if char[] not found, 
// or the position of the first occurence of the string s2 in s1
int mystr_find_str(char s1[], char s2[]){
  int i = 0, j = 0;
  int s1l = mystr_len(s1),
      s2l = mystr_len(s2);
  boolean found = true;
  char p = -1;
  while(i < STR_MAX_SIZE) {
    if(s1[i] == 0) {
      break;
    }
    // Break if there is not enough space remaining in the string for it to be there(no need to continue testing)
    if((s1l - i) < s2l) {
      break;
    }
    // Test if the s2 is at this position
    found = true;
    for(j=0;j<s2l;j++) {
      if(s1[i+j] != s2[j]) {
        found = false;
        break;
      }
    }
    if(found == true) {
      p = i;
      break;
    }
    i += 1;
  }
  return p;
}

// SERIAL //////////////////////////////////////////////////////////////////////////////////////
#define CMD_MAX_SIZE 1024
boolean serial_cmd_begin = false,
        serial_cmd_end = false;
char serial_cmd[CMD_MAX_SIZE];
int serial_cmd_char_pos = 0;

// Run from read_serial_cmd
void parse_serial_cmd() {
  // We look at serial_cmd, and do actions on commands.
  if(mystr_find_str(serial_cmd, "LED_ON") == 0) { led_on(); }
  if(mystr_find_str(serial_cmd, "LED_OFF") == 0) { led_off(); }

  if(mystr_find_str(serial_cmd, "STEPS_A=") == 0) {
    /**** Simple version, no error check:
    int wanted_steps = atoi(&serial_cmd[mystr_find_char(serial_cmd, '=') + 1]);
    stepperA_move(wanted_steps);
    ****/
    // Position of "=" sign in the cmd string we received.
    int pos_equal = mystr_find_char(serial_cmd, '=');
    // Run the command only if we have more data after the "=" sign (numbers...).
    if(mystr_len(serial_cmd) > pos_equal){
      int wanted_steps = atoi(&serial_cmd[pos_equal + 1]);
      stepperA_move(wanted_steps);
    }
  }

  if(mystr_find_str(serial_cmd, "SPEED_A=") == 0) {
    // Position of "=" sign in the cmd string we received.
    int pos_equal = mystr_find_char(serial_cmd, '=');
    // Run the command only if we have more data after the "=" sign (numbers...).
    if(mystr_len(serial_cmd) > pos_equal){
      int wanted_speed = atoi(&serial_cmd[pos_equal + 1]);
      stepperA_speed(wanted_speed);
    }
  }

  if(mystr_find_str(serial_cmd, "STEPS_B=") == 0) {
    // Position of "=" sign in the cmd string we received.
    int pos_equal = mystr_find_char(serial_cmd, '=');
    // Run the command only if we have more data after the "=" sign (numbers...).
    if(mystr_len(serial_cmd) > pos_equal){
      int wanted_steps = atoi(&serial_cmd[pos_equal + 1]);
      stepperB_move(wanted_steps);
    }
  }

  if(mystr_find_str(serial_cmd, "SPEED_B=") == 0) {
    // Position of "=" sign in the cmd string we received.
    int pos_equal = mystr_find_char(serial_cmd, '=');
    // Run the command only if we have more data after the "=" sign (numbers...).
    if(mystr_len(serial_cmd) > pos_equal){
      int wanted_speed = atoi(&serial_cmd[pos_equal + 1]);
      stepperB_speed(wanted_speed);
    }
  }

}

void read_serial_cmd(int size = CMD_MAX_SIZE, boolean parse = false, boolean echo = false) {
  for(int i = 0; i < size; i++) {
    if(Serial.available()) {
      char c = Serial.read();

      if(c == '{') {
        serial_cmd_begin = true;
        serial_cmd_end = false;
        serial_cmd_char_pos = 0;
      }
      else if(c == '}') { 
        if(serial_cmd_begin == true) {
          serial_cmd_begin = false;
          serial_cmd_end = true;
          serial_cmd[serial_cmd_char_pos] = 0;
          break;
        }
      }
      // also include { and } but not it's not important since tested after the 2 others
      // Otherwise we get lots of "ÿÿÿÿÿÿÿÿÿ" especially with delay with reads and writes
      else if((c > 31) && (c < 126)) { 
        if(serial_cmd_begin == true) {
          serial_cmd[serial_cmd_char_pos] = c;
          serial_cmd_char_pos += 1;
        }
      }
    }
  }
  
  if(serial_cmd_end == true) {
    if(parse == true) {
      parse_serial_cmd();
    }
    if(echo == true) {
      Serial.println(&serial_cmd[0]);
    }
    memset(serial_cmd, 0, CMD_MAX_SIZE);
    serial_cmd_end = false;
  }

  Serial.flush();
}

// the loop routine runs over and over again forever:
void loop() {
  read_serial_cmd(CMD_MAX_SIZE, true, true);
////  delay(100);
}


