package fr.uparis.lemaire.projet_s1_ai_lm

import ActiviteViewModel
import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.Image
import androidx.compose.foundation.gestures.detectTapGestures
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.FabPosition
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Activite
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Animal
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.platform.LocalContext
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Espece
import fr.uparis.lemaire.projet_s1_ai_lm.models.AnimalViewModel
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch


class AnimalDetailActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            Projet_S1_AI_LMTheme {
                val animal = intent.getSerializableExtra("animal") as Animal
                var AnimalImageRessourceId = 0

                val viewModelAnimal: AnimalViewModel = viewModel()
                var animal_id = animal.id_Animal


                var animal_bdd = viewModelAnimal.getAnimalById(animal_id).collectAsState(initial = null)


                if (animal != null) {

                    when (animal_bdd.value?.type) {
                        Espece.POISSON -> AnimalImageRessourceId = R.drawable.animal_image_1
                        Espece.CHAT -> AnimalImageRessourceId = R.drawable.animal_image_2
                        Espece.CHIEN -> AnimalImageRessourceId = R.drawable.animal_image_3
                        Espece.HAMSTER -> AnimalImageRessourceId = R.drawable.animal_image_4
                        null -> println("Erreur")
                    }


                    VueDetailAnimal(
                        animalID = animal.id_Animal,


                        imageResId = AnimalImageRessourceId,
                        onAddActivityClicked = {
                            val intent = Intent(this, AddActivityActivity::class.java)
                            intent.putExtra("animal_id", animal.id_Animal)
                            startActivity(intent)
                        },
                        onEditAnimalClicked = {
                            val intent = Intent(this, EditAnimalActivity::class.java)
                            intent.putExtra("animal_id", animal.id_Animal)
                            startActivity(intent)
                        }
                    )
                }
            }
        }
    }
}




