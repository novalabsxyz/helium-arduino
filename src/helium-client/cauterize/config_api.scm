;; -*- cauterize -*-
(name "config_api")
(version "2.0.0")

(type key vector u8 20)
(type string vector u8 32)
(type value union
      (fields
       (field b bool)
       (field i s32)
       (field f f32)
       (field s string)
       (empty n) ))

(type assoc record
      (fields
       (field k key)
       (field v value) ))

(type arr_assoc vector assoc 10)
(type arr_key vector key 10)

(type cmd_config union
      (fields
       (field get cmd_config_get)
       (field set cmd_config_set)
       (empty invalidate ) ))

(type cmd_config_get_res union
      (fields
        (field err u8)
        (field res arr_assoc) ))

(type cmd_config_get union
      (fields
       (field req arr_key)
       (field res cmd_config_get_res) ))

(type cmd_config_set_res union
      (fields
        (field err u8)
        (empty res)
      ))

(type cmd_config_set union
      (fields
       (field req arr_assoc)
       (field res cmd_config_set_res)))

