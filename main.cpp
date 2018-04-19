#include<iostream>
#include<clocale>
#include<windows.h>
#include<stdio.h>

#define DEFAULT_FILENAME_MEMORY "memoria.txt"
#define DEFAULT_FILENAME_INSTRUCTIONS "instrucoes.txt"
#define MAX_FILENAME_SIZE 35

using namespace std;

void reloadFile(FILE** txt, char* nomeArquivo);

void showMenuHeader()
{
	system("cls");
	cout << "--------------------------------------------------------------------------------\n";
	cout << "\tEC 208 - Arquitetura de Computadores II\n";
	cout << "\tProjeto: Máquina Virtual/ Interpretador de Alto Nível\n";
	cout << "\tAutores: Bruno Balestra/ Felipe Santos/ Matheus Oliveira\n";
	cout << "--------------------------------------------------------------------------------\n";
}

void showFileStatus(FILE* file, char* filename)
{
	if(file == NULL)
	{
		cout << "\tArquivo '" << filename << "' não carregado ou não encontrado.\n\n";
	}
	else
	{
		cout << "\tArquivo '" << filename << "' carregado.\n\n";
	}
}

void manageFileMenu(FILE* file, char* filename)
{
	char option_1;
	showMenuHeader();
	cout << "\t\t\tGerenciar Arquivo\n";
	cout << "--------------------------------------------------------------------------------\n\n";
	showFileStatus(file, filename);
	cout << "\t\t1 - Recarregar\n";
	cout << "\t\t2 - Fechar arquivo" << endl;
	cout << "\t\t3 - Alterar arquivo" << endl;
	cout << "\n\t\t9 - Voltar ao Menu Principal" << endl;
	cout << "\nPor favor, digite a opção desejada\n> ";
	cin.clear();
	cin.sync();
	cin >> option_1;
	switch(option_1)
	{
	case '1':
		reloadFile(&file, filename);
		break;
	case '2':
		showMenuHeader();
		cout << "\t\t\tFechar arquivo\n";
		cout << "--------------------------------------------------------------------------------\n\n";
		fclose(file);
		file = NULL;
		cout << "Arquivo '" << filename << "' fechado.\n\n";
		system("pause");
		break;
	case '3':
		showMenuHeader();
		cout << "\t\t\tAlterar arquivo\n";
		cout << "--------------------------------------------------------------------------------\n\n";
		cout << "O sistema atualmente busca o arquivo '" << filename << "'.\n\n";
		cout << "Digite o novo nome do arquivo (com a extensão) que o sistema deve buscar\n>";
		cout << "Exemplo: 'arquivo.txt' sem apóstrofos.\n>";
		for(int i = 0; i < MAX_FILENAME_SIZE; i++)
			filename[i] = 0;
		cin.clear();
		cin.sync();
		cin.getline(filename, MAX_FILENAME_SIZE);
		cout << "\nNome do arquivo que o sistema busca alterado para '" << filename << "'.\n\n";
		reloadFile(&file, filename);
		system("pause");
		break;
	case '9':
		break;
	default:
		cout << "\tOpção inválida\n\n";
		system("pause");
	}
}

int main()
{
	char option_1, option_2;

	FILE* memFile = NULL;
	FILE* instrFile = NULL;
	char memFilename[MAX_FILENAME_SIZE] = DEFAULT_FILENAME_MEMORY;
	char instrFilename[MAX_FILENAME_SIZE] = DEFAULT_FILENAME_INSTRUCTIONS;

	setlocale(LC_ALL, "Portuguese");

	reloadFile(&memFile, memFilename);
	reloadFile(&instrFile, instrFilename);

	int i;

	do
	{
		showMenuHeader();
		cout << "\t\t\tMenu Principal\n";
		cout << "--------------------------------------------------------------------------------\n\n";
		showFileStatus(memFile, memFilename);
		showFileStatus(instrFile, instrFilename);
		cout << "\t\t1 - Gerenciar arquivo\n";
		cout << "\t\t2 - Executar" << endl;
		cout << "\n\t\t9 - Sair" << endl;
		cout << "\nPor favor, digite a opção desejada\n> ";
		cin.clear();
		cin.sync();
		cin >> option_1;
		switch(option_1)
		{
		case '1':
			do
			{
				showMenuHeader();
				cout << "\t\t\tGerenciar Arquivo\n";
				cout << "--------------------------------------------------------------------------------\n\n";
				showFileStatus(memFile, memFilename);
				showFileStatus(instrFile, instrFilename);
				cout << "\t\t1 - Arquivo de memória ('" << memFilename << "')\n";
				cout << "\t\t2 - Arquivo de instruções ('" << instrFilename << "')" << endl;
				cout << "\n\t\t9 - Voltar ao Menu Principal" << endl;
				cout << "\nPor favor, digite a opção desejada\n> ";
				cin.clear();
				cin.sync();
				cin >> option_2;
				switch(option_2)
				{
				case '1':
					manageFileMenu(memFile, memFilename);
					break;
				case '2':
					manageFileMenu(instrFile, instrFilename);
					break;
				case '9':
					break;
				default:
					cout << "\tOpção inválida\n\n";
					system("pause");
				}
			}
			while(option_2 != '9');
			break;
		case '2':
			showMenuHeader();
			cout << "\t\t\tExecução\n";
			cout << "--------------------------------------------------------------------------------\n";
			system("pause");
			break;
		case '3':
			showMenuHeader();
			cout << "\t\t\tListagem da memória\n";
			cout << "--------------------------------------------------------------------------------\n";
			system("pause");
			break;
		case '9':
			break;
		default:
			cout << "\tOpção inválida\n\n";
			system("pause");
		}
	}
	while(option_1 != '9');
	fclose(memFile);
	fclose(instrFile);
	return 0;
}

void reloadFile(FILE** file, char* filename)
{
	fclose(*file);
	*file = NULL;
	*file = fopen(filename, "r+");
}
