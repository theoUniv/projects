/* 
Théo Lemaire L3-B - Cours d'introduction à la sécurité
Implémentation du chiffrement AES en langage C
Type de données utilisées --> Hexadécimal
*/

// --------- Imports néssécaires -----------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "AES.h"
// -----------------------------------------

// Fonction qui trouve dans la sBox la substitution hexadécimale correspondante.
unsigned char find_in_s_box(unsigned char hexa_caracter, data SBox_Data) {
    unsigned char first = (hexa_caracter >> 4) & 0x0F;
    unsigned char second = hexa_caracter & 0x0F;
    if (first<SBox_Data.size && second<SBox_Data.size) {
        unsigned char finded = SBox_Data.matrix[first][second];
        return finded;
    } else {
        return 0;
    }
}
// -----------------------------------------------------------------------------

// Fonction qui créer la data correspondante à la Sbox.
data create_s_box_matrix(const unsigned char *tab_to_convert) {
    int i, j;
    int index = 0;
    data s_box_in_data;
    s_box_in_data.size = 16;  // Taille de la matrice SBox
    s_box_in_data.matrix = malloc(16 * sizeof(unsigned char *)); // Allouer la mémoire pour la matrice
    for (i=0; i<16; i++) {
        s_box_in_data.matrix[i] = malloc(16 * sizeof(unsigned char));
    }
    for (i=0; i<16; i++) {
        for (j=0; j<16; j++) {
            s_box_in_data.matrix[i][j] = tab_to_convert[index];
            index++;
        }
    }
    return s_box_in_data;
}
// -----------------------------------------------------------------------------

// Fonction qui prends un tableau d'unsigned char et qui le convertit en data
data create_matrix(unsigned char * tab_to_convert, int size_tab) {
    int i, j;
    int index = 0;
    data new_matrix;
    new_matrix.size = 4;  // Taille de la matrice SBox
    new_matrix.matrix = malloc(4 * sizeof(unsigned char *)); // Allouer la mémoire pour la matrice
    for (i=0; i<4; i++) {
        new_matrix.matrix[i] = malloc(4 * sizeof(unsigned char));
    }
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
        	if (index < size_tab){
        		new_matrix.matrix[j][i] = tab_to_convert[index];
        	} else {
        		new_matrix.matrix[j][i] = 0;
        	}
        	index++;
        }
    }
    return new_matrix;
}
// -----------------------------------------------------------------------------

// Fonction qui libère la mémoire d'une data.
void free_data(data matrix){
	int i;
    if (matrix.matrix != NULL) {
        for(i=0; i<matrix.size; i++) {
            free(matrix.matrix[i]);
        }
        free(matrix.matrix);
    }
}
// -----------------------------------------------------------------------------

// Fonction qui retourne l'index d'un caractère hexadécimal dans le tableau ascii pour la substitution.
int get_index(unsigned char caracter){
	int i;
	for(i=0; i<16; i++){
		if(s_box_categories[i]==caracter){
			printf("FINDED !");
			return i;
		}
	} 
	return -1;
}
// -----------------------------------------------------------------------------

// Fonction qui affiche la matrice d'une data.
void print_data(data Message){
	int i, j;
	printf("Visualisation de la donnée.\n");
	for(i=0; i<Message.size; i++)
		printf("########");
	printf("\n");
	for(i=0; i<Message.size; i++){
		for(j=0; j<Message.size; j++){
			printf("%2X\t", Message.matrix[i][j]);
		}
		printf("\n");
	}
	for(i=0; i<Message.size; i++)
		printf("########");
	printf("\n");
}
// -----------------------------------------------------------------------------

// Fonction qui prends en paramètre une chaine de caractère et qui la convertit en tableau d'hexadécimal.
unsigned char *convert_char_to_hexa(char *chain, int size) {
	int i;
    unsigned char *key_text = (unsigned char *)malloc(size * sizeof(unsigned char));
    for (i=0; i<size; i++) 
        key_text[i] = (unsigned char)chain[i];
    return key_text;
}
// -----------------------------------------------------------------------------

