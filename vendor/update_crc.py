#!/usr/bin/env python3
import crcmod
import pathlib
import struct
import sys

def parse_config(dump):
    header_length = int(dump[0])
    header = dump[1:1+header_length]
    header_dict = dict(zip(
        'fw_ver_maj fw_ver_min empty fw_rev'.split(),
        struct.unpack_from('>2B2I', header, 0)))
    header_dict['extra'] = header[10:]
    config_length = struct.unpack_from('<H', dump, 1+header_length)[0]
    if len(dump) != 1 + header_length + 2 + config_length:
        raise 'Configuration size does not match headers'
    crc = crcmod.predefined.PredefinedCrc('crc-ccitt-false').new()
    crc.update(dump[1+header_length:-2])
    crc_offset = 1 + header_length + config_length
    return header_dict, crc_offset, crc.digest()

def main():
    if len(sys.argv) != 2:
        print('Usage: update_crc.py <config.bin>')
        sys.exit(1)
    config_path = pathlib.Path(sys.argv[1]).expanduser()
    config = config_path.read_bytes()
    header_dict, crc_offset, crc_dgst = parse_config(config)
    print(header_dict)
    with config_path.open('br+') as f:
        f.seek(crc_offset)
        f.write(crc_dgst[::-1])
    print('Updated CRC')

if __name__ == '__main__':
    main()
