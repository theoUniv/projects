from threading import Thread
import socket
import sys
import os
import time
from tkinter import *
import tkinter as tk
import threading

Host = "127.0.0.1"  # Trouvable avec ifconfig.
Port = 6390
client_list = []

# Création du socket général, pour acceuillir le système de chat.
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# On attribue au socket un hote et un port à ouvrir pour qu'il puisse recevoir des paquets client.
server_socket.bind((Host, Port))
server_socket.listen(2)

print("|--- Live-Chat Python v1.0 (Serveur) ---|\n")
print("Attente d'une connexion entrante sur le port:",Port, "...\n\n")
# Partie envoit de message.

def send_to_all(message):
	print(message)
	for client in client_list:
		client.send(message.encode('utf-8'))

def send_command():
	while True:
		server_request = input("")
		# /list -c pour afficher la liste des clients connectés au serveur.
		if (server_request == "/list -c"):
			print(client_list)
		# /list -i pour afficher l'ip du serveur.
		elif (server_request == "/list -i"):
			print(Host)
		# /list -i pour afficher le nombre de messages envoyés depuis le serveur.
		elif (server_request == "/list -n"):
			print(number_of_messages)
		elif (server_request == "/broadcast hello"):
			send_to_all("|-- SERVEUR --| Hey tout le monde !")
		else:
			print("|-- ERREUR:COMMANDE INCONNUE --|")

first_user, first_ip = server_socket.accept()
print("Une machine avec l'adresse IP: ",first_ip, " s'est connectée.\n")
second_user, second_ip = server_socket.accept()
print("Une machine avec l'adresse IP: ",second_ip, " s'est connectée.\n")

client_list.append(first_user)
client_list.append(second_user)

def re_send(fromUser):
	while(True):
		to_resend = fromUser.recv(1024).decode('utf-8')
		if(to_resend == "quitter"):
			send_to_all("Discussion terminée")
			first_user.close()
			second_user.close()
			break
		send_to_all(to_resend)
		if not to_resend: #Si on perd la connexion.
 			print("|-- DECONNEXION DU CLIENT --|")
 			break

thread_send_command = threading.Thread(target=send_command)

thread_resend = threading.Thread(target=re_send, args=(first_user,))
thread_resend2 = threading.Thread(target=re_send, args=(second_user,))

thread_resend.start()
thread_resend2.start()
thread_send_command.start()

thread_resend.join()
thread_resend2.join()
thread_send_command.join()


print("|-- SORTIE DU SERVEUR --|")

for user_i in client_list:
	user_i.close()