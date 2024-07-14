from threading import Thread
import socket
from tkinter import *
import tkinter as tk
import time
from time import strftime
from datetime import datetime

#------------------------------------------- Partie graphique (tkinter) -------------------------------------------------------
client_app = Tk()

client_app.title("Live-Chat Python v1.0 (Client)\n")
client_app.geometry("300x500")
client_app.resizable(False, False)

chat_box = Text(client_app, width=35, height=15)
chat_box.grid(row=5, column=0)

send_box = Entry(client_app, width=35)
send_box.grid(row=6, column=0)

quitter = "quitter"
button_quit = Button(client_app, text ="Quitter", width=10, height=2, command=lambda:[client_socket.send(quitter.encode('utf-8')), client_socket.close()], bg='#0052cc', fg='#ffffff')
button_quit.place(x=0, y=285)

# button_quit = Button(client_app, text ="Envoyer", width=10, height=2, command=lambda:send_command(), bg='#0052cc', fg='#ffffff')
# button_quit.place(x=100, y=285)

ip_to_connect = Entry(client_app)
ip_to_connect.insert(END, "127.0.0.1")
ip_to_connect.place(x=0, y=380)

ip_text = "|Adresse du serveur distant|"
session_information = Label(client_app, text=ip_text)
session_information.place(x=0, y=360)

port_to_connect = Entry(client_app)
port_to_connect.insert(END, "6390")
port_to_connect.place(x=0, y=420)

port_text = "|Port du serveur distant|"
session_information = Label(client_app, text=port_text)
session_information.place(x=0, y=400)

button_connect = Button(client_app, text="Se connecter", width=10, height=2, bg='#0052cc', fg='#ffffff', command=lambda:connect_to_server())
button_connect.place(x=0, y=445)

chat_box.insert(END, "|--- Live-Chat Python v1.0 (Client) ---|")
nickname = ""
nbr_mess = 0
# -------------------------------- Création du socket général, pour acceuillir le système de chat. ------------------------
Host = ip_to_connect.get() #Trouvable avec ifconfig.
Port = port_to_connect.get()
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_connection_text = "Connecté au serveur: " + Host + "\n"
pseudo_text = "Veuillez entrer votre pseudo et appuyer sur ENTREE.\n"
chat_box.insert(END, server_connection_text)
chat_box.insert(END, pseudo_text)
Host = ip_to_connect.get() #Trouvable avec ifconfig.
Port = port_to_connect.get()
client_socket.connect((Host, int(Port)))

def connect_to_server():
	global Host
	global Port
	Host = ip_to_connect.get() #Trouvable avec ifconfig.
	Port = port_to_connect.get()
	client_socket.connect((Host, int(Port)))

# --------------------------------------- Partie envoi de message ------------------------------------
def send_command(event):
	global nbr_mess
	global nickname
	server_request = send_box.get()
	send_box.delete(0, 'end')
	if(nbr_mess == 0):
		nickname = server_request
		new_chat = "Votre pseudo est: " + nickname+ "\n"
		chat_box.insert(END, new_chat)
		nbr_mess+=1
	elif(server_request):
		if(server_request == "quitter"):
			end = "Discussion terminée."
			client_socket.send(end.encode('utf-8'))
			#break
		server_request = nickname + " --> " + server_request
		now=datetime.now()
		temps = now.strftime("%H:%M:%S")
		server_request = "|" + temps + "|" + server_request + "\n"
		client_socket.send(server_request.encode('utf-8'))
		#print("Envoit de: {} réussi".format(server_request))
		nbr_mess+=1

client_app.bind("<Return>", send_command)

# --------------------------------------- Partie récéption -------------------------------------------------
def receive():
	while True:
		client_request = client_socket.recv(1000).decode('utf-8')
		if(client_request == "Discussion terminée."):
			chat_box.insert(END, client_request)
			client_socket.close()
			break
		chat_box.insert(END, client_request)

#Threading des fonctions send et receive pour qu'elles puissent s'excuter autrement que dans l'ordre classique d'execution.
#to_send = Thread(target=send_command)
to_receive = Thread(target=receive)
#to_send.start()
to_receive.start()

client_app.mainloop()

#to_send.join()
to_receive.join()
