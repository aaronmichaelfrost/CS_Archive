# Aaron Frost - May 18th, 2021


# Prim's and Kruskal's algorithm brute force implementation 
#    - they calculate minumum spannig trees given adjacency 
#       matrices


def Prims(G):
  result = []
  visited = [] 

  visited.append(0)


  # Visit every node
  while(len(visited) < len(G)):

    hugeNumber = 1000000
    minimum = [(0,0), hugeNumber]
      
    for a in visited:
      for b in range(0, len(G)):
        if b not in visited:
          if(G[a][b] < minimum[1] and G[a][b] > 0):
            minimum[0] = (a,b)
            minimum[1] = G[a][b]
          
    result.append(minimum)
    visited.append(minimum[0][1])

  return result


G = [[0, 9, 75, 0, 0],
          [9, 0, 95, 19, 42],
          [75, 95, 0, 51, 66],
          [0, 19, 51, 0, 31],
          [0, 42, 66, 31, 0]]

print("Prims: ")
print(Prims(G))




def Kruskal(G):

  # An edge: ((vertex1, vertex2), weight)

  visited = []
  edges = []

  for i in range(0, len(G)):
    for j in range(0, len(G)):
      if(G[i][j] > 0):
        edges.append(((i, j), G[i][j]))

  
  edgesSorted = sorted(edges, key=lambda tup: tup[1])

  MST = []

  for e in edgesSorted:

    # We've finished when we have included every vertex once
    if len(MST) == len(G):
      break;

    aVisited = False
    bVisited = False
      

    for v in visited:
      if e[0][0] == v:
        aVisited = True

      if e[0][1] == v:
        bVisited = True

    # Only append edges that contain a vertex that is not in the MST
    #   such to not create cycles
    if (not aVisited) or (not bVisited):
      MST.append(e)
      
      if not aVisited:
        visited.append(e[0][0])
      if not bVisited:
        visited.append(e[0][1])

  return MST


print("Kruskal: ")
print(Kruskal(G))