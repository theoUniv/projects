package fr.uparis.lemaire.projet_s1_ai_lm

import ActiviteViewModel
import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ExposedDropdownMenuBox
import androidx.compose.material3.ExposedDropdownMenuDefaults
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewModelScope
import androidx.lifecycle.viewmodel.compose.viewModel
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Activite
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Regularite
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Espece
import fr.uparis.lemaire.projet_s1_ai_lm.models.AnimalViewModel
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme
import kotlinx.coroutines.launch
import org.xmlpull.v1.XmlPullParser
import org.xmlpull.v1.XmlPullParserFactory
import java.io.FileInputStream

class AddAnimalActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            Projet_S1_AI_LMTheme {
                VueAjout(onClose = { finish() })
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun VueAjout(onClose: () -> Unit,
             viewModel: AnimalViewModel = viewModel(),
             viewModelActivite : ActiviteViewModel = viewModel()) {
    var nom by remember { mutableStateOf("") }
    var expanded by remember { mutableStateOf(false) }
    val items = Espece.values().map { it.name }
    var selectedItem by remember { mutableStateOf(items[0]) }
    var context = LocalContext.current



    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(8.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        TextField(
            value = nom,
            onValueChange = { nom = it },
            label = { Text("Nom de l'animal") },
            modifier = Modifier.fillMaxWidth()
        )

        Spacer(modifier = Modifier.height(16.dp))

        ExposedDropdownMenuBox(
            expanded = expanded,
            onExpandedChange = { expanded = !expanded }
        ) {
            TextField(
                value = selectedItem,
                onValueChange = {},
                readOnly = true,
                label = { Text("Sélectionnez une option") },
                trailingIcon = {
                    ExposedDropdownMenuDefaults.TrailingIcon(expanded = expanded)
                },
                modifier = Modifier
                    .menuAnchor()
                    .fillMaxWidth()
            )

            // Menu déroulant contenant les options.
            ExposedDropdownMenu(
                expanded = expanded,
                onDismissRequest = { expanded = false }
            ) {
                items.forEach { item ->
                    DropdownMenuItem(
                        text = { Text(item) },
                        onClick = {
                            selectedItem = item
                            expanded = false
                        }
                    )
                }
            }
        }

        Spacer(modifier = Modifier.height(16.dp))

        Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            Button(onClick = {
                if (nom.isBlank()) {
                    println("Erreur, champ vide !")
                } else {

                    viewModel.viewModelScope.launch {
                        val animalID = viewModel.ajouterAnimal(nom = nom.trim(), type = Espece.valueOf(selectedItem))

                        val defaultActivities = loadDefaultActivitiesFromFile(context, selectedItem, animalID)
                        defaultActivities.forEach {
                            viewModelActivite.addActivite(it)
                        }
                    }

                    onClose()

                    nom = ""
                }
            }) {
                Text("Ajouter un Animal")
            }
            Button(onClick = onClose) {
                Text("Retour")
            }
        }
    }
}
fun loadDefaultActivitiesFromFile(context: Context, type: String, Id_animal: Int): List<Activite> {
    val activities = mutableListOf<Activite>()
    try {
        val fileInputStream: FileInputStream = context.openFileInput("default_activities.xml")
        val parser = XmlPullParserFactory.newInstance().newPullParser()
        parser.setInput(fileInputStream, null)

        var currentAnimal = ""
        var currentName = ""
        var currentHoraire = ""
        var currentRegularite = ""

        while (parser.eventType != XmlPullParser.END_DOCUMENT) {
            when (parser.eventType) {
                XmlPullParser.START_TAG -> {
                    when (parser.name) {
                        "array" -> {
                            currentAnimal = parser.getAttributeValue(null, "name")
                            Log.d("LoadDefaultActivities", "Début de section pour l'animal : $currentAnimal")
                        }
                        "name" -> {
                            currentName = parser.nextText()
                            Log.d("LoadDefaultActivities", "Nom de l'activité trouvé : $currentName")
                        }
                        "horaire" -> {
                            currentHoraire = parser.nextText()
                            Log.d("LoadDefaultActivities", "Horaire trouvé : $currentHoraire")
                        }
                        "regularite" -> {
                            currentRegularite = parser.nextText()
                            Log.d("LoadDefaultActivities", "Régularité trouvée : $currentRegularite")
                        }
                    }
                }
                XmlPullParser.END_TAG -> {
                    if (parser.name == "item" && currentAnimal.uppercase() == type.uppercase()) {
                        activities.add(
                            Activite(
                                id_animal = Id_animal,
                                nom_Activite = currentName,
                                Horaire = currentHoraire,
                                regularite = Regularite.valueOf(currentRegularite)
                            )
                        )
                        currentName = ""
                        currentHoraire = ""
                        currentRegularite = ""
                    }
                }
            }
            parser.next()
        }
        fileInputStream.close()
    } catch (e: Exception) {
        Log.e("LoadDefaultActivities", "Erreur lors du chargement des activités depuis le fichier", e)
    }
    return activities
}
