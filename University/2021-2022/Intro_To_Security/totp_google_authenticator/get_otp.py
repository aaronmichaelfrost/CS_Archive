# Generate's a one time password. Key is defined in code

import time
import hashlib
import hmac
import base64
import struct
import sys



def generate_otp_loop():
    now = int(time.time())
    interval = 30
    counter = int(now / interval)

    # Key is base32 encoded
    key = "JBSWY3DPEHPK3PXP"
    digits = 6
    key = base64.b32decode(key.upper() + '=' * ((8 - len(key)) % 8))
    counter = struct.pack('>Q', counter)
    # hash the counter with the key
    mac = hmac.new(key, counter, 'sha1').digest()
    offset = mac[-1] & 0x0f
    binary = struct.unpack('>L', mac[offset:offset+4])[0] & 0x7fffffff
    the_string = str(binary)[-digits:].zfill(digits)
    print(the_string)
    print("waiting ", interval, " seconds")
    time.sleep(interval)
    generate_otp_loop()

generate_otp_loop()
