# Квадратка
*****
Эта программа умеет решать квадратные уравнения.

Была написана на летней школе по программированию от [МФТИ](https://mipt.ru).

*****
## Несколько примеров

У программы два основных способа ввода данных:

1. Через консоль:

![](docs/Example_console.png)

2. Или через агументы при запуске программы через командную строку

![](docs/Example_args.png)

Доступные аргументы будут описаны далее.


*****
## Компиляция и запуск

Зависимости:
1. `git` (опционально)
2. Компилятор C или C++, например `g++`
3. `make`
4. `doxygen` и `dot` (если хотите сгенерировать документацию)

Установка:
1. Склонируйте репозиторий любым удобным способом, например
    ```
    git clone https://github.com/orientiered/Kvadratka
    ```
    Также можно просто скачать его архивом
2. По умолчанию Makefile сконфигурирован для **Windows**

    Для компиляции под Linux смотри п.3

    Откройте **директорию** проекта в терминале и соберите его при помощи команды
    ```
    make
    ```
    По умолчанию сборка происходит с компилятором `g++`, но вы можете указать свой компилятор при помощи аргумента `CC=`
    ```
    make CC=clang
    ```
    Чтобы изменить имя исполняегомого файла, укажите его с помощью аргумента `NAME=`
    ```
    make CC=gcc NAME=kvadratka.exe
    ```
    Если вы хотите перекомпилировать проект, то сначала очистите объектные файлы командой
    ```
    make clean
    ```
    Также можно сгенерировать документацию при помощи doxygen, для этого установите doxygen и dot на вашу систему и добавьте их в PATH
    ```
    make doxygen
    ```
    По умолчанию сборка проходит в папке build, документация генерируется в папке doxDocs

    Это можно изменить при помощи аргументов `OBJDIR=` и `DOXYDIR=`

3. Linux <br>
    Зайдите в Makefile и замените значение переменной `SYSTEM` на `LINUX`:

    >SYSTEM=LINUX

    Теперь вы можете выполнять команды, описанные в п.2

Запуск:

Предположим, вы скомпилировали программу с именем kvadratka.exe

1. Windows, в директории, где лежит программа
    ```
    .\kvadratka
    ```
2. Linux, в директории, где лежит программа
    ```
    ./kvadratka.exe
    ```

*****

## Возможности:

В программу заложено два способа ввода коэффициентов уравнения: через консоль и с помощью аргументов командой строки.

Рассмотрим подробно ввод через консоль.

1. Коэффициенты нужно вводить в порядке a, b, c.
2. Можно писать любое количество коэффициентов на одной строке
3. Коэффициенты можно разделять пробелами, табами, переносами строк
4. Если программа не смогла интепретировать ввод как число, то она сообщит об этом и будет пытаться считать текущий коэффициент снова.
5. Поддерживается математическая нотация, а также nan и inf
6. Чтобы было очевидно, какие коэффициенты программа смогла считать, она выводит каждый считанный коэффициент

Затем программа печатает введённое уравнение в красивом формате, а также ответы на него.


