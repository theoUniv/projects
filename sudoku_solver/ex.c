#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Définition des prototypes de fonctions ainsi que des variables globales.
int write_in_txt(const char * restrict text);
void tab_free(int ** grille);
int is_empty_case(int row, int column, int ** grille);
void print_grid(int ** grille);
int** generer_grille_x9(int difficulty);
void acc_row_column(int * row , int * column, int ** grille);
int is_grid_complete(int ** grille);
int test_value_in_case(int row, int column, int value, int ** grille);
void find_correct_value_in_case(int ** grille, int row, int column);
int resolve_sudoku(int ** grille);
// ------------------------------------------------------------------------

// Fonction qui retourne 1 si une case est vide. 0 sinon.
int is_empty_case(int row, int column, int ** grille){
  if(grille[row][column] == 0) return 1;
  return 0;
}
// ------------------------------------------------------

// Fonction qui affiche la grille.
void print_grid(int ** grille){
  printf("\n|~~~~~|~~~~~|~~~~~|\n");
  for (int x = 0; x < 9; ++x) {
    for (int y = 0; y < 9; ++y) printf("|%d", grille[x][y]);
    printf("|\n");
    if (x % 3 == 2) printf("|~~~~~|~~~~~|~~~~~|\n");
  }
}
// -------------------------------

// Fonction qui génère une grille valide.
int** generer_grille_x9(int difficulty){
    time_t t;
    srand((unsigned) time(&t));
    int i, j ,number, placable, alea;
    // Partie consacrée à l'allocation de la mémoire pour notre grille de sudoku.
    int **new_grille = NULL; 
    new_grille = (int**)calloc(9 , sizeof(new_grille)); // Calloc pour set directement à 0 les valeurs de celle-ci.
    for(i=0 ; i<9 ; i++) new_grille[i] = (int*)calloc(9, sizeof(*new_grille));
    // ------------------------------
    number = rand()%(9 - 1) + 1;
    new_grille[8][8] = number;
    while(!resolve_sudoku(new_grille)){
    placable = 5;
    for(i=0 ; i<placable ; i++){
      for(i=0 ; i<9 ; i++){
        for(j=0 ; j<9 ; j++){
          placable = rand()%(10 - 0) + 0;
          if(placable <= 5){
            number = rand()%(10 - 1) + 1;
            if(test_value_in_case(i, j, number, new_grille))
              new_grille[i][j] = number;
          }
        }
      }
    }
    // Partie consacrée à la correction d'éventuelle erreurs en re-résolvant la grille après le premier passage.
    for(i=0 ; i<9 ; i++){
        for(j=0 ; j<9 ; j++){
          if(new_grille[i][j] != 0)
            if(test_value_in_case(i, j, new_grille[i][j], new_grille))
              find_correct_value_in_case(new_grille, i, j);
        }
    }
    // --------------------------------
  }
  // Partie consacrée à au positionnement des 'trous' dans la grille.
  for(i=0 ; i<=difficulty ; i++){
    alea = rand()%(80 - 0) + 0;
    new_grille[alea / 9][alea % 9] = 0;
  }
  // -----------------------------------
  return new_grille;
}
// ---------------------------------------

// Fonction qui trouve une valeur valide pour une case donnée.
void find_correct_value_in_case(int ** grille, int row, int column){
  int i;
  int alea = rand()%(9 - 1) - 0;
  if(test_value_in_case(row, column, i, grille)){
    grille[row][column] = i;
    //printf("row = %d,column = %d, valeur = %d\n", row, column, grille[row][column]);
  }
}
// ----------------------------------------------------------

// Fonction qui free une grille de sudoku.
void tab_free(int ** grille){
    int i; 
    for(i=0 ; i<9 ; i++) free(grille[i]);
    free(grille); 
}
// ---------------------------------------

// Fonction qui actualise 'row' et 'column' lorsqu'elle est appelée.
void acc_row_column(int * row , int * column, int ** grille){
  int i, j;
  for(i=0 ; i<9 ; i++)
    for(j=0 ; j<9 ; j++){
      if(is_empty_case(i, j, grille)){
        *row = i;
        *column = j;
        //write_in_txt("Pointeurs row et column actualisée\n");
      }
    }
}
// ----------------------------------------------------------------

// Fonction qui retourne 1 si le sudoku est terminé, 0 sinon.
int is_grid_complete(int ** grille){ // ERROR: must have bounds for all dimensions except the first.
  int i, j;
  for(i=0 ;  i<9; i++){
    for(j=0 ; j<9 ; j++){
      if(grille[i][j] == 0) return 0;
    }
  } return 1;
}
// ----------------------------------------------------------

// Fonction qui teste si une valeur dans une case données est valide ou non.
int test_value_in_case(int row, int column, int value, int ** grille){
  int i, j;
  int corner_up_left_x = row / 3 * 3;
  int corner_up_left_y = column / 3 * 3;
  for(i=0 ; i<9 ; i++){
    // Premier cas ou l'un resolve à la fois les colonnes et les grilles.
    if(grille[i][column] == value || grille[row][i] == value) return 0;
    // Second cas ou l'on resolve la grille environnante (3x3 pour un sudoku 9x9).
    // On vient définir les coordonnées des angles haut-gauche des sous-grilles (3x3).
    if (grille[corner_up_left_x + (i % 3)][corner_up_left_y + (i / 3)] == value) return 0;
  } return 1;
}
// ---------------------------------------------------------------------------

// Fonction qui effectue une première vérification du sudoku
int resolve_sudoku(int ** grille){
  int i, row, column;
  if(is_grid_complete(grille)) return 1; // OK
  acc_row_column(&row, &column, grille); // OK
  for(i=0 ; i<10 ; i++){
    if(test_value_in_case(row, column, i, grille)){
      grille[row][column] = i;
      if(resolve_sudoku(grille)) return 1;
      grille[row][column] = 0;
    }
  } return 0;
}
// ------------------------------------------------------

// main ----------------------------------------------------------------------
int main(void){
  // On représente notre grille via une matrice à deux dimensions.
  // On suppose ici que les 0 représentent des cases vides, donc à compléter.
  // Ici la déclaration de notre matrice. On la fait pointer vers null avant.
  int **new_tab = NULL;
  new_tab = generer_grille_x9(50); // On appelle la méthode 'generer_grille_x9'.

  printf("Voici la grille generée aléatoirement -->\n");
  print_grid(new_tab); // On affiche notre grille générée.

  resolve_sudoku(new_tab); // On résout notre grille.

  printf("\nVoici la grille une fois corrigée -->\n");
  print_grid(new_tab); // On affiche notre grille une fois résolue.

  tab_free(new_tab); // On oublie pas de libérer la mémoire alloué pour notre grille de sudoku.
  return 0;
}
// ------------------------------------------------------------------------------

//Fonction qui imprime les logs du jeu dans un fichier dédié -> 'logs.txt'.
/*
int write_in_txt(const char * restrict text){
  FILE * f;
  f = fopen("logs.txt", "w");
  if(f){
    fprintf(f, "%s \r\n", text);
    fclose(f);
    return 1;
  } 
  else {
    perror("logs.txt");
    return 0;
  }
}
*/
// -------------------------------------------------------------------------------