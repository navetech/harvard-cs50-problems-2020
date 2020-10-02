# Check if a list of integer is balanceable

from itertools import permutations


def balanceable(numbers):

    # If the sum of list elements is odd, is not balanceable
    total = sum(numbers)
    if total % 2 != 0:
        return False

    # Sort the list
    numbers.sort()

    # For each sublist from the 0 to 1 smallers elements
    for i in range(1):

        # Sum the elements of the smaller sublist to the biggest element of the entire list
        total1 = sum(numbers[0:i], numbers[len(numbers) - 1])

        # Sum the remaning elements of the list
        total2 = sum(numbers[i:(len(numbers) - 1)])

        # If the totals are equal, is balanceable
        # If the first total is greater than the second one, is not balanceable
        if total1 == total2:
            return True
        elif total1 > total2:
            return False

    # For each permutation of list elements
    for permutation in permutations(numbers):

        # For each sublist from the 1 to len(list) elements on the left
        for i in range(1, len(permutation)):

            # Sum the elements of the left sublist
            total1 = sum(permutation[0:i])

            # Sum the remaning elements of the list
            total2 = sum(permutation[i:len(permutation)])

            # If the totals are equal, is balanceable
            if total1 == total2:
                return True

    # All permutations checked: is not balanceable
    return False
