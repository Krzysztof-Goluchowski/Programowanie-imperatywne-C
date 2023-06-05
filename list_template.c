#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->compare_data = compare_data;
    p_list->free_data = free_data;
    p_list->dump_data = dump_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    if (p_list->head == NULL){
        return;
    }
    ListElement *element = p_list->head;
    while (element != NULL){
        p_list->dump_data(element->data);
        element = element->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    if (p_list->compare_data == NULL) return;
    for (ListElement *temp = p_list->head; temp != NULL; temp = temp->next) {
        if (p_list->compare_data(temp->data, data) == 0) {
            if (p_list->dump_data != NULL) {
                p_list->dump_data(temp->data);
            }
        }
    }
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
    free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
    ListElement *current = p_list->head;
    while (current != NULL) {
        ListElement *todelete = current;
        current = current->next;
        if (p_list->free_data != NULL){
            p_list->free_data(todelete->data);
        }
        free(todelete);
    }
    p_list->head = NULL;
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement *new_element = safe_malloc(sizeof(ListElement));
    new_element->data = data;
    if (p_list->head == NULL) {
        p_list->head = new_element;
        p_list->tail = new_element;
    } else {
        new_element->next = p_list->head;
        p_list->head = new_element;
    }
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *new_element = safe_malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = NULL;
    if (p_list->tail == NULL){
        p_list->head = new_element;
        p_list->tail = new_element;
    } else {
        p_list->tail->next = new_element;
        p_list->tail = new_element;
    }


}

// Remove the first element
void pop_front(List *p_list) {
    if (p_list->head == NULL){
        return;
    } else {
        ListElement *first = p_list->head;
        ListElement *second = first->next;
        p_list->head = second;
        free(first);
    }
}

// Reverse the list
void reverse(List *p_list) {
    if (p_list->head == NULL || p_list->head == p_list->tail){
        return;
    }
    ListElement *new_tail = p_list->head;
    ListElement *second = new_tail->next;
    ListElement *first = p_list->head;
    while (second != p_list->tail){
        ListElement *third = second->next;
        second->next = first;
        first = second;
        second = third;
    }
    second->next = first;
    p_list->head = second;
    p_list->tail = new_tail;
    new_tail->next = NULL;
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement* current = p_list->head;
    ListElement* prev = NULL;
    while (current != NULL && p_list->compare_data(current->data, p_element->data) < 0) {
        prev = current;
        current = current->next;
    }
    return prev;
}


// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    ListElement *curr = p_list->head;
    while (curr != NULL) {
        if (p_list->compare_data(curr->data, p_data) == 0) {
            if (p_list->modify_data) {
                p_list->modify_data(curr->data);
            }
            return;
        }
        curr = curr->next;
    }

    ListElement *new_element = safe_malloc(sizeof(ListElement));
    new_element->data = p_data;
    new_element->next = NULL;

    if (p_list->head == NULL){
        p_list->head = new_element;
        p_list->tail = new_element;
    } else {
        ListElement *insertion_point = find_insertion_point(p_list, new_element);
        if (insertion_point == NULL) {
            new_element->next = p_list->head;
            p_list->head = new_element;
        } else {
            new_element->next = insertion_point->next;
            insertion_point->next = new_element;

            if (insertion_point == p_list->tail){
                p_list->tail = new_element;
            }
        }
    }

}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
    const int *data = d;
    printf("%d ", *data);
}

void free_int(void *d) {
    if (d == NULL) return;
    free(d);
}

int cmp_int(const void *a, const void *b) {
    const int *x = (const int*)a;
    const int *y = (const int*)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

int *create_data_int(int v) {
    int *data = safe_malloc(sizeof(int));
    *data = v;
    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
    const DataWord *data = (const DataWord*)d;
    printf("%s ", data->word);
}

void dump_word_lowercase(const void *d){
    const DataWord *data = (const DataWord*)d;
    char *str = data->word;
    for (int i = 0; str[i]; i++) {
        putchar(tolower(str[i]));
    }
    printf("\n");
}

void free_word(void *d) {
    DataWord *data = (DataWord*)d;
    free(data->word);
    free(data);
}

int cmp_word_alphabet(const void *a, const void *b) {
    const DataWord *x = (const DataWord*) a;
    const DataWord *y = (const DataWord*) b;
    return strcmp(x->word, y->word);
}

int cmp_word_counter(const void *a, const void *b) {
    const DataWord *x = (const DataWord*) a;
    const DataWord *y = (const DataWord*) b;
    return (x->counter - y->counter);
}

void modify_word(void *p) {
    DataWord *data = (DataWord*) p;
    data->counter++;
}

void to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void *create_data_word(const char *string, int counter) {
    char *word = safe_strdup(string);
    DataWord *dataWord = (DataWord *)safe_malloc(sizeof(DataWord));

    dataWord->word = word;
    dataWord->counter = counter;
    return dataWord;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char buffer[BUFFER_SIZE];
    int cnt = 0;
    if (cmp == NULL) {
        while (1) {
            if (fgets(buffer, sizeof(buffer), stream) == NULL) {
                break;
            }
            if (buffer[0] == '\n' && cnt != 0) {
                break;
            }
            char *token = strtok(buffer, ".,?!:;- \n\r\t");
            while (token != NULL) {
                DataWord *new_element = create_data_word(token, cnt);
                push_back(p_list, new_element);
                cnt++;
                token = strtok(NULL, ".,?!:;- \n\r\t");
            }
        }
    } else {
        p_list->compare_data = cmp;
        while (1) {
            if (fgets(buffer, sizeof(buffer), stream) == NULL) {
                break;
            }
            if (buffer[0] == '\n' && cnt != 0) {
                break;
            }
            char *token = strtok(buffer, ".,?!:;- \n\r\t");
            while (token != NULL) {
                DataWord *new_element = create_data_word(token, 1);
                to_lower(new_element->word);
                insert_in_order(p_list, new_element);
                cnt++;
                token = strtok(NULL, ".,?!:;- \n\r\t");
            }
        }
    }
}
// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