// Fonction "SubBytes" de AES qui effectue la substitution d'une matrice d'une data donnée dans la SBox.
data sub_bytes(data Message, data SBox_matrix){
	int i, j;
	data Matrix_before_first_step;
	Matrix_before_first_step.size = 4;
	Matrix_before_first_step.matrix = malloc(4 * sizeof(unsigned char *));
    for (i=0; i<4; i++) {
        Matrix_before_first_step.matrix[i] = malloc(4 * sizeof(unsigned char));
    }
    /* On substitue par colonnes les hexadécimaux dans la SBox en fonction
	du premier et du deuxième élément de notre exadécimal.*/
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			Matrix_before_first_step.matrix[j][i] = find_in_s_box(Message.matrix[j][i], SBox_matrix);
		}
	}
    return Matrix_before_first_step;
}
// -----------------------------------------------------------------------------

// Fonction "Shiftrows" de AES qui effectue un décalage des octets de la matrice d'une data vers la gauche.
data shift_rows(data Message){
	int i;
	data Matrix_before_second_step;
	Matrix_before_second_step.size = 4;
	Matrix_before_second_step.matrix = malloc(4 * sizeof(unsigned char *));
    for (i=0; i<4; i++) {
        Matrix_before_second_step.matrix[i] = malloc(4 * sizeof(unsigned char));
    }
    // 1ere ligne --> On copie simplement.
    for(i=0; i<4; i++){
    	Matrix_before_second_step.matrix[0][i] = Message.matrix[0][i];
    }
    // 2eme ligne --> Décalage de 1 vers la gauche.
    for(i=0; i<4; i++){
    	Matrix_before_second_step.matrix[1][i] = Message.matrix[1][(i+1)%4];
    }
    // 3eme ligne --> Décalage de 2 vers la gauche.
    for(i=0; i<4; i++){
    	Matrix_before_second_step.matrix[2][i] = Message.matrix[2][(i+2)%4];
    }
    // 4eme ligne --> Décalage de 3 vers la gauche.
    for(i=0; i<4; i++){
    	Matrix_before_second_step.matrix[3][i] = Message.matrix[3][(i+3)%4];
    }
    return Matrix_before_second_step;
}
// -----------------------------------------------------------------------------

// Fonction "RotWord" de AES qui effectue un décalage vers le haut d'une colonne donnée dans une matrice d'une data.
void RotWord(data Message, int wich_column){
	assert(wich_column<4);
	int i;
	unsigned char temp[4];
	temp[0] = Message.matrix[1][wich_column];
	temp[1] = Message.matrix[2][wich_column];
	temp[2] = Message.matrix[3][wich_column];
	temp[3] = Message.matrix[0][wich_column];
	for(i=0; i<4; i++){
		Message.matrix[i][wich_column] = temp[i];
	}
}
// -----------------------------------------------------------------------------

// Fonction "KeySchedule" de AES qui genère les 10 clés de tours néssécaires au chiffrement.
void Key_Schedule(data Cle, data SBox_matrix){
	int i, j, k;
	for(i=0; i<11; i++){
		ListOfRoundKey[i].matrix = malloc(4 * sizeof(unsigned char *));
    	for (j=0; j<4; j++) {
     	  	ListOfRoundKey[i].matrix[j] = malloc(4 * sizeof(unsigned char));
	    }
	}
	for(i=0; i<11; i++){
		if(i==0){
			//La première clé de tour est simplement une copie de la clé de chiffrement.
			ListOfRoundKey[0] = copie_data(Cle);
		} else {
			// On copie 2 fois la round key précedente.
			data result_column = copie_data(ListOfRoundKey[i-1]);
			data temp_0 = copie_data(ListOfRoundKey[i-1]);
			// On fait rotword sur la 4 ème colone de la matrice.
			RotWord(temp_0, 3);
			// On fait sub_bytes pour la 4ème colonne de la matrice
			temp_0 = sub_bytes(temp_0, SBox_matrix);
			//printf("TEST 2\n\n\n");
			//print_data(temp_0);
			// On xor la dernière colonne en question avec RonCon pour obtenir la première colonne
			for(k=0; k<4; k++){
		    	result_column.matrix[k][0] = temp_0.matrix[k][3] ^ RonCon[i-1][k];
		    }
		    //printf("TEST 3\n\n\n");
			//print_data(result_column);
		    // Dernière etape pour obtenir la première colonne, on xor avec la première colonne de la roundkey précedente.
		    for(k=0; k<4; k++){
		    	result_column.matrix[k][0] = result_column.matrix[k][0] ^ ListOfRoundKey[i-1].matrix[k][0];
		    }
		    

		    // Maintenant, on complete les 3 colonnes suivantes de la roundkey.
		    // La première
		   	for(k=0; k<4; k++){
		    	result_column.matrix[k][1] = result_column.matrix[k][0] ^ ListOfRoundKey[i-1].matrix[k][1];
		    }
		    // La seconde
		    for(k=0; k<4; k++){
		    	result_column.matrix[k][2] = result_column.matrix[k][1] ^ ListOfRoundKey[i-1].matrix[k][2];
		    }
		    // La troisième
		    for(k=0; k<4; k++){
		    	result_column.matrix[k][3] = result_column.matrix[k][2] ^ ListOfRoundKey[i-1].matrix[k][3];
		    }

		    ListOfRoundKey[i] = copie_data(result_column);
		    if(debug == 1){
		    	printf("\n===Round_key generée n°%d===\n", i);
		    	//print_data(ListOfRoundKey[i]);
				printf("%s\n", convert_data_to_char(ListOfRoundKey[i]));

		    }
		    free_data(temp_0);
		    free_data(result_column);
		}
	}
	for(i=0; i<11; i++){
		ListOfRoundKey[i].size = 4;
		//printf("round key n°%d", i);
		//print_data(ListOfRoundKey[i]);
	}
}
// -----------------------------------------------------------------------------

