#ifndef HELIUM_ATOM_H
#define HELIUM_ATOM_H

#include "Arduino.h"
#include "carbon/carbon.h"

#define HELIUM_BAUD_RATE 115200

class Helium {
public:
    /** Create an Atom connected to the specified Serial port. The
     * power pin is used to turn the power to the Atom on or off.
     *
     * @param serial Serial port to connect to
     * @param power Pin to turn control power to the atom
     */
    Helium(Stream *serial);

    bool connected();
    enum carbon_connect_status connect();
    enum carbon_sleep_status sleep();
    enum carbon_info_status info(struct res_info *info);

    enum carbon_send_status send(void const *data, size_t len);

private:
    struct carbon_ctx _ctx;
};

#endif
