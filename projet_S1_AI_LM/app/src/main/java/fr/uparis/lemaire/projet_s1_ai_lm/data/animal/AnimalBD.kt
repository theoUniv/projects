package fr.uparis.lemaire.projet_s1_ai_lm.data.animal

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase

@Database(entities = [Animal::class], version = 1)
abstract class AnimalBD : RoomDatabase() {
    abstract fun animalDao(): AnimalDAO
    companion object {
        @Volatile
        private var instance: AnimalBD? = null
        fun getDataBase(c: Context): AnimalBD {
            if (instance != null) return instance!!
            val db = Room.databaseBuilder(c.applicationContext, AnimalBD::class.java, "Animals")
                .fallbackToDestructiveMigration()
                .build()
            instance = db
            return instance!!
        }
    }
}