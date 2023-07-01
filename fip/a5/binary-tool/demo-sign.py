#!/usr/bin/python3 -B

import argparse
import binascii
import os
import pathlib
import struct
import sys

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.asymmetric import utils

#
# Variables
#

#
# Functions
#

def stderr_print(*arg, **kwargs):
    print(*arg, file=sys.stderr, *kwargs);
    return

#
# Classes
#

class DemoSign(object):

    #
    # Static variables
    #

    __FMT_HEADER__ = '<QIHH'
    __MAGIC64__ = int.from_bytes(bytes.fromhex('58a848a4843ff07d'), byteorder='little')

    #
    # Initialization
    #

    def __init__(self, autorun=False):
        self._args = None

        if autorun:
            self._parseArgs()
            self._run()
        return

    #
    # Methods (non-public)
    #

    def _parseArgs(self):
        parser = argparse.ArgumentParser(description="Demo Sign")
        parser.add_argument('INFILE_PRIVKEY', help='Input file of private RSA key')

        self._args = parser.parse_args()
        # print(self._args)
        # print(vars(self._args))
        # print(vars(self._args)['INFILE_PRIVKEY'])
        # print(self._args.INFILE_PRIVKEY)
    
    def _run(self):
        self._recv_header()
        self._recv_payload()

        self._process()

        self._send_header()
        self._send_payload()
        return

    def _recv_header(self):
        sz_hdr = struct.calcsize(type(self).__FMT_HEADER__)
        buf = sys.stdin.buffer.read(sz_hdr)
        assert len(buf) == sz_hdr

        (
            magic,
            self._sz_payload,
            self._seq_no,
            self._flags
        ) = struct.unpack(type(self).__FMT_HEADER__, buf)
        # stderr_print('magic: 0x{:x}'.format(magic))
        assert magic == type(self).__MAGIC64__
        assert (magic % 2) == 0
        assert (self._seq_no % 2) == 0

        return

    def _recv_payload(self):
        self._buf_digest = sys.stdin.buffer.read(self._sz_payload)
        assert len(self._buf_digest) == self._sz_payload
        return

    def _process(self):
        # Load private key
        with open(self._args.INFILE_PRIVKEY, "rb") as key_file:
            stderr_print('RSA key file: {}'.format(self._args.INFILE_PRIVKEY))
            private_key = serialization.load_pem_private_key(
                key_file.read(),
                password=None,
            )
            del key_file

        # Signing
        assert len(self._buf_digest) == 32
        # stderr_print(self._buf_digest.hex())
        sig = private_key.sign(
            self._buf_digest,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=32
            ),
            utils.Prehashed(hashes.SHA256())
        )
        self._buf_sig = sig
        return

    def _send_header(self):
        sz_hdr = struct.calcsize(type(self).__FMT_HEADER__)

        self._seq_no += 1
        sz_payload = len(self._buf_sig)
        buf = struct.pack(type(self).__FMT_HEADER__,
            type(self).__MAGIC64__ + 1,
            sz_payload,
            self._seq_no,
            0
        )
        assert len(buf) == sz_hdr

        sz_out = sys.stdout.buffer.write(buf)
        assert sz_out == sz_hdr

        return

    def _send_payload(self):
        # stderr_print(self._buf_sig.hex())
        sz_out = sys.stdout.buffer.write(self._buf_sig)
        with open('test.sig', 'wb') as file2:
            file2.write(self._buf_sig)
            file2.close()
            
        assert sz_out == len(self._buf_sig)
        return

    #
    # Methods (public)
    #

#
# Main
#

if __name__ == '__main__':
    DemoSign(True)

# vim: set expandtab ai tabstop=4 shiftwidth=4 softtabstop=-1:
