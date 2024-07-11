import scrapy
import os
import json

class AutoPlusSpider(scrapy.Spider):
    name = 'quotes'
    # Pour chaque page de l'url de base 
    start_urls = ['https://www.autosphere.fr/recherche?market=VO&page={}&'.format(page) for page in range(1, 1000000)]
    # On vient désactiver la prise en compte de robots.txt.
    custom_settings = {
        'ROBOTSTXT_OBEY': False
    }
    counter = 0

    def parse(self, response):
        # On extrait chaque liens d'annonces de la page de recherche parcourue.
        fiche_links = response.css('a.lien-fiche.link_veh::attr(href)').extract()
        # Pour chaque lien d'annonce
        for link in fiche_links:
            yield scrapy.Request(url=link, callback=self.parse_fiche_technique)

    # On scrappe les informations de la page d'annonce qui nous interessent.
    def parse_fiche_technique(self, response):
        # On garde uniquement l'essentiel pour notre futur model.
        marque = response.css('div.description__item:nth-child(3) span.valeur::text').extract_first()
        couleur = response.css('div.description__item:nth-child(4) span.valeur::text').extract_first()
        modele = response.css('div.description__item:nth-child(5) span.valeur::text').extract_first()
        kilometrage = response.css('div.description__item:nth-child(6) span.valeur::text').extract_first()
        boite_de_vitesse = response.css('div.description__item:nth-child(7) span.valeur::text').extract_first()
        annee = response.css('div.description__item:nth-child(8) span.valeur::text').extract_first()
        prix_ttc = response.css('div.description__item:nth-child(9) span.valeur::text').extract_first()
        puissance_fiscale = response.css('div.description__item:nth-child(10) span.valeur::text').extract_first()
        #type_vehicule = response.css('div.description__item:nth-child(11) span.valeur::text').extract_first()
        carburant = response.css('div.description__item:nth-child(16) span.valeur::text').extract_first()
        portes = response.css('div.description__item:nth-child(17) span.valeur::text').extract_first()
        places = response.css('div.description__item:nth-child(18) span.valeur::text').extract_first()
        #ville = response.css('div.description__item:nth-child(19) span.valeur::text').extract_first()
        #garantie = response.css('div.description__item:nth-child(20) span.valeur::text').extract_first()
        #mise_en_circulation = response.css('div.description__item:nth-child(21) span.valeur::text').extract_first()
        #reference = response.css('div.description__item:nth-child(22) span.valeur::text').extract_first()
        #equipements = response.css('ul.equipement_option.trois_colonne li::attr(id)').getall()
        #liste_equipements = []
        #for equipment in equipements:
        #    liste_equipements.append(equipment)
            
        #equipements_serie = response.css('ul.new_coche.vehicule_equipement li::text').getall()
            
        #liste_equipements_serie = []
        #for equipment_serie in equipements_serie:
        #    liste_equipements_serie.append(equipment_serie)
            
            
        # On vien stocker dans "scrapped" le fichier json.
        filename = os.path.join("scrapped", f"fiche_{modele}_"+ str(self.counter) +"_.json")
        self.counter +=1

        # Enregistrement et Serialisation des données dans un fichier JSON
        with open(filename, 'w', encoding="utf-8") as f:
            f.write(json.dumps({
                'marque': marque,
                'modele': modele,
                'couleur': couleur,
                'kilometrage': kilometrage,
                'boite_de_vitesse': boite_de_vitesse,
                'annee': annee,
                'prix_ttc': prix_ttc,
                'puissance_fiscale': puissance_fiscale,
                #'type_vehicule': type_vehicule,
                'carburant': carburant,
                'portes': portes,
                'places': places,
                #'ville': ville,
                #'garantie': garantie,
                #'mise_en_circulation': mise_en_circulation,
                #'reference': reference,
                #'equipements': liste_equipements,
                #'liste_equipements_serie': liste_equipements_serie
            }, indent=4))
            
    # On modifie les paramètres du scrapper pour bypasser la sécurité.
    def process_request(self, request, spider):
        request.headers['User-Agent'] = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3'
        return None
