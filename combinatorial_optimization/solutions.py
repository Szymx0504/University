import heapq
from collections import Counter
n = int(input())
counts = Counter()
queue = []
for item in input().split():
    num = int(item)
    if num > 0:
        heapq.heappush(queue, num)
        counts[num] += 1
    else:
        if counts[-num] > 0:
            counts[-num] -= 1
    while queue and counts[queue[0]] == 0:
        heapq.heappop(queue)
    print(queue[0] if queue else 0, end=" ")