from grammar import Grammar
from sets.first_set_calculator import FirstSetCalculator
from sets.follow_set_calculator import FollowSetCalculator


class SelectSetCalculator:
    def __init__(self, grammar: Grammar, first_sets: dict, follow_sets: dict):
        self.grammar = grammar
        self.first_sets = first_sets
        self.follow_sets = follow_sets
        self.select_sets = {head: [] for head in grammar.rules}

    def calculate(self):
        for head, productions in self.grammar.rules.items():
            for production in productions:
                select_set = set()

                if len(production) == 1 and production[0] == 'ε':
                    select_set = self.follow_sets[head]
                else:
                    for symbol in production:
                        if symbol in self.grammar.terminals:
                            select_set.add(symbol)
                            break
                        else:
                            if 'ε' not in self.first_sets[symbol]:
                                select_set.update(self.first_sets[symbol])
                                break
                            else:
                                first_set = self.first_sets[symbol]
                                select_set.update(first_set - {'ε'})
                    else:
                        select_set.add('ε')
                        select_set.update(self.follow_sets[head])

                self.select_sets[head].append((production, select_set))

        return self.select_sets

    def is_ll1(self):
        """
        判断文法是否为LL(1)文法。
        如果每个非终结符的所有产生式的SELECT集合两两不相交，则是LL(1)文法。
        """
        for head, production_selects in self.select_sets.items():
            seen_selects = []
            for production, select_set in production_selects:
                for seen_set in seen_selects:
                    if not select_set.isdisjoint(seen_set):
                        return False
                seen_selects.append(select_set)
        return True