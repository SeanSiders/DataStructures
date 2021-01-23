/*
 *
 * 
 * @author, Sean Siders, sean.siders@icloud.com
 */

#include "list.hpp"

int main()
{

    lll::SortedList<int> sList;

    int input = 99;

    while (input != -1)
    {
        size_t loc = sList.insert(input);
        size_t length = sList.display();
        std::cout << "INSERT LOCATION : " << loc << "\nLENGTH : " << length << "\n\n";

        if (length != sList.getLength()) std::cout << "LENGTH DOES NOT MATCH\n\n";

        std::cin >> input;
    }

    lll::List<int> list(sList);

    list.display();

    return 0;
}
