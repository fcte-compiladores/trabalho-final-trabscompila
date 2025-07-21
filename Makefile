CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGET=pddl_parser
SOURCE=pddl_parser_novo.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	@echo "=== Testando exemplos ==="
	@for case in "Hello-World" "Sequence" "Logistics" "Factory" "Delivery" "Tracking" "World of Tanks"; do \
		echo "Testando $$case..."; \
		./$(TARGET) "Exemplos/$$case/domain.pddl" "Exemplos/$$case/problem.pddl" || exit 1; \
	done
	@echo "Todos os testes passaram!"

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: clean test install
