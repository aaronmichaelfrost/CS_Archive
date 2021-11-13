# Aaron Frost
# 4/7/2021
# CS 325
# Description: Divide and conquer to calculate the element in an array that appears most frequently (mode)
#               note: assumes that there is a mode



def mode(arr, start, end):

 
    # Base case
    if start == end:
        return arr[start]

    mid = (start + end) / 2

    left = mode(arr, start, mid)
    right = mode(arr, mid + 1, end)


    # If the mode of the two divisions is the same, default to returning the left node (although returning either is fine because they are the same)
    if left == right:
        return left

    leftcount = 0
    rightcount = 0


    # Count the occurences of the left
    for x in range(mid - start):
        if arr[x] == left:
            leftcount += 1


    # Count the occurences of the right
    for x in range(end - mid + 1):
        if arr[x] == right:
            rightcount += 1


    # Return which had more occurences
    if leftcount > rightcount:
        return left
    else:
        return right



def majority(arr):
    return mode(arr, 0, len(arr) - 1)




arr = [3,9,9,6,6,6,6,8,8,8,8,12,12,12,12,12]            # output 12
print(majority(arr))

arr = [3,2,3]                                           # ouput 3
print(majority(arr))

arr = [2,2,1,1,1,2,2]                                   # output 2
print(majority(arr))