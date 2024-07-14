from tkinter import *
import tkinter as tk
import pygame
from PIL import Image, ImageTk
import random
from tkinter import scrolledtext

class Jeu(Tk):
    def __init__(self, nb_wagons, nb_joueurs, nb_actions, nb_tours):
        super().__init__()
        self.minsize(1200, 700)
        self.maxsize(1280, 768)
        #Initialisation des variables de jeu------------------------------------------------------------------------------------------------------
        self.up = StringVar()
        self.up.set("UP")
        self.down = StringVar()
        self.down.set("DOWN")
        self.left = StringVar()
        self.left.set("LEFT")
        self.right = StringVar()
        self.right.set("RIGHT")
        # Intégration des images ------------------------------------------------------------------------------------------------------------------
        self.gun_image = Image.open("gun.jpg").resize((30, 30))
        self.gun_image = ImageTk.PhotoImage(self.gun_image)
        self.image_loco = Image.open("locomotive.jpg").resize((200, 150))
        self.image_loco = ImageTk.PhotoImage(self.image_loco)
        self.image_wagon = Image.open("wagon.png").resize((200, 150))
        self.image_wagon = ImageTk.PhotoImage(self.image_wagon)
        self.image_up = Image.open("fleche_haut.png")
        self.image_up = ImageTk.PhotoImage(self.image_up)
        self.image_down = Image.open("fleche_bas.png")
        self.image_down = ImageTk.PhotoImage(self.image_down)
        self.image_left = Image.open("fleche_gauche.png")
        self.image_left = ImageTk.PhotoImage(self.image_left)
        self.image_right = Image.open("fleche_droite.png")
        self.image_right = ImageTk.PhotoImage(self.image_right)
        self.image_valider = Image.open("valider.png")
        self.image_valider = ImageTk.PhotoImage(self.image_valider)
        self.image_delete = Image.open("delete.png")
        self.image_delete = ImageTk.PhotoImage(self.image_delete)
        self.image_marshall = Image.open("marshall.png").convert("RGBA").convert("RGBA")
        self.image_marshall = ImageTk.PhotoImage(self.image_marshall)
        self.image_bandit = Image.open("bandit.png").convert("RGBA").convert("RGBA")
        self.image_bandit = ImageTk.PhotoImage(self.image_bandit)
        self.gold = Image.open("gold.png").resize((20, 20))
        self.gold = ImageTk.PhotoImage(self.gold)
        self.button_add_image = Image.open("button_base.png").resize((40, 40))
        self.button_add_image = ImageTk.PhotoImage(self.button_add_image)
        self.background = Image.open("back.png").resize((1024,768))
        self.background = ImageTk.PhotoImage(self.background)
        # Initialisation des éléments du jeu (Train, UI..)---------------------------------------------------------------------------------------------
        self.nb_tours_played = 0
        self.nb_butin = 3
        self.nb_wagons = nb_wagons
        self.nb_joueurs = nb_joueurs
        self.nb_actions = nb_actions
        self.nb_tours = nb_tours
        self.current_bandit = 0
        self.train = Train(self, 1024, 768)
        self.train.grid(row = 0, column = 0)
        frame_UI = Frame(self,width = 500, height = 750, bg='orange')
        frame_UI.grid(row = 0, column = 1)
        #Partie création de bandits ---------------------------------------------------------------------------------------------------------------
        self.bandit_list = []
        self.bandit_entry_name = Entry(frame_UI)
        self.bandit_entry_name.grid(column=2, row=1)
        self.add_bandit = Button(frame_UI, command=self.create_bandit, image=self.button_add_image)
        self.add_bandit.grid(column=2, row=2)
        #MARSHALL ------------------------------------------------------------------------------------------------------------------------------------
        self.marshall_bandit = Marshall(self, "Marshall", 0, 1)
        self.bandit_list.append(self.marshall_bandit)
        # Création des boutons -----------------------------------------------------------------------------------------------------------------
        self.haut = Button(frame_UI, width=30, image=self.image_up)
        self.tirer = Button(frame_UI, width=30, image=self.gun_image)    
        self.braque = Button(frame_UI, width=15, highlightbackground='ivory', text="Braquer")    
        self.dep_d = Button(frame_UI, width=30, image=self.image_right)
        self.dep_g = Button(frame_UI, width=30, image=self.image_left)   
        self.bas = Button(frame_UI, width=30, image=self.image_down)   
        self.suiv = Button(frame_UI, width=30, image=self.image_valider)
        self.delete = Button(frame_UI, width=30, image=self.image_delete)
        self.actions_selected = scrolledtext.ScrolledText(frame_UI,
                                      font = ('bold', 10), 
                                      wrap = tk.WORD, 
                                      width = 30, 
                                      height = 20, 
                                      )
        self.var_text = StringVar()
        self.current_text = Label(frame_UI, textvariable=self.var_text)

        #Méthodes de la classe principale pour le placement/affichage des bandits --------------------------------------------------------------------------------------
    
    def create_bandit(self): # Méthode --> Création des bandits
        if(Bandit.nb_joueurs_acc < (self.nb_joueurs)):
            if(self.bandit_entry_name.get()!= ""):
                self.bandit_list.append(Bandit(self, self.bandit_entry_name.get(), 1, 0))
                self.bandit_entry_name.delete (0, END)
                print("Bandit vient d'etre crée")
                return True
            else:
                print("Veuillez entrer un nom valide !")
        else:
            print("Limite de bandits atteinte !")
            for i in range(0, len(self.bandit_list)):
                self.bandit_list[i].position = i
                if i == 0:
                    self.bandit_list[i].place(x=(i*200)+220, y=530)
                else:
                    self.bandit_list[i].place(x=(i*200)+120, y=440)
            self.add_bandit.destroy()
            self.bandit_entry_name.destroy()
            self.after(1000, self.play_prog())
            self.generate_butin()
            return False

    def generate_butin(self):
        self.clear_butin()
        for j in range(0, len(self.train.wagons_list)):
                for i in range(0, game.nb_butin):
                    wich_butins = random.randint(1, 2)
                    print(wich_butins)
                    if wich_butins == 1:
                        new_butin_b = Bourse(self)
                        Train.wagons_list[j].wagon_butin_list.append(new_butin_b)
                        new_butin_b.place(x=(j*200)+220, y=600)
                        print("Bourse générée avec succès")
                    if wich_butins == 2:
                        new_butin_bi = Bijou(self)
                        self.train.wagons_list[j].wagon_butin_list.append(new_butin_bi)
                        new_butin_bi.place(x=(j*200)+220, y=600)
                        print("Bijou généré avec succès")
                new_butin_m = Magot(self)
                self.train.wagons_list[j].wagon_butin_list.append(new_butin_m)
                new_butin_m.place(x=(j*200)+220, y=600)
                print("Bourse généré avec succès")

    def clear_butin(self):
        for i in self.train.wagons_list:
            i.wagon_butin_list.clear()

    def configure_ui_button(self): # Méthode --> Configuration des boutons de l'interface utilisateur
        self.haut.configure(command=lambda:self.add_action(Deplacement(self.up, self.bandit_list[self.current_bandit]))) 
        self.dep_d.configure(command=lambda:self.add_action(Deplacement(self.right, self.bandit_list[self.current_bandit])))
        self.dep_g.configure(command=lambda:self.add_action(Deplacement(self.left, self.bandit_list[self.current_bandit])))
        self.bas.configure(command=lambda:self.add_action(Deplacement(self.down, self.bandit_list[self.current_bandit])))
        self.tirer.configure(command=lambda:self.add_action(Tir(self.bandit_list[self.current_bandit])))
        self.braque.configure(command=lambda:self.add_action(Braquage(self.bandit_list[self.current_bandit], Train.wagons_list[self.bandit_list[self.current_bandit].position])))
        self.suiv.configure(command=lambda:self.check_turn_and_change(self.bandit_list[self.current_bandit]))
        self.delete.configure(command=lambda:self.remove_last_action(self.bandit_list[self.current_bandit]))

    def show_ui(self, true): #Méthode --> Affiche ou non les éléments de l'UI.
        if true == True:
            self.haut.grid(column=2, row=3)   
            self.tirer.grid(column=2, row=4)
            self.braque.grid(column=2, row=5)
            self.dep_d.grid(column=3, row=5)
            self.dep_g.grid(column=1, row=5)
            self.bas.grid(column=2, row=6)
            self.suiv.grid(column=2, row=10)
            self.delete.grid(column=2, row=15)
            self.actions_selected.grid(column=2, row=20)
            self.current_text.grid(column=2, row=0)
        else:
            self.haut.grid_remove()   
            self.tirer.grid_remove()
            self.braque.grid_remove()
            self.dep_d.grid_remove()
            self.dep_g.grid_remove()
            self.bas.grid_remove()
            self.suiv.grid_remove()
            self.delete.grid_remove()
            self.current_text.grid_remove()

        #Phase de programmation ---------------------------------------------------------------------------------------------------------------------

    def play_prog(self): # Méthode --> Joue la phase de programmation.
        if self.nb_tours_played <= self.nb_tours:
            self.generate_butin()
            self.current_bandit = 1
            self.var_text.set("{} séléctionne ses actions.".format(self.bandit_list[self.current_bandit].nom))
            print(self.current_bandit)
            self.show_ui(True)
            self.configure_ui_button()
            print("|---Phase de Programmation---|\n\n")
            text = "Phase de Programmation\n\n"
            self.actions_selected.insert(END, text)
        else:
            self.end_game()

    def check_turn_and_change(self, bandit): # Méthode --> Elle check si tout est effectué pour la phase de Programmation et switch avec la pĥase d'Action
        if(self.is_enought_action(bandit) == True):
            if self.current_bandit != self.nb_joueurs:
                self.current_bandit+=1
                self.var_text.set("{} séléctionne ses actions.".format(self.bandit_list[self.current_bandit].nom))
                print("PROG||Passage au tour du bandit suivant. --> {}".format(self.bandit_list[self.current_bandit].nom))
                text = "Passage au tour du bandit suivant. --> {}\n\n".format(self.bandit_list[self.current_bandit].nom)
                self.actions_selected.insert(END, text)
            else:
                self.generate_marshall_action()
                print("Chargement de la phase d'action.")
                self.after(1000, self.play_action())
                self.current_bandit = 1
                #Passage à la phase d'action.

    def remove_last_action(self, bandit): # Méthode --> Retire la dernière action séléctionnée.
        if(len(bandit.list_temp_action) > 0):
            bandit.list_temp_action.clear()
            print("PROG||La dernière action a été retirée.")
            text = "La dernière action a été retirée.\n\n"
            self.actions_selected.insert(END, text)
        else:
            print("PROG||La liste d'action est vide.")
            text = "La liste d'action est vide.\n\n"
            self.actions_selected.insert(END, text)

    def add_action(self, action): # Méthode --> Ajoute une action quand elle est séléctionnée
        if self.check_double(self.bandit_list[self.current_bandit], action):
            if (self.is_enought_action(self.bandit_list[self.current_bandit]) == False):
                self.bandit_list[self.current_bandit].list_temp_action.append(action)
                print("L'action {} à été séléctionnée par {}.".format(action.name, self.bandit_list[self.current_bandit].nom))
                text = "L'action {} à été séléctionnée par {}.\n\n".format(action.name, self.bandit_list[self.current_bandit].nom)
                self.actions_selected.insert(END, text)
            else:
                print("Vous avez atteint le maximum d'actions possible ({}), veuillez valider vos actions séléctionnés.".format(self.nb_actions))
                text = "Vous avez atteint le maximum d'actions possible ({}), veuillez valider vos actions séléctionnés.\n\n".format(self.nb_actions)
                self.actions_selected.insert(END, text)
            self.actions_selected.see("end")

    def is_enought_action(self, bandit): # Méthode --> Regarde si le bandit a séléctionné toutes ses actions.
        if len(bandit.list_temp_action) < self.nb_actions:
            print("Encore {} action(s) à choisir".format((self.nb_actions-1) - len(bandit.list_temp_action)))
            text = "Encore {} action(s) à choisir.\n\n".format((self.nb_actions-1) - len(bandit.list_temp_action))
            self.actions_selected.insert(END, text)
            return False
        else:
            print("Le bandit a déjà séléctionné toutes ses actions.")
            return True

    def check_double(self, bandit, action):
            if len(bandit.list_temp_action) != 0:
                if isinstance(action, Deplacement):
                    if isinstance(bandit.list_temp_action[0], Deplacement):
                        if bandit.list_temp_action[0].direction == action.direction:
                            print("Impossible de séléctionner un doublon.")
                            return False
                        else:
                            return True
                if isinstance(bandit.list_temp_action[0], type(action)):
                    print("Impossible de séléctionner un doublon.")
                    return False
                return True 
            else:
                return True

    def generate_marshall_action(self):
            wich_action = 1
            if wich_action == 0:
                new_action = Deplacement(self.left, self.bandit_list[0])
            else:
                new_action = Deplacement(self.right, self.bandit_list[0])
            self.bandit_list[0].list_temp_action.append(new_action)

    #Phase d'Action --------------------------------------------------------------------------------------------------------------------------------------------

    def play_action(self): # Méthode --> Joue la phase d'Action.
        self.after(100, self.show_ui(False))
        self.after(500, self.actions_selected.delete("1.0","end"))
        print("\n\n\n\n")
        text = "La phase d'action commence !"
        self.after(500, self.actions_selected.insert(END, text))
        for i in range(0, self.nb_actions):
                for j in range(1, len(self.bandit_list)): #Pour chaque bandit 
                    self.check_action_and_play_it(self.bandit_list[j].list_temp_action, self.bandit_list[j], i)
                    text = "L'action n°{} du bandit {} est {} :\n\n".format(i+1, self.bandit_list[j].nom, self.bandit_list[j].list_temp_action[i].name)
                    self.actions_selected.insert(END, text)
        self.bandit_list[0].check_around() #Le marshall regarde autour de lui 
        self.check_action_and_play_it(self.bandit_list[0].list_temp_action, self.bandit_list[0], 0)
        text = "L'action du Marshall est {} :\n\n".format(self.bandit_list[0].list_temp_action[0].name)
        self.actions_selected.insert(END, text)
        self.nb_tours_played+=1
        # REINITIALISATION DE TOUS LES PARAMETRES DE JEU POUR LE TOUR SUIVANT
        for i in self.bandit_list:
            if i.nom == "Marshall":
                i.list_temp_action.clear()
            else:
                i.somme_butin_bandit()
                i.list_temp_action.clear()
                print("Butin de {} = {}".format(i.nom, i.somme)) 
        self.current_bandit = 1
        self.actions_selected.delete("1.0","end")
        # ----------------------------------------------------------------------------------------------------------------------------------------
        self.play_prog()

    def check_action_and_play_it(self, list_action, bandit, number_action): # Méthode appelée pour l'execution d'une action donnée par l'indice 'number_action'
            if isinstance(list_action[number_action], Tir):
                list_action[number_action].tirer()
            if isinstance(list_action[number_action], Braquage):
                list_action[number_action].braquer()
            if isinstance(list_action[number_action], Deplacement):
                #print("{} est la position de {}".format(bandit.position, bandit.nom))
                list_action[number_action].se_deplacer()
                bandit.actualiser_position()
            

    # Fonctions globales de fin de jeu. ---------------------------------------------------------------------------------------------------------

    def end_game(self):
        self.show_ui(False)
        self.who_win()

    def who_win(self): # Méthode --> Regarde qui a gagné
        for i in range(1, self.nb_joueurs-1):
            self.bandit_list[i].somme_butin_bandit()
            print("BUTIN {}|| {}".format(self.bandit_list[i].nom, self.bandit_list[i].somme))
            if self.bandit_list[i].somme > self.bandit_list[i+1].somme:
                winner = self.bandit_list[i]
            else:
                winner = self.bandit_list[i+1]
        print("Le vainqueur est {} !".format(winner.nom))

