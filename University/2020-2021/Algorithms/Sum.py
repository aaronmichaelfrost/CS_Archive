# Aaron Frost - 4/27/2021


def combination_helper(length, result, target, accumulator, combination, start):

  # if accumulation has surpassed target, return
  if accumulator > target or start - 1 > length:
    return


  # if we have found a combination that adds up to the target sum and is of 
  # the correct length, add it if it isn't already in the array
  if accumulator == target and start - 1 == length:

    sorted_combination = sorted(combination)

    if sorted_combination not in result:
      result.append(combination[:])

    return


  # for each possible integer from 1-9 that hasn't already been added, add 
  # it to the combination, then remove it to backtrack
  for i in range(start,10):
    if i not in combination:
      combination.append(i)
      combination_helper(length, result, target, i + accumulator, combination, start + 1)
      combination.pop()
      




def combination(n,k):
  result = []
  combination_helper(n, result, k, 0, [], 1)
  return result


print(combination(3,9))

print(combination(3,6))