# ASKON_TESTOVOE
Test Task For Askon

## Описание
Консольное приложение на C++, которое проверяет авторизацию по публичному ключу. Приватный ключ зашит в коде.

## Сборка и запуск

### Требования
- CMake 3.20+
- Conan 2.x
- Компилятор: GCC (MinGW на Windows) или Clang/GCC на Linux

### Шаги сборки
1. Установите зависимости через Conan:
   ```
   conan install . --build=missing
   ```
   На Windows с MinGW:
   ```
   conan install . --profile mingw --build=missing
   ```

2. Сгенерируйте файлы сборки:
   ```
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
   ```

3. Соберите проект:
   ```
   cmake --build build
   ```

4. Запустите тесты:
   ```
   cd build && ctest --output-on-failure
   ```

### Запуск программы
```
./build/auth_app "-----BEGIN PUBLIC KEY-----\n...\n-----END PUBLIC KEY-----"
```

Вывод: "Authorization SUCCESS" или "Authorization FAILED"
