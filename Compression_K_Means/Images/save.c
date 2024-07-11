/* Il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void vert(Image *im){
  GLubyte *crt,val1,val2;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  val1=0;
  val2=255;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=val1;
    *crt++=val2;
    *crt++=val1;
  }
}


void vert_moy(Image *im){
  GLubyte *crt,val1;
  int i, nb_pixels, moyenne;
  nb_pixels = im->sizeX * im->sizeY;
  val1=0;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    moyenne = (*crt + *(crt+1) + *(crt+2)) / 3; // Calcul de la moyenne.
    *crt++=val1; // Rouge
    *crt++=moyenne; // Vert
    *crt++=val1; // Bleu
  }
}


void vert_uniforme(Image *im){
  GLubyte *crt,val1;
  int i, nb_pixels, moyenne;
  nb_pixels = im->sizeX * im->sizeY;
  val1=0;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    moyenne = ((*crt*2) + (*(crt+1)*5) + *(crt+2)) / 8; // Calcul de la moyenne.
    *crt++=val1; // Rouge
    *crt++=moyenne; // Vert
    *crt++=val1; // Bleu
  }
}

void rouge_origin(Image *im){
  GLubyte *crt,val1;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  val1=0;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt+=1; // Rouge
    *crt++=val1; // Vert
    *crt++=val1; // Bleu
  }
}

void gris_uniform(Image *im){
  GLubyte *crt;
  int i, nb_pixels, moyenne;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    moyenne = ((*crt*2) + (*(crt+1)*5) + *(crt+2)) / 8; // Calcul de la moyenne.
    *crt++=moyenne; // Rouge
    *crt++=moyenne; // Vert
    *crt++=moyenne; // Bleu
  }
}

/*5. swap_green_red, qui échange dans chaque pixel les teintes de rouge et de vert, et
qui met toutes les teintes de bleu à 0.*/
void swap_green_red(Image *im){
  GLubyte *crt,val1, temp_red, temp_green;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  val1=0;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    temp_red = *crt;
    temp_green = *(crt+1);
    *crt++=temp_green; // Rouge
    *crt++=temp_red; // Vert
    *crt++=val1; // Bleu
  }
}

/*1. Écrire une fonction qui permet de trier les 3 couleurs d'une image : on stockera
tous les GLubyte de rouge, de vert et de bleu dans un grand tableau.*/
GLubyte * create_tab(Image *im){
  GLubyte * crt, *result;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  result = malloc((nb_pixels*3)*(sizeof(GLubyte)));
  for (crt=im->data,i=0;i<nb_pixels;i++){
      result[i] = *crt;
      crt++;
      result[i+nb_pixels] = *crt;
      crt++;
      result[i+(nb_pixels*2)] = *crt;
      crt++;
  }
  return result;
}


/*2. Écrire une fonction qui permet de compresser un tableau de GLubyte selon le
principe de l'algorithme RLE naïf.*/
GLubyte * RLE_compress(GLubyte * tab, int taille) {
  GLubyte *result = malloc(taille * sizeof(GLubyte)); // Allocation initiale
  int i, taille_result = 0;
  int counter = 1;
  for (i = 1; i < taille; i++) {
    if (tab[i] == tab[i - 1] && counter < UCHAR_MAX && counter < taille/3) {
      counter++;
    } else {
      result[taille_result] = counter;  // Stocke le nombre de répétitions
      result[taille_result + 1] = tab[i - 1];  // Stocke la valeur
      taille_result += 2;
      counter = 1;
    }
  }

  result[taille_result] = counter;  // Stocke le nombre de répétitions de la dernière valeur
  result[taille_result + 1] = tab[taille - 1];  // Stocke la dernière valeur
  taille_result += 2;

  result = realloc(result, taille_result * sizeof(GLubyte));

  printf("Taille result = %d\n", taille_result);
  write_tab_in_file(result, taille_result);
  return result;
}

void write_tab_in_file(GLubyte * tab, int size_of_tab){
    int i;
    int * tableau_int = malloc(size_of_tab*sizeof(int));

    //Copie du tableau GLubyte vers tableau de int
    for(i=0; i<size_of_tab; i++){
      tableau_int[i] = tab[i];
      //printf("INT = %d\n, GLUBYTE = %u\n\n", tableau_int[i], tab[i]);
    }

    FILE *fichier = fopen("test.txt", "w");

    for(i=0; i<size_of_tab; i+=2){
      fprintf(fichier, "%d,%d/", tableau_int[i], tableau_int[i+1]);
    }
    fclose (fichier);
}

