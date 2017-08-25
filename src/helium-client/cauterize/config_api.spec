(name "config_api")
(version "1.0.0")
(fingerprint 820c22dbdfaf528609a7d4b43323b4807ad8fb8f)
(size 1 553)
(depth 7)
(typelength 2)
(lengthtag t2)
(type 
  string
  vector
  (fingerprint 9c11e8a6397ee5d852dddc709106a031280e5410)
  (size 1 33)
  (depth 2)
  u8
  32
  t1)
(type 
  value
  union
  (fingerprint 8054f4d47bfb8ac2c9cb851855e8c7cbf43cd082)
  (size 1 34)
  (depth 3)
  t1
  (fields 
    (field b 0 bool)
    (field i 1 s32)
    (field f 2 f32)
    (field s 3 string)
    (empty n 4)))
(type 
  key
  vector
  (fingerprint fc1acd1a1e69e65a45dbeced686c656027f12020)
  (size 1 21)
  (depth 2)
  u8
  20
  t1)
(type 
  assoc
  record
  (fingerprint 5497422e7abc9db96ea1091ddd87623e5c2af7e3)
  (size 2 55)
  (depth 4)
  (fields (field k 0 key) (field v 1 value)))
(type 
  arr_key
  vector
  (fingerprint 2a685a97b8c36e82a38fbfe2c4614a1bacfd579e)
  (size 1 211)
  (depth 3)
  key
  10
  t1)
(type 
  arr_assoc
  vector
  (fingerprint cf0f50b0b29d3fcccbcf64a76d03fcff267c5d16)
  (size 1 551)
  (depth 5)
  assoc
  10
  t1)
(type 
  cmd_config_get
  union
  (fingerprint 1db523aaee3bcac2030a13ba87b37bd22f5bdafd)
  (size 2 552)
  (depth 6)
  t1
  (fields (field req 0 arr_key) (field res 1 arr_assoc)))
(type 
  cmd_config_set
  union
  (fingerprint 54c50d707bc57dbb703b1a676f4db1c0f65c00e4)
  (size 2 552)
  (depth 6)
  t1
  (fields (field req 0 arr_assoc) (field res 1 u8)))
(type 
  cmd_config
  union
  (fingerprint d8b08141cbf2823d829045a36119d7536def6ab7)
  (size 1 553)
  (depth 7)
  t1
  (fields (field get 0 cmd_config_get) (field set 1 cmd_config_set) (empty invalidate 2)))
