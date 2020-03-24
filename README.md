# UAV
Custom MAVLink with lightweight crypto

# Original
[MAVSec](https://github.com/aniskoubaa/mavsec)

# Technologies
[MAVLink](https://github.com/mavlink/mavlink)

[Ardupilot](https://github.com/ArduPilot/ardupilot)

[QGroundContro](https://github.com/mavlink/qgroundcontrol)

# Crypto ref
[FELICS](https://www.cryptolux.org/index.php/FELICS)


# Custom
[CUSTOM MAVLINK](https://github.com/angelopassaro/c_library_v2) MAVLink with lightweight crypto implementation
- Added crypto/chacha20.h (implementation from mavsec)
- Added crypto/trivium.h  (implementation from FELICS)
- Edit mavlink_helpers.h for support encryption between UAV ang GCS

[ARDUPILOT](https://github.com/angelopassaro/ardupilot) Custom Ardupilot
- Added two parameters in sim_vehicle.py for gcs-ip and uav-ip
- Added bridge in Vagrantfile


### TODO

- [ ] Add encryption

Stream ciphers
- [x] Add ChaCha20
- [x] Add Trivium
- [ ] Add Snow3G

Block ciphers
- [ ] Simon
- [ ] Speck
- [ ] Present

- [ ] Add key exchange

- [ ] Clone https://github.com/ArduPilot/pymavlink/ and update https://github.com/ArduPilot/pymavlink/tree/master/generator/C/include_v2.0 with custom files
- [ ] Benchmark
