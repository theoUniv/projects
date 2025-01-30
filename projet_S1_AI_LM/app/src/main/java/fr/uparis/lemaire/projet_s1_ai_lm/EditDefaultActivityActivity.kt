package fr.uparis.lemaire.projet_s1_ai_lm

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Activite
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Regularite
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Espece
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme
import org.xmlpull.v1.XmlPullParser
import org.xmlpull.v1.XmlPullParserFactory
import org.xmlpull.v1.XmlSerializer
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.InputStreamReader
import java.io.OutputStreamWriter

class EditDefaultActivityActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            Projet_S1_AI_LMTheme {
                SettingsActivity()
            }
        }
    }

    private fun saveActivitiesToXml(context: Context, activities: List<Activite>, animalType: Espece) {
        try {
            val fileOutputStream: FileOutputStream =
                context.openFileOutput("default_activities.xml", Context.MODE_PRIVATE)
            val outputStreamWriter = OutputStreamWriter(fileOutputStream)

            val serializer: XmlSerializer = android.util.Xml.newSerializer()
            serializer.setOutput(outputStreamWriter)

            serializer.startDocument("UTF-8", true)
            serializer.startTag("", "resources")

            for (animal in Espece.values()) {
                val filteredActivities = activities.filter { it.id_animal == animal.ordinal }

                if (filteredActivities.isNotEmpty()) {
                    serializer.startTag("", "array")
                    serializer.attribute("", "name", animal.name)

                    for (activity in filteredActivities) {
                        serializer.startTag("", "item")
                        serializer.startTag("", "activity")
                        serializer.startTag("", "name")
                        serializer.text(activity.nom_Activite)
                        serializer.endTag("", "name")
                        serializer.startTag("", "horaire")
                        serializer.text(activity.Horaire)
                        serializer.endTag("", "horaire")
                        serializer.startTag("", "regularite")
                        serializer.text(activity.regularite.name)
                        serializer.endTag("", "regularite")
                        serializer.endTag("", "activity")
                        serializer.endTag("", "item")
                    }
                    serializer.endTag("", "array")
                }
            }

            serializer.endTag("", "resources")
            serializer.endDocument()

            outputStreamWriter.flush()
            fileOutputStream.close()

        } catch (e: Exception) {
            e.printStackTrace()
            Log.e("SaveActivities", "Error saving activities to XML", e)
        }
    }

    private fun loadActivitiesFromXml(context: Context): List<Activite> {
        val activities = mutableListOf<Activite>()
        try {
            val fileInputStream: FileInputStream = context.openFileInput("default_activities.xml")
            val inputStreamReader = InputStreamReader(fileInputStream)

            val parser: XmlPullParser = XmlPullParserFactory.newInstance().newPullParser()
            parser.setInput(inputStreamReader)

            var eventType = parser.eventType
            var currentActivity: Activite? = null
            var currentRegularite: Regularite? = null
            var currentHoraire: String? = null
            var currentNom: String? = null
            var currentTypeAnimal: Espece? = null

            while (eventType != XmlPullParser.END_DOCUMENT) {
                when (eventType) {
                    XmlPullParser.START_TAG -> {
                        when (parser.name) {
                            "array" -> {
                                currentTypeAnimal =
                                    Espece.valueOf(parser.getAttributeValue(null, "name"))
                            }

                            "name" -> {
                                currentNom = parser.nextText()
                            }

                            "horaire" -> {
                                currentHoraire = parser.nextText()
                            }

                            "regularite" -> {
                                currentRegularite = Regularite.valueOf(parser.nextText())
                            }
                        }
                    }

                    XmlPullParser.END_TAG -> {
                        if (parser.name == "item") {
                            currentActivity = Activite(
                                nom_Activite = currentNom ?: "",
                                Horaire = currentHoraire ?: "",
                                regularite = currentRegularite ?: Regularite.UNIQUE,
                                id_animal = currentTypeAnimal?.ordinal ?: 0
                            )

                            currentActivity?.let {
                                activities.add(it)
                            }
                        }
                    }
                }
                eventType = parser.next()
            }

            inputStreamReader.close()

        } catch (e: Exception) {
            e.printStackTrace()
            Log.e("LoadActivities", "Error loading activities from XML", e)
        }
        return activities
    }

    @OptIn(ExperimentalMaterial3Api::class)
    @Composable
    fun SettingsActivity() {
        val activities = remember { mutableStateListOf<Activite>() }

        LaunchedEffect(Unit) {
            activities.clear()
            activities.addAll(loadActivitiesFromXml(applicationContext))
            Log.d("Activités", activities.toString())
        }

        var isDialogOpen by remember { mutableStateOf(false) }

        Scaffold(
            topBar = {
                TopAppBar(
                    title = { Text("Activités par défaut", color = Color.White) },
                    navigationIcon = {
                        IconButton(onClick = { finish() }) {
                            Icon(
                                imageVector = Icons.Default.ArrowBack,
                                contentDescription = "Retour"
                            )
                        }
                    },
                    actions = {
                        IconButton(onClick = { isDialogOpen = true }) {
                            Icon(
                                imageVector = Icons.Default.Add,
                                contentDescription = "Ajouter Activité",
                                tint = Color.White
                            )
                        }
                    },
                    colors = TopAppBarDefaults.smallTopAppBarColors(
                        containerColor = MaterialTheme.colorScheme.primary
                    )
                )
            }
        ) { innerPadding ->
            Box(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(innerPadding)
            ) {
                Column(
                    modifier = Modifier
                        .padding(16.dp)
                        .verticalScroll(rememberScrollState())
                        .fillMaxSize()
                ) {
                    ActivityList(
                        activities = activities,
                        onDelete = { activityToDelete ->
                            activities.remove(activityToDelete)
                            saveActivitiesToXml(applicationContext, activities, Espece.CHIEN)
                        }
                    )
                }

                if (isDialogOpen) {
                    AddActivityDialog(
                        onActivityAdded = { newActivity ->
                            activities.add(newActivity)
                            saveActivitiesToXml(applicationContext, activities, Espece.CHIEN)
                            isDialogOpen = false
                        },
                        onDismiss = { isDialogOpen = false }
                    )
                }
            }
        }
    }


    @Composable
    fun ActivityList(
        activities: MutableList<Activite>,
        onDelete: (Activite) -> Unit
    ) {
        Column(modifier = Modifier.fillMaxWidth()) {
            if (activities.isEmpty()) {
                Text(
                    text = "Aucune activité disponible",
                    style = MaterialTheme.typography.bodyMedium,
                    modifier = Modifier.fillMaxWidth(),
                    textAlign = TextAlign.Center
                )
            } else {
                activities.forEach { activity ->
                    ActivityItem(
                        activity = activity,
                        onDelete = onDelete
                    )
                    Spacer(modifier = Modifier.height(16.dp))
                }
            }
        }
    }
}

