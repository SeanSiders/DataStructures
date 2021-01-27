#ifndef NUTTIEST_HPP
#define NUTTIEST_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <stdexcept>


namespace nuttiest {
    //=====================================================================
    // Globals & Configuration Variables
    //=====================================================================

    /// Just because I like this syntax, and it reminds me of Rust.
    #define let auto

    /// Comment this to disable colored output.
    #define COLOR_OUTPUT

    /// The stream to which test results will be output.
    static std::ostream& STREAM = std::cout;

    /// Used for __FILE__ instead of creating a std::string every time.
    using  str_literal = const char *;

    //=====================================================================
    // Public API
    //=====================================================================
    /**
    *   Use these macros for your unit testing.
    *   The line numbers and the file name depends on these being macros.
    **/

    /// Prints a new section divider for a logical grouping of tests.
    #define section(name) __section_name = name; _print_section();

    /// Print a summary of all passed and failed tests at the end of your file.
    #define summary() _print_summary()

    /// Set the name of the current test.
    #define unit_test(name) __test_name = name;

    /// Enables exception handling. Test will fail if an exception is thrown.
    #define may_throw(y)\
        try { y; }\
        catch(const std::exception& e) { fail_test(e.what()); }\
        catch(...) { fail_test("Unknown Exception"); }
    
    /// Test will only pass if an exception is thrown. 
    #define must_throw(y) __throw_flag = true;\
        try { y; __throw_flag = false; fail_test("Expected an exception, but none was thrown."); }\
        catch(...) { __throw_flag = false; pass_test(); }

    /// Explicitly pass a test.
    #define pass_test()\
        _explicit_pass(__FILE__, __LINE__);

    /// Explicitly fail a test, providing a reason.
    #define fail_test(reason)\
        _explicit_fail(__FILE__, __LINE__, reason);

    /// Pass if lhs == rhs, otherwise fail.
    #define assert_eq(lhs, rhs)\
        _print_result(_eq::functor(), lhs, rhs, __FILE__, __LINE__);

    /// Pass if lhs != rhs, otherwise fail.
    #define assert_ne(lhs, rhs)\
        _print_result(_ne::functor(), lhs, rhs, __FILE__, __LINE__);

    /// Pass if lhs < rhs, otherwise fail.
    #define assert_lt(lhs, rhs)\
        _print_result(_lt::functor(), lhs, rhs, __FILE__, __LINE__);

    /// Pass if lhs <= rhs, otherwise fail.
    #define assert_le(lhs, rhs)\
        _print_result(_le::functor(), lhs, rhs, __FILE__, __LINE__);

    /// Pass if lhs > rhs, otherwise fail.
    #define assert_gt(lhs, rhs)\
        _print_result(_gt::functor(), lhs, rhs, __FILE__, __LINE__);

    /// Pass if lhs >= rhs, otherwise fail.
    #define assert_ge(lhs, rhs)\
        _print_result(_ge::functor(), lhs, rhs, __FILE__, __LINE__);

    /// Pass if two sections of memory are equal for num_bytes, otherwise fail.
    #define mem_eq(lhs, rhs, num_bytes)\
        _print_result(_eq::functor(), lhs, rhs, num_bytes, __FILE__, __LINE__);

    /// Pass if two sections of memory are not equal for num_bytes, otherwise fail.
    #define mem_ne(lhs, rhs, num_bytes)\
        _print_result(_ne::functor(), lhs, rhs, num_bytes, __FILE__, __LINE__);

    /// Pass if memory of lhs < memory of rhs up to num_bytes, otherwise fail.
    #define mem_lt(lhs, rhs, num_bytes)\
        _print_result(_lt::functor(), lhs, rhs, num_bytes, __FILE__, __LINE__);

    /// Pass if memory of lhs <= memory of rhs up to num_bytes, otherwise fail.
    #define mem_le(lhs, rhs, num_bytes)\
        _print_result(_le::functor(), lhs, rhs, num_bytes, __FILE__, __LINE__);

    /// Pass if memory of lhs > memory of rhs up to num_bytes, otherwise fail.
    #define mem_gt(lhs, rhs, num_bytes)\
        _print_result(_gt::functor(), lhs, rhs, num_bytes, __FILE__, __LINE__);

    /// Pass if memory of lhs >= memory of rhs up to num_bytes, otherwise fail.
    #define mem_ge(lhs, rhs, num_bytes)\
        _print_result(_ge::functor(), lhs, rhs, num_bytes, __FILE__, __LINE__);


    //=====================================================================
    // Forward Declarations For Helper Functions
    //=====================================================================
    /**
    *   These are public because the macros need them to be.
    *   Use the macros instead of these functions.
    **/
    
