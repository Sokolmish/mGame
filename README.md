# mGame

**[README_EN](https://github.com/Sokolmish/mGame/README_EN.md)** - Unfortunately - currently not finished :(

Приветствуется любое участие в проекте, будь то реализация существующих issue, доработка кода, а также реализация всевозможных интересных вам фич в отдельных ветках с их вероятным включением в основной код.

## Структура кода

* [mGame.cpp](https://github.com/Sokolmish/mGame/blob/master/game/src/mGame.cpp) - точка входа, главный цикл
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
* GLM - **!!!НЕ УКАЗАН В CMAKE - надо указать!!!**
* FreeType - `apt install libfreetype6 libfreetype6-dev`

