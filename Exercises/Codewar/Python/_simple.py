def countBits(n):
    ret = 0
    while n > 0:
        ret += n & 1
        n >>= 1
    return ret

def sum_two_smallest_numbers(numbers):
    min1 = min(numbers)
    numbers.remove(min1);
    return min1 + min(numbers)


from random import randint,setstate,getstate
state = getstate()
guess = randint(1,100)
setstate(state)
