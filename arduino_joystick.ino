/**
 * This is a code stub for BLUEsat's orientation day. It provides a basis for code that runs on an arduino to take
 * inputs from sensors and use them to drive the BLUEtounge Rover.
 * @date: 24/02/2017
 * @author: Harry J.E Day, William Miles
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

    uint8_t drive_mode;

    uint16_t MAGIC_2;
} __attribute__((packed)) Cmd;
//  NOTE: on some arduinos double is 4 bytes here, so we need to use float
// otherwise our struct changes size and breaks everything

/**
 * This is used as part of the serialisation process. 
 * It alows us to covert the struct used for communication into bytes we can send across the serial port.
 */
typedef struct _msg_adapter {
    union {
        Cmd cmd;
        uint8_t buffer[sizeof(Cmd)];
    };
} Msg_Adapter;

Cmd joystick_loop();
void send_msg(Cmd msg);

/**
 * Code run when the arduino boots.
 */
void setup() {
    // start the serial driver
    Serial.begin(9600);
}

/**
 * Main loop of the arduino. Handles communication and calling joystick_loop.
 */
void loop() {
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
 * Don't modify this, uses the serial connection to send the command to the rover
 * @param msg the struct to send
 */
void send_msg(Cmd msg) {
    Msg_Adapter adapter;
    adapter.cmd = msg;
    Serial.write(adapter.buffer, sizeof(Cmd));
}

/**
 * Implement your setup code here.
 */
void joystick_setup() {

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
    command.drive_mode = 0;

    return command;
}
