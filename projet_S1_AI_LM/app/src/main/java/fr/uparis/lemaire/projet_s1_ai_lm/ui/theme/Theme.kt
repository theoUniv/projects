package fr.uparis.lemaire.projet_s1_ai_lm.ui.theme

import android.os.Build
import android.util.Log
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material3.ColorScheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.dynamicDarkColorScheme
import androidx.compose.material3.dynamicLightColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import fr.uparis.lemaire.projet_s1_ai_lm.GlobalTheme

// Palette de couleurs pour le mode sombre
private val DarkColorScheme = darkColorScheme(
    primary = Color(0xFF388E3C),
    secondary = Color(0xFF66BB6A),
    tertiary = Color(0xFF8BC34A),
    background = Color(0xFF1B1B1B),
    surface = Color(0xFF2C2C2C),
    onPrimary = Color.White,
    onSecondary = Color.White,
    onTertiary = Color.Black,
    onBackground = Color(0xFFE0E0E0),
    onSurface = Color(0xFFE0E0E0)
)

// Palette de couleurs pour le mode clair
private val LightColorScheme = lightColorScheme(
    primary = Color(0xFF4CAF50),
    secondary = Color(0xFF81C784),
    tertiary = Color(0xFF9CCC65),
    background = Color(0xFFF1F8E9),
    surface = Color(0xFFFFFFFF),
    onPrimary = Color.White,
    onSecondary = Color.Black,
    onTertiary = Color.Black,
    onBackground = Color(0xFF1B5E20),
    onSurface = Color(0xFF1B5E20)
)

fun generateColorScheme(baseColor: Color): ColorScheme {
    Log.d("ColorScheme", "Base color: $baseColor")

    // Créer des couleurs à partir de la couleur de base.
    val primary = baseColor
    val secondary = baseColor.copy(alpha = 0.7f)
    val tertiary = baseColor.copy(alpha = 0.5f)
    val background = baseColor.copy(alpha = 0.1f)
    val surface = baseColor.copy(alpha = 0.3f)
    val onPrimary = Color.White
    val onSecondary = Color.White
    val onTertiary = Color.Black
    val onBackground = Color.Black
    val onSurface = Color.Black

    // Couleurs spécifiques pour les boutons et éléments interactifs.
    val onPrimaryContainer = Color.Black
    val onSecondaryContainer = Color.Black
    val onSurfaceVariant = Color.Gray

    return lightColorScheme(
        primary = primary,
        secondary = secondary,
        tertiary = tertiary,
        background = background,
        surface = surface,
        onPrimary = onPrimary,
        onSecondary = onSecondary,
        onTertiary = onTertiary,
        onBackground = onBackground,
        onSurface = onSurface,
        onPrimaryContainer = onPrimaryContainer,
        onSecondaryContainer = onSecondaryContainer,
        onSurfaceVariant = onSurfaceVariant
    )
}



@Composable
fun Projet_S1_AI_LMTheme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    dynamicColor: Boolean = true,
    content: @Composable () -> Unit
) {
    // Utiliser la couleur globale du thème si elle est définie.
    val colorScheme = GlobalTheme.colorScheme.takeIf { it != generateColorScheme(Color.Green) }
        ?: when {
            dynamicColor && Build.VERSION.SDK_INT >= Build.VERSION_CODES.S -> {
                val context = LocalContext.current
                if (darkTheme) dynamicDarkColorScheme(context) else dynamicLightColorScheme(context)
            }
            darkTheme -> DarkColorScheme
            else -> LightColorScheme
        }

    MaterialTheme(
        colorScheme = colorScheme,
        typography = Typography,
        content = content
    )
}
