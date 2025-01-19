#include "src/CMDParser.h"

int main(int argc, char* argv[]) {
    try 
    {
        TCMDParser p(argc, argv);
        p.convert();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}