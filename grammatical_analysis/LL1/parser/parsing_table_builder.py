from typing import Dict
from grammar import Grammar
from tabulate import tabulate

class ParsingTableBuilder:
    def __init__(self, grammar: Grammar, first_sets: dict, follow_sets: dict, select_sets: dict):
        self.grammar = grammar
        self.first_sets = first_sets
        self.follow_sets = follow_sets
        self.select_sets = select_sets

    def build(self) -> Dict[str, Dict[str, str]]:
        parsing_table = {non_terminal: {} for non_terminal in self.grammar.non_terminals}

        for non_terminal, production_selects in self.select_sets.items():
            for production, select_set in production_selects:
                for terminal in select_set:
                    if terminal in parsing_table[non_terminal]:
                        raise ValueError(f"Conflict in parsing table at {non_terminal}, {terminal}")
                    parsing_table[non_terminal][terminal] = production

        return parsing_table

    def print_parsing_table(self, parsing_table: dict):
        # Collect all terminals
        terminals = set()
        terminals.add('$')
        for terminal in self.grammar.terminals:
            terminals.update(terminal)
        terminals.remove('ε')
        terminals = sorted(terminals)
        # Prepare table data
        table_data = []
        header = [""] + terminals
        for non_terminal, row in parsing_table.items():
            row_data = [non_terminal]
            for terminal in terminals:
                production = row.get(terminal, "")
                row_data.append(" ".join(production))
            table_data.append(row_data)

        # Print table using tabulate
        print(tabulate(table_data, headers=header, tablefmt="grid"))