Color *build_CLUT(Image *im) {
    int nb_pixels = im->sizeX * im->sizeY;
    GLubyte *crt;
    Color *CLUT = malloc(nb_pixels * sizeof(Color));

    for (int i = 0; i < nb_pixels; i++) {
        CLUT[i].frequence = 0;
        CLUT[i].r = 0;
        CLUT[i].g = 0;
        CLUT[i].b = 0;
        CLUT[i].is_last = -1;
    }

    int size_of_CLUT = 0;

    for (crt = im->data; crt < im->data + nb_pixels * 3; crt += 3) {
        GLubyte r = *crt;
        GLubyte g = *(crt + 1);
        GLubyte b = *(crt + 2);
        
        int index = is_in_CLUT(CLUT, r, g, b, size_of_CLUT);
        
        if (index == -1) {
            CLUT[size_of_CLUT].r = r;
            CLUT[size_of_CLUT].g = g;
            CLUT[size_of_CLUT].b = b;
            CLUT[size_of_CLUT].frequence = 1;
            size_of_CLUT++;
        } else {
            CLUT[index].frequence++;
        }
    }

    printf("CLUT créée, sa taille est %d éléments.\n", size_of_CLUT);

    // Réduire la taille de la CLUT pour correspondre exactement au nombre de couleurs uniques trouvées
    CLUT = realloc(CLUT, size_of_CLUT * sizeof(Color));
    CLUT[size_of_CLUT - 1].is_last = 1;

    return CLUT;
}


void sort_CLUT(Color *CLUT) {
    int i, j;
    Color temp;
    int CLUT_sorted=0;
    int size_of_CLUT = get_size_of_CLUT(CLUT);
    for(i=size_of_CLUT-1; i>1; i--) {
      CLUT_sorted=1;
      for(j=0; j<i; j++) {
        if(CLUT[j+1].frequence < CLUT[j].frequence){
              // SWAP LES COULEURS
              temp = CLUT[j+1];
              CLUT[j+1] = CLUT[j];
              CLUT[j] = temp;
              CLUT_sorted = 0;
          }
      }
      if(CLUT_sorted == 1) break;
    }
    // Assigner les indicateurs is_last
    for(j=0; j<size_of_CLUT; j++) {
        if(j==size_of_CLUT-1) {
            CLUT[j].is_last = 1;
        } else {
            CLUT[j].is_last = -1;
        }
    }
    //print_CLUT(CLUT);
}


void print_CLUT(Color * CLUT){
  int i;
  printf("\n-------------------");
  printf("\nAffichage de la CLUT:\n");
  for(i=0; i<get_size_of_CLUT(CLUT); i++){
    printf("Couleur: %u, R=%u, G=%u, B=%d, FREQUENCE: %d, ISLAST = %d CENTROIDE:%d,%d,%d\n", i, CLUT[i].r, CLUT[i].g, CLUT[i].b, CLUT[i].frequence, CLUT[i].is_last, CLUT[i].r_centroid, CLUT[i].g_centroid, CLUT[i].b_centroid);
  }
  printf("-------------------");
}

int is_in_CLUT(Color * CLUT, GLubyte r, GLubyte g, GLubyte b, int size_of_tab){
  int i;
  for(i=0; i<size_of_tab; i++){
    if (CLUT[i].r == r && CLUT[i].g == g && CLUT[i].b == b)
      return i;
  } return -1;
}


/*float distance_RGB(int upper, int lower) {
    return abs(upper-lower);
}*/

float distance_RGB(Color upper, Color lower) {
    return sqrt(((upper.r-lower.r)*(upper.r-lower.r))+((upper.g-lower.g)*(upper.g-lower.g))+((upper.b-lower.b)*(upper.b-lower.b)));
}

void RLE_transform(Color *CLUT, int k, Image *im) {
    GLubyte *crt;
    int nb_pixels = im->sizeX * im->sizeY;
    int i;
    int size_of_CLUT = 0;
    for(i=0; i<nb_pixels ;i++){
        size_of_CLUT++;
        if(CLUT[i].is_last == 1){
          break;
        } 
      }
    for (i = 0, crt = im->data; i < nb_pixels; i++) {
        Color current_color;
        current_color.r = *(crt);
        current_color.g = *(crt + 1);
        current_color.b = *(crt + 2);



        double min_distance = 10000000000;
        int closest_index = 0;

        // Parcours des k couleurs pour trouver la plus proche
        for (int j = 0; j < k; j++) {
            double dist = distance_RGB((current_color), CLUT[size_of_CLUT-j]);
            if (dist < min_distance) {
                min_distance = dist;
                closest_index = size_of_CLUT-j;
                //printf("min distance %f\n", min_distance);
            }
        }

        // Mettre à jour le pixel avec la couleur la plus proche des k couleurs
        *crt++ = CLUT[closest_index].r;
        *crt++ = CLUT[closest_index].g;
        *crt++ = CLUT[closest_index].b;
    }

    printf("\nCOMPRESSION ET TRAITEMENT TERMINÉ");
}

