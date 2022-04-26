#!/usr/bin/python3 -B

import argparse
import binascii

from Crypto.Cipher import AES
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Signature import PKCS1_v1_5
from Crypto.Random import new as Random

#
# Global variables
#

global_vars = {}

#
# Functions
#

def modinv(a, m):
    from Crypto.Util.number import inverse
    return inverse(a, m)

def load_vars_from_file(var_filename, varname, varsize):
    global_vars[varname] = None
    if vars(args)[var_filename] is not None:
        with open(vars(args)[var_filename], 'rb') as f:
            ba_var = f.read()
            assert len(ba_var) == varsize
            global_vars[varname] = ba_var

#
# Classes
#

class _Util:
    def genDUID(usage):
        duid_full = SHA256.new(usage).digest()
        duid = duid_full[:16]
        duid_ext = duid_full[16:]
        return (duid, duid_ext)

class CommonHeader:
    def __init__(self, ba_name):
        magic = SHA256.new(ba_name).digest()
        self.__magic64 = b'@ML ' + magic[:4]
        #print(binascii.hexlify(self.__magic64))

        self.setVersion(0, 0)
        self.setFlags(0)
        self.setLength(0)

    def setVersion(self, major, minor):
        assert major.bit_length() <= 8
        assert minor.bit_length() <= 8
        self.__version =  major.to_bytes(1, byteorder='little')
        self.__version += minor.to_bytes(1, byteorder='little')

    def setFlags(self, flags):
        assert flags.bit_length() <= 16
        self.__flags = flags

    def setLength(self, length):
        assert length.bit_length() <= 32
        self.__length = length

    def packData(self):
        data = b''
        data += self.__magic64
        data += self.__version
        data += self.__flags.to_bytes(2, byteorder='little')
        data += self.__length.to_bytes(4, byteorder='little')
        return data

