# SET: membership testing, duplicates
seen = set()
if x in seen:

# DICT: counting, grouping, mapping
count = {}
count[x] = count.get(x, 0) + 1

# SORT: custom ordering  
arr.sort(key=lambda x: (x[0], -x[1]))

# HEAP: always get min/max
import heapq
heapq.heappush(heap, item)

# LIST as MAP: when keys are indices
arr = [0] * n
arr[index] = value

# DEQUE: queue/stack operations
from collections import deque
dq.popleft()  # queue
dq.pop()      # stack