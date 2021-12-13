#!/usr/bin/python3 -B

import argparse
import sys
import binascii

from lib.python3.NOCS import FlashImg

#
# Main
#

flash_img = FlashImg.new('input/refImgSigned.bin')

flash_img.extract_bb1st("intermediate/bb1st.bin")
flash_img.extract_blob_bl2e("intermediate/blob-bl2e.bin")

# vim: set expandtab ai tabstop=4 shiftwidth=4 softtabstop=-1:
