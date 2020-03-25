// borrow lots of code from wikipedia and the following link:
// https://web.archive.org/web/20140329042423/http://en.literateprograms.org/Special:DownloadCode/Red-black_tree_(C)
#include <iostream>
#include <assert.h>
#include <string>
#include <queue>
#include <vector>
using namespace std;

enum color_t
{
    BLACK,
    RED
};

struct Node
{
    // 需要包含 父节点 儿子节点的地址 还有自身的颜色信息，还有自身储存的数据
    int value;
    color_t color;
    Node *parent, *left, *right;

    Node() : value(0), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

    // 必要的属性函数，用于获取“亲属”节点
    Node *getGrandParent() { return parent == nullptr ? nullptr : parent->parent; }
    Node *getSibling()
    {
        if (parent == nullptr)
        {
            return nullptr;
        }
        return (this == parent->left) ? parent->right : parent->left;
    }
    Node *getUncle() { return (parent == nullptr) ? nullptr : parent->getSibling(); }
};

color_t nodeColor(Node *n) { return n == nullptr ? BLACK : n->color; }

class RBTree
{
private:
    Node *root, *NIL;

public:
    RBTree()
    {
        NIL = new Node();
        NIL->color = BLACK;
        root = NIL;
    }
    ~RBTree()
    {
        if (root != NIL)
        {
            // delete all the tree
            deleteTree(root);
        }

        delete NIL;
        root = nullptr;
        NIL = nullptr;
    }

    void insert(int value)
    {
        std::cout << "Prepare to insert value: " << value << std::endl;
        // 根节点为空直接插入，不为空则搜寻后插入
        if (root == NIL)
        {
            // IMPORTANT: 这里已经讨论了insert case1，就是插入节点为根节点的情况
            // 那么在下面的这个else中还是“有可能"进入case1,这有没有必要呢？
            // 此时把焦点放在insert case3上， insert case3把节点n的grandparent当作新的插入节点
            // 那么便可以得到，n的grandparent有可能就是根节点，
            // 此时，就出现了必须要调用insert case1的情况 
            root = new Node();
            root->color = BLACK;
            root->value = value;
            root->left = root->right = NIL;
        }
        else
        {
            insert(root, value);
        }
        std::cout << "Inserted value: " << value << std::endl;
    }

    bool deleteValue(int value)
    {
        std::cout << "Prepare to delete value: " << value << std::endl;

        Node *child;
        Node *n = lookupNode(value);
        if (n == NIL)
        {
            return false; // 未找到key
        }
        if (n->left != NIL && n->right != NIL)
        {
            // 转化删除问题
            Node *pred = maximumNode(n->left);
            n->value = pred->value;
            n = pred;
        }

        assert(n->left == NIL || n->right == NIL);
        child = ((n->right == NIL) ? n->left : n->right);

        // 将child移动到n的位置
        replaceNode(n, child);

        if (n->color == BLACK)
        {
            if (child->color == RED)
            {
                child->color = BLACK;
            }
            else
            {
                // 进入红黑树的deleteCase函数
                // 所有的deleteCase全是在处理分支上少了一个黑节点的情况
                deleteCase1(child);
            }
        }
        else // n->color == RED
        {
            // 这是当 n->color == RED时，可以不用处理，直接删除节点即可
        }

        // Note: 这里必须释放节点n
        delete n;
        n = nullptr;

        verifyProperties();
        std::cout << "Deleted value: " << value << std::endl;
        return true;
    }

    Node *getRoot() { return root; }

