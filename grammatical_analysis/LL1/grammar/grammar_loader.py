from typing import Dict, List, Set
from collections import OrderedDict


class Grammar:
    def __init__(self, rules: Dict[str, List[List[str]]], non_terminals: OrderedDict, terminals: OrderedDict):
        self.rules = rules
        self.non_terminals = non_terminals
        self.terminals = terminals


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
                        if not symbol.isupper():
                            terminals[symbol] = None
                        else:
                            if symbol not in non_terminals:
                                non_terminals[symbol] = None
        return Grammar(rules, non_terminals, terminals)
