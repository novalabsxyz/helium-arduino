#include "Arduino.h"
#include "helium.h"

int main() {
    helium::Atom atom(0, 1, -1);
    helium::AtomFrame frame;
    const char *data = "HELLO";

    atom.enable();
    // enum carbon_connect_status connect_status =
    atom.connect();

    // printf("CONNECT: %d\n", connect_status);

    frame.append(data, strlen(data));
    // enum carbon_send_data_status send_status =
    atom.send(frame);

    // printf("SEND: %d\n", send_status);

}
