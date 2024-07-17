'''
question 4

change min help (retreiving lowest priority number)
to max heap (retreiving highest priority number)
'''

#plan: keep it sorted as min -> max and just pop the last element?

class Heap:
    def __init__(self):
        self.heap = [None]
        self.size = 0

    def get_left_child_index(self, parent_index):
        return 2 * parent_index
    def get_right_child_index(self, parent_index):
        return 2 * parent_index + 1
    def get_parent_index(self, child_index):
        return child_index // 2
    
    def swap(self, index1, index2):
        self.heap[index1], self.heap[index2] = self.heap[index2], self.heap[index1]

    def has_left_child(self, index):
        return self.get_left_child_index(index) <= self.size
    def has_right_child(self, index):
        return self.get_right_child_index(index) <= self.size
    def has_parent(self, index):
        return self.get_parent_index(index) >= 1
    

    def left_child(self, index):
        return self.heap[self.get_left_child_index(index)]
    def right_child(self, index):
        return self.heap[self.get_right_child_index(index)]
    def parent(self, index):
        return self.heap[self.get_parent_index(index)]


    def insert(self, value):
        self.heap.append(value)
        self.size += 1
        self.heapify_up()

    #made it < so that the maximum one goes to the top
    def heapify_up(self):
        index = self.size 
        while self.has_parent(index) and self.parent(index)[0] < self.heap[index][0]:
            self.swap(self.get_parent_index(index), index)
            index = self.get_parent_index(index)

    #right is bigger than left > then switch
    #if top is bigger than current index then just break
    #then keep looping until there are no more parents to check
    def heapify_down(self):
        index = 1
        while self.has_left_child(index):
            smaller_child_index = self.get_left_child_index(index)
            if self.has_right_child(index) and self.right_child(index)[0] > self.left_child(index)[0]:
                smaller_child_index = self.get_right_child_index(index)
            if self.heap[index][0] > self.heap[smaller_child_index][0]:
                break
            else:
                self.swap(index, smaller_child_index)
            index = smaller_child_index


    def extract_max(self):
        if self.size == 0:
            return None
        min = self.heap[1]
        self.heap[1] = self.heap[self.size]
        self.heap.pop()
        self.size -= 1
        self.heapify_down()
        return min[1]
    

if __name__ == '__main__':
    h = Heap()
    h.insert((5, "a"))
    h.insert((3, "b"))
    h.insert((4, "c"))
    h.insert((2, "d"))
    print(h.extract_max())
    h.insert((6, "e"))
    print(h.extract_max())
    print(h.extract_max())
    print(h.extract_max())
    print(h.extract_max())
    #should be a, e, c, b, d
    
    
    