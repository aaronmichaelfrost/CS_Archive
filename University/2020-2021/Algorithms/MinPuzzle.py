
# Aaron Frost - May 12th, 2021
# Implementation of Dijkstra's shortest path algorithm BFS
#   time complexity: O(V^2) if V = rows * cols

def minEffort(puzzle):
  leastEffort = {}

  unvisited = [0 for i in range(len(puzzle[0] * len(puzzle)))]


  for i in range(0, len(puzzle)):
    for j in range(0, len(puzzle[0])):
      unvisited[i * len(puzzle[0]) + j] = (i,j)


  visited = [()]

  optimalPath = []

  hugeNumber = 99999999


  # Predecessor is the coordinates of the last element visited
  predecessor = {}

  start = puzzle[0][0]

  for node in unvisited:
    leastEffort[node] = hugeNumber
  

  leastEffort[(0 , 0)] = 0

  while unvisited:
    optimalNode = None

    for node in unvisited:

        if(optimalNode is None):
          optimalNode = node
        elif leastEffort[node] < leastEffort[optimalNode]:
          optimalNode = node


    children = []

    # Create a list of children (top, left, bottom, right)

    above = (optimalNode[0] - 1, optimalNode[1])
    right = (optimalNode[0], optimalNode[1] + 1)
    below = (optimalNode[0] + 1, optimalNode[1])
    left = (optimalNode[0], optimalNode[1] - 1)

    # Above
    if(above[0] > -1 and not (above in visited)):
      children.append(above)

    # Right
    if(right[1] < len(puzzle[0]) and not (right in visited)):
      children.append(right)
      
    # Below
    if(below[0] < len(puzzle) and not (below in visited)):
      children.append(below)

    # Left 
    if(left[1] > -1 and not (left in visited) ):
      children.append(left)
    

    for childNode in children:
      weight = max(leastEffort[optimalNode], abs(puzzle[childNode[0]][childNode[1]] - puzzle[optimalNode[0]][optimalNode[1]]))

      if(weight < leastEffort[childNode]):
        leastEffort[childNode] = weight
        predecessor[childNode] = optimalNode

    unvisited.remove(optimalNode)
    visited.append(optimalNode)

  currentNode = (len(puzzle) - 1, len(puzzle[0]) - 1)


  while (currentNode != start):
    try:
      optimalPath.insert(0, puzzle[currentNode[0]][currentNode[1]])

      currentNode = predecessor[currentNode]
    except KeyError:
      break


  if(not(leastEffort[(len(puzzle) - 1, len(puzzle[0]) - 1)] == hugeNumber)):
    print(optimalPath)

  print("Minimal effort route has effort value of ", leastEffort[(len(puzzle) - 1, len(puzzle[0]) - 1)])

  return leastEffort[(len(puzzle) - 1, len(puzzle[0]) - 1)]





print(minEffort([[1, 3, 5], [2, 8, 3], [3, 4,5]]))


minEffort([[1,100,7,8,1000],[2,100,6,9,1000],[3,4,5,10,1000], [14,13,12,11,1000], [15,16,17,18,19], [20,21,22,23,24]])