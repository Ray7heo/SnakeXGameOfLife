#include "include/Menu.h"




int main(int argc, char* argv[])
{
    const auto config = GameConfig(100, 100, 1000, 1000);
    auto menu = Menu(config);
    menu.update();
    return 0;
}