    /// Sets and prints the name of the section
    static void _print_section(std::ostream& stream = STREAM);

    /// Prints a summary of all passed/failed tests.
    static int _print_summary(std::ostream& stream = STREAM);

    /// The different types of comparisons available.
    enum Comparison {
        Equal,
        NotEqual,
        LessThan,
        LessOrEqual,
        GreaterThan,
        GreaterOrEqual,
    };

    /// Print the result of a comparison.
    template <typename Functor, typename L, typename R>
    static void _print_result(
        const Functor&,
        const L&,
        const R&,
        str_literal,
        const size_t&,
        std::ostream& = STREAM
    );

    /// Print the result of a memory comparison.
    template <typename Functor>
    static void _print_result(
        const Functor&,
        const void * const,
        const void * const,
        const size_t&,
        str_literal,
        const size_t&,
        std::ostream& = STREAM
    );

    /// Explicitly passes a test.
    static void _explicit_pass(
        str_literal file_name,
        const size_t& line_number,
        std::ostream& stream = STREAM
    );

    /// Explicitly fails a test.
    static void _explicit_fail(
        str_literal file_name,
        const size_t& line_number,
        const std::string& reason,
        std::ostream& stream = STREAM
    );

    //=====================================================================
    // Singleton Comparison Functors
    //=====================================================================

    /**
    *   Singleton functor for comparing (lhs == rhs). Can be retrieved via _eq::functor().
    *   Do not use this class directly. Use the assert_eq(lhs, rhs) macro.
    **/
    class _eq {
        _eq(){}
    public:
        _eq(const _eq&) = delete;
        void operator=(const _eq&) = delete;
        static constexpr Comparison COMPARISON_TYPE { Comparison::Equal };
        static const _eq& functor() {
            static _eq _functor;
            return _functor;
        }
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const {
            return lhs == rhs;
        }
        bool operator()(void const * const lhs, void const * const rhs, const size_t & num_bytes) const {
            return 0 == memcmp(lhs, rhs, num_bytes);
        }
    };

    /**
    *   Singleton functor for comparing (lhs < rhs). Can be retrieved via _lt::functor().
    *   Do not use this class directly. Use the assert_lt(lhs, rhs) macro.
    **/
    class _lt {
        _lt(){}
    public:
        _lt(const _lt&) = delete;
        void operator=(const _lt&) = delete;
        static constexpr Comparison COMPARISON_TYPE { Comparison::LessThan };
        static const _lt& functor() {
            static _lt _functor;
            return _functor;
        }
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const {
            return lhs < rhs;
        }
        bool operator()(void const * const lhs, void const * const rhs, const size_t & num_bytes) const {
            return 0 > memcmp(lhs, rhs, num_bytes);
        }
    };

    /**
    *   Singleton functor for comparing (lhs > rhs). Can be retrieved via _gt::functor().
    *   Do not use this class directly. Use the assert_gt(lhs, rhs) macro.
    **/
    class _gt {
        _gt(){}
    public:
        _gt(const _gt&) = delete;
        void operator=(const _gt&) = delete;
        static constexpr Comparison COMPARISON_TYPE { Comparison::GreaterThan };
        static const _gt& functor() {
            static _gt _functor;
            return _functor;
        }
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const {
            return lhs > rhs;
        }
        bool operator()(void const * const lhs, void const * const rhs, const size_t & num_bytes) const {
            return 0 < memcmp(lhs, rhs, num_bytes);
        }
    };

    /**
    *   Singleton functor for comparing (lhs != rhs). Can be retrieved via _ne::functor().
    *   Do not use this class directly. Use the assert_ne(lhs, rhs) macro.
    **/
    class _ne {
        _ne(){}
    public:
        _ne(const _ne&) = delete;
        void operator=(const _ne&) = delete;
        static constexpr Comparison COMPARISON_TYPE { Comparison::NotEqual };
        static const _ne& functor() {
            static _ne _functor;
            return _functor;
        }
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const {
            return lhs != rhs;
        }
        bool operator()(void const * const lhs, void const * const rhs, const size_t & num_bytes) const {
            return 0 != memcmp(lhs, rhs, num_bytes);
        }
    };

    /**
    *   Singleton functor for comparing (lhs <= rhs). Can be retrieved via _le::functor().
    *   Do not use this class directly. Use the assert_le(lhs, rhs) macro.
    **/
    class _le {
        _le(){}
    public:
        _le(const _le&) = delete;
        void operator=(const _le&) = delete;
        static constexpr Comparison COMPARISON_TYPE { Comparison::LessOrEqual };
        static const _le& functor() {
            static _le _functor;
            return _functor;
        }
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const {
            return lhs <= rhs;
        }
        bool operator()(void const * const lhs, void const * const rhs, const size_t & num_bytes) const {
            return 0 >= memcmp(lhs, rhs, num_bytes);
        }
    };

