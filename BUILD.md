# Build Instructions

**Rusty Lake: The Alchemist's Orrery**

---

## 📋 Prerequisites

### Required Software

1. **Unreal Engine 4.27**
   - Download from [Epic Games Launcher](https://www.unrealengine.com/download)
   - Install with Android support

2. **Visual Studio 2019 or later**
   - Install with "Game Development with C++" workload
   - Install "Desktop Development with C++" workload

3. **Android Development Tools**
   - Android Studio (latest version)
   - Android SDK API Level 21-30
   - Android NDK r21 or later
   - Java JDK 8 or later

### Environment Setup

1. **Set ANDROID_HOME environment variable**:
   ```
   ANDROID_HOME=C:\Users\YourName\AppData\Local\Android\Sdk
   ```

2. **Set JAVA_HOME environment variable**:
   ```
   JAVA_HOME=C:\Program Files\Java\jdk1.8.0_xxx
   ```

3. **Configure UE4 Android SDK/NDK paths**:
   - Open UE4 Editor
   - Edit → Project Settings → Platforms → Android SDK
   - Set SDK, NDK, and JDK paths

---

## 🚀 Building the Project

### Step 1: Clone Repository

```bash
git clone https://github.com/gdszyy/rusty-lake-orrery-game.git
cd rusty-lake-orrery-game
```

### Step 2: Generate Project Files

**Windows**:
```bash
# Right-click on RustyLakeOrrery.uproject
# Select "Generate Visual Studio project files"
```

Or use command line:
```bash
"%UE4_ROOT%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%CD%\RustyLakeOrrery.uproject" -game -engine
```

### Step 3: Build C++ Code

**Option A: Using Visual Studio**
1. Open `RustyLakeOrrery.sln`
2. Set build configuration to "Development Editor"
3. Build → Build Solution (Ctrl+Shift+B)

**Option B: Using UE4 Editor**
1. Double-click `RustyLakeOrrery.uproject`
2. If prompted to rebuild modules, click "Yes"
3. Wait for compilation to complete

### Step 4: Verify Build

1. Open UE4 Editor
2. Check Output Log for any errors
3. Test InteractionSystem in a test level

---

## 📱 Building for Android

### Development Build

1. **Open Project Settings**:
   - Edit → Project Settings

2. **Configure Android Settings**:
   - Platforms → Android
   - Package Name: `com.rustylake.orrery`
   - Min SDK Version: 21
   - Target SDK Version: 30
   - Enable "Package game data inside .apk"

3. **Package Project**:
   - File → Package Project → Android → Android (Multi:ETC2)
   - Select output directory
   - Wait for packaging to complete

4. **Install on Device**:
   - Enable USB debugging on Android device
   - Connect device via USB
   - Run generated APK or use:
     ```bash
     adb install -r YourApp.apk
     ```

### Shipping Build

1. **Update Build Configuration**:
   - Open `Config/DefaultGame.ini`
   - Set `BuildConfiguration=PPBC_Shipping`

2. **Configure Signing** (for distribution):
   - Project Settings → Platforms → Android
   - Set KeyStore, KeyAlias, and passwords

3. **Package**:
   - File → Package Project → Android → Android (Multi:ETC2)
   - Build will be optimized and signed

4. **Output**:
   - APK location: `[OutputDir]/Android_Multi/`
   - OBB files (if any): Same directory

---

## 🧪 Testing

### PC Testing (Editor)

1. Open test level: `Content/Maps/TestLevel`
2. Click Play (Alt+P)
3. Test interactions with mouse
4. Check Output Log for debug messages

### Android Device Testing

1. **Launch on Device**:
   - File → Launch → [Your Device Name]
   - Or install APK manually

2. **Debug Logging**:
   ```bash
   # View real-time logs
   adb logcat -s UE4
   
   # Filter for InteractionSystem logs
   adb logcat -s UE4 | findstr "Interaction"
   ```

3. **Performance Profiling**:
   - Enable "Show FPS" in game
   - Use Android Profiler in Android Studio

---

## 🛠️ Troubleshooting

### Common Issues

**Issue**: "Failed to compile C++ modules"
- **Solution**: 
  - Clean solution in Visual Studio
  - Delete `Intermediate` and `Binaries` folders
  - Regenerate project files
  - Rebuild

**Issue**: "Android SDK not found"
- **Solution**:
  - Verify ANDROID_HOME environment variable
  - Check SDK path in Project Settings
  - Restart UE4 Editor

**Issue**: "APK install failed"
- **Solution**:
  - Uninstall existing app on device
  - Check device storage space
  - Verify USB debugging is enabled

**Issue**: "Touch input not working"
- **Solution**:
  - Check `bEnableTouchEvents=true` in PlayerController
  - Verify Input settings in DefaultInput.ini
  - Test on actual device (not emulator)

### Clean Build

If you encounter persistent issues:

```bash
# 1. Close UE4 Editor and Visual Studio

# 2. Delete generated files
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved
del /q *.sln

# 3. Regenerate project files
# Right-click RustyLakeOrrery.uproject → Generate Visual Studio project files

# 4. Rebuild
# Open .sln → Build → Rebuild Solution
```

---

## 📊 Build Configurations

| Configuration | Use Case | Optimization | Debug Info |
|---------------|----------|--------------|------------|
| **DebugGame** | C++ debugging | None | Full |
| **Development** | General development | Partial | Partial |
| **Shipping** | Final release | Full | None |

### Selecting Configuration

**Visual Studio**:
- Top toolbar → Configuration dropdown

**UE4 Editor**:
- File → Package Project → Build Configuration

---

## 🔧 Advanced Build Options

### Custom Build Flags

Edit `Source/RustyLakeOrrery/RustyLakeOrrery.Build.cs`:

```csharp
// Enable additional optimizations
PublicDefinitions.Add("WITH_CUSTOM_OPTIMIZATION=1");

// Add custom include paths
PublicIncludePaths.Add("ThirdParty/MyLibrary/Include");
```

### Packaging Settings

Edit `Config/DefaultGame.ini`:

```ini
[/Script/UnrealEd.ProjectPackagingSettings]
# Compression
bCompressed=True
PakFileCompressionMethod=Kraken

# Exclude content from package
+DirectoriesToNeverCook=(Path="/Game/Debug")

# Include additional files
+DirectoriesToAlwaysCook=(Path="/Game/Data")
```

---

## 📦 Build Outputs

### Development Build

```
[OutputDir]/
├── Android_Multi/
│   ├── RustyLakeOrrery-Android-Shipping.apk  # Main APK
│   └── main.obb                               # Data file (if large)
└── Manifest_NonUFSFiles_Android.txt           # Build manifest
```

### File Sizes (Approximate)

- APK: 50-100 MB
- OBB: 100-500 MB (depending on assets)

---

## 🚢 Distribution Checklist

Before distributing:

- [ ] Test on multiple Android devices
- [ ] Verify all touch gestures work
- [ ] Check performance (60 FPS target)
- [ ] Test all three chapters
- [ ] Verify save/load functionality
- [ ] Test on different screen sizes
- [ ] Check audio playback
- [ ] Verify all UI elements are visible
- [ ] Test network connectivity (if any)
- [ ] Sign APK with release keystore

---

## 📞 Support

If you encounter build issues:

1. Check this document first
2. Review UE4 Output Log for errors
3. Search [UE4 AnswerHub](https://answers.unrealengine.com/)
4. Contact project maintainer

---

**Last Updated**: 2025-12-07
