Théo Lemaire - Aymane Ichou -> L3-X Informatique

######################## Car Price Predictor #######################

Execution du projet:

--> Dans le sous-répertoire "car_price_predictor"
    - Si vous souhaitez re-scrapper l'entièreté des données:
            --> clear && scrapy crawl quotes
        (Scrappe le site Autosphere et crée un fichier Json pour chaque annonce dans le répertoire "scrapped")

    - Pour convertir les fichiers .json scrappés en dataset.csv:
            --> clear && python3 JsonToCsv.py
        (Insère dans le fichier "dataset.csv" l'ensemble des données dans les fichiers Json)

    - Pour entrainer le modèle sur le dataset, et effectuer une prédiction:
            --> clear && python3 modele.py
        (Entraine le modèle sur le dataset actuel)

- Pour modifier les caractéristiques du véhicule:
        --> Fichier de configuration "car_config.json"