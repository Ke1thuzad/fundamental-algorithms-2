#include "../include/bst.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>

TEST(BSTTest, InsertAndSize) {
    MyMap<int, std::string, BST> map;
    EXPECT_EQ(map.size(), 0);

    map.insert(1, "one");
    EXPECT_EQ(map.size(), 1);

    map.insert(2, "two");
    EXPECT_EQ(map.size(), 2);

    map.insert(1, "new_one");
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(*map.find(1), "new_one");
}

TEST(BSTTest, FindAndContains) {
    MyMap<int, std::string, BST> map;
    map.insert(3, "three");

    EXPECT_NE(map.find(3), nullptr);
    EXPECT_EQ(*map.find(3), "three");
    EXPECT_EQ(map.find(5), nullptr);

    EXPECT_TRUE(map.contains(3));
    EXPECT_FALSE(map.contains(5));
}

TEST(BSTTest, Remove) {
    MyMap<int, std::string, BST> map;
    map.insert(1, "one");
    map.insert(2, "two");

    EXPECT_TRUE(map.remove(1));
    EXPECT_EQ(map.size(), 1);
    EXPECT_FALSE(map.contains(1));

    EXPECT_FALSE(map.remove(5));
}

TEST(BSTTest, Clear) {
    MyMap<int, std::string, BST> map;
    map.insert(1, "one");
    map.insert(2, "two");

    map.clear();
    EXPECT_EQ(map.size(), 0);
    EXPECT_FALSE(map.contains(1));
}

TEST(BSTTest, InorderTraversal) {
    MyMap<int, std::string, BST> map;
    map.insert(3, "3");
    map.insert(1, "1");
    map.insert(4, "4");
    map.insert(2, "2");

    std::vector<int> keys;
    map.inorder([&keys](const int& k, std::string&) {
        keys.push_back(k);
    });

    EXPECT_EQ(keys, (std::vector<int>{1, 2, 3, 4}));
}

TEST(BSTTest, PreorderTraversal) {
    MyMap<int, std::string, BST> map;
    map.insert(3, "3");
    map.insert(1, "1");
    map.insert(4, "4");
    map.insert(2, "2");

    std::vector<int> keys;
    map.preorder([&keys](const int& k, std::string&) {
        keys.push_back(k);
    });

    EXPECT_EQ(keys, (std::vector<int>{3, 1, 2, 4}));
}

TEST(BSTTest, PostorderTraversal) {
    MyMap<int, std::string, BST> map;
    map.insert(3, "3");
    map.insert(1, "1");
    map.insert(4, "4");
    map.insert(2, "2");

    std::vector<int> keys;
    map.postorder([&keys](const int& k, std::string&) {
        keys.push_back(k);
    });

    EXPECT_EQ(keys, (std::vector<int>{2, 1, 4, 3}));
}

TEST(BSTTest, Iterator) {
    MyMap<int, std::string, BST> map;
    map.insert(3, "3");
    map.insert(1, "1");
    map.insert(4, "4");

    auto it = map.begin();
    EXPECT_EQ((*it).first, 1);
    ++it;
    EXPECT_EQ((*it).first, 3);
    it++;
    EXPECT_EQ((*it).first, 4);
    ++it;
    EXPECT_EQ(it, map.end());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}