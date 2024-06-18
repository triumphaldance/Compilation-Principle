class LR0Parser:
    def __init__(self, automaton, grammar, parser_table):
        self.grammar = grammar
        self.automaton = automaton
        self.parser_table = parser_table

    def parse(self, input_string: str):
        # 初始化栈和输入
        stack = ['$', 0]
        input_string += '$'
        index = 0
        step = 1

        print(f"{input_string[:-1]}的分析过程:")
        print(f"{'步骤':<5} {'分析栈':<20} {'输入':<10} {'动作'}")

        while True:
            state = stack[-1]
            token = input_string[index]
            action = self.parser_table.get_action(state, token)

            stack_content = ''.join(map(str, stack))
            input_remaining = input_string[index:]

            if action is None:
                print(f"{step:<5} {stack_content:<20} {input_remaining:<10} 分析失败!")
                break

            if action.startswith('s'):
                next_state = int(action[1:])
                print(f"{step:<5} {stack_content:<20} {input_remaining:<15} s{next_state}")
                stack.append(token)
                stack.append(next_state)
                index += 1

            elif action.startswith('r'):
                reduced = int(action[1:])
                key, production = self.grammar.get_production_by_index(reduced)
                for symble in reversed(production):
                    stack.pop()
                    stack.pop()
                print(f"{step:<5} {stack_content:<20} {input_remaining:<15} r{reduced}")
                state = stack[-1]
                token = key
                next_state = int(self.parser_table.get_goto(state, token))
                stack.append(key)
                stack.append(next_state)


            elif action == 'acc':
                print(f"{step:<5} {stack_content:<20} {input_remaining:<15} 分析成功!")
                break

            step += 1
