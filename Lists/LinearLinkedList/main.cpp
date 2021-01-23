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
    list = list;
    list.display();

    list = list;

    lll::List<int> copy(list);

    copy.display();
    copy.clear();

    copy = list;
    copy.display();

    return 0;
}
