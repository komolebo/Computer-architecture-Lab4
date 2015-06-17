#include "../serialization/serialization.h"

#define BOOST_TEST_DYN_LINK        // this is optional
#define BOOST_TEST_MODULE Boooooooooost   // specify the name of your test module
#include <boost/test/included/unit_test.hpp>  // include this to get main()

// function for find some substring in string
bool testSubstr(string str, string sub)
{
    return str.find(sub) < str.length() - 1;
}

/* create first test case template
   checking our function for finding all substring in string */
BOOST_AUTO_TEST_CASE(test_myCount)
{
        BOOST_CHECK_MESSAGE(myCount("ma", "mamay") == 2, "1st subtest failed");
        BOOST_CHECK_MESSAGE(myCount("m", "mamaym") == 3, "2nd subtest failed");
        BOOST_CHECK_MESSAGE(myCount("o", "lolypop") == 2, "3rd subtest failed");
        BOOST_CHECK_MESSAGE(myCount("", "mamay") == 0, "4th subtest failed");
}

/* create second test case template
   checking our function for finding our parameters in serialize data */
BOOST_AUTO_TEST_CASE(test_serialize)
{
        BOOST_CHECK_MESSAGE(testSubstr(serialize("polo", "o"), "4"), "1st subtest failed");
        BOOST_CHECK_MESSAGE(testSubstr(serialize("polo", "o"), "polo"), "2nd subtest failed");
        BOOST_CHECK_MESSAGE(testSubstr(serialize("polo", "o"), "1"), "3rd subtest failed");
        BOOST_CHECK_MESSAGE(testSubstr(serialize("polo", "o"), "o"), "4th subtest failed");
}