#
#   Aaron Frost
#   CS370
#   Programming Project 2 - Bloom Filters
#   11/17/2021
#   Program will create two Bloom filters (one using 3 hash functions, one using 5) given an input file with a password on each line. 
#       Then it will check each password in a second input file against the bloom filter, writing to output.
#




import sys
from cryptography.hazmat.primitives import hashes 
from cryptography.hazmat.backends import default_backend
import base64

# Arguments are 
# 1: dictionary
# 2: input
# 3: ouptut3 
# 4: output5

backend = default_backend()

# Get command line arguments
dictionary = sys.argv[1]
inputFile = sys.argv[2]
output3 = sys.argv[3]
output5 = sys.argv[4]

filter_size = 16777216
filter = [False] * filter_size

dictionaryStream = open(dictionary, 'r')
dictionaryLines = dictionaryStream.readlines()


print("Creating first bloom filter. . . ")

# First let's create a bloom filter set with 3 hash functions
for line in dictionaryLines:

    strippedLine = line.strip()

    # SHA256
    digest = hashes.Hash(hashes.SHA256(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

    # MD5
    digest = hashes.Hash(hashes.MD5(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

    # SHA1
    digest = hashes.Hash(hashes.SHA1(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True



# Now lets hash the input and see if it is in the filter

print("Processing input. . . ")
inputStream = open(inputFile)
inputLines = inputStream.readlines()

output3Stream = open(output3, 'w')

for i in range(1, len(inputLines)):
    passes = False
    strippedLine = inputLines[i].strip()

    digest = hashes.Hash(hashes.SHA256(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
            passes = True

    digest = hashes.Hash(hashes.MD5(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
        passes = True
            
    digest = hashes.Hash(hashes.SHA1(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
        passes = True

    if(passes):
        output3Stream.write(strippedLine + " no\n")
    else:
        output3Stream.write(strippedLine + " maybe\n")


# Now let's create a bloom filter set with 5 hash functions
print("Creating second bloom filter. . . ")

for line in dictionaryLines:

    strippedLine = line.strip()

    # SHA256
    digest = hashes.Hash(hashes.SHA256(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

    # MD5
    digest = hashes.Hash(hashes.MD5(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

    # SHA1
    digest = hashes.Hash(hashes.SHA1(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

    # SHA384
    digest = hashes.Hash(hashes.SHA384(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

    # SHA512
    digest = hashes.Hash(hashes.SHA512(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    filter[the_hash] = True

print("Processing input. . . ")

output5Stream = open(output5, 'w')

for i in range(1, len(inputLines)):
    passes = False
    strippedLine = inputLines[i].strip()

    digest = hashes.Hash(hashes.SHA256(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
            passes = True

    digest = hashes.Hash(hashes.MD5(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
        passes = True
            
    digest = hashes.Hash(hashes.SHA1(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
        passes = True

    digest = hashes.Hash(hashes.SHA384(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
        passes = True
            
    digest = hashes.Hash(hashes.SHA512(), backend)
    digest.update(strippedLine)
    the_hash = int(digest.finalize().encode('hex'), 16) % filter_size
    if(filter[the_hash] == False):
        passes = True


    if(passes):
        output5Stream.write(strippedLine + " no\n")
    else:
        output5Stream.write(strippedLine + " maybe\n")
        