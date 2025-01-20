#include <iostream>
#include <string>
#include <stdexcept>

bool isValue(char sym)
{
    const char *digits = "0123456789";
    for (int i = 0; i < 10; i++)
    {
        if (sym == digits[i])
        {
            return true;
        }
    }

    return false;
}

namespace
{
    class Node
    {
    public:
        Node() = default;
        virtual ~Node() = default;

        Node *lnode = nullptr;
        Node *rnode = nullptr;
        Node(Node *lnode, Node *rnode) : lnode(lnode), rnode(rnode) {}
        virtual float calculate() const = 0;
        virtual void print() const = 0;
    };

    class Op : public Node
    {
    public:
        explicit Op(char op, Node *lnode = nullptr, Node *rnode = nullptr)
            : Node(lnode, rnode), op(op) {}

        float calculate() const override
        {
            float lvalue = lnode ? lnode->calculate() : 0;
            float rvalue = rnode ? rnode->calculate() : 0;

            switch (op)
            {
            case '+':
                return lvalue + rvalue;
            case '-':
                return lvalue - rvalue;
            case '*':
                return lvalue * rvalue;
            case '/':
                if (rvalue == 0)
                    throw std::runtime_error("Division by zero");
                return lvalue / rvalue;
            default:
                throw std::runtime_error("Unknown operator");
            }
        }

        void print() const override
        {
            std::cout << "Op(" << op << ")" << std::endl;
        }

        char getOp() const
        {
            return op;
        }

    private:
        char op;
    };

    class Value : public Node
    {
    public:
        Value(char val) : val((int)val - (int)'0') {}
        Value(int val) : val(val) {}

        float calculate() const override
        {
            return val;
        }

        void print() const override
        {
            std::cout << "Value(" << val << ")" << std::endl;
        }

        int getVal() const
        {
            return val;
        }

    private:
        int val;
    };

    class Tree
    {
    public:
        Tree(Node *root = nullptr) : root(root) {}

        void printTree() const
        {
            if (root == nullptr)
            {
                std::cout << "Tree is empty." << std::endl;
                return;
            }

            std::cout << "Tree structure:" << std::endl;
            print(root);

            try
            {
                float result = root->calculate();
                std::cout << "Calculation result: " << result << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

    private:
        Node *root;

        void print(const Node *node, int indent = 0) const
        {
            if (!node)
            {
                return;
            }

            for (int i = 0; i < indent; ++i)
            {
                std::cout << "  ";
            }
            node->print();
            print(node->lnode, indent + 1);
            print(node->rnode, indent + 1);
        }
    };

    class Parser
    {
    public:
        Parser(const std::string &text) : text(text) {}

        Tree parse()
        {
            int pos = 0;
            Node *root = S(pos);
            skipSpaces(pos);
            if (pos < text.length())
            {
                throw std::runtime_error("Unexpected characters at the end of input");
            }
            return Tree(root);
        }

    private:
        std::string text;

        void skipSpaces(int &pos) const
        {
            while (pos < text.length() && text[pos] == ' ')
                pos++;
        }

        Node *S(int &pos)
        {
            skipSpaces(pos);
            Node *lnode = M(pos);
            skipSpaces(pos);

            while (pos < text.length() && (text[pos] == '+' || text[pos] == '-'))
            {
                char op = text[pos];
                pos++;
                skipSpaces(pos);
                Node *rnode = M(pos);
                skipSpaces(pos);
                lnode = new Op(op, lnode, rnode);
            }

            return lnode;
        }

        Node *M(int &pos)
        {
            skipSpaces(pos);
            Node *lnode = P(pos);
            skipSpaces(pos);

            while (pos < text.length() && (text[pos] == '*' || text[pos] == '/'))
            {
                char op = text[pos];
                pos++;
                skipSpaces(pos);
                Node *rnode = P(pos);
                skipSpaces(pos);
                lnode = new Op(op, lnode, rnode);
            }

            return lnode;
        }

        Node *P(int &pos)
        {
            skipSpaces(pos);

            if (text[pos] == '(')
            {
                pos++;
                skipSpaces(pos);
                Node *node = S(pos);
                skipSpaces(pos);
                if (text[pos] != ')')
                {
                    throw std::runtime_error("Expected closing parenthesis");
                }
                pos++;
                return node;
            }
            else if (isValue(text[pos]))
            {
                Node *node = new Value(text[pos]);
                pos++;
                return node;
            }
            else
            {
                throw std::runtime_error("Invalid character in input");
            }
        }
    };

} // namespace