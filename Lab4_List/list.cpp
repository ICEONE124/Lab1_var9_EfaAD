#include <cstdlib>
#include <cmath>
#include "list.h"

/* ================= Item ================= */

Item::Item() {
    next = nullptr;
    prev = nullptr;
    owner = nullptr;
    type = TYPE_SCALAR;
    rows = 1;
    cols = 1;
    data = nullptr;
}

Item::Item(double val) {
    next = nullptr;
    prev = nullptr;
    owner = nullptr;
    type = TYPE_SCALAR;
    rows = 1;
    cols = 1;

    data = new double*[1];
    data[0] = new double[1];
    data[0][0] = val;
}

Item::Item(int cols, const double *arr, bool rowVector) {
    next = nullptr;
    prev = nullptr;
    owner = nullptr;

    if (rowVector) {
        type = TYPE_ROW_VECTOR;
        rows = 1;
        this->cols = cols;

        data = new double*[1];
        data[0] = new double[cols];
        for (int j = 0; j < cols; j++) {
            data[0][j] = arr ? arr[j] : 0.0;
        }
    } else {
        type = TYPE_COL_VECTOR;
        rows = cols;
        this->cols = 1;

        data = new double*[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[1];
            data[i][0] = arr ? arr[i] : 0.0;
        }
    }
}

Item::Item(int rows, int cols, double **arr) {
    next = nullptr;
    prev = nullptr;
    owner = nullptr;

    if (rows == 1 && cols == 1)      type = TYPE_SCALAR;
    else if (rows == 1)              type = TYPE_ROW_VECTOR;
    else if (cols == 1)              type = TYPE_COL_VECTOR;
    else                             type = TYPE_MATRIX;

    this->rows = rows;
    this->cols = cols;

    data = new double*[rows];
    for (int i = 0; i < rows; i++) {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++) {
            data[i][j] = arr ? arr[i][j] : 0.0;
        }
    }
}

Item::~Item() {
    if (owner != nullptr) {
        owner->RemoveByPointer(this);
    }
    FreeData();
}

void Item::FreeData() {
    if (data == nullptr) return;

    for (int i = 0; i < rows; i++) {
        delete[] data[i];
    }
    delete[] data;
    data = nullptr;
}

/* ================= List ================= */

List::List() {
    head = nullptr;
    tail = nullptr;
}

List::~List() {
    Clear();
}

void List::Add(Item *item) {
    if (item == nullptr) return;
    if (item->owner != nullptr) return;

    item->next = nullptr;
    item->prev = nullptr;
    item->owner = this;

    if (head == nullptr) {
        head = item;
        tail = item;
    } else {
        item->prev = tail;
        tail->next = item;
        tail = item;
    }
}

Item* List::GetItem(int n) const {
    if (n < 0) return nullptr;

    Item *item = head;
    int i = 0;
    while (item != nullptr && i < n) {
        item = item->next;
        i++;
    }
    return item;
}

Item* List::RemoveByPointer(Item *item) {
    if (item == nullptr) return nullptr;

    if (item->prev != nullptr) item->prev->next = item->next;
    else                       head = item->next;

    if (item->next != nullptr) item->next->prev = item->prev;
    else                       tail = item->prev;

    item->next = nullptr;
    item->prev = nullptr;
    item->owner = nullptr;
    return item;
}

Item* List::Remove(int n) {
    Item *item = GetItem(n);
    return RemoveByPointer(item);
}

void List::Delete(int n) {
    Item *item = Remove(n);
    if (item == nullptr) {
        printf("Объект не найден\n");
    } else {
        delete item;
    }
}

int List::Count() const {
    int n = 0;
    for (Item *p = head; p != nullptr; p = p->next) n++;
    return n;
}

void List::Clear() {
    while (head != nullptr) {
        Item *p = head;
        head = head->next;
        p->owner = nullptr;
        p->next = nullptr;
        p->prev = nullptr;
        delete p;
    }
    tail = nullptr;
}

int List::GetIndex(const Item *item) const {
    if (item == nullptr) return -1;

    int number = 0;
    for (Item *p = head; p != nullptr; p = p->next) {
        if (p == item) return number;
        number++;
    }
    return -1;
}

void List::Insert(Item *item, int n) {
    if (item == nullptr) return;
    if (item->owner != nullptr) return;

    Item *old_item = GetItem(n);

    if (old_item == nullptr) {
        Add(item);
        return;
    }

    item->next = old_item;
    item->prev = old_item->prev;
    item->owner = this;

    if (old_item->prev != nullptr) {
        old_item->prev->next = item;
    } else {
        head = item;
    }
    old_item->prev = item;
}