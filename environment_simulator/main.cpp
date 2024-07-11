

// ---------------------------------------------------------
// Mai 2023
//
// Théo Lemaire & Aymane Ichou L2-Y N°Etudiant 20016837, 20017668
//
// Rendu Projet Final.
// Partie .cpp
// ---------------------------------------------------------
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <chrono>
#include <thread>
#include <array>

class Loup;
class Mouton;
class Case;
class Plateau;

using namespace std;

class Loup{
private:
public:
	int age; 
	int temps_jeune;
	int sexe; //Comme sur les cartes vitales (1 = homme, 2 = femme).
	bool est_immobile;
	int x;
	int y;

	Loup(void){
		this->est_immobile = false;
		this->x = 0;
		this->y = 0;
		this->age = 0;
		this->temps_jeune = 0;
		std::srand(std::time(nullptr));
		int aleatoire = std::rand() % 10;
    	if(aleatoire<5){
    		this->sexe = 1;
    	} else{
    		this->sexe = 2;
    	}
	}

	Loup(int x, int y){
		this->est_immobile = false;
		this->x = x;
		this->y = y;
				std::srand(std::time(nullptr));

		this->age = 0 ; 
		this->temps_jeune= 0 ;  
    	int aleatoire = rand() % 10;
    	if(aleatoire<5){
    		this->sexe = 1;
    	} else{
    		this->sexe = 2;
    	}
	}

	void set_coords(int x, int y){
		this->x = x;
		this->y = y;
	}

	pair<int, int> get_coords(void){
		return{this->x, this->y};
	}

	void augm_faim(void){
		this->temps_jeune +=1 ;
	}

	void augm_vie(void){
		this->age +=1 ; 
	}

	void manger(void){
		this->temps_jeune = 0;
	}

	Loup& operator=(const Loup& l){
        if (this != &l) {
            sexe = l.sexe;
            age = l.age;
            temps_jeune = l.temps_jeune;
            x = l.x;
            y = l.y;
        }
        return *this;
    }

    ~Loup(void){
    	//cout << "Loup est mort par DESTRUCTEUR" << endl;
	}

};

class Mouton {
private:
public:
	int age; 
	int temps_jeune;
	int sexe; //Comme sur les cartes vitales (1 = homme, 2 = femme).
	bool est_immobile;
	int x;
	int y;

	Mouton(void){
		this->est_immobile = false;
		this->x = 0;
		this->y = 0;
		this->age = 0 ; 
		this->temps_jeune= 0 ;
    	int aleatoire = rand() % 10;
    	if(aleatoire<5){
    		this->sexe = 1;
    	} else{
    		this->sexe = 2;
    	}
    	//cout << "Je suis le mouton à l'adresse = " << this << endl;
	}

	Mouton(int x, int y){
		this->est_immobile = false;
		srand(time(0));
		this->x = x;
		this->y = y;
		this->age = 0 ; 
		this->temps_jeune= 0 ;  
    	int aleatoire = rand() % 10;
    	if(aleatoire<5){
    		this->sexe = 1;
    	} else{
    		this->sexe = 2;
    	}
	}

	void set_coords(int x, int y){
		this->x = x;
		this->y = y;
	}

	pair<int, int> get_coords(void){
		return{this->x, this->y};
	}

	void augm_faim(void){
		this->temps_jeune +=1 ;
	}

	void augm_vie(void){
		this->age +=1 ; 
	}

	void manger(void){
		this->temps_jeune = 0;
	}

	Mouton& operator=(const Mouton& m){
        if (this != &m) {
            sexe = m.sexe;
            age = m.age;
            temps_jeune = m.temps_jeune;
            x = m.x;
            y = m.y;
        }
        return *this;
    }

	~Mouton(void){
    	//cout << "Mouton mort = " << this << endl;
	}

};

class Case{
private:
	bool ressource;
	bool herbe;
	char entite;
	//int x, y;
public:
	Case(void){
		this->herbe = true;
		entite = 'V';
		this->ressource = false;
	}

	Case(char entite){
		//VIDE , MOUTON , LOUP , SEL 
		this->entite = entite;
		this->herbe = true;
	}

