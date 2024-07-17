import numpy as np

class Node:
    def __init__(self, value):
        self.value = value
        self.connections = []
        self.distance_from_start = np.inf

class Con:
    def __init__(self, node, weight):
        self.node = node
        self.weight = weight
        #think of weight as the distance between the two nodes

def dijkstra(start, end):
    start.distance_from_start = 0
    visited = set([start])
    current = start
    #run while the new node is not the end
    while current != end:
        
        cur_dist = np.inf
        cur_v = None
        #for every node given...
        for node in visited:
            #look at the connections for each node, save if cur_dist is greater than new path
            #so it keeps the smallest paths
            for con in node.connections:
                if con.node in visited:
                    continue
                #add the current node distance to the weight of the connection
                #so it becomes total distance from the start to end
                if cur_dist > node.distance_from_start + con.weight:
                    cur_dist = node.distance_from_start + con.weight
                    cur_v = con.node
    
        current = cur_v
        current.distance_from_start = cur_dist
        visited.add(current)
    return current.distance_from_start

#write code that outputs the shortest path from start to end

if __name__ == '__main__':
    #how to use the code:
    #name = Node(value)
    #to connect two nodes, use:
    #name.connections.append(Con(other_node, weight))
    #i set all the weights to 1 so it's easier to test, output will be # nodes
    
    #0 -> 1 -> 2 -> 3 -> 5 = 4
    #0 -> 4 -> 5           = 2

    #not super sure but i think we have to tell them what distance from start is for each node
    #because i dont see how it is updated in the code other than setting to inf at creation

    start = Node(0)
    one = Node(1)
    two = Node(2)
    three = Node(3)
    four = Node(4)
    end = Node(5)
    
    start.connections.append(Con(one, 1))
    one.connections.append(Con(two, 1))
    two.connections.append(Con(three, 1))
    three.connections.append(Con(end, 1))
    start.connections.append(Con(four, 1))
    four.connections.append(Con(end, 1))
    
    res = dijkstra(start, end)
    print("The shortest path from an input to destination is: ", res)
    
    
    
    
                                                
