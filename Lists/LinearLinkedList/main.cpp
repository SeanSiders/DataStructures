/* @author, Sean Siders, sean.siders@icloud.com */

#include "list.hpp"

int main()
{
    /*
    lll::SortedList<int> sList;

    int input = 99;

    while (input != -1)
    {
        size_t loc = sList.insert(input);
        size_t length = sList.display();
        std::cout << "INSERT LOCATION : " << loc << "\nLENGTH : " << length << "\n\n";

        if (length != sList.length()) std::cout << "LENGTH DOES NOT MATCH\n\n";

        std::cin >> input;
    }

    lll::List<int> list(sList);

    list.display();
    */

    lll::List<int> list;

    list.insertBack(20);
    list.insertBack(3);
    list.insertBack(99);
    list.insertBack(1);
    list.insertBack(1001);
    list.insertBack(1002);
    list.insertBack(2);
    list.insertBack(999);

    lll::SortedList<int> sList;

    sList = list;

    /*
    size_t length = sList.display();
    std::cout << "\nLENGTH : " << length << "\n\n";
    if (length != sList.length()) std::cout << "LENGTH DOES NOT MATCH\n\n";

    lll::SortedList<int> sList2;
    sList2 = sList;
    length = sList2.display();
    std::cout << "\n2 LENGTH : " << length << "\n\n";
    if (length != sList2.length()) std::cout << "LENGTH DOES NOT MATCH\n\n";
    */

    lll::List<int> listy;
    listy = list;
    size_t length = listy.display();
    std::cout << "\nLISTY LENGTH : " << length << "\n\n";
    if (length != listy.length()) std::cout << "LENGTH DOES NOT MATCH\n\n";

    listy = sList;
    length = listy.display();
    std::cout << "\nLISTY LENGTH : " << length << "\n\n";
    if (length != listy.length()) std::cout << "LENGTH DOES NOT MATCH\n\n";

    return 0;
}
