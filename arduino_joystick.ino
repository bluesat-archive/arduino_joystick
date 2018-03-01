/**
 * This is a code stub for BLUEsat's orientation day. It provides a basis for code that runs on an arduino to take
 * inputs from sensors and use them to drive the BLUEtounge Rover.
 * @date: 24/02/2017
 * @author: Harry J.E Day
 */
#include <Arduino.h>
#include <inttypes.h>

const uint16_t MAGIC_1 = 0xBEEF;
const uint16_t MAGIC_2 = 0xF00D;

/**
 * Struct that represents the commands given to all the rover's joints
 */
typedef struct _cmd {

    uint16_t MAGIC_1;

    // the direction we want the rover to drive as a vector
    // the units are in m/s
    struct _drive_vector {
        float x;
        float y;
    } __attribute__((packed)) drive_vector;

    // the arm's rotation (rads)
    float arm_rotation;
    // the arm's lower extension speed
    // (pwm. 1500 is stop, 2000 is full outwards, 1000 is full backwards)
    float arm_lower_pwm;
    // the arm's upper extension speed
    // (pwm. 1500 is stop, 2000 is full outwards, 1000 is full backwards)
    float arm_upper_pwm;

    // the claw's rotation speed (rad/s)
    float claw_rot_speed;
    //the claw's grip position (in pwm - get someone to explain this to you)
    float claw_grip_pwm;

    uint16_t MAGIC_2;
} __attribute__((packed)) Cmd;

//  NOTE: on some arduinos double is 4 bytes here, so we need to use float
// otherwise our struct changes size and breaks everything

typedef struct _to_arduino {
   uint16_t magic;
} __attribute__((packed)) To_Arduino;

/**
 * This is used as part of the serialisation process. 
 * It alows us to covert the struct used for communication into bytes we can send across the serial port.
 */
typedef struct _msg_adaptor {
    union {
        Cmd cmd;
        uint8_t buffer[sizeof(Cmd)];
    };
} Msg_Adaptor;

Cmd joystick_loop();
void send_msg(Cmd msg);

/**
 * Code run when the arduino boots.
 */
void setup() {
    // start the serial driver
    Serial.begin(19200);
}

/**
 * Main loop of the arduino. Handles communication and calling joystick_loop.
 */
void loop() {
    // we use this so the computer can tell where the message starts
    uint8_t MAGIC[2] = {0xBE, 0xEF};
    
    bool found_first = false;
    int bytes_read = 0;
    
    // read until there are no bytes left
    while(Serial.available() > 0) {
        uint8_t val = Serial.read();
        if(val == MAGIC[1]) {
          bytes_read = 0;
          found_first = true;
        }

        // once we've found the start read out the bytes
        if(found_first) {
          if(bytes_read >=  sizeof(To_Arduino))  {
              break;
          }
        }
    }
    
    //declare a cmd struct
    Cmd cmd;
    // zero the command struct
    memset(&cmd, 0, sizeof(Cmd));
    
    // get the values from the user supplied code
    cmd = joystick_loop();
    cmd.MAGIC_1 = MAGIC_1;
    cmd.MAGIC_2 = MAGIC_2;
    
    // write to the buffer
    send_msg(cmd);
}

/**
 * Implement your code in this function. Does all the sensor processing and provides commands for the rover
 * @return the commands for the rover's joints
 */
Cmd joystick_loop() {
    Cmd command;

    // Add you code here to control the rover
    // this should make the rover turn at a 45deg whilst driving forward
    command.drive_vector.x = 1.0;
    command.drive_vector.y = 1.0;

    return command;
}

/**
 * Don't modify this, uses the serial connection to send the command to the rover
 * @param msg the struct to send
 */
void send_msg(Cmd msg) {
    Msg_Adaptor adaptor;
    adaptor.cmd = msg;
    Serial.write(adaptor.buffer, sizeof(Cmd));
}



