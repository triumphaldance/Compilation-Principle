from grammar_loader import Grammar


class FirstSetCalculator:
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.first_sets = {non_terminal: set() for non_terminal in grammar.non_terminals}

    def calculate(self):
        while True:
            updated = False
            for non_terminal in self.grammar.non_terminals:
                first_temp = self.first_sets[non_terminal].copy()
                for production in self.grammar.rules[non_terminal]:
                    for i, signal in enumerate(production):
                        if signal in self.grammar.terminals:
                            first_temp.add(signal)
                            break
                        else:
                            first_temp.update(self.first_sets[signal] - {'ε'})
                            if 'ε' not in self.first_sets[signal]:
                                break
                            if i == len(production)-i:
                                first_temp.add('ε')

                if not self.first_sets[non_terminal].issuperset(first_temp):
                    self.first_sets[non_terminal].update(first_temp)
                    updated = True
            if not updated:
                break
        return self.first_sets
