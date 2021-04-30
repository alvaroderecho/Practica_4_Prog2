#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types.h"
#include "vertex.h"
#include "bstree.h"

#define MAX_LINE 150

Vertex *read_vertex_from_file(FILE * f_in){
    Vertex *v = NULL;
    char buff[MAX_LINE];

    if (fgets(buff, MAX_LINE, f_in) != NULL){
        v = vertex_initFromString(buff);
    }

    return v;
}

void destroy_vertex_function(void *e){
    vertex_free((Vertex *) e);
}

void *copy_vertex_function(const void *e){
    return vertex_copy((Vertex *) e);
}

int print_vertex_function(FILE * f, const void *e){
    return vertex_print(f, (Vertex *) e);
}

int cmp_vertex_function_by_id(const void *e1, const void *e2){
    /* we implement this function again to have control over the fields being compared */
    /* instead of simply calling to
    return vertex_cmp((Vertex *) e1, (Vertex *) e2);
    */
	Vertex * aux1, * aux2;
	int diff;

	if (!e1 || !e2) return 0;

	aux1 = (Vertex*) e1;
	aux2 = (Vertex*) e2;

	diff = vertex_getId(aux1) - vertex_getId(aux2);
	if (diff == 0) {
		diff = strcmp(vertex_getTag(aux1), vertex_getTag(aux2));
	}

	return diff;
}

int cmp_vertex_function_by_tag(const void *e1, const void *e2){
	Vertex * aux1, * aux2;
	int diff;

	if (!e1 || !e2) return 0;

	aux1 = (Vertex*) e1;
	aux2 = (Vertex*) e2;

	diff = strcmp(vertex_getTag(aux1), vertex_getTag(aux2));
	if (diff == 0) {
    	diff = vertex_getId(aux1) - vertex_getId(aux2);
	}

	return diff;
}

int compare_by_id(const void *b, const void *a){
    Vertex **e1, **e2;
    e1 = (Vertex **) a;
    e2 = (Vertex **) b;
    return cmp_vertex_function_by_id(*e1, *e2);
}

int compare_by_tag(const void *b, const void *a){
    Vertex **e1, **e2;
    e1 = (Vertex **) a;
    e2 = (Vertex **) b;
    return cmp_vertex_function_by_tag(*e1, *e2);
}

int getNumberLines(FILE * pf){
    char line[MAX_LINE];
    int n = 0;
    while (fgets(line, MAX_LINE, pf) != NULL) {
	n++;
    }
    return n;
}

void loadDataFromFile(FILE * pf, Vertex *** pdata){
    Vertex **paux = *pdata;
    Vertex *p;

    while (!feof(pf)) {
        p = read_vertex_from_file(pf);
        *paux = vertex_copy(p);
        paux++;
        vertex_free(p);
    }
}

void balance(Vertex ** sorted_data, BSTree * t, int first, int last){
    int middle = (first + last) / 2;
    Vertex *p;

    if (first <= last) {
        p = *(&(sorted_data[0]) + middle);
        if (tree_insert(t, p) == ERROR) {
            fprintf(stdout, "Vertex ");
            print_vertex_function(stdout, p);
            fprintf(stdout, " not inserted!\n");
        }

        balance(sorted_data, t, first, middle - 1);
        balance(sorted_data, t, middle + 1, last);
    }
}

BSTree *loadBalancedTreeFromData(Vertex ** data, int n, P_tree_ele_cmp cmp_f){
    BSTree *t;

    t = tree_init(print_vertex_function, cmp_f);

    balance(data, t, 0, n - 1);

    return t;
}

BSTree *loadTreeFromData(Vertex ** data, int n, P_tree_ele_cmp cmp_f){
    BSTree *t;
    Vertex *v;
    int i;

    t = tree_init(print_vertex_function, cmp_f);

    for (i = 0; i < n; i++) {
        v = data[i];
        if (tree_insert(t, v) == ERROR) {
            fprintf(stdout, "Vertex ");
            print_vertex_function(stdout, v);
            fprintf(stdout, " not inserted!\n");
        }
    }

    return t;
}

