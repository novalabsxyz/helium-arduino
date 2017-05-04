#ifndef HELIUM_ATOM_H
#define HELIUM_ATOM_H

#include <SoftwareSerial.h>
#include <carbon.h>

#define ATOM_MAX_DATA_SIZE CARBON_MAX_DATA_SIZE

namespace helium {

    class Atom {
    public:
        /** Create an Atom connected to the specified Serial port. The
         * power pin is used to turn the power to the Atom on or off.
         *
         * @param serial Serial port to connect to
         * @param power Pin to turn control power to the atom
         */
        Atom(int tx, int rx);

        bool is_connected();
        enum carbon_connect_status connect(struct connection *connection);
        enum carbon_sleep_status sleep(struct connection *connection);

        enum carbon_send_status send(void const *data, size_t len);

    private:
        struct carbon_ctx ctx;
        SoftwareSerial serial;
    };

}

#endif