# Définition des différentes classes ---------------------------------------------------------------------------------------------------------------
class Train(Canvas):
    wagons_list = []
    nb_train_acc = 0
    def __init__(self,game : Tk, width, height):
        super().__init__(game, width= width, height =height, bg='orange')
        #nb_wagons = nb_joueurs + 1
        self.create_image(0, 0, image=game.background, anchor="nw")
        Train.wagons_list.append(Elt_train(self, True, "Locomotive")) #On créer la locomotive.
        for i in range(1, game.nb_wagons+1): # A partir 1 car ya la locomotive.
                temp_wagon = Elt_train(self, False, "Wagon n°{}".format(i))
                Train.wagons_list[i-1].succ = temp_wagon
                temp_wagon.pred = Train.wagons_list[i-1]
                Train.wagons_list.append(temp_wagon)
                #temp_wagon.grid(column=i, row=0)
        # Placement des wagons ------------------------------------------------------------------------
        for i in range(0, len(self.wagons_list)):
            if(i==0):
                self.create_image((i*200)+100, 500, image=game.image_loco, anchor="nw")
            else:
                self.create_image((i*200)+100, 500, image=game.image_wagon, anchor="nw")
            #self.wagons_list[i].place(x=(i*150)+50, y=300)
        print("Voici la liste des wagons: \n")
        for i in self.wagons_list:
            print("|-----| {}".format(i.name))

