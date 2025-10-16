from collections import OrderedDict, Counter

a = set()
a.add("A")
a.add("C")
a.add("D")
a.add("B")
a.add("A")
a.remove("C")
print(a)

# ordered_dict = OrderedDict([("A", 1), ("B", 2)])
ordered_dict = OrderedDict({"A": 1, "B": 2, "C": 3})
ordered_dict.move_to_end("A")
ordered_dict.popitem(last=False)
print(ordered_dict)

unordered_dict = {"A": 1, "B": 2, "C": 3}
to_count = ["B", "A", "B", "D"]
for letter in to_count:
    unordered_dict[letter] = unordered_dict.get(letter, 0) + 1
print(unordered_dict)

# counter
counts = Counter(to_count)
counts["E"] = 3
counts["D"] += 1
print(counts)

# sorting
data = [(2, 4), (1, 3), (1, 5)]
data.sort(key=lambda x: (x[0], -x[1]))
print(data)

employees = [
    ("HR", "Alice"), ("HR", "Bob"), 
    ("IT", "Charlie"), ("IT", "Alice")
]
employees.sort(key=lambda x: (x[0], [-ord(c) for c in x[1]]))
print(employees)

# priority queues and lists
# (particularly in a context of direct indexing, as a much more efficient replacement of the map/dictionary, if possible).

import heapq

# k-th largest - fastest to write and runs efficiently
def kth_largest(arr, k):
    return heapq.nlargest(k, arr)[-1]  # One line solution!

# Top k frequent elements
def top_k_frequent(nums, k):
    from collections import Counter
    freq = Counter(nums)
    return heapq.nlargest(k, freq.keys(), key=freq.get)

# k-th smallest  
def kth_smallest(arr, k):
    return heapq.nsmallest(k, arr)[-1]



# Instead of dictionary for 0...n-1 keys
n = 100
positions = [None] * n  # O(1) access vs O(1) dict
positions[5] = value

# Character frequency
freq = [0] * 26
for char in string:
    freq[ord(char) - ord('a')] += 1

# Boolean visited array
visited = [False] * n
if not visited[i]:
    visited[i] = True




from collections import deque

# Queue (FIFO)
queue = deque()
queue.append(1)  # enqueue
queue.append(2)
front = queue.popleft()  # dequeue → 1

# Stack (LIFO)
stack = deque()
stack.append(1)  # push
stack.append(2)
top = stack.pop()  # pop → 2

# Useful for sliding window
dq = deque()
for i in range(n):
    # maintain decreasing order for max in window
    while dq and arr[dq[-1]] < arr[i]:
        dq.pop()
    dq.append(i)
