/* 
This file contains an implementation of a linear linked list template.
Underlying data in this list is deep copied onto the stack, following
the conventions of data abstraction.

*** PUBLIC INTERFACE

display     : display all data in the list
removeAt    : remove data at a specified location
remove      : remove all data that matches a provided key of type |K|
retrieve    : retrieve all data that matches a provided key of type |K|
clear       : removal all data from the list
isEmpty     : check if the list is empty
length   : get the number of items in the list

*** STANDARD LLL INTERFACE

insertFront : insert at the front of the list 
insertBack  : insert at the back of the list
insertAt    : insert data at a specified location

*** SORTED LLL INTERFACE

insert      : insert the data in its respective sorted location

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

//////////////// FORWARD DECLARATIONS

template <typename T>
class BaseList;

template <typename T>
class SortedList;

//////////////// GLOBAL OPERATOR OVERLOAD (for display)

template <typename T>
std::ostream& operator<<(std::ostream&, const BaseList<T>&);

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
        copy = *data;
    }

    //Compare this node's data |compare|
    //True if this node's data is greater than or equal to |compare|
    bool greaterThanEq(const T& compare) const
    {
        if (!data) return false;
        return *data >= compare;
    }

    //Compare this node's data to |key|
    //Return true if they are equivalent
    template <typename K = T>
    bool equals(K& key)
    {
        return (data && *data == key);
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

////////////////////////////// LIST ABSTRACT BASE

template <typename T>
class BaseList
{
    public:

    //////////////// CONSTRUCTORS

    BaseList() : head(nullptr), tail(nullptr), listLength(0) {}

    BaseList(const BaseList& source) : head(nullptr), tail(nullptr), listLength(0)
    {
        *this = source;
    }

    //////////////// DESTRUCTOR 

    ~BaseList() { clear(); }

    //////////////// OPERATOR OVERLOADS

    friend std::ostream& operator<<(std::ostream& out, const BaseList& list)
    {
        list.display(out);
        return out;
    }

    //////////////// PURE FUNCTION

    virtual size_t insert(const T& data) = 0;

    //////////////// PUBLIC FUNCTIONS 

    //Display the contents of the list from |head| to |tail|
    //Return the number of items in the list
    size_t display(std::ostream& out = std::cout) const
    {
        //Empty list
        if (!head) return 0;

        return display(out, head);
    }

    //Remove an item at the specified |index| with |head| being |index = 0|
    void removeAt(const size_t index)
    {
        //1) If the list is empty, or |index| specifiies a location beyond the list 
        if (!head || index >= listLength) return;

        //2) Recursively traverse to the |index| and remove that node
        removeAt(index, head);
    }

    //This overload allows for the caller to retrieve a copy of the removed data with |removed|
    //This copy is done with a simple assignment operator : =
    //See the top of this file for the required operator overloads of the caller's data
    void removeAt(const size_t index, T& removed)
    {
        //1) If the list is empty, or |index| specifies a location beyond the list 
        if (!head || index >= listLength) return;

        //2) Recursively traverse to the |index| and remove that node
        removeAt(index, head, removed);
    }

    //Attempt to remove any items that match the provided |removeKey|
    //|removeKey| can be of any specified datatype as |K|
    //Return the number of removals that occured
    template <typename K = T>
    size_t remove(const K& removeKey)
    {
        //Empty list
        if (!head) return 0;

        return remove(removeKey, head);
    }

    //Attempt to retrieve the |n|th item that matches the provided |retrieveKey|
    //|n| will default to the 1st occurence
    //Return true if a retrieve was successful
    template <typename K = T>
    bool retrieve(const K& retrieveKey, T& retrieved, size_t n = 1) const
    {
        //Empty list or |n| was not semantically valid input
        if (!head || n > listLength) return false;

        return retrieve(retrieveKey, retrieved, n, head);
    }

    //Attempt to retrieve any items that match the provided |retrieveKey|
    //Insert any matching items into the provided |retrieveList|
    //Return the number of items retrieved, which will consequently be the length of |retrieveList|
    template <typename K = T>
    size_t retrieve(const K& retrieveKey, BaseList<T>& retrieveList) const
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
        listLength = 0;
    }

    //True if the list is empty
    bool isEmpty() const
    {
        return !head;
    }

    //Returns the number of items currently in the list
    size_t length() const
    {
        return listLength;
    }

    protected:

    //////////////// DATA 

    //The head of the list
    Node<T>* head;

    //The tail of the list
    Node<T>* tail;

    //The length of the list
    size_t listLength;
    
    //////////////// PROTECTED FUNCTIONS 

    //WRAPPER
    //Check if a list already exists, if so, deallocate before recursive copy
    void copy(Node<T>* sourceHead, Node<T>* sourceTail)
    {
        if (head) clear();

        copy(head, tail, sourceHead, sourceTail);
    }

    //Deep copy |source| into this list with |head|
    //Effiency will always be O(N) : N = (length of source list)
    void copy(Node<T>*& head, Node<T>*& tail, Node<T>* sourceHead, Node<T>* sourceTail)
    {
        //Allocate and copy over data
        head = new Node<T>(*sourceHead->_data());

        //If this is the tail of |source|
        if (sourceHead == sourceTail)
        {
            //Assign tail, and exit
            tail = head;
            return;
        }

        copy(head->_next(), tail, sourceHead->_next(), sourceTail);
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

    //Traverse with |head| until |currentIndex| is equivalent to |removeIndex|
    //Remove the node at this location in the list
    //|currentIndex = 0| reflects the default starting node index of this procedure : (this->head)
    //* In the case that tail is being removed, tail will be reset as the stack unwinds from the recursive calls
    //* This is specified with the bool return value of the function
    void removeAt(const size_t removeIndex, Node<T>*& head, size_t currentIndex = 0)
    {
        //Remove location
        if (currentIndex == removeIndex)
        {
            //Remove the node at this index
            //If tail is the node being removed it will be set to null here
            remove(head);

            return;
        }

        //Traverse to next node, incrementing |currentIndex|
        //This will return true in the instance that tail is removed from the list
        removeAt(removeIndex, head->_next(), currentIndex + 1);

        //If the |tail| was removed it will be null in the first occurence of head recursion
        //|tail| will need to be reset to the new list tail here
        if (!tail) tail = head;
    }

    //This recursive overload allows the client to retrieve a copy of the removed data with |removed|
    void removeAt(const size_t removeIndex, Node<T>*& head, T& removed, size_t currentIndex = 0)
    {
        //Remove location
        if (currentIndex == removeIndex)
        {
            //Copy over the data that will be removed
            head->copyData(removed);

            //Remove the node at this index
            //If tail is the node being removed it will be set to null here
            remove(head);

            return;
        }

        //Traverse to next node, incrementing |currentIndex|
        //This will return true in the instance that tail is removed from the list
        removeAt(removeIndex, head->_next(), removed, currentIndex + 1);

        //If the |tail| was removed it will be null in the first occurence of head recursion
        //|tail| will need to be reset to the new list tail here
        if (!tail) tail = head;
    }

    //Remove |toRemove| and link up the remaining list
    //If the tail of the list is being removed, it will be set to null here
    void remove(Node<T>*& toRemove)
    {
        //If the list tail is being removed, set the true tail to null
        //|tail| is reset in head recursion
        if (toRemove == tail)
        {
            delete toRemove;
            toRemove = tail = nullptr;
        }

        //The list tail is not being removed
        else
        {
            //Hold onto any list that may exist beyond the removal
            Node<T>* hold = toRemove->_next();

            //Detach and deallocate the node to remove from the list
            toRemove->setNext(nullptr);
            delete toRemove;

            //Link any list that may exist beyond the removal
            toRemove = hold;
        }

        --listLength;
    }

    //Traverse with |head| removing any nodes that contain matching data to |removeKey|
    //Return the number of removals that occured
    //If tail is removed |resetTail| is set to true, and tail will be reassigned in head recursion
    template <typename K = T>
    size_t remove(const K& removeKey, Node<T>*& head)
    {
        if (!head) return 0;

        //The number of removals to return
        size_t removeCount = 0;

        //If the item was removed, increment by 1 and head will already be set to the next consecutive node
        if (removeIfMatched(removeKey, head)) removeCount += remove(removeKey, head) + 1;

        //Otherwise continue with traversal
        else removeCount = remove(removeKey, head->_next());

        //If the |tail| was removed it will be null in the first occurence of head recursion
        //|tail| will need to be reset to the new list tail here
        if (!tail) tail = head;

        return removeCount;
    }

    //Compare |removeKey| with head's data
    //If they match, remove the current node from the list
    //If the removal occurs, return true
    template <typename K = T>
    bool removeIfMatched(const K& removeKey, Node<T>*& head)
    {
        //If the data matches
        if (head->equals(removeKey))
        {
            //If the list tail is being removed, set the true tail to null
            //|tail| is reset in head recursion
            if (head == tail)
            {
                delete head;
                head = tail = nullptr;
            }

            //The list tail is not being removed
            else
            {
                //Hold onto any list that may exist beyond the removal
                Node<T>* hold = head->_next();

                //Detach and deallocate the node to remove from the list
                head->setNext(nullptr);
                delete head;

                //Link any list that may exist beyond the removal
                head = hold;
            }

            --listLength;

            //1 will be added to the |remove| calling routine
            return true;
        }

        return false;
    }

    //Traverse the list with |head|
    //Decrement |n| for each matching item in the list
    //The |n|th occurence to copy into |retrieved| will happen when |n| is 0
    //Return false if the |n|th matching occurence does not exist in the list
    template <typename K = T>
    bool retrieve(const K& retrieveKey, T& retrieved, size_t& n, Node<T>* head) const
    {
        //End of list, no match on |n|th occurence
        if (!head) return false;

        //If a match is found
        if (head->equals(retrieveKey))
        {
            --n;

            //If n is 0, this match is the item to retrieve
            if (0 == n)
            {
                head->copyData(retrieved);
                return true;
            }
        }

        return retrieve(retrieveKey, retrieved, n, head->_next());
    }

    //Traverse the list with |head| comparing all items to |retrieveKey|
    //Insert any matching items into the provided |retrieveList|
    //Return the number of items retrieved, which will consequently be the length of |retrieveList|
    template <typename K = T>
    size_t retrieve(const K& retrieveKey, BaseList<T>& retrieveList, Node<T>* head) const
    {
        //End of the list
        if (!head) return 0;

        //If a match is found
        if (head->equals(retrieveKey))
        {
            //Insert it at the end of the retrieve list
            retrieveList.insert(*head->_data());

            //Increment return value by 1
            return retrieve(retrieveKey, retrieveList, head->_next()) + 1;
        }

        return retrieve(retrieveKey, retrieveList, head->_next());
    }
};

////////////////////////////// LINEAR LINKED LIST 

template <typename T>
class List : public BaseList<T>
{
    friend SortedList<T>;

    public:

    //////////////// CONSTRUCTORS

    List() {}

    List(const List& source)
    {
        *this = source;
    }

    //Makes a complete deep copy of |rhs| into this list
    List& operator=(const List& rhs)
    {
        //If this is not self assignment, or |rhs| is not empty
        if (this != &rhs && rhs.head)
        {
            BaseList<T>::copy(rhs.head, rhs.tail);
            this->listLength = rhs.listLength;
        }

        return *this;
    }

    List& operator=(const SortedList<T>& rhs)
    {
        BaseList<T>::copy(rhs.head, rhs.tail);
        this->listLength = rhs.listLength;

        return *this;
    }

    //////////////// PUBLIC FUNCTIONS 

    size_t insert(const T& data)
    {
        insertBack(data);
        return 0;
    }

    //Insert |data| at the front of the list
    void insertFront(const T& data)
    {
        //Allocate node and deep copy in |data|
        Node<T>* alloc = new Node<T>(data);

        //1) Empty list
        if (!this->head) this->head = this->tail = alloc;

        //2) Head insertion
        else
        {
            alloc->setNext(this->head);
            this->head = alloc;
        }

        ++this->listLength;
    }

    //Append |data| to the end of the list
    void insertBack(const T& data)
    {
        //Allocate node and deep copy in |data|
        Node<T>* alloc = new Node<T>(data);

        //1) Empty list 
        if (!this->head) this->head = this->tail = alloc;

        //2) Append the new node as the new tail of the list
        else
        {
            this->tail->setNext(alloc);
            this->tail = alloc;
        }

        ++this->listLength;
    }

    //Insert into the list at the specified |index| with |head| being |index = 0|
    void insertAt(const size_t index, const T& data)
    {
        //1) If the list is empty or |index| is : >= |length| : insert at the end
        //  * Note that this means |tail| will never be modified in the recursive call : 2)
        //  * Instead this mutation will be delegated to |insertBack|
        if (!this->head || index >= this->listLength) insertBack(data);

        //2) Recursively traverse to the |index| inserting |data| in a new node
        else
        {
            insertAt(index, data, this->head);
            ++this->listLength;
        }
    }

    protected:

    //////////////// PROTECTED FUNCTIONS 

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
};

////////////////////////////// SORTED LINEAR LINKED LIST 
//This list will automatically sort incoming data from least at |head| to greatest at |tail|
//Assigning any other type of list to a list of this type will still result in a sorted LLL

template <typename T>
class SortedList : public BaseList<T>
{
    friend List<T>;

    public:

    //////////////// CONSTRUCTORS

    SortedList() {}

    SortedList(const SortedList& source)
    {
        *this = source;
    }

    SortedList(const List<T>& source)
    {
        *this = source;
    }

    //////////////// OPERATOR OVERLOADS

    //Assigning a |List| to |SortedList| will result in a sorted list
    //Worst case scenario of efficiency : O(N log N)
    SortedList& operator=(const List<T>& rhs)
    {
        if (this->head) BaseList<T>::clear();

        sortedCopy(rhs.head);
        this->listLength = rhs.listLength;

        return *this;
    }

    //Makes a complete deep copy of |rhs| into this list
    SortedList& operator=(const SortedList& rhs)
    {
        //If this is not self assignment, or |rhs| is not empty
        if (this != &rhs && rhs.head)
        {
            //Non-sorted copy algorithm for O(N) performance
            //Since |rhs| is guaranteed to be sorted
            BaseList<T>::copy(rhs.head, rhs.tail);
            this->listLength = rhs.listLength;
        }

        return *this;
    }

    //////////////// PUBLIC FUNCTIONS 

    //Insert |data| in sorted order
    //Return the node index this data was inserted at
    size_t insert(const T& data)
    {
        ++this->listLength;
        return insert(data, this->head);
    }

    protected:

    //////////////// PROTECTED FUNCTIONS 

    //Traverse with |head| until the sorted location is found to insert |data|
    size_t insert(const T& data, Node<T>*& head)
    {
        //End of list, or list is empty
        if (!head)
        {
            head = new Node<T>(data);
            this->tail = head;
            return 0;
        }
        
        //Sorted location found
        if (head->greaterThanEq(data))
        {
            //Hold onto the rest of the list
            Node<T>* hold = head;

            //Allocate a new node, effectively setting the previous node's next pointer
            head = new Node<T>(data);

            //Link up the rest of the list
            head->setNext(hold);

            return 0;
        }

        return insert(data, head->_next()) + 1;
    }

    //Deep copy |source| into this list with |head|
    //This will always result in the list being sorted, regardless of list input
    void sortedCopy(Node<T>* sourceHead)
    {
        //End of the source list
        if (!sourceHead) return;

        //Copy data into the sorted insert algorithm
        insert(*sourceHead->_data(), this->head);

        sortedCopy(sourceHead->_next());
    }
};

}

#endif //LIST_HPP
