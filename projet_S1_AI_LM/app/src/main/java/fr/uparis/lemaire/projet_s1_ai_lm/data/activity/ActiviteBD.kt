package fr.uparis.lemaire.projet_s1_ai_lm.data.activity

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase


@Database(entities = [Activite::class], version = 1)
abstract class ActiviteBD : RoomDatabase() {
    abstract fun activiteDao(): ActiviteDAO
    companion object {
        @Volatile
        private var instance: ActiviteBD? = null
        fun getDataBase(c: Context): ActiviteBD {
            if (instance != null) return instance!!
            val db = Room.databaseBuilder(c.applicationContext, ActiviteBD::class.java, "Activites")
                .fallbackToDestructiveMigration()
                .build()
            instance = db
            return instance!!
        }
    }
}