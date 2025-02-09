import random

class Node:
    def __init__(self, key, level):
        self.key = key
        self.level = level
        self.next: list[Node] = [None] * (level + 1)

class SkipList:
    def __init__(self, max_level: int, p: float):
        self.p = p
        self.max_level = max_level
        self.header = Node(None, level=max_level)
        self.level = 0

    def insert(self, key):
        # update: 儲存每一層中該node應該被插入的節點位置
        update: list[Node] = [None] * self.max_level + 1
        
        # 由上往下，每一層去看這個key要被放在哪裡
        cursor = self.header
        for l in range(self.level, -1, -1):
            while cursor.next[l] and key > cursor.next[l].key:
                cursor = cursor.next[l]
            update[l] = cursor  # 記下當層到達的位置

        # 決定這個key的level
        key_lvl = self._random_level()
        if key_lvl > self.level:
            for l in range(self.level+1, key_lvl+1):
                update[l] = self.header  # 到key_lvl的每層從header開始接
            self.level = key_lvl  # update the list level

        # 到key_lvl的每層都插入新節點
        new_node = Node(key, key_lvl)
        for l in range(key_lvl+1):
            new_node.next[l] = update[l].next[l]
            update[l].next[l] = new_node

    def search(self, key):
        # 從上往下找
        cursor = self.header
        for l in range(self.level, -1, -1):
            while cursor.next[l] and cursor.next[l].key <= key:
                cursor = cursor.next[l]  # 往右找
            
                # 如果該cursor就是我們要找的key，就直接回傳
                if cursor.key == key:
                    return cursor
                
            # 如果cursor在該層沒了下個node，或是cursor的下個key太大了，就往下層找

        return None
    
    def remove(self, key):
        update: list[Node] = [None] * (self.level+1)

        cursor = self.header
        for l in range(self.level, -1, -1):
            while cursor.next[l] and cursor.next[l].key < key:
                cursor = cursor.next[l]

            update[l] = cursor

        target = cursor.next[0]
        if target and target.key == key:
            for l in range(target.level):
                if update[l].next[l].key != target.key:
                    break

                update[l].next[l] = target.next[l]

            while self.level > 0 and not self.header.next[self.level]:
                self.level -= 1

            return True
        
        return False

    def _random_level(self):
        lvl = 0
        while random.random() <= self.p and lvl < self.max_level:
            lvl += 1
        
        return lvl
