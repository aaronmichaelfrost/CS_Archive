# BoxAlgorithm
# Aaron Frost 
# 4/7/2021
# CS 325
# Description: Function for checking if two boxes overlap given the coordinates of the bottom left followed by top right coordinates of 2d boxes


def doBoxesOverlap(box1, box2):
    return ((box1[0] < box2[0] and box2[0] < box1[2]) or (box1[0] < box2[2] and box2[2] < box1[2])) and ((box1[1] < box2[1] and box2[1] < box1[3]) or (box1[1] < box2[3] and box2[3] < box1[3]))
# returns true if x sides overlap and y sides overlap



# Tests
print(doBoxesOverlap([0,0,2,2], [1,1,3,3]))

print(doBoxesOverlap([0,0,1,1], [1,0,2,1]))


# Can recursion be done?

# I'm sure you could use recursion in this problem, like for instance, instead of checking the y sides through hard code I could have incremented the array index on the original and performed 
# the check once more. I don't think that this problem is inherently recursive though, at least with my approach. I'd love to know if there is a more clever and less "hard coded" approach.