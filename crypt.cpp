#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

void showHelp()
{
	cout << "$ crypt [options] <key> <source> [<dest>]\n\n";
	cout << "options:\n";
	cout << "-a, --alphabet=<alphabet>  alphabet — алфавит для работы алгоритма (по умолчанию содержит буквы из латниского алфавита и цифры: AaBbCc..Zz0..9)\n"; 
	cout << "-t, --type=<type> - type может быть 'encode' или 'decode', по умолчанию — encode\n";
	cout << "-h, --help - выводит эту справку\n";
	cout << "key - ключ для шифрования/дешифрования\n";
	cout << "source - исходный файл для шифрования/дешифрования\n";
	cout << "dest - файл, куда будет записан новый, зашифрованный текст. Если не указан, то переписывает source\n";
}
// При дешифровании index+key отрицательные, чтобы считал правильно, меняем option1 на положителное
int mod(int option1,int option2)
{
	while(option1 < 0) option1+=option2;
	return option1%option2; 
}

int main(int argc, char* argv[])
{
	int key, keyIndex, index, i;
	char symbol;
	// false - если шифрование, true - дешифрование
	bool type = false;
	// Переменная для проверки наличия файла для записи
	bool oneFile = true; 
	// Введем стандратный алфавит
	string alphabet="AaBbCcDdEeFfGgHhIiKkLlMmNnOoPpQqRrSsTtVvXxYyZz0123456789";
	// Введем дополнительную переменнуб для обработки argv
	string temp;
	FILE * source;
	FILE * dest;

	// Если аргумент равен 2, то это может быть только справка
	if(argc == 2)
	{
		if((string(argv[1]) == "-h") or (string(argv[1]) == "--help"))
		{
			showHelp();
			return 0;
		}
		else
		{
			cout << "Ошибка!" << endl;
			return -1;
		}
	}
	else if(argc > 2) 
	{
		for(int i=1; i<argc; i++)
		{
			// Если параметр -а, то аргументом i+1 задаем алфавит
			if((!(strcmp(argv[i],"-a")))) 
			{
				i++;
				alphabet = argv[i];
			}
			// Если параметр --alphabet, то также меняется алфавит на новый
			else if(string(argv[i]).find("--alphabet=") + 1) 
			{
				alphabet = string(argv[i]).erase(0,11);
			}
			// Если -t или --type, то меняем тип - шифрование или дешифрование.
			else if(string(argv[i]).find("-t") + 1)
			{
				if(string(argv[i]).find("--type=") + 1)
				{
					temp = string(argv[i]).erase(0,7);
				}
				else
				{
					i++; 
					temp = string(argv[i]);
				}
				// Распознаем, на что изменен тип
				if(temp == "encode")
					type = false;
				else if(temp == "decode")
					type = true;
				else
				{
					cout << "Ошибка!1" <<endl;
					return -1;
				}	
			}
			// Если параметр не тот, что описаны выше, то это ключ
			// Если строку с ключом можно преобразовать в число, то все правильно. Иначе ошибка
			else if(key = atoi(argv[i]))
			{
				if (key < 1)
					{
					cout << "Ошибка!" << endl;
					return -1;				  
				}
				keyIndex = i;
				break;
			}
			else	 
			{
				cout << "Ошибка!2" << endl;
				return -1;
			}
		}
	}	
	// Если файл для записи не задан, то перезаписываем тот, где хранится информация
	if (keyIndex == (argc-2))
	{
		if (!(source = fopen(argv[keyIndex+1],"r+")))
		{
			cout<< "Файл не найден!" << endl;
			return -1;
		}
		 dest = source;
	}
	// Если же файл для записи все-таки задан, то записываем результат в него
	else if (keyIndex == (argc-3))
	{
		if (!(source = fopen(argv[keyIndex+1],"r")))
		{
			cout<< "Файл не найден!" << endl;
			return -1;
		}
		if (!(dest = fopen(argv[keyIndex+2],"w")))
		{
			cout<< "Файл не найден!" << endl;
			return -1;
		}
		oneFile=false;
	}
	else
	{
		cout << "Ошибка!3 " << endl;
	}
	

	// Если дешифрование, то меняем ключ на противоположный
	if(type) key *= -1;
	// Считываем символы из файла, пока он не кончится
	while((symbol=fgetc(source)) != EOF)
	{ 
		// Если файл записи отсутсвует, то возвращаем курсор на 1 влево и перезаписываем каждый символ
		if(oneFile) fseek ( dest , -1 , SEEK_CUR );
		if((index = alphabet.find(symbol)) + 1)
		{
			//шифруем или дешифруем
			symbol=alphabet[mod((index + key),alphabet.length())];
		}
		// Записываем результат в 2-й файл
		fputc(symbol,dest);
	}
	
	fclose(source);
	// Если 2 файла, то 2-й тоже закрываем
	if(!(oneFile)) fclose(dest);
	return 0;
}