// Fonction "AddRoundKey" de AES qui Xor une matrice avec une autre.
data AddRoundKey(data first, data second){
    data result = copie_data(first);
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            result.matrix[i][j] = first.matrix[i][j] ^ second.matrix[i][j];  // Utilisation de XOR
        }
    }
    return result;
}
// -----------------------------------------------------------------------------

// Fonction qui prends en argument une data et renvoit une nouvelle data avec les memes attributs.
data copie_data(data laquelle){
	int i, j;
	data nouvelle;
	nouvelle.size = laquelle.size;
	nouvelle.matrix = malloc(4 * sizeof(unsigned char *));
	for (j=0; j<4; j++) {
 	  	nouvelle.matrix[j] = malloc(4 * sizeof(unsigned char));
    }
    for(i=0; i<4; i++){
    	for(j=0; j<4; j++){
    		nouvelle.matrix[i][j] = laquelle.matrix[i][j];
    	}
    }
    return nouvelle;
}
// -----------------------------------------------------------------------------


// Fonction qui effectue une multiplication dans le corps de Galois entre deux hexadécimaux.
unsigned char galois_multiply(unsigned char a, unsigned char b) {
	int i;
    unsigned char p = 0;
    unsigned char carry;
    for (i=0; i<8; i++) {
        if(b&1) p^=a;
        carry = a&0x80;
        a<<=1;
        if(carry) a^=0x1B; 
        b >>= 1;
    }
    return p;
}
// -----------------------------------------------------------------------------

// Fonction "MixColumns" de AES qui effectue une multiplication dans le corps de Galois entre deux matrices.
data MixColumns(data input) {
	int i, j, k;
	unsigned char val;
    data result = copie_data(input); // Soit second soit lui-meme, le choix est arbitraire.
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            val = 0;
            for (k=0; k<4; k++) {
                val ^= galois_multiply(MixColumns_matrix[i][k], input.matrix[k][j]);
            }
            result.matrix[i][j] = val;
        }
    }
    return result;
}
// -----------------------------------------------------------------------------

// Fonction qui prends une data et qui retourne une chaine de caractère correspondante à sa matrice.
char *convert_data_to_char(data donnee) {
	int i, j;
    char *chaine = (char *)malloc(donnee.size * donnee.size * 2 + 1);
    int index = 0;
    for (i=0; i<donnee.size; i++) {
        for (j=0; j<donnee.size; j++) {
            snprintf(&chaine[index], 3, "%02X", donnee.matrix[j][i]);
            index += 2;
        }
    }
    chaine[donnee.size*donnee.size*2] = '\0';
    return chaine;
}
// -----------------------------------------------------------------------------

