import json
import csv
import os

json_dir = "scrapped/"
outputCsv = "dataset.csv"

json_files = [f for f in os.listdir(json_dir) if f.endswith('.json')]

# Modèle de données avec tous les champs possibles et leurs valeurs par défaut.
data_model = {
    'marque': '', 'modele': '', 'couleur': '', 'kilometrage': 5, 'boite_de_vitesse': '',
    'annee': 5, 'prix_ttc': 5, 'puissance_fiscale': 5, 'type_vehicule': '', 'carburant': 'Inconnu',
    'portes': 5, 'places': 5, 'ville': '', 'garantie': '', 'mise_en_circulation': '',
    'reference': '', 'equipements': '', 'liste_equipements_serie': ''
}

def WriteInCsv(inputJson, output, mode, i):
    with open(inputJson) as json_file:
        data = json.load(json_file)

    data_with_defaults = data_model.copy()
    data_with_defaults.update(data)

    for key, value in data_with_defaults.items():
        if isinstance(value, str):
            # On supprime le bruit dans les données. Ex: les caractères € et "Km".
            data_with_defaults[key] = value.replace('€', '').replace('Km', '').replace('CV', '').replace('portes', '').replace('places', '').replace(' ', '').replace('-', '5')
        
    if not str(data_with_defaults['carburant']).isdigit():
        with open(output, mode, newline='') as csv_file:
            writer = csv.writer(csv_file)

            if mode == 'w':
                headers = list(data_with_defaults.keys())
                writer.writerow(headers)

            if((data_with_defaults['marque']) != ""):
                writer.writerow(list(data_with_defaults.values()))

# On vient écrire nos lignes dans le dataset.
for i, json_file in enumerate(json_files):
    if(i<10000000):
        mode = 'w' if i == 0 else 'a'  # 'w' pour le premier fichier, 'a' pour les suivants
        WriteInCsv(os.path.join(json_dir, json_file), outputCsv, mode, i)
        print(str(i) + "/" + json_file + str(outputCsv[0]) + "_" + str(outputCsv[1]))
        
    else: 
        pass


# On effectue un nouveau traitement sur le fichier csv pour s'assurer qu'aucune ligne ne soit vide.
import pandas as pd
def remove_empty_brand_rows(csv_file):
    # Lire le fichier csv
    df = pd.read_csv(csv_file)

    df = df[df['marque'].notna()]

    df.to_csv(csv_file, index=False)

remove_empty_brand_rows(outputCsv)

# On normalise tout le texte du .csv en minuscule.
def convert_to_lowercase(csv_file):
    df = pd.read_csv(csv_file)

    df = df.apply(lambda x: x.astype(str).str.lower())

    df.to_csv(csv_file, index=False)

convert_to_lowercase(outputCsv)
