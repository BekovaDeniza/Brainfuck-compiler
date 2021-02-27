#include <stdio.h>
#include <stdlib.h>
#define MEM_LEN 30000

void except(FILE *file, char *array, int a) {
	free(array);
	fclose(file);

	if (a >= 0) exit(a);
}


int brainfuck(FILE *file) {
//Реализация компилятора
	int mem = 0, i = 0;
	char memory[MEM_LEN + 1];
	//Устанавливаем указатель на конец файла
	//Присваиваем значение переменной количеству символ в файле
	//Возвращаем указатель обратно в начало файла
	fseek(file, 0, SEEK_END);
	int file_len = ftell(file);
	char *array = (char*) malloc(file_len);
	rewind(file);
	
	for (int a = 0; a < file_len; ++a) {
	//Заполнфем массив данными из файла
		array[a] = fgetc(file);
	}

	while (i < file_len) {
	//Проходимся по массиву и выполняем действие описанное в каждой ячейке

		char c = array[i];

		if (c == '>' && (++mem) == MEM_LEN) mem = 0; //Переход к следующей ячейке
		else if (c == '<' && !(mem--)) mem = MEM_LEN - 1; //Переход к предыдущей ячейки
		else if (c == '.') printf("%c", memory[mem]); //Печатаем значение из ячейки
		else if (c == ',') memory[mem] = fgetc(stdin); //Ввод значения и сохраннение в ячейке
		else if (c == '+') ++memory[mem]; //Увеличиваем значение в ячейке на 1
		else if (c == '-') --memory[mem]; //Уменьшаем значение в ячейке на 1
		else if (c == '[' && !memory[mem]) {
			int open = 1; //Считаем количество открывающих скобок

			while (open) { //Цикл продолжается пока скобка не закроется или не дойдем до конца файла
				if (++i == file_len) except(file, array, 0); //Конец программы
				if (array[i] == '[') ++open;
				if (array[i] == ']') --open;
			}
		}

		else if (c == ']' && memory[mem]) { //В принципе тоже самое что и выше
			int close = 1; //Считаем количество закрывающих скобок
			
			while (close) {
				if (--i == -1) except(file, array, 0); //Конец программы
				if (array[i] == ']') ++close;
				if (array[i] == '[') --close;
			}
		}

		else if (!(c == '\t' || c == '\n' 
			   || c == ' ' || c == '<' 
			   || c == '>' || c == '[' || c == ']')) {
			fprintf(stderr, "Неизвестный символ? (%c)\n", c);
			except(file, array, 1);
		}
		++i;
	}

	except(file, array, -1);
	return 0;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Укажите brainfuck файл\n");
		exit(1);
	}

	FILE *file = fopen(argv[1], 'r');	
	if (!file) {
		fprintf(stderr, "Не удалось открыть файл %s\n", argv[1]);
		exit(1);
	}

  return brainfuck(file);
}
