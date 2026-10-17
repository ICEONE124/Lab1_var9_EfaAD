#include <stdio.h>
#include <stdlib.h>

#include "list.h"

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
