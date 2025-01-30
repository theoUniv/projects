package fr.uparis.lemaire.projet_s1_ai_lm.models

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Animal
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.AnimalBD
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Espece
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.flatMapLatest
import kotlinx.coroutines.launch

class AnimalViewModel(application: Application) : AndroidViewModel(application)  {
    private val dao by lazy { AnimalBD.getDataBase(application).animalDao()}

    private val _lastAnimalCreatedID = MutableStateFlow(0)
    val lastAnimalID: StateFlow<Int> = _lastAnimalCreatedID

    val allAnimals: Flow<List<Animal>> = dao.loadAllAnimal()

    private val _searchPrefix = MutableStateFlow("")
    val searchPrefix: StateFlow<String> = _searchPrefix

    val filteredAnimals = searchPrefix.flatMapLatest { prefix ->
        dao.loadAnimalsByPrefix(prefix)
    }

    private val _searchID = MutableStateFlow(0)
    val searchID: StateFlow<Int> = _searchID

    suspend fun updateSearchID() : Int {
        var id = -1
        var animals = dao.loadAllAnimal()
        animals.collect{
            id = it.maxOfOrNull { it.id_Animal } ?: 0
        }
        return id
    }

    suspend fun getLastId(): Int {
        val animals = dao.loadAllAnimal().first()
        return animals.maxOfOrNull { it.id_Animal } ?: 0
    }


    fun getAnimalById(id_Animal: Int): Flow<Animal> {
        return dao.loadAnimalById(id_Animal)
    }

    fun deleteAnimalbyId(id_Animal: Int) {
        dao.deleteAnimal(id_Animal)
    }

    fun updateSearchPrefix(prefix: String) {
        _searchPrefix.value = prefix.trim()
    }

    fun updateAnimal(animal: Animal) {
        viewModelScope.launch {
            dao.updateAnimal(animal)
        }
    }

    fun clearAllAnimals() {
        viewModelScope.launch {
            dao.clearAll()
        }
    }

    suspend fun ajouterAnimal(nom: String, type: Espece): Int {
        val animal = Animal( name = nom, type = type )
        dao.insertAnimal(animal)
        return dao.loadAllAnimal().first().maxOfOrNull { it.id_Animal } ?: 0
    }


    fun effacerBase() {
        viewModelScope.launch {
            dao.clearAll()
        }
    }

}