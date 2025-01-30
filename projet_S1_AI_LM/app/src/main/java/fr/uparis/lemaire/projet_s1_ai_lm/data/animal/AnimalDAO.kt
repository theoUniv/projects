package fr.uparis.lemaire.projet_s1_ai_lm.data.animal

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update
import kotlinx.coroutines.flow.Flow

@Dao
interface AnimalDAO {

    @Update
    suspend fun updateAnimal(animal: Animal)

    @Insert
    suspend fun insertAnimal(animal: Animal)

    @Query("SELECT * FROM Animal")
    fun loadAllAnimal(): Flow<List<Animal>>

    @Query("DELETE FROM Animal")
    suspend fun clearAll()

    @Query("SELECT * FROM Animal WHERE name LIKE :prefix || '%'")
    fun loadAnimalsByPrefix(prefix: String): Flow<List<Animal>>

    @Query("SELECT * FROM Animal WHERE id_Animal = :id_Animal")
    fun loadAnimalById(id_Animal: Int): Flow<Animal>

    @Query("DELETE FROM Animal WHERE id_Animal = :id_Animal")
    fun deleteAnimal(id_Animal: Int)
}