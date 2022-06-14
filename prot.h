#include <iostream>
#include <fstream>
#include <cmath>
#include <cctype>
using namespace std;

struct Storage {
	char produto[30];
	float preco;
	unsigned int quant;
};

struct DinamicVet {
	Storage gaveta;
	unsigned short size;
	unsigned short cont;
	unsigned short n;
};

struct Pedidos { //Registro para guardar os pedidos do arquivo .txt
	char prod[30];
	int quantidade;
};

DinamicVet* Pedir(char* arquivo, DinamicVet* estoque);
DinamicVet* Adicionar(DinamicVet* ptrEstoque, unsigned short Size, unsigned short Val, unsigned short N);
DinamicVet* Excluir(DinamicVet* ptrEstoque, unsigned short size, char* prod);
void Listar(DinamicVet* estoque, unsigned short size, char* prod);