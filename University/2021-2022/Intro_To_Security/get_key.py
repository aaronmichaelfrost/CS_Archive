################################################################################
#
#   Aaron Frost
#   CS370 Programming Project 1
#   933659237
#   10/20/2021
#
#   Program finds key given ciphertext and plaintext.
#
################################################################################


from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding
import urllib2
import ssl


plaintext = "This is a top secret."
ciphertext_hex = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"
ciphertext_data = ciphertext_hex.decode("hex")
iv = bytes(bytearray(16))
url = "https://web.ecs.syr.edu/~wedu/seed/Labs/Crypto/Crypto_Encryption/files/words.txt"
context = ssl._create_unverified_context()
backend = default_backend()
file = urllib2.urlopen(url, context=context)


# Pad the secret message
secret_message = bytes(plaintext)
padder = padding.PKCS7(128).padder()
secret_message_padded = padder.update(secret_message)
secret_message_padded += padder.finalize()


matched_key = ""

# for each possible key
for line in file:
    decoded_line = line.decode("utf-8")
    decoded_line = decoded_line.rstrip() # remove trailing newline character
    if len(decoded_line) < 16:
        padded = decoded_line.ljust(16, " ") # pad with spaces

        key = bytes(padded)
        cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend) # use key to encrypt plaintext
        encryptor = cipher.encryptor()
        ct = encryptor.update(secret_message_padded) + encryptor.finalize()

        if ct == ciphertext_data: # if the ciphertext is the original ciphertext we have found the key
            matched_key = decoded_line
            break

print(matched_key)

exit()