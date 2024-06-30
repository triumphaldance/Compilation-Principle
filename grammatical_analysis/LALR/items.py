# items.py
class Item:
    def __init__(self, lhs, rhs, predict, dot_position=0):
        self.lhs = lhs
        self.rhs = rhs
        self.predict = set(predict) if isinstance(predict, (set, list)) else {predict}
        self.dot_position = dot_position

    def __eq__(self, other):
        return (self.lhs == other.lhs and self.rhs == other.rhs and self.predict == other.predict and
                self.dot_position == other.dot_position)

    def __hash__(self):
        return hash((self.lhs, tuple(self.rhs), tuple(self.predict), self.dot_position))

    def next_symbol(self):
        if self.dot_position < len(self.rhs):
            return self.rhs[self.dot_position]
        return None

    def predict_symbol(self):
        if self.dot_position < len(self.rhs) - 1:
            return self.rhs[self.dot_position + 1]
        return None

    def advance_dot(self):
        return Item(self.lhs, self.rhs, self.predict, self.dot_position + 1)

    def is_completed(self):
        return self.dot_position >= len(self.rhs)

    def __repr__(self):
        rhs_with_dot = self.rhs[:self.dot_position] + ['.'] + self.rhs[self.dot_position:]
        return f"{self.lhs} -> {' '.join(rhs_with_dot)} , {'/'.join(self.predict)}"

    def merge_predict(self, other):
        self.predict.update(other.predict)
