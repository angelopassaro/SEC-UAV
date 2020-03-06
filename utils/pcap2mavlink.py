#!/bin/python

from scapy.all import *
from pymavlink.dialects.v20 import ardupilotmega as mavlink2
from builtins import object
import time
import argparse


class fifo(object):
    def __init__(self):
        self.buf = []

    def write(self, data):
        self.buf += data
        return len(data)

    def read(self):
        return self.buf.pop(0)


def convert(gcs, uav, file):
    f = fifo()
    packets = rdpcap(file)
    mav = mavlink2.MAVLink(f)
    output = file.replace("pcap", "csv")

    print(f"Write {output}.\nStarting...")
    with open(output, "w") as outfile:
        outfile.write("Timestamp;Src;Dst;MavLinkMsg;MavLinkParam\n")
        timestamp, dst, src, data = ("", "", "", "")
        for packet in packets:
            if packet.getlayer("ICMP") is None and packet.getlayer("IP") is not None:
                timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(int(packet.time)))
                if packet.getlayer("IP").src == uav and packet.getlayer("IP").dst == gcs:
                    data = packet.getlayer("Raw").load
                    src, dst = ("UAV", "GCS")
                    # src, dst = (uav, gcs)              write ip
                elif packet.getlayer("IP").src == gcs and packet.getlayer("IP").dst == uav:
                    data = packet.getlayer("Raw").load
                    src, dst = ("GCS", "UAV")
                    # src, dst = (gcs, uav)              write ip
                else:
                    data = packet.getlayer("Raw").load
                    print("qui")
                    print("Garbage: ", mav.decode(bytearray(data)))
            type_msg, param = str(mav.decode(bytearray(data))).split('{')
            outfile.write(f"{timestamp};{src};{dst};{type_msg};{param.split('}')[0]}\n")

    print("Conversion completed")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-g", "--gcs-ip",
        type=str,
        dest="gcs",
        default="127.0.0.1",
        help="ip of GCS"
    )

    parser.add_argument(
        "-u", "--gcs-uav",
        type=str,
        dest="uav",
        default="127.0.0.1",
        help="ip of UAV"
    )

    parser.add_argument(
        "-i", "--input-file",
        type=str,
        dest="file",
        help="Path of pcap file"
    )

    args = parser.parse_args()
    if args.gcs is None or args.uav is None or args.file is None:
        parser.print_help()
    else:
        convert(args.gcs, args.uav, args.file)


if __name__ == '__main__':
    main()
