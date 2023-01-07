#include <gtest/gtest.h>
#include "any.hpp"
#include <vector>

using namespace sphere;

class TestAny: public testing::Test {};

TEST_F(TestAny, BadCast)
{
    try {
        throw bad_any_cast();
    }
    catch (const bad_any_cast &e) {
        ASSERT_EQ(e.what(), "bad any_cast");
    }
}

TEST_F(TestAny, InitEmpty)
{
    any a;
    ASSERT_EQ(a.has_value(), false);
    ASSERT_THROW(any_cast<double>(a), bad_any_cast);
    ASSERT_EQ(any_cast<int>(&a), nullptr);
}

TEST_F(TestAny, InitRvalue)
{
    any a(12.5);
    ASSERT_EQ(a.has_value(), true);
    ASSERT_NO_THROW(any_cast<double>(a));
    ASSERT_EQ(any_cast<double>(a), 12.5);
    ASSERT_THROW(any_cast<std::string>(a), bad_any_cast);
    ASSERT_EQ(any_cast<int>(&a), nullptr);
}

TEST_F(TestAny, InitLvalue)
{
    std::string str = "meow";
    any a(str);
    ASSERT_EQ(a.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(a));
    ASSERT_EQ(any_cast<std::string>(a), std::string("meow"));
    ASSERT_THROW(any_cast<bool>(a), bad_any_cast);
}

TEST_F(TestAny, InitCopy)
{
    std::string str = "meow";
    any a(str);
    any b(a);
    // check that a is still correct
    ASSERT_EQ(a.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(a));
    ASSERT_EQ(any_cast<std::string>(a), std::string("meow"));
    ASSERT_THROW(any_cast<bool>(a), bad_any_cast);

    ASSERT_EQ(b.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(b));
    ASSERT_EQ(any_cast<std::string>(b), std::string("meow"));
    ASSERT_THROW(any_cast<bool>(b), bad_any_cast);
}

TEST_F(TestAny, InitMove)
{
    std::string str = "meow";
    any a(str);
    any b(std::move(a));

    ASSERT_EQ(b.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(b));
    ASSERT_EQ(any_cast<std::string>(b), std::string("meow"));
    ASSERT_THROW(any_cast<bool>(b), bad_any_cast);
}

TEST_F(TestAny, Assigments)
{
    any a = std::string("meow");
    any b = a;
    // check that a is correct
    ASSERT_EQ(a.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(a));
    ASSERT_EQ(any_cast<std::string>(a), std::string("meow"));
    ASSERT_THROW(any_cast<bool>(a), bad_any_cast);

    ASSERT_EQ(b.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(b));
    ASSERT_EQ(any_cast<std::string>(b), std::string("meow"));
    ASSERT_THROW(any_cast<bool>(b), bad_any_cast);

    b = std::move(a);
    a = std::string("not meow");

    ASSERT_EQ(a.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(a));
    ASSERT_EQ(any_cast<std::string>(a), std::string("not meow"));
    ASSERT_THROW(any_cast<int>(a), bad_any_cast);

    ASSERT_EQ(b.has_value(), true);
    ASSERT_NO_THROW(any_cast<std::string>(b));
    ASSERT_EQ(any_cast<std::string>(b), std::string("meow"));
    ASSERT_THROW(any_cast<std::vector<float>>(b), bad_any_cast);
}

TEST_F(TestAny, Reset)
{
    any a = std::string("meow");
    a.reset();
    ASSERT_EQ(a.has_value(), false);
    ASSERT_THROW(any_cast<double>(a), bad_any_cast);
    ASSERT_EQ(any_cast<int>(&a), nullptr);
}

TEST_F(TestAny, NewValue)
{
    const any a = 10;
    const int num = any_cast<int>(a);
    any b = (float)(num * 2);

    ASSERT_EQ(b.has_value(), true);
    ASSERT_NO_THROW(any_cast<float>(b));
    ASSERT_DOUBLE_EQ(any_cast<float>(b), 20.0);
    ASSERT_THROW(any_cast<std::string>(b), bad_any_cast);
}

TEST_F(TestAny, Nullcast)
{
    ASSERT_EQ(any_cast<int>(static_cast<any*>(nullptr)), nullptr);
}

TEST_F(TestAny, Nullptr)
{
    ASSERT_EQ(any_cast<int>((any*)nullptr), nullptr);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
