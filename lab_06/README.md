# Лабораторна робота №6

## Тема
C++ OpenCV. Обробка відео з камери у реальному часі.

## Опис
Програма зчитує відео з вебкамери, показує його у вікні та дозволяє перемикати режими обробки кадрів за допомогою клавіатури. Проєкт має модульну структуру: окремі класи відповідають за камеру, обробку клавіш, обробку кадру та відображення результату.

## Структура проєкту
```text
.
├── CMakeLists.txt
├── include
│   ├── CameraProvider.hpp
│   ├── Display.hpp
│   ├── FrameProcessor.hpp
│   └── KeyProcessor.hpp
├── src
│   ├── CameraProvider.cpp
│   ├── Display.cpp
│   ├── FrameProcessor.cpp
│   ├── KeyProcessor.cpp
│   └── main.cpp
├── preinstall.sh
├── build.sh
├── run.sh
├── .gitignore
└── README.md
```

## Вимоги
- Linux / Ubuntu
- CMake 3.10+
- g++
- OpenCV
- Вебкамера

## Встановлення залежностей
```bash
chmod +x preinstall.sh build.sh run.sh
./preinstall.sh
```

## Білд
```bash
./build.sh
```

## Запуск
```bash
./run.sh
```

## Керування
- `0` — оригінальне зображення
- `1` — інверсія кольорів
- `2` — відтінки сірого
- `3` — Gaussian blur
- `4` — Canny filter
- `5` — Sobel filter
- `6` — бінаризація
- `7` — cartoon effect
- `+` / `-` — зміна сили розмиття
- `S` — зберегти поточний кадр
- `Q` або `ESC` — вихід

## Особливості
Програма показує поточний режим, кількість оброблених кадрів та FPS. Збережені кадри записуються у форматі `frame_<number>.png`.
