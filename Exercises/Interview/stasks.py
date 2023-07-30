
# 2 List Intersection

# Easiest way is to hash one lists into a sets, then loop over the other list and pick only numbers that are not in the set

# Worst-case time complexity is O(nmk)
# where n is size of the list 1, m size of the list and k is length of elements in a list


def list_intersection(lst1, lst2):
    # make the first variable to be the longest list
    if (len(lst2) > len(lst1)):
        lst1, lst2 = lst2, lst1
    set1 = set(lst1)
    intersection = filter(lambda num: num in set1, lst2)
    return list(intersection)


def list_intersection(lst1, lst2):
	set1 = set(lst1)
	set2 = set(lst2)
	return list(set1.intersection(set2))