    void printTree()
    {
        // 先求出 rbtree的最大深度-
        int depth = calculateDepth(root);
        std::cout << "Tree depth is :  " << depth << std::endl;

        int one_inst_len = 3;
        std::vector<std::string> ret_string(depth + 1, std::string((((1 << depth) + 1) * one_inst_len), ' '));

        // 再使用层次遍历，记下空格数即可
        struct Info
        {
            /* data */
            int depth;
            int spaceNum;
            Info(int x, int y) : depth(x), spaceNum(y) {}
        };

        int spaceNum = (1 << (depth - 1));
        typedef std::pair<Node *, Info> NNode;
        NNode tmp = std::make_pair(root, Info(depth, spaceNum));
        std::queue<NNode> q;
        q.push(tmp);
        while (!q.empty())
        {
            tmp = q.front();
            q.pop();
            if (tmp.first != NIL)
            {
                // 打印处理
                ret_string[tmp.second.depth][tmp.second.spaceNum * one_inst_len] = (tmp.first->color == RED ? 'R' : 'B');

                string tmp_value_str = std::to_string(tmp.first->value);
                int len = tmp_value_str.size();
                for (int i = 0; i < len; ++i)
                {
                    ret_string[tmp.second.depth][(tmp.second.spaceNum + 1) * one_inst_len - i - 1] = tmp_value_str[len - i - 1];
                }
                // 子节点
                depth = tmp.second.depth - 1;
                if (depth == 0)
                {
                    continue;
                }
                spaceNum = (1 << (depth - 1));
                q.push(std::make_pair(tmp.first->left, Info(depth, tmp.second.spaceNum - spaceNum)));
                q.push(std::make_pair(tmp.first->right, Info(depth, tmp.second.spaceNum + spaceNum)));
            }
        }
        for (int i = ret_string.size() - 1; i >= 0; --i)
        {
            std::cout << ret_string[i] << std::endl;
        }
    }

    // 用来验证属性
    void verifyProperties()
    {
        verify_property_1(root);
        verify_property_2(root);
        /* Property 3 is implicit */
        verify_property_4(root);
        verify_property_5(root);
    }

private:
    void deleteCase1(Node *n)
    {
        if (n->parent != nullptr)
        {
            deleteCase2(n);
        }
        // 下面也是多余的，因为此时n的节点颜色肯定是黑色
        // 留在这里只是notice detetecase1是用于处理节点n可能是根节点的情况
        else
        {
            n->color = BLACK;
        }
    }

    // 黑黑红 - NPS
    // 注意这种情况sibling的颜色为红色时，sibling的子节点肯定不是NIL，
    // 因为在删除一个黑节点前，P的两边的黑节点数肯定不小于1, 所以sibling必定带有黑色子节点
    void deleteCase2(Node *n)
    {
        Node *s = n->getSibling();

        if (s->color == RED)
        {
            n->parent->color = RED;
            s->color = BLACK;
            if (n == n->parent->left)
            {
                rotateLeft(n->parent);
            }
            else
            {
                rotateRight(n->parent);
            }
        }
        deleteCase3(n);
    }
    // 黑黑黑黑黑 - N P S SL SR
    void deleteCase3(Node *n)
    {
        Node *s = n->getSibling();

        if ((n->parent->color == BLACK) && (s->color == BLACK) &&
            (s->left->color == BLACK) && (s->right->color == BLACK))
        {
            s->color = RED;
            deleteCase1(n->parent);
        }
        else
        {
            deleteCase4(n);
        }
    }

    // 黑红黑黑黑 - N P S SL SR
    void deleteCase4(Node *n)
    {
        Node *s = n->getSibling();

        if ((n->parent->color == RED) && (s->color == BLACK) &&
            (s->left->color == BLACK) && (s->right->color == BLACK))
        {
            s->color = RED;
            n->parent->color = BLACK;
        }
        else
        {
            deleteCase5(n);
        }
    }

