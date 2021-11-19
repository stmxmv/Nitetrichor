/*!
* Created by Molybdenum on 9/13/21.
*
*/
#ifndef NITETRICHOR_LIST_H
#define NITETRICHOR_LIST_H


#ifdef __cplusplus
namespace NC {

namespace school_use {

template<typename T>
class List {

    template<typename U>
    struct Node {
        Node* prev;
        Node* next;
        U data;
        Node() : prev{}, next{}, data{} {}
        Node(U data, Node *prev, Node *next) : prev(prev), next(next), data(data) {}
    };
    typedef Node<int> HeadNode;

    HeadNode *head; // head's data store the size
public:
    struct wrap_iter {
        Node<T> * ptr;
    public:
        typedef std::ptrdiff_t                    difference_type;
        typedef T                                      value_type;
        typedef T&                                      reference;
        typedef T*                                        pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        explicit wrap_iter(Node<T> * ptr) noexcept : ptr(ptr) {}
        T& operator*() {
            return ptr->data;
        }
        Node<T>* operator->() {
            return ptr;
        }
        bool operator== (const wrap_iter& iter) {
            return ptr == iter.ptr;
        }
        bool operator!= (const wrap_iter& iter) {
            return ptr != iter.ptr;
        }
        wrap_iter& operator--() noexcept {
            ptr = ptr->prev;
            return *this;
        }
        wrap_iter& operator++() noexcept {
            ptr = ptr->next;
            return *this;
        }
        wrap_iter operator++(int) noexcept {
            auto ret = *this;
            ++(*this);
            return ret;
        }
        wrap_iter operator--(int) noexcept {
            auto ret = *this;
            --(*this);
            return ret;
        }
    };
    typedef wrap_iter               iterator;
    typedef const wrap_iter   const_iterator;
    typedef Node<T>                 ListNode;

    List() : head(new HeadNode()) {
        head->next = head;
        head->prev = head;
    }

    ~List() {
        auto cur = head;
        while (cur->next != head) {
            auto temp = cur->next;
            delete cur;
            cur = temp;
        }
        delete cur;
    }

    iterator begin() { return iterator(reinterpret_cast<ListNode*>(head->next)); }
    iterator end() { return iterator(reinterpret_cast<ListNode*>(head)); }

    bool insert(int pos, const T &data) {
        if (pos < 0 || pos > size()) {
            cout << "error" << endl;
            return false;
        } else {
            auto cur = begin();
            std::advance(cur, pos);
            insert(cur, data);
        }
        return true;
    }

    iterator insert(iterator iter, const T& data) {
        ++head->data;
        auto pre = std::prev(iter);
        pre->next = new ListNode(data, pre.ptr, iter.ptr);
        iter->prev = pre->next;
        return iterator(pre->next);
    }

    void push_back(const T& data) {
        ++head->data;
        head->prev->next = (HeadNode*)(new ListNode(data, (ListNode*)(head->prev), (ListNode*)(head)));
        head->prev = head->prev->next;
    }

    bool erase(int pos) {
        if (pos < 0 || pos >= size()) {
            cout << "error" << endl;
            return false;
        } else {
            auto cur = begin();
            std::advance(cur, pos);
            erase(cur);
        }
        return true;
    }

    bool swap(int pa, int pb) {
        if (pa < 0 || pa >= size()) {
            cout << "error" << endl;
            return false;
        }
        if (pb < 0 || pb >= size()) {
            cout << "error" << endl;
            return false;
        }
        auto p1 = begin();
        auto p2 = begin();
        std::advance(p1, pa);
        std::advance(p2, pb);
        auto p1p = std::prev(p1);
        auto p2p = std::prev(p2);
        auto p1n = std::next(p1);
        auto p2n = std::next(p2);
        std::swap(p1p->next, p2p->next);
        std::swap(p1n->prev, p2n->prev);
        std::swap(p1->next, p2->next);
        std::swap(p1->prev, p2->prev);
        return true;
    }

