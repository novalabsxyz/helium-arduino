#ifndef HELIUM_ATOM_H
#define HELIUM_ATOM_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "carbon.h"

#define ATOM_MAX_FRAME_SIZE CARBON_TX_MAX_SIZE

namespace helium {

    class AtomFrame {
    public:
        AtomFrame();
        size_t append(void const *data, size_t len);
        void sequence_set(uint8_t sequence);
        void reset();

        uint8_t used;
        uint8_t sequence;
        uint8_t frame[ATOM_MAX_FRAME_SIZE];


    };

    class Atom {
    public:
        /** Create an Atom connected to the specified Serial port. The
         * power pin is used to turn the power to the Atom on or off.
         *
         * @param serial Serial port to connect to
         * @param power Pin to turn control power to the atom
         */
        Atom(int tx, int rx, int power_pin);

        void enable();
        void disable();

        enum carbon_connect_status connect();
        bool is_connected();
        enum carbon_sleep_status sleep();

        enum carbon_send_data_status send(AtomFrame &frame);

        virtual ~Atom() {
            disable();
        }

    private:
        SoftwareSerial serial;
        int power;

        uint8_t sequence;
        struct carbon_ctx carbon_ctx;
        struct carbon_transport_iface carbon_iface;
        static void transport_init(void *param);
        static void transport_enable(void *param);
        static void transport_disable(void *param);
        static enum carbon_iface_tx_stat transport_tx(void * param, struct carbon_tx_buffer const * buffer);
        static enum carbon_iface_rx_stat transport_rx(void * param, struct carbon_rx_buffer * buffer);
    };
}

#endif
