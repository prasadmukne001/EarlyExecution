# Early Execution in Android Using JNI and ContentProvider

## 📌 Overview
This project demonstrates how to execute native C++ code **before** the Android `Application` class initializes. It uses:
- **JNI (Java Native Interface)** with `__attribute__((constructor))` to run code early.
- **ContentProvider** for additional early execution verification.
- **CMake** to build the native shared library (`early_exec.so`).
- **Kotlin (MVVM architecture)** to interact with native code and log execution order.

## 🎯 Key Features
✅ Native C++ execution **before Application class** using `__attribute__((constructor))`.<br>
✅ Capturing **app startup timestamp** using native code.<br>
✅ ContentProvider (`EarlyInitProvider`) to verify execution order.<br>
✅ Kotlin module (`NativeCommunicationModule`) to communicate with native functions.<br>
✅ CMake integration for compiling the C++ library.

---

## 📂 Project Structure
```
EarlyExecution/
├── .gradle/
├── .idea/
├── app/
│   ├── .cxx/
│   ├── build/
│   ├── libs/
│   ├── src/
│   │   ├── androidTest/
│   │   ├── main/
│   │   │   ├── cpp/
│   │   │   │   ├── CMakeLists.txt
│   │   │   │   ├── early_exec.cpp
│   │   │   ├── java/com/yinkozi/earlyexecution/
│   │   │   │   ├── application/
│   │   │   │   │   ├── EarlyExecutionApplication.kt
│   │   │   │   ├── components.main_screen/
│   │   │   │   │   ├── activity/
│   │   │   │   │   │   ├── MainActivity.kt
│   │   │   │   │   ├── viewmodel/
│   │   │   │   ├── content_provider/
│   │   │   │   │   ├── EarlyInitProvider.kt
│   │   │   │   ├── modules/
│   │   │   │   │   ├── NativeCommunicationModule.kt
│   │   │   ├── res/
│   │   │   │   ├── AndroidManifest.xml
│   │   ├── test/ [unitTest]
├── gradle/
├── .gitignore
├── build.gradle
├── gradle.properties
├── gradlew
├── gradlew.bat
├── local.properties
├── proguard-rules.pro
├── README.md
├── settings.gradle
└── External Libraries/

```

---

## 🚀 How It Works

### 1️⃣ **Native Execution Before Application**
- The **`early_exec.cpp`** file contains a constructor function:
  ```cpp
  __attribute__((constructor)) void early_exec() {
      LOGE("Native Library : Executing before Application class!");
      LOGE("App Startup Time: %lld ms", getStartupTime());
  }
  ```
- This function **runs automatically** before any Android class (including `Application`).
- Logs execution order via **Logcat**.

### 2️⃣ **Interfacing Native Code with Kotlin**
- `NativeCommunicationModule.kt` loads the native library:
  ```kotlin
  object NativeCommunicationModule {
      init {
          System.loadLibrary("early_exec")
          Log.d("NativeEarlyExec","Native library loaded!")
      }
      external fun getStartupTime(): Long
  }
  ```
- It allows retrieving the startup time from native code.

### 3️⃣ **Verifying Execution with ContentProvider**
- `EarlyInitProvider.kt` logs execution before `Application`:
  ```kotlin
  class EarlyInitProvider : ContentProvider() {
      override fun onCreate(): Boolean {
          Log.d("NativeEarlyExec","EarlyInitContentProvider executed before Application class")
          return true
      }
  }
  ```

### 4️⃣ **Logging Execution Order in Application Class**
- `EarlyExecutionApplication.kt` logs native execution time and verifies order:
  ```kotlin
  class EarlyExecutionApplication : Application() {
      companion object {
          init { NativeCommunicationModule }
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
  ```

---

## 🔧 **Building and Running the Project**
### **1️⃣ Configure CMake**
- Ensure `CMakeLists.txt` exists in `app/src/main/cpp/`:
  ```cmake
  cmake_minimum_required(VERSION 3.22.1)
  project("earlyexecution")
  add_library(early_exec SHARED early_exec.cpp)
  find_library(log-lib log)
  target_link_libraries(early_exec ${log-lib} android log)
  ```

### **2️⃣ Sync and Build the Project**
1. Open Android Studio.
2. Go to `File` → `Sync Project with Gradle Files`.
3. Build the project (`Build > Make Project`).
4. Run the app on an emulator or device.

### **3️⃣ Check Execution Order in Logcat**
Filter logs by `NativeEarlyExec` to see:
```
 Native Library : Executing before Application class!
 App Startup Time: 12345 ms
 EarlyInitContentProvider executed before Application class
 Native library loaded!
 Application Constructor: Running after native execution.
 App Startup Time (from Native library): 12345 ms
 Application onCreate: Running last.
```
This confirms that **native execution happened before everything else**!

---

## 📌 **Conclusion**
This project successfully demonstrates executing **native code before the Application class**, ensuring early logging, analytics, crashlytics or security-related tasks can run first. 🚀