    void erase(iterator iter) {
        iter->prev->next = iter->next;
        iter->next->prev = iter->prev;
        delete iter.ptr;
        --head->data;
    }

    int size() { return head->data; }

    friend std::ostream &operator<<(std::ostream &os,  List &list) {
        for (auto i : list) {
            os << i << ' ';
        }
        return os;
    }

    static int LL_merge(List<T> &La, List<T> &Lb) {
        auto cur = La.begin();
        for (auto iter = Lb.begin(); iter != Lb.end();) {
            cur = std::lower_bound(cur, La.end(), *iter);
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;
            auto temp = iter->next;
            auto pre = std::prev(cur);
            pre->next = iter.ptr;
            cur->prev = iter.ptr;
            iter->prev = pre.ptr;
            iter->next = cur.ptr;
            iter = iterator(temp);
        }
        return 1;
    }

};


}

template<typename T>
class List {

    template<typename U>
    struct Node {
        Node* prev;
        Node* next;
        U data;
        Node() : prev{}, next{}, data{} {}
        Node(U data, Node *prev, Node *next) : prev(prev), next(next), data(data) {}
    };
    typedef Node<int> HeadNode;

    HeadNode *head; // head's data store the size
public:
    struct wrap_iter {
        Node<T> * ptr;
    public:
        typedef std::ptrdiff_t                    difference_type;
        typedef T                                      value_type;
        typedef T&                                      reference;
        typedef T*                                        pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        explicit wrap_iter(Node<T> * ptr) noexcept : ptr(ptr) {}
        T& operator*() {
            return ptr->data;
        }
        Node<T>* operator->() {
            return ptr;
        }
        bool operator== (const wrap_iter& iter) {
            return ptr == iter.ptr;
        }
        bool operator!= (const wrap_iter& iter) {
            return ptr != iter.ptr;
        }
        wrap_iter& operator--() noexcept {
            ptr = ptr->prev;
            return *this;
        }
        wrap_iter& operator++() noexcept {
            ptr = ptr->next;
            return *this;
        }
        wrap_iter operator++(int) noexcept {
            auto ret = *this;
            ++(*this);
            return ret;
        }
        wrap_iter operator--(int) noexcept {
            auto ret = *this;
            --(*this);
            return ret;
        }
    };
    typedef wrap_iter               iterator;
    typedef const wrap_iter   const_iterator;
    typedef Node<T>                 ListNode;

    List() : head(new HeadNode()) {
        head->next = head;
        head->prev = head;
    }

    ~List() {
        auto cur = head;
        while (cur->next != head) {
            auto temp = cur->next;
            delete cur;
            cur = temp;
        }
        delete cur;
    }

    iterator begin() { return iterator(reinterpret_cast<ListNode*>(head->next)); }
    iterator end() { return iterator(reinterpret_cast<ListNode*>(head)); }

    void insert(iterator iter, const T& data) {
        ++head->data;
        auto pre = std::prev(iter);
        pre->next = new ListNode(data, pre.ptr, iter.ptr);
        iter->prev = pre->next;
    }

    void push_back(const T& data) {
        ++head->data;
        head->prev->next = (HeadNode*)(new ListNode(data, (ListNode*)(head->prev), (ListNode*)(head)));
        head->prev = head->prev->next;
    }

    void erase(iterator iter) {
        iter->prev->next = iter->next;
        iter->next->prev = iter->prev;
        delete iter.ptr;
        --head->data;
    }

};


template<typename T>
class List1 {
    struct Node {
        T data;
        Node * next;
    public:
        Node() noexcept : data{}, next{} {}
        Node(T data, Node* next) noexcept : data(data), next(next) {}
    };

