from grammar_loader import GrammarLoader
from automaton import Automaton
from parser_table import ParserTable
from lr0_parser import LR0Parser
from first_set_calculator import FirstSetCalculator

def main():
    loader = GrammarLoader()
    grammar = loader.load("grammar.txt")

    filtered_terminals = {t for t in grammar.terminals if t != 'ε'}
    print(f"\n非终结符：{{ {', '.join(grammar.non_terminals)} }}")
    print(f"终结符集：{{ {', '.join(filtered_terminals)} }}")

    # 输出处理后的文法
    print('文法：')
    for non_terminal, productions in grammar.rules.items():
        print(f"{non_terminal} -> {' | '.join([' '.join(prod) for prod in productions])}")

    # 输出first集
    first_calculator = FirstSetCalculator(grammar)
    first_sets = first_calculator.calculate()
    print("\nFIRST sets:")
    for non_terminal, first_set in first_sets.items():
        print(f"FIRST({non_terminal}) = {{ {', '.join(first_set)} }}")

    # 生成自动机
    automaton = Automaton(grammar, first_sets)
    automaton.draw_automaton('png/lr0_automaton')

    print("States:")
    for i, state in enumerate(automaton.states):
        print(f"State {i}:")
        for item in state:
            print(f"  {item}")

    print("\nTransitions:")
    for (state, symbol), next_state in automaton.transitions.items():
        print(f"  State {state} -- {symbol} --> State {next_state}")

    # 创建解析表
    parser_table = ParserTable(automaton, grammar)
    parser_table.draw_parser_table()
    # 解析
    lr0_parser = LR0Parser(automaton, grammar, parser_table)
    input_string = input("请输入要分析的单词串: ")
    lr0_parser.parse(input_string)


if __name__ == "__main__":
    main()
