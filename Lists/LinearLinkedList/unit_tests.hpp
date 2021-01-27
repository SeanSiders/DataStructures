/*
*/

#include "nuttiest/nuttiest.hpp"
using namespace nuttiest;

#ifndef UNIT_TESTS_HPP 
#define UNIT_TESTS_HPP 

template <typename T>
class ListTests : public lll::List<T>
{
    public:

    ListTests()
    {
        section("EMPTY LIST")
        {
            emptyCase();
        }

        section("LIST OF 1")
        {
            section("insertFront")
            {
                this->insertFront(1);
                unit_test("head is tail")
                {
                    assert_eq(this->head, this->tail);
                }

                unit_test("head data is 1")
                {
                    assert_eq(*this->head->_data(), 1);
                }

                unit_test("length is 1")
                {
                    assert_eq(this->length(), 1);
                }

                this->clear();
            }

            section("insertBack")
            {
                this->insertBack(1);
                unit_test("head is tail")
                {
                    assert_eq(this->head, this->tail);
                }

                unit_test("head data is 1")
                {
                    assert_eq(*this->head->_data(), 1);
                }

                unit_test("length is 1")
                {
                    assert_eq(this->length(), 1);
                }

                this->clear();
            }

            section("removeAt(0)")
            {
                this->insertFront(1);
                T removed;
                this->removeAt(0, removed);
                unit_test("retrieved removal")
                {
                    assert_eq(removed, 1);
                }
                
                emptyCase();
            }

            section("remove(1)")
            {
                this->insertFront(1);
                size_t removeCount = this->remove(1);
                unit_test("removed 1 item")
                {
                    assert_eq(removeCount, 1);
                }

                emptyCase();
            }

            section("retreive(1)")
            {
                this->insertFront(1);
                T retrieved;
                unit_test("retrieve(1) is true")
                {
                    assert_eq(this->retrieve(1, retrieved), true);
                }

                unit_test("retrieved 1")
                {
                    assert_eq(retrieved, 1);
                }

                this->clear();
            }
        }

        section("LIST OF 2")
        {
            section("insertFront")
            {
                this->insertFront(2);
                this->insertFront(1);
                unit_test("head is not tail")
                {
                    assert_ne(this->head, this->tail);
                }

                unit_test("head data is 1")
                {
                    assert_eq(*this->head->_data(), 1);
                }

                unit_test("tail data is 2")
                {
                    assert_eq(*this->tail->_data(), 2);
                }

                unit_test("length is 2")
                {
                    assert_eq(this->length(), 2);
                }

                this->clear();
            }

            section("insertBack")
            {
                this->insertBack(1);
                this->insertBack(2);
                unit_test("head is not tail")
                {
                    assert_ne(this->head, this->tail);
                }

                unit_test("head data is 1")
                {
                    assert_eq(*this->head->_data(), 1);
                }

                unit_test("tail data is 2")
                {
                    assert_eq(*this->tail->_data(), 2);
                }

                unit_test("length is 2")
                {
                    assert_eq(this->length(), 2);
                }

                this->clear();
            }

            section("removeAt(0) [retrieval]")
            {
                this->insertBack(1);
                this->insertBack(2);
                T removed;
                this->removeAt(0, removed);
                unit_test("retrieved removal")
                {
                    assert_eq(removed, 1);
                }

                unit_test("head is 2")
                {
                    assert_eq(*this->head->_data(), 2);
                }
                
                oneListCase();

                this->clear();
            }

            section("removeAt(0)")
            {
                this->insertBack(1);
                this->insertBack(2);
                this->removeAt(0);
                unit_test("head is 2")
                {
                    assert_eq(*this->head->_data(), 2);
                }

                oneListCase();

                this->clear();
            }
            section("removeAt(1) [retrieval]")
            {
                this->insertBack(1);
                this->insertBack(2);
                T removed;
                this->removeAt(1, removed);
                unit_test("retrieved removal")
                {
                    assert_eq(removed, 2);
                }

                unit_test("head is 1")
                {
                    assert_eq(*this->head->_data(), 1);
                }

                oneListCase();

                this->clear();
            }
            
            section("removeAt(1)")
            {
                this->insertBack(1);
                this->insertBack(2);
                this->removeAt(1);
                unit_test("head is 1")
                {
                    assert_eq(*this->head->_data(), 1);
                }

                oneListCase();

                this->clear();
            }

            section("remove(1)")
            {
                this->insertBack(1);
                this->insertBack(2);
                size_t removeCount = this->remove(1);
                unit_test("removed 1 item")
                {
                    assert_eq(removeCount, 1);
                }

                oneListCase();
            }

            section("retreive")
            {
                this->insertBack(1);
                this->insertBack(2);
                T retrieved;
                unit_test("retrieve(1) is true")
                {
                    assert_eq(this->retrieve(1, retrieved), true);
                }

                unit_test("retrieved 1")
                {
                    assert_eq(retrieved, 1);
                }

                unit_test("retrieve(2) is true")
                {
                    assert_eq(this->retrieve(2, retrieved), true);
                }

                unit_test("retrieved 2")
                {
                    assert_eq(retrieved, 2);
                }

                this->clear();
            }
        }

        section("LIST OF 10")
        {
            makeTenList();

            unit_test("head is  52")
            {
                assert_eq(*this->head->_data(), 52);
            }

            unit_test("tail is 10")
            {
                assert_eq(*this->tail->_data(), 10);
            }

            unit_test("length is 10")
            {
                assert_eq(this->length(), 10);
            }

            section("remove at head [retrieval]")
            {
                T removed;
                this->removeAt(0, removed);
                unit_test("removed is 52")
                {
                    assert_eq(removed, 52);
                }

                unit_test("head is 34")
                {
                    assert_eq(*this->head->_data(), 34);
                }

                unit_test("length is 9")
                {
                    assert_eq(this->length(), 9);
                }

                this->clear();
            }

            section("remove at head")
            {
                makeTenList();
                this->removeAt(0);

                unit_test("head is 34")
                {
                    assert_eq(*this->head->_data(), 34);
                }

                unit_test("length is 9")
                {
                    assert_eq(this->length(), 9);
                }

                this->clear();
            }

            section("remove at tail [retrieval]")
            {
                makeTenList();
                T removed;
                this->removeAt(9, removed);

                unit_test("removed is 10")
                {
                    assert_eq(removed, 10);
                }

                unit_test("tail is 99")
                {
                    assert_eq(*this->tail->_data(), 99);
                }

                unit_test("length is 9")
                {
                    assert_eq(this->length(), 9);
                }

                this->clear();
            }

            section("remove at tail")
            {
                makeTenList();
                this->removeAt(9);

                unit_test("tail is 99")
                {
                    assert_eq(*this->tail->_data(), 99);
                }

                unit_test("length is 9")
                {
                    assert_eq(this->length(), 9);
                }

                this->clear();
            }

            section("removeAt(5) [retrieval]")
            {
                makeTenList();
                T removed;
                this->removeAt(5, removed);

                unit_test("removed is 41")
                {
                    assert_eq(removed, 41);
                }

                unit_test("length is 9")
                {
                    assert_eq(this->length(), 9);
                }

                this->clear();
            }

            section("removeAt(5)")
            {
                makeTenList();
                this->removeAt(5);

                unit_test("length is 9")
                {
                    assert_eq(this->length(), 9);
                }
                this->clear();
            }
        }

        summary();
    }

    private:

    //Used in |nullptr| comparisons
    lll::Node<T>* null = nullptr;

    void emptyCase()
    {
            unit_test("head is null")
            {
                assert_eq(this->head, null);
            }

            unit_test("tail is null")
            {
                assert_eq(this->tail, null);
            }

            unit_test("length is 0")
            {
                assert_eq(this->listLength, 0);
            }
    }

    void oneListCase()
    {
            unit_test("head is tail")
            {
                assert_eq(this->head, this->tail);
            }

            unit_test("length is 1")
            {
                assert_eq(this->length(), 1);
            }
    }

    void makeTenList()
    {
        this->insert(52);
        this->insert(34);
        this->insert(79);
        this->insert(81);
        this->insert(79);
        this->insert(41);
        this->insert(7);
        this->insert(9);
        this->insert(99);
        this->insert(10);
    }
};

#endif //UNIT_TESTS_HPP
