/*
 *
 * 
 * @author, Sean Siders, sean.siders@icloud.com
 */

#include "list.hpp"

int main()
{

    lll::List<int> list;

    list.insertBack(33);
    list.insertBack(100);
    list.insertBack(2);
    list.insertBack(33);
    list.insertBack(324);
    list.insertBack(3);
    list.insertBack(32489);
    list.insertBack(33);
    list.insertBack(234);
    list.insertBack(342);
    list.insertBack(33);

    list.display();

    lll::List<int> r;

    size_t rCount = list.retrieve(33, r);
    std::cout << "RETRIEVE COUNT : " << rCount << "\n\n";

    r.display();

    return 0;
}