    /**
    *   Singleton functor for comparing (lhs >= rhs). Can be retrieved via _ge::functor().
    *   Do not use this class directly. Use the assert_ge(lhs, rhs) macro.
    **/
    class _ge {
        _ge(){}
    public:
        _ge(const _ge&) = delete;
        void operator=(const _ge&) = delete;
        static constexpr Comparison COMPARISON_TYPE { Comparison::GreaterOrEqual };
        static const _ge& functor() {
            static _ge _functor;
            return _functor;
        }
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const {
            return lhs >= rhs;
        }
        bool operator()(void const * const lhs, void const * const rhs, const size_t & num_bytes) const {
            return 0 <= memcmp(lhs, rhs, num_bytes);
        }
    };

    /**
    *   A class to encapsulate everything that actually can be, and should be private.
    *   Most importantly, this includes the count of passed tests and failed tests.
    *   Unfortunately, none of these macros are actually private, which is why they begin with
    *   a double underscore. Do not use them. use the macros in the Public API. 
    **/
    class Private {
        //=========================
        // Macros and Data Members
        //=========================
        #define __S_LINE    "________________________________________"
        #define __H_LINE    "==================================================================\n"
        #define __FILE_INFO "[" << file_name << " @ " << std::setw(4) << line_number << "]"

        #define __UP_ARROWS   "^\n^\n^\n"
        #define __DOWN_ARROWS "\nv\nv\nv\n"

        #define __PASS_TEST "\t" << green("[PASS]") << " ==> " << __section_name << "/" << __test_name
        #define __FAIL_TEST "\t" <<   red("[FAIL]") << " ==> " << __section_name << "/" << __test_name << std::endl

        #define __INDICATE_UP   __H_LINE      << __UP_ARROWS
        #define __INDICATE_DOWN __DOWN_ARROWS << __H_LINE

        #define __comparison_message(expectation)\
            return stream << "[reason]: expected " << expectation << "\n"\
                          << "[lhs]: " << lhs      << "\n"\
                          << "[rhs]: " << rhs      << "\n"
        #define __memcmp_message(expectation)\
            return stream << "[reason]: expected memcmp(lhs, rhs) " << expectation << "\n"\
                          << "[memcmp(lhs, rhs)]: " << memcmp(lhs, rhs, num_bytes) << "\n"
        
        #ifdef COLOR_OUTPUT
            #define green(str) "\033[1;32m" << str << "\033[0m"
            #define   red(str) "\033[1;31m" << str << "\033[0m"
        #else
            #define green(str) str
            #define   red(str) str
        #endif

        static size_t passed_tests;
        static size_t failed_tests;

        //=========================
        // Friend Functions
        //=========================
        
        friend int _print_summary(std::ostream&);

        friend void _explicit_pass(
            str_literal,
            const size_t&,
            std::ostream&
        );

        friend void _explicit_fail(
            str_literal,
            const size_t&,
            const std::string&,
            std::ostream&
        );

        template <typename Functor, typename L, typename R>
        friend void _print_result(
            const Functor&,
            const L&,
            const R&,
            str_literal,
            const size_t&,
            std::ostream&
        );

        template <typename Functor>
        friend void _print_result(
            const Functor&,
            const void * const,
            const void * const,
            const size_t&,
            str_literal,
            const size_t&,
            std::ostream&
        );

        /**
        *   Prints verbose information if a test fails.
        *   Used for all tests invoked by the macros assert_*()
        **/ 
        template <typename Functor, typename L, typename R>
        static std::ostream& verbose_output(
            const Functor& functor, 
            const L& lhs, const R& rhs, 
            std::ostream& stream = STREAM)
        {
            switch(Functor::COMPARISON_TYPE) {
                case Comparison::Equal:          __comparison_message("lhs == rhs");
                case Comparison::NotEqual:       __comparison_message("lhs != rhs");
                case Comparison::LessThan:       __comparison_message("lhs < rhs" );
                case Comparison::LessOrEqual:    __comparison_message("lhs <= rhs");
                case Comparison::GreaterThan:    __comparison_message("lhs > rhs" );
                case Comparison::GreaterOrEqual: __comparison_message("lhs >= rhs");
            }
            return stream;
        }

