import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.Activite
import fr.uparis.lemaire.projet_s1_ai_lm.data.activity.ActiviteBD
import fr.uparis.lemaire.projet_s1_ai_lm.data.animal.Animal
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.flatMapLatest
import kotlinx.coroutines.launch

class ActiviteViewModel(application: Application) : AndroidViewModel(application) {
    private val dao by lazy { ActiviteBD.getDataBase(application).activiteDao() }

    val _activites: Flow<List<Activite>> = dao.loadAllActivite()

    private val _searchID = MutableStateFlow(0)
    val searchID: StateFlow<Int> = _searchID

    val filteredActivities = searchID.flatMapLatest { id ->
        dao.loadActiviteById(id.toInt())
    }

    fun updateSearchID(prefix: Int) {
        _searchID.value = prefix
    }

    fun getActiviteById(id_Activite: Int): Flow<Activite> {
        return dao.loadActiviteByIdActivite(id_Activite)
    }

    fun addActivite(activite: Activite) {
        viewModelScope.launch {
            dao.insertActivite(activite)
        }
    }

    fun deleteActivite(id_Activite: Int) {
        dao.deleteActivite(id_Activite)
    }

    fun updateActivite(activite: Activite) {
        viewModelScope.launch {
            dao.updateActivite(activite)
        }
    }


}
