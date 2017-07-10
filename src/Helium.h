/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#ifndef HELIUM_H
#define HELIUM_H

#include "Arduino.h"
#include "helium-client/helium-client.h"

#if defined(ARDUINO_AVR_UNO)
#include <SoftwareSerial.h>
#endif

/**
 * \class Helium
 *
 * \brief Enables communication with the Helium Atom
 *
 * This class offers the basic ability to communicate with the Helium
 * Atom module.
 *
 * See the @ref Basic.ino sketch for an example on how to create a
 * channel and send to it.
 *
 */
class Helium
{
  public:
    /** Create a Helium instance on a hardware serial port.
     *
     * @param serial The hardware serial port to use
     */
    Helium(HardwareSerial * serial);

#if defined(ARDUINO_AVR_UNO)
    /** Create a Helium instance on a software serial port
     *
     * @note SoftwareSerial is only exposed for the Arduino UNO.
     *
     * @param serial The software serial port to use
     */
    Helium(SoftwareSerial * serial);
#endif

    /** Begin communication with the Helium Atom.
     *
     * This starts communication with the Helium Atom and upgrades the
     * baud rate used to talk to the Atom to the given value.
     *
     * @param baud The baud rate to use.
     */
    int begin(enum helium_baud baud);

    /** Get information on the Helium atom
     *
     * Gets basic information about the Helium Atom such as it's MAC
     * address and the current network time, if connected.
     *
     * @param info The information structure to fill out
     */
    int info(struct helium_info * info);

    /** Connect the Helium Atom to the network.
     *
     * This tries to connect the Helium Atom to a nearby Element and
     * the Helium Network. Connections can be quick or slow. A quick
     * connection attempt to re-connect to the network using revious
     * _connection_ state which was retrieved using %sleep. If the
     * quick connect was not requested or if it fails a normal, slower
     * connection attempt is made.
     *
     * Connect is an asynchronous operation. The given are a
     * convenience to attempt a number of retries before returning the
     * current connection state.
     *
     * @param connection A previously retrieved connection structure
     *        that reflects sleep state.
     *
     * @param retries The number of retries to attempt to connect.
     *
     * @return helium_connect_status
     */
    int connect(struct connection * connection = NULL,
                uint32_t            retries    = HELIUM_POLL_RETRIES_5S);

    /** Check if the Atom is connected to the network
     *
     * Checks whether the Atom is connected to the network.
     *
     * @returns true if connected, false if not connected
     */
    bool connected();

    /** Disconnects the Atom from the network and puts it in sleep mode.
     *
     * This disconnects the Helium Atom from the network and puts it
     * in sleep mode. If a connection structure is given, it is filled
     * with sleep state which can be used in a sub-sequent connect
     * call to quick connect to the network.
     *
     * @param connection Sleep connection buffer to fill in.
     */
    int sleep(struct connection * connection = NULL);

    /** Checks if the Atom needs resetting.
     *
     * Checks if the Atom needs a reset. This is set over the air by
     * the Helium network when the Helium Atom has a firmware update
     * ready to apply.
     *
     * @returns true if the Atom needs to be reset, false otherwise.
     */
    bool needs_reset();

    /** Resets the Atom.
     *
     * This resets the Atom. Use this method when `needs_reset`
     * returns true to apply any pending firmware updates to the Atom.
     */
    int reset();

  private:
    struct helium_ctx _ctx;

    friend class Channel;
};

/**
 * \class Channel
 *
 * \brief A Channel to an IoT platform.
 *
 * Channels represent a delivery mechanism from the device to a number
 * of supported back-end IoT platforms.
 *
 * To use a channel, make sure you have it set up in your Helium
 * Dashboard and then call Channel#begin with the channel name as you
 * configured it on the Helium Dashboard. This will automatically
 * ensure that the device is securely authenticated and registered
 * with the channel.
 *
 */
class Channel
{
  public:
    /** Construct a channel.
     *
     * @param helium The Helium Atom to communicate with
     */
    Channel(Helium * helium);

    /** Begin communicating over a channel
     *
     * Always call this method before calling Channel#send. Beginning
     * communication will ensure that the device is authenticated and
     * registered with the channel with the given name as configured
     * in the Helium Dashboard.
     *
     * The `result` value will be 0 if the channel was created
     * successfully and non-`0` otherwise.
     *
     * @param name The name of the channel to authenticate with.
     * @param result An out parameter for the result of the request.
     */
    int begin(const char * name, int8_t * result);

    /** Send data to this channel.
     *
     * Send data to a given channel. The given data is sent to the
     * configured channel and the result as returned by the channel is
     * put in the `result` output variable. The result value is `0`
     * for success and non-`0` if an error occurred.
     *
     * @note The maximum number of bytes that can be transmitted is
     * limited to HELIUM_MAX_DATA_SIZE (about 100 bytes).
     *
     * @param data The data bytes to send
     * @param len The number of bytes in data to send
     * @param result An out parameter for the result returned by the channel
     */
    int send(void const * data, size_t len, int8_t * result);

    /** Asynchronous begin method.
     *
     * The Channel#begin(const char *) method is a synchronous version
     * of this method. Sending a request to a channel returns a
     * `token` which can be used in a subsequent call to #poll() to
     * check for results from the remote channel.
     *
     * @param name The name of the channel to authenticate with.
     * @param token The output parameter for the pending result token.
     */
    int begin(const char * name, uint16_t * token);

    /** Asynchornous send method.
     *
     * This is the asynchronous version of the #send(void const *data,
     * size_t int8_t *) method.
     *
     * @param data The data bytes to send
     * @param len The number of bytes in data to send
     * @param token The output parameter for the pending result token
     */
    int send(void const * data, size_t len, uint16_t * token);

    /** Poll for a result token.
     *
     * This polls the Helium Atom for the given number of retries for
     * the result of a given `token`. Use HELIUM_POLL_RETRIES_5S for
     * the recommended number of retries.
     *
     * If succcessful the result will be helium_status_OK and the
     * result value will be set to the result of the original request
     * that the token represents.
     *
     * @param token The token to check for
     * @param result[out] The result of the given request token
     * @param retries The number of times to retry
     */
    int poll_result(uint16_t token,
                    int8_t * result,
                    uint32_t retries = HELIUM_POLL_RETRIES_5S);

    /** Poll for data on a channel.
     *
     * This polls the Helium Atom for the given number of retries for
     * any data on a given `channel`.
     *
     * If successful the result will be helium_status_OK and the given
     * `data` buffer will be filled with the received data. The `used`
     * out parameter will be set to the number of bytes read. Note
     * that the maximum number of bytes that can be sent is set by
     * HELIUM_MAX_DATA_SIZE.
     *
     * @param channel_id The channel to check for
     * @param[out] data The data buffer to fill with received data
     * @param len The available length of the data buffer
     * @param[out] used On success the number of bytes used up in data
     * @param retries The number of times to retry
     */
    int poll_data(uint16_t channel_id,
                  void *   data,
                  size_t   len,
                  size_t * used,
                  uint32_t retries = HELIUM_POLL_RETRIES_5S);

  private:
    Helium * _helium;
    int8_t   _channel_id;
};

#endif // HELIUM_H

/** @example Basic.ino
 *
 * shows a basic example of how to construct Helium, an MQTT channel
 * and transmitting data to that channel
 */
