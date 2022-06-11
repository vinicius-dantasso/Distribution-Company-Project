#include "prot.h"

DinamicVet* Pedir(char* arquivo, DinamicVet* estoque) {
	//conversão do arquivo .txt para o recibo em .nfc
	char newArquivo[30];
	for (int i = 0; i <= strlen(arquivo); i++) {
		newArquivo[i] = arquivo[i];
	}
	int j = 0;
	while (newArquivo[j] != '.') {
		j++;
	}
	newArquivo[j] = '\0';
	strcat_s(newArquivo, ".nfc");

	//leitura do arquivo .txt e criação do recibo em .nfc
	ofstream fout;
	ifstream input;

	const int tam = 30;
	char reading[30];
	Pedidos* recibo = new Pedidos[tam];

	input.open(arquivo, ios_base::in);

	//Leitura das primeiras palavras e caracteres do pedido
	int contador = 0;
	while (contador < 2 && !(input.eof())) {
		input.getline(reading, 30);
		contador++;
	}

	int tam2 = 0;
	while (!(input.eof())) { //Leitura dos itens pedidos e passá-los para o vetor recibo[]

		input >> recibo[tam2].prod;

		for (int l = 0; l <= strlen(recibo[tam2].prod); l++) {
			recibo[tam2].prod[l] = tolower(recibo[tam2].prod[l]);
		}

		input >> recibo[tam2].quantidade;

		int result = 1, update = 0, m = 0;
		while (result != 0 && m < tam2) {
			result = strcmp(recibo[tam2].prod, recibo[m].prod);
			if (result == 0) {
				update = m;
			}
			m++;
		}

		if (result == 0) {
			recibo[update].quantidade = recibo[tam2].quantidade + recibo[update].quantidade;
			tam2--;
		}
		tam2++;
	}
	
	input.close();

	// Primeira Verificação:
	int result = 1, cont = estoque->size - 1, num = 0;
	bool verify = true, show = true;
	for (int m = 0; m < tam2; ++m) {  //Irá passar por todos os itens do pedido e do estoque e caso haja problema em algum avisará de imediato e cancelará a criação do recibo
		while (result != 0 && cont >= 0) {
			result = strcmp(recibo[m].prod, estoque[cont].gaveta.produto);

			if (result == 0) {
				num = cont;
				if (recibo[m].quantidade > estoque[num].gaveta.quant) {
					if (show) {
						cout << "\n\nPedido Falhou!\n";
					}
					cout << recibo[m].prod << ": Solicitado = " << recibo[m].quantidade
						<< "Kg /  Em Estoque = " << estoque[num].gaveta.quant << "Kg\n";
					verify = false;
					show = false;
				}
			}
			if (result != 0 && cont == 0) {
				if (show) {
					cout << "\n\nPedido Falhou!\n";
				}
				cout << recibo[m].prod << " está em falta!\n";
				verify = false;
				show = false;
			}
			cont--;
		}
		cont = estoque->size - 1;
		result = 1;
	}

	// Segunda Verificação:
	if (show) { // Caso tenha passado pela primeira verific. irá abrir o arquivo de recibo
		fout.open(newArquivo, ios_base::out);
		input.open(arquivo, ios_base::in);

		contador = 0;
		while (contador < 2 && !(input.eof())) {
			input.getline(reading, 30);
			if (contador == 0) {
				fout << reading;
			}
			contador++;
		}
		fout << "\n-------------------------------------------------------\n";

		input.close();
	}

	result = 1, cont = estoque->size - 1, num = 0;
	if (verify) {                                  // Escrita do recibo em .nfc
		float valor = 0, valorTotal = 0, desconto = 0;

		for (int o = 0; o < tam2; ++o) {
			while (result != 0 && cont >= 0) {
				result = strcmp(recibo[o].prod, estoque[cont].gaveta.produto);

				if (result == 0) {
					num = cont;
				}
				cont--;
			}

			if (recibo[o].quantidade <= estoque[num].gaveta.quant) {
				estoque[num].gaveta.quant = estoque[num].gaveta.quant - recibo[o].quantidade;
				valor = recibo[o].quantidade * estoque[num].gaveta.preco;
				valorTotal += valor;

				recibo[o].prod[0] = toupper(recibo[o].prod[0]);
				fout << fixed; fout.precision(2); fout.fill('0');

				fout << left; fout.width(8); fout.fill(' ');
				fout << recibo[o].prod;
				fout << right; fout.width(5); fout.fill(' ');
				fout << recibo[o].quantidade << "Kg";
				fout << right; fout.width(5); fout.fill(' ');
				fout << "a";
				fout << right; fout.width(8); fout.fill(' ');
				fout << estoque[num].gaveta.preco << "/Kg";
				fout << right; fout.width(5); fout.fill(' ');
				fout << "=";
				fout << right; fout.width(5); fout.fill(' ');
				fout << "R$" << valor << endl;
			}

			cont = estoque->size - 1;
			result = 1;

			if (o == tam2 - 1) {
				fout << fixed; fout.precision(2); fout.fill('0');

				fout << "-------------------------------------------------------\n";

				fout << right; fout.width(30); fout.fill(' ');
				fout << "Compra";
				fout << right; fout.width(6); fout.fill(' ');
				fout << "=";
				fout << right; fout.width(5); fout.fill(' ');
				fout << "R$" << valorTotal << endl;

				if (valorTotal > 1000) {
					desconto = 0.1 * valorTotal;
					valorTotal = valorTotal - desconto;

					fout << right; fout.width(30); fout.fill(' ');
					fout << "Desconto";
					fout << right; fout.width(6); fout.fill(' ');
					fout << "=";
					fout << right; fout.width(5); fout.fill(' ');
					fout << "R$" << desconto << endl;
				}
				else {
					fout << right; fout.width(30); fout.fill(' ');
					fout << "Desconto";
					fout << right; fout.width(6); fout.fill(' ');
					fout << "=";
					fout << right; fout.width(5); fout.fill(' ');
					fout << "R$" << desconto << endl;
				}

				fout << right; fout.width(30); fout.fill(' ');
				fout << "Total";
				fout << right; fout.width(6); fout.fill(' ');
				fout << "=";
				fout << right; fout.width(5); fout.fill(' ');
				fout << "R$" << valorTotal << endl;
			}

		}
		cout << "Recibo feito com sucesso!\n";
	}

	fout.close();
	delete[] recibo;
	return estoque;
}