// ------------------ K-MEANS ----------------

int get_size_of_CLUT(Color * CLUT){
  int i;
  for(i=0; CLUT[i].is_last != 1; i++);
  i++;
  return i;
}


// Fonction qui prends les K couleurs les plus fréquentes dans une image avec une distance minimale entre elles.
//OK
Color * get_K_colors_from_CLUT(int k, Color * CLUT, int min_distance){
  int i, j;
  int counter = 0;
  Color * result = malloc(get_size_of_CLUT(CLUT)*sizeof(Color));
  for(i=get_size_of_CLUT(CLUT)-1; counter<k; i--){
    if(distance_RGB(CLUT[i-1], CLUT[i]) > min_distance){
      if(CLUT[i].r != 0 && CLUT[i].g != 0 && CLUT[i].b != 0 && CLUT[i].frequence != 0){
        result[counter] = CLUT[i];
        counter++;
      }
    }
  }
  // On set tous les is_last du nouveau tableau.
  for(j=0; j<counter; j++){
    if(j==counter-1){
      result[j].is_last = 1;
    } else {
      result[j].is_last = -1;
    }
  }
  return result;
}

// OK
Color get_nearest_centroid(Color * centroids, int nb_centroids, Color color){
  int i;
  int min_dist = distance_RGB(centroids[0], color);
  Color result;
  for(i=1; i<nb_centroids; i++){
    if(distance_RGB(centroids[i], color) < min_dist){
      min_dist = distance_RGB(centroids[i], color);
    }
  }
  for(i=0; i<nb_centroids; i++){
    if(distance_RGB(centroids[i], color) == min_dist){
      result = centroids[i];
    }
  }
  return result;
}

void set_centroids_for_image_colours(Color * centroids, Color * image_representation, int nb_centroids, Image * im){
  int i;
  int nb_pixels = im->sizeX * im->sizeY;
  Color nearest_centroid;
  Color temp;
  GLubyte *crt;
  Color * result = malloc(nb_pixels*sizeof(Color));
  for(crt=im->data,i=0;i<nb_pixels;i++){
    temp.r = *crt;
    temp.g = *(crt+1);
    temp.b = *(crt+2);
    nearest_centroid = get_nearest_centroid(centroids, nb_centroids, temp);
    printf("Centroide le plus proche trouvé: R:%d, G:%d, B:%d N°%d\n", nearest_centroid.r, nearest_centroid.g, nearest_centroid.b, i);
    // Mettre à jour la plus petite distance.
    image_representation[i].r_centroid = nearest_centroid.r;
    image_representation[i].g_centroid = nearest_centroid.g;
    image_representation[i].b_centroid = nearest_centroid.b;
    image_representation[i].is_last = -1;
    crt+=3;
  }
  image_representation[i].is_last = 1;
} 

Color mean(Color color1, Color color2) {
    Color mean_result;
    mean_result.r = (color1.r + color2.r) / 2;
    mean_result.g = (color1.g + color2.g) / 2;
    mean_result.b = (color1.b + color2.b) / 2;
    return mean_result;
}

// Fonction qui update la couleur d'un centroide en f(x) des couleurs les plus proches de celui-ci.
void update_centroid_from_colours(Color *centroide, Color * image_colour_tab, Image * im){
  printf("Avant R:%d, G:%d, B:%d\n", centroide->r, centroide->g, centroide->b);
  int nb_pixels = im->sizeX * im->sizeY;
  int i;
  Color * temp = malloc(nb_pixels*sizeof(Color));
  int indice_temp = 0;
  int accR = 0;
  int accG = 0;
  int accB = 0;
  // On remplis un tableau avec les couleurs qui ont pour centroid celui qu'on a selectionné.
  for(i=0; i<nb_pixels; i++){
    if(image_colour_tab[i].r_centroid == centroide->r && image_colour_tab[i].g_centroid == centroide->g && image_colour_tab[i].b_centroid == centroide->b){
      temp[indice_temp] = image_colour_tab[i];
      indice_temp++;
    }
  }
  // On fait la somme de toutes les couleurs.
  for(i=0; i<indice_temp; i++){
    accR += temp[i].r;
    accG += temp[i].g;
    accB += temp[i].b;
  }
  // Update de la couleur du centroide avec la moyenne calculée de toutes les couleurs les plus proches de celui-ci.
  printf("Après R:%d, G:%d, B:%d N°%d\n", accR, accG, accB);

  centroide->r = accR / indice_temp;
  centroide->g = accG / indice_temp;
  centroide->b = accB / indice_temp;

}

