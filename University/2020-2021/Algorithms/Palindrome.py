
# Aaron Frost 

# CS 325 Portfolio Assignment Problem I
# 6/1/2021


# APPROACH 1 -------------------------------------------------

def checkPalindrome_1(string, k):
  return palindromeHelper_1(string, 0, k)

# Helper function which returns a new string with the character
# at index k removed
def removeAtIndex(string, k):
  newString = ""
  for i in range(len(string)):
    if i != k:
      newString = newString + string[i]

  return newString


def palindromeHelper_1(string, currRemovals, maxRemovals):

  # If we have made too many removals, using this decision branch won't work
  if(currRemovals > maxRemovals):
    return False

  # If we reach the center of the string, it is a palindrone
  if(len(string) == 1):
    return True
  else:

    # Compare the start index to the end index, if they are equal make another iteration inwards
    if(string[0] == string[len(string) - 1]):
      return palindromeHelper_1(removeAtIndex(removeAtIndex(string, len(string) -1), 0), currRemovals, maxRemovals)
    else:
      return (palindromeHelper_1(removeAtIndex(string, 0), currRemovals + 1, maxRemovals) 
      or palindromeHelper_1(removeAtIndex(string, len(string) - 1), currRemovals + 1, maxRemovals) 
      or palindromeHelper_1(removeAtIndex(removeAtIndex(string, len(string) -1), 0), currRemovals + 2, maxRemovals))





# APPROACH 2 -------------------------------------------------

def checkPalindrome_2(string, k):
     
  reversed = string[::-1]
  n = len(string)
    
  # Check if the result is less than or equal to 2 times the original 
  # maximum since our algorithm computes 2 times the value needed since it
  # iterates over two strings
  return (palindromeHelper_2(string, reversed, n) <= k * 2)


def palindromeHelper_2(originalString, reversedString, n):
     
  cache = [[0] * (n + 1) for _ in range(n + 1)]

  for i in range(n + 1):
        
    for j in range(n + 1):
        
      # Initialize number of elements removed to zero
      if(i == 0 or  j == 0):
        cache[i][j] = i + j

      # If they are the same, continue iterating "inwards"
      # by moving forwards in parallel on both strings
      elif (originalString[i - 1] == reversedString[j - 1]):
        cache[i][j] = cache[i - 1][j - 1]

      # If they are different store the solution to this palindrome
      # as the minimum solution for removing either the left or right
      # elements + 1 
      else:
        cache[i][j] = 1 + min(cache[i - 1][j],  (cache[i][j - 1])) 

  return cache[n][n]