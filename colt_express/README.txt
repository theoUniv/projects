Travail réalisé par Ichou Aymane et Lemaire Théo 
Pour lancer le jeu , il faut entrer nom par nom les joueurs (par defaut 3) : 
Mettre le nom puis appuyer sur l'icone "+" , et une fois fini , appuyer encore une fois sur le symbole "+" pour confirmer et lancer le jeu.
Les parties du sujet que vous avez traitées en nous reportant au partie présente sur le pdf du projet (joint à l'archive): 

Les consignes annotés de ● signifie que nous les avons traités. 
Les consignes annotés de ●●● signifie que nous les avons pas traités. 

Dans la partie 1 : 
	
	Les actions possibles pour les bandits sont :
	● Se déplacer d’un wagon en avant ou en arrière, en restant au même étage.
	● Aller à l’intérieur ou grimper sur le toit de leur wagon actuel.
	● Braquer un voyageur pour récupérer du butin (ou simplement récupérer un butin qui a été
	abandonné là).
	● Tirer sur un autre bandit proche pour lui faire lâcher son butin.
	Les butins récupérables à bord du train sont :
	● Des bourses valant 100€ ou 200€, auprès des passagers, à l’intérieur des wagons.
	● Des bijoux valant 500€, auprès des passagers, à l’intérieur des wagons.
	● Un magot valant 1000€, à l’intérieur de la locomotive, sous la garde du Marshall.
	● Un Marshall est présent à bord du train et peut se déplacer entre la locomotive et les wagons, en
	restant toujours à l’intérieur. Il tire sur tous les bandits qui se trouvent à la même position que lui et les
	force à se retrancher sur le toit.
	
Dans la partie 2 : 

	● Le nombre des wagons est donné par une constante NB_WAGONS que vous devez définir
	comme attribut de la classe principale. Vous pourrez par exemple la fixer à 4.
	● On placera pour l’instant un unique bandit à bord du train, sur le toit du dernier wagon. Le
	nom de ce bandit est donné par une constante NOM_BANDIT_1.
	● Un déplacement vers l’arrière lorsque l’on est dans le dernier wagon ou en avant lorsque l’on
	est dans la locomotive n’a pas d’effet. De même pour un déplacement vers le toit ou vers
	l’intérieur lorsque l’on y est déjà.
	● A chaque action du bandit, affichez un compte rendu sur la sortie standard.
	Par exemple :
	Joueur1 grimpe sur le toit.
	Joueur1 est déjà sur le dernier wagon.
	
Dans la partie 3 : 
	
	La partie la plus importante du projet est de réaliser une belle interface graphique.
	Pour commencer, ajoutez une interface comportant deux parties :
	● Un panneau d’affichage dans lequel on voit les différents éléments du train et la position du
	bandit.
	● Un bouton “Action” qui effectue la prochaine action du bandit.
	Quelques précisions :
	● Pour l’instant, le bandit n’effectue qu’une suite d’actions prédéterminées.
	● L’affichage doit être mis à jour à chaque déplacement du bandit.
	● Continuez à afficher le compte-rendu sur la sortie standard.
	
Dans la partie 4 : 

	● A l'intérieur de chaque wagon se trouvent initialement entre 1 et 4 butins (de type bourse ou
	bijou). La nature des butins présents à chaque position doit apparaître sur l’interface, mais
	pas leur valeur.
	● Le magot et le Marshall sont placés à l’intérieur de la locomotive.
	● Avant chaque action du bandit, le Marshall se déplace dans une direction aléatoire (mais
	toujours à l'étage inférieur, c’est-à-dire à l'intérieur des wagons).
	● Le bandit a maintenant accès à une nouvelle action : braquage. Cette action lui fait récupérer
	un butin au hasard parmi ceux présents sur sa position.
	● Si le Marshall atteint ainsi la position du bandit, le bandit lâche un des butins qu’il a ramassé
	tiré au hasard (s’il en a) et se déplace immédiatement vers le toit. Le butin est ajouté à
	l’ensemble des butins de la position dont le bandit vient d’être chassé, et peut être récupéré à
	nouveau.
	● Continuez à afficher sur la sortie standard un compte-rendu de tous ces évènements.
	● Une fois toutes les actions effectuées, affichez le montant total des butins possédés par le
	bandit.
	
Dans la partie 5 : 
	
	Toujours en considérant un seul bandit, ajoutez maintenant de nouveaux boutons permettant au
	joueur de planifier les actions de son personnage. A l'état de planification, le joueur doit donner un
	certain nombre d’ordres en cliquant sur les boutons correspondants. Le bouton “Action” est inopérant
	pendant cette phase. A l'état d’action, dans lequel chaque clic sur le bouton “Action” effectue la
	prochaine action, les autres boutons doivent être inopérants.
	Quelques précisions :
	● Le panneau de commande doit afficher une ligne de texte indiquant quelle est la phase en
	cours (voir par exemple la classe Label).
	●Le nombre d’actions que doit indiquer le joueur lors d’une phase de planification est une
	constante NB_ACTIONS que vous pouvez fixer par exemple à 4.
	