class Elt_train():
    def __init__(self, game : Tk, locomotive, name):
        Train.nb_train_acc += 1
        self.pred = None
        self.succ = None
        self.name = name
        self.locomotive = locomotive
        self.wagon_butin_list = []
        self.locomotive_empty = False

class Bandit(Canvas):
    nb_joueurs_acc = 0
    def __init__(self, game: Tk, nom, position, est_interieur):
        super().__init__(game, width="35", height="65")
        self.nb_tir_eff = 0
        self.nb_braq_eff = 0
        self.nb_dep_eff = 0
        Bandit.nb_joueurs_acc+=1
        self.liste_butins = []
        self.nom = nom
        self.position = position #position = position dans la liste des wagons
        self.est_interieur = est_interieur #position_vertical = position toit/intérieur 1 -> Intérieur && 0 -> toit
        self.list_temp_action = []
        self.somme = 0
        self.nb_balles = 6
        #for i range(0, len(wagons_list)):
        #int_pos = random.randint(0, len(wagons_list))

        self.show_name = Text(self, height=1)
        self.show_name.insert(END, self.nom)
        self.show_name.config(font=('Helvetica bold', 6))
        self.show_name.place(x=0, y=0)
        self.show_name.config(state=DISABLED)

        self.create_image(7, 15, image=game.image_bandit, anchor="nw")
        #self.show_name.place(x=self.position*150, y=0)

    def afficher_butin_bandit(self):
        print("Butins de --> {}".format(self.nom))
        for i in self.liste_butins:
            print("Butin : {} \n".format(i))

    def somme_butin_bandit(self):
        for i in self.liste_butins:
            self.somme += i.butin_val
        return self.somme

    def actualiser_position(self):
        for i in range(0, len(Train.wagons_list)):
            for j in range(0, len(game.bandit_list)-1):
                if self.position == 0:
                    if self.position == game.bandit_list[j].position and self.nom != game.bandit_list[j].nom:
                        if self.est_interieur == game.bandit_list[j].est_interieur and self.nom != game.bandit_list[j].nom:
                            if self.est_interieur == 1:
                                self.place(x=140, y= 540)
                            if self.est_interieur == 0:
                                self.place(x=180, y= 450)
                    if self.est_interieur == 1:
                        self.place(x=(200), y=530)
                    if self.est_interieur == 0:
                        self.place(x=(200), y=450)
                if self.position == game.bandit_list[j].position and self.nom != game.bandit_list[j].nom:
                    if self.position == i and self.est_interieur == 0:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+40, 200))
                        self.place(x=((i*200)+220), y=450)
                    if self.position == i and self.est_interieur == 1:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+40, 350))
                        self.place(x=((i*200)+180), y=530)
                else:
                    if self.position == i and self.est_interieur == 0:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+40, 200))
                        self.place(x=(i*200)+120, y=450)
                    if self.position == i and self.est_interieur == 1:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+400, 350))
                        self.place(x=(i*200)+120, y=530)

