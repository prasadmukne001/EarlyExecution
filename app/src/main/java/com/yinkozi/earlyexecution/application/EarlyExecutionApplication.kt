//
// Created by Prasad on 02/03/2025.
//

package com.yinkozi.earlyexecution.application

import android.app.Application
import android.util.Log
import com.yinkozi.earlyexecution.modules.NativeCommunicationModule

class EarlyExecutionApplication : Application() {

    companion object {
        init {
            NativeCommunicationModule
        }
    }

    init {
        Log.d("NativeEarlyExec","Application Constructor: Running after native execution.")
        Log.d("NativeEarlyExec","App Startup Time (from Native library): ${NativeCommunicationModule.getStartupTime()} ms")
    }

    override fun onCreate() {
        super.onCreate()
        Log.d("NativeEarlyExec","Application onCreate: Running last.")
    }
}
