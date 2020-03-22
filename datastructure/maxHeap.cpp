#include <vector>
#include <iostream>
using namespace std;

class KHeap
{
private:
    vector<int> elements_;
    ssize_t size_;

    void downfilter(int i)
    {
        while ((2 * i + 1) < size_)
        {
            // 父节点和子节点之间的比较
            int maxChild;
            if ((2 * i + 2) == size_)
            {
                maxChild = elements_[2 * i + 1];
            }
            else
            {
                maxChild = max(elements_[2 * i + 1], elements_[2 * i + 2]);
            }

            if (elements_[i] >= maxChild)
            {
                break;
            }

            int maxChildIdx = (elements_[2 * i + 1] == maxChild) ? (2 * i + 1) : (2 * i + 2);
            swap(elements_[maxChildIdx], elements_[i]);
            i = maxChildIdx;
        }
    }

public:
    //
    KHeap() : elements_(), size_(0) {}

    // 堆的初始化函数，用无序数组来初始化，排列成有序数组的时间复杂度为O(N)
    // 只要弄清楚堆对应的索引 然后判断什么时候上滤什么时候下滤，便可快速完成堆的调整
    KHeap(vector<int> &data) : elements_(data), size_(data.size())
    {
        // Todo
        for (int i = size_ / 2 - 1; i >= 0; --i)
        {
            // 第一个可能只有一个左儿子
            if ((2 * i + 2) == size_)
            {
                if (elements_[2 * i + 1] > elements_[i])
                {
                    swap(elements_[2 * i + 1], elements_[i]);
                }
                continue;
            }

            int maxChild = max(elements_[2 * i + 1], elements_[2 * i + 2]);
            if (maxChild <= elements_[i])
            {
                continue;
            }

            int maxChildIdx = (elements_[2 * i + 1] == maxChild) ? (2 * i + 1) : (2 * i + 2);
            swap(elements_[maxChildIdx], elements_[i]);
            downfilter(maxChildIdx);
        }
    }
    ~KHeap() {}

    void insert(int value)
    {
        if (size_ == elements_.size())
        {
            elements_.push_back(value);
        }
        int i = ++size_ - 1;
        for (; (((i - 1) / 2) >= 0) && elements_[(i - 1) / 2] < elements_[i]; i = (i - 1) / 2)
        {
            swap(elements_[(i - 1) / 2], elements_[i]);
        }
    }

    int deleteTop()
    {
        if (elements_.size() == 0)
        {
            // error
            return 0;
        }
        int ret = elements_[0];
        // 取出最后一个元素，并且 size_减去1
        elements_[0] = elements_[--size_];

        downfilter(0);
        return ret;
    }

    // 打印函数，用于观察堆的情况
    void print()
    {
        cout << "========  Start Printing ========" << endl;
        for (size_t i = 0; i < size_; ++i)
        {
            cout << elements_[i] << " ";
            if (((i + 2) & (i + 1)) == 0)
            {
                cout << endl;
            }
        }
        cout << endl
             << "======== Finish Printing ========" << endl
             << endl;
    }
};

// ===== 测试程序 ===== //
int main()
{
    KHeap kheap;
    kheap.insert(44);
    kheap.insert(25);
    kheap.insert(18);
    kheap.insert(10);
    kheap.insert(31);
    kheap.insert(55);
    kheap.insert(60);
    kheap.insert(70);
    kheap.print();
    kheap.deleteTop();
    kheap.print();

    vector<int> data = {10, 50, 30, 20, 70, 80, 60, 15, 65, 45, 75, 90, 35};
    KHeap kheapv(data);
    kheapv.print();

    vector<int> data2 = {79, 66, 43, 83, 72, 87, 38, 55, 91, 30, 49, 9};
    KHeap kheapv2(data2);
    kheapv2.print();

    return 0;
}