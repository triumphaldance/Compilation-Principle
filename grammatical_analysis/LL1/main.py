from grammar import GrammarLoader, LeftRecursionEliminator, LeftFactoringExtractor
from sets import FirstSetCalculator, FollowSetCalculator, SelectSetCalculator
from parser import ParsingTableBuilder, LL1Parser


def main():
    # 读取文法规则
    grammar_loader = GrammarLoader()
    grammar = grammar_loader.load("grammar.txt")


    # 消除左递归
    lr_eliminator = LeftRecursionEliminator()
    grammar = lr_eliminator.eliminate(grammar)


    # 提取左因子
    lf_extractor = LeftFactoringExtractor()
    grammar = lf_extractor.extract(grammar)

    # 输出处理后的文法
    print('文法：')
    for non_terminal, productions in grammar.rules.items():
        print(f"{non_terminal} -> {' | '.join([' '.join(prod) for prod in productions])}")

    # 输出最终的终结符集和非终结符集
    filtered_terminals = {t for t in grammar.terminals if t != 'ε'}
    print(f"\n非终结符：{{ {', '.join(grammar.non_terminals)} }}")
    print(f"终结符集：{{ {', '.join(filtered_terminals)} }}")

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

    # 输出select集
    select_calculator = SelectSetCalculator(grammar, first_sets, follow_sets)
    select_sets = select_calculator.calculate()
    print("\nSELECT sets:")
    for head, productions in select_sets.items():
        for production, select_set in productions:
            production_str = ' '.join(production) if production else 'ε'
            print(f"SELECT({head} -> {production_str}) = {{ {', '.join(select_set)} }}")

    if select_calculator.is_ll1():
        print("该文法是LL(1)文法")
        parsing_table_builder = ParsingTableBuilder(grammar, first_sets, follow_sets, select_sets)
        parsing_table = parsing_table_builder.build()
        parsing_table_builder.print_parsing_table(parsing_table)

        # 创建LL1解析器
        parser = LL1Parser(parsing_table, grammar)

        # 输入字符串并进行分析
        input_string = input('请输入要分析的字符串:')
        # input_string = "i+i*i"
        parser.parse(input_string)
    else:
        print("该文法不是LL(1)文法")


if __name__ == "__main__":
    main()
