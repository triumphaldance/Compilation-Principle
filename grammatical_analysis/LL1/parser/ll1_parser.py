from grammar import Grammar
class LL1Parser:
    def __init__(self, parsing_table: dict, grammar: Grammar):
        self.parsing_table = parsing_table
        self.grammar = grammar

    def parse(self, input_string: str):
        input_string += '#'
        start_symbol = list(self.grammar.non_terminals.keys())[0]
        stack = ['#', start_symbol]
        index = 0
        step = 1

        print(f"请输入要分析的单词串:\n{input_string[:-1]}")
        print(f"{input_string[:-1]}的分析过程:")

        while stack:
            stack_top = stack[-1]
            current_input = input_string[index]

            if stack_top == current_input:
                stack_content = ''.join(stack)
                input_remaining = input_string[index:]
                print(f"{step} {stack_content:<10} {input_remaining:<10} 匹配成功{current_input}")
                stack.pop()
                index += 1
                step += 1
                if stack_top == '#':
                    print("分析成功!")
                    break
            elif stack_top in self.grammar.terminals:
                print("分析失败!")
                break
            else:
                if current_input == '#':
                    production = self.parsing_table.get(stack_top, {}).get('$')
                else:
                    production = self.parsing_table.get(stack_top, {}).get(current_input)
                if not production:
                    print("分析失败!")
                    break
                stack_content = ''.join(stack)
                input_remaining = input_string[index:]
                print(f"{step} {stack_content:<10} {input_remaining:<10} 用{stack_top}→{''.join(production)}, {current_input}逆序进栈")
                stack.pop()
                if production != ['ε']:
                    for symbol in reversed(production):
                        stack.append(symbol)

                step += 1