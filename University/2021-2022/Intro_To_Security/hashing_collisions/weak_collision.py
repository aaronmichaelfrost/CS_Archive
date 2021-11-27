################################################################################
#
#   Aaron Frost
#   CS370 Programming Project 1
#   933659237
#   10/20/2021
#
#   Program finds weak collisions.
#
################################################################################

import os
from cryptography.hazmat.primitives import hashes 
from cryptography.hazmat.backends import default_backend

backend = default_backend()

# First lets break the weak collision resistance 



# Hash random values until we find one that matches the first hash

num_trials = 30
attempts = [0] * num_trials

for trial_index in range(num_trials):

    # Given some value:
    plaintext = os.urandom(16)

    # Hash it
    digest = hashes.Hash(hashes.SHA256(), backend)
    digest.update(plaintext)
    first_hash = digest.finalize()

    # Only use the first 3 bytes so it doesn't take forever
    first_hash = first_hash[:3]

    second_hash = ""
    i = 0
    while second_hash != first_hash:
        i += 1
        digest = hashes.Hash(hashes.SHA256(), backend)

        random = os.urandom(16)
        
        if(random == plaintext): # Make sure the new plaintext is not the same as the old
            continue

        digest.update(random)
        
        second_hash = digest.finalize()
        second_hash = second_hash[:3]
        
    attempts[trial_index] = i
    print(i)

print("Average: ", sum(attempts) / len(attempts))