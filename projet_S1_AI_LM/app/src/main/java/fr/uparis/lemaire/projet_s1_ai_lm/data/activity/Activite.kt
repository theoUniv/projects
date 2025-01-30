package fr.uparis.lemaire.projet_s1_ai_lm.data.activity

import androidx.room.Entity
import androidx.room.PrimaryKey

enum class Regularite{
    QUOTIDIEN,
    HEBDOMADAIRE,
    UNIQUE
}

@Entity
data class Activite (
    @PrimaryKey(autoGenerate = true)
    val id_Activite: Int = 0,
    var nom_Activite: String = "",
    var Horaire: String = "",
    var regularite: Regularite,
    var id_animal: Int = 0
){

}