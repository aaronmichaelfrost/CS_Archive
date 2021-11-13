
# Aaron Frost 2021

def game_topdown(N):

  if N == 1: 
    return False

  if N == 2:
    return True

  for i in range(1, int(N/2) + 1):
    j = N-i
    
    if N % i == 0 and not game_topdown(j):
      return True

  return False




# this is O(n) complexity 
# example call: print(game_topdown_memoized(10, {}))
def game_topdown_memoized(N, cache):

  if N in cache:
    return cache[N]

  if N == 1: 
    return False

  if N == 2:
    return True


  for i in range(1, int(N/2) + 1):
    j = N-i
    
    if N % i == 0 and not game_topdown(j):
      cache[N] = True
      return True

  cache[N] = False
  return False



def game_bottomup(N):

  game = {}
  game[1] = False
  game[2] = True

  for i in range(3, N + 1):

    for k in range(1, int(i/2) + 1):

      j = N-k

      if N % k == 0 and not game_bottomup(j):
        game[k] = True
        return True
      else:
        game[k] = False
        return False


  return game[N]
  
  
print("Top-down tests")
print("Testing N=2 expected true", game_topdown(2))
print("Testing N=3 expected false", game_topdown(3))
print("Testing N=4 expected true", game_topdown(4))

print("Top-down memoized tests")
print("Testing N=2 expected true", game_topdown_memoized(2, {}))
print("Testing N=3 expected false", game_topdown_memoized(3, {}))
print("Testing N=4 expected true", game_topdown_memoized(4, {}))

print("Bottom-up tests")
print("Testing N=2 expected true", game_bottomup(2))
print("Testing N=3 expected false", game_bottomup(3))
print("Testing N=4 expected true", game_bottomup(4))