    void deleteCase5(Node *n)
    {
        // 有问题还需要修改
        Node *s = n->getSibling();

        // This if statement is trivial, due to case 2 (even though case 2 changed
        // the sibling to a sibling's child, the sibling's child can't be red, since
        // no red parent can have a red child).
        assert(s->color == BLACK); // trivial
        if (s->color == BLACK)
        {
            // The following statements just force the red to be on the left of the
            // left of the parent, or right of the right, so case six will rotate
            // correctly.
            if ((n == n->parent->left) && (s->right->color == BLACK) &&
                (s->left->color == RED))
            {
                // This last test is trivial too due to cases 2-4.
                s->color = RED;
                s->left->color = BLACK;
                rotateRight(s);
            }
            else if ((n == n->parent->right) && (s->left->color == BLACK) &&
                     (s->right->color == RED))
            {
                // This last test is trivial too due to cases 2-4.
                s->color = RED;
                s->right->color = BLACK;
                rotateLeft(s);
            }
        }
        deleteCase6(n);
    }

    void deleteCase6(Node *n)
    {
        Node *s = n->getSibling();

        // s->color = n->parent->color;
        // n->parent->color = BLACK;
        swap(s->color, n->parent->color);

        if (n == n->parent->left)
        {
            s->right->color = BLACK;
            rotateLeft(n->parent);
        }
        else
        {
            s->left->color = BLACK;
            rotateRight(n->parent);
        }
    }

    // 寻找n子树中的最大节点
    Node *maximumNode(Node *n)
    {
        assert(n != NIL);
        while (n->right != NIL)
        {
            n = n->right;
        }
        return n;
    }

    // return: 返回NIL节点或者查询到的节点
    Node *lookupNode(int value)
    {
        Node *n = this->root;
        while (n != NIL)
        {
            if (n->value == value)
            {
                return n;
            }
            else if (n->value > value)
            {
                n = n->left;
            }
            else
            {
                assert(n->value < value);
                n = n->right;
            }
        }
        return n;
    }

    int calculateDepth(Node *n)
    {
        if (n == NIL)
            return 0;
        int left = calculateDepth(n->left);
        int right = calculateDepth(n->right);
        return std::max(left, right) + 1;
    }

    void replaceNode(Node *oldn, Node *newn)
    {
        // 更换oldn和newn在rbtree中的位置，主要是处理父节点的指向问题
        Node *p = oldn->parent;
        if (p == nullptr)
        {
            // Important: 注意根节点的赋值问题
            this->root = newn;
        }
        else
        {
            if (p->left == oldn)
            {
                p->left = newn;
            }
            else if (p->right == oldn)
            {
                p->right = newn;
            }
            else
            {
                abort();
            }
        }

        // Very Important: 
        // IMPORTANT: 如果判断加了下面的判断条件，代码就会报错
        // 在删除节点的时候报错，此时删除节点的时候，如果只剩NIL，则NIL必须替换上去，
        // 此时在我们的判断条件中需要访问 NIL的parent, 所以必须添加NIL的parent
        // if(newn != NIL)
        // {
        //    newn->parent = p; 
        // }

        assert(newn != nullptr);
        newn->parent = p;
    }

    void rotateRight(Node *n)
    {
        Node *nnew = n->left;
        assert(nnew != NIL);
        replaceNode(n, nnew);

        n->left = nnew->right;
        nnew->right = n;
        if (n->left != NIL)
        {
            n->left->parent = n;
        }
        n->parent = nnew;
    }
    void rotateLeft(Node *n)
    {
        Node *nnew = n->right;
        assert(nnew != NIL);
        replaceNode(n, nnew);
        n->right = nnew->left;
        nnew->left = n;
        if (n->right != NIL)
        {
            n->right->parent = n;
        }
        n->parent = nnew;
    }

    void insert(Node *n, int value)
    {
        if (n->value > value)
        {
            if (n->left != NIL)
            {
                insert(n->left, value);
            }
            else
            {
                Node *tmp = new Node();
                tmp->value = value;
                tmp->parent = n;
                tmp->left = tmp->right = NIL;
                n->left = tmp;
                insertNode(tmp);
            }
        }
        else if (n->value < value)
        {
            if (n->right != NIL)
            {
                insert(n->right, value);
            }
            else
            {
                Node *tmp = new Node();
                tmp->value = value;
                tmp->parent = n;
                tmp->left = tmp->right = NIL;
                n->right = tmp;
                insertNode(tmp);
            }
        }
        else
        {
            // 暂不考虑插入相同key值的情况
            return;
        }
    }

