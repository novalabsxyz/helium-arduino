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
    friend class Config;
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
     * `token` which can be used in a subsequent call to
     * #poll_result() to check for results from the remote channel.
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
     * @param[out] result The result of the given request token
     * @param retries The number of times to retry
     */
    int poll_result(uint16_t token,
                    int8_t * result,
                    uint32_t retries = HELIUM_POLL_RETRIES_5S);

    /** Poll for data on a channel.
     *
     * This polls the Helium Atom for the given number of retries for
     * any data on the channel.
     *
     * If successful the result will be helium_status_OK and the given
     * `data` buffer will be filled with the received data. The `used`
     * out parameter will be set to the number of bytes read. Note
     * that the maximum number of bytes that can be sent is set by
     * HELIUM_MAX_DATA_SIZE.
     *
     * @param[out] data The data buffer to fill with received data
     * @param len The available length of the data buffer
     * @param[out] used On success the number of bytes used up in data
     * @param retries The number of times to retry
     */
    int poll_data(void *   data,
                  size_t   len,
                  size_t * used,
                  uint32_t retries = HELIUM_POLL_RETRIES_5S);

  private:
    Helium * _helium;
    int8_t   _channel_id;

    friend class Config;
};


enum config_poll_get_status
{
    config_status_POLL_FOUND      = 0,
    config_status_POLL_FOUND_NULL = -1,
    config_status_POLL_ERR_TYPE   = -2,
};


/**
 * \class Config
 *
 * \brief A Channel Configuration
 *
 * Channels can have configuration data for the Helium Atom
 * available. Depending on the IoT platform the configuration data is
 * a representation of device configuration. Other terms used are
 * device "twins" or "shadows".
 *
 * To use a channel configuration construct it with a channel that
 * supports configuration and use the get methods to retrieve
 * configuration values from the IoT platform's device
 * representation. Use the set methods to set values in the IoT
 * platform's representation of the device.
 *
 * Note that most IoT platforms represent sets and gets in different
 * namespaces. The get methods represent the IoT platform's desired or
 * expected namespace of the device, while the set methods are
 * reflected in the actual, or current namespace of the device.
 *
 */
class Config
{
  public:
    /** Construct a Configuration
     *
     * @param channel The channel to get/set configuration with
     */
    Config(Channel * channel);

    /** Get an integer configuration value
     *
     * @param key The configuration key to get
     * @param[out] value The target for the received value
     * @param default_value The default value in case of errors
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. If the result is > 0 the result code is
     *     one of the helium_status_ error codes. If the result is < 0
     *     it is one of the config_poll_get_status error codes
     */
    int get(const char * key,
            int32_t *    value,
            int32_t      default_value,
            uint32_t     retries = HELIUM_POLL_RETRIES_5S)
    {
        return _get(key,
                    helium_config_i32,
                    value,
                    sizeof(*value),
                    &default_value,
                    sizeof(default_value),
                    retries);
    }

    /** Get a float configuration value
     *
     * @param key The configuration key to get
     * @param[out] value The target for the received value
     * @param default_value The default value in case of errors
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. If the result is > 0 the result code is
     *     one of the helium_status_ error codes. If the result is < 0
     *     it is one of the config_poll_get_status error codes
     */
    int get(const char * key,
            float *      value,
            float        default_value,
            uint32_t     retries = HELIUM_POLL_RETRIES_5S)
    {
        return _get(key,
                    helium_config_f32,
                    value,
                    sizeof(*value),
                    &default_value,
                    sizeof(default_value),
                    retries);
    }


    /** Get a boolean configuration value
     *
     * @param key The configuration key to get
     * @param[out] value The target for the received value
     * @param default_value The default value in case of errors
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. If the result is > 0 the result code is
     *     one of the helium_status_ error codes. If the result is < 0
     *     it is one of the config_poll_get_status error codes
     */
    int get(const char * key,
            bool *       value,
            bool         default_value,
            uint32_t     retries = HELIUM_POLL_RETRIES_5S)
    {
        return _get(key,
                    helium_config_bool,
                    value,
                    sizeof(*value),
                    &default_value,
                    sizeof(default_value),
                    retries);
    }

    /** Get a string configuration value
     *
     * @param key The configuration key to get
     * @param[out] value The target buffer for the received string
     * @param value_len The length of the available buffer space
     * @param default_value The default value to use if not found
     * @param default_value_len The length of the default_value buffer.
     *     Note: Ensure to include the trailing NULL in this length
     *     parameter
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. If the result is > 0 the result code is
     *     one of the helium_status_ error codes. If the result is < 0
     *     it is one of the config_poll_get_status error codes
     */
    int get(const char * key,
            char *       value,
            size_t       value_len,
            char *       default_value,
            size_t       default_value_len,
            uint32_t     retries = HELIUM_POLL_RETRIES_5S)
    {
        return _get(key,
                    helium_config_str,
                    value,
                    value_len,
                    default_value,
                    default_value_len,
                    retries);
    }

