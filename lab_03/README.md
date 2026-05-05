# Лабораторна робота №3 — Візуалізація даних

## Дані студента
- ПІБ: Кінаш Єгор
- Група: ФБ-44
- Дисципліна / курс: ЗПАД

## Опис завдання
У роботі використано датасет **Iris** з UCI Machine Learning Repository. Датасет відповідає вимогам:
- Dataset Characteristics: Multivariate
- Attribute Characteristics: Real
- Number of Attributes: 4 числові real-атрибути
- Has Missing Values: No

У ноутбуці виконано:
1. Завантаження датасету.
2. Data Cleaning.
3. Побудова графіків залежностей між числовими атрибутами.
4. Побудова гістограм.
5. Додаткові корисні графіки для аналізу датасету.

## Структура проєкту
- `lab3_visualization.ipynb` — основний Jupyter Notebook.
- `requirements.txt` — залежності Python.
- `.gitignore` — виключення службових файлів з репозиторію.
- `README.md` — опис проєкту та інструкція запуску.

## Як запустити
1. Створіть віртуальне середовище:
   ```bash
   python -m venv .venv
   ```
2. Активуйте його:
   ```bash
   # Windows
   .venv\Scripts\activate

   # macOS/Linux
   source .venv/bin/activate
   ```
3. Встановіть залежності:
   ```bash
   pip install -r requirements.txt
   ```
4. Запустіть Jupyter Notebook:
   ```bash
   jupyter notebook
   ```
5. Відкрийте файл `lab3_visualization.ipynb` і виконайте клітинки зверху вниз.

## Використаний датасет
UCI Iris Dataset: https://archive.ics.uci.edu/dataset/53/iris
