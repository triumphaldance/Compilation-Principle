# automaton.py

from items import Item
from grammar_loader import Grammar
import graphviz

class Automaton:
    def __init__(self, grammar: Grammar, first_sets):
        self.grammar = grammar
        self.first_sets = first_sets
        self.states = []
        self.transitions = {}
        self._build_automaton()

    def _closure(self, items):
        closure_set = set(items)
        while True:
            new_items = set()
            for item in closure_set:
                next_symbol = item.next_symbol()
                if next_symbol and next_symbol in self.grammar.non_terminals:
                    predict_symbol = item.predict_symbol()
                    if predict_symbol:
                        if predict_symbol in self.grammar.non_terminals:
                            for production in self.grammar.rules[next_symbol]:
                                new_items.add(Item(next_symbol, production, self.first_sets[predict_symbol]))

                        else:
                            for production in self.grammar.rules[next_symbol]:
                                new_items.add(Item(next_symbol, production, predict_symbol))
                    else:
                        for production in self.grammar.rules[next_symbol]:
                            new_items.add(Item(next_symbol, production, item.predict))
            if new_items.issubset(closure_set):
                break
            closure_set.update(new_items)
        # 去重处理：合并predict集合
        unique_items = {}
        for item in closure_set:
            key = (item.lhs, tuple(item.rhs), item.dot_position)
            if key in unique_items:
                unique_items[key].predict.update(item.predict)
            else:
                unique_items[key] = item

        return set(unique_items.values())

    def _goto(self, state, symbol):
        goto_set = set()
        for item in state:
            if item.next_symbol() == symbol:
                goto_set.add(item.advance_dot())
        return self._closure(goto_set)

    def _build_automaton(self):
        initial_item = Item(self.grammar.augmented_start_symbol, [self.grammar.start_symbol], "$")
        initial_state = self._closure([initial_item])
        self.states.append(initial_state)

        queue = [initial_state]
        while queue:
            current_state = queue.pop(0)
            symbols = list(self.grammar.non_terminals) + list(self.grammar.terminals)
            for symbol in symbols:
                next_state = self._goto(current_state, symbol)
                if next_state:
                    if next_state not in self.states:
                        self.states.append(next_state)
                        queue.append(next_state)
                    self.transitions.setdefault((self.states.index(current_state), symbol), set()).add(self.states.index(next_state))

    def draw_automaton(self, filename='automaton'):
        dot = graphviz.Digraph(comment='LR(0) Automaton')

        # Add states with box shape
        for i, state in enumerate(self.states):
            state_label = f"State {i}\n"
            state_label += "\n".join(str(item) for item in state)
            dot.node(str(i), state_label, shape='box')

        # Add transitions with improved style
        for (from_state, symbol), to_states in self.transitions.items():
            for to_state in to_states:
                dot.edge(str(from_state), str(to_state), label=str(symbol), fontsize='10', color='blue')

        dot.attr(rankdir='LR')  # Left to right layout
        dot.render(filename, format='png', cleanup=True)


