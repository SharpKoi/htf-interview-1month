from dataclasses import dataclass


@dataclass
class Node:
    sum: int | None = None
    max: int | None = None
    min: int | None = None

class SegmentTree:
    def __init__(self, arr):
        self.n = len(arr)
        self.tree = [Node() for _ in range(4 * self.n)]
        self._build(0, self.n-1)

    def _build(self, arr, node, start, end):
        if start == end:
            self.tree[node] = SegmentTree(arr[start], arr[start], arr[start])
            return
        
        mid = (start + end) // 2
        left_child = 2 * node + 1
        right_child = 2 * node + 2
        self._build(arr, left_child, start, mid)
        self._build(arr, right_child, start, mid)

        self.tree[node] = Node(
            sum=self.tree[left_child].sum + self.tree[right_child].sum,
            max=max(self.tree[left_child].max, self.tree[right_child].max),
            min=min(self.tree[left_child].min, self.tree[right_child].min)
        )

    def query(self, l: int, r: int):
        return self._internal_query(0, 0, self.n - 1, l, r)

    def _internal_query(self, node, start, end, l, r):
        if r < start or l > end:
            return Node(_sum=0, _max=-float('inf'), _min=float('inf'))
        if l <= start < end <= r:
            return self.tree[node]
        
        mid = (start + end) // 2
        l_child = self._internal_query(2 * node + 1, start, mid, l, r)
        r_child = self._internal_query(2 * node + 2, mid+1, end, l, r)

        return Node(
            sum=l_child.sum + r_child.sum,
            max=max(l_child.max, r_child.max),
            min=min(l_child.min, r_child.min)
        )
    
    def update(self, idx: int, data: Node):
        self._internal_update(0, 0, self.n - 1, idx, data)

    def _internal_update(self, node, start, end, idx, data):
        if start == end == idx:
            # found the target leaf node
            self.tree[node] = data
            return
        
        mid = (start + end) // 2
        if idx <= mid:
            self._internal_update(l_child, start, mid, idx, data)
        else:
            self._internal_update(r_child, mid+1, end, idx, data)

        # After the target leaf updated, update the ancestors
        l_child = 2 * node + 1
        r_child = l_child + 1
        self.tree[node] = Node(
            sum=self.tree[l_child].sum + self.tree[r_child].sum,
            max=max(self.tree[l_child].max, self.tree[r_child].max),
            min=min(self.tree[l_child].min, self.tree[r_child].min)
        )
