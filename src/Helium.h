#ifndef HELIUM_ATOM_H
#define HELIUM_ATOM_H

#include "Arduino.h"
#include "carbon/carbon.h"

#define HELIUM_BAUD_RATE 115200

class Channel;

class Helium
{
  public:
    /** Create an Atom connected to the specified Serial port.
     *
     * @param serial Serial port to connect to
     */
    Helium(Stream * serial);

    bool connected();
    int  connect();
    int  sleep();
    int info(struct res_info * info);

    // int send(void const *data, size_t len);
  private:
    struct carbon_ctx _ctx;

    friend class Channel;
};

class Channel
{
  public:
    Channel(Helium * helium);

    int begin(const char *name);
    int send(void const * data, size_t len);

  private:
    Helium *_helium;
    uint8_t _id;
};

#endif
