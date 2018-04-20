/********************************************************************************

Detalhes do set de instrução

	Tamanho das instruções: 16 bits

	Código das intruções:

		LOAD: 	00
		STORE:	01
		ADD: 	10
		SUB: 	11

	Instruções Tipo 1:

		- Utilizado para operaçções aritméticas (soma, subtração, ...)

             MSB                                      LSB

		(Tipo instr.) (End. Reg 1) (End. Reg 2) (End Reg Dest.)

           2 bits        4 bits        4 bits       6 bits


         - Exemplo: 0b0001000000010010 >>> |01|0000|0001|000010

         	 	Realiza a soma (10 >> tipo da instrução) do registro 0 (0000
 	 	 	 >> end. Reg 1) com o registro 1 (0001 >> end. Reg 2) e salva o resultado
 	 	 	 em registro 2 (000010 >> end. Reg Dest.)


    Instruções Tipo 2:

     	 - Uitlizado para operações de LOAD e STORE

     	       MSB                        LSB

     	 (Tipo instr.) (End Reg) (End Memória de dados)

		    2 bits       4 bits        10 bits

   	   - Exemplo: 0b1000000000010010 >>> |00|0000|0000001010

         	 	Realiza o LOAD (00 >> tipo da instrução) do endereço de
			memória 10 (0000001010 >> end. Memória) para o registro 0
			(0000 >> end. Reg )

********************************************************************************/

#include<iostream>
#include<clocale>
#include<windows.h>
#include<stdio.h>
#include<bitset>

#define DEFAULT_FILENAME_MEMORY "memoria.txt"
#define DEFAULT_FILENAME_INSTRUCTIONS "instrucoes.txt"
#define MAX_FILENAME_SIZE 35

#define MAX_PROG_MEMORY 32
#define MAX_DATA_MEMORY 32

using namespace std;

// Memoria de programa
unsigned int ProgMemory[MAX_PROG_MEMORY];

// Memoria de dados
unsigned int DataMemory[MAX_DATA_MEMORY];

// Registradores
unsigned int PC;
unsigned int Instr;
unsigned int InstrType;
unsigned int RegSourceA;
unsigned int RegSourceB;
unsigned int RegDest;
unsigned int RegAddrMemory;
unsigned int Reg[10];

int instructionsQuantity;

// Prototipos
void decode(void);
void execute(void);

// Relacionado a interface
void reloadFile(FILE** txt, char* nomeArquivo);
void executeInterpreter();
void fileToProgMemory(FILE* file);
void fileToDataMemory(FILE* file);
void showProgMemory();
void showDataMemory();

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

	do
	{
		showMenuHeader();
		cout << "\t\t\tMenu Principal\n";
		cout << "--------------------------------------------------------------------------------\n\n";
		showFileStatus(memFile, memFilename);
		showFileStatus(instrFile, instrFilename);
		cout << "\t\t1 - Gerenciar arquivo\n";
		cout << "\t\t2 - Carregar do arquivo para a memória" << endl;
		cout << "\t\t3 - Listar memória" << endl;
		cout << "\t\t4 - Executar" << endl;
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
			fileToProgMemory(instrFile);
			fileToDataMemory(memFile);
			break;
		case '3':
			showMenuHeader();
			cout << "\t\t\tListagem da memória\n";
			cout << "--------------------------------------------------------------------------------\n";

			showProgMemory();
			showDataMemory();

			cout << "\n\t\tReg" << endl;
			for(int i = 0; i < 10; i++)
			{
				cout << i << ":\t" << Reg[i] << endl;
			}

			cout << endl;

			system("pause");
			break;
		case '4':
			showMenuHeader();
			cout << "\t\t\tExecução\n";
			cout << "--------------------------------------------------------------------------------\n";

			executeInterpreter();

			cout << "Instruções executadas.\n\n";

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

void fileToProgMemory(FILE* file)
{
	char str[18];

	int i = 0;
	if (file)
	{
		while(fgets(str, sizeof(str), file) != NULL)
		{
			ProgMemory[i] = (unsigned int)(strtol(str, NULL, 2));

			i++;
		}
	}

	instructionsQuantity = i;
}

void fileToDataMemory(FILE* file)
{
	char str[18];

	int i = 0;
	if (file)
	{
		while(fgets(str, sizeof(str), file) != NULL)
		{
			DataMemory[i] = (unsigned int)(strtol(str, NULL, 10));

			i++;
		}
	}

}

void showProgMemory()
{
	cout << "\n\tProgMemory\n";
	for(int i = 0; i < MAX_PROG_MEMORY; i++)
	{
		cout << i << ":\t" << bitset<16>(ProgMemory[i]) << endl;
	}
}

void showDataMemory()
{
	cout << "\n\tDataMemory\n";
	for(int i = 0; i < MAX_DATA_MEMORY; i++)
	{
		cout << i << ":\t" << DataMemory[i] << endl;
	}
}

void executeInterpreter()
{
	PC = 0;
	for(int i = 0; i < 10; i++)
	{
		Reg[i] = 0;
	}

	while(PC < instructionsQuantity)
	{
		Instr = ProgMemory[PC]; // busca da instrução na linha do PC do bloco
		PC++;
		decode();    // decodificação
		execute();
	}
}

void decode(void)
{
	InstrType = Instr >> 14;

	if(InstrType == 2 || InstrType == 3)
	{
		// Soma, Subtracao
		RegSourceA = Instr >> 10;
		RegSourceA = RegSourceA & 0b0000000000001111;
		RegSourceB = Instr >> 6;
		RegSourceB = RegSourceB & 0b0000000000001111;
		RegDest = Instr & 0b0000000000001111;
	}
	else if(InstrType == 0)
	{
		/* Load */
		RegDest = Instr >> 10;
		RegDest = RegDest & 0b0000000000001111;
		RegAddrMemory = Instr & 0b0000000011111111;
	}
	else if(InstrType == 1)
	{
		/* Store */
		RegSourceA = Instr >> 10;
		RegSourceA = RegSourceA & 0b0000000000001111;
		RegAddrMemory = Instr & 0b0000000011111111;
	}
}

void execute(void)
{
	if(InstrType == 2)
	{
		// Soma
		Reg[RegDest] = Reg[RegSourceA] + Reg[RegSourceB];
	}
	else if(InstrType == 3)
	{
		// Subtracao
		Reg[RegDest] = Reg[RegSourceA] - Reg[RegSourceB];
	}
	else if(InstrType == 0)
	{
		// Load
		Reg[RegDest] = DataMemory[RegAddrMemory];
	}
	else if(InstrType == 1)
	{
		// Store
		DataMemory[RegAddrMemory] = Reg[RegSourceA];
	}
}
