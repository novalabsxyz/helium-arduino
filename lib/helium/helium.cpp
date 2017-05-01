#include "helium.h"
#include "Arduino.h"

static void transport_delay_quarter_seconds(void *param, uint32_t quarters) {
    (void)param;
    delay(250 * quarters);
}

static uint64_t transport_get_system_time(void *param) {
    printf("RETURNING TIME\n");
    return 0;
}

int logging_vprintf(void * param, char const * fmt, va_list ap) {
    vprintf(fmt, ap);
    return 0;
}

#define LONG_TIMEOUT  2000
#define SHORT_TIMEOUT 10
static bool wait_for_byte(SoftwareSerial &serial, size_t timeout) {
    for (size_t i = 0; i < timeout; i++) {
        if (serial.available() > 0) {
            return true;
        } else {
            delayMicroseconds(50);
        }
    }

    return false;
}

namespace helium {

    AtomFrame::AtomFrame() : used(0) {
        static uint8_t header[] = {0x82, 0, 0, 0};
        this->append(header, sizeof(header));
    }

    size_t AtomFrame::append(void const *data, size_t len) {
        size_t available = sizeof(frame) - used;
        if (available < len) {
            len = available;
        }
        memcpy(frame + used, data, len);
        used += len;
        frame[2] = used;
        return len;
    }

    void AtomFrame::sequence_set(uint8_t sequence) {
        frame[3] = sequence;
    }

    void AtomFrame::reset() {
        used = 0;
        frame[2] = used;
    }

    //
    // Atom
    //
    Atom::Atom(int tx, int rx, int powerPin)
        : serial(rx, tx), power(powerPin), sequence(0) {

        if (power >= 0) {
            pinMode(power, OUTPUT);
        }

        carbon_iface.param = (void *)this;
        carbon_iface.init = transport_init;
        carbon_iface.enable = transport_enable;
        carbon_iface.disable = transport_disable;
        carbon_iface.delay_quarter_seconds = transport_delay_quarter_seconds;
        carbon_iface.get_system_time = transport_get_system_time;
        carbon_iface.tx = transport_tx;
        carbon_iface.rx = transport_rx;
        carbon_iface.vprintf = logging_vprintf;
        carbon_init(&carbon_ctx, &carbon_iface);

        serial.begin(115200);
    }

    void Atom::enable() {
        carbon_enable(&carbon_ctx);
    }

    void Atom::disable() {
        carbon_disable(&carbon_ctx);
    }

    enum carbon_connect_status Atom::connect() {
        return carbon_connect(&carbon_ctx);
    }

    bool Atom::is_connected() {
        return carbon_connection_state(&carbon_ctx) == carbon_connection_state_CONNECTED;
    }

    enum carbon_sleep_status Atom::sleep() {
        return carbon_sleep(&carbon_ctx);
    }


    enum carbon_send_data_status Atom::send(AtomFrame &frame) {
        if (sequence == 0) {
            srand(carbon_calendar_time(&carbon_ctx));
            sequence = rand();
        } else {
            sequence += 1;
        }
        frame.sequence_set(sequence);
        return carbon_send_data(&carbon_ctx, frame.frame, frame.used);
    }

    //
    // Callback methods for carbon_iface
    //
    void Atom::transport_init(void *param) {
        (void)param;
        // Do nothing
    }

    void Atom::transport_enable(void *param) {
        Atom *atom = (Atom *)param;
        if (atom->power >= 0) {
            digitalWrite(atom->power, 1);
        }
    }

    void Atom::transport_disable(void *param) {
        Atom *atom = (Atom *)param;
        if (atom->power >= 0) {
            digitalWrite(atom->power, 0);
        }
    }

    enum carbon_iface_tx_stat Atom::transport_tx(void * param, struct carbon_tx_buffer const * buffer) {
        Atom *atom = (Atom *)param;
        SoftwareSerial &ser = atom->serial;
        uint8_t *ptr = (uint8_t *)buffer->data;
        const uint8_t *end = ptr + buffer->used;
        while (ptr != end) {
            ser.write(*ptr++);
        }
        if (ptr < end) {
            return carbon_iface_tx_ERR;
        }

        return carbon_iface_tx_OK;
    }

    enum carbon_iface_rx_stat Atom::transport_rx(void * param, struct carbon_rx_buffer * buffer) {
        Atom *atom = (Atom *)param;
        SoftwareSerial &serial = atom->serial;
        buffer->used = 0;
        if (!wait_for_byte(serial, LONG_TIMEOUT)) {
            return carbon_iface_rx_ERR_TIMEOUT;
        }

        do {
            if (!wait_for_byte(serial, SHORT_TIMEOUT)) {
                buffer->stat = carbon_iface_rx_DONE;
                // printf("RX: DONE %d\n", buffer->used);
                return carbon_iface_rx_DONE;
            } else {
                uint8_t val = serial.read();
                buffer->data[buffer->used++] = val;
            }
        } while(buffer->used < sizeof(buffer->data));
        buffer->stat = carbon_iface_rx_MORE;
        return carbon_iface_rx_MORE;
    }


}
