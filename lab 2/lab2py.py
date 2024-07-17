'''
Lab 2 Q4
Binary Search Deluxe
search and return lowest and highest index of a target in sorted list
Ologn because it's recursive
'''

def binary_search_deluxe(L, target, left, right, res):
    if left <= right:
        mid = (left + right) // 2

        #when it actually finds the target
        if target == L[mid]:
            i = mid
            for i in range(len(L)): #while loop broke because of index range 
                if L[i] == target:
                    res.append(i)
                    i += 1
            if len(res) > 1:
                ind = [res[0], res[-1]]
            else:
                ind = [res[0]]
            return ind

        if target > L[mid]:
            return binary_search_deluxe(L, target, mid + 1, right, res)
        else:
            return binary_search_deluxe(L, target, left, mid - 1, res)
        print("end")
    return left


if __name__ == "__main__":
    L = [1, 2, 3, 10, 10, 10, 12, 12]
    n = 12
    res = []
    res = binary_search_deluxe(L, n, 0, len(L) - 1, res)
    print(res)
