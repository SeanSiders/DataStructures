/* 
This file contains an implementation of a linear linked list template.
Underlying data in this list is deep copied onto the stack, following
the conventions of data abstraction.

*** PUBLIC INTERFACE

display     : display all data in the list
insertFront : insert at the front of the list 
insertBack  : insert at the back of the list
insertAt    : insert data at a specified location
removeAt    : remove data at a specified location
remove      : remove all data that matches a provided key of type |K|
retrieve    : retrieve all data that matches a provided key of type |K|
clear       : removal all data from the list
isEmpty     : check if the list is empty
getLength   : get the number of items in the list

*** LIST OPERATOR OVERLOADS

==  | Determine if two lists contain the same data in the same order
=   | Make a new copy of the list (no shared memory)
<<  | Display the entire list

*** CLIENT REQUIRED OPERATOR OVERLOADS

The following operator overloads must be implemented for user-defined datatypes
that are managed by this list.

==  |  Comparison with key (of type K) for retrieval / removal
<   |  Sorted insert LLL                   
=   |  Deep copy : insert / retrieve 
<<  |  Inserting stream to specified ostream  : display

@author, Sean Siders, sean.siders@icloud.com
*/

#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>

namespace lll {

////////////////////////////// NODE

/* This node abstraction manages the underlying data of the linear linked list
 * Data is deep copied into dynamic memory
 * All operations such as display, comparison, and copying out are handled here
 */

template <typename T>
class Node
{
    public:

    //////////////// CONSTRUCTORS

    Node() : next(nullptr), data(nullptr) {}

    //Deep copy |data| into this node
    Node(const T& _data) : next(nullptr), data(nullptr)
    {
        data = new T(_data);
    }

    //////////////// DESTRUCTOR 

    ~Node()
    {
        delete data;
        data = nullptr;

        delete next;
        next = nullptr;
    }

    //////////////// PUBLIC FUNCTIONS 

    //Insert this nodes data into ostream |out|
    void display(std::ostream& out = std::cout) const
    {
        if (data) out << *data;
    }

    //Copy |data| into |copy| with the simple assignment operator
    void copyData(T& copy)
    {
        *data = copy;
    }

    //Compare this node's data this other's data
    //True if this node's data is less than the other's
    bool lessThan(Node* other) const
    {
        if (!data || !other || !other->data) return false;
        return *data < *other->data;
    }

    //Compare this node's data to |key|
    //Return true if they are equivalent
    template <typename K = T>
    bool equals(K& key)
    {
        return *data == key;
    }

    //|next| getter
    Node*& _next() 
    {
        return next;
    }

    //|next| setter
    void setNext(Node* _next)
    {
        next = _next;
    }

    //|data| getter
    T* _data() const
    {
        return data;
    }

    private:

    //The next node in the list
    Node* next;

    //The underlying data of each node
    //This is dynamically allocated, allowing for dynamic binding
    T* data;
};

////////////////////////////// LINEAR LINKED LIST

template <typename T>
class BaseList
{
    public:

    //////////////// CONSTRUCTORS
    //////////////// DESTRUCTOR 
    //////////////// PUBLIC FUNCTIONS 

    protected:

    //////////////// DATA 

    //////////////// PRIVATE FUNCTIONS 
};

template <typename T>
class List : public BaseList<T>
{
    public:

    //////////////// CONSTRUCTORS

    List() : head(nullptr), tail(nullptr), length(0) {}

    List(const List& source) : head(nullptr), tail(nullptr), length(0)
    {
        *this = source;
    }

    //////////////// DESTRUCTOR 

    ~List() { clear(); }

    //////////////// OPERATOR OVERLOADS

    //Makes a complete deep copy of |rhs| into this list
    List& operator=(const List& rhs)
    {
        //If this is not self assignment, or |rhs| is not empty
        if (this != &rhs && rhs.head)
        {
            copy(head, tail, rhs.head, rhs.tail);
            length = rhs.length;
        }

        return *this;
    }

    //////////////// PUBLIC FUNCTIONS 

    //Display the contents of the list from |head| to |tail|
    //Return the number of items in the list
    size_t display(std::ostream& out = std::cout) const
    {
        //Empty list
        if (!head) return 0;

        return display(out, head);
    }

    //Insert |data| at the front of the list
    void insertFront(const T& data)
    {
        //Allocate node and deep copy in |data|
        Node<T>* alloc = new Node<T>(data);

        //1) Empty list
        if (!head) head = tail = alloc;

        //2) Head insertion
        else
        {
            alloc->setNext(head);
            head = alloc;
        }

        ++length;
    }

