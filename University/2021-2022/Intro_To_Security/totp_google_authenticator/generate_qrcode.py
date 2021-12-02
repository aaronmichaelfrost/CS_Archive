# Program generates a jpg qr code that encodes a URI that Google Authenticator expects
# URI contains secret key and userid 
# Example URI
# otpauth://totp/Example:alice@google.com?secret=JBSWY3DPEHPK3PXP&issuer=Example

import qrcode

URI = 'otpauth://totp/Example:xxtheivesguildxx@google.com?secret=JBSWY3DPEHPK3PXP&issuer=Example'
output = "qrcode.jpeg"
img = qrcode.make(URI)
img.save(output)