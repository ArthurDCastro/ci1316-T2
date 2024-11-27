# Nome do executável
EXEC = multi_partition

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -pthread -O2

# Arquivos fonte
SRC = main.c multi_partition.c util.c chrono.c
# Arquivo de cabeçalho (opcional para listagem)
HEADERS = multi_partition.h 

# Arquivo objeto gerado a partir dos arquivos fonte
OBJ = $(SRC:.c=.o)

# Regra padrão para compilar o projeto
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar os arquivos objeto
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(OBJ) $(EXEC)

# Regra para rodar o programa com exemplo
run: $(EXEC)
	./$(EXEC) 16000000 4

# Regra para verificar memória com Valgrind
valgrind: $(EXEC)
	valgrind --leak-check=full --track-origins=yes ./$(EXEC) 16000000 4