    void insertNode(Node *n)
    {
        // 1. 先讨论n的父节点是否为空
        if (n->parent == nullptr)
        {
            insertCase1(n);
            return;
        }
        // 2. 接下来在讨论n的父节点的颜色问题
        if (n->parent->color == BLACK)
        {
            // n的父节点是黑色，这种情况下性质4和性质5没有受到影响，不需要调整, 直接插入即可
            insertCase2(n);
        }
        else
        {
            // 这种情况下n的父节点的颜色是 RED， 此时有两个连续的红节点需要处理
            // 此时两个连续的红节点， 那么 n的 grandparent肯定是黑色
            assert(n->getGrandParent()->color == BLACK);
            // assert(nodeColor(n->getGrandParent()) == BLACK);
            // 两个连续的红节点影响了性质4, 此时根据uncle节点的颜色进行不同的分类讨论
            if (nodeColor(n->getUncle()) == RED)
            {
                insertCase3(n);
            }
            else
            {
                insertCase4(n);
            }
        }
    }

    void insertCase1(Node *n)
    {
        n->color = BLACK;
    }
    void insertCase2(Node *n) {}
    void insertCase3(Node *n)
    {
        n->parent->color = BLACK;
        n->getUncle()->color = BLACK;
        n->getGrandParent()->color = RED;
        insertNode(n->getGrandParent());
    }
    void insertCase4(Node *n)
    {
        Node *p = n->parent;
        Node *g = n->getGrandParent();
        if (n == p->right && p == g->left)
        {
            rotateLeft(p);
            n = n->left;
        }

        if (n == p->left && p == g->right)
        {
            rotateRight(p);
            n = n->right;
        }
        insertCase5(n);
    }
    void insertCase5(Node *n)
    {
        Node *p = n->parent;
        Node *g = n->getGrandParent();
        if (n == p->left)
        {
            rotateRight(g);
        }
        else if (n == p->right)
        {
            rotateLeft(g);
        }
        else
        {
            abort();
        }
        p->color = BLACK;
        g->color = RED;
    }

    void deleteTree(Node *n)
    {
        if (n == NIL)
        {
            return;
        }
        deleteTree(n->left);
        deleteTree(n->right);
        delete n;
    }

    void verify_property_1(Node *n)
    {
        if (n == NIL)
            return;
        assert(n->color == RED || n->color == BLACK);
        verify_property_1(n->left);
        verify_property_1(n->right);
    }

    void verify_property_2(Node *n)
    {
        assert(n->color == BLACK);
    }

    void verify_property_4(Node *n)
    {
        if (n == NIL)
            return;
        if (nodeColor(n) == RED)
        {
            assert(nodeColor(n->left) == BLACK);
            assert(nodeColor(n->right) == BLACK);
            assert(nodeColor(n->parent) == BLACK);
        }
        verify_property_4(n->left);
        verify_property_4(n->right);
    }

    void verify_property_5_helper(Node *n, int black_count, int *path_black_count)
    {
        if (nodeColor(n) == BLACK)
        {
            black_count++;
        }
        if (n == NIL)
        {
            if (*path_black_count == -1)
            {
                // 第一次到达NIL时，记录数据
                *path_black_count = black_count;
            }
            else
            {
                assert(black_count == *path_black_count);
            }
            return;
        }
        verify_property_5_helper(n->left, black_count, path_black_count);
        verify_property_5_helper(n->right, black_count, path_black_count);
    }

    void verify_property_5(Node *n)
    {
        int path_black_count = -1;
        verify_property_5_helper(n, 0, &path_black_count);
    }
};
