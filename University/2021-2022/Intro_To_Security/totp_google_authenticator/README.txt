Note:
This does not work on the OSU flip servers.

How to use:
1. run "pip install qrcode[pil]"

2. Set your desired email address by editing generate_qrcode.py

3. If you want to specify a different secret key, you can edit the secret/key in both python files
	I realize in an actual application you'd provide a user interface for the user to do this,
	but it did not say we needed to do this in the assignment description.

4. run python generate_qrcode.py to output a qr code into "qrcode.jpeg"

5. Scan this jpeg with Google Authenticator

6. Run python get_otp.py, it will output the 6 digit TOTP