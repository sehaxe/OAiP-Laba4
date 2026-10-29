all: 1/main 2/main 3/main

.PHONY: report schemes screens clean

# Полная сборка: программы -> схемы -> окна терминала -> PDF.
report: 1/main 2/main 3/main schemes screens
	typst compile --root .. report/main.typ

1/main: 1/main.c
	gcc -Wall 1/main.c -o 1/main

2/main: 2/main.c
	gcc -Wall 2/main.c -o 2/main

3/main: 3/main.c
	gcc -Wall 3/main.c -o 3/main

# Блок-схемы — по упрощённым копиям алгоритма в report/schemes/
# (без блоков проверки ввода): полный код в N/main.c. --no-split
# оставляет каждую схему одним листом, масштаб общий на всю пачку.
GOSTPADI ?= $(HOME)/gostpadi/target/release/gostpadi
schemes:
	rm -f report/assets/*.svg
	cd report/schemes && $(GOSTPADI) --labels=ru --no-split \
		1-main.c 2-main.c 3-main.c -o ../assets/ --lw=1.5

# Реальный вывод программ -> report/assets/term.json (без растра).
screens: 1/main 2/main 3/main
	python3 report/make_screens.py

clean:
	rm -f 1/main 2/main 3/main