Dans la partie 6 : 

	Une vue supplémentaire devra montrer : 
		●l’état de chaque bandit, 
			●●●en particulier les butins qu’il porte et 
			●●●le nombre de balles qu’il a encore à sa disposition.
	
	●C’est le moment également d’ajouter aux bandits l’action de tir.
	Quelques précisions :
	● Le nombre de joueurs est donné par une constante NB_JOUEURS qui est égale au nombre
	de wagons.
	● Les joueurs jouent toujours dans le même ordre.
	● Lors de la phase de planification, chaque joueur à son tour donne tous ses ordres à la fois.
	Lors de la phase d’action, on effectue d’abord toutes les premières actions (dans l’ordre des
	joueurs), puis toutes les deuxièmes actions (dans le même ordre), et ainsi de suite.
	●●● Un tir est fait dans l’une des quatre directions: en avant ou en arrière au même étage, ou vers
	le haut ou vers le bas dans le même wagon. Un tir vers le haut (resp. vers la bas) lorsqu’un
	bandit se trouve sur le toit (resp. à l’intérieur) vise la position occupée par le bandit.
	● Le tir touche un bandit tiré au hasard parmi les occupants de la position visée, excepté le
	tireur lui-même.
	● Un bandit touché par un tir lâche l’un de ses butins tiré au hasard. Le butin est ajouté à ceux
	présents à la position du bandit touché et pourra être récupéré à nouveau.
	●●●Chaque tir utilise une balle. Chaque bandit possède à l’origine un nombre de balles donné
	par une constante NB_BALLES, que vous pourrez par exemple fixer à 6.	
	● Le compte-rendu textuel doit également mentionner ces actions de tir et leurs conséquences.
	
Expression libre : 

	● Le déplacement du Marshall a été fait de la manière suivante : 
		Il se déplace d'une position uniquement .
		La nervosité du marshall est choisi aléatoirement(random) entre 0 et 1 : 
			0 correspondant à un déplacement sur la gauche, 
			et 1 sur la droite . 
			Nous avons donc décidé que le Marshall se déplaçait obligatoirement lors d'un tour . 
			Il est donc possible que le Marshall ne se déplace pas dans le cas où : 
			0 et le Marshall est dans la locomotive,
			1 et le Marshall est dans le dernier wagon. 
	● Blocage de la taille de la fenêtre . 
	● Ajout d'un nametag au dessus de chaque bandit. 
	● Lors de la planification, afficher la séquence d’ordres donnée par le joueur courant.
	● Afficher le compte-rendu de la partie non sur la sortie standard mais dans une zone dédiée
	de la fenêtre graphique, ducoup nous avons retiré les compte rendus dans le terminal. 
	
---------------------------------------

Les problèmes que vous avez rencontrés et que vous avez réussi à éliminer: 

	Lors des débuts, nous comprenions pas bien l'utilisation des classes, leurs impacts, leurs dépendances, leurs héritages etc.. 
	Lors de l'élaboration du sytème de tours, l'utilisation de boucle while faisait bugger notre interface : 
		Nous ne pouvions ni afficher, ni utiliser les boutons. Nous avons donc dû eviter l'utilisation de la boucle while.
		Et rechercher des substerfuges pour ne pas à l'avoir à l'utiliser. 
		
	Lors de l'élaboration de la fenêtre de jeu et de l'ajout des différentes images, les images conservaient leurs background.
		Ce qui avait comme conséquence, une interface avec des images pas très esthétique détouré d'un rectangle gris.
		
	Lors de la recherche d'image pour notre projet, les images ne nous convenaient pas. Nous avons donc décidé de tous dessiner nous même.
	
	La concordance de nos emplois du temps , et le partage de nos informations , nous y avons remédié en : 
		Mettant en place des créneaux horaires pour pouvoir travailler ensemble sur le projet . 
			On a fait le choix de travailler en même temps pour être toujours au courant des agissements de l'autre .
		Création régulière de backup , pour pouvoir retourner en arrière si le programme bug suite à un ajout . 
		Création d'un discord pour répertorier tous les ajouts fait ou non .  
	
	L'apport d'image , et la gestion de celle-ci : 
		Savoir comment les placer , comment les agrandir ou les rétrécir , les positionner à l'endroit voulu .
		
	La compréhension des attentes que vous avez envers nous avec le pdf . 
		Le problème a été résolu en vous posant des questions pour essayer de répondre aux maximums à vos attentes 
		
	
	
--------------------------------------

Les problèmes qui sont présents et que vous n’avez pas pu éliminer : 
	Comme dit précédemment , nous avons pas su faire le système de tour en utilisant la boucle while , mais nous avons fait différemment ducoup
	Afficher un canvas sur un canvas : il est impossible de le faire , mais nous nous sommes rendu compte trop tard de cette impossibilité 
		Il aurait fallu refaire une refonte partielle de notre code , car plusieurs parties de notre code dépendent du fonctionnement
		d'un canvas . 
		Visuellement, cela à pour effet de détourer nos bandits d'un rectangle gris , mais aussi de cacher un bandit si il est sur même
		position qu'un autre .
		
--------------------------------------

Les morceaux de code écrits à plusieurs ou empruntés ailleurs : 
	Tout le code est 100% fait maison, artisanal, de saint-denis meme, soit par Théo , soit par Aymane . 
	
	
	
		
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
