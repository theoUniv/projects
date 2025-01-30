package fr.uparis.lemaire.projet_s1_ai_lm

import ActiviteViewModel
import android.app.TimePickerDialog
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
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.filled.ArrowBack
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
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Activite
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Regularite
import fr.uparis.lemaire.projet_s1_ai_lm.ui.theme.Projet_S1_AI_LMTheme


class EditActivityActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {

            Projet_S1_AI_LMTheme{
                val activityId = intent.getIntExtra("activity_id", -1)

                EditActivityVue(
                    viewModel = ActiviteViewModel(application = application),
                    onActivityEdited = {},
                    activityId = activityId,
                    onClose = { finish() }
                )
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun EditActivityVue(
    viewModel: ActiviteViewModel = viewModel(),
    onActivityEdited: () -> Unit,
    activityId: Int,
    onClose: () -> Unit
) {
    val activite by viewModel.getActiviteById(activityId).collectAsState(
        Activite(nom_Activite = "", Horaire = "", regularite = Regularite.QUOTIDIEN)
    )

    var name by remember { mutableStateOf(activite.nom_Activite) }
    var horaire by remember { mutableStateOf(activite.Horaire) }
    var regularite by remember { mutableStateOf(activite.regularite) }
    var expanded by remember { mutableStateOf(false) }
    val items = Regularite.values().map { it.name }
    val context = LocalContext.current

    Column(
        modifier = Modifier
            .fillMaxWidth()
            .padding(16.dp)
    ) {
        androidx.compose.material3.TopAppBar(
            title = { Text("Modifier l'Activité", style = androidx.compose.material3.MaterialTheme.typography.titleMedium) },
            navigationIcon = {
                androidx.compose.material3.IconButton(onClick = onClose) {
                    androidx.compose.material.icons.Icons.Default.ArrowBack
                        .let { androidx.compose.material3.Icon(imageVector = it, contentDescription = "Retour") }
                }
            },
        )

        Spacer(modifier = Modifier.size(24.dp))

        TextField(
            value = name,
            onValueChange = { name = it },
            label = { Text("Nom de l'activité") },
            modifier = Modifier.fillMaxWidth()
        )

        Spacer(modifier = Modifier.size(16.dp))

        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween
        ) {
            Text(
                text = "Horaire : $horaire",
                style = androidx.compose.material3.MaterialTheme.typography.bodyMedium,
                modifier = Modifier.alignByBaseline()
            )
            Button(
                onClick = {
                    val currentHour = 0
                    val currentMinute = 0
                    TimePickerDialog(
                        context,
                        { _, hour, minute ->
                            horaire = String.format("%02d:%02d", hour, minute)
                        },
                        currentHour,
                        currentMinute,
                        true
                    ).show()
                }
            ) {
                Text("Modifier l'heure")
            }
        }

        Spacer(modifier = Modifier.size(16.dp))

        ExposedDropdownMenuBox(
            expanded = expanded,
            onExpandedChange = { expanded = !expanded }
        ) {
            TextField(
                value = regularite.toString(),
                onValueChange = {},
                readOnly = true,
                label = { Text("Régularité") },
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
                            regularite = Regularite.valueOf(item)
                            expanded = false
                        }
                    )
                }
            }
        }

        Spacer(modifier = Modifier.size(24.dp))

        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.SpaceBetween
        ) {
            Button(onClick = onClose, modifier = Modifier.weight(1f)) {
                Text("Annuler")
            }

            Spacer(modifier = Modifier.size(16.dp))

            Button(
                onClick = {
                    viewModel.updateActivite(
                        activite.copy(
                            nom_Activite = name,
                            Horaire = horaire,
                            regularite = regularite
                        )
                    )
                    Toast.makeText(context, "Activité modifiée avec succès", Toast.LENGTH_SHORT).show()
                    onClose()
                },
                modifier = Modifier.weight(1f)
            ) {
                Text("Enregistrer")
            }
        }
    }
}
