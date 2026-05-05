# Лабораторна робота №7

## Тема
Computer Vision та багатопотоковість у C++.

## Опис
Програма створена на основі попередньої роботи з OpenCV. Вона читає відео з вебкамери, а детекцію облич виконує в окремому потоці. Основний потік не очікує завершення інференсу, тому відео залишається плавним, а рамки навколо знайдених облич оновлюються асинхронно.

## Архітектура
- `CameraProvider` — відповідає за отримання кадрів з камери.
- `FaceDetector` — містить `cv::dnn::Net`, `std::thread`, `std::mutex` і `std::atomic<bool>`. Клас приймає кадри з основного потоку, виконує `net.forward()` у фоні та зберігає координати знайдених облич.
- `FrameProcessor` — малює рамки, FPS, кількість кадрів та підказки керування.

## Структура
```text
.
├── CMakeLists.txt
├── include
│   ├── CameraProvider.hpp
│   ├── FaceDetector.hpp
│   └── FrameProcessor.hpp
├── src
│   ├── CameraProvider.cpp
│   ├── FaceDetector.cpp
│   ├── FrameProcessor.cpp
│   └── main.cpp
├── models
├── preinstall.sh
├── build.sh
├── run.sh
├── .gitignore
└── README.md
```

## Вимоги
- Ubuntu/Linux
- OpenCV з модулем `dnn`
- CMake
- g++
- make
- wget
- вебкамера

## Підготовка
```bash
chmod +x preinstall.sh build.sh run.sh
./preinstall.sh
```

Скрипт встановлює залежності та завантажує два файли моделі:
- `models/deploy.prototxt`
- `models/res10_300x300_ssd_iter_140000.caffemodel`

## Білд
```bash
./build.sh
```

## Запуск
```bash
./run.sh
```

## Керування
- `Q` або `ESC` — завершити програму.
- `S` — зберегти поточний кадр.

## Очікуваний результат
У вікні показується відео з камери. Навколо знайдених облич малюються прямокутники. Детектор працює у фоновому потоці, тому відео не блокується під час обробки нейромережею.
