;; -*- cauterize -*-
(name "atom_api")
(version "1.0.0")


;;
;; Generic types
;;
(type frame_app                 vector u8 103)
(type arr_u8_32                 array u8 32)

(type txn record
    (fields
        (field seq              u16)
        (field needs_reset      bool)
        (field cmd              cmd)))

(type cmd union
    (fields
        (field info             cmd_info)
        (field send             cmd_send)
        (field connect          cmd_connect)
        (field connected        cmd_connected)
        (field poll             cmd_poll)
        (field sleep            cmd_sleep)
        (field baud             cmd_baud)
        (field logging          cmd_logging)
        (field reset            cmd_reset)))

(type cmd_mac union
    (fields
        (empty req)
        (field res u64)))

(type cmd_info union
    (fields
        (empty req)
        (field res              res_info)))

(type res_info record
    (fields
        (field mac              u64)
        (field uptime           u32)
        (field time             u32)
        (field fw_version       u32)
        (field radio_count      u8)))

;; A command to ask Atom for a received data frame waiting in its buffer
(type cmd_poll union
    (fields
        ;; Used for selection, set let to 0 to poll for all frames
        (field req              frame_app)
        (field res              res_poll)))

(type res_poll union
    (fields
        (empty none)
        (field frame            frame_app)
        (field log              frame_app)))

;; Send data from application to platform
(type cmd_send union
    (fields
        (field req              frame_app)
        (field res              res_send)))

;; Represents a response to a cmd_send.req
(type res_send enumeration
    (values
        ok
        err_not_connected
        err_dropped
        err_nack
        err_channel_access))

(type cmd_connect union
    (fields
        (field req              req_connect)
        (field res              res_connect)))

(type req_connect union
    (fields
        ;; Attempt to connect without the aid of quick connect data
        (empty cold)
        ;; Attempt to quick connect using a previous session's connection data
        (field quick            connection)))

;; Connect request always returns immediately. The response does not
;; indicate if connection was successful, since it can take some time
;; and varies with the wind.
(type res_connect enumeration
     (values
        ;; No hiccups in processing the reponse
        ok
        ;; Quick-connect parameters are invalid, will connect normally
        ;; without the benefit of quickconnect data
        err_fallback_to_slow_connect))

;; A type that represents all the data required to maintain a
;; connection to Helium cloud. `connection` is returned from sleep
;; requests is an optional parameter to connect requests
(type connection record
    (fields
        ;; Long (MAC) address of this node
        (field long_addr        u64)
        ;; Firmware version
        ;; If Atom gets an OTA, it must re-associate after restarting.
        ;; By including version here, Atom can make sure that it matches,
        ;; and force a re-association
        (field fw_version       u32)
        ;; Unix time
        (field time             u32)
        ;; The PAN ID for the network this node is associated with
        (field pan_id           u16)
        ;; The short address for this node
        (field short_addr       u16)
        ;; The short address of the gateway node
        (field gateway_addr     u16)
        ;; Connected
        (field connected        u8)
        ;; PAN sequence number last used, must be non-zero to indicate connection
        (field pan_seq          u8)
        ;; Current radio index
        (field radio_index      u8)
        ;; The channel number this node is operating on
        (field channel          u8)
        ;; Radio speed selected for operation
        (field speed            u8)
        ;; Current keyslot
        (field key_slot         u8)
        ;; Session key for crypto
        (field sess_key         arr_u8_32)))

;;
(type cmd_connected union
    (fields
        (empty req)
        (field res              bool)))

;; A command to put atom in a state where it's safe to sleep
(type cmd_sleep union
    (fields
        (empty req)
        (field res              res_sleep)))

;; A response to sleep request
(type res_sleep union
    (fields
        (empty not_connected)
        (empty keep_awake)
        (field connection       connection)))

;; A command to set the bauderate at runtime
(type cmd_baud union
    (fields
        (field req              atom_baud)
        (empty res)))

(type atom_baud enumeration
    (values
        b9600
        b14400
        b19200
        b38400
        b57600
        b115200
        b128000
        b230400))

;; If enabled, polling will return log messages
;; NOTE: Frame data takes priority when polling, and log messages can be
;; dropped when polling infrequently or during times of heavy traffic
(type cmd_logging union
    (fields
        (field req              req_logging)
        (empty res)))

;; Turns logging on/off
(type req_logging enumeration
    (values
        enable
        disable))

;; A command to reset atom
(type cmd_reset enumeration
    (values
        req
        res))
