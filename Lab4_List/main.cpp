#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "list.h"

class DebugList : public List {
private:
    int GetSize(const Item *item) const {
        return item ? item->rows * item->cols : 0;
    }

public:
    void Print();
    void Menu();
    Item* InputItemFromUser();
    void SortBySize();
    void SearchInList(double target, double eps);
    int AddElements(int n1, int n2);
    int SubtractElements(int n1, int n2);
    int MultiplyElements(int n1, int n2);
    int DivideElements(int n1, int n2);
};

/* ---------- Print ---------- */

void DebugList::Print() {
    Item *p;
    int n;
    int r, c;

    printf("List: %p, Head: %p, Tail: %p\n",
           (void*)this, (void*)Head(), (void*)Tail());
    printf("#\ttype\t\tsize\tvalues\n");

    for (p = Head(), n = 0; p != nullptr; p = p->Next(), n++) {
        printf("%d\t", n);
        switch (p->Type()) {
            case TYPE_SCALAR:     printf("Scalar\t\t"); break;
            case TYPE_ROW_VECTOR: printf("RowVector\t"); break;
            case TYPE_COL_VECTOR: printf("ColVector\t"); break;
            case TYPE_MATRIX:     printf("Matrix\t\t"); break;
        }
        printf("%dx%d\t", p->Rows(), p->Cols());

        for (r = 0; r < p->Rows(); r++) {
            if (r > 0) printf("\n\t\t\t\t");
            for (c = 0; c < p->Cols(); c++) {
                printf("%.2f ", p->Data()[r][c]);
            }
        }
        printf("\n");
    }
}

/* ---------- Menu ---------- */

void DebugList::Menu() {
    printf("Выберите операцию:"
           "\n0 - Выход"
           "\n1 - Добавить элемент в конец списка"
           "\n2 - Удалить элемент"
           "\n3 - Вставить элемент"
           "\n4 - Вывести количество элементов"
           "\n5 - Очистить список"
           "\n6 - Вывести список"
           "\n7 - Сортировать элементы по размеру"
           "\n8 - Найти число в элементах"
           "\n9 - Сложение элементов (n1 + n2)"
           "\n10 - Вычитание элементов (n1 - n2)"
           "\n11 - Умножение элементов (n1 * n2)"
           "\n12 - Деление элементов (n1 / n2)"
           "\n-> ");
}

/* ---------- Ввод ---------- */

Item* DebugList::InputItemFromUser() {
    int type;
    printf("Выберите тип объекта (0 - Скаляр, 1 - Вектор-строка, "
           "2 - Вектор-столбец, 3 - Матрица): ");
    if (scanf("%d", &type) != 1 || type < 0 || type > 3) {
        printf("Некорректный тип!\n");
        return nullptr;
    }

    if (type == 0) {
        double val;
        printf("Введите число: ");
        scanf("%lf", &val);
        return new Item(val);
    } else if (type == 1) {
        int cols;
        printf("Введите длину вектора-строки: ");
        scanf("%d", &cols);
        if (cols <= 0) return nullptr;

        double *arr = new double[cols];
        for (int j = 0; j < cols; j++) {
            printf("[%d]: ", j);
            scanf("%lf", &arr[j]);
        }
        Item *item = new Item(cols, arr, true);
        delete[] arr;
        return item;
    } else if (type == 2) {
        int rows;
        printf("Введите длину вектора-столбца: ");
        scanf("%d", &rows);
        if (rows <= 0) return nullptr;

        double *arr = new double[rows];
        for (int i = 0; i < rows; i++) {
            printf("[%d]: ", i);
            scanf("%lf", &arr[i]);
        }
        Item *item = new Item(rows, arr, false);
        delete[] arr;
        return item;
    } else {
        int rows, cols;
        printf("Введите количество строк и столбцов: ");
        scanf("%d %d", &rows, &cols);
        if (rows <= 0 || cols <= 0) return nullptr;

        double **arr = new double*[rows];
        for (int i = 0; i < rows; i++) {
            arr[i] = new double[cols];
            for (int j = 0; j < cols; j++) {
                printf("[%d][%d]: ", i, j);
                scanf("%lf", &arr[i][j]);
            }
        }
        Item *item = new Item(rows, cols, arr);
        for (int i = 0; i < rows; i++) delete[] arr[i];
        delete[] arr;
        return item;
    }
}

/* ---------- Сортировка ---------- */

void DebugList::SortBySize() {
    Item *i, *j, *min_item;
    int size_j, min_size;

    for (i = Head(); i != nullptr; i = i->Next()) {
        min_item = i;
        min_size = GetSize(i);

        for (j = i->Next(); j != nullptr; j = j->Next()) {
            size_j = GetSize(j);
            if (size_j < min_size) {
                min_size = size_j;
                min_item = j;
            }
        }

        if (min_item != i) {
            ObjectType t_type = i->type;
            int t_rows = i->rows;
            int t_cols = i->cols;
            double **t_data = i->data;

            i->type = min_item->type;
            i->rows = min_item->rows;
            i->cols = min_item->cols;
            i->data = min_item->data;

            min_item->type = t_type;
            min_item->rows = t_rows;
            min_item->cols = t_cols;
            min_item->data = t_data;
        }
    }
}

/* ---------- Поиск ---------- */

