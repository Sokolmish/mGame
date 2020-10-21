# mGame

**[README_EN](https://github.com/Sokolmish/mGame/README_EN.md)** - Unfortunately - currently not finished :(

Приветствуется любое участие в проекте, будь то реализация существующих issue, доработка кода, а также реализация всевозможных интересных вам фич в отдельных ветках с их вероятным включением в основной код.

## Структура кода

* [mGame.cpp](https://github.com/Sokolmish/mGame/blob/master/game/src/mGame.cpp) - точка входа
* [MaimMachine](https://github.com/Sokolmish/mGame/blob/master/game/include/mainMachine.hpp) - главный цикл
* [font](https://github.com/Sokolmish/mGame/blob/master/game/include/font.h) - отрисовка текста, инициализируется файлом *.ttf (класть в [fonts](https://github.com/Sokolmish/mGame/tree/master/game/fonts)) и жалаемым размером глифа (если параметр равен нулю - значение будет вычислено на основе известного параметра)
* [image](https://github.com/Sokolmish/mGame/blob/master/game/include/image.h) - загружает изображения используя библиотеку [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [inputEvents](https://github.com/Sokolmish/mGame/blob/master/game/include/inputEvents.h) - обработка пользовательского ввода
* [param.h](https://github.com/Sokolmish/mGame/blob/master/game/include/param.h) - разные глобальные дефайны
* [util](https://github.com/Sokolmish/mGame/blob/master/game/include/util.h) - универсальные функции
  * formatFloat - форматирует float согласно шаблону и выозвращает строку (пример шаблона: "%.2f")
  * Оператор вывода в поток (operator<<) для glm::vec3
  * fractf - получение дробной части float (обёртка над modf)

## Используемые библиотеки

* OpenGL
* GLFW3
* GLEW - хедер находится в папке [include](https://github.com/Sokolmish/mGame/tree/master/game/include), но необходимо установить исходники
* GLM
* FreeType

```bash
sudo apt install libopengl0 libglfw3 libglfw3-dev libglm-dev libglew-dev libfreetype6 libfreetype6-dev
```

## Особенности кода

### Шейдеры

Файлы шейдеров хранятся в директории [shaders](https://github.com/Sokolmish/mGame/tree/master/game/shaders). Для каждого шейдера его название должна быть добавлена запись в файл [list.txt](https://github.com/Sokolmish/mGame/blob/master/game/shaders/list.txt) и создана директория с таким же названием. Запись шейдера фрамируется по шаблону `<shaderName>:<mask>`, где `mask` - число, получаемое сложением:

0. +1 - если содержит вершинный шейдер с именем `vert.glsl`
1. +2 - если содержит геометрический шейдер с именем `geom.glsl`
2. +4 - если содержит фрагментный шейдер с именем `frag.glsl`
3. +8 - если содержит шейдер тесселяции с именем `tess.glsl` (сейчас не поддерживается)

Каждый шейдер загружается одним из двух путей:
```cpp
// С указанием полного пути к файлу каждого шейдера
Shader s1("/root/vert.glsl", "/root/frag.glsl", "/root/geom.glsl");
Shader s2("/root/vert.glsl", "/root/frag.glsl"); // Можно создавать без геометрического шейдера

// С указанием имени шейдера (рекомендуемый метод)
Shader::shaderDirectory = "./game/shaders/"; // Перед использованием нужно указать директорию, для поиска шейдеров (выполняется один раз)
Shader s3 = Shader::loadShader("shaderName");
```
