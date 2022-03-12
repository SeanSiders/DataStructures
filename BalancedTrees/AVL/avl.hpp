#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>

////////////////////////////// NODE 

template <typename T>
class Node
{
    public:

    //////////////// CONSTRUCTORS

    Node() : data(nullptr), left(nullptr), right(nullptr), height(1) {}

    Node(const T& source) : data(nullptr), left(nullptr), right(nullptr), height(1)
    {
        data = new T(source);
    }

    //recursive deallocation of entire tree
    ~Node()
    {
        delete data;
        delete left;
        delete right;

        data = nullptr;
        left = nullptr;
        right = nullptr;
    }

    //////////////// PUBLIC FUNCTIONS 

    Node*& _left()
    {
        return left;
    }

    Node*& _right()
    {
        return right;
    }

    void setLeft(Node*& _left)
    {
        left = _left;
    }

    void setRight(Node*& _right)
    {
        right = _right;
    }

    //True if |other| is less than |data|
    bool lessThan(const T& other) const
    {
        return (data && other < *data);
    }

    //True if |left| and |right| are null
    bool isLeaf() const
    {
        return (!left && !right);
    }

    //Height will be set to one larger than the largest subtree height
    //In AVL rotation height is recursively updated using this
    void updateHeight()
    {
        height = 1 + std::max(_height(left), _height(right));
    }

    //True if |leftHeight| and |rightHeight| have a difference larger than 1
    //|hasLeftImbalance| will be set to true if the left subtree is the larger one
    bool isUnbalanced(bool& hasLeftImbalance)
    {
        size_t leftHeight = _height(left);
        size_t rightHeight = _height(right);

        if (std::max(leftHeight, rightHeight) - std::min(leftHeight, rightHeight) > 1)
        {
            hasLeftImbalance = (leftHeight > rightHeight);
            return true;
        }

        return false;
    }

    void display(std::ostream& out) const
    {
        if (data) out << *data;
        out << "\tHEIGHT : " << height;
    }

    private:

    //////////////// DATA 

    //The data this node contains
    T* data;

    //The left child of this node
    Node* left;

    //The right child of this node
    Node* right;

    //The height of this node
    //A leaf has a height of 1
    size_t height;

    //////////////// PRIVATE FUNCTIONS

    //Return the height of the passed |root|
    //0 if |root| is null
    static size_t _height(Node<T>* root)
    {
        return root ? root->height : 0;
    }
};

////////////////////////////// AVL TREE 

template <typename T>
class AvlTree
{
    public:

    //////////////// CONSTRUCTORS

    AvlTree() : root(nullptr), dataCount(0) {}

    ~AvlTree()
    {
        delete root;
    }

    //////////////// PUBLIC FUNCTIONS 

    void insert(const T& data)
    {
        insert(data, root);
    }

    void display(std::ostream& out = std::cout) const
    {
        display(root, out);
    }

    void debugDisplay() const
    {
        if (!root) return;

        std::cout << "\nROOT   : ";
        root->display(std::cout);
        std::cout << '\n';

        debugDisplay(root);
    }

    private:

    //////////////// DATA 

    //The root node of the tree
    Node<T>* root;

    //The number of items in the tree
    size_t dataCount;

    //////////////// PRIVATE FUNCTIONS 

    void insert(const T& data, Node<T>*& root)
    {
        if (!root)
        {
            root = new Node<T>(data);
            ++dataCount;
            return;
        }

        //Will be set to true if :
        // a) The root's left child has a left subtree imbalance
        // b) The root's right child has a left subtree imbalance
        bool hasLeftImbalance = false;

        //If |data| is less than the root's data, traverse left
        if (root->lessThan(data))
        {
            insert(data, root->_left());

            //Update the subtree height
            root->updateHeight();

            if (root->isUnbalanced(hasLeftImbalance))
            {
                std::cout << "\n\nIMBALANCE FOUND\n\n";
                root->display(std::cout);

                //1) The root's left child has a left subtree imbalance
                if (hasLeftImbalance)
                {
                    std::cout << "\nLEFT IMBALANCE\n\n";
                    rotateRight(root);
                }

                //2) The root's left child has a right subtree imbalance
                else
                {
                    std::cout << "\nRIGHT IMBALANCE\n\n";
                    rotateLeft(root->_left());
                    rotateRight(root);
                }
            }
        }

        //|data| is larger than or equal to root's data, traverse right
        else
        {
            insert(data, root->_right());

            //Update the subtree height
            root->updateHeight();

            if (root->isUnbalanced(hasLeftImbalance))
            {
                std::cout << "\n\nIMBALANCE FOUND\n\n";
                root->display(std::cout);

                //3) The root's right child has a left subtree imbalance
                if (hasLeftImbalance)
                {
                    std::cout << "\nLEFT IMBALANCE\n\n";
                    rotateRight(root->_right());
                    rotateLeft(root);
                }

                //4) The root's right child has a right subtree imbalance
                else
                {
                    std::cout << "\nRIGHT IMBALANCE\n\n";
                    rotateLeft(root);
                }
            }
        }
    }

    //////////////// BALANCING ROTATIONS 

    static void rotateRight(Node<T>*& root)
    {
        //Hold onto the old root
        Node<T>* oldRoot = root;

        //Set the left child to be the new root
        root = root->_left();

        //Set the old root's left to the new root's right subtree (null possiblity)
        oldRoot->setLeft(root->_right());
        
        //Adopt |oldRoot| as right subtree
        root->setRight(oldRoot);
        //root->setLeft(root);

        //Update the node heights
        oldRoot->updateHeight();
        root->updateHeight();
    }

    static void rotateLeft(Node<T>*& root)
    {
        //Hold onto the old root
        Node<T>* oldRoot = root;

        //Set the right child to be the new root
        root = root->_right();

        //Set the old root's right to the new root's left subtree (null possiblity)
        oldRoot->setRight(root->_left());
        
        //Adopt |oldRoot| as right subtree
        root->setLeft(oldRoot);

        //Update the node heights
        oldRoot->updateHeight();
        root->updateHeight();
    }

    static void display(Node<T>* root, std::ostream& out)
    {
        if (!root) return;

        display(root->_left(), out);
        root->display(out);
        display(root->_right(), out);
    }

    static void debugDisplay(Node<T>* root, size_t level = 1)
    {
        if (!root) return;
        if (root->isLeaf()) return;

        std::cout << "LVL " << level;

        if (level < 10) std::cout << "  : ";
        else std::cout << " : ";

        //True if a left item was displayed
        bool left = false;

        if (root->_left())
        {
            left = true;
            root->_left()->display(std::cout);
        }

        if (root->_right())
        {
            if (left) std::cout << ", ";
            root->_right()->display(std::cout);
        }

        std::cout << '\n';

        debugDisplay(root->_left(), level + 1);
        debugDisplay(root->_right(), level + 1);
    }
};

#endif // AVL_HPP
