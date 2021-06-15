#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <errno.h>
#include <string.h>


char arr[1000] = { 0 };
char line[512];
int i = 0;
int comment = 0;


void analiser();
void offset(int loc, int leng);
int check(char* check, int location, int leng);
int enclosure();
void deleter(int i);
void delcomments();

int main()
{
	setlocale(LC_ALL, "Rus");
	int strcounter = 0;
	int cycle = 0;
	FILE* read;
	read = fopen("rfile.txt", "rt");
	if (read != NULL)
	{
		// считывание данных из текстового файла
		while (fgets(line, 512, read) != NULL)
		{
			delcomments();
			strcounter++;
		}
		// проверка ошибки в потоке 
		if (ferror(read))
		{
			printf("Файл поврежден.");
			return 1;
		}
		// проверка текстового файла на пустоту
		if (strcounter == 0)
		{
			printf("\tФайл пустой.\n");
			return 0;
		}
		// закрытие текстового файла 
		fclose(read);
		cycle = enclosure();
		if (cycle == 0)
		{
			printf("\tВложенных циклов в программе нет.\n");
		} else
		printf("\tВложенность циклов в данной программе равна %d\n", cycle);
	}
	else
	{
		printf("\tФайл не найден.\n");
		return 2;
	}
	return 0;
}
//Функция удаляющая комментарии
void delcomments()
{
	char* symb1;
	char* symb2;
	char* symb3;
	int length = strlen(line);
	symb1 = strstr(line,"//");
	symb2 = strstr(line, "/*");
	symb3 = strstr(line, "*/");
	if (symb1 == NULL)
	{
		if (symb2 == NULL)
		{
			if (symb3 == NULL)
			{
				if (comment == 0)
				{
					analiser();
				}
			}
			else
			{
				comment--;
				int result = (int)(symb3 - line);
				if (result != 0)
				{
					offset(result, 1);
				}
				analiser();
			}
		}
		else
		{
			comment++;
			int result = (int)(symb2 - line);
			if (result != 0)
			{
				
				int res = (int)(symb3 - line);
				if (symb3 == NULL)
				{
					res = 0;
				}
				else
				{
					comment--;
					res = length - res;
				}
				for (int k = result; k < length-res+2; k++)
				{
					line[k] = ' ';
				}
				analiser();
			}
		}
	}
	else
	{
		int result = (int)(symb1 - line);
		if (result != 0)
		{
			for (int k = result; k < length; k++)
			{
				line[k] = ' ';
			}
			analiser();
		}
	}
}
//Функция анализирующая считанную строку
void analiser()
{
	char* symb;
	int counter = 0;//счетчик найденных в строке элементов
	int farr = 0;//номер for в считаннной строке
	int warr = 0;//номер while в считаннной строке
	int darr = 0;//номер do в считаннной строке
	int iarr = 0;//номер if в считаннной строке
	int place = 0;//номер элемента в считанной строке
	int result = -1;//наличие в строке необходимых данных
	int bracketin = 1000;//номер открывающейся скобки в считанной строке
	int bracketout = 1000;//номер закрывающейся скобки в считанной строке
	int semicolon = 1000;//номер точки с запятой в считанной строке
	while (1)
	{
		farr = check("for", 1000, 3);
		warr = check("while", 1000, 5);
		darr = check("do", 1000, 2);
		iarr = check("if", 1000, 2);
		symb = strchr(line, '{');
		if (symb != 0)
			bracketin = (int)(symb - line);
		symb = strchr(line, '}');
		if (symb != 0)
			bracketout = (int)(symb - line);
		symb = strchr(line, ';');
		if (symb != 0)
			semicolon = (int)(symb - line);
		if ((farr < warr) && (farr < darr) && (farr < iarr) && (farr < bracketin) && (farr < bracketout) && (farr < semicolon))
		{
			arr[i] = 'f';
			i++;
			symb = strchr(line, ')');
			result = (int)(symb - line);
			offset(result, 1);
			counter++;
		} else
		if ((warr < farr) && (warr < darr) && (warr < iarr) && (warr < bracketin) && (warr < bracketout) && (warr < semicolon))
		{
			symb = strchr(line, ')');
			place = (int)(symb - line);
			if (line[place + 1] != ';')
			{
				arr[i] = 'w';
				result = place;
				counter++;
			}
			else
			{
				arr[i] = ' ';
				result = place + 1;
			}
			i++;
			offset(result, 1);
		} else
		if ((darr < warr) && (darr < farr) && (darr < iarr) && (darr < bracketin) && (darr < bracketout) && (darr < semicolon))
		{
			arr[i] = 'd';
			i++;
			result = darr + 1;
			offset(result, 1);
			counter++;
		} else
		if ((iarr < warr) && (iarr < farr) && (iarr < darr) && (iarr < bracketin) && (iarr < bracketout) && (iarr < semicolon))
		{
			arr[i] = 'i';
			i++;
			symb = strchr(line, ')');
			result = (int)(symb - line);
			offset(result, 1);
			counter++;
		} else
		if((bracketin < warr) && (bracketin < farr) && (bracketin < darr) && (bracketin < iarr) && (bracketin < bracketout) && (bracketin < semicolon))
		{
			arr[i] = '{';
			i++;
			result = bracketin;
			offset(result, 1);
			counter++;
		} else
		if ((bracketout < warr) && (bracketout < farr) && (bracketout < darr) && (bracketout < iarr) && (bracketout < bracketin) && (bracketout < semicolon))
		{
			arr[i] = '}';
			i++;
			result = bracketout;
			offset(result, 1);
			counter++;
		}
		else
		if ((semicolon < warr) && (semicolon < farr) && (semicolon < darr) && (semicolon < iarr) && (semicolon < bracketin) && (semicolon < bracketout))
		{
			arr[i] = ' ';
			i++;
			result = semicolon;
			offset(result, 1);
			counter++;
		}
		else
		{
			if (counter == 0)
			{
				arr[i] = ' ';
				i++;
			}
			break;
		}
		bracketin = 1000;
		bracketout = 1000;
		semicolon = 1000;
	}
}
//Функция удаляющая часть строки в массиве
void offset(int loc, int leng)
{
	int length = strlen(line);
	int k = 0;
	for (int i = loc + leng; i < length+1; i++)
	{
		line[k] = line[i];
		line[i] = ' ';
		k++;
	}
}
//Функция определяющая находится ли название цикла в строке
int check(char* check, int location, int leng)
{
	char* word;
	int result;
	word = strstr(line, check);
	result = (int)(word - line);
	if (word == NULL)
	{
		return location;
	}
	if (result == 0)
	{
		if ((line[result + leng] == ' ') || (line[result + leng] == '(') || (line[result + leng] == '{') || (line[result + leng] == '\n'))
		{
			if (result < location)
			{
				return result;
			}
		}
	}
	if (result > 0)
	{
		if ((line[result - 1] == ' ')||(line[result - 1] == '\t'))
		{
			if ((line[result + leng] == ' ') || (line[result + leng] == '(') || (line[result + leng] == '{') || (line[result + leng] == '\n'))
			{
				if (result < location)
				{
					return result;
				}
			}
		}
	}
	return location;
}
//Функция определяющая вложенность цикла
int enclosure()
{
	int count = 0;//текущая вложенность
	int max = 0;//максимальная вложенность
	int openedcycle = 0;//скобки цикла
	int currbracket = 0;//номер скобки открывающейся при цикле
	int brackets = 0;//скобки
	int place = 0;//расположение скобки
	int cyclenum = 0;//вложенность при открытии скобки при цикле
	int length = strlen(arr);
	for (i = 0; i < length; i++)
	{
		if (arr[i+1] == 'i')
		{
			deleter(i+1);
		}
		if (arr[i] == '{')
		{
			brackets++;
		}
		if ((arr[i] == 'f') || (arr[i] == 'w') || (arr[i] == 'd'))
		{
			if (i > 0)
			{
				if ((arr[i - 1] == ' ') || (arr[i - 1] == '{') || (arr[i - 1] == '}'))
				{
					if (openedcycle > 0)
					{
						count++;
					}
				}
			}
			if (arr[i + 1] == '{')
			{
				openedcycle++;
				currbracket = brackets;
				place = i;
				cyclenum = count;
			} 
			else if ((arr[i + 1] == 'f') || (arr[i + 1] == 'w') || (arr[i + 1] == 'd'))
			{
				count++;
			} 
			else if ((arr[i + 1] == ' ') || (arr[i + 1] == '}'))
			{
				if (openedcycle > 0)
				{
					count = cyclenum;
				}
				else
				{
					count = 0;
				}
			}
		}
		if (arr[i] == '}')
		{
			brackets--;
			if (currbracket == brackets)
			{
				if (place != i)
				{
					openedcycle--;
				}
			}
		}
		if (max < count)
		{
			max = count;
		}
	}
	return max;
}
//Функция удаляющая элемент массива
void deleter(int i)
{
	int length = strlen(arr);
	int k;
	for (k = i; k < length - 1; k++)
	{
		arr[k] = arr[k + 1];
		arr[k + 1] = ' ';
	}
}