// Fonction qui effecture le chiffrement AES, avec un message et une cle de chiffrement donnée.
void AES(char * message, char * cle_de_chiffrement){
	unsigned char * message_en_clair = convert_char_to_hexa(message, 16);
	unsigned char * cle = convert_char_to_hexa(cle_de_chiffrement, 16);
	int i;
	printf("\n-----------  SBox (HEXADECIMAL)-------------\n");
    data matrice_de_s_box = create_s_box_matrix(SBox);
    print_data(matrice_de_s_box);

    printf("\n----------- MESSAGE EN CLAIR (HEXADECIMAL)-------------\n");
	data message_clear = create_matrix(message_en_clair, 16);
    print_data(message_clear);

    printf("\n----------- CLEF DE CHIFFREMENT (HEXADECIMAL)-------------\n");
    data cle_matrix = create_matrix(cle, 16);
    print_data(cle_matrix);

    // Etape 1 --> On génère les add round key.
	Key_Schedule(cle_matrix, matrice_de_s_box);
	//Etape 2 --> Chiffrement !
	data temp_result = AddRoundKey(message_clear, cle_matrix);
	if(debug == 1){
		printf("Première transformation\n\n\n");
		print_data(temp_result);
	}
	// On effectue les 10 tours de chiffrement, mais on se garde le dernier tour pour la fin.
	for(i=0; i<10; i++){
		if(i<=8){
			temp_result = sub_bytes(temp_result, matrice_de_s_box);
			temp_result = shift_rows(temp_result);
			temp_result = MixColumns(temp_result);
			temp_result = AddRoundKey(ListOfRoundKey[i+1], temp_result);
			if(debug == 1){
				printf("\n\n\n===APRES ROUND N°%d===\n", i+1);
				print_data(temp_result);
			}
		} else { // Lors du dernier tours on oublie pas de ne pas faire de MixColumns.
			temp_result = sub_bytes(temp_result, matrice_de_s_box);
			temp_result = shift_rows(temp_result);
			temp_result = AddRoundKey(ListOfRoundKey[i+1], temp_result);
			if(debug == 1){
				printf("\n\n\n===APRES ROUND N°%d===\n", i+1);
				print_data(temp_result);
			}
		}
	}
	printf("\n ---------- MESSAGE CHIFFRE (HEXADECIMAL) -------------\n");
	print_data(temp_result);

	char * text_chiffre = convert_data_to_char(temp_result);
	printf("MESSAGE CHIFFRE HEXADECIMAL = %s \n", text_chiffre);

	free(text_chiffre);
	free_data(temp_result);
    free_data(matrice_de_s_box);
    free_data(message_clear);
    free_data(cle_matrix);
}
// -----------------------------------------------------------------------------


int main() {
	clock_t start, end;
	char chaine[16];
	char cle[16];
	int choix = 0;
	printf("\n\n\n\n\n\n");
	printf("Tester l'exactitude du chiffrement (0)\nOu\nChiffrer votre propre message (1) :");
	scanf("%d", &choix);
	printf("\n\nDEBUG_MODE (1) --> YES, (0) --> NO :");
	scanf("%d", &debug);

	if(choix == 0){
		printf("\n\n\n\n\n\n");
		printf("########## CHIFFREMENT AES ###########\n");
		printf("Chiffrage de Two One Nine Two, avec la clé Thats my Kung Fu\n");
		// On entre les données présentes dans l'article https://www.kavaliro.com/wp-content/uploads/2014/03/AES.pdf pour tester la l'exactitude de notre chiffrement.
		start = clock();
		AES("Two One Nine Two", "Thats my Kung Fu");
		end = clock();
		clock_t time = (end - start);
		printf("Temps d'execution de mon implémentation : %ld ticks\n", time);
	} if(choix == 1) {
		printf("\n\n\n\n\n\n");
		printf("Veuillez entrer un message (<16): ");
		scanf("%15s", chaine);
		printf("Veuillez entrer une cle de chiffrement (<16): ");
		scanf("%15s", cle);

		printf("########## CHIFFREMENT AES ###########\n");
		printf("Chiffrage de %s, avec la clé %s\n", chaine, cle);
		// On entre les données présentes dans l'article https://www.kavaliro.com/wp-content/uploads/2014/03/AES.pdf pour tester la l'exactitude de notre chiffrement.
		start = clock();
		AES(chaine, cle);
		end = clock();
		clock_t time = (end - start);
		printf("Temps d'execution de mon implémentation : %ld ticks\n", time);
	}
    

    return 0;
}
