package fr.uparis.lemaire.projet_s1_ai_lm

import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ExposedDropdownMenuBox
import androidx.compose.material3.ExposedDropdownMenuDefaults
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Animal
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Espece
import fr.uparis.lemaire.projet_s1_ai_lm.models.AnimalViewModel
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme

class EditAnimalActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {

            Projet_S1_AI_LMTheme{
                val animalId = intent.getIntExtra("animal_id", -1)

                EditAnimalVue(
                    viewModel = AnimalViewModel(application = application),
                    onAnimalEdited = {},
                    animal = animalId,
                    onClose = { finish() }
                )
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun EditAnimalVue(
    viewModel: AnimalViewModel = viewModel(),
    onAnimalEdited: () -> Unit,
    animal: Int,
    onClose: () -> Unit
) {
    var expanded by remember { mutableStateOf(false) }

    val animal by viewModel.getAnimalById(animal).collectAsState(
        initial = Animal(name = "", type = Espece.CHIEN)
    )
    var name by remember(animal) { mutableStateOf(animal.name) }
    var type by remember(animal) { mutableStateOf(animal.type) }

    val items = Espece.values().map { it.name }

    val context = LocalContext.current
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        TextField(
            value = name,
            onValueChange = { name = it },
            label = { Text("Nom de l'animal") },
            modifier = Modifier.fillMaxWidth()
        )

        // DropdownMenuBox pour sélectionner l'espèce.
        ExposedDropdownMenuBox(
            expanded = expanded,
            onExpandedChange = { expanded = !expanded }
        ) {
            TextField(
                value = type.toString(),
                onValueChange = {},
                readOnly = true,
                label = { Text("Sélectionnez une espèce") },
                trailingIcon = {
                    ExposedDropdownMenuDefaults.TrailingIcon(expanded = expanded)
                },
                modifier = Modifier
                    .menuAnchor()
                    .fillMaxWidth()
            )

            ExposedDropdownMenu(
                expanded = expanded,
                onDismissRequest = { expanded = false }
            ) {
                items.forEach { item ->
                    DropdownMenuItem(
                        text = { Text(item) },
                        onClick = {
                            type = Espece.valueOf(item)
                            expanded = false
                        }
                    )
                }
            }
        }

        Row {
            Button(onClick = onClose) {
                Text("Retour")
            }

            Spacer(modifier = Modifier.weight(1f))

            Button(onClick = {
                viewModel.updateAnimal(animal.copy(name = name, type = type))
                onAnimalEdited()
                Toast.makeText(context, "Animal modifié avec succès", Toast.LENGTH_SHORT).show()
                onClose()
            }) {
                Text("Valider")
            }
        }
    }
}
