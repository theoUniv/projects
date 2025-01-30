package fr.uparis.lemaire.projet_s1_ai_lm

import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.background
import androidx.compose.foundation.gestures.detectTapGestures
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.itemsIndexed
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavGraph.Companion.findStartDestination
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.compose.rememberNavController
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Animal
import fr.uparis.lemaire.projet_s1_ai_lm.models.AnimalViewModel
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.generateColorScheme
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch



class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            Projet_S1_AI_LMTheme{
                MyApp( onClose = { finish() })
            }
        }
    }
}


@Composable
fun MyApp(onClose: () -> Unit) {
    val navController = rememberNavController()
    Scaffold(
        bottomBar = { BottomNavigationBar(navController) }
    ) { innerPadding ->
        NavHost(
            navController = navController,
            startDestination = "home",
            Modifier.padding(innerPadding)
        ) {
            composable("home") { VuePrincipale(onClose = { onClose }) }
            composable("settings") {  SettingsView() }

        }

    }
}


@Composable
fun VuePrincipale(onClose: () -> Unit, viewModel: AnimalViewModel = viewModel()) {
    val context = LocalContext.current
    val animals by viewModel.filteredAnimals.collectAsState(initial = emptyList())
    var searchText by remember { mutableStateOf("") }
    val selectedAnimals = remember { mutableStateListOf<Animal>() }

    Scaffold(
        topBar = {
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(horizontal = 16.dp, vertical = 8.dp),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                TextField(
                    value = searchText,
                    onValueChange = {
                        searchText = it
                        viewModel.updateSearchPrefix(it)
                    },
                    label = { Text("Recherche par préfixe") },
                    modifier = Modifier
                        .fillMaxWidth(0.8f)
                        .background(
                            MaterialTheme.colorScheme.surface,
                            shape = MaterialTheme.shapes.medium
                        )
                )
                FloatingActionButton(
                    onClick = {
                        val intent = Intent(context, AddAnimalActivity::class.java)
                        context.startActivity(intent)
                    },
                    containerColor = MaterialTheme.colorScheme.primary
                ) {
                    Text("+", color = MaterialTheme.colorScheme.onPrimary)
                }
            }
        },
        floatingActionButton = {
            if (selectedAnimals.isNotEmpty()) {
                FloatingActionButton(
                    onClick = {
                        CoroutineScope(Dispatchers.IO).launch {
                            selectedAnimals.forEach { viewModel.deleteAnimalbyId(it.id_Animal) }
                            selectedAnimals.clear()
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
        LazyColumn(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .padding(horizontal = 16.dp, vertical = 8.dp)
        ) {
            itemsIndexed(animals) { index, animal ->
                AnimalItem(
                    animal = animal,
                    isSelected = selectedAnimals.contains(animal),
                    onShortClick = {
                        if (selectedAnimals.isNotEmpty()) {
                            // Mode sélection multiple actif.
                            if (selectedAnimals.contains(animal)) {
                                selectedAnimals.remove(animal)
                            } else {
                                selectedAnimals.add(animal)
                            }
                        } else {
                            // Pas en mode sélection multiple, effectuer une autre action.
                            Toast.makeText(context, "Clic court : ${animal.name}", Toast.LENGTH_SHORT).show()
                            val intent = Intent(context, AnimalDetailActivity::class.java)
                            intent.putExtra("animal", animal)
                            context.startActivity(intent)
                        }
                    },
                    onLongClick = {
                        if (!selectedAnimals.contains(animal)) {
                            selectedAnimals.add(animal)
                        }
                        Toast.makeText(context, "Mode sélection multiple activé", Toast.LENGTH_SHORT).show()
                    }
                )
            }
        }

        if (selectedAnimals.isNotEmpty()) {
            Toast.makeText(
                context,
                "Animaux sélectionnés : ${selectedAnimals.joinToString { it.name }}",
                Toast.LENGTH_SHORT
            ).show()
        }
    }
}

object GlobalTheme{
    var colorScheme: ColorScheme by mutableStateOf(lightColorScheme())
}

@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SettingsView() {
    var color by remember { mutableStateOf(Color.White) }

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("Paramètres") }
            )
        }
    ) {
        val scrollState = rememberScrollState()

        Column(
            modifier = Modifier
                .padding(16.dp)
                .fillMaxSize()
                .verticalScroll(scrollState)
        ) {
            RGBColorPicker(onColorSelected = { color = it })

        }
    }
}



