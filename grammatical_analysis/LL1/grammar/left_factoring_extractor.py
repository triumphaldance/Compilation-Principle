from typing import List, Dict
from collections import defaultdict, OrderedDict
from .grammar_loader import Grammar


class LeftFactoringExtractor:
    def extract(self, grammar: Grammar) -> Grammar:
        def get_longest_common_prefix(strings):
            if not strings:
                return []
            prefix = strings[0]
            for s in strings[1:]:
                i = 0
                while i < len(prefix) and i < len(s) and prefix[i] == s[i]:
                    i += 1
                prefix = prefix[:i]
            return prefix

        def extract_left_factoring(non_terminal, productions):
            if not productions:
                return {non_terminal: productions}

            prefix_groups = defaultdict(list)
            for production in productions:
                prefix = tuple(production[:1])
                prefix_groups[prefix].append(production)

            new_rules = {}
            if len(prefix_groups) == len(productions):
                new_rules[non_terminal] = productions
            else:
                new_rules[non_terminal] = []
                for prefix, group in prefix_groups.items():
                    if len(group) > 1:
                        common_prefix = get_longest_common_prefix(group)
                        new_non_terminal = non_terminal + "'"
                        while new_non_terminal in grammar.rules or new_non_terminal in new_rules:
                            new_non_terminal += "'"
                        new_rules[non_terminal].append(list(common_prefix) + [new_non_terminal])
                        remaining_productions = [prod[len(common_prefix):] if len(prod) > len(common_prefix) else ['ε']
                                                 for prod in group]
                        new_rules.update(extract_left_factoring(new_non_terminal, remaining_productions))
                    else:
                        new_rules[non_terminal].append(group[0])
            return new_rules

        new_rules = {}
        for non_terminal, productions in grammar.rules.items():
            factored_rules = extract_left_factoring(non_terminal, productions)
            new_rules.update(factored_rules)

        # 创建一个新的 Grammar 对象，并使用提取出的规则、非终结符和终结符
        non_terminals = OrderedDict((non_terminal, None) for non_terminal in new_rules.keys())
        terminals = grammar.terminals

        # 更新终结符集合
        for production_list in new_rules.values():
            for production in production_list:
                for symbol in production:
                    if not symbol.isupper():
                        terminals[symbol] = None

        return Grammar(new_rules, non_terminals, terminals)