        /**
        *   Prints verbose information if a test fails.
        *   Used for all tests invoked by the macros mem_*()
        **/ 
        template <typename Functor>
        static std::ostream& verbose_output(
            const Functor& functor, 
            void const * const lhs, 
            void const * const rhs, 
            const size_t& num_bytes, 
            std::ostream& stream = STREAM)
        {
            switch(Functor::COMPARISON_TYPE) {
                case Comparison::Equal:          __memcmp_message("== 0");
                case Comparison::NotEqual:       __memcmp_message("!= 0");
                case Comparison::LessThan:       __memcmp_message( "< 0");
                case Comparison::LessOrEqual:    __memcmp_message("<= 0");
                case Comparison::GreaterThan:    __memcmp_message( "> 0");
                case Comparison::GreaterOrEqual: __memcmp_message(">= 0");
            }
            return stream;
        }
    };

    size_t Private::passed_tests { 0 };
    size_t Private::failed_tests { 0 };

    /// The name of the test: __section_name/__test_name
    static std::string __test_name { "undeclared test" };

    /// The name of the section: __section_name/__test_name
    static std::string __section_name { "undeclared section" };

    /// If set to true, test may pass only if it throws.
    static bool __throw_flag = false;

    /// Prints a new section divider for a logical grouping of tests.
    static void _print_section(std::ostream & stream) {
        stream << __S_LINE << "\n\nTesting { " << __section_name << " }\n" << __S_LINE << std::endl << std::endl;
    }

    /// Print a _print_summary of all passed and failed tests at the end of your file.
    static int _print_summary(std::ostream & stream) {
        auto total_tests = Private::passed_tests + Private::failed_tests;
        auto digits = std::to_string(total_tests).length();
        stream << "\n\n======================RESULTS======================\n" << std::endl
               << "[TOTAL]:  { " << std::setw(digits) << Private::passed_tests + Private::failed_tests << " }" << std::endl
               << "[PASSED]: { " << green(std::setw(digits) << Private::passed_tests) << " }" << std::endl
               << "[FAILED]: { ";
               (!Private::failed_tests 
                    ? stream << green(std::setw(digits) << Private::failed_tests)
                    : stream <<   red(std::setw(digits) << Private::failed_tests)
               );
               stream << " }" << std::endl << std::endl;
        return 0;
    }

    /// Explicitly passes a test.
    static void _explicit_pass(
        str_literal file_name,
        const size_t& line_number,
        std::ostream& stream)
    {
        if (__throw_flag) { return; }
        ++Private::passed_tests;
        stream << __FILE_INFO << __PASS_TEST << std::endl;
    }

    /// Explicitly fails a test, providing a reason.
    static void _explicit_fail(
        str_literal file_name,
        const size_t& line_number,
        const std::string& reason,
        std::ostream& stream)
    {
        if (__throw_flag) { return; }
        ++Private::failed_tests;
        stream << std::endl;
        stream << __INDICATE_DOWN;
        stream << __FILE_INFO;
        stream << __FAIL_TEST;
        stream << "[reason]: " << reason << "\n";
        stream << __INDICATE_UP << std::endl;
    }

    /// Prints the result of a regular (assert_*()) comparison test.
    template <typename Functor, typename L, typename R>
    static void _print_result(
        const Functor& functor,
        const L& lhs,
        const R& rhs,
        str_literal file_name,
        const size_t& line_number,
        std::ostream& stream) 
    {
        if (functor(lhs, rhs)) {
            if (__throw_flag) { return; }
            ++Private::passed_tests;
            stream << __FILE_INFO << __PASS_TEST;
        } else {
            if (__throw_flag) { return; }
            ++Private::failed_tests;
            stream << __INDICATE_DOWN;
            stream << __FILE_INFO;
            stream << __FAIL_TEST;
            Private::verbose_output(functor, lhs, rhs, stream);
            stream << __INDICATE_UP;
        }
        stream << std::endl;
    }

    /// Prints the result of a memory (mem_*()) comparison test.
    template <typename Functor>
    static void _print_result(
        const Functor& functor,
        void const * const lhs,
        void const * const rhs,
        const size_t& num_bytes,
        str_literal file_name,
        const size_t& line_number,
        std::ostream& stream) 
    {
        if (functor(lhs, rhs, num_bytes)) {
            if (__throw_flag) { return; }
            ++Private::passed_tests;
            stream << __FILE_INFO << __PASS_TEST;
        } else {
            if (__throw_flag) { return; }
            ++Private::failed_tests;
            stream << __INDICATE_DOWN;
            stream << __FILE_INFO;
            stream << __FAIL_TEST;
            Private::verbose_output(functor, lhs, rhs, num_bytes, stream);
            stream << __INDICATE_UP;
        }
        stream << std::endl;
    }
}

#endif // NUTTIEST_HPP