	void set_entite(char entite){
		this->entite = entite;
	}

	char get_entite(void){
		return this->entite;
	}

	void set_ressource(bool choix, int x, int y){
		this->ressource = choix;
		if(choix == true) 
			cout << "Herbe a poussé en: [" << x << "," << y << "]" << endl;
	}

	bool get_ressource(void){
		return this->ressource;
	} 

	void set_herbe(bool choix){
		this->herbe = choix;
	}

	bool get_herbe(void){
		return this->herbe;
	}

	~Case(void){
		this->herbe = false;
		this->entite = 'V';
		//cout << "CASE DETRUITE !" << endl;
	}

};

class Plateau{
private:
	int w; 
	int h;
	Case grille[7][7];
	//string actions[50];
public:
	static int nb_tour;
	array<Mouton,49> liste_moutons;
	int nb_moutons;
	int nb_loups;
	array<Loup,49> liste_loups;


	Plateau(void){
		//grille = malloc((w*h)*sizeof(Case));
		this->nb_tour=0;
		this->w = 7;
		this->h = 7;
		nb_moutons = 0;
		nb_loups = 0;
		cout << "---------- |Plateau de jeu créé.| ----------" << endl;
	}

	template <typename entite>
	int pos_entite_liste(array<entite,49> &liste, int taille, int x, int y){
		int i;
		for(i=0; i<taille; i++){
			if(x == liste[i].x && y == liste[i].y){
				return i;
			}
		} return -1;
	}

	int get_tour(void){
		return this->nb_tour;
	}

	void init_plateau(void){
		std::srand(std::time(nullptr));
		int x = rand() % w;
		int y = rand() % h;
		cout << "Entrez le nombre de loup, puis le nombre de moutons souhaités:" << endl;
		cin >> this->nb_loups;
		cin >> this->nb_moutons;
		while(true){
			if((this->nb_moutons + this->nb_loups) < 49)
				break;
			cout << "Trop d'entités entrées , Loup + Mouton doivent être inférieur à 49" << endl; 
			cin >> this->nb_loups;
			cin >> this->nb_moutons;
		}
		int temp = 0;
		// On place de façon aléatoire les loups et les moutons.
		while(temp!=nb_moutons){
			if(this->grille[x][y].get_entite() == 'V' && this->grille[x][y].get_entite() != 'L'){
				if(pos_entite_liste(liste_moutons, temp, x, y) == -1){
			        this->liste_moutons[temp].set_coords(x, y);
			        this->grille[x][y].set_entite('M'); 
			       	temp++;
			    }
		    }
		    x = rand() % w;
		    y = rand() % h;
	    }
	    //this->afficher_plateau();
	    temp = 0;
	    while(temp!=nb_loups){
	    	if((this->grille[x][y].get_entite() == 'V')){
				x = rand() % w;
		        y = rand() % h;
		        if(this->grille[x][y].get_entite() != 'M'){

		        	this->liste_loups[temp].set_coords(x, y);
		        	this->grille[x][y].set_entite('L'); 
		        	temp++;
		        }
		    }
			x = rand() % w;
		    y = rand() % h;
	    }
	}

void afficher_plateau(void) {
	int temp_mouton = -1;
	int temp_loup = -1;
    cout << "Tour : " << this->get_tour() << "| Loups : " << this->nb_loups << "| Moutons : " << this->nb_moutons << "\n\n" << endl;
    cout << "    0   1   2   3   4   5   6" << endl;
    cout << "  +---+---+---+---+---+---+---+" << endl;
	int nb = 0; // Ajout de 1 pour la sixième ligne
    for (int i = 0; i < h; i++) {
        cout << nb << " |";
        nb++;
        for (int j = 0; j < w; j++) {
           	temp_mouton = pos_entite_liste(this->liste_moutons, this->nb_moutons, i, j);
           	if(temp_mouton == -1){
           		temp_loup = pos_entite_liste(this->liste_loups, this->nb_loups, i, j);
           	}
            if (temp_mouton == -1 && temp_loup ==-1) {
                if(grille[i][j].get_ressource() == true)
                	cout << "\033[1;30m" << " S " << "\033[0m";
                else 
                	cout << "   ";
            } 
            if(temp_mouton != -1) {
            	if(liste_moutons[temp_mouton].sexe == 1)
                	cout << "\033[1;34m" << " M " << "\033[0m";
               	else
               		cout << "\033[1;35m" << " M " << "\033[0m";
            }

            if(temp_loup != -1) {
                if(liste_loups[temp_loup].sexe == 1)
                	cout << "\033[1;34m" << " L " << "\033[0m";
               	else
               		cout << "\033[1;35m" << " L " << "\033[0m";
            }

            cout << "|";
            temp_mouton = -1 ; 
            temp_loup = -1 ; 
        }
        cout << endl;
        cout << "  +---+---+---+---+---+---+---+" << endl;
    }
    cout << endl;  // Ajout d'une ligne vide pour la lisibilité
}

