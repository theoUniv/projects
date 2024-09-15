package com.example.titask

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.ListView
import android.widget.Toast

class MainActivity : AppCompatActivity() {
    private val itemList = ArrayList<String>()
    // Créez un adaptateur pour votre ListView
    private lateinit var adapter: ArrayAdapter<String>
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, itemList)
        val listView: ListView = findViewById(R.id.task_list)
        listView.adapter = adapter
        addItemToList("Tache N°1")
        addItemToList("Tache N°2")
        addItemToList("Tache N°3")
        addItemToList("Tache N°4")
        addItemToList("Tache N°6")
        addItemToList("Tache N°7")
        addItemToList("Tache N°8")
        addItemToList("Tache N°9")
        addItemToList("Tache N°10")
        addItemToList("Tache N°11")
        addItemToList("Tache N°12")

        listView.setOnItemClickListener { _, _, position, _ -> onItemClick(position) }


    }

    // Méthode pour ajouter un élément à la liste et mettre à jour l'adaptateur
    private fun addItemToList(item: String) {
        itemList.add(item)
        adapter.notifyDataSetChanged()
    }

    // Exemple de méthode pour gérer le clic sur un élément de la liste
    private fun onItemClick(position: Int) {
        // Gérez l'action lorsqu'un élément est cliqué
        // Par exemple, affichez un message avec l'élément sélectionné
        val selectedItem = itemList[position]
        Toast.makeText(this, "Selection: $selectedItem", Toast.LENGTH_SHORT).show()
    }


}



