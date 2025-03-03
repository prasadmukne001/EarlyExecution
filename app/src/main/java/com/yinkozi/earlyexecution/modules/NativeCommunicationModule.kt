//
// Created by Prasad on 02/03/2025.
//

package com.yinkozi.earlyexecution.modules

import android.util.Log

object NativeCommunicationModule {
    init {
        System.loadLibrary("early_exec")
        Log.d("NativeEarlyExec","NativeCommunicationModule: Native library loaded!")
    }

    external fun getStartupTime(): Long
}