@Composable
fun RGBColorPicker(onColorSelected: (Color) -> Unit) {
    var red by remember { mutableStateOf(255f) }
    var green by remember { mutableStateOf(255f) }
    var blue by remember { mutableStateOf(255f) }

    val context = LocalContext.current


    Column(
        modifier = Modifier.padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        Spacer(modifier = Modifier.height(48.dp))

        Slider(
            value = red,
            onValueChange = { red = it },
            valueRange = 0f..255f,
            colors = SliderDefaults.colors(thumbColor = Color.Red)
        )
        Slider(
            value = green,
            onValueChange = { green = it },
            valueRange = 0f..255f,
            colors = SliderDefaults.colors(thumbColor = Color.Green)
        )
        Slider(
            value = blue,
            onValueChange = { blue = it },
            valueRange = 0f..255f,
            colors = SliderDefaults.colors(thumbColor = Color.Blue)
        )

        Box(
            modifier = Modifier
                .fillMaxWidth()
                .height(50.dp)
                .background(Color(red.toInt(), green.toInt(), blue.toInt()))
        )

        Button(onClick = {
            onColorSelected(Color(red.toInt(), green.toInt(), blue.toInt()))
            GlobalTheme.colorScheme = generateColorScheme(Color(red.toInt(), green.toInt(), blue.toInt()))
        }) {
            Text("Sélectionner cette couleur")
        }


        Button(onClick = {
            val intent = Intent(context, EditDefaultActivityActivity::class.java)
            context.startActivity(intent)
        }) { Text("Changer les activitées par défaut") }
    }
}



data class NavigationItem(
    val title: String,
    val icon: androidx.compose.ui.graphics.vector.ImageVector,
    val route: String
)

@Composable
fun BottomNavigationBar(navController: NavHostController) {
    val items = listOf(
        NavigationItem("Accueil", Icons.Default.Home, "home"),
        NavigationItem("Paramètres", Icons.Default.Settings, "settings")
    )

    NavigationBar {
        val currentBackStackEntry by navController.currentBackStackEntryAsState()
        val currentDestination = currentBackStackEntry?.destination?.route

        items.forEach { item ->
            NavigationBarItem(
                selected = currentDestination == item.route,
                onClick = {
                    navController.navigate(item.route) {
                        popUpTo(navController.graph.findStartDestination().id) { saveState = true }
                        launchSingleTop = true
                        restoreState = true
                    }
                },
                icon = { Icon(item.icon, contentDescription = item.title) },
                label = { Text(item.title) }
            )
        }
    }
}

@Composable
fun AnimalItem(
    animal: Animal,
    isSelected: Boolean,
    onShortClick: () -> Unit,
    onLongClick: () -> Unit
) {
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
            containerColor = if (isSelected) GlobalTheme.colorScheme.primaryContainer
            else GlobalTheme.colorScheme.surfaceVariant
        )
    ) {
        Row(
            modifier = Modifier
                .padding(16.dp)
                .fillMaxWidth(),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Column {
                Text(
                    text = "Nom : ${animal.name}",
                    style = MaterialTheme.typography.titleMedium,
                    color = if (isSelected) GlobalTheme.colorScheme.onPrimaryContainer
                    else GlobalTheme.colorScheme.onSurfaceVariant
                )
                Text(
                    text = "Espèce : ${animal.type}",
                    style = MaterialTheme.typography.bodyMedium,
                    color = if (isSelected) GlobalTheme.colorScheme.onPrimaryContainer
                    else GlobalTheme.colorScheme.onSurface
                )
            }
        }
    }
}