@Composable
fun ActivityItem(
    activity: Activite,
    onDelete: (Activite) -> Unit
) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 8.dp),
        shape = RoundedCornerShape(12.dp),
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp)
    ) {
        Column(modifier = Modifier.padding(20.dp)) {
            Text(
                text = activity.nom_Activite,
                style = MaterialTheme.typography.bodyMedium,
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(4.dp))
            Text(
                text = "Horaire: ${activity.Horaire}",
                style = MaterialTheme.typography.bodyMedium
            )
            Text(
                text = "Fréquence: ${activity.regularite.name}",
                style = MaterialTheme.typography.bodyMedium
            )
            Text(
                text = "Animal: ${Espece.values()[activity.id_animal].name}",
                style = MaterialTheme.typography.bodyMedium
            )
            Spacer(modifier = Modifier.height(8.dp))
            Button(
                onClick = { onDelete(activity) },
                colors = ButtonDefaults.buttonColors(MaterialTheme.colorScheme.error),
                modifier = Modifier.align(Alignment.End)
            ) {
                Text("Supprimer", color = Color.White)
            }
        }
    }
}


@Composable
fun AddActivityDialog(
    onActivityAdded: (Activite) -> Unit,
    onDismiss: () -> Unit,
    initialActivity: Activite? = null
) {
    var activityName by remember { mutableStateOf(initialActivity?.nom_Activite ?: "") }
    var activityTime by remember { mutableStateOf(initialActivity?.Horaire ?: "") }
    var activityRegularity by remember {
        mutableStateOf(
            initialActivity?.regularite ?: Regularite.HEBDOMADAIRE
        )
    }
    var selectedAnimal by remember {
        mutableStateOf(initialActivity?.id_animal?.let { Espece.values()[it] } ?: Espece.CHIEN)
    }
    var context = LocalContext.current
    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text("Ajouter une activité") },
        text = {
            Column(modifier = Modifier.padding(16.dp)) {
                OutlinedTextField(
                    value = activityName,
                    onValueChange = { activityName = it },
                    label = { Text("Nom de l'activité") },
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(bottom = 8.dp)
                )
                OutlinedTextField(
                    value = activityTime,
                    onValueChange = { activityTime = it },
                    label = { Text("Horaire") },
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(bottom = 8.dp)
                )
                Text("Fréquence", style = MaterialTheme.typography.bodyLarge)
                Column(modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 8.dp)) {
                    Regularite.values().forEach { regularite ->
                        Row(
                            modifier = Modifier.fillMaxWidth(),
                            verticalAlignment = Alignment.CenterVertically
                        ) {
                            RadioButton(
                                selected = activityRegularity == regularite,
                                onClick = { activityRegularity = regularite },
                                modifier = Modifier.padding(end = 8.dp)
                            )
                            Text(regularite.name)
                        }
                    }
                }

                Text("Type d'animal", style = MaterialTheme.typography.bodyLarge)
                Column(modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 8.dp)) {
                    Espece.values().forEach { animalType ->
                        Row(
                            modifier = Modifier.fillMaxWidth(),
                            verticalAlignment = Alignment.CenterVertically
                        ) {
                            RadioButton(
                                selected = selectedAnimal == animalType,
                                onClick = { selectedAnimal = animalType },
                                modifier = Modifier.padding(end = 8.dp)
                            )
                            Text(animalType.name)
                        }
                    }
                }
            }
        },
        confirmButton = {
            Button(
                onClick = {
                    val newActivity = Activite(
                        nom_Activite = activityName,
                        Horaire = activityTime,
                        regularite = activityRegularity,
                        id_animal = selectedAnimal.ordinal
                    )
                    if(!activityTime.isEmpty() && !activityName.isEmpty() && activityRegularity.name.isNotEmpty()){
                        onActivityAdded(newActivity)
                    } else {
                        Toast.makeText(context, "Remplissez tout les champs...", Toast.LENGTH_SHORT).show()
                    }
                }
            ) {
                Text("Ajouter Activité")
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text("Annuler")
            }
        }
    )
}