void Listar(DinamicVet* estoque, unsigned short size, char* prod) { //Lista os elementos do vetor estoque
	for (int i = 0; i < size; i++) {
		prod[0] = toupper(estoque[i].gaveta.produto[0]);
		for (int j = 1; j <= strlen(estoque[i].gaveta.produto); j++) {
			prod[j] = estoque[i].gaveta.produto[j];
		}

		cout << left; cout.width(8); cout.fill(' ');
		cout << prod;

		cout << fixed; cout.precision(2); cout.fill('0');

		cout << right; cout.width(3); cout.fill(' ');
		cout << "-";
		cout << right; cout.width(4); cout.fill(' ');
		cout << "R$" << estoque[i].gaveta.preco;
		cout << right; cout.width(4); cout.fill(' ');
		cout << "-";
		cout << right; cout.width(4); cout.fill(' ');
		cout << estoque[i].gaveta.quant << "Kg\n";
		
	}
}

DinamicVet* Adicionar(DinamicVet* Estoque, unsigned short Size, unsigned short Val, unsigned short N) {
	if (Size == Val) {        //Criação do vetor dinâmico que continua crescendo
		DinamicVet* newEstoque = new DinamicVet[Size + pow(2,N)];
		for (int i = 0; i < Size; i++) {
			newEstoque[i] = Estoque[i];
		}
		delete[] Estoque;
		Estoque = newEstoque;
		newEstoque = 0;
		
		Val = Size + pow(2, N);
		Estoque->cont = Val;
		N++;
		Estoque->n = N;
	}
	if (Size != Val) {  //Inserção dos itens desejados no vetor de estoque
		cout << "Produto: ";
		cin >> Estoque[Size].gaveta.produto;

		for (int i = 0; i <= strlen(Estoque[Size].gaveta.produto); i++) { //Passa todas letras do produto para miniscúlas
			Estoque[Size].gaveta.produto[i] = tolower(Estoque[Size].gaveta.produto[i]);
		}

		int result = 1, update = 0, j = 0;
		while (result != 0 && j < Size) { //Verificação se o produto então inserido é um mesmo já existente
			result = strcmp(Estoque[Size].gaveta.produto, Estoque[j].gaveta.produto);
			if (result == 0) {
				update = j;
			}
			j++;
		}

		if (result == 0) { //Caso verdadeiro na Verific. irá apenas atualizar o produto de mesmo nome
			cout << "Preco: ";
			cin >> Estoque[update].gaveta.preco;
			cout << "Quant: ";
			cin >> Estoque[update].gaveta.quant;
			Size--;
			Estoque->size = Size;
		}
		else { //Caso contrário irá inserir os dados para o novo produto
			cout << "Preco: ";
			cin >> Estoque[Size].gaveta.preco;
			cout << "Quant: ";
			cin >> Estoque[Size].gaveta.quant;
		}
		Size++;
		Estoque->size = Size;
	}
	return Estoque;
}

DinamicVet* Excluir(DinamicVet* ptrEstoque, unsigned short size, char* prod) {
	int i = 0;
	for (i; i < size; i++) { //Lista os produtos do vetor estoque
		prod[0] = toupper(ptrEstoque[i].gaveta.produto[0]);
		for (int j = 1; j <= strlen(ptrEstoque[i].gaveta.produto); j++) {
			prod[j] = ptrEstoque[i].gaveta.produto[j];
		}
		cout << i + 1 << ") " << prod << endl;
	}

	char ch2;
	int numProd;

	cout << "Número do produto: ";
	cin >> numProd;
	
	while (numProd > i) {
		cout << "Opção Inválida!\n";
		cout << "Número do produto: ";
		cin >> numProd;
	}

	cout << "Deseja excluir """ << ptrEstoque[numProd - 1].gaveta.produto << """ (S/N)? ";
	cin >> ch2;
	if (ch2 == 'S' || ch2 == 's') {
		for (int i = numProd; i < size; i++) { //Exclui o item escolhido
			ptrEstoque[i - 1] = ptrEstoque[i];
		}
		cout << "Produto excluído!\n";
		size--;
		ptrEstoque->size = size;
	}
	else {
		cout << "Produto mantido!\n";
	}
	return ptrEstoque;
}