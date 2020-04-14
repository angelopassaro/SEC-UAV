# UAV

In this work I added different  algorithms of  lightweight cryptography for test their performance. The original idea of add encryption in MAVLink was propose in "MAVSec: Securing the MAVLink Protocol for Ardupilot/PX4 Unmanned Aerial Systems"

# Original
[MAVSec](https://github.com/aniskoubaa/mavsec)

# Technologies
[MAVLink](https://github.com/mavlink/mavlink)

[Ardupilot](https://github.com/ArduPilot/ardupilot)

[QGroundControl](https://github.com/mavlink/qgroundcontrol)


# Custom
[CUSTOM MAVLINK](https://github.com/angelopassaro/c_library_v2) MAVLink with lightweight crypto implementation
- Added crypto/chacha20.h (implementation from mavsec)
- Added crypto/trivium.h  (implementation from FELICS)
- Added crypto/rabbit.h   (implementation from paper Rabbit: A New High-Performance Stream Cipher + iv from ECRYPT)
- Added crypto/simon6496.h (implementation from (https://github.com/angelopassaro/simon-speck)
- Added crypto/simon64128.h (implementation from (https://github.com/angelopassaro/simon-speck)
- Added crypto/speck6496.h (implementation from (https://github.com/angelopassaro/simon-speck)
- Added crypto/speck64128.h (implementation from (https://github.com/angelopassaro/simon-speck)
- Edit mavlink_helpers.h for support encryption between UAV ang GCS

[ARDUPILOT-SITL](https://github.com/angelopassaro/ardupilot) Custom Ardupilot
- Added two parameters in sim_vehicle.py for gcs-ip and uav-ip
- Added bridge in Vagrantfile


### TODO

- [x] Add encryption

Stream ciphers
- [x] Add ChaCha20
- [x] Add Trivium
- [x] Add Rabbit

Block ciphers CTR-mode
- [x] Simon
- [x] Speck

- [ ] Add key exchange

- [ ] Create new message in MAVLink
- [ ] Integrate new message in SITL
- [ ] Integrate new message in QgrounControl

- [ ] Clone https://github.com/ArduPilot/pymavlink/ and update https://github.com/ArduPilot/pymavlink/tree/master/generator/C/include_v2.0 with custom files
- [ ] Benchmark

- [x] Remove useless modules in repo (Ardupilot, QgroundControl)
- [ ] Script for setup
# Usage
- Clone [Ardupilot](https://github.com/ArduPilot/ardupilot) or [Ardupilot Custom](https://github.com/angelopassaro/ardupilot)(for bridge network in vagrant)  and [QGroundControl](https://github.com/mavlink/qgroundcontrol)
- Copy contents of [Custom Mavlink](https://github.com/angelopassaro/c_library_v2) for ardupilot in ardupilot/build/sitl/libraries/GCS_MAVLink/include/mavlink/v2.0/ and for qgroundcontrol in qgroundcontrol/libs/mavlink/include/mavlink/v2.0/ 
