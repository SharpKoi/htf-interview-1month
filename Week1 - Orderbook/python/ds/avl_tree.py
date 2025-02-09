class AVLNode:
    def __init__(self, data):
        self.data = data

        self.ancestor: AVLNode = None
        self.left: AVLNode = None
        self.right: AVLNode = None
        
        self.height = 0

    def __repr__(self):
        lv = self.left.data if self.left else ""
        rv = self.right.data if self.right else ""
        return f"AVLNode(v={self.data}, l={lv}, r={rv}, h={self.height})"

    def inorder(self, root, outcome: list = []):
        if root is None:
            return
        
        self.inorder(root.left, outcome)
        outcome.append(root.data)
        self.inorder(root.right, outcome)

    def get_balance(self):
        lh = self.left.height + 1 if self.left else 0
        rh = self.right.height + 1 if self.right else 0

        return rh - lh

    def insert(self, data):
        if data < self.data:
            if self.left:
                self.left = self.left.insert(data)
            else:
                self.left = AVLNode(data)
                self.height += 1
                return self
        elif data > self.data:
            if self.right:
                self.right = self.right.insert(data)
            else:
                self.right = AVLNode(data)
                self.height += 1
                return self
        else:
            return self

        self._update_height()

        return self._rebalance()

    def remove(self, data):
        if data < self.data and self.left:
            self.left = self.left.remove(data)
        elif data > self.data and self.right:
            self.right = self.right.remove(data)
        else:
            if not self.left:
                return self.right
            elif not self.right:
                return self.left
            
            # both are not null
            right_min = self.right._find_min()
            self.data = right_min.data
            self.right = self.right.remove(right_min.data)

        self._update_height()

        return self._rebalance()

    def _find_min(self):
        cursor = self
        while cursor.left:
            cursor = cursor.left

        return cursor

    def _rebalance(self):
        balance = self.get_balance()
        if balance < -1 and self.left.get_balance() < 0:
            # LL
            return self._rotate_right()
        elif balance < -1 and self.left.get_balance() > 0:
            # LR
            self.left = self.left._rotate_left()
            return self._rotate_right()
        elif balance > 1 and self.right.get_balance() > 0:
            # RR
            return self._rotate_left()
        elif balance > 1 and self.right.get_balance() < 0:
            # RL
            self.right = self.right._rotate_right()
            return self._rotate_left()

        return self

    def _rotate_left(self):
        new_root = self.right
        rl = new_root.left
        new_root.left = self
        self.right = rl

        self._update_height()
        new_root._update_height()

        return new_root

    def _rotate_right(self):
        new_root = self.left
        lr = new_root.right
        new_root.right = self
        self.left = lr

        self._update_height()
        new_root._update_height()

        return new_root

    def _update_height(self):
        lh = self.left.height if self.left else -1
        rh = self.right.height if self.right else -1

        self.height = 1 + max(lh, rh)


