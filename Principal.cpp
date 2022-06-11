#include "prot.h"

int main() {
	ofstream fout;
	ifstream fin;

	DinamicVet estoque;
	DinamicVet* ptrEstoque = &estoque; //Ponteiro apontando para o endereço de estoque
	unsigned short size = 0, val = 0, n = 0;

	fin.open("armazem.dat", ios_base::in | ios_base::binary); //Abertura do arquivo binário para ler as informações salvas nele
	if (fin.is_open()) {
		fin.read((char*)&size, sizeof(unsigned short));
		fin.read((char*)&val, sizeof(unsigned short));
		fin.read((char*)&n, sizeof(unsigned short));

		ptrEstoque = new DinamicVet[val];
		ptrEstoque->size = size;
		ptrEstoque->cont = val;
		ptrEstoque->n = n;

		for (int i = 0; i < size; i++) {
			fin.read((char*)&ptrEstoque[i].gaveta, sizeof(ptrEstoque[i].gaveta));
		}
	}
	else { //Caso não abra inicializa o vetor vazio
		ptrEstoque = new DinamicVet[size];
		ptrEstoque->size = 0;
		size = ptrEstoque->size;
		ptrEstoque->cont = 0;
		val = ptrEstoque->cont;
		ptrEstoque->n = 0;
		n = ptrEstoque->n;
	}
	fin.close();

	char arquivo[30];
	char nomeProd[30];

	setlocale(LC_ALL, "portuguese");
	
	char ch;
	cout << "Sistema de Controle\n";
	cout << "=====================\n";
	cout << "(P)edir\n"
		<< "(A)dicionar\n"
		<< "(E)xcluir\n"
		<< "(L)istar\n"
		<< "(S)air\n";
	cout << "=====================\n";
	cout << "Opção: [ ]\b\b"; 
	cin >> ch;
	system("CLS");

	while (ch != 'S' && ch != 's') {
		switch (ch) {
			case 'P':
			case 'p':
				//Aqui será feito os pedidos
				cout << "Pedir\n"
					<< "-----\n";
				cout << "Arquivo: ";
				cin >> arquivo;

				ptrEstoque = Pedir(arquivo, ptrEstoque);

				system("pause");
				system("CLS");
				break;

			case'A':
			case'a':
				//Aqui será feito as adições e atualizações ao vetor estoque
				cout << "Adicionar\n";
				cout << "---------\n";
				
				ptrEstoque = Adicionar(ptrEstoque, size, val, n);
				size = ptrEstoque->size;
				val = ptrEstoque ->cont;
				n = ptrEstoque->n;
				
				system("CLS");
				break;

			case'E':
			case'e':
				//Aqui será feito a exclusão de um item do vetor estoque
				cout << "Excluir\n";
				cout << "-------\n";
				
				ptrEstoque = Excluir(ptrEstoque, size, nomeProd);
				size = ptrEstoque->size;

				system("pause");
				system("CLS");
				break;

			case'L':
			case'l':
				//Listagem dos itens do vetor estoque
				cout << "Listagem\n"
					<< "--------\n";

				Listar(ptrEstoque, size, nomeProd);

				system("pause");
				system("CLS");
				break;

			case'S':
			case's':
				break;

			default:
				cout << "Opção inválida!\n";
				system("pause");
				system("CLS");
				break;
		}

		cout << "Sistema de Controle\n";
		cout << "=====================\n";
		cout << "(P)edir\n"
			<< "(A)dicionar\n"
			<< "(E)xcluir\n"
			<< "(L)istar\n"
			<< "(S)air\n";
		cout << "=====================\n";
		cout << "Opção: [ ]\b\b";
		cin >> ch;
		system("CLS");

	}

	fout.open("armazem.dat", ios_base::out | ios_base::binary);// Abertura/Criação do arquivo binário que guardará as informações do vetor estoque
	fout.write((char*)&size, sizeof(unsigned short));
	fout.write((char*)&val, sizeof(unsigned short));
	fout.write((char*)&n, sizeof(unsigned short));

	for (int i = 0; i < size; i++) {
		fout.write((char*)&ptrEstoque[i].gaveta, sizeof(ptrEstoque[i].gaveta));
	}
	fout.close();
	delete[] ptrEstoque;
}