    //Append |data| to the end of the list
    void insertBack(const T& data)
    {
        //Allocate node and deep copy in |data|
        Node<T>* alloc = new Node<T>(data);

        //1) Empty list 
        if (!head) head = tail = alloc;

        //2) Append the new node as the new tail of the list
        else
        {
            tail->setNext(alloc);
            tail = alloc;
        }

        ++length;
    }

    //Insert into the list at the specified |index| with |head| being |index = 0|
    void insertAt(const size_t index, const T& data)
    {
        //1) If the list is empty or |index| is : >= |length| : insert at the end
        //  * Note that this means |tail| will never be modified in the recursive call : 2)
        //  * Instead this mutation will be delegated to |insertBack|
        if (!head || index >= length) insertBack(data);

        //2) Recursively traverse to the |index| inserting |data| in a new node
        else
        {
            insertAt(index, data, head);
            ++length;
        }
    }

    //Remove an item at the specified |index| with |head| being |index = 0|
    void removeAt(const size_t index)
    {
        //1) If the list is empty, or |index| specifiies a location beyond the list 
        if (!head || index >= length) return;

        //2) Recursively traverse to the |index| and remove that node
        removeAt(index, head);
        --length;
    }

    //This overload allows for the caller to retrieve a copy of the removed data with |removed|
    //This copy is done with a simple assignment operator : =
    //See the top of this file for the required operator overloads of the caller's data
    void removeAt(const size_t index, T& removed)
    {
        //1) If the list is empty, or |index| specifies a location beyond the list 
        if (!head || index >= length) return;

        //2) Recursively traverse to the |index| and remove that node
        removeAt(index, head, removed);
        --length;
    }

    //Attempt to remove any items that match the provided |removeKey|
    //|removeKey| can be of any specified datatype as |K|
    //Return the number of removals that occured
    template <typename K = T>
    size_t remove(const K& removeKey)
    {
        //Empty list
        if (!head) return false;

        //Used in the recursive function to reset tail if it is removed
        bool resetTail = false;

        return remove(removeKey, head, resetTail);
    }

    //Attempt to retrieve any items that match the provided |retrieveKey|
    //Insert any matching items into the provided |retrieveList|
    //Return the number of items retrieved, which will consequently be the length of |retrieveList|
    template <typename K = T>
    size_t retrieve(const K& retrieveKey, List<T>& retrieveList) const
    {
        //Empty list
        if (!head) return 0;

        return retrieve(retrieveKey, retrieveList, head);
    }

    //Remove all items from the list
    void clear()
    {
        delete head;
        head = tail = nullptr;
        length = 0;
    }

    //True if the list is empty
    bool isEmpty() const
    {
        return !head;
    }

    //Returns the number of items currently in the list
    size_t getLength() const
    {
        return length;
    }

    private:

    //////////////// DATA 

    //The head of the list
    Node<T>* head;

    //The tail of the list
    Node<T>* tail;

    //The length of the list
    size_t length;
    
    //////////////// PRIVATE FUNCTIONS 

    //Deep copy |source| into this list with |head|
    void copy(Node<T>*& head, Node<T>*& tail, Node<T>* srcHead, Node<T>* srcTail)
    {
        //Allocate and copy over data
        head = new Node<T>(*srcHead->_data());

        //If this is the tail of |source|
        if (srcHead == srcTail)
        {
            //Assign tail, and exit
            tail = head;
            return;
        }

        copy(head->_next(), tail, srcHead->_next(), srcTail);
    }

    //Traverse with |head| displaying the entire list
    size_t display(std::ostream& out, Node<T>* head) const
    {
        //Display node data 
        head->display(out);

        //End of list has been reached, display new lines and return
        if (head == tail) 
        {
            out << "\n\n";
            return 1;
        }

        //If there are more nodes in the list, display a connecting arrow
        out << " -> ";

        return display(out, head->_next()) + 1;
    }

    //Traverse with |head| until |currentIndex| is equivalent to |insertIndex|
    //Insert |data| at this location in the list
    //|currentIndex = 0| reflects the default starting node index of this procedure : (this->head)
    void insertAt(const size_t insertIndex, const T& data, Node<T>*& head, size_t currentIndex = 0)
    {
        //Insert location
        if (currentIndex == insertIndex)
        {
            //Hold onto the list that exists beyond this insertion
            Node<T>* hold = head;

            //Allocate a new node, deep copying in |data|
            head = new Node<T>(data);

            //Link the list that exists beyond this insertion
            head->setNext(hold);

            return;
        }
        
        //Traverse to next node, incrementing |currentIndex|
        insertAt(insertIndex, data, head->_next(), currentIndex + 1);
    }

