# 🚀 ARMv9-A Extreme Build через GitHub Actions

## 🎯 Оптимізації для Poco F6

**Архітектура:** ARMv9-A + Cortex-X4  
**Компілятор:** LLVM/Clang 19 (NDK r27)  
**Оптимізації:** `-O3 -Ofast -flto=full -march=armv9-a -mcpu=cortex-x4`

### Що вимкнено для максимальної продуктивності:
- ❌ Весь debugging
- ❌ Логування
- ❌ Телеметрія
- ✅ Strict frame pacing для Real Steel (30 FPS)

## 📦 Автоматична збірка

Workflow запускається автоматично при:
- Push в `main` гілку
- Pull Request
- Вручну через GitHub UI

## 🎮 Як запустити збірку:

### Метод 1: Автоматично (при push)
```bash
git add .
git commit -m "Trigger build"
git push
```

### Метод 2: Вручну через UI
1. Перейдіть на GitHub: https://github.com/aenu1/aps3e/actions
2. Натисніть **Actions** (вгорі)
3. Виберіть **Build Poco F6 ARMv9-A Optimized APK**
4. Натисніть **Run workflow** ➜ **Run workflow**
5. Зачекайте ~20-30 хвилин (перша збірка)
6. Наступні збірки ~12-15 хвилин (з кешем)

## 📥 Де забрати APK:

1. Відкрийте завершену збірку в **Actions**
2. Прокрутіть вниз до секції **Artifacts**
3. Завантажте `aps3e-poco-f6-armv9-optimized-YYYYMMDD-HHMMSS`
4. Розархівуйте ZIP
5. Встановіть APK на Poco F6

## 🔧 Технічні деталі збірки:

### Compiler Flags:
```
-march=armv9-a+sve+sve2
-mcpu=cortex-x4
-mtune=cortex-x4
-O3
-Ofast
-flto=full (Full Link Time Optimization)
-fomit-frame-pointer
-ffast-math
-funroll-loops
-fvectorize
-fslp-vectorize
-fno-signed-zeros
-fno-trapping-math
-fassociative-math
-freciprocal-math
-ffp-contract=fast
```

### Linker Flags:
```
-flto=full
-fuse-ld=lld
-Wl,--lto-O3
-Wl,--icf=all (Identical Code Folding)
-Wl,--gc-sections (Garbage Collection)
-Wl,-O3
```

### Build Definitions:
```
NDEBUG
RELEASE_BUILD
NO_DEBUG_LOG
DISABLE_TELEMETRY
DISABLE_LOGGING
TARGET_FPS=30
FRAME_PACING_STRICT
REAL_STEEL_OPTIMIZED
```

### R8/ProGuard:
- ✅ Full mode enabled
- ✅ Code shrinking
- ✅ Resource shrinking
- ✅ Obfuscation

## ⚡ Переваги GitHub Actions:

✅ **Потужніші машини:** 4 CPU cores, 16GB RAM  
✅ **LLVM/Clang 19:** Найновіший компілятор з NDK r27  
✅ **Full LTO:** Максимальна оптимізація  
✅ **Кешування:** Наступні збірки в 2x швидше  
✅ **Автоматизація:** Збирається при кожному push  
✅ **Безкоштовно:** 2000 хвилин/місяць для публічних репо  
✅ **ARMv9-A:** Специфічні оптимізації для Cortex-X4  

## 🎯 Real Steel Performance:

Після встановлення оптимізованого APK:

**Рекомендовані налаштування:**
- CPU Decoder: LLVM (Recompiler)
- SPU Decoder: LLVM (ASMJIT)
- Renderer: Vulkan
- Resolution: Native (1080p) або 1.5x
- VSync: OFF
- Frame Limit: 30 FPS
- Shader Cache: ON

**Очікувана продуктивність:**
- Stable 30 FPS з perfect frame pacing
- Немає stuttering
- Мінімальна input lag
- Smooth gameplay

## 🖥️ Локальна збірка (для розробників):

Якщо маєте потужний ПК з 8+ cores:

```bash
git clone https://github.com/aenu1/aps3e.git
cd aps3e
chmod +x build_poco_f6.sh
./build_poco_f6.sh
```

**Час збірки:**
- 8-core CPU: ~8-10 хвилин
- 16-core CPU: ~5-7 хвилин

---

**⚠️ Важливо:**  
ARMv9-A оптимізації працюють ТІЛЬКИ на:
- Snapdragon 8s Gen 3 (Poco F6)
- Snapdragon 8 Gen 2/Gen 3
- Dimensity 9200+/9300

Для старіших чіпів використовуйте ARMv8.2-A збірку.
