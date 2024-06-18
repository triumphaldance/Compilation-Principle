from typing import Dict, List
from collections import OrderedDict


class Grammar:
    def __init__(self, rules: OrderedDict[str, List[List[str]]], non_terminals: OrderedDict, terminals: OrderedDict):
        self.rules = rules
        self.non_terminals = non_terminals
        self.terminals = terminals
        self.start_symbol = list(non_terminals.keys())[0]
        self.augmented_start_symbol = self.start_symbol + "'"
        self._augment_grammar()

    def get_non_terminals(self):
        return self.non_terminals

    def get_terminals(self):
        return self.terminals
    def _augment_grammar(self):
        if self.augmented_start_symbol not in self.non_terminals:
            new_non_terminals = OrderedDict([(self.augmented_start_symbol, None)])
            new_non_terminals.update(self.non_terminals)
            self.non_terminals = new_non_terminals
            new_rules = OrderedDict([(self.augmented_start_symbol, [[self.start_symbol]])])
            new_rules.update(self.rules)
            self.rules = new_rules

    def get_rule_index(self, lhs, rhs) -> int:
        index = 0
        for key, productions in self.rules.items():
            for production in productions:
                if key == lhs and production == rhs:
                    return index
                index += 1
        return -1

    def get_production_by_index(self, index):
        current_index = 0
        for key, productions in self.rules.items():
            for production in productions:
                if current_index == index:
                    return key, production
                current_index += 1
        return None


class GrammarLoader:
    def load(self, filename: str) -> Grammar:
        rules = {}
        non_terminals = OrderedDict()
        terminals = OrderedDict()

        with open(filename, 'r', encoding='utf8') as file:
            for line in file:
                line = line.strip()
                if not line or '->' not in line:
                    continue

                head, productions = line.split('->')
                head = head.strip()

                if head not in non_terminals:
                    non_terminals[head] = None

                rules.setdefault(head, [])

                for production in productions.split('|'):
                    production = production.strip().split()
                    rules[head].append(production)

                    for symbol in production:
                        if symbol.isupper():
                            if symbol not in non_terminals:
                                non_terminals[symbol] = None
                        else:
                            if symbol not in terminals:
                                terminals[symbol] = None

        return Grammar(rules, non_terminals, terminals)
