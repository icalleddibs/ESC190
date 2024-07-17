# Question 1: Makr a circular queue class that uses a list to store the data.

class CircularQueue:
    def __init__(self, size):
        L = [None] * size
        self.data = L
        self.size = size
        self.begin = 0
        self.end = 0

    def enqueue(self, item):
        if ((self.end == (self.size - 1))): #so when it's at the end
            (self.data)[0] = item
            self.end = 0
        elif ((self.data[0] == None) and (self.end == 0)):
            (self.data)[0] = item
        else:
            self.end += 1
            (self.data)[self.end] = item

    def dequeue(self):
        if (self.begin == (self.size - 1)):
            self.data[self.begin] = None
            self.begin = 0
        else:
            self.data[self.begin] = None
            self.begin += 1
    
    def __str__(self):
        return str(self.data)
    
    def __lt__(self, other): #assuming they are the same length and not None
        num = len(other.data)
        for i in range(num):
            if (self.data[i] == None):
                return True
            elif (other.data[i] == None):
                return False
            elif (self.data[i] < other.data[i]): #if less, true sort below
                return True
            elif (self.data[i] == other.data[i]): #else, repeat next loop with new i
                i += 1
                continue
            else: #after all loops, if not less than, then greater than
                return False
    
def compare(cq_data):
    num_cq = len(cq_data)
    # make a new list that starts at begin and ends at end for comparing
    new_data = []
    for i in range(num_cq):
        if (cq_data[i].begin < cq_data[i].end):
            each_list = cq_data[i].data[cq_data[i].begin:]
        elif (cq_data[i].begin == cq_data[i].end):
            if (cq_data[i].data[cq_data[i].begin] == None):
                each_list = []
            else:
                each_list = cq_data[i].data[cq_data[i].begin:cq_data[i].end+1]
        else:
            each_list = cq_data[i].data[cq_data[i].begin:]+ cq_data[i].data[cq_data[i].data[0]:cq_data[i].end + 1]
        
        if (each_list != []):
            new_data.append(each_list)
    new_data.sort() #uses the __lt__ we defined
    return new_data
    
if __name__ == "__main__":



    '''
    Queue testing code: (Q1)
    q = CircularQueue(6)
    q.enqueue(1)
    print(q)
    q.enqueue(1)
    print(q)
    q.enqueue(1)
    print(q)
    q.dequeue()
    print(q)
    q.enqueue(1)
    print(q)
    q.enqueue(1)
    print(q)
    q.enqueue(2)
    print(q)
    q.enqueue(321564)
    print(q)
    q.dequeue()
    print(q)
    q.dequeue()
    print(q)
    q.dequeue()
    print(q)
    q.enqueue(0)
    print(q)
    q.dequeue()
    print(q)
    q.dequeue()
    q.dequeue()
    print(q)
    q.enqueue(5)
    print(q)
    q.dequeue()
    print(q)
    q.dequeue()
    print(q)
    q.enqueue(6)
    print(q)
    '''

    '''
    #Queue Sort testing code: (Q2)
    q1 = CircularQueue(4)
    q2 = CircularQueue(4)
    q3 = CircularQueue(4)
    #comment out to test for fully None queue
    # q1.enqueue(2)
    # q1.enqueue(2)
    # q1.enqueue(1)
    # q1.enqueue(1)
    # q1.dequeue()
    # q1.dequeue()
    # q1.enqueue(0)
    # q1.enqueue(5)

    q2.enqueue(2)
    q2.enqueue(2)
    q2.enqueue(2)
    q2.enqueue(1)

    q3.enqueue(2)
    q3.enqueue(1)
    q3.enqueue(1)
    q3.enqueue(1)

    cq_data = [q1, q2, q3] #have to access like cq_data[0].data is [2, 2, 1]

    res = compare(cq_data)
    for i in range(len(res)):
        print(res[i])
    '''