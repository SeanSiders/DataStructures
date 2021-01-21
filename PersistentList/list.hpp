#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <memory>

template<typename T>
struct Node
{
    std::shared_ptr<T> _data;
    std::shared_ptr<Node<T>> _next;

    explicit Node(const T& data) : _data(std::make_shared<T>(data)) {}
    Node(const std::shared_ptr<T>& ptr) : _data(ptr) {}

    ~Node()
    {
        _data = nullptr;
        _next = nullptr;
    }
};


template<typename T>
class List
{
    template <typename R>
    friend class List;

    public:
        
        /**
         * A lambda function defined by the client will be used to
         * operate on every item in the list.
         */
         template <typename R>
         List<R> map(R (*f)(const T&)) const
         {
             List<R> new_list(f(*head->_data));

             for (auto current = head->_next; current; current = current->_next)
                new_list.grow_tail(std::make_shared<R>(f(*current->_data)));

             return new_list;
         }

        /**
         * Creates an empty list.
         */
        List() : list_length(0) {}

        /**
         * Creates a list with a single element whose value is |data|.
         */
        explicit List(const T& data) : head(std::make_shared<Node<T>>(data)), tail(head), list_length(1) {}

        /**
         * Sets all pointers to null, delegating memory frees to std::shared_ptr.
         */
        ~List()
        {
            head = nullptr;
            tail = nullptr;
        }
        /**
         * Returns the number of elements in the list.
         */
        size_t length() const
        {
            return list_length;
        }

        /**
         * True if the list is empty, otherwise false.
         */
        bool is_empty() const
        {
            return !list_length;
        }

        /**
         * Returns the first element in the list.
         * Returns nullptr if the list is empty.
         */
        std::shared_ptr<const T> first() const
        {
            return head ? head->_data : nullptr;
        }

        /**
         * Returns the last element in the list.
         * Returns nullptr if the list is empty.
         */
        std::shared_ptr<const T> last() const
        {
            return tail ? tail->_data : nullptr;
        }

        /**
         * Returns the nth element in the list.
         * Returns nullptr if |n| is out of bounds.
         */
        std::shared_ptr<const T> nth(size_t n) const
        {
            if (is_empty() || n < 0 || n > list_length - 1) return nullptr;

            auto current = head;

            for (int i = 0; i < n; ++i) current = current->_next;

            return current->_data;
        }

        /**
         * Pushes a new element containing |data| to the back of the list.
         */
        List<T> push_back(const T& data) const
        {
            if (is_empty()) return List(data);

            //duplicate the entire list
            List new_list(head->_data);
            duplicate_nodes(new_list, head->_next, list_length - 1);

            //allocate and append the new node with |data|
            new_list.grow_tail(std::make_shared<T>(data));

            new_list.set_length(list_length + 1);

            return new_list;
        }

        /**
         * Pushes a new element containing |data| to the front of the list.
         */
        List<T> push_front(const T& data) const
        {
            if (is_empty()) return List(data);

            //allocate new node at the head
            List new_list(data);

            //link up the old list
            new_list.head->_next = head;
            new_list.tail = tail;
            new_list.set_length(list_length + 1);

            return new_list;
        }

        /**
         * Inserts |data| at |index| location in the list.
         * If index is larger than the list's size, then |data| will
         * be pushed to the back of the list.
         */
        List<T> insert_at(size_t index, const T& data) const
        {
            //index is out of bounds
            if (index >= list_length || index < 0) return push_back(data);

            //insert is desired at the head
            if (0 == index) return push_front(data);

            //duplicate all nodes leading up to the mutation
            List new_list(head->_data);
            auto link = duplicate_nodes(new_list, head->_next, index - 1);

            //insert the new node
            new_list.grow_tail(std::make_shared<T>(data));

            //link up the rest of the old list
            new_list.tail->_next = link;
            new_list.tail = tail;

            new_list.set_length(list_length + 1);

            return new_list;
        }

        /**
         * Removes the data at |index| in the list.
         * If the list does not have an element that corresponds
         * to |index| then nothing will happen.
         */
        List<T> remove_at(size_t index) const
        {
            //invalid index provided
            if (index >= list_length || index < 0 || is_empty()) return *this;
            
            //only one node is in the list
            if (list_length == 1) return List();

            //removal is at the head
            if (0 == index)
            {

                //allocate head with the node after head
                auto next = head->_next;
                List new_list(next->_data);

                //link up the rest of the list
                new_list.head->_next = next->_next;

                new_list.tail = tail;
                new_list.set_length(list_length - 1);

                return new_list;
            }

            //duplicate all nodes leading up to the mutation
            List new_list(head->_data);
            auto link = duplicate_nodes(new_list, head->_next, index - 1);

            //if the removal was not at the tail
            if (index < list_length - 1)
            {
                new_list.tail->_next = link->_next;
                new_list.tail = tail;
            }

            new_list.set_length(list_length - 1);

            return new_list;
        }

        /**
         * Displays relevant debug information for the list.
         */
        void debug_info(const char* name = "list") const
        {
              std::cout << name << " { " << std::endl;

              for (auto current = &head; *current; current = &(*current)->_next)
              {
                  std::cout << "  data(" << *(*current)->_data
                  << "), data_count(" << (*current)->_data.use_count()
                  << "), self_count(" << (*current).use_count()
                  << ")," << std::endl;
              }

              std::cout << "}" << std::endl;
        }

    private:
        //front of the list
        std::shared_ptr<Node<T>> head;
        //end of the list
        std::shared_ptr<Node<T>> tail;

        //the number of nodes currently in the list
        size_t list_length = 0;

        /**
         * Makes a new list with a single node at the value |data|.
         */
         explicit List(std::shared_ptr<T>& data) : head(std::make_shared<Node<T>>(data)), tail(head), list_length(1) {}

        /**
         * Allocate a new node with |data| at the end of the list.
         */
        void grow_tail(std::shared_ptr<T> data) 
        {
            tail->_next = std::make_shared<Node<T>>(data);
            tail = tail->_next;
        }

        /**
         * Assign |list_length| a new specified value
         */
        void set_length(const size_t length) { list_length = length; }

        /**
         * Make a copy of the nodes from |source| in the current list up to |index|.
         * The nodes will still share the underlying data's memory.
         * If the index is out of bounds, the entire list will be duplicated.
         * Return the node that follows all duplications, which could be null.
         */
        static std::shared_ptr<Node<T>> duplicate_nodes(List& new_list, std::shared_ptr<Node<T>> source, const size_t index)
        {
            for (int i = 0; i < index; ++i)
            {
                new_list.grow_tail(source->_data);
                source = source->_next;
            }

            return source;
        }
};

#endif //LIST_HPP
