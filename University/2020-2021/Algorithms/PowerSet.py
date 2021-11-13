# Aaron Frost - March 21st, 2021

# Adapted from pseudocode from Backtracking exploration, time complexity O(2^n)


def powerset_helper(idx, choices_made, arr, result):

  if idx < 0:
    result.append(choices_made[:])
    return

  choices_made.append(arr[idx])

  powerset_helper(idx - 1, choices_made, arr, result)

  choices_made.remove(choices_made[len(choices_made) - 1])

  powerset_helper(idx - 1, choices_made, arr, result)


def powerset(input):

    result = []

    powerset_helper(len(input)-1, [], input, result)

    return result


print(powerset([1,2,3]))