    Node *head;
    int size;
public:
    struct wrap_iter {
        Node * ptr;
    public:
        typedef std::ptrdiff_t              difference_type;
        typedef T                                value_type;
        typedef T&                                reference;
        typedef T*                                  pointer;
        typedef std::forward_iterator_tag iterator_category;
        explicit wrap_iter(Node * ptr) noexcept : ptr(ptr) {}
        T& operator*() {
            return ptr->data;
        }
        Node* operator->() {
            return ptr;
        }
        bool operator!= (const wrap_iter& iter) {
            return ptr != iter.ptr;
        }
        wrap_iter& operator++() noexcept {
            ptr = ptr->next;
            return *this;
        }
        wrap_iter operator++(int) noexcept {
            auto ret = *this;
            ++(*this);
            return ret;
        }
    };
    typedef wrap_iter iterator;
    typedef const wrap_iter const_iterator;

    List1() noexcept : head(new Node()), size{} {
        head->next = head;
    }

    ~List1() {
        auto cur = head;
        while (cur->next != head) {
            auto temp = cur->next;
            delete cur;
            cur = temp;
        }
        delete cur;
    }

    iterator begin() { return iterator(head->next); }

    iterator end() { return iterator(head); }

    void insert(iterator iter, const T& data) {
        ++size;
        auto node = new Node(iter->data, iter->next);
        iter->next = node;
        iter->data = data;
        if (iter.ptr == head) {
            head = node;
        }
    }

    void push_back(const T& data) {
        ++size;
        auto node = new Node(head->data, head->next);
        head->next = node;
        head->data = data;
        head = node;
    }

    void erase(iterator iter) {
        auto pre = iter++;
        *pre = *iter;
        pre->next = iter->next;
        delete iter.ptr;
        if (iter.ptr == head) {
            head = pre.ptr;
        }
        --size;
    }

};




}
#endif

struct s_NCListNode {
    struct s_NCListNode * parent;
    struct s_NCListNode * child;
    // data below

};

// the data of the list head node
struct s_NCListHead {
    int elem_size;
    int size;
};

typedef struct s_NCListNode *NCListRef;
typedef struct s_NCListNode *NCListNodeRef;

NCListRef NCListCreate(int elem_size) {
    NCListRef head = calloc(1,sizeof(struct s_NCListNode) + sizeof(struct s_NCListHead));
    head->child = head;
    head->parent = head;
    struct s_NCListHead * headData = (void*)head + sizeof(struct s_NCListNode);
    headData->elem_size = elem_size;
    return head;
}

int NCListGetSize(NCListRef self) {
    return ((struct s_NCListHead *)((void*)self + sizeof(struct s_NCListHead)))->size;
}

int NCListGetElemSize(NCListRef self) {
    return ((struct s_NCListHead *)((void*)self + sizeof(struct s_NCListHead)))->elem_size;
}

void * NCListNodeGetValue(NCListNodeRef self) {
    return (void*)self + sizeof(struct s_NCListNode);
}

NCListNodeRef NCListGetNodeAt(NCListRef self, int pos) {
    int size = NCListGetSize(self);
    if (pos < 0 || pos >= size) {
        return NULL;
    }
    NCListNodeRef cur = self->child;
    while (pos--) {
        cur = cur->child;
    }
    return cur;
}


void * NCListGetValueAt(NCListRef self, int pos) {
    return NCListNodeGetValue(NCListGetNodeAt(self, pos));
}



NCListNodeRef NCListAppendWithSize(NCListRef self, void * item, int elemSize) {
    NCListNodeRef cur, temp = self->child;
    while (temp != self) {
        cur = temp;
        temp = temp->child;
    }
    cur->child = calloc(1, sizeof(struct s_NCListNode) + elemSize);
    cur->child->parent = cur;
    cur->child->child = self;
    self->parent = cur->child;
    void * data = (void*)cur->child + sizeof(struct s_NCListNode);
    memcpy(data, item, elemSize);
    // adjust size
    ++((struct s_NCListHead *)((void*)self + sizeof(struct s_NCListHead)))->size;
    return cur->child;
}

