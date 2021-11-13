# Aaron Frost - 4/27/2021


def feedDog(hunger_levels, biscuit_sizes):

  hunger_levels.sort()
  biscuit_sizes.sort()

  current_biscuit = 0

  for i in range(0, len(hunger_levels)):
    if biscuit_sizes[current_biscuit] >= hunger_levels[i]:
      current_biscuit += 1
    else:
      break

  return current_biscuit


print(feedDog([1,2,3], [1,1]))
print(feedDog([1,2], [1,2,3]))