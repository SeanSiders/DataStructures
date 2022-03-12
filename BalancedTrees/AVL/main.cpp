#include "avl.hpp"

int main()
{
    AvlTree<int> tree;

    tree.insert(50);

    tree.debugDisplay();
    std::cin.get();

    tree.insert(45);
    
    tree.debugDisplay();
    std::cin.get();

    tree.insert(15);

    tree.debugDisplay();
    std::cin.get();

    tree.insert(17);

    tree.debugDisplay();
    std::cin.get();

    tree.insert(23);

    tree.debugDisplay();
    std::cin.get();

    tree.insert(18);

    tree.debugDisplay();
    std::cin.get();

    return 0;
}
