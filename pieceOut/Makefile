# Paramètres du compilateur et flags.
CPP = g++ --std=c++11 -Wall
INCLUDES = -I/opt/homebrew/include -I Case -I Plateau -I Piece -I Piece/OperateurDeplacement \
           -I Piece/OperateurSymetrie -I Piece/OperateurRotation -I Piece/Piece_abstract \
           -I Piece/PieceConcrete -I Piece/PieceOperateur -I Levels
LIBS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

#Partie build.
pieceOut: menu.o Levels/niveau1.o Levels/niveau2.o Levels/niveau3.o Plateau.o Case.o Piece/OperateurDeplacement.o \
        Piece/OperateurSymetrie.o Piece/OperateurRotation.o \
        Piece/Piece_abstract.o Piece/PieceConcrete.o Piece/PieceOperateur.o
	$(CPP) -o pieceOut menu.o Levels/niveau1.o Levels/niveau2.o Levels/niveau3.o Plateau.o Case.o \
	       Piece/OperateurDeplacement.o Piece/OperateurSymetrie.o \
	       Piece/OperateurRotation.o Piece/Piece_abstract.o \
	       Piece/PieceConcrete.o Piece/PieceOperateur.o $(LIBS)

# Les .o
menu.o: menu.cpp
	$(CPP) $(INCLUDES) -c menu.cpp -o menu.o

Levels/%.o: Levels/%.cpp
	$(CPP) $(INCLUDES) -c $< -o $@

Plateau.o: Plateau/Plateau.cpp
	$(CPP) $(INCLUDES) -c Plateau/Plateau.cpp -o Plateau.o

Case.o: Case/Case.cpp
	$(CPP) $(INCLUDES) -c Case/Case.cpp -o Case.o

Piece/OperateurDeplacement.o: Piece/OperateurDeplacement/OperateurDeplacement.cpp
	$(CPP) $(INCLUDES) -c Piece/OperateurDeplacement/OperateurDeplacement.cpp -o Piece/OperateurDeplacement.o

Piece/OperateurSymetrie.o: Piece/OperateurSymetrie/OperateurSymetrie.cpp
	$(CPP) $(INCLUDES) -c Piece/OperateurSymetrie/OperateurSymetrie.cpp -o Piece/OperateurSymetrie.o

Piece/OperateurRotation.o: Piece/OperateurRotation/OperateurRotation.cpp
	$(CPP) $(INCLUDES) -c Piece/OperateurRotation/OperateurRotation.cpp -o Piece/OperateurRotation.o

Piece/Piece_abstract.o: Piece/Piece_abstract/Piece.cpp
	$(CPP) $(INCLUDES) -c Piece/Piece_abstract/Piece.cpp -o Piece/Piece_abstract.o

Piece/PieceConcrete.o: Piece/PieceConcrete/PieceConcrete.cpp
	$(CPP) $(INCLUDES) -c Piece/PieceConcrete/PieceConcrete.cpp -o Piece/PieceConcrete.o

Piece/PieceOperateur.o: Piece/PieceOperateur/PieceOperateur.cpp
	$(CPP) $(INCLUDES) -c Piece/PieceOperateur/PieceOperateur.cpp -o Piece/PieceOperateur.o

# Partie clean.
clean:
	rm -f *.o pieceOut
	rm -f Levels/*.o
	rm -f Plateau.o Case.o
	rm -f Piece/*.o Piece/OperateurDeplacement/*.o Piece/OperateurSymetrie/*.o \
	      Piece/OperateurRotation/*.o Piece/Piece_abstract/*.o \
	      Piece/PieceConcrete/*.o Piece/PieceOperateur/*.o
