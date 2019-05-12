CC=g++-8
CFLAGS=-std=c++17 -Wall -O3
USANDO_VERSAO_MODERNA = USANDO_VERSAO_MODERNA

build:
	$(CC) main.cpp -o main $(CFLAGS)
test:
	$(CC) teste.cpp -o teste $(CFLAGS)
nova:
	$(CC) main.cpp -o main $(CFLAGS) -lstdc++fs -D$(USANDO_VERSAO_MODERNA)
