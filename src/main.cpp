#include "Arduino.h"
#include "helium.h"

int main() {
    helium::Atom atom(0, 1);
    const char *data = "HELLO";

    // enum carbon_connect_status connect_status =
    atom.connect(NULL);

    // printf("CONNECT: %d\n", connect_status);

    // enum carbon_send_data_status send_status =
    atom.send(data, strlen(data));

    // printf("SEND: %d\n", send_status);

}