@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun VueDetailAnimal(
    animalID: Int,
    imageResId: Int,
    viewModel: ActiviteViewModel = viewModel(),
    viewModelAnimal: AnimalViewModel = viewModel(),
    onAddActivityClicked: () -> Unit,
    onEditAnimalClicked: () -> Unit
) {
    // On observe l'animal depuis la base de données.
    val animal by viewModelAnimal.getAnimalById(animalID).collectAsState(initial = null)

    // Si l'animal est null, On affiche le chargement.
    if (animal == null) {
        Text("Chargement des détails de l'animal...", modifier = Modifier.fillMaxSize())
        return
    }
    viewModel.updateSearchID(animalID)
    val activities by viewModel.filteredActivities.collectAsState(initial = emptyList())
    val selectedActivites = remember { mutableStateListOf<Activite>() }

    val context = LocalContext.current
    Scaffold(
        topBar = {
            TopAppBar(
                title = {
                    Text("${animal!!.name}", style = MaterialTheme.typography.titleLarge)
                },
                navigationIcon = {
                    // Bouton de retour avec une icône Material Design.
                    IconButton(onClick = {

                        val intent = Intent(context, MainActivity::class.java)
                        context.startActivity(intent)
                    }) {
                        Icon(
                            imageVector = Icons.Default.ArrowBack,
                            contentDescription = "Retour",
                            tint = MaterialTheme.colorScheme.onPrimary
                        )
                    }
                },
                colors = TopAppBarDefaults.topAppBarColors(
                    containerColor = MaterialTheme.colorScheme.primary,
                    titleContentColor = MaterialTheme.colorScheme.onPrimary
                )
            )
        },
        floatingActionButton = {
            if (selectedActivites.isNotEmpty()) {
                FloatingActionButton(
                    onClick = {
                        // Supprime les animaux sélectionnés dans un thread séparé.
                        CoroutineScope(Dispatchers.IO).launch {
                            selectedActivites.forEach { viewModel.deleteActivite(it.id_Activite) }
                            selectedActivites.clear()
                        }
                    },
                    containerColor = MaterialTheme.colorScheme.error
                ) {
                    Icon(
                        imageVector = Icons.Default.Delete,
                        contentDescription = "Supprimer",
                        tint = MaterialTheme.colorScheme.onError
                    )
                }
            }
        },
        floatingActionButtonPosition = FabPosition.End
    ) { innerPadding ->
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .padding(16.dp)
        ) {
            Column {
                // Contenu des détails de l'animal.
                AnimalDetailContent(animal = animal!!, imageResId = imageResId)

                Spacer(modifier = Modifier.height(8.dp))

                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Button(
                        onClick = onAddActivityClicked,
                        modifier = Modifier.padding(end = 8.dp)
                    ) {
                        Text("Ajouter une activité")
                    }
                    Button(onClick = onEditAnimalClicked) {
                        Text("Éditer l'animal")
                    }
                }

                Spacer(modifier = Modifier.height(16.dp))

                Text(
                    text = "Activités associées",
                    style = MaterialTheme.typography.headlineSmall,
                    color = MaterialTheme.colorScheme.primary
                )

                Spacer(modifier = Modifier.height(8.dp))


                LazyColumn(
                    modifier = Modifier
                        .fillMaxWidth()
                        .weight(1f)
                ) {
                    items(activities) { activity ->
                        ActiviteItem(activity = activity,
                            isSelected = selectedActivites.contains(activity),
                            onShortClick = {
                                if (selectedActivites.isNotEmpty()) {
                                    // Mode sélection multiple actif.
                                    if (selectedActivites.contains(activity)) {
                                        selectedActivites.remove(activity)
                                    } else {
                                        selectedActivites.add(activity)
                                    }
                                } else {
                                    val intent = Intent(context, EditActivityActivity::class.java)
                                    intent.putExtra("activity_id", activity.id_Activite)
                                    context.startActivity(intent)
                                }
                            },
                            onLongClick = {
                                if (!selectedActivites.contains(activity)) {
                                    selectedActivites.add(activity)
                                }
                                Toast.makeText(context, "Mode sélection multiple activé", Toast.LENGTH_SHORT).show()
                            }
                            )
                        }
                    }
                }
            }
        }
    }



@Composable
fun ActiviteItem(activity: Activite,
                 onShortClick: () -> Unit,
                 onLongClick: () -> Unit,
                 isSelected: Boolean) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 8.dp)
            .pointerInput(Unit) {
                detectTapGestures(
                    onTap = { onShortClick() },
                    onLongPress = { onLongClick() }
                )
            },
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp),
        colors = CardDefaults.cardColors(
            containerColor = if (isSelected) MaterialTheme.colorScheme.primaryContainer
            else MaterialTheme.colorScheme.surfaceVariant
        ),
    ) {
        Column(
            modifier = Modifier.padding(16.dp)
        ) {
            Text(
                text = "${activity.nom_Activite}",
                style = MaterialTheme.typography.titleMedium,
                color = MaterialTheme.colorScheme.onSurface
            )
            Text(
                text = "${activity.Horaire}",
                style = MaterialTheme.typography.bodyLarge,
                color = MaterialTheme.colorScheme.onSurface
            )
        }
    }
}



@Composable
fun AnimalDetailContent(animal: Animal, imageResId: Int) {
    Column(
        modifier = Modifier.fillMaxWidth(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Image(
            painter = painterResource(id = imageResId),
            contentDescription = "Image de l'animal",
            modifier = Modifier
                .size(128.dp)
                .padding(16.dp)
                .align(Alignment.CenterHorizontally),
        )

        Text(
            text = "Espèce : ${animal.type}",
            style = MaterialTheme.typography.bodyLarge,
            fontSize = 18.sp,
            fontStyle = MaterialTheme.typography.bodyLarge.fontStyle,
            textAlign = TextAlign.Center,
            color = MaterialTheme.colorScheme.onSurfaceVariant
        )
    }
}