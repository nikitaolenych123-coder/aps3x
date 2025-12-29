# Інструкції зі збірки APS3E для Poco F6

## ✅ Оптимізації вже застосовані!

Всі оптимізації для Poco F6 вже додані до проекту:

### Змінені файли:
- ✅ `app/src/main/cpp/CMakeLists.txt` - ARMv9-A + Cortex-X4 оптимізації
- ✅ `app/build.gradle` - Налаштування Release збірки
- ✅ `gradle.properties` - Gradle оптимізації

### Нові файли:
- ✅ `app/src/main/cpp/adreno735_optimizations.h` - GPU оптимізації
- ✅ `app/src/main/cpp/poco_f6_optimizations.h` - Пам'ять та JIT
- ✅ `app/src/main/cpp/real_steel_fps_patch.h` - FPS розблокування
- ✅ `build_poco_f6.sh` - Скрипт автозбірки

## Збірка на локальній машині

### Вимоги:
- Ubuntu/Debian Linux (рекомендовано)
- 8GB+ RAM
- 20GB+ вільного місця
- Інтернет з'єднання

### Швидка збірка:

```bash
# 1. Клонуйте репозиторій (якщо ще не зробили)
git clone https://github.com/aenu1/aps3e.git
cd aps3e

# 2. Запустіть скрипт автозбірки
chmod +x build_poco_f6.sh
./build_poco_f6.sh
```

Скрипт автоматично:
- Встановить Android SDK та NDK v27.2.12479018
- Встановить залежності (CMake, Ninja)
- Зберe оптимізований APK
- Створить звіт про збірку

### Результат:
- **APK**: `build_output/aps3e-poco-f6-optimized.apk`
- **Звіт**: `build_output/BUILD_REPORT.txt`

### Час збірки:
- Перша збірка (з встановленням SDK): ~30-45 хвилин
- Наступні збірки: ~10-15 хвилин

## Ручна збірка (якщо маєте Android SDK)

```bash
# Встановіть шлях до SDK
export ANDROID_HOME=$HOME/Android/Sdk
export ANDROID_NDK_HOME=$HOME/Android/Sdk/ndk/27.2.12479018

# Оновіть local.properties
echo "sdk.dir=$ANDROID_HOME" > local.properties

# Зберіть
./gradlew assembleRelease \
    --parallel \
    --max-workers=$(nproc)

# APK буде тут:
# app/build/outputs/apk/release/app-release-unsigned.apk
```

## Альтернатива: GitHub Actions

Можна налаштувати автоматичну збірку через GitHub Actions:

```yaml
# .github/workflows/build.yml
name: Build Poco F6 APK

on:
  push:
    branches: [ main ]
  workflow_dispatch:

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Set up JDK 17
        uses: actions/setup-java@v4
        with:
          java-version: '17'
          distribution: 'temurin'
      
      - name: Build with script
        run: |
          chmod +x build_poco_f6.sh
          ./build_poco_f6.sh
      
      - name: Upload APK
        uses: actions/upload-artifact@v4
        with:
          name: aps3e-poco-f6
          path: build_output/*.apk
```

## Збірка в Docker

```bash
# Створіть Dockerfile
cat > Dockerfile << 'EOF'
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    openjdk-17-jdk \
    wget \
    unzip \
    git

WORKDIR /build
COPY . .

RUN chmod +x build_poco_f6.sh && ./build_poco_f6.sh

CMD ["cp", "-r", "build_output", "/output"]
EOF

# Зберіть
docker build -t aps3e-builder .
docker run -v $(pwd)/output:/output aps3e-builder
```

## Що містить оптимізована збірка?

### CPU:
- ✅ ARMv9-A з SVE2
- ✅ Cortex-X4 специфічна оптимізація
- ✅ -Ofast (максимальна швидкість)
- ✅ Link Time Optimization (LTO)

### GPU (Adreno 735):
- ✅ Shader pre-caching
- ✅ Оптимізовані descriptor pools (4096)
- ✅ Зменшена синхронізація CPU-GPU
- ✅ 128-wide wavefront оптимізація

### Пам'ять (LPDDR5X):
- ✅ PPU JIT Cache: 512 MB
- ✅ SPU JIT Cache: 256 MB
- ✅ SPU LLVM Cache: 1024 MB
- ✅ LLVM потоків: 6

### Real Steel:
- ✅ FPS розблокування (30→60 FPS)
- ✅ Компенсація фізики
- ✅ Оптимізація VSync

## Налаштування в додатку (після встановлення)

**Для найкращої продуктивності:**
- CPU Decoder: LLVM (Recompiler)
- SPU Decoder: LLVM (ASMJIT)
- Renderer: Vulkan
- Resolution: 1x або 2x
- VSync: OFF
- Frame Limit: 60 FPS

**Для Real Steel:**
- Resolution: Native або 1.5x
- Shader Cache: ON
- Frame Limit: 60 FPS

## Очікувана продуктивність

| Тип гри | FPS (Native) | FPS (2x) |
|---------|--------------|----------|
| 2D ігри | 60 (стабільно) | 60 |
| Легкі 3D | 50-60 | 40-50 |
| Середні 3D | 30-45 | 25-35 |
| Важкі 3D | 20-30 | 15-25 |

**Real Steel**: 50-60 FPS (було 30 FPS)

## Проблеми та рішення

### Проблема: "SDK location not found"
**Рішення:** Встановіть Android SDK або запустіть `build_poco_f6.sh`

### Проблема: "Java 17 required"
**Рішення:**
```bash
sudo apt-get install openjdk-17-jdk
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
```

### Проблема: Out of memory
**Рішення:** Збільште RAM в `gradle.properties`:
```properties
org.gradle.jvmargs=-Xmx8192m
```

### Проблема: NDK not found
**Рішення:** Скрипт встановить автоматично, або:
```bash
sdkmanager "ndk;27.2.12479018"
```

## Підтримка

Для питань та проблем:
2. Запустіть з `--stacktrace` для детальної інформації
3. Створіть issue на GitHub з логами

---

**Версія**: Poco F6 Optimized  
**Дата**: Грудень 2025  
**Статус**: Готово до збірки ✅
