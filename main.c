#include <stdio.h>
#include <stdlib.h>

typedef struct Item {
	struct Item *next;
	struct Item *prev;
} Item;

typedef struct List {
	Item *head;
	Item *tail;
} List;

void Add(List *list, Item *item);
Item* GetItem(const List *list, const int n);
Item* Remove(List *list, const int n);
void Delete(List *list, const int n);
int Count(const List *list);
void Clear(List *list);
int GetIndex(const List *list, const Item *item);
int Insert(List *list, Item *item, const int n);
void PrintList(const List *list);

void Add(List *list, Item *item) {
	if (list == NULL || item == NULL)
		return;

	item->next = NULL;
	item->prev = NULL;

	if (list->head == NULL) {
		list->head = item;
		list->tail = item;
	} else {
		item->prev = list->tail;
		list->tail->next = item;
		list->tail = item;
	}
}

Item* GetItem(const List *list, const int n) {
	if (list == NULL || n < 0)
		return NULL;

	Item *item = NULL;
	int i;
	for (i = 0, item = list->head; item != NULL && i < n; i++) {
		item = item->next;
	}
	return item;
}

Item* Remove(List *list, const int n) {
	if (list == NULL)
		return NULL;

	Item *item = GetItem(list, n);
	if (item != NULL) {
		if (list->head == list->tail) {
			list->head = NULL;
			list->tail = NULL;
		} else {
			if (item == list->head) {
				list->head = item->next;
				list->head->prev = NULL;
			} else if (item == list->tail) {
				list->tail = item->prev;
				list->tail->next = NULL;
			} else {
				item->next->prev = item->prev;
				item->prev->next = item->next;
			}
		}
		item->next = NULL;
		item->prev = NULL;
		return item;
	}
	return NULL;
}

void Delete(List *list, const int n) {
	if (list == NULL)
		return;

	Item *item = Remove(list, n);
	if (item == NULL) {
		printf("Объект не найден\n");
	} else {
		free(item);
	}
}

int Count(const List *list) {
	if (list == NULL)
		return -1;

	Item *item;
	int n;

	for (n = 0, item = list->head; item != NULL; item = item->next) {
		n++;
	}

	return n;
}

void Clear(List *list) {
	if (list == NULL)
		return;

	while (list->head) {
		Delete(list, 0);
	}
}

int GetIndex(const List *list, const Item *item) {
	if (list == NULL || item == NULL)
		return -1;

	Item *i;
	int number;
	for (number = 0, i = list->head; i != NULL; i = i->next) {
		if (i == item) {
			return number;
		}
		number++;
	}
	return -1;
}

int Insert(List *list, Item *item, const int n) {
	if (list == NULL || item == NULL)
		return -1;

	item->next = NULL;
	item->prev = NULL;

	Item *old_item = GetItem(list, n);
	if (old_item != NULL) {
		if (old_item == list->head) {
			list->head = item;
			item->prev = NULL;
			old_item->prev = item;
			item->next = old_item;
		} else {
			item->next = old_item;
			item->prev = old_item->prev;
			old_item->prev->next = item;
			old_item->prev = item;
		}
	} else {
		Add(list, item);
	}
	return 0;
}

void PrintList(const List *list) {
	if (list == NULL) {
		printf("List: NULL\n");
		return;
	}

	Item *i;
	int n;

	printf("List: %p, Head: %p, Tail: %p\n",
			(void*)list, (void*)list->head, (void*)list->tail);
	printf("#\tp\tprev\tnext\n");
	for (i = list->head, n = 0; i != NULL; i = i->next, n++) {
		printf("%d\t%p\t%p\t%p\n", n,
				(void*)i, (void*)i->prev, (void*)i->next);
	}
}

int main() {

	system("chcp 65001 > nul");

	List list = {NULL, NULL};

	int key;
	int stop = 1;
	int n;
	int c;
	Item *item;

	while (stop) {
//		while ((c = getchar()) != '\n' && c != EOF);

		printf("Выберите операцию:"
				"\n0 - Выход"
				"\n1 - Добавить элемент в конец списка"
				"\n2 - Удалить элемент"
				"\n3 - Вставить элемент"
				"\n4 - Вывести количество элементов"
				"\n5 - Очистить список"
				"\n6 - Вывести указатели"
				"\n-> ");
		if (scanf("%d", &key) != 1) {
			printf("Данные введены некорректно!\n\n");
			continue;
		}
		switch (key) {
			case 0:
				stop = 0;
				break;
			case 1:
				item = malloc(sizeof(Item));
				if (item == NULL) {
					printf("Ошибка при выделении памяти!\n");
					break;
				}
				Add(&list, item);
				break;
			case 2:
				printf("Введите номер объекта: ");
				scanf("%d", &n);
				Delete(&list, n);
				break;
			case 3:
				printf("Введите номер позиции: ");
				scanf("%d", &n);
				item = malloc(sizeof(Item));
				if (item == NULL) {
					printf("Ошибка при выделении памяти!\n");
					break;
				}
				if (Insert(&list, item, n) != 0) {
					printf("Некорректная позиция: %d\n", n);
					free(item);
				}
				break;
			case 4:
				printf("%d\n", Count(&list));
				break;
			case 5:
				Clear(&list);
				break;
			case 6:
				PrintList(&list);
				break;
		}
		printf("\n");
	}

	Clear(&list);

	system("pause");

	return 0;
}
