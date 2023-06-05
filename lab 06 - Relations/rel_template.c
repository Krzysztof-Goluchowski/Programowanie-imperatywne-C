
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *relation, int n){
    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        int flaga1 = 0;
        int flaga2 = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == jeden && relation[j].second == jeden){
                flaga1 = 1;
            }
            if (relation[j].first == dwa && relation[j].second == dwa){
                flaga2 = 1;
            }
        }
        if (flaga1 != 1 || flaga2 != 1){
            return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *relation, int n){
    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        int flaga1 = 0;
        int flaga2 = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == jeden && relation[j].second == jeden){
                flaga1 = 1;
            }
            if (relation[j].first == dwa && relation[j].second == dwa){
                flaga2 = 1;
            }
        }
        if (flaga1 == 1 || flaga2 == 1){
            return 0;
        }
    }
    return 1;
}


// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *relation, int n){
    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        int flaga = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == dwa && relation[j].second == jeden){
                flaga = 1;
            }
        }
        if (flaga == 0){
            return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *relation, int n){
    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == dwa && relation[j].second == jeden){
                if (relation[j].first != dwa || relation[j].second != jeden){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *relation, int n){
    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == dwa && relation[j].second == jeden){
                return 0;
            }
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *relation, int n){
    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == dwa){
                int flag = 0;
                int trzy = relation[j].second;
                for (int k = 0; k < n; ++k) {
                    if (relation[k].first == jeden && relation[k].second == trzy){
                        flag = 1;
                    }
                }
                if (flag == 0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *relation, int n){
    int reflexive;
    int transitive;
    int antisymmetric;
    reflexive = is_reflexive(relation, n);
    transitive = is_transitive(relation, n);
    antisymmetric = is_antisymmetric(relation, n);
    if (reflexive == 1 && transitive == 1 && antisymmetric == 1){
        return 1;
    }
    return 0;
}



int czy_jest(int *arr, int n, int liczba){
    for (int i = 0; i < n; ++i) {
        if (arr[i] == liczba){
            return 1;
        }
    }
    return 0;
}

int get_domain(pair *relation, int n, int *domain){
    int ind = 0;
    int flag1;
    int flag2;

    for (int i = 0; i < n; ++i) {
        int jeden = relation[i].first;
        int dwa = relation[i].second;
        flag1 = 0;
        flag2 = 0;
        for (int j = 0; j < ind; ++j) {
            if (domain[j] == jeden){
                flag1 = 1;
            }
            if (domain[j] == dwa){
                flag2 = 1;
            }
        }
        if (flag1 == 0 && czy_jest(domain, ind, jeden) == 0){
            domain[ind] = jeden;
            ind++;
        }
        if (flag2 == 0 && czy_jest(domain, ind, dwa) == 0){
            domain[ind] = dwa;
            ind++;
        }
    }
    return ind;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *relation, int n){
    int tab[MAX_REL_SIZE];
    get_domain(relation, n, tab);
    int ind = 0;

    for (int i = 0; i < 2*n; ++i) {
        int jeden = tab[i];
        int dwa = tab[i+1];
        if (jeden != dwa){
            int flag = 0;
            for (int j = 0; j < n; ++j) {
                if ((relation[j].first == jeden && relation[j].second == dwa) || (relation[j].first == dwa && relation[j].second == jeden)){
                    flag = 1;
                }
            }
            if (flag == 0){
                return 0;
            }
        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *relation, int n){
    int partial_order;
    partial_order = is_partial_order(relation, n);
    int connected;
    connected = is_connected(relation, n);
    if (partial_order == 1 && connected == 1){
        return 1;
    }
    return 0;
}

int find_max_elements(pair *relation, int n, int *max_elements){
    int dziedzina[MAX_REL_SIZE];
    int ind = 0;
    int licznik = 0;
    int n_dziedziny = get_domain(relation, n, dziedzina);
    for (int i = 0; i < n_dziedziny; ++i) {
        int g = dziedzina[i];
        int flag = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == g && relation[j].second != g){
                flag = 1;
            }
        }
        if(flag == 0){
            max_elements[ind] = g;
            ind++;
            licznik++;
        }
    }
    return licznik;
}

int find_min_elements(pair *relation, int n, int *min_elements){
    int dziedzina[MAX_REL_SIZE];
    int ind = 0;
    int licznik = 0;
    int n_dziedziny = get_domain(relation, n, dziedzina);
    for (int i = 0; i < n_dziedziny; ++i) {
        int m = dziedzina[i];
        int flag = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first != m && relation[j].second == m){
                flag = 1;
            }
        }
        if(flag == 0){
            min_elements[ind] = m;
            ind++;
            licznik++;
        }
    }
    return licznik;
}



// Case 3:

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
    if (a == b){
        return 1;
    }
    return 0;
}

int composition (pair *relation1, int n1, pair *relation2, int n2, pair *relation3){
    int licznik = 0;
    for (int i = 0; i < n1; ++i) {
        int jeden = relation1[i].first;
        int dwa = relation1[i].second;
        for (int j = 0; j < n2; ++j) {
            int trzy = relation2[j].first;
            int cztery = relation2[j].second;
            if (dwa == trzy){
                pair para;
                para.first = jeden;
                para.second = cztery;
                int unikat = add_relation(relation3, licznik, para);
                if (unikat == 1){
                    licznik++;
                }
            }
        }
    }
    return licznik;
}


int insert_int (int *tab, int n, int new_element) {
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {

    int a = new_pair.first;
    int b = new_pair.second;
    int flag = 0;
    for (int i = 0; i < n; ++i) {
        if (a == tab[i].first && b == tab[i].second){
            flag = 1;
        }
    }
    if (flag == 0){
        tab[n] = new_pair;
        return 1;
    }
    return 0;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    scanf("%d",&n);

    for (int i = 0; i < n; ++i) {
        int jeden;
        int dwa;
        scanf("%d", &jeden);
        scanf("%d", &dwa);
        relation[i].first = jeden;
        relation[i].second = dwa;
    }
    return n;
}

void print_int_array(int *array, int n) {
    printf("%d \n", n);
    bubbleSort(array, n);
    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;

		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