class Marshall(Canvas):
    nb_joueurs_acc = 0
    def __init__(self, game : Tk, nom, position, est_interieur):
        super().__init__(game, width="35", height="65")
        self.nb_dep_eff = 0
        self.nom = nom
        self.position = position
        self.est_interieur = est_interieur
        self.list_temp_action = []
        self.nb_balles = 1
        self.create_image(7, 15, image=game.image_marshall, anchor="nw")
        self.show_name = Text(self, height=1)
        self.show_name.insert(END, self.nom)
        self.show_name.config(font=('Helvetica bold', 6))
        self.show_name.place(x=0, y=0)
        self.show_name.config(state=DISABLED)

    def check_around(self):
        for i in range(1, len(game.bandit_list)):
            if game.bandit_list[i].position == self.position:
                self.list_temp_action.append(Tir(self))
                print("MARSHALL|| Le marshall à trouvé {} !".format(game.bandit_list[i].nom))

    def actualiser_position(self):
        for i in range(0, len(Train.wagons_list)):
            for j in range(0, len(game.bandit_list)-1):
                if self.position == 0:
                    if self.position == game.bandit_list[j].position and self.nom != game.bandit_list[j].nom:
                        if self.est_interieur == game.bandit_list[j].est_interieur and self.nom != game.bandit_list[j].nom:
                            if self.est_interieur == 1:
                                self.place(x=220, y= 540)
                            if self.est_interieur == 0:
                                self.place(x=220, y= 450)
                    if self.est_interieur == 1:
                        self.place(x=(200), y=530)
                    if self.est_interieur == 0:
                        self.place(x=(200), y=450)
                if self.position == game.bandit_list[j].position and self.nom != game.bandit_list[j].nom:
                    if self.position == i and self.est_interieur == 0:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+40, 200))
                        self.place(x=((i*200)+180), y=450)
                    if self.position == i and self.est_interieur == 1:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+40, 350))
                        self.place(x=((i*200)+180), y=530)
                else:
                    if self.position == i and self.est_interieur == 0:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+40, 200))
                        self.place(x=(i*200)+120, y=450)
                    if self.position == i and self.est_interieur == 1:
                        #print("Position graphique de {} est x={} et y={}".format(self.nom, i*150+50+400, 350))
                        self.place(x=(i*200)+120, y=530)


