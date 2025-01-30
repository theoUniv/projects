package fr.uparis.lemaire.projet_s1_ai_lm.data.activity

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update
import kotlinx.coroutines.flow.Flow


@Dao
interface ActiviteDAO {


    @Update
    suspend fun updateActivite(activite: Activite)

    @Insert
    suspend fun insertActivite(activite: Activite)

    @Query("SELECT * FROM Activite")
    fun loadAllActivite(): Flow<List<Activite>>

    @Query("SELECT * FROM Activite WHERE id_animal = :id_Activite")
    fun loadActiviteById(id_Activite: Int): Flow<List<Activite>>

    @Query("DELETE FROM Activite WHERE id_Activite = :id_Activite")
    fun deleteActivite(id_Activite: Int)

    @Query("DELETE FROM Activite")
    suspend fun clearAll()

    @Query("SELECT * FROM Activite WHERE id_Activite = :id_Activite")
    fun loadActiviteByIdActivite(id_Activite: Int): Flow<Activite>

}