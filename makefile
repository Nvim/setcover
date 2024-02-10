# $@ = nom cible
# $< = nom premiere dépendance
# $^ = liste des dépendances
# $? = liste des dépendances plus récentes que la cible (? pas capté)
# $* = nom du fichier dans extension

# --------------------------------------------- VARIABLES -------------------------------------------

CC=gcc
FLAGS=
LINK_FLAGS=
SRC=src
OBJ=obj
BIN=bin/main

#tous les fichiers .c dans le dossier src:
SRCS=$(wildcard $(SRC)/*.c)

#remplace les occurences d'un src/fichier.c par un obj/fichier.o, pour tous les fichiers du dossier src
# => Dans le dossier obj, fichiers ayant le même nom que les fichiers du dossier src, mais avec l'extension .o
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS)) 


all: $(BIN) #on veut créer l'éxécutable ./bin/main
objs: $(OBJS)

# --------------------------------------------- COMMANDS -------------------------------------------

#	~3e COMMANDE: 
#Création de l'éxécutable BIN à partir de OBJS: fichiers.o de ./obj, ayant les mêmes noms que les fichiers.c de ./src (linking)

#gcc obj/*.o -o bin/main -(link flags)
$(BIN): $(OBJS)
	$(CC) $(FLAGS) $(OBJ)/*.o -o $@ $(LINK_FLAGS)

#	~1e COMMANDE: 
#Création des fichiers objets à partir des fichiers.c du dossier ./src (compiling)
#gcc -c src/%.c -O3 -o obj/%.o
$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p bin obj
	$(CC) -c $< $(FLAGS) -o $@	

#	~Nettoyage:
clean:
	rm $(OBJ)/*.o
	rm bin/*