""" Data Structure: Public RSA Key """
class DSPubRSAKey:
    def __init__(self, pem_file):
        self.__rsakey = None

        # Common Header
        self.__cmn_hdr = CommonHeader(b'AML: PUBLIC RSA KEY')
        self.__cmn_hdr.setVersion(0, 0)
        self.__cmn_hdr.setFlags(0)

        """ Load RSA key from PEM file """
        if pem_file is not None:
            f_rsakey = open(pem_file)
            self.__rsakey = RSA.importKey(f_rsakey.read())
            self.__nbits_rsakey = self.__rsakey.n.bit_length()

            while True:
                if self.__nbits_rsakey == 2048:
                    break

                if self.__nbits_rsakey == 4096:
                    break

                assert False, "RSA Key must be 2048-bit or 4096-bit"

            f_rsakey.close();

            self.__prepare_pubrsa_keydata()

        return

    def __prepare_pubrsa_keydata(self):
        """ Preparation """
        # Modulus: N
        self.__nbytes_rsakey = self.__nbits_rsakey//8
        self.__ba_rsakey_n = self.__rsakey.n.to_bytes(self.__nbytes_rsakey,
                                                      byteorder='little')
        self.__ba_rsakey_n += b'\x00' * (512 - len(self.__ba_rsakey_n))
        #print(binascii.hexlify(self.__ba_rsakey_n))

        # Exponent: E
        self.__ebits_rsakey = self.__rsakey.e.bit_length()
        assert self.__ebits_rsakey <= 32
        self.__ba_rsakey_e = self.__rsakey.e.to_bytes(4, byteorder='little')
        #print(binascii.hexlify(self.__ba_rsakey_e))

        # Reserved
        self.__ba_reserved = b'\x00' *8

        # RR: R*R mod N
        rr = pow(2, self.__nbits_rsakey*2, self.__rsakey.n)
        rinv = modinv(2**self.__nbits_rsakey, self.__rsakey.n)
        assert rr*rinv*rinv % self.__rsakey.n == 1
        self.__ba_rr = rr.to_bytes(self.__nbytes_rsakey, byteorder='little')
        self.__ba_rr += b'\x00' * (512 - len(self.__ba_rr))
        #print('0x{:x}'.format(rr))
        #print(binascii.hexlify(self.__ba_rr))

        # Ninv: -(N^-1) % 2^128
        ninv = modinv(self.__rsakey.n, 2**self.__nbits_rsakey)
        ninv = 2**self.__nbits_rsakey - ninv
        assert ((ninv*self.__rsakey.n+1) % 2**self.__nbits_rsakey) == 0

        ba_ninv = ninv.to_bytes(self.__nbytes_rsakey, byteorder='little')
        self.__ba_ninv128 = ba_ninv[:(128//8)]
        ninv128 = int.from_bytes(self.__ba_ninv128, byteorder='little')
        assert (ninv128*(self.__rsakey.n % (2**128)) + 1) % (2**128) == 0

    def getRSAKey(self):
        return self.__rsakey

    def __packData(self):
        data = b''

        data += self.__cmn_hdr.packData()

        data += (self.__nbytes_rsakey//4).to_bytes(4, byteorder='little')
        data += self.__ba_rsakey_e
        data += self.__ba_reserved
        data += self.__ba_rsakey_n
        data += self.__ba_rr
        data += self.__ba_ninv128

        return data

    def packData(self):
        if self.__rsakey is None:
            return b'\0' * 1072

        length = len(self.__packData())
        #print(length)
        assert length == 1072

        self.__cmn_hdr.setLength(length)
        return self.__packData()

class DSRootCert:
    def __init__(self):
        self.__aeskey = None

        # Common Header
        self.__cmn_hdr = CommonHeader(b'AML: ROOT CERTIFICATE')

        # DUID
        usage = b'DUID/CERT: SECPU ROOT CERTIFICATE'
        ( self.__meta_duid, _ ) = _Util.genDUID(usage + b' - METADATA')
        #print(self.__meta_duid)
        #print(binascii.hexlify(self.__meta_duid))

        # Tables of Hashes
        self.__tbl_hashes = [ b'\x00' * 32, b'\x00' * 32,
                              b'\x00' * 32, b'\x00' * 32 ]
        self.__vld_hashes = [ False, False, False, False ]

        # EPKs: EPK1_Lo, EPK2_Lo, EPK2_Hi, EPK2_Hi
        self.__epks = [ b'\x00' * 16, b'\x00' * 16,
                        b'\x00' * 16, b'\x00' * 16 ]

        ### Associated data of Public RSA Key ###
        self.__idx_rsakey = -1
        self.__ba_reserved = b'\x00' * 31

        # Random nonce for protecting Public RSA Key
        self.__ba_rnd_nonce = Random().read(16)

        # Empty buffer for Public RSA Key
        self.__ba_pubrsakey = b'\x00' * 1072

        ### DUID of Root Public RSA Key ###
        ( self.__rootrsa_duid, _ ) = _Util.genDUID(usage + b' - ROOT PUBLIC RSA KEY')

    def setHash(self, idx, ba_key):
        assert len(ba_key) == 1072

        if ba_key == b'\0' * 1072:
            self.__tbl_hashes[idx] = b'\0' * 32
            self.__vld_hashes[idx] = True
            return

        """ Compose PubRSA key with associated data """
        buf  = idx.to_bytes(1, byteorder='little')
        buf += self.__ba_reserved
        buf += b'\0' * 16
        buf += ba_key
        buf += self.__rootrsa_duid
        assert len(buf) == 1136
        #print(binascii.hexlify(buf))

        self.__tbl_hashes[idx] = SHA256.new(buf).digest()
        self.__vld_hashes[idx] = True

    def setPubRSAKey(self, idx, ba_key):
        self.__idx_rsakey = idx
        assert len(self.__ba_pubrsakey) == len(ba_key)
        self.__ba_pubrsakey = ba_key

    def setAESKey(self, key):
        self.__aeskey = key
        assert len(self.__aeskey) == 16

    def setEk2PubRSA(self, ek):
        if ek is None:
            return

        assert len(ek) == 16
        self.__epks[0] = ek

    def setEk2Loader(self, ek):
        if ek is None:
            return

        assert len(ek) == 16
        self.__epks[1] = ek

    def setEk1PubRSA(self, ek):
        if ek is None:
            return

        assert len(ek) == 16
        self.__epks[2] = ek

    def setEk1Loader(self, ek):
        if ek is None:
            return

        assert len(ek) == 16
        self.__epks[3] = ek

    def setNoncePubRSA(self, nonce):
        if nonce is None:
            return

        assert len(nonce) == 16
        self.__ba_rnd_nonce = nonce

    def __packData(self, metaonly=False):
        data = b''

        # Metadata
        data += self.__cmn_hdr.packData()

        data += self.__tbl_hashes[0]
        data += self.__tbl_hashes[1]
        data += self.__tbl_hashes[2]
        data += self.__tbl_hashes[3]

        data += self.__epks[0]
        data += self.__epks[1]
        data += self.__epks[2]
        data += self.__epks[3]

        # Reserved
        data += b'\0' * 48

        # DUID of Root Certificate Metadata
        data += self.__meta_duid

        # Return only Metadata
        if metaonly:
            return data

        ### Public RSA Key and Associated Information ###
        buf = b''

        buf += self.__idx_rsakey.to_bytes(1, byteorder='little')
        buf += self.__ba_reserved

        # Fill nonce with 0s for blank PubRSA Key
        assert len(self.__ba_pubrsakey) == 1072
        if self.__ba_pubrsakey == b'\0' * 1072:
            self.__ba_rnd_nonce = b'\0' * 16

        # Encrypt Public RSA Key with random nonce
        tmpbuf = b''
        tmpbuf += self.__ba_rnd_nonce
        tmpbuf += self.__ba_pubrsakey

        if self.__aeskey is not None:
            cipher = AES.new(self.__aeskey, AES.MODE_CBC, b'\x00' * 16)
            tmpbuf = cipher.encrypt(tmpbuf)

        buf += tmpbuf

        # DUID of Root Public RSA Key
        buf += self.__rootrsa_duid


        data += buf

        return data

    def packData(self):
        assert self.__vld_hashes[0]
        assert self.__vld_hashes[1]
        assert self.__vld_hashes[2]
        assert self.__vld_hashes[3]

        assert self.__idx_rsakey >= 0
        assert self.__idx_rsakey <= 3

        length = len(self.__packData())
        #print(length)
        assert length == 1408

        self.__cmn_hdr.setLength(length)
        return self.__packData()

    def getHashLock(self):
        data = self.__packData(True)
        #print(len(data))
        #print(binascii.hexlify(data))

        return SHA256.new(data).digest()

""" Data: SECPU Loader """
class DataSECPULoader:
    def __init__(self, loader_file):
        self.__aeskey = None
        self.__sig_loader = None

        ### Controlling Data of SECPU Loader
        self.__seg_id = None
        self.__mask_segid = 0xFFFFFFFF;
        self.__arb_vers = None
        self.__mask_arbvers = 0xFFFFFFFF;

        ### DUID of SECPU Loader ###
        ( self.__loader_duid, _ ) = _Util.genDUID(b'DUID/BLOB: SECPU LOADER')
        #print(binascii.hexlify(self.__loader_duid))

        """ Read file and do sanity check"""
        f_loader = open(loader_file, 'rb')
        self.__ba_loader = f_loader.read()
        len_loader = len(self.__ba_loader)

        while True:
            if len_loader == (28*1024):
                break;

            if len_loader == (60*1024):
                break;

            if len_loader == (76*1024):
                break;

            if len_loader == (92*1024):
                break;

            assert False, "Length of Loader file must be 28 KiB, 60 KiB, 76 KiB or 92 KiB"
        f_loader.close()

    def setAESKey(self, key):
        self.__aeskey = key
        assert len(self.__aeskey) == 16

    def setSegID(self, seg_id):
        if seg_id is None:
            return

        assert isinstance(seg_id, int)
        assert seg_id.bit_length() <= 32
        self.__seg_id = seg_id;

    def setArbVers(self,arb_vers):
        if arb_vers is None:
            return

        assert isinstance(arb_vers, int)
        assert arb_vers.bit_length() <= 32
        self.__arb_vers = arb_vers

    def updateCtrlData(self):
        # Not inject DUID and controlling data if bot SegID and ArbVers are None
        if self.__seg_id is None and self.__arb_vers is None:
            return

        # Inject DUID and controlling data
        buf = b''
        buf += self.__loader_duid
        buf += self.__seg_id.to_bytes(4, byteorder='little')
        buf += self.__mask_segid.to_bytes(4, byteorder='little')
        buf += self.__arb_vers.to_bytes(4, byteorder='little')
        buf += self.__mask_arbvers.to_bytes(4, byteorder='little')
        assert len(buf) == 32

        sz_loader = len(self.__ba_loader)
        self.__ba_loader = self.__ba_loader[:-32] + buf
        assert sz_loader == len(self.__ba_loader)

    def genSig(self, rsakey):
        hash_loader = SHA256.new(self.__ba_loader)
        #print(hash_loader.hexdigest())

        self.__sig_loader = PKCS1_v1_5.new(rsakey).sign(hash_loader)

        #self.__sig_loader = b'\x00' + self.__sig_loader[1:]
        #self.__sig_loader = b'd' + self.__sig_loader[1:]
        assert PKCS1_v1_5.new(rsakey).verify(hash_loader, self.__sig_loader)

    def packLoader(self):
        data = self.__ba_loader

        if self.__aeskey is not None:
            cipher = AES.new(self.__aeskey, AES.MODE_CBC, b'\x00' * 16)
            data = cipher.encrypt(data)

        return data

    def packSig(self):
        if self.__sig_loader is None:
            return b'\0' * 512

        data  = self.__sig_loader
        data += b'\x00' * (512 - len(data))
        return data

#
# Main
#

""" Parse arguments """
parser = argparse.ArgumentParser(description='Generate SECPU Loader Image')

parser.add_argument('--aes128-keyfile-pubrsa', dest='aes128-keyfile-pubrsa')
parser.add_argument('--aes128-keyfile-loader', dest='aes128-keyfile-loader')

parser.add_argument('--ek2file-pubrsa', dest='ek2file-pubrsa')
parser.add_argument('--ek2file-loader', dest='ek2file-loader')
parser.add_argument('--ek1file-pubrsa', dest='ek1file-pubrsa')
parser.add_argument('--ek1file-loader', dest='ek1file-loader')

parser.add_argument('--noncefile-pubrsa', dest='noncefile-pubrsa')

parser.add_argument('--seg-id', dest='seg-id')
parser.add_argument('--arb-vers', dest='arb-vers')

parser.add_argument('--rootrsa-priv-key', dest='rootrsa-priv-key', required=False)
parser.add_argument('--rootrsa-idx', dest='rootrsa-idx', required=False, default=0)
parser.add_argument('--rootrsa-key-0', dest='rootrsa-key-0', required=False)
parser.add_argument('--rootrsa-key-1', dest='rootrsa-key-1', required=False)
parser.add_argument('--rootrsa-key-2', dest='rootrsa-key-2', required=False)
parser.add_argument('--rootrsa-key-3', dest='rootrsa-key-3', required=False)
parser.add_argument('--output', dest='output-file', required=True)
parser.add_argument('--out-hash', dest='output-hash-file')
parser.add_argument('LOADER_FILE', help='SECPU Loader')

args = parser.parse_args()

rootrsa_idx = int(vars(args)['rootrsa-idx'])
assert 0 <= rootrsa_idx and rootrsa_idx <= 3

""" AES Key for PubRSA """
if vars(args)['aes128-keyfile-pubrsa'] is not None:
    f_key = open(vars(args)['aes128-keyfile-pubrsa'], 'rb')
    ba_key_pubrsa = f_key.read()
    assert len(ba_key_pubrsa) == 16
    f_key.close()
else:
    ba_key_pubrsa = None

""" AES Key for Loader """
if vars(args)['aes128-keyfile-loader'] is not None:
    f_key = open(vars(args)['aes128-keyfile-loader'], 'rb')
    ba_key_loader = f_key.read()
    assert len(ba_key_loader) == 16
    f_key.close()
else:
    ba_key_loader = None

""" EKs """
load_vars_from_file('ek2file-pubrsa', 'ek2-pubrsa', 16)
load_vars_from_file('ek2file-loader', 'ek2-loader', 16)
load_vars_from_file('ek1file-pubrsa', 'ek1-pubrsa', 16)
load_vars_from_file('ek1file-loader', 'ek1-loader', 16)

""" Nonce for protecting PubRSA """
load_vars_from_file('noncefile-pubrsa', 'nonce-pubrsa', 16)

""" Controlling Data """
global_vars['seg-id'] = None
strval = vars(args)['seg-id']
if strval is not None:
    val = int(strval, 0)
    assert val.bit_length() <= 32
    global_vars['seg-id'] = val

global_vars['arb-vers'] = None
strval = vars(args)['arb-vers']
if strval is not None:
    val = int(strval, 0)
    assert val.bit_length() <= 32
    global_vars['arb-vers'] = val

""" Private RSA Key """
if vars(args)['rootrsa-priv-key'] is not None:
    with open(vars(args)['rootrsa-priv-key'], 'r') as f_privkey:
        obj_priv_rsakey = RSA.importKey(f_privkey.read())
        assert obj_priv_rsakey.has_private() is True
else:
    obj_priv_rsakey = None

""" Public RSA Key """
tbl_ds_pubrsakey = [ None, None, None, None]
tbl_ba_pubrsa = [ None, None, None, None]

for i in range(4):
    tbl_ds_pubrsakey[i] = DSPubRSAKey(vars(args)['rootrsa-key-' + str(i)])
    tbl_ba_pubrsa[i] = tbl_ds_pubrsakey[i].packData()
#print(binascii.hexlify(tbl_ba_pubrsa[0]))

""" Load SECPU Loader """
secpu_loader = DataSECPULoader(args.LOADER_FILE)
if ba_key_loader is not None:
    secpu_loader.setAESKey(ba_key_loader)

secpu_loader.setSegID(global_vars['seg-id'])
secpu_loader.setArbVers(global_vars['arb-vers'])
secpu_loader.updateCtrlData()

""" Generate RSA Signature """
if obj_priv_rsakey is not None:
    obj_pub_rsakey = tbl_ds_pubrsakey[rootrsa_idx].getRSAKey()
    assert obj_pub_rsakey.n == obj_priv_rsakey.n
    assert obj_pub_rsakey.e == obj_priv_rsakey.e
    secpu_loader.genSig(obj_priv_rsakey)

""" Root Certificate """
ds_rootcert = DSRootCert()
if ba_key_pubrsa is not None:
    ds_rootcert.setAESKey(ba_key_pubrsa)

for i in range(4):
    ds_rootcert.setHash(i, tbl_ba_pubrsa[i])

ds_rootcert.setPubRSAKey(rootrsa_idx, tbl_ba_pubrsa[rootrsa_idx])

ds_rootcert.setEk2PubRSA(global_vars['ek2-pubrsa'])
ds_rootcert.setEk2Loader(global_vars['ek2-loader'])
ds_rootcert.setEk1PubRSA(global_vars['ek1-pubrsa'])
ds_rootcert.setEk1Loader(global_vars['ek1-loader'])

ds_rootcert.setNoncePubRSA(global_vars['nonce-pubrsa'])

ba_rootcert = ds_rootcert.packData()
ba_hashlock = ds_rootcert.getHashLock()
#print(binascii.hexlify(ba_rootcert))

print("SHA256 for OTP Hash-Lock:")
print(binascii.hexlify(ba_hashlock).decode('UTF-8'))

fname_hash = vars(args)['output-hash-file']
if fname_hash is not None:
    f_hash = open(fname_hash, 'wb')
    f_hash.write(ba_hashlock)
    f_hash.close()

#
# Pack SECPU Loader Image
#

ba_sig = secpu_loader.packSig()
ba_loader = secpu_loader.packLoader()

buf  = ba_rootcert
buf += (4096 - 32 - len(ba_sig) - len(buf)) * b'\x00'
buf += ba_sig
assert len(buf) == (4096 - 32)

buf += ba_loader

chksum = SHA256.new(buf).digest()
buf = chksum + buf
#print(len(buf))
#print(binascii.hexlify(buf))

f_img = open(vars(args)['output-file'], "wb")
f_img.write(buf)
f_img.close()

# vim: set expandtab ai tabstop=4 shiftwidth=4 softtabstop=-1:
