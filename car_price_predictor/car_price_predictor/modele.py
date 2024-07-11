import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder, StandardScaler
from sklearn.compose import ColumnTransformer
from sklearn.pipeline import Pipeline
from xgboost import XGBRegressor
from sklearn.metrics import mean_squared_error
from math import * 
import xgboost as xgb
from datetime import *

# Lecture de notre dataset, en retirant les champs indésirés.
df = pd.read_csv('dataset.csv').drop(columns=['reference', 'garantie', 'ville', 'equipements', 'liste_equipements_serie', 'type_vehicule', 'mise_en_circulation'])

print(f"Nombre de lignes prises en compte : {len(df)}")

# Anciennes conversions du champ 'mise_en_circulation'.
#df['mise_en_circulation'] = pd.to_datetime(df['mise_en_circulation'], format='%d/%m/%Y')
#df['mise_en_circulation'] = ((datetime.now() - df['mise_en_circulation']).dt.days.astype(int))

# On retire la colonne que l'on souhaite prédire.
X = df.drop('prix_ttc', axis=1)
y = df['prix_ttc']

# On split notre dataset (train/test).
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# On défini les colonnes numériques et catégorielles
num_cols = ['annee', 'kilometrage', 'places', 'portes', 'puissance_fiscale']
cat_cols = [col for col in X.columns if col not in num_cols]

# On utilise un pré-processeur pour normaliser nos données.
preprocessor = ColumnTransformer(
    transformers=[
        ('num', StandardScaler(), num_cols),
        ('cat', OneHotEncoder(handle_unknown='ignore'), cat_cols)])

# On utilise XGBoost car plus adapté sur les données tabulaires.
model = Pipeline(steps=[('preprocessor', preprocessor), 
                        ('regressor', XGBRegressor())])

model.fit(X_train, y_train)

y_pred = model.predict(X_test)

# On évalue l'erreur.
mse = mean_squared_error(y_test, y_pred)
print(f"Erreur quadratique moyenne: {mse}")
print(f"L'écart de prix moyen est : {sqrt(mse)}")

import json

with open("../car_config.json", 'r') as fichier_json:
    car_config = json.load(fichier_json)
prix_predit = model.predict(pd.DataFrame(car_config, index=[0]))
prix_predit = int(prix_predit[0])

print(f"Le prix prédit pour la {car_config['marque']} {car_config['modele']} est de : {prix_predit}€")
