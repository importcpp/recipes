#include "RBTree.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

void testCreate()
{
    RBTree *rbtree = new RBTree();
    delete rbtree;
    rbtree = nullptr;
}

void testRandomData(int threshold, int print_flag)
{
    RBTree *rbtree = new RBTree();

    /// ======= 随机测试 ======== ///
    std::vector<int> rand_inta(threshold, 0);
    for (int i = 1; i <= threshold; ++i)
    {
        rand_inta[i] = i;
    }
    std::random_shuffle(rand_inta.begin(), rand_inta.end());

    for (auto &i : rand_inta)
    {
        rbtree->insert(i);
        if (print_flag > 0)
            rbtree->printTree();
        rbtree->verifyProperties();
    }

    std::random_shuffle(rand_inta.begin(), rand_inta.end());

    for (auto &i : rand_inta)
    {
        rbtree->deleteValue(i);
        if (print_flag > 0)
            rbtree->printTree();
        rbtree->verifyProperties();
    }

    delete rbtree;
    rbtree = nullptr;
}

void testPrepareData(int threshold, int print_flag)
{
    RBTree *rbtree = new RBTree();

    /// ======= 顺序测试 ======== ///

    int half_thresh = (threshold / 2);

    for (int i = 1; i <= half_thresh; ++i)
    {
        rbtree->insert(i);
        if (print_flag > 0)
            rbtree->printTree();
        rbtree->verifyProperties();
    }

    for (int i = threshold; i > half_thresh; --i)
    {
        rbtree->insert(i);
        if (print_flag > 0)
            rbtree->printTree();
        rbtree->verifyProperties();
    }

    for (int i = 1; i <= half_thresh; ++i)
    {
        rbtree->deleteValue(i);
        if (print_flag > 0)
            rbtree->printTree();
        rbtree->verifyProperties();
    }

    for (int i = threshold; i > half_thresh; --i)
    {
        rbtree->deleteValue(i);
        if (print_flag > 0)
            rbtree->printTree();
        rbtree->verifyProperties();
    }

    //delete rbtree;
    //rbtree = nullptr;
}

int main(int argc, char *argv[])
{
    testCreate();
    int threshold = 12;
    int print_flag = 1;
    if (argc > 1)
    {
        threshold = std::atoi(argv[1]);
    }
    if (argc > 2)
    {
        print_flag = std::atoi(argv[2]);
    }

    testPrepareData(threshold, print_flag);

    testRandomData(threshold, print_flag);

    return 0;
}