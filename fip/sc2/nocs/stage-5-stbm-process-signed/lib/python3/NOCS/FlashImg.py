
from Crypto import Random
from Crypto.Cipher import AES
from Crypto.Hash import SHA256

""" Object FlashImg """
class FlashImg(object):

    #
    # Constant
    #

    _BLOB_BL2E_SIZE_A = 74864
    _TOTAL_AREA_SIZE_A = 334960

    #
    # Initialization
    #

    def __init__(self, fname):
        self._fpk = None

        buf = None
        with open(fname, 'rb') as fin:
            buf = fin.read()
        assert buf is not None
        self._buf= bytearray(buf)

        self._process_flashimg()

    #
    # Functions
    #

    def _update_buf(self, offset, data):
        length = len(data)
        assert type(data) is bytes
        assert (offset + length) < len(self._buf)

        self._buf[offset:offset + length] = data

    def _update_checksum(data):
        length = len(data)
        dgst = SHA256.new(data[:length - 32])
        data = data[:len(data) - 32]
        data += dgst.digest()
        assert len(data) == length
        return data

    def _process_flashimg(self):
        # Fill layout skeleton of Device Level-1 Certificate (if it's cleared by Nagra)
        offset = 0x1900
        length = 0x2000 - 0x1900
        if self._buf[offset:offset + 16] == b'\0' * 16 \
           and self._buf[offset + 32:offset + length] == b'\0' * (length - 32):
            # Header
            data = bytes.fromhex('404d4c20 e2df0e93 00000000 00070000'.replace(' ',''))
            self._update_buf(0x1900, data)
            # EPKs
            data = bytes.fromhex('''
                9d2a6f0c 5dcf9652 233d26b0 05f69c1c
                f2e20fc7 a09d9036 7b77bf98 e295a3d5
                4a703740 812ed450 ae79db19 0d3422b4
                6ffc975b 8494d1ed 3b90eb6a 0835cc54
            '''.replace(' ','').replace('\n',''))
            self._update_buf(0x1d50, data)
            # Size of ExtStartParams
            data = bytes.fromhex('f0df0100')
            self._update_buf(0x1d90, data)
            # DblkEndMark, DUID and CtrlData
            data = bytes.fromhex('''
                44424c4b 5f504159 4c4f4144 5f454e44
                4380caee 2efa289c 4f2d5815 8a7cff39
                00000000 ffffffff 00000000 ffffffff
            '''.replace(' ','').replace('\n',''))
            self._update_buf(0x1dd0, data)

        # Fill nonce for Device Level-1/2 PubRSA Key
        obj_rand = Random.new()

        self._update_buf(0x1910, obj_rand.read(16))
        self._update_buf(0x2090, obj_rand.read(16))

        # Figure out size of BL2E
        self._sz_total_area = int.from_bytes(self._buf[0x2040:][:4], byteorder='little')
        #print(sz_total_area)
        self._sz_blob_bl2e  = type(self)._BLOB_BL2E_SIZE_A
        self._sz_blob_bl2e += (self._sz_total_area - type(self)._TOTAL_AREA_SIZE_A)
        #print(sz_blob_bl2e)
        self._sz_bl2e_payload = self._sz_blob_bl2e - type(self)._BLOB_BL2E_SIZE_A + 65536
        #print(sz_bl2e_payload)

        if self._sz_bl2e_payload == 64 * 1024:
            pass
        elif self._sz_bl2e_payload == 128 * 1024:
            pass
        elif self._sz_bl2e_payload == 256 * 1024:
            pass
        elif self._sz_bl2e_payload == 1024 * 1024:
            pass
        else:
            assert False

    def _encrypt_bl2e(self):
            if self._buf[0x201F] & 0x80 == 0:
                return

            offset = 254 * 1024 + 8192
            length = 16 + self._sz_bl2e_payload + 16

            buf = bytes(self._buf[offset:offset + length])
            cipher = AES.new(self._fpk, AES.MODE_CBC, b'\0'*16)
            buf = cipher.encrypt(buf)
            self._buf[offset:offset + length] = buf

    #
    # Methods
    #

    def loadFPKE(self, fname):
        with open(fname, 'rb') as fin:
            fpke = fin.read()
            assert len(fpke) == 16
            self._update_buf(0x2020, fpke)
            return
        assert False

    def setFPK(self, fname):
        assert False
        return
        with open(fname, 'rb') as fin:
            self._fpk = fin.read()
            assert len(self._fpk) == 16
            self._encrypt_bl2e()
            return
        assert False

    def extract_bb1st(self, fname):
        with open(fname, 'wb') as fout:
            fout.write(type(self)._update_checksum(self._buf[:254*1024]))
            return
        assert False

    def extract_blob_bl2e(self, fname):
        with open(fname, 'wb') as fout:
            buf = self._buf[254*1024:]
            fout.write(buf[:8192])
            buf = buf[8192:]

            buf = buf[:self._sz_blob_bl2e - 8192]
            fout.write(type(self)._update_checksum(buf))
            return
        assert False

def new(fname):
    return FlashImg(fname)

# vim: set expandtab ai tabstop=4 shiftwidth=4 softtabstop=-1:
