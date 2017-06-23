/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#ifndef HELIUM_ERROR_H
#define HELIUM_ERROR_H

enum helium_status
{
    helium_status_OK,
    helium_status_OK_NO_DATA,
    helium_status_ERR_COMMUNICATION,
    helium_status_ERR_NOT_CONNECTED,
    helium_status_ERR_DROPPED,
    helium_status_ERR_KEEP_AWAKE,
};

enum helium_info_status
{
    helium_info_OK                = helium_status_OK,
    helium_info_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};

enum helium_connected_status
{
    helium_connected_CONNECTED         = helium_status_OK,
    helium_connected_NOT_CONNECTED     = helium_status_ERR_NOT_CONNECTED,
    helium_connected_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};

enum helium_connect_status
{
    helium_connect_CONNECTED         = helium_status_OK,
    helium_connect_NOT_CONNECTED     = helium_status_ERR_NOT_CONNECTED,
    helium_connect_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};

enum helium_sleep_status
{
    helium_sleep_OK                = helium_status_OK,
    helium_sleep_ERR_NOT_CONNECTED = helium_status_ERR_NOT_CONNECTED,
    helium_sleep_ERR_KEEP_AWAKE    = helium_status_ERR_KEEP_AWAKE,
    helium_sleep_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};

enum helium_poll_status
{
    helium_poll_OK_DATA           = helium_status_OK,
    helium_poll_OK_NO_DATA        = helium_status_OK_NO_DATA,
    helium_poll_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};

enum helium_channel_create_status
{
    helium_channel_create_OK                = helium_status_OK,
    helium_channel_create_ERR_NOT_CONNECTED = helium_status_ERR_NOT_CONNECTED,
    helium_channel_create_ERR_DROPPED       = helium_status_ERR_DROPPED,
    helium_channel_create_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};

enum helium_channel_send_status
{
    helium_channel_send_OK                = helium_status_OK,
    helium_channel_send_ERR_NOT_CONNECTED = helium_status_ERR_NOT_CONNECTED,
    helium_channel_send_ERR_DROPPED       = helium_status_ERR_DROPPED,
    helium_channel_send_ERR_COMMUNICATION = helium_status_ERR_COMMUNICATION,
};



#endif
