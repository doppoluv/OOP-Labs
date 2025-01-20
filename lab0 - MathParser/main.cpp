#include "src/MathParser.cpp"

int main()
{
    std::string text{"((5+3) /4) * (0 + 2)"};
    try
    {
        Parser parser(text);
        Tree tree = parser.parse();
        tree.printTree();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}