class Deplacement():
    nb_dep = 0
    def __init__(self, direction, bandit_actuel):
        self.name = type(self).__name__
        Deplacement.nb_dep+=1
        print(bandit_actuel)
        self.bandit_actuel = bandit_actuel
        self.direction = direction

    def possible_dep(self):
        """
        # Algo qui check si la locomotive est pleine pour ne pas avoir plus de deux personnes dans la locomotive.
        for i in game.bandit_list:
            for j in game.bandit_list:
                if i.position == 0 and i.position == j.position and i.est_interieur == 1 and j.est_interieur == 1:
                    Train.wagons_list[0].locomotive_empty = True
        if Train.wagons_list[0].locomotive_empty == True and self.bandit_actuel.position == 1:
            if self.direction == game.left:
                print("La locomotive comprends déjà au maximum 2 joueurs !")
                self.bandit_actuel.locomotive_empty = True
                return False
        """
        # Fin de l'algo -----------------------------------------------------------------------------------------
        if self.bandit_actuel.position == len(Train.wagons_list)-1 and self.direction == game.right:
            print("DEPLACEMENT {}|| Déplacement vers l'arrière impossible\n".format(self.bandit_actuel.nom))
            return False
        if self.bandit_actuel.position == 0 and self.direction == game.left:
            print("DEPLACEMENT {}|| Déplacement vers l'avant impossible (vous etes dans la locomotive)\n".format(self.bandit_actuel.nom))
            return False
        if self.bandit_actuel.est_interieur == 1 and self.direction == game.down:
            print("DEPLACEMENT {}|| Vous etes déjà à l'intérieur\n".format(self.bandit_actuel.nom))
            return False
        if self.bandit_actuel.est_interieur == 0 and self.direction == game.up:
            print("DEPLACEMENT {}|| Vous etes déjà à l'exterieur\n".format(self.bandit_actuel.nom))
            return False
        else:
            return True

    def se_deplacer(self):
        if self.possible_dep() == True:
            self.bandit_actuel.nb_dep_eff+=1
            if self.direction == game.left:
                self.bandit_actuel.position = self.bandit_actuel.position - 1
                print("DEPLACEMENT {}|| {} se déplace vers la gauche.".format(self.bandit_actuel.nom, self.bandit_actuel.nom))
            if self.direction == game.right:
                self.bandit_actuel.position = self.bandit_actuel.position + 1
                print("DEPLACEMENT {}|| {} se déplace vers la droite.".format(self.bandit_actuel.nom, self.bandit_actuel.nom))
            if self.direction ==  game.up:
                self.bandit_actuel.est_interieur = 0
                print("DEPLACEMENT {}|| {} se déplace sur le toit".format(self.bandit_actuel.nom, self.bandit_actuel.nom))
            if self.direction ==  game.down:
                self.bandit_actuel.est_interieur = 1
                print("DEPLACEMENT {}|| {} se déplace vers l'intérieur du wagon.".format(self.bandit_actuel.nom, self.bandit_actuel.nom))
            print("le bandit {} se déplace !".format(self.bandit_actuel.nom))
            return True
        return False

