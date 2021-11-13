def ways(accumulator, target, cache):

  if accumulator in cache:
    return cache[accumulator]

  if(accumulator == target):
    return 1

  if(accumulator > target):
    return 0

  cache[accumulator] = ways(accumulator + 1, target, cache) + ways(accumulator + 2, target, cache)
  return cache[accumulator]


def blockpuzzle_dp(N):
  return ways(0, N, {})




print("Testing n = 2 [expect 2]")
print(blockpuzzle_dp(2))



print("Testing n = 3 [expect 3]")
print(blockpuzzle_dp(3))