# Classic Format with "0x"

0x55 0x50 0x0F 0x01 0x01 0x00 0x01 0x25 0x84 0x03 0x63 0x55 0x51 0x78 0x00 0x23 0x00 0x5F 0xF8 0x57 0x0E 0xFD 0x55 0x52 0xF4 0xFF 0x6A 0x00 0xA5 0xFF 0x57 0x0E 0x0D 0x55 0x53 0x1A 0x7F 0x65 0xFD 0x31 0xC7 0x57 0x0E 0x00 0x55 0x54 0xA3 0xFE 0xBD 0xFF 0xB3 0x01 0x57 0x0E 0x1F

# Minified Format

55 50 0F 01 01 00 02 39 C8 00 B9 55 51 F0 FF 57 00 5F F8 FC 0D 4C 55 52 03 00 EE FF D9 FF FC 0D 78 55 53 1F 7E 00 00 08 C6 FC 0D 1C 55 54 BD FE BF FF CD 01 FC 0D F9

# 0x55 0x50 - Time Output
9 bytes of Data

```
0x55 0x50 0x0F 0x01 0x01 0x00 0x01 0x25 0x84 0x03 0x63
0x55 0x50   YY   MM   DD   hh   mm   ss  msL  msH  SUM
```


# 0x55 0x51 - Acceleration Output
9 bytes of data

```
0x55 0x51 0x78 0x00 0x23 0x00 0x5F 0xF8 0x57 0x0E 0xFD
0x55 0x51  AxL  AxH  AyL  AyH  AzL  AzH   TL   TH  SUM
```

# 0x55 0x52 - Angular Velocity Output
9 bytes of data

```
0x55 0x52 0xF4 0xFF 0x6A 0x00 0xA5 0xFF 0x57 0x0E 0x0D
0x55 0x52  wxL  wxH  wyL  wyH  wzL  wzH   TL   TH  SUM
```

# 0x55 0x53 - Angle Output
9 bytes of data

```
0x55 0x53 0x1A  0x7F  0x65   0xFD   0x31 0xC7 0x57 0x0E 0x00
0x55 0x53 RollL RollH PitchL PitchH YawL YawH   TL   TH SUM
```

Roll(x axis)Roll=((RollH<<8)|RollL)/32768*180(°)

rollH * 256 + rollL / 32768*180(°)

Pitch(y axis)Pitch=((PitchH<<8)|PitchL)/32768*180(°)

Yaw(z axis)Yaw=((YawH<<8)|YawL)/32768*180(°)


# 0x55 0x54 - Magnetic Output
9 bytes of data

```
0x55 0x54 0xA3 0xFE 0xBD 0xFF 0xB3 0x01 0x57 0x0E 0x1F
0x55 0x54  HxL  HxH  HyL  HyH  HzL  HzH   TL   TH  SUM
```