class Braquage():
    nb_braquage = 0
    def __init__(self, bandit_braqueur, wagon_actuel):
        self.name = type(self).__name__
        Braquage.nb_braquage+=1
        self.bandit_braqueur = bandit_braqueur
        self.wagon_actuel = wagon_actuel
    def braquer(self):
        if self.bandit_braqueur.est_interieur == 1:
            len(self.wagon_actuel.wagon_butin_list)
            if len(self.wagon_actuel.wagon_butin_list) != 0:
                if len(self.wagon_actuel.wagon_butin_list) == 1:
                    pic_butin = random.randint(1, len(self.wagon_actuel.wagon_butin_list))
                else:
                    pic_butin = random.randint(0, len(self.wagon_actuel.wagon_butin_list)-1)
                self.bandit_braqueur.liste_butins.append(self.wagon_actuel.wagon_butin_list[pic_butin])
                del self.wagon_actuel.wagon_butin_list[pic_butin]
                self.bandit_braqueur.nb_braq_eff+=1
                print("BRAQUAGE {}|| {} à braqué le wagon n°{}".format(self.bandit_braqueur.nom, self.bandit_braqueur.nom, self.bandit_braqueur.position))
                self.bandit_braqueur.afficher_butin_bandit()
            else:
                print("BRAQUAGE {}||Il n'y avait rien dans le wagon".format(self.bandit_braqueur.nom))
        else:
            print("BRAQUAGE {}||Vous ne pouvez pas braquer le toit d'un wagon...".format(self.bandit_braqueur.nom))

