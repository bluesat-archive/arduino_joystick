/*
 * BLUESat Induction Day 2019 joystick activity for software recruits. 
 * Author: Ricky Qu
 */

/*
 * Don't touch this stuff.
 */
const int UP = 2;
const int DOWN = 7;
const int LEFT = 4;
const int RIGHT = 5;
const int X_READ = A0;
const int Y_READ = A1;
const int NEUTRAL = 512;
const int THRESHOLD = 5;

void setup() {
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);

  digitalWrite(RIGHT, HIGH);
  digitalWrite(LEFT, HIGH);
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, HIGH);
}

void loop() {
  /*
   * Put your code here.
   */
}
