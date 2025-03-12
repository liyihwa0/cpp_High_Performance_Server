#include "../global_def.h"

namespace wa{

    template <typename T>
    struct MaxHeapComparator {
        static Bool Compare(const T& a, const T& b) {
            return a > b;
        }
    };

    template <typename T>
    struct MinHeapComparator {
        static Bool Compare(const T& a, const T& b) {
            return a < b;
        }
    };

    template<class T,class Comparator = MinHeapComparator<T> >
    class Heap{
        T* datas_;
        Size size_;
        Size capacity_;
    private:
        void swap(Size a,Size b){
            T temp=datas_[a];
            datas_[a]=datas_[b];
            datas_[b]=temp;
        }

    public:
        Heap(Size capacity):capacity_(capacity),size_(0){
            datas_= new T[capacity+1];
        }
        ~Heap(){
            delete[] datas_;
        }
        void up(Size idx)
        {
            // idx/2为父节点
            while(idx / 2 && Comparator::Compare(datas_[idx],datas_[idx / 2])) // 如果x的父节点存在,且x比父节点要小
            {
                swap(idx / 2, idx);	// 交换值
                idx /= 2;			   // 迭代父节点
            }
        }

        void down(Size idx){
            Size t=idx;

            // 找到最小的子节点,并交换其
            if(idx * 2 <= size_ && Comparator::Compare(datas_[idx * 2],datas_[idx])) t= t * 2;
            if(idx * 2 + 1 <= size_ && Comparator::Compare(datas_[idx * 2+1],datas_[idx])) t= t * 2 + 1;
            // 上面在从x和x的两个子树中找最小值
            if(t != idx){
                swap(t, idx);
                down(t);
            }
        }

        void add(T data){    // 增加新元素
            datas_[++size_]=data;
            up(size_);
        }

        T top() const {
            return datas_[1];
        }

        void pop(){
            datas_[1]=datas_[size_--];
            down(1);
        }

        void erase(Size idx){
            datas_[idx]=datas_[size_--];
            down(idx);
            up(idx);
        }

        void set(Size idx, T val){
            datas_[idx]=val;
            down(idx);
            up(idx);
        }

        Size size() const{
            return size_;
        }

    };
};