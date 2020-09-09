# UAV

In this work I added different  algorithms of  lightweight cryptography for test their performance. The original idea of add encryption in MAVLink was propose in "MAVSec: Securing the MAVLink Protocol for Ardupilot/PX4 Unmanned Aerial Systems"

# Original
[MAVSec](https://github.com/aniskoubaa/mavsec)

[key exchange good start point](https://github.com/PX4/Firmware/issues/13538)

# Technologies
[MAVLink](https://github.com/mavlink/mavlink)

[Ardupilot](https://github.com/ArduPilot/ardupilot)

[QGroundControl](https://github.com/mavlink/qgroundcontrol)


# Custom
[CUSTOM MAVLINK](https://github.com/angelopassaro/c_library_v2) MAVLink with lightweight crypto implementation
- Edit mavlink_helpers.h for support encryption between UAV ang GCS
- Added crypto/chacha20.h (implementation from mavsec)
- Added crypto/trivium.h  (implementation from FELICS)
- Added crypto/rabbit.h   (implementation from paper Rabbit: A New High-Performance Stream Cipher + iv from ECRYPT)
- Added crypto/simon6496.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/simon64128.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/simon128128.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/simon128192.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/simon128256.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/speck6496.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/speck64128.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/speck128128.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/speck128192.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added crypto/speck128256.h [implementation](https://github.com/angelopassaro/simon-speck)
- Added two parameters in sim_vehicle.py for gcs-ip and uav-ip
- Added bridge in Vagrantfile
- Added FourQlib [original implementation](https://github.com/microsoft/FourQlib)
- Tiger hash [Implementation](https://github.com/rhash/RHash)

#### Encryption
- [x] Add encryption

Stream ciphers
- [x] Add ChaCha20
- [x] Add Trivium
- [x] Add Rabbit

Block ciphers CTR-mode
- [x] Simon
- [x] Speck
-------------------------------------------------------------
#### Key exchange
- [x] Create new message in MAVLink (Certificate for pubkey)
- [x] FourQlib integration in mavlink
- [X] Handle in MAVLink the new message
-------------------------------------------------------------
- [X] Integrate new message in SITL(ardupilot)
- [X] Integrate new message in QgrounControl
-------------------------------------------------------------
#### Benchmark
- [x] Benchmark
- [x] Certificate Benchmark
------------------------------------------------------------
#### Other
- [x] Merge in unique file crypto 
- [x] Clone https://github.com/ArduPilot/pymavlink/ and update https://github.com/ArduPilot/pymavlink/tree/master/generator/C/include_v2.0 with custom files
- [x] Remove useless modules in repo (Ardupilot, QgroundControl)

# Disclaimer
This implementation is for testing purposes **ONLY** and safety is not guaranteed.

# Usage
- Clone [Ardupilot Custom](https://github.com/angelopassaro/ardupilotcustom)(key exchange and encryption support) and [QGroundControlCustom](https://github.com/angelopassaro/qgroundcontrolcustom)(key exchange and encryption support)
- Copy contents of [Custom Mavlink](https://github.com/angelopassaro/c_library_v2) for:
    - ardupilot in ardupilot/build/sitl/libraries/GCS_MAVLink/include/mavlink/v2.0/
    - qgroundcontrol in qgroundcontrol/libs/mavlink/include/mavlink/v2.0/
- Build QGroundControlCustom [Build instructions](https://dev.qgroundcontrol.com/master/en/getting_started/index.html#native-builds)
- Generate certificates for GCS and UAV [certificate generator](https://github.com/angelopassaro/SEC-UAV/blob/master/utils/cert_generator.c)
- Copy the generated certificates to the root directory of Ardupilot and QgroundControl. If you prefer a different path change the paths in:
     - [Ardupilot](https://github.com/angelopassaro/ArdupilotCustom/blob/42451935ac905105d64df6a852c15cf332e682a9/libraries/GCS_MAVLink/GCS_Common.cpp#L863)
     - [QgroundControl](https://github.com/angelopassaro/qgroundcontrolcustom/blob/7c7dc01f5d184c354a70e8543abca1c5da082f08/src/comm/MAVLinkProtocol.cc#L343)
     - Rebuild QgroundControl