// Fonction qui met à jour toutes les couleurs de l'image en fonction du centroide qui lui est attribué.
void update_colours_from_centroid(Color *centroide,Color * image_colour_tab, Image * im){
  int i;
  int nb_pixels = im->sizeX * im->sizeY;  
  Color temp_mean;
  for(i=0;i<nb_pixels;i++){
    if(image_colour_tab[i].r_centroid == centroide->r && image_colour_tab[i].g_centroid == centroide->g && image_colour_tab[i].b_centroid == centroide->b){
      //temp_mean = mean(centroide, image_colour_tab[i]);
      image_colour_tab[i].r = centroide->r;
      image_colour_tab[i].g = centroide->g;
      image_colour_tab[i].b = centroide->b;
    }
  }
}

// Fonction qui créer un tableau de couleur pour une image donnée.
Color * build_color_table_from_image(Image *im){
  int i;
  GLubyte *crt;
  int nb_pixels = im->sizeX * im->sizeY;
  Color * result = malloc(nb_pixels*sizeof(Color));
  for(crt=im->data,i=0;i<nb_pixels;i++){
    result[i].r = *crt;
    result[i].g = *crt+1;
    result[i].b = *crt+2;
    crt+=3;
    result[i].is_last = -1;
  }
  result[i].is_last = 1;
  return result;
}

// Fonction qui update l'image en fonction de son tableau de couleurs.
void update_image_with_color_vector(Image *im, Color * vector){
  int i;
  int nb_pixels = im->sizeX * im->sizeY;
  GLubyte *crt;
  for(crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=vector[i].r;
    *crt++=vector[i].g;
    *crt++=vector[i].b;
  }
}

int RGBToInt(GLubyte r, GLubyte g, GLubyte b){ // convertie un RGB en un int 
  int bits = 8;
  int rgb = 0;
 
  int nb = 0;
  for(int i = bits ; i > 0; i --){
    rgb = rgb  | ((1&r)<<nb);
    r = r >> 1;
    nb++;
    
    rgb = rgb | ((1&g)<<nb) ;
    g = g >> 1;
    nb++;
    
    rgb = rgb | ((1&b)<<nb) ;
    b = b >> 1;
    nb++;
  }
  return rgb;
}

int ColorToInt(Color couleur){ // convertie un RGB en un int 
  GLubyte r = couleur.r;
  GLubyte g = couleur.g;
  GLubyte b = couleur.b;

  int bits = 8;
  int rgb = 0;
 
  int nb = 0;
  for(int i = bits ; i > 0; i --){
    rgb = rgb  | ((1&r)<<nb);
    r = r >> 1;
    nb++;
    
    rgb = rgb | ((1&g)<<nb) ;
    g = g >> 1;
    nb++;
    
    rgb = rgb | ((1&b)<<nb) ;
    b = b >> 1;
    nb++;
  }
  return rgb;
}

void k_means(Image * image, int nb_centroids, int min_distance_centroid, int nb_iteration){
  int i, k;
  printf("COMPRESSION K_MEANS\n\n");
  Color * CLUT_k_means = build_CLUT(image);
  sort_CLUT(CLUT_k_means);
  Color * list_of_centroids = get_K_colors_from_CLUT(nb_centroids, CLUT_k_means, min_distance_centroid);
  //printf("\nPrint de la liste des centroides: \n");
  //print_CLUT(list_of_centroids);
  Color * color_table = build_color_table_from_image(image);
  set_centroids_for_image_colours(list_of_centroids, color_table, nb_centroids, image);
  print_CLUT(list_of_centroids);
  //print_CLUT(color_table);
  for(i=0; i<nb_iteration; i++){
    for(k=0; k<nb_centroids; k++){
      update_colours_from_centroid(&list_of_centroids[k], color_table, image);
      update_centroid_from_colours(&list_of_centroids[k], color_table, image);
    }
    update_image_with_color_vector(image, color_table);
    sleep(2);
    print_CLUT(list_of_centroids);
    // Display de l'image
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    glFlush();
    // ------------------

    printf("\nIteration effectuee n°%d\n", i);
    //sleep(0.5); // On attends 2 secondes entre chaque iteration
    
  }
  //print_CLUT(build_CLUT(image));
  printf("Voici le nombre de couleurs dans l'image après compression:%d", get_size_of_CLUT(build_CLUT(image)));
  
}