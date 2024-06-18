from grammar_loader import GrammarLoader
from automaton import Automaton
from parser_table import ParserTable
from lr0_parser import LR0Parser
from first_set_calculator import FirstSetCalculator
from follow_set_calculator import FollowSetCalculator
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

    # 输出follow集
    follow_calculator = FollowSetCalculator(grammar, first_sets)
    follow_sets = follow_calculator.calculate()
    print("\nFOLLOW sets:")
    for non_terminal, follow_set in follow_sets.items():
        print(f"FOLLOW({non_terminal}) = {{ {', '.join(follow_set)} }}")

    # 创建自动机
    automaton = Automaton(grammar)
    automaton.draw_automaton('png/lr0_automaton')

    # 创建解析表
    parser_table = ParserTable(automaton, grammar, follow_sets)
    parser_table.draw_parser_table()
    # 解析
    lr0_parser = LR0Parser(automaton, grammar, parser_table)
    input_string = input("请输入要分析的单词串: ")
    lr0_parser.parse(input_string)


if __name__ == "__main__":
    main()
