# sets/follow_set_calculator.py

from grammar.grammar_loader import Grammar


class FollowSetCalculator:
    def __init__(self, grammar: Grammar, first_sets: dict):
        self.grammar = grammar
        self.first_sets = first_sets
        self.follow_sets = {non_terminal: set() for non_terminal in grammar.non_terminals}
        start_symbol = next(iter(grammar.non_terminals))
        self.follow_sets[start_symbol].add('$')  # Assuming the first non-terminal is the start symbol

    def calculate(self):
        while True:
            updated = False
            for non_terminal in self.grammar.non_terminals:
                follow_temp = self.follow_sets[non_terminal].copy()
                for head, productions in self.grammar.rules.items():
                    for production in productions:
                        if non_terminal in production:
                            indices = [i for i, symbol in enumerate(production) if symbol == non_terminal]
                            for i in indices:
                                if i == len(production)-1:
                                    follow_temp.update(self.follow_sets[head])
                                else:
                                    for j in range(i + 1, len(production)):
                                        next_symbol = production[j]
                                        if next_symbol in self.grammar.terminals:
                                            # Case 2: A -> αBc
                                            follow_temp.add(next_symbol)
                                            break
                                        else:
                                            # Case 3: A -> αBC
                                            first_of_next = self.first_sets[next_symbol] - {'ε'}
                                            follow_temp.update(first_of_next)
                                            if 'ε' not in self.first_sets[next_symbol]:
                                                continue
                                            if j == len(production) - 1:
                                                # If we reach the end and all intermediate symbols can be ε
                                                follow_temp.update(self.follow_sets[head])
                        else:
                            continue
                if not self.follow_sets[non_terminal].issuperset(follow_temp):
                    self.follow_sets[non_terminal].update(follow_temp)
                    updated = True
            if not updated:
                break
        return self.follow_sets
