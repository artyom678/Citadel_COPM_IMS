Программа gzprocessor выполняет архивацию или распаковку одного файла. В качестве
алгоритма сжатия используется deflate, реализованный в библиотеке zlib. Важно отметить,
что фактически файл, сжатый нашей программой, имеет формат gzip (т.е. его можно распаковать при
помощи утилиты gunzip, а любой файл, сжатый утилитой gzip, можно распаковать при помощи
gzprocessor). Поэтому рекомендуется сохранять сжатый файл с расширением .gz

Установка на Linux (пример для дистрибутива Ubuntu):  
0) для начала убедитесь, что на вашей системе уже установлены:
- компилятор, поддерживающий C++17 (например, g++ 7 или выше)
- CMake (версия 3.28.3 или выше)
- git 
Переходим в терминал, выполняем:
1) sudo apt install build-essential cmake git zlib1g-dev (установка библиотеки zlib)
2) git clone https://github.com/artyom678/Citadel_COPM_IMS.git (клонируем репозитрорий)
   cd Citadel_COPM_IMS (переходим в директорию проекта)
3) mkdir build && cd build (создаём директорию для сборки и переходим в неё)
4) cmake .. (запускаем cmake для генерации make-файла)
5) cmake --build
Готово, исполняемый файл gzprocessor будет находться в директории build.


Установка на Windows:
0) для начала вам необходимо убедиться, что на вашей системе уже установлены:
- Visual Studio 2022 CE с компонентой "desktop development with C++"
- CMake (версия 3.28.3 или новее)
- git
1) установим vcpkg (если он у вас уже установлен, то этот шаг можно пропустить):
- заходим в командную строку
- выполняем git clone https://github.com/microsoft/vcpkg.git (клонируем репозиторий)
- выполянем cd vcpkg
- выполняем .\bootstrap-vcpkg.bat
- выполняем .\vcpkg.exe integrate install (для интерграции с CMake и Visual Studio)
2) устанавливаем библиотеку zlib:
- выполянем .\vcpkg.exe install zlib:x64-windows (для 64-разрядной архитектуры)
3) Клонируем репозиторий проекта:
- выполняем git clone https://github.com/artyom678/Citadel_COPM_IMS.git (клонируем репозиторий)
- выполняем cd Citadel_COPM_IMS (переходим в директорию проекта)
- выполняем mkdir build && cd build (создаём директорию сборки и переходим в неё)
- выполняем cmake.. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
- выполняем cmake --build . --config Release
Готово, исполняемый файл gzprocessor.exe должен находиться в директории build/Release


Использование:  
./gzprocessor -c|-d <input_file_path> <output_file_path>
 - используйте флаг -c для архивации
 - используйте флаг -d для распаковки
 - <input_file_path> - путь к источнику
 - <ouput_file_path> - путь к файлу, в который будет помещён результат работы программы

Чтобы посмотреть инструкцию воспользуйтесь флагом --help (./gzprocessor --help).






