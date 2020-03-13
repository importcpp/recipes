#include <vector>
using namespace std;

// version 1
class UFV1
{
private:
    int count_;          // 记录连通分量
    vector<int> parent_; // 节点x的父节点是 parent_[x]

    /* 构造函数，n 为图的节点总数 */
public:
    UFV1(int n)
    {
        // 一开始互不连通
        count_ = n;
        // 父节点指针初始指向自己
        parent_.resize(n);
        for (int i = 0; i < n; i++)
        {
            parent_[i] = i;
        }
    }

    // 其他函数

    // 如果某两个节点被连通，则让其中的（任意）一个节点的根节点接到另一个节点的根节点上
    void merge(int p, int q)
    {
        int root_p = find(p);
        int root_q = find(q);
        if (root_p == root_q)
        {
            return;
        }
        // 将两棵树合并为一棵
        parent_[root_p] = root_q;
        // parent[root_q] = root_p 也一样
        --count_; // 两个分量合二为一
    }

    /* 返回某个节点 x 的根节点 */
    int find(int x)
    {
        // 根节点的 parent[x] == x
        while (parent_[x] != x)
        {
            x = parent_[x];
        }
        return x;
    }

    // 如果节点p和q连通的话，它们一定拥有相同的根节点
    bool connected(int p, int q)
    {
        int root_p = find(p);
        int root_q = find(q);
        return root_p == root_q;
    }
};

// version 2
class UFV2
{
private:
    int count_;          // 记录连通分量
    vector<int> parent_; // 节点x的父节点是 parent_[x]
    vector<int> size_;   // 新增一个数组记录树的“重量”

    /* 构造函数，n 为图的节点总数 */
public:
    UFV2(int n)
    {
        // 一开始互不连通
        count_ = n;
        // 父节点指针初始指向自己
        parent_.resize(n);
        size_.resize(n);
        for (int i = 0; i < n; i++)
        {
            parent_[i] = i;
            size_[i] = 1;
        }
    }

    // 其他函数

    // 如果某两个节点被连通，则让其中的（任意）一个节点的根节点接到另一个节点的根节点上
    // 优化merge过程，小一些的树接到大一些的树下面，这样就能避免头重脚轻，更平衡一些
    void merge(int p, int q)
    {
        int root_p = find(p);
        int root_q = find(q);
        if (root_p == root_q)
        {
            return;
        }
        // 将两棵树合并为一棵
        if (size_[p] > size_[q])
        {
            parent_[root_q] = root_p;
            size_[root_p] += size_[root_q];
        }
        else
        {
            parent_[root_p] = root_q;
            size_[root_q] += size_[root_p];
        }
        --count_; // 两个分量合二为一
    }

    /* 返回某个节点 x 的根节点 */
    int find(int x)
    {
        // 根节点的 parent[x] == x
        while (parent_[x] != x)
        {
            // important 路径压缩
            parent_[x] = parent_[parent_[x]];
            x = parent_[x];
        }
        return x;
    }

    // 如果节点p和q连通的话，它们一定拥有相同的根节点
    bool connected(int p, int q)
    {
        int root_p = find(p);
        int root_q = find(q);
        return root_p == root_q;
    }

    int count()
    {
        return count_;
    }
};