NCListNodeRef NCListAppend(NCListRef self, void * item) {
    return NCListAppendWithSize(self, item, NCListGetElemSize(self));
}


void NCListRelease(NCListRef self) {
    NCListNodeRef cur = self->child;
    while (cur != self) {
        NCListNodeRef temp = cur;
        cur = cur->child;
        free(temp);
    }
    free(self);
}

NCListNodeRef NCListRemoveWithNode(NCListRef self, NCListNodeRef node) {
    NCListNodeRef pre = node->parent, next = node->child;
    free(node);
    pre->child = next;
    next->parent = pre;
    // adjust size
    --((struct s_NCListHead *)((void*)self + sizeof(struct s_NCListHead)))->size;
    return next;
}

NCListNodeRef NCListRemove(NCListRef self, int pos) {
    int size = NCListGetSize(self);
    if (pos < 0 || pos >= size) {
        return NULL;
    }
    NCListNodeRef cur = self->child;
    while (pos--) {
        cur = cur->child;
    }
    return NCListRemoveWithNode(self, cur);
}


NCListNodeRef NCListInsertWithNodeAndSize(NCListRef self, NCListNodeRef node, void * item, int elemSize) {
    NCListNodeRef pre = node->parent;
    NCListNodeRef new = calloc(1, sizeof(struct s_NCListNode) + elemSize);
    void * data = (void*)new->child + sizeof(struct s_NCListNode);
    memcpy(data, item, elemSize);
    pre->child = new;
    new->parent = pre;
    new->child = node;
    node->parent = new;
    // adjust size
    ++((struct s_NCListHead *)((void*)self + sizeof(struct s_NCListHead)))->size;
    return new;
}

NCListNodeRef NCListInsertWithSize(NCListRef self, int pos, void * item, int elemSize) {
    int size = NCListGetSize(self);
    if (pos < 0 || pos > size) {
        return NULL;
    } else if (pos == size) {
        NCListAppendWithSize(self, item, elemSize);
        return self->parent;
    }
    NCListNodeRef cur = self->child;
    while (pos--) {
        cur = cur->child;
    }
    return NCListInsertWithNodeAndSize(self, cur, item, elemSize);
}

NCListNodeRef NCListInsertWithNode(NCListRef self, NCListNodeRef node, void * item) {
    return NCListInsertWithNodeAndSize(self, node, item, NCListGetElemSize(self));
}

NCListNodeRef NCListInsert(NCListRef self, int pos, void * item) {
    return NCListInsertWithSize(self, pos, item, NCListGetElemSize(self));
}

void NCListMerge(NCListRef self, NCListRef list) {

}

typedef struct s_NCPolyNode {
    float  coef;
    int    expn;
} *NCPolyNodeRef ;

typedef struct s_NCListNode * NCPolyRef;

NCPolyRef NCPolyCreate(void) {
    return NCListCreate(sizeof(struct s_NCPolyNode));
}

void NCPolyRelease(NCPolyRef self) {
    NCListRelease(self);
}

void NCPolyAppend(NCPolyRef self, float coef, int expn) {
    if (NCListGetSize(self) == 0) {
        struct s_NCPolyNode node = {.coef = coef, .expn = expn};
        NCListAppend(self, &node);
        return;
    }

    NCListNodeRef listNode = self;
    NCPolyNodeRef temp;

    do {
        listNode = listNode->child;
        temp = NCListNodeGetValue(listNode);
    } while (listNode != self && temp->expn < expn);

    if (listNode == self || temp->expn > expn) {
        struct s_NCPolyNode node = {coef, expn};
        NCListInsertWithNode(self, listNode, &node); // if listNode == self, this works !!!
    }

    temp->coef += coef;
}

void NCPolyAdd(NCPolyRef self, NCPolyRef poly) {
    if (NCListGetSize(self) == 0) {

    }
}

#endif //NITETRICHOR_LIST_H
