package fr.uparis.lemaire.projet_s1_ai_lm.notifications

import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.Handler
import android.os.Looper
import android.widget.Toast
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import androidx.core.content.ContextCompat
import android.content.pm.PackageManager
import android.Manifest

// Cet objet gère l'envoit notifications.
object NotificationScheduler {

    private const val CHANNEL_ID = "activity_channel"
    private const val CHANNEL_NAME = "Activité Notifications"
    private const val CHANNEL_DESCRIPTION = "Notifications pour les nouvelles activités"

    private fun createNotificationChannel(context: Context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                CHANNEL_ID,
                CHANNEL_NAME,
                NotificationManager.IMPORTANCE_DEFAULT
            ).apply {
                description = CHANNEL_DESCRIPTION
            }
            val notificationManager = context.getSystemService(NotificationManager::class.java)
            notificationManager?.createNotificationChannel(channel)
        }
    }

    private fun hasNotificationPermission(context: Context): Boolean {
        return ContextCompat.checkSelfPermission(context, Manifest.permission.POST_NOTIFICATIONS) == PackageManager.PERMISSION_GRANTED
    }

    fun scheduleNotification(context: Context, message: String, delayInSeconds: Long) {
        if (hasNotificationPermission(context)) {
            createNotificationChannel(context)

            val handler = Handler(Looper.getMainLooper())
            handler.postDelayed({
                sendNotification(context, message)
            }, delayInSeconds * 1000)
        } else {
            Toast.makeText(context, "Veuillez accorder la permission de notification", Toast.LENGTH_SHORT).show()
        }
    }

    private fun sendNotification(context: Context, message: String) {
        try {
            val notificationIntent = Intent(context, context::class.java).apply {
                flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
            }
            val pendingIntent: PendingIntent = PendingIntent.getActivity(
                context, 0, notificationIntent, PendingIntent.FLAG_IMMUTABLE
            )

            val notification = NotificationCompat.Builder(context, CHANNEL_ID)
                .setSmallIcon(android.R.drawable.ic_dialog_info)
                .setContentTitle("Nouvelle activité ajoutée")
                .setContentText(message)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                .setContentIntent(pendingIntent)
                .setAutoCancel(true)
                .build()

            NotificationManagerCompat.from(context).notify(1001, notification)

        } catch (e: SecurityException) {
            e.printStackTrace()
            Toast.makeText(context, "Erreur de permission: Impossible d'envoyer la notification.", Toast.LENGTH_SHORT).show()
        }
    }
}
