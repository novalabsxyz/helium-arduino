#include <Arduino.h>
// #include "carbon.h"
#include "helium.h"

namespace helium {

    Atom::Atom(int tx, int rx) : serial(rx, tx) {
        serial.begin(115200);
    }

    enum carbon_connect_status Atom::connect(struct connection *connection) {
        return carbon_connect(&ctx, connection);
    }

    bool Atom::is_connected() {
        return carbon_connected(&ctx) == carbon_connected_CONNECTED;
    }

    enum carbon_sleep_status Atom::sleep(struct connection *connection) {
        return carbon_sleep(&ctx, connection);
    }


    enum carbon_send_status Atom::send(void const *data, size_t len) {
        return carbon_send(&ctx, data, len);
    }

}

size_t carbon_read(void *param, void *buf, size_t len) {
    return 0;
}

size_t carbon_write(void *param, void *buf, size_t len) {
    return 0;
}

void carbon_wait_ms(uint32_t ms) {
    return;
}
