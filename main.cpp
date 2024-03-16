#include "include/Config.h"
#include "include/Menu.h"

int main(int argc, char* argv[])
{
    const auto config = Config();

    auto menu = Menu(config);

    menu.update();
    
    return 0;
}
