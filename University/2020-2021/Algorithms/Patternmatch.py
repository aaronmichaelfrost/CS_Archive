# Aaron Frost

# Portfolio Assignment Problem B
# 6/1/2021


def patternmatch(string, pattern):

  # Create a 2d boolean cache
  cache = [[False for i in range(len(pattern)+1)] for j in range(len(string)+1)]
  cache[0][0] = True
  

  # Initialize all elements in the first row of the cache 

  # The first row will be null unless the pattern is just '*'
  for i in range(1, len(pattern) + 1):
    if (pattern[i - 1] == '*'):
      cache[0][i] = cache[0][i - 1]


  for i in range(1, len(string) + 1):

    for j in range(1, len(pattern) + 1):

      # Copy value above or to the left
      if (pattern[j - 1] == '*'):
        cache[i][j] = cache[i - 1][j] or cache[i][j - 1]

      # If the characters from the pattern and the text are the same,
      # use the value diagonal up to the left
      elif (pattern[j-1] == '?' or pattern[j - 1] == string[i - 1]):
        cache[i][j] = cache[i - 1][j-1]

  return cache[len(string)][len(pattern)]