	bool est_occupe(int x, int y){
		if(grille[x][y].get_entite() == 'M' || grille[x][y].get_entite() == 'L')
			return true;
		return false;
	}

	pair<int, int> trouver_case_autour(int x, int y, char entite){
		int i, j;
		// On est dans un coin
		if((x == 6 || x == 0) && (y == 6 || y == 0)){
			if(x==0 && y==0){ // HG
				if(grille[x+1][y].get_entite()==entite)
					return {x+1, y};
				if(grille[x+1][y+1].get_entite()==entite)
					return {x+1, y+1};
				if(grille[x][y+1].get_entite()==entite)
					return {x, y+1};
			}
			if(x==6 && y==0){ //BG
				if(grille[x-1][y].get_entite()==entite)
					return {x-1, y};
				if(grille[x][y+1].get_entite()==entite)
					return {x, y+1};
				if(grille[x-1][y+1].get_entite()==entite)
					return {x-1, y+1};
			}
			if(x==0 && y==6){ //HD
				if(grille[x][y-1].get_entite()==entite)
					return {x, y-1};
				if(grille[x+1][y].get_entite()==entite)
					return {x+1, y};
				if(grille[x+1][y-1].get_entite()==entite)
					return {x+1, y-1};
			}
			if(x==6 && y==6){ //BD
				if(grille[x-1][y].get_entite()==entite)
					return {x-1, y};
				if(grille[x][y-1].get_entite()==entite)
					return {x, y-1};
				if(grille[x-1][y-1].get_entite()==entite)
					return {x-1, y-1};
			}
			return {x, y};
		}
		// On se trouve aux bordures (mur).
		if(x==0 || x == 6 || y== 0 || y ==6){
					if(x==0){
						for(i=-1; i<2; i++){
							if(grille[x][y+i].get_entite()==entite){
								return {x,y+i};
							}

							if(grille[x+1][y+i].get_entite()==entite){
								return {x+1,y+i};
							}
							if(grille[x+1][y+i].get_entite()=='V'){
								return {x+1,y+i};
							}
							if(grille[x][y+i].get_entite()=='V'){
								return {x, y+i};
							}
						}
					}
					if(x==6){
						for(i=-1; i<2; i++){
							if(grille[x][y+i].get_entite()==entite){
								return {x,y+i};
							}
							if(grille[x-1][y+i].get_entite()==entite){
								return {x-1,y+i};
							}
							if(grille[x-1][y+i].get_entite()=='V'){
								return {x-1,y+i};
							}
							if(grille[x][y+i].get_entite()=='V'){
								return {x, y+i};
							}
						}
					}
					if(y==0){
						for(i=-1; i<2; i++){
							if(grille[x+i][y].get_entite()==entite){
								return {x+i,y};
							}
							if(grille[x+i][y+1].get_entite()==entite){
								return {x+i,y+1};
							}
							if(grille[x+i][y].get_entite()=='V'){
								return {x+i,y};
							}
							if(grille[x+i][y+1].get_entite()=='V'){
								return {x+i, y+1};
							}
						}
					}
					if(y==6){
						for(i=-1; i<2; i++){
							if(grille[x+i][y].get_entite()==entite){
								return {x+i,y};
							}
							if(grille[x+i][y-1].get_entite()==entite){
								return {x+i,y-1};
							}
							if(grille[x+i][y-1].get_entite()=='V'){
								return {x+i,y-1};
							}
							if(grille[x+i][y].get_entite()=='V'){
								return {x+i, y};
							}
						}
					}
					return {x, y};
				} 

		for(i=-1; i<2; i++){
			for(j=-1; j<2; j++){
				if((x < 6 || x>0) && (j < 6 || j>0)){
					// On ne se trouve pas a proximité d'un mur/coin.
					if(grille[x+i][y+j].get_entite()==entite && (i!=0 && j!=0)){
						//cout << x+i << ", " << y+j << endl;
						return {x+i, y+j};
					}
				}
			}
		}
		return {x, y};
	}

