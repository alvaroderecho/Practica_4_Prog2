# include this file in your main Makefile as
# include makefile_ext

run_e1: clean_e1 p4_e1 p4_e1_bal
	./p4_e1 data50K.txt "id:88997 tag:Golf_de_Ibiza" id
	./p4_e1_bal data50K.txt "id:88997 tag:Golf_de_Ibiza" id
	./p4_e1 data50K.txt "id:88997 tag:Golf_de_Ibiza" tag
	./p4_e1_bal data50K.txt "id:88997 tag:Golf_de_Ibiza" tag
	./p4_e1 data50K.txt "id:889907 tag:Golf_de_Ibiza" id
	./p4_e1_bal data50K.txt "id:889907 tag:Golf_de_Ibiza" id
	./p4_e1 data50K.txt "id:889907 tag:Golf_de_Ibiza" tag
	./p4_e1_bal data50K.txt "id:889907 tag:Golf_de_Ibiza" tag

clean_e1:
	rm -f p4_e1.o p4_e1 p4_e1_bal

p4_e1: p4_e1.o bstree.o vertex.o
	gcc -o p4_e1 p4_e1.o bstree.o vertex.o

p4_e1_bal: p4_e1
	gcc -o p4_e1_bal p4_e1.o bstree.o vertex.o

p4_e1.o: p4_e1.c
	gcc -g -Wall -pedantic -ansi -c p4_e1.c

run_e2: clean_e2 p4_e2 p4_e2_bal
	./p4_e2 data50K.txt id
	./p4_e2_bal data50K.txt id
	./p4_e2 data50K.txt tag
	./p4_e2_bal data50K.txt tag
	./p4_e2 data10.txt id
	./p4_e2_bal data10.txt id
	./p4_e2 data10.txt tag
	./p4_e2_bal data10.txt tag

clean_e2:
	rm -f p4_e2.o p4_e2 p4_e2_bal

p4_e2: p4_e2.o search_queue.o bstree.o vertex.o
	gcc -o p4_e2 p4_e2.o search_queue.o bstree.o vertex.o

p4_e2_bal: p4_e2
	gcc -o p4_e2_bal p4_e2.o search_queue.o bstree.o vertex.o

p4_e2.o: p4_e2.c
	gcc -g -Wall -pedantic -ansi -c p4_e2.c

# Rules for bstree, vertex, ... in main Makefile
