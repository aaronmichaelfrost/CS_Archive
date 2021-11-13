# Aaron Frost - March 21st, 2021

# This program computes the subset of non-adjacent elements in an array that produces the maximum sum

# It uses classic dynamic programming techniques including memoization and recursion


def max_independent_set(nums):
  cache = {}
  fill_cache(0, len(nums), nums, cache)
  return extract_subset(cache)


# Fills the cache with sums
def fill_cache(i, n, arr, cache):

  # Memoization
  if i in cache:
    return cache[i]

  # Base case
  if i >= n:
    cache[i] = 0
    return cache[i]

  # Sub problem / recursive formula
  cache[i] = max(fill_cache(i+1, n, arr, cache), arr[i] + fill_cache(i+2, n, arr, cache))

  return cache[i]



# Retrace the recursion, return the optimal subset
def extract_subset(cache):

  subset = []

  i = 0
  n = len(cache)

  while i < n:

    a = cache[i]

    if i + 2 > n - 1:
      b = 0
    else:
      b = cache[i + 2]

    c = a - b

    if c > 0:
      subset.append(c)

    i += 2

  return subset




nums = [7,2,5,8,6]
print(max_independent_set(nums))