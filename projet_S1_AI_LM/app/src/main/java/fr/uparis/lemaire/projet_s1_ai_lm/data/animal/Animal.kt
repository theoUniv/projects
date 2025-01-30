package fr.uparis.lemaire.projet_s1_ai_lm.data.animal
import androidx.room.Entity
import androidx.room.PrimaryKey
import java.io.Serializable


enum class Espece {
    CHIEN,
    CHAT,
    HAMSTER,
    POISSON
}

@Entity
data class Animal (
    @PrimaryKey(autoGenerate = true)
    val id_Animal: Int = 0,
    val name: String = "",
    val type: Espece
): Serializable {
    override fun toString(): String {
        return "Animal(name='$name', type=$type)"
    }

}