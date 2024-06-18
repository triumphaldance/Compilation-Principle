# items.py
class Item:
    def __init__(self, lhs, rhs, dot_position=0):
        self.lhs = lhs
        self.rhs = rhs
        self.dot_position = dot_position

    def __eq__(self, other):
        return (self.lhs == other.lhs and self.rhs == other.rhs and
                self.dot_position == other.dot_position)

    def __hash__(self):
        return hash((self.lhs, tuple(self.rhs), self.dot_position))

    def next_symbol(self):
        if self.dot_position < len(self.rhs):
            return self.rhs[self.dot_position]
        return None

    def advance_dot(self):
        return Item(self.lhs, self.rhs, self.dot_position + 1)

    def is_completed(self):
        return self.dot_position >= len(self.rhs)

    def __repr__(self):
        rhs_with_dot = self.rhs[:self.dot_position] + ['.'] + self.rhs[self.dot_position:]
        return f"{self.lhs} -> {' '.join(rhs_with_dot)}"
