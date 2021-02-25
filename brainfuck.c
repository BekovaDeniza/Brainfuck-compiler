#include <stdio.h>
#include <stdlib.h>
#define MEM_LEN 30000

void except(FILE *file, char *source, int a) {
	free(source);
	fclose(file);

	if (a >= 0) exit(a);
}

int brainfuck(FILE *file) {
//Реализация компилятора
	int mem = 0, code = 0;
	char memory[MEM_LEN + 1];
	fseek(file, 0, SEEK_END); //Устанавливаем указатель на конец файла
	int source_len = ftell(file); //Присваиваем значение указателя
	char *source = (char*) malloc(source_len);
	rewind(file); //Возвращаем указатель в начало файла
	
	for (int a = 0; a < source_len; a++) {
	//Заполнфем массив данными из файла
		source[a] = fgetc(file);
	}
	
	while (code < source_len) {
		char c = source[code];
		
		if (c == '>' && (++mem) == MEM_LEN) mem=0; //Переход к следующей ячейке
		else if (c == '<' && !(mem--)) mem = MEM_LEN - 1; //Переход к предыдущей ячейки
		else if (c == '.') printf("%c", memory[mem]); //Печатаем значение из ячейки
		else if (c == ',') memory[mem] = fgetc(stdin); //Ввод значения и сохраннение в ячейке
		else if (c == '+') ++memory[mem]; //Увеличиваем значение в ячейке на 1
		else if (c == '-') --memory[mem]; //Уменьшаем значение в ячейке на 1
		else if (c == '[' && !memory[mem]) {
			int open = 1; //Считаем количество открывающих скобок

			while (open) {
				if (++code == source_len) except(file, source, 0);
				if (source[code] == '[') ++open;
				if (source[code] == ']') --open;
			}
		}
		else if (c == ']' && memory[mem]) {
			int close = 1; //Считаем количество закрывающих скобок
			while (close) {
				if (--code == -1) except(file, source, 0);
				if (source[code] == ']') ++close;
				if (source[code] == '[') --close;
			}
		}

		else if (!(c == '\t' || c == '\n' 
			   || c == ' ' || c == '<' 
			   || c == '>' || c == '[' || c == ']')) {
			fprintf(stderr, "Неизвестный символ? (%c)\n", c);
			except(file, source, 1);
		}
		++code;
	}

	except(file, source, -1);
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Укажите brainfuck файл\n");
		exit(1);
	}

	FILE *source = fopen(argv[1], "r");	
	if (!source) {
		fprintf(stderr, "Не удалось открыть файл %s\n", argv[1]);
		exit(1);
	}

  return brainfuck(source);
}
