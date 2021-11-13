
# Aaron Frost

# CS325 Portfolio Assignment Problem C
# 6/1/2021

def getTesla(M):

  maxPath = maxHealthRecursive(0,0,0,M)

  print("max path cost is ", maxPath)

  if(maxPath < 0):
    return (maxPath * -1) + 1
  return 1


def maxHealthRecursive(i, j, health, maze):

  health += maze[i][j]


	# Base Case: X has reached the end of the maze
  if (i == len(maze) - 1 and j == len(maze[0]) - 1):
    return health;

  downHealth = -10000000  
  rightHealth = -10000000


  if (i + 1 < len(maze)):
	  downHealth = maxHealthRecursive(i + 1, j, health, maze);
		

  if (j + 1 < len(maze[0])):
    rightHealth = maxHealthRecursive(i, j + 1, health, maze);

  return max(downHealth, rightHealth)