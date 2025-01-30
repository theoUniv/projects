package fr.uparis.lemaire.projet_s1_ai_lm

import ActiviteViewModel
import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Activite
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Regularite
import android.app.TimePickerDialog
import android.icu.util.Calendar
import android.util.Log
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Edit
import fr.uparis.lemaire.projet_s1_ai_lm.notifications.NotificationPermissionHandler
import fr.uparis.lemaire.projet_s1_ai_lm.notifications.NotificationScheduler
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme

class AddActivityActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        createNotificationChannel(this)

        setContent {
            Projet_S1_AI_LMTheme {
                val animalId = intent.getIntExtra("animal_id", -1)
                Log.d("ID de l'animal auquel on ajoute une activité: " , animalId.toString())
                val viewModel: ActiviteViewModel = viewModel()

                if (animalId != -1) {
                    AddActivityScreen(
                        animalId = animalId,
                        onActivityAdded = { activity ->
                            viewModel.addActivite(activity)

                            Toast.makeText(
                                this,
                                "Activité ajoutée : ${activity.nom_Activite}",
                                Toast.LENGTH_SHORT
                            ).show()


                            val resultIntent = Intent()
                            resultIntent.putExtra("refresh", true)
                            setResult(RESULT_OK, resultIntent)
                            finish()
                        },
                        onClose = { finish() }
                    )
                }
            }
        }
    }
}

private fun createNotificationChannel(context: Context) {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
        val name = "Activité Notifications"
        val descriptionText = "Notifications pour les nouvelles activités"
        val importance = NotificationManager.IMPORTANCE_DEFAULT
        val channel = NotificationChannel("activity_channel", name, importance).apply {
            description = descriptionText
        }
        val notificationManager = context.getSystemService(Context.NOTIFICATION_SERVICE) as? NotificationManager
        notificationManager?.createNotificationChannel(channel)
    }
}


@Composable
fun AddActivityScreen(
    animalId: Int,
    onActivityAdded: (Activite) -> Unit,
    viewModel: ActiviteViewModel = viewModel(),
    onClose: () -> Unit
) {
    val calendar = Calendar.getInstance()
    val currentHour = calendar.get(Calendar.HOUR_OF_DAY)
    val currentMinute = calendar.get(Calendar.MINUTE)
    var activityName by remember { mutableStateOf("") }
    var activityTime by remember { mutableStateOf("") }
    var regularite by remember { mutableStateOf(Regularite.QUOTIDIEN) }
    var isDropdownExpanded by remember { mutableStateOf(false) }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        TextField(
            value = activityName,
            onValueChange = { activityName = it },
            label = { Text("Nom de l'activité") },
            modifier = Modifier.fillMaxWidth()
        )
        var context = LocalContext.current
        Spacer(modifier = Modifier.height(8.dp))



        Row(
            modifier = Modifier.fillMaxWidth(),
            verticalAlignment = Alignment.CenterVertically
        ){

            TextField(
                value = activityTime,
                readOnly = true,
                onValueChange = { activityTime = it },
                label = { Text("Heure de l'activité") },
                modifier = Modifier
                    .weight(2f)
                    .padding(end = 8.dp)
            )

            // Champ horaire (avec clic pour ouvrir le calendrier).
            Button(
                onClick = {
                    TimePickerDialog(
                        context,
                        { _, hour, minute ->
                            // Mettre à jour l'heure sélectionnée.
                            activityTime = String.format("%02d:%02d", hour, minute)
                        },
                        currentHour,
                        currentMinute,
                        true // Format 24h.
                    ).show()
                },
                modifier = Modifier.weight(1f)
            ) {
                Icon(
                    imageVector = Icons.Filled.Edit,
                    contentDescription = "Sélectionner l'heure",
                    modifier = Modifier.size(24.dp)
                )
            }
        }


        Spacer(modifier = Modifier.height(8.dp))

        // Menu déroulant pour la régularité.
        Box(modifier = Modifier.fillMaxWidth()) {
            Button(onClick = { isDropdownExpanded = !isDropdownExpanded }) {
                Text(text = "Régularité: ${regularite.name}")
            }

            DropdownMenu(
                expanded = isDropdownExpanded,
                onDismissRequest = { isDropdownExpanded = false }
            ) {
                Regularite.values().forEach { reg ->
                    DropdownMenuItem(
                        text = { Text(text = reg.name) },
                        onClick = {
                            regularite = reg
                            isDropdownExpanded = false
                        }
                    )
                }
            }
        }

        Spacer(modifier = Modifier.height(16.dp))

        Row{
            Button(onClick = onClose) {
                Text("Retour")
            }

            Spacer(modifier = Modifier.weight(1f))

            Button(
                onClick = {
                    if (NotificationPermissionHandler.hasNotificationPermission(context)) {
                        val newActivity = Activite(
                            nom_Activite = activityName,
                            Horaire = activityTime,
                            regularite = regularite,
                            id_animal = animalId
                        )

                        val message = "Activité ajoutée : ${newActivity.nom_Activite}"

                        NotificationScheduler.scheduleNotification(context, message, 1)

                        if (activityName.isNotEmpty() && activityTime.isNotEmpty()) {
                            // On vient appeller la callback d'ajout d'activité.
                            onActivityAdded(newActivity)
                        } else {
                            Toast.makeText(context, "Veuillez remplir tous les champs", Toast.LENGTH_SHORT).show()
                        }
                    } else {
                        // Si la permission n'est pas accordée, on la demande.
                        NotificationPermissionHandler.requestNotificationPermission(context as ComponentActivity)
                    }


                }
            ) {
                Text("Ajouter l'activité")
            }
        }
    }
}