    /** Send a request for a configuration value.
     *
     * Getting a configuration value requires sending a request with
     * the configuration key and then using the resulting token in a
     * #poll_get_result() call to wait for a response.
     *
     * @param key The configuration key to get
     * @param[out] token The token representing the response.
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int get(const char * key, uint16_t * token);

    /** Poll the response of a configuration request.
     *
     * Polls the given token and validates any response against the
     * given configuration key and expected type. If these match the
     * value is copied into the given value buffer.
     *
     * Note: The short form methods for getting config values hide
     * most of the complexity required to make a configuration get
     * work.
     *
     * @param token The token returned from a previous #get() request
     * @param config_key The configuration key to check for
     * @param config_type The configuration type to check for
     * @param[out] value The destination buffer to copy the result into
     * @param value_len The size of the given destination buffer
     * @param default_value The default value to use if not found
     * @param default_value_len The length of the default_value buffer.
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. If the result is > 0 the result code is
     *     one of the helium_status_ error codes. If the result is < 0
     *     it is one of the config_poll_get_status error codes
     */
    int poll_get_result(uint16_t                token,
                        const char *            config_key,
                        enum helium_config_type config_type,
                        void *                  value,
                        size_t                  value_len,
                        void *                  default_value,
                        size_t                  default_value_len,
                        uint32_t retries = HELIUM_POLL_RETRIES_5S);

    /** Set a float configuration value
     *
     * @param key The configuration key to set
     * @param value The value to set
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int set(const char * key, float value, uint32_t retries = HELIUM_POLL_RETRIES_5S)
    {
        return _set(key, helium_config_f32, &value, retries);
    }

    /** Set an integer configuration value
     *
     * @param key The configuration key to set
     * @param value The value to set
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int set(const char * key,
            int32_t      value,
            uint32_t     retries = HELIUM_POLL_RETRIES_5S)
    {
        return _set(key, helium_config_i32, &value, retries);
    }

    /** Set a boolean configuration value
     *
     * @param key The configuration key to set
     * @param value The value to set
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int set(const char * key, bool value, uint32_t retries = HELIUM_POLL_RETRIES_5S)
    {
        return _set(key, helium_config_bool, &value, retries);
    }

    /** Set a string configuration value
     *
     * @param key The configuration key to set
     * @param value The value to set
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int set(const char * key,
            const char * value,
            uint32_t     retries = HELIUM_POLL_RETRIES_5S)
    {
        return _set(key, helium_config_str, &value, retries);
    }

    /** Set a null configuration value
     *
     * @param key The configuration key to set
     * @param retries The number of times to retry (optional)
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int set_null(const char * key, uint32_t retries = HELIUM_POLL_RETRIES_5S)
    {
        return _set(key, helium_config_null, NULL, retries);
    }

    /** Send a request for setting a configuration value.
     *
     * Setting a configuration value requires sending a request with
     * the configuration key, the value type and the value and then
     * using the resulting token in a #poll_set_result() call to wait
     * for a response.
     *
     * @param key The configuration key to set
     * @param value_type The type of the configuration value to set
     * @param value A pointer to the value that needs to be st
     * @param[out] token The token representing the response.
     * @returns 0 on success. One of the helium_status_ error
     *     codes otherwise.
     */
    int set(const char *       key,
            helium_config_type value_type,
            void *             value,
            uint16_t *         token);

    /** Poll the response of a configuration set request.
     *
     * Polls the given token and returns the result code of the set
     * request.
     *
     * Note: The short form methods for getting config values hide
     * most of the complexity required to make a configuration get
     * work.
     *
     * @param token The token returned from a previous #set() request
     * @param[out] result A pointer to storage for the response code
     * @param retries The number of times to retry (optional)
     * @returns A helium_status result code for the actual
     *     communication part. If the result is helium_status_OK, the
     *     result code can be used to check if the set was
     *     successful. The result code will be 0 on success, and
     *     non-zero otherwise.
     */
    int poll_set_result(uint16_t token,
                        int8_t * result,
                        uint32_t retries = HELIUM_POLL_RETRIES_5S);

    /** Check whether configuration values are stale.
     *
     * Checks whether there has been a system indication that
     * configuration attributes may have gone stale.
     *
     * When this returns true you should assume that any configuration
     * values you have previously retrieved are no longer valid.
     *
     * @returns true if previous configuration values are stale, false
     *     if not
     */
    bool is_stale();


  private:
    int       _get(const char *            config_key,
                   enum helium_config_type config_type,
                   void *                  value,
                   size_t                  value_len,
                   void *                  default_value,
                   size_t                  default_value_len,
                   uint32_t                retries);
    int       _set(const char *            config_key,
                   enum helium_config_type value_type,
                   void *                  value,
                   uint32_t                retries);
    Channel * _channel;
};

#endif // HELIUM_H

/** @example Basic.ino
 *
 * shows a basic example of how to construct Helium, an MQTT channel
 * and transmitting data to that channel
 */
