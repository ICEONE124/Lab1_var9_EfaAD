#ifndef LIST_H
#define LIST_H

typedef enum {
    TYPE_SCALAR,
    TYPE_ROW_VECTOR,
    TYPE_COL_VECTOR,
    TYPE_MATRIX
} ObjectType;

class List;

class Item {
private:
    Item *next;
    Item *prev;
    List *owner;

    ObjectType type;
    int rows;
    int cols;
    double **data;

    void FreeData();

    friend class List;

public:
    Item();
    Item(double val);
    Item(int cols, const double *arr, bool rowVector);
    Item(int rows, int cols, double **arr);
    ~Item();

    ObjectType Type() const { return type; }
    int Rows() const { return rows; }
    int Cols() const { return cols; }
    double** Data() const { return data; }

    Item* Next() const { return next; }
    Item* Prev() const { return prev; }
};

class List {
private:
    Item *head;
    Item *tail;

    Item* RemoveByPointer(Item *item);

public:
    List();
    ~List();

    void Add(Item *item);
    Item* GetItem(int n) const;
    Item* Remove(int n);
    void Delete(int n);
    int Count() const;
    void Clear();
    int GetIndex(const Item *item) const;
    void Insert(Item *item, int n);

    Item* Head() const { return head; }
    Item* Tail() const { return tail; }
};

#endif