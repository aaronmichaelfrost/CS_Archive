################################################################################
#
#   Aaron Frost
#   CS370 Programming Project 1
#   933659237
#   10/20/2021
#
#   Program finds strong collisions.
#
################################################################################

import os
from cryptography.hazmat.primitives import hashes 
from cryptography.hazmat.backends import default_backend

backend = default_backend()

lookup_table = {}


num_trials = 100
attempts = [0] * num_trials

for trial_index in range(num_trials):

    for i in range(256 * 256 * 256):
        x = os.urandom(16)
        digest = hashes.Hash(hashes.SHA256(), backend)
        digest.update(x)
        hash = digest.finalize()
        hash = hash[:3]

        if hash in lookup_table:
            print(i)
            attempts[trial_index] = i
            break

        lookup_table[hash] = True

print("Average was: ", sum(attempts) / len(attempts))