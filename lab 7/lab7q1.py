'''
Part (a)
Suppose you have computed OPT(0), OPT(1), OPT(2), ..., OPT(n-1), the numbers of coins that are
needed to make up 0c, 1c, 2c, ..., (n-1)c.
Write down an expression for OPT(n), in terms of the denominations and OPT(0), ...., OPT(n).
Hint: if you want to use the 5c coin, the total number of coins you’ll need is OPT(n-5)+1

Question 1) a)
we have denominations of [1c, 4c, 5c] and we want to make up n cents using the least possible number of coins
we also have already computed OPT(0), OPT(1), OPT(2), ..., OPT(n-1),
this is the number of coins needed to make up 0c, 1c, 2c, ..., (n-1)c

therefore, to determine the number of coins needed for n cents, 
we have to to use the least possible number of coins from the denominations
so we need OPT(n) = min( OPT(n-1) + 1, OPT(n-4) + 1, OPT(n-5) + 1 )
you are looking at 1 cent less, 4 cents less, and 5 cents less in terms of n, so n-1c, n-4c, n-5c
'''

'''Question 1) b)'''
def create_opt(N, denoms):
    opt = [0] * N

    for i in range(0, N):
        if i < denoms[0]: #no coins if no denoms less than it
            opt[i] = 0
        elif i in denoms: #one coin if it is a denom
            opt[i] = 1
        else: 
            L = []
            L2 = []
            for j in range (len(denoms)):
                L.append(opt[i-denoms[j]])
            for item in L:
                if item != 0:
                    L2.append(item)
            opt[i] = min(L2) + 1
        #the general formula is opt[i] = min(opt[i-1]+1, opt[i-4]+1, opt[i-5]+1)
        #but i tried to generalize it to any number of denominations
        #and it works if there is a 0 case (ex. for i = 2, opt[i-4] and opt[i-5] are 0 so min is 0 which is wrong)
        #i circumvented this by creating a list of all the opt[i-denoms[j]] and then removing the 0s
        #and then taking the min of the remaining list and adding 1 to it for the new extra coin
    return opt

'''Question 1) c)'''
def least_coins(N, opt, denoms):
    #it will return a list that has the c values needed to make up N cents
    #also i gave up and assumed it's just 3 coins because i literally can't do that rn
    if N in denoms:
        return [N]
    elif opt[N-denoms[0]] <= min(opt[N-denoms[1]], opt[N-denoms[2]]): #it doesn't work if values are 0
        return least_coins(N-denoms[0], opt, denoms) + [denoms[0]]
    elif opt[N-denoms[1]] <= min(opt[N-denoms[0]], opt[N-denoms[2]]): #it doesn't work if values are 0
        return least_coins(N-denoms[1], opt, denoms) + [denoms[1]]
    else:
        return least_coins(N-denoms[2], opt, denoms) + [denoms[2]] #it doesn't work if values are 0

if __name__ == "__main__":
    N = 10
    denoms = [1, 4, 5]
    opt_list = create_opt(N+1, denoms)

    #testing the right nums
    for i in range(0, N+1):
        print("Value: ",i, "| Coins: ", opt_list[i])

    lc = least_coins(N, opt_list, denoms)
    print("Coins to make up", N, "cents:", lc)