from .grammar_loader import Grammar
from collections import OrderedDict


class LeftRecursionEliminator:
    def eliminate(self, grammar: Grammar) -> Grammar:
        new_rules = {}
        new_non_terminals = OrderedDict()
        new_terminals = OrderedDict((terminal, None) for terminal in grammar.terminals)

        for non_terminal in grammar.rules:
            alpha_rules = []
            beta_rules = []
            for production in grammar.rules[non_terminal]:
                if production[0] == non_terminal:
                    alpha_rules.append(production[1:])
                else:
                    beta_rules.append(production)

            if alpha_rules:
                new_non_terminal = non_terminal + "'"
                new_non_terminals[non_terminal] = None
                while new_non_terminal in grammar.rules or new_non_terminal in new_rules:
                    new_non_terminal += "'"
                new_non_terminals[new_non_terminal] = None
                new_rules[non_terminal] = [beta + [new_non_terminal] for beta in beta_rules]
                new_rules[new_non_terminal] = [alpha + [new_non_terminal] for alpha in alpha_rules] + [['ε']]
            else:
                new_non_terminals[non_terminal] = None
                new_rules[non_terminal] = grammar.rules[non_terminal]
        return Grammar(new_rules, new_non_terminals, new_terminals)