int main(int argc, char const *argv[]){
    FILE *f_in = NULL, *f_out = NULL;
    BSTree *pa = NULL;
    Vertex **data, *vertex;
    const char *cmp_type, *desc;
    int i, n;
    time_t time;
    double time_in_secs;
    P_tree_ele_cmp cmp_fun;
    P_tree_ele_cmp qsort_fun;


    if (argc != 4) {
        printf("Usage: %s vertex_file vertex_desc cmp_type[id|tag]\n", argv[0]);
        return -1;
    }

    cmp_type = argv[3];
    if (strcmp(cmp_type, "id") == 0){
        cmp_fun = cmp_vertex_function_by_id;
        qsort_fun = compare_by_id;
    }else if (strcmp(cmp_type, "tag") == 0){
        cmp_fun = cmp_vertex_function_by_tag;
        qsort_fun = compare_by_tag;
    }else{
        printf("Incorrect cmp_type: %s\n", cmp_type);
        return -1;
    }

    f_in = fopen(argv[1], "r");
    if (!f_in) {
    	return -1;
    }

    f_out = stdout;

    desc = argv[2];
    vertex = vertex_initFromString((char*)desc);
    if (vertex == NULL){
        printf("Error when initialising vertex with description: %s\n", desc);
        fclose(f_in);
        return -1;
    }

    n = getNumberLines(f_in);
    fclose(f_in);
    fprintf(f_out, "%d lines\n", n);
    data = (Vertex **) malloc(n * sizeof(Vertex *));
    f_in = fopen(argv[1], "r");
    loadDataFromFile(f_in, &data);
    fclose(f_in);

    /* We want to compare only the last characters, so it works when debugging */
    if (strcmp("p4_e1", argv[0] + (strlen(argv[0]) - strlen("p4_e1"))) == 0) {
        fprintf(f_out, "**Raw data**\n");
        time = clock();
        pa = loadTreeFromData(data, n, cmp_fun);
        time = clock() - time;
        time_in_secs = ((float) time) / CLOCKS_PER_SEC;
    } else if (strcmp("p4_e1_bal", argv[0] + (strlen(argv[0]) - strlen("p4_e1_bal"))) == 0) {
        qsort(data, n, sizeof(Vertex *), qsort_fun);
        fprintf(f_out, "**Sorted data**\n");
        time = clock();
        pa = loadBalancedTreeFromData(data, n, cmp_fun);
        time = clock() - time;
        time_in_secs = ((float) time) / CLOCKS_PER_SEC;
    }

    fprintf(f_out, "\nTree building time: %ld ticks (%f seconds)\n", (long) time, time_in_secs);
    fprintf(f_out, "Tree size: %ld\nTree depth: %d\n", tree_size(pa), tree_depth(pa));

    time = clock();
    fprintf(f_out, "Min element in tree: ");
    print_vertex_function(f_out, tree_find_min(pa));
    time = clock() - time;
    time_in_secs = ((float) time) / CLOCKS_PER_SEC;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long) time, time_in_secs);
    time = clock();
    fprintf(f_out, "Max element in tree: ");
    print_vertex_function(f_out, tree_find_max(pa));
    time = clock() - time;
    time_in_secs = ((float) time) / CLOCKS_PER_SEC;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long) time, time_in_secs);

    /*tree_inOrder(f_out, pa);fprintf(f_out, "\n"); */

    time = clock();
    if (tree_contains(pa, vertex) == TRUE) {
    	fprintf(f_out, "\nElement found!\n");
        time = clock() - time;
        time_in_secs = ((float) time) / CLOCKS_PER_SEC;
        fprintf(f_out, "Tree search time: %ld ticks (%f seconds)\n\n", (long) time, time_in_secs);
        time = clock();
        fprintf(f_out, "Removing element in tree: ");
        fprintf(f_out, "%s", tree_remove(pa, vertex)==OK?"OK":"ERR");
        time = clock() - time;
        time_in_secs = ((float) time) / CLOCKS_PER_SEC;
        fprintf(f_out, " - %ld ticks (%f seconds)\n", (long) time, time_in_secs);
        fprintf(f_out, "After removal\n\tTree size: %ld\n\tTree depth: %d\n\n", tree_size(pa), tree_depth(pa));
    } else {
	    fprintf(f_out, "\nElement NOT found!\n");
        time = clock() - time;
        time_in_secs = ((float) time) / CLOCKS_PER_SEC;
        fprintf(f_out, "Tree search time: %ld ticks (%f seconds)\n\n", (long) time, time_in_secs);
    }
    destroy_vertex_function(vertex);

    tree_destroy(pa);
    for (i = 0; i < n; i++) {
    	destroy_vertex_function(data[i]);
    }
    free(data);

    return 0;
}
