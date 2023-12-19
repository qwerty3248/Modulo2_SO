HOME		= .
C_FILES 	= $(wildcard $(HOME)/*.c)
BIN_FILES 	= $(patsubst $(HOME)/%.c, $(HOME)/%, $(C_FILES))

%: %.c
	@echo "compilando $@..."
	@gcc -o $@ $^

all: $(BIN_FILES)
	@echo "[+] todo compilado!"

clean:
	@echo "[-] limpiando..."
	@rm -rf $(BIN_FILES)
