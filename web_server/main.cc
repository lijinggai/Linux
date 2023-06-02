#include "HttpServer.hpp"
#include <cstdlib>
#include <memory>

void Usage()
{
    std::cout << "usage: ./main port" << std::endl;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage();
        return 1;
    }
    HttpSocket s(atoi(argv[1]));
    s.InitServer();
    s.loop();
    return 0;
}