    //Traverse with |head| until |currentIndex| is equivalent to |removeIndex|
    //Remove the node at this location in the list
    //|currentIndex = 0| reflects the default starting node index of this procedure : (this->head)
    //* In the case that tail is being removed, tail will be reset as the stack unwinds from the recursive calls
    //* This is specified with the bool return value of the function
    bool removeAt(const size_t removeIndex, Node<T>*& head, size_t currentIndex = 0)
    {
        //Remove location
        if (currentIndex == removeIndex)
        {
            //If tail is being removed, this will allow for tail to be reset in head recursion
            bool resetTail = (head == tail);

            //Hold onto any list that exists beyond this insertion
            Node<T>* hold = head->_next();

            //Detach and deallocate the node to remove from the list
            head->setNext(nullptr);
            delete head;

            //Link any list that may exist beyond the removal
            head = hold;

            return resetTail;
        }

        //Traverse to next node, incrementing |currentIndex|
        //This will return true in the instance that tail is removed from the list
        if(removeAt(removeIndex, head->_next(), currentIndex + 1)) tail = head;
        
        //Continue to return false so tail is only reset once
        return false;
    }

    //This recursive overload allows the client to retrieve a copy of the removed data with |removed|
    bool removeAt(const size_t removeIndex, Node<T>*& head, T& removed, size_t currentIndex = 0)
    {
        //Remove location
        if (currentIndex == removeIndex)
        {
            //If tail is being removed, this will allow for tail to be reset in head recursion
            bool resetTail = (head == tail);

            //Hold onto any list that exists beyond this insertion
            Node<T>* hold = head->_next();

            //Copy the data to remove into the caller's supplied object |removed|
            head->copyData(removed);

            //Detach and deallocate the node to remove from the list
            head->setNext(nullptr);
            delete head;

            //Link any list that may exist beyond the removal
            head = hold;

            return resetTail;
        }

        //Traverse to next node, incrementing |currentIndex|
        //This will return true in the instance that tail is removed from the list
        if (removeAt(removeIndex, head->_next(), removed, currentIndex + 1)) tail = head;
        
        //Continue to return false so tail is only reset once
        return false;
    }

    //Traverse with |head| removing any nodes that contain matching data to |removeKey|
    //Return the number of removals that occured
    //If tail is removed |resetTail| is set to true, and tail will be reassigned in head recursion
    template <typename K = T>
    size_t remove(const K& removeKey, Node<T>*& head, bool& resetTail)
    {
        if (!head) return 0;

        //The number of removals to return
        size_t removeCount = 0;

        //Remove and increment |removeCount| if data matches, traverse to the next node
        if (removeCheck(removeKey, head, resetTail)) removeCount = remove(removeKey, head, resetTail) + 1;
        else removeCount = remove(removeKey, head->_next(), resetTail);

        //If the tail was removed, it will need to be reset to the first non-null head that occurs here
        if (resetTail && head)
        {
            tail = head;
            resetTail = false;
        }

        return removeCount;
    }

    //Compare |removeKey| with head's data
    //If they match, remove the current node from the list
    //If the removal occurs, return true
    //|restTail| will only be set to true if the tail is the node being removed
    template <typename K = T>
    size_t removeCheck(const K& removeKey, Node<T>*& head, bool& resetTail)
    {
        //If the data matches
        if (head->equals(removeKey))
        {
            //Set to true if tail is being removed
            resetTail = (head == tail);

            //Hold onto any list that may exist beyond the removal
            Node<T>* hold = head->_next();

            //Detach and deallocate the node to remove from the list
            head->setNext(nullptr);
            delete head;

            //Link any list that may exist beyond the removal
            head = hold;

            --length;

            //1 will be added to the |remove| calling routine
            return 1;
        }

        return 0;
    }

    //Traverse the list with |head| comparing all items to |retrieveKey|
    //Insert any matching items into the provided |retrieveList|
    //Return the number of items retrieved, which will consequently be the length of |retrieveList|
    template <typename K = T>
    size_t retrieve(const K& retrieveKey, List<T>& retrieveList, Node<T>* head) const
    {
        //End of the list
        if (!head) return 0;

        //If a match is found
        if (head->equals(retrieveKey))
        {
            //Insert it at the end of the retrieve list
            retrieveList.insertBack(*head->_data());

            //Increment return value by 1
            return retrieve(retrieveKey, retrieveList, head->_next()) + 1;
        }

        return retrieve(retrieveKey, retrieveList, head->_next());
    }
};

}

#endif //LIST_HPP
