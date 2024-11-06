#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test) {
    int i = 1;
    BOOST_TEST(i);
    i = 2;
    BOOST_TEST(i == 2);
    
}