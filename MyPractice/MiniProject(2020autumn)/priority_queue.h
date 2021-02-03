#include <stdlib.h>

template <typename T>
struct Priority_Queue
{
private:
    T *data;
    size_t rear;
    size_t capacity;
    int (*cmp)(const T &a, const T &b);

public:
    static void init(struct Priority_Queue &q, size_t capacity, int (*cmp)(const T &a, const T &b));
    static void destroy(struct Priority_Queue &q);
    static bool enqueue(struct Priority_Queue &q, const T &e);
    static bool dequeue(struct Priority_Queue &q, T &e);
    static bool top(const struct Priority_Queue &q, T &e);
    static size_t length(const struct Priority_Queue &q);
    static bool empty(const struct Priority_Queue &q);
    static bool full(const struct Priority_Queue &q);
};

template <typename T>
void Priority_Queue<T>::init(struct Priority_Queue<T> &q, size_t capacity, int (*cmp)(const T &a, const T &b))
{
    q.data = new T[capacity + 1];
    q.rear = 1;
    q.capacity = capacity;
    q.cmp = cmp;
}

template <typename T>
void Priority_Queue<T>::destroy
(struct Priority_Queue<T> &q)
{
    delete[] q.data;
}

template <typename T>
bool Priority_Queue<T>::enqueue(struct Priority_Queue<T> &q, const T &e)
{
    if (q.rear != q.capacity + 1)
    {
        q.data[q.rear] = e;
        size_t i = q.rear;
        while ((i > 1) && (q.cmp(q.data[i / 2], q.data[i]) > 0))
        {
            T temp = q.data[i];
            q.data[i] = q.data[i / 2];
            q.data[i / 2] = temp;
            i = i / 2;
        }
        ++q.rear;
        return true;
    }
    return false;
}

template <typename T>
bool Priority_Queue<T>::dequeue(struct Priority_Queue<T> &q, T &e)
{
    if (q.rear != 1)
    {
        e = q.data[1];
        --q.rear;
        q.data[1] = q.data[q.rear];
        q.data[q.rear].~T();
        for (int i = 1; i <= (q.rear - 1) / 2; ++i)
        {
            if ((2 * i < q.rear) && (q.cmp(q.data[i], q.data[2 * i]) > 0))
            {
                T temp = q.data[i];
                q.data[i] = q.data[2*i];
                q.data[2*i] = temp;
            }
            if ((2 * i + 1 < q.rear) && (q.cmp(q.data[i], q.data[2 * i + 1]) > 0))
            {
                T temp = q.data[i];
                q.data[i] = q.data[2*i+1];
                q.data[2*i+1] = temp;
            }
        }
        return true;
    }
    return false;
}

template <typename T>
bool Priority_Queue<T>::top(const struct Priority_Queue<T> &q, T &e)
{
    if (q.rear != 1)
    {
        e = q.data[1];
        return true;
    }
    return false;
}

template <typename T>
size_t Priority_Queue<T>::length(const struct Priority_Queue<T> &q)
{
    return q.rear - 1;
}

template <typename T>
bool Priority_Queue<T>::empty(const struct Priority_Queue<T> &q)
{
    if (q.rear == 1)
        return true;
    return false;
}

template <typename T>
bool Priority_Queue<T>::full(const struct Priority_Queue<T> &q)
{
    if (q.rear == q.capacity + 1)
        return true;
    return false;
}
