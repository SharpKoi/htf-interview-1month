

class MinHeap:
    """每個節點都比其所有子節點還小。"""
    def __init__(self, a: list):
        self._arr = a

    def pop(self):
        root = self._arr[0]
        self._arr[0] = self._arr[-1]
        self._arr = self._arr[:-1]
        self._sink()

        return root

    def insert(self, val):
        self._arr.append(val)
        self._swim()

    def _swim(self):
        i = len(self._arr) - 1
        while (j := (i - 1) // 2) >= 0:
            if self._arr[i] >= self._arr[j]:
                break

            self._swap(i, j)

            i = j

    def _sink(self):
        i = 0
        while (j := 2*i+1) < len(self._arr):
            if j+1 < len(self._arr) and self._arr[j+1] < self._arr[j]:
                j = j + 1

            if self._arr[j] >= self._arr[i]:
                break
            
            self._swap(i, j)
            i = j

    def _swap(self, i, j):
        tmp = self._arr[i]
        self._arr[i] = self._arr[j]
        self._arr[j] = tmp