class Tir():
    nb_tir = 0
    def __init__(self, bandit_tireur):
        self.name = type(self).__name__
        Tir.nb_tir+=1
        self.bandit_tireur = bandit_tireur
    def tirer(self):
        if self.bandit_tireur.nb_balles!=0:
            print("TIR {}|| Regarde autour de lui...".format(self.bandit_tireur.nom))
            for i in game.bandit_list:
                if i.nom == "Marshall":
                            pass
                elif self.bandit_tireur.position == i.position and self.bandit_tireur.nom != i.nom: #Le premier bandit qu'il trouve dans son wagon
                    print("TIR {}|| {} vient de tirer sur {} !".format(self.bandit_tireur.nom, self.bandit_tireur.nom, i.nom))
                    if len(i.liste_butins) != 0:
                        Train.wagons_list[self.bandit_tireur.position].wagon_butin_list.append(i.liste_butins[-1])
                        Train.wagons_list[self.bandit_tireur.position].wagon_butin_list.pop()#Il lui retire un butin en lui tirant dessus.
                        i.est_interieur = 0 
                        self.bandit_tireur.nb_tir_eff+=1
                        if self.bandit_tireur.nom != "Marshall":
                            self.bandit_tireur.nb_balles-=1
                            print("TIR {}|| Il lui reste {} balles.".format(self.bandit_tireur.nom, self.bandit_tireur.nb_balles))
                        break
                    else:
                        print("TIR {}|| N'a aucun butin sur lui".format(self.bandit_tireur.nom))
                else:
                    print("TIR {}|| N'a trouvé personne !".format(self.bandit_tireur.nom))
        else:
            print("TIR {}|| N'a plus de balles !".format(self.bandit_tireur.nom))

class Magot(Canvas):
    def __init__(self, game : Tk):
        super().__init__(game, width="10", height="10")
        self.butin_val = 1000   

class Bijou(Canvas):
    def __init__(self, game : Tk):
        super().__init__(game, width="10", height="10")
        self.butin_val = 500
        self.create_image(0, 0, image=game.gold, anchor="nw")

class Bourse(Canvas):
    def __init__(self, game : Tk):
        super().__init__(game, width="10", height="10")
        wich = random.randint(1, 2)
        if wich == 1:
            self.butin_val = 100
        else:
            self.butin_val = 200
        self.create_image(0, 0, image=game.gold, anchor="nw")

#isinstance -> booléen qui renvoit si le 1er paramètre (objet) est une instance du deuxième paramètre (classe)

game = Jeu(3, 3, 2, 3)
#nb_wagons, nb_joueurs, nb_actions, nb_tours
game.title('Colt_express')
game.mainloop() 