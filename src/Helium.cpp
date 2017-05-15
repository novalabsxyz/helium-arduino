#include "helium.h"
#include "Arduino.h"
#include "carbon/carbon.h"

bool carbon_serial_readable(void *param) {
    Stream *serial = (Stream *)param;
    return serial->available() > 0;
}

bool carbon_serial_getc(void *param, uint8_t *ch) {
    Stream *serial = (Stream *)param;
    *ch = serial->read();
    return *ch != -1;
}

bool carbon_serial_putc(void *param, uint8_t ch) {
    Stream *serial = (Stream *)param;
    return serial->write(ch) == 1;
}

void carbon_wait_us(void *param, uint32_t ms) {
    (void)param;
    delay(ms);
}


Helium::Helium(Stream *stream) {
    carbon_init(&_ctx, (void *)stream);
}

enum carbon_connect_status Helium::connect() {
    return carbon_connect(&_ctx, NULL);
}

enum carbon_info_status Helium::info(struct res_info *info) {
    return carbon_info(&_ctx, info);
}

bool Helium::connected() {
    return carbon_connected(&_ctx) == carbon_connected_CONNECTED;
}

enum carbon_sleep_status Helium::sleep() {
    return carbon_sleep(&_ctx, NULL);
}


enum carbon_send_status Helium::send(void const *data, size_t len) {
    return carbon_send(&_ctx, data, len);
}