void DebugList::SearchInList(double target, double eps) {
    int idx, i, j;
    int found = 0;

    for (Item *p = Head(), idx = 0; p != nullptr; p = p->Next(), idx++) {
        if (p->Data() == nullptr) continue;
        for (i = 0; i < p->Rows(); i++) {
            if (p->Data()[i] == nullptr) continue;
            for (j = 0; j < p->Cols(); j++) {
                if (fabs(p->Data()[i][j] - target) < eps) {
                    printf("Найдено в элементе №%d [%d][%d]: %.2f\n",
                           idx, i, j, p->Data()[i][j]);
                    found = 1;
                }
            }
        }
    }
    if (!found) {
        printf("Число %.2f в элементах списка не найдено.\n", target);
    }
}

/* ---------- Арифметика ---------- */

int DebugList::AddElements(int n1, int n2) {
    Item *a = GetItem(n1);
    Item *b = GetItem(n2);

    if (a == nullptr || b == nullptr) return 0;
    if (a->Rows() != b->Rows() || a->Cols() != b->Cols()) return 0;

    Item *res = new Item(a->Rows(), a->Cols(), nullptr);
    for (int i = 0; i < a->Rows(); i++)
        for (int j = 0; j < a->Cols(); j++)
            res->Data()[i][j] = a->Data()[i][j] + b->Data()[i][j];

    Add(res);
    return 1;
}

int DebugList::SubtractElements(int n1, int n2) {
    Item *a = GetItem(n1);
    Item *b = GetItem(n2);

    if (a == nullptr || b == nullptr) return 0;
    if (a->Rows() != b->Rows() || a->Cols() != b->Cols()) return 0;

    Item *res = new Item(a->Rows(), a->Cols(), nullptr);
    for (int i = 0; i < a->Rows(); i++)
        for (int j = 0; j < a->Cols(); j++)
            res->Data()[i][j] = a->Data()[i][j] - b->Data()[i][j];

    Add(res);
    return 1;
}

int DebugList::MultiplyElements(int n1, int n2) {
    Item *a = GetItem(n1);
    Item *b = GetItem(n2);

    if (a == nullptr || b == nullptr) return 0;

    if (a->Type() == TYPE_SCALAR) {
        Item *res = new Item(b->Rows(), b->Cols(), nullptr);
        for (int i = 0; i < b->Rows(); i++)
            for (int j = 0; j < b->Cols(); j++)
                res->Data()[i][j] = a->Data()[0][0] * b->Data()[i][j];
        Add(res);
        return 1;
    }

    if (a->Cols() != b->Rows()) return 0;

    Item *res = new Item(a->Rows(), b->Cols(), nullptr);
    for (int i = 0; i < a->Rows(); i++) {
        for (int j = 0; j < b->Cols(); j++) {
            res->Data()[i][j] = 0;
            for (int k = 0; k < a->Cols(); k++) {
                res->Data()[i][j] += a->Data()[i][k] * b->Data()[k][j];
            }
        }
    }
    Add(res);
    return 1;
}

int DebugList::DivideElements(int n1, int n2) {
    Item *a = GetItem(n1);
    Item *b = GetItem(n2);

    if (a == nullptr || b == nullptr) return 0;
    if (b->Type() != TYPE_SCALAR) return 0;
    if (fabs(b->Data()[0][0]) < 1e-9) return 0;

    Item *res = new Item(a->Rows(), a->Cols(), nullptr);
    for (int i = 0; i < a->Rows(); i++)
        for (int j = 0; j < a->Cols(); j++)
            res->Data()[i][j] = a->Data()[i][j] / b->Data()[0][0];

    Add(res);
    return 1;
}

/* ---------- main ---------- */

int main() {
    system("chcp 65001 > nul");

    DebugList L;

    int key;
    int stop = 1;
    int n, n1, n2;
    double target;
    Item *item;

    while (stop) {
        L.Menu();
        if (scanf("%d", &key) != 1) {
            printf("Данные введены некорректно!\n\n");
            continue;
        }
        switch (key) {
            case 0:
                stop = 0;
                break;
            case 1:
                item = L.InputItemFromUser();
                if (item != nullptr) L.Add(item);
                break;
            case 2:
                printf("Введите номер объекта: ");
                scanf("%d", &n);
                L.Delete(n);
                break;
            case 3:
                printf("Введите номер позиции: ");
                scanf("%d", &n);
                item = L.InputItemFromUser();
                if (item != nullptr) L.Insert(item, n);
                break;
            case 4:
                printf("Количество элементов: %d\n", L.Count());
                break;
            case 5:
                L.Clear();
                break;
            case 6:
                L.Print();
                break;
            case 7:
                L.SortBySize();
                printf("Список отсортирован по размеру элементов.\n");
                break;
            case 8:
                printf("Введите число для поиска: ");
                scanf("%lf", &target);
                L.SearchInList(target, 1e-6);
                break;
            case 9:
                printf("Введите номера двух элементов (n1 n2): ");
                scanf("%d %d", &n1, &n2);
                if (!L.AddElements(n1, n2)) printf("Операция неправомерна!\n");
                break;
            case 10:
                printf("Введите номера двух элементов (n1 n2): ");
                scanf("%d %d", &n1, &n2);
                if (!L.SubtractElements(n1, n2)) printf("Операция неправомерна!\n");
                break;
            case 11:
                printf("Введите номера двух элементов (n1 n2): ");
                scanf("%d %d", &n1, &n2);
                if (!L.MultiplyElements(n1, n2)) printf("Операция неправильна!\n");
                break;
            case 12:
                printf("Введите номера двух элементов (n1 n2): ");
                scanf("%d %d", &n1, &n2);
                if (!L.DivideElements(n1, n2)) printf("Операция неправомерна!\n");
                break;
        }
        printf("\n");
    }

    system("pause");
    return 0;
}