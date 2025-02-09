from collections import deque
from typing import Sequence

class BTNode:
    __preorder = []
    __inorder = []
    __postorder = []

    def __init__(self, data):
        self.data = data
        self.left: BTNode = None
        self.right: BTNode = None

    @classmethod
    def parse(cls, data_seq: Sequence):
        root = cls(data_seq[0])
        q = deque()
        q.append(root)

        idx = 0
        while idx < len(data_seq):
            head = q.popleft()
            if head is not None:
                l, r = idx+1, idx+2
                if r < len(data_seq):
                    ld, rd = data_seq[l], data_seq[r]
                    head.left = BTNode(ld) if ld is not None else None
                    head.right = BTNode(rd) if rd is not None else None
                    q.extend([head.left, head.right])
                    idx = r
                elif l < len(data_seq):
                    ld = data_seq[l]
                    head.left = BTNode(ld) if ld is not None else None
                    q.append(head.left)
                    idx = l
                else:
                    break
            else:
                q.extend([None, None])
                idx += 2

        return root

    def preorder(self):
        self.__preorder.append(self.data)

        if self.left:
            self.left.preorder()
        
        if self.right:
            self.right.preorder()

        return self.__preorder

    def inorder(self):
        if self.left:
            self.left.inorder()

        self.__inorder.append(self.data)

        if self.right:
            self.right.inorder()

        return self.__inorder

    def postorder(self):
        if self.left:
            self.left.postorder()

        if self.right:
            self.right.postorder()

        self.__postorder.append(self.data)

        return self.__postorder
