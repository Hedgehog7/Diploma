# MergerPluginPlugin

Описание подпроекта MergerPlugin (текущая поддиректория с файлами - `src/plugins/Merger`).

Данный плагин является реализацией средств автоматизированного объекдинения темпоральных знаний.
В качестве внутреннего представления ПЗ используется библиотека `KnowledgeField`.

## Внутреннее устройство

Файл `mergerplugin.h` содержит описание внешнего класса необходимого для игтеграции с планировщиком.
Файл `mainwindow.h` содержит описание класса, отвечающейго за интерфейс.
Файл `merger_t` содержит описание класса, отвечающего за объединение фрагментов ПЗ. В конструктор передаются ссылки на объединяемые фрагменты ПЗ. После этапов объединения интервалов, событий и правил можно получить описание расширенной таблицы решений и таблицы мер схожести в формате `csv`.
Файл `tester_t.h` содержит описание класса, позволяющего создать тестовые фрагменты ПЗ.

## Ограничения на входные данные

Объединение возможно только для фрагментов ПЗ представленных в виде дампа ПЗ, пригодного для чтения библиотекой `KnowledgeField`.

## Процесс взаимодействия с ПО

* В главном окне программы необходимо указать файлы с дампов фрагментов ПЗ.
* В тестовом варианте доступна кнопка с заполнением файлов с дампами тестовыми фрагментами ПЗ.
* Загрузить фрагменты ПЗ.
* Объединить типы. Если кнопка `Объединить объекты` недоступна, то присутствуют конфлитные типы. Лог работы содержит в специальном файле (название можно задать в интерфейсе программы с помощью соответствующего поля).
* Объединить События.
* Показать расширенную таблицу решений (опционально). В результате нажатия на данную кнопку в соответствующий файл (название выбирается с помощью интерфейса) записывается описание расширенной таблицы решений в формате `csv`.
* Показать таблицу мер схожести (опционально). В результате нажатия на данную кнопку в соответствующий файл (название выбирается с помощью интерфейса) записывается описание таблицы мер схожести в формате `csv`.
* Объединить Интервалы. Возможно получить описание расширенной таблицы решений и талицы мер схожести.
* бъединить Правила. Возможно получить описание расширенной таблицы решений и талицы мер схожести.
* Сохранить результат. В результат в соответствующем файле хранится лог программы: пары событий, интервалов и правил, предназначенные для обработки в ручном режиме.
