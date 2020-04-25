#include <vector>
#include <iostream>
using namespace std;

class KHeap
{
private:
    vector<int> elements_;
    size_t size_;

    void heapify(int i) // 感觉叫做downfilter更好
    {
        int largest = i; // 将最大元素设置为堆顶元素
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size_ && elements_[left] < elements_[largest])
        {
            largest = left;
        }

        if (right < size_ && elements_[right] < elements_[largest])
        {
            largest = right;
        }

        if (largest != i)
        {
            swap(elements_[i], elements_[largest]);

            // 递归地定义子堆
            heapify(largest);
        }
    }

    void heapifyIteration(int i) // 感觉叫做downfilter更好
    {
        while (2 * i + 1 < size_)
        {
            int largest = i; // 将最大元素设置为堆顶元素
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < size_ && elements_[left] > elements_[largest])
            {
                largest = left;
            }

            if (right < size_ && elements_[right] > elements_[largest])
            {
                largest = right;
            }

            if (largest == i)
            {
                break;
            }
            swap(elements_[i], elements_[largest]);
            i = largest;
        }
    }

public:
    //
    KHeap() : elements_(), size_(0) {}

    // 堆的初始化函数，用无序数组来初始化，排列成有序数组的时间复杂度为O(N)
    // 只要弄清楚堆对应的索引 完成堆的调整即可
    KHeap(vector<int> &data) : elements_(data), size_(data.size())
    {
        for (int i = size_ / 2 - 1; i >= 0; --i)
        {
            heapify(i);
        }
    }
    ~KHeap() {}

    void insert(int value)
    {
        if (size_ == elements_.size())
        {
            elements_.push_back(value);
        }
        int i = size_++;
        int parent = (i - 1) / 2;
        while (parent >= 0 && elements_[parent] < elements_[i])
        {
            swap(elements_[parent], elements_[i]);
            i = parent;
            parent = (i - 1) / 2;
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

        heapify(0);
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