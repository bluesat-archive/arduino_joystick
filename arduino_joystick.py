from sys import argv
from time import sleep
from serial import Serial
from struct import unpack


class SIO(object):
    MSGLEN = 11
    M1 = 0xBEEF
    M2 = 0xF00D

    def __init__(self, serial_port):
        self.ser = Serial(serial_port)
        self.count = 0
        self.prev = bytes((0,))

    def get_msg(self):
        self.ser.reset_input_buffer()
        while True:
            curr = self.ser.read()
            (m,) = unpack("<H", self.prev + curr)

            if m != self.M1:
                self.prev = curr
                continue

            msg_bytes = self.ser.read(self.MSGLEN)
            (x, y, mode, m) = unpack("<ffBH", msg_bytes)

            if m == self.M2:
                print("R:(%f,%f),%d" % (x, y, mode))
            else:
                print("Invalid magic on packet %d" % self.count)

            self.prev = self.ser.read()
            self.count += 1

            break


if __name__ == "__main__":
    sio = SIO(argv[1])
    while True:
        sio.get_msg()
        sleep(1)
