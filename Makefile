# Utilisation
# 'make' ou 'make all' -> création de l'exécutable
# 'make clean' -> suppression des fichiers temporaires
# 'make doc' -> création de la doc

# Définition des variables
CC = g++
CFLAGS = -Wall -std=c++11 -Wl,-rpath,.

.PHONY: doc

all: 
	$(CC) $(CFLAGS) src/*.cpp src/*.h src/*.so -o bin/glacier 		
	
clean:
	rm -rf src/*.o src/*~ tex/*{~,.aux,.blg,.pdf,.bbl,.log,.gz,.toc,.out} tex/contents/*{~,.aux,.blg,.pdf,.bbl,.log,.gz,.toc,.out}

tex:
	cd tex && pdflatex report.tex && bibtex report.aux && pdflatex report.tex && pdflatex report.tex
