from tabulate import tabulate


class ParserTable:
    def __init__(self, automaton, grammar, follow_sets):
        self.automaton = automaton
        self.grammar = grammar
        self.follow_sets = follow_sets
        self.action_table = {}
        self.goto_table = {}
        self.build_parser_table()

    def build_parser_table(self):
        for i, state in enumerate(self.automaton.states):
            for item in state:
                if item.is_completed():
                    if item.lhs == self.grammar.augmented_start_symbol:
                        self.action_table[(i, '$')] = 'acc'
                    else:
                        for symbol in self.follow_sets.get(item.lhs):
                            index = self.grammar.get_rule_index(item.lhs, item.rhs)
                            self.action_table[(i, symbol)] = f'r{index}'

                else:
                    next_symbol = item.next_symbol()
                    if next_symbol:
                        next_state = self.automaton.transitions.get((i, next_symbol))
                        if next_state is not None:
                            if next_symbol in self.grammar.get_non_terminals():
                                self.goto_table[(i, next_symbol)] = ''.join(map(str, next_state))
                            else:
                                self.action_table[(i, next_symbol)] = 's' + ''.join(map(str, next_state))

    def get_action(self, state, symbol):
        return self.action_table.get((state, symbol))

    def get_goto(self, state, symbol):
        return self.goto_table.get((state, symbol))

    def draw_parser_table(self):
        # 获取终结符和非终结符
        terminals = list(self.grammar.get_terminals())
        non_terminals = list(self.grammar.get_non_terminals())
        filtered_non_terminals = list({t for t in non_terminals if t != self.grammar.augmented_start_symbol})
        # 打印表头
        header = ['State'] + terminals + ['$'] + filtered_non_terminals

        # 打印表格内容
        num_states = len(self.automaton.states)
        table = []
        for i in range(num_states):
            row = [str(i)]
            for terminal in terminals + ['$']:
                action = self.action_table.get((i, terminal))
                row.append(action if action else '')
            for non_terminal in filtered_non_terminals:
                goto = self.goto_table.get((i, non_terminal))
                row.append(str(goto) if goto is not None else '')
            table.append(row)

        print(tabulate(table, headers=header, tablefmt="grid"))