		template <typename entite>
		pair<int, int> trouver_case_autour_sexe(int x, int y, char entite_nom, int sexe, array<entite,49> &liste_entite, int nb_entite){
		int i, j;
		int indice = 0; 
		// On est dans un coin
		if((x == 6 || x == 0) && (y == 6 || y == 0)){
			if(x==0 && y==0){ // HG
				if(grille[x+1][y].get_entite()==entite_nom)
					indice = pos_entite_liste(liste_entite, nb_entite, x+1, y);
					//cout << indice << "HEHO LINDICE EST ICI HEHO --------------" << endl;
					if(liste_entite[indice].sexe == sexe)
						return {x+1, y};
				if(grille[x+1][y+1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x+1, y+1);
					if(liste_entite[indice].sexe == sexe)
						return {x+1, y+1};
				}
				if(grille[x][y+1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x, y+1);
					if(liste_entite[indice].sexe == sexe)
						return {x, y+1};
				}
			}
			if(x==6 && y==0){ //BG
				if(grille[x-1][y].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x-1, y);
					if(liste_entite[indice].sexe == sexe)
						return {x-1, y};
				}
				if(grille[x][y+1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x, y+1);
					if(liste_entite[indice].sexe == sexe)
						return {x, y+1};
				}
				if(grille[x-1][y+1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x-1, y+1);
					if(liste_entite[indice].sexe == sexe)
						return {x-1, y+1};
				}
			}
			if(x==0 && y==6){ //HD
				if(grille[x][y-1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x, y-1);
					if(liste_entite[indice].sexe == sexe)
						return {x, y-1};
				}
				if(grille[x+1][y].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x+1, y);
					if(liste_entite[indice].sexe == sexe)
						return {x+1, y};
				}
				if(grille[x+1][y-1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x+1, y-1);
					if(liste_entite[indice].sexe == sexe)
						return {x+1, y-1};
				}
			}
			if(x==6 && y==6){ //BD
				if(grille[x-1][y].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x-1, y);
					if(liste_entite[indice].sexe == sexe)
						return {x-1, y};
				}
				if(grille[x][y-1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x, y-1);
					if(liste_entite[indice].sexe == sexe)
						return {x, y-1};
				}
				if(grille[x-1][y-1].get_entite()==entite_nom){
					indice = pos_entite_liste(liste_entite, nb_entite, x-1, y-1);
					if(liste_entite[indice].sexe == sexe)
						return {x-1, y-1};
				}
			}
			return {x, y};
		}
		// On se trouve aux bordures (mur).
		if(x==0 || x == 6 || y== 0 || y ==6){
					if(x==0){
						for(i=-1; i<2; i++){
							if(grille[x][y+i].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x,y+i};
							}

							if(grille[x+1][y+i].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x+1, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x+1,y+i};
							}
							if(grille[x+1][y+i].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x+1, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x+1,y+i};
							}
							if(grille[x][y+i].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x, y+i};
							}
						}
					}
					if(x==6){
						for(i=-1; i<2; i++){
							if(grille[x][y+i].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x,y+i};
							}
							if(grille[x-1][y+i].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x-1, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x-1,y+i};
							}
							if(grille[x-1][y+i].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x-1, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x-1,y+i};
							}
							if(grille[x][y+i].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x, y+i);
								if(liste_entite[indice].sexe == sexe)
									return {x, y+i};
							}
						}
					}
					if(y==0){
						for(i=-1; i<2; i++){
							if(grille[x+i][y].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y);
								if(liste_entite[indice].sexe == sexe)
									return {x+i,y};
							}
							if(grille[x+i][y+1].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y+1);
								if(liste_entite[indice].sexe == sexe)
									return {x+i,y+1};
							}
							if(grille[x+i][y].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y);
								if(liste_entite[indice].sexe == sexe)
									return {x+i,y};
							}
							if(grille[x+i][y+1].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y+1);
								if(liste_entite[indice].sexe == sexe)
									return {x+i, y+1};
							}
						}
					}
					if(y==6){
						for(i=-1; i<2; i++){
							if(grille[x+i][y].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y);
								if(liste_entite[indice].sexe == sexe)
									return {x+i,y};
							}
							if(grille[x+i][y-1].get_entite()==entite_nom){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y-1);
								if(liste_entite[indice].sexe == sexe)
									return {x+i,y-1};
							}
							if(grille[x+i][y-1].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y-1);
								if(liste_entite[indice].sexe == sexe)
									return {x+i,y-1};
							}
							if(grille[x+i][y].get_entite()=='V'){
								indice = pos_entite_liste(liste_entite, nb_entite, x+i, y);
								if(liste_entite[indice].sexe == sexe)
									return {x+i, y};
							}
						}
					}
					return {x, y};
				} 

		for(i=-1; i<2; i++){
			for(j=-1; j<2; j++){
				if((x < 6 || x>0) && (j < 6 || j>0)){
					// On ne se trouve pas a proximité d'un mur/coin.
					if(grille[x+i][y+j].get_entite()==entite_nom && (i!=0 && j!=0)){
						indice = pos_entite_liste(liste_entite, nb_entite, x+i, y+j);
						if(liste_entite[indice].sexe == sexe)
							//cout << x+i << ", " << y+j << endl;
							return {x+i, y+j};
					}
				}
			}
		}
		return {x, y};
	}

	void jouer_tour(void){
		int i, j;
		for(i=0; i<this->nb_moutons; i++){
			action(this->liste_moutons[i]);
			if(this->liste_moutons[i].age <= 50 && this->liste_moutons[i].temps_jeune < 6){
				this->liste_moutons[i].augm_vie();
				this->liste_moutons[i].augm_faim();
			} else {
				// MORT NATURELLE
				int pos_entite = pos_entite_liste(liste_moutons, nb_moutons, this->liste_moutons[i].x, this->liste_moutons[i].y);
				retirer_entite(liste_moutons, this->liste_moutons[i].x, this->liste_moutons[i].y, true);
			}
		}
		for(i=0; i<this->nb_loups; i++){
			action(this->liste_loups[i]);
			if(this->liste_loups[i].age <= 60 && this->liste_loups[i].temps_jeune <= 10){
				this->liste_loups[i].augm_vie();
				this->liste_loups[i].augm_faim();
				//cout << "Il a jeuné depuis = " << this->liste_loups[i].temps_jeune << endl;
			} else {
				//cout << "IL EST SENCE DIE" << endl;
				// MORT NATURELLE
				int pos_entite = pos_entite_liste(liste_loups, nb_loups, this->liste_loups[i].x, this->liste_moutons[i].y);
				retirer_entite(liste_loups, this->liste_loups[i].x, this->liste_loups[i].y, true);
			}
		}
		for(i=0; i<7; i++){
			for(j=0; j<7; j++){
				if(this->grille[i][j].get_ressource() == true){
					this->grille[i][j].set_ressource(false, i, j);
					this->grille[i][j].set_herbe(true);
				}
			}
		}
		cout << "-------------| TOUR N°" << this->nb_tour << "|--------------" << endl;
	}

	template <typename entite>
	pair<int, int> action(entite &animal){
		// MANGER
		if(typeid(animal) == typeid(Loup)){
			if(animal.temps_jeune > 4 && animal.temps_jeune < 10){ //si il a
				int tempo = pos_entite_liste(liste_loups, nb_loups, animal.x, animal.y);
				pair<int, int> new_pos = trouver_case_autour(liste_loups[tempo].x, liste_loups[tempo].y, 'M');
				if((new_pos.first != animal.x) && (new_pos.second != animal.y)){// SI ya a graille
					retirer_entite(liste_moutons, new_pos.first, new_pos.second, false);
					animal.manger();
					cout << "Loup a mangé en " << new_pos.first << ", " << new_pos.second << endl;
					this->grille[new_pos.first][new_pos.second].set_entite('L');
					this->grille[animal.x][animal.y].set_entite('V');
					animal.x = new_pos.first;
					animal.y = new_pos.second;
					return new_pos;
				}
			}
		}
		if(typeid(animal) == typeid(Mouton)){
			if(animal.temps_jeune > 2 && animal.temps_jeune < 6){
				if(grille[animal.x][animal.y].get_herbe() == true){
					animal.manger();
					grille[animal.x][animal.y].set_herbe(false);
					return {animal.x, animal.y};
				}
			}
		}
		if((this->nb_moutons + this->nb_loups) <49){
			// SE REPRODUIRE POSSIBLE ?
			pair<int, int> new_pos_sexe, case_vide;
			int to_find_sexe;
			int indice_second;
			if(animal.sexe == 1)
				to_find_sexe = 2;
			if(animal.sexe == 2)
				to_find_sexe = 1;
			if(typeid(animal) == typeid(Loup)){
				new_pos_sexe = this->trouver_case_autour_sexe(animal.x, animal.y, 'L', to_find_sexe, liste_loups, nb_loups);
				// On trouve un autre loup du sexe opposé
				if((new_pos_sexe.first != animal.x) || (new_pos_sexe.second != animal.y)){
					// On trouve une case libre pour le ptit
					case_vide = this->trouver_case_autour(animal.x, animal.y, 'V');
					if(case_vide.first != animal.x || case_vide.second != animal.y){
						// Case vide && animal du sexe opposé trouvé.
						// Ajout du nouveau né.
						animal.est_immobile = true;
						indice_second = pos_entite_liste(liste_loups, nb_loups, new_pos_sexe.first, new_pos_sexe.second);
						liste_loups[indice_second].est_immobile = true;
					}
				}
			}
			if(typeid(animal) == typeid(Mouton)){
				new_pos_sexe = this->trouver_case_autour_sexe(animal.x, animal.y, 'M', to_find_sexe, liste_moutons, nb_moutons);
				// On trouve un autre loup du sexe opposé
				if((new_pos_sexe.first != animal.x) || (new_pos_sexe.second != animal.y)){
					// On trouve une case libre pour le ptit
					case_vide = this->trouver_case_autour(animal.x, animal.y, 'V');
					if(case_vide.first != animal.x || case_vide.second != animal.y){
						// Case vide && animal du sexe opposé trouvé.
						// Ajout du nouveau né.
						animal.est_immobile = true;
						indice_second = pos_entite_liste(liste_moutons, nb_moutons, new_pos_sexe.first, new_pos_sexe.second);
						liste_moutons[indice_second].est_immobile = true;
						//cout << "MOUTON A KEN OUAIIIS" << endl;
					}
				}
			}
			if(animal.est_immobile == true){
				//cout << "Nouveau née !" << endl;
				if(typeid(animal)==typeid(Mouton)){
					ajouter_entite(liste_moutons, nb_moutons, case_vide.first, case_vide.second);
				}
				if(typeid(animal)==typeid(Loup)){
					ajouter_entite(liste_loups, nb_loups, case_vide.first, case_vide.second);
				}
				animal.est_immobile = false;
			}
		}
		if(animal.est_immobile==false){
			//DEPLACEMENT
			int temp;
			pair<int, int> new_pos = this->trouver_case_autour(animal.x, animal.y, 'V');
			if((new_pos.first != animal.x) || (new_pos.second != animal.y)){
				if(typeid(animal)==typeid(Loup)){
					this->grille[new_pos.first][new_pos.second].set_entite('L');
					//cout << "Loup se déplace"<< endl;
					temp = pos_entite_liste(liste_loups, nb_loups, animal.x, animal.y);
					this->grille[animal.x][animal.y].set_entite('V');
					//cout << "TEMP = " << temp << endl;
					this->liste_loups[temp].x = new_pos.first;
					this->liste_loups[temp].y = new_pos.second;
				}
				if(typeid(animal)==typeid(Mouton)){
					this->grille[new_pos.first][new_pos.second].set_entite('M');
					temp = pos_entite_liste(liste_moutons, nb_moutons, animal.x, animal.y);
					this->grille[animal.x][animal.y].set_entite('V');
					//cout << "TEMP = " << temp << endl;
					this->liste_moutons[temp].x = new_pos.first;
					this->liste_moutons[temp].y = new_pos.second;
				}
			}		
			return new_pos;
		}
		 return {-1, -1};
}

	template <typename entite>
	void afficher_liste_entite(array<entite,49> &liste){
		int i;
		int nb;
		if(typeid(liste[i]) == typeid(Loup)){
			nb = this->nb_loups;
			cout << "liste de loups:" << endl;
		}
		if(typeid(liste[i]) == typeid(Mouton)) {
			nb = this->nb_moutons;
			cout << "liste de moutons: " << endl;
		}
		for(i=0; i<nb; i++){
			cout << "("<< liste[i].x << ", " << liste[i].y << ")" << endl;
		}
	}

	template <typename entite>
	void retirer_entite(array<entite,49> &liste, int x, int y, bool naturelle){
		int nb = 0;
		int i = 0;
		//cout << "Nombres de moutons = " << this->nb_moutons << endl;
		if(typeid(liste[i]) == typeid(Loup)) 
			nb = nb_loups;
		if(typeid(liste[i]) == typeid(Mouton))
			nb = nb_moutons;
		int indice = pos_entite_liste(liste, nb, x, y);
		//cout << "le moin est ici"<< nb  << " nb moutons = "<< this->nb_moutons << endl;
		nb--;
		liste[indice] = liste[nb];
		if(typeid(liste[i]) == typeid(Loup) && this->nb_loups >0) 
			this->nb_loups--;
			if(naturelle == false)
				cout << "Loup est mort en: [" << x << "," << y << "]" << endl;
		if(typeid(liste[i]) == typeid(Mouton) && this->nb_moutons >0)
			this->nb_moutons--;
			if(naturelle == false)
				cout << "Mouton est mort en: [" << x << "," << y << "]" << endl;
			//cout << "Le delete commence ici !"  << endl;
			//cout << "Nombre de moutons actuels = " << this->nb_moutons << endl;
		// SI la mort est naturelle.
		this->grille[x][y].set_entite('V');
		if(naturelle == true){
			if(typeid(liste[0]) == typeid(Mouton)){
				cout << "Mouton est mort naturellement en: [" << x << "," << y << "]" << endl;
			}
			if(typeid(liste[0]) == typeid(Loup)){
				cout << "Loup est mort naturellement en: [" << x << "," << y << "]" << endl;
			}
			this->grille[x][y].set_entite('S');
			this->grille[x][y].set_ressource(true, x, y);
			cout << "Sel minéral dans la case: [" << x << "," << y << "]" << endl;
		}
	}

	template <typename entite>
	void ajouter_entite(array<entite,49> &liste, int nb_entite, int x, int y){
		// On donne en argument une case vide.
		if(typeid(liste[0]) == typeid(Loup)) {
			this->liste_loups[this->nb_loups].set_coords(x, y);
			this->nb_loups++;
			grille[x][y].set_entite('L');
		}
		if(typeid(liste[0]) == typeid(Mouton)) {
			this->liste_moutons[this->nb_moutons].set_coords(x, y);
			this->nb_moutons++;
			grille[x][y].set_entite('M');
			//cout << "======================ici" << endl;
		}
	}

	~Plateau(void){
		cout << "---------- |Plateau de jeu détruit.| ----------" << endl;
	}

};

int Plateau::nb_tour=0;