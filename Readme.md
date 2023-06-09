# ИДЗ 2

## Вариант 17
Задача о нелюдимых садовниках. Имеется пустой участок земли (двумерный массив) и план сада, разбитого на отдельные квадраты. От 10 до 30 процентов площади сада заняты прудами или камнями. То есть недоступны для ухаживания. Эти квадраты располагаются на плане произвольным образом. Ухаживание за садом выполняют два садовника, которые не хотят встречаться друг другом (то есть, одновременно появляться в одном и том же квадрате). Первый садовник начинает работу с верхнего левого угла сада и перемещается слева направо, сделав ряд, он спускается вниз и идет в обратном направлении, пропуская обработанные участки. Второй садовник начинает работу с нижнего правого угла сада и перемещается снизу вверх, сделав ряд, он перемещается влево и также идет в обратную сторону. Если садовник видит, что участок сада уже обработан другим садовником или является необрабатываемым, он идет дальше. Если по пути какой-то участок занят другим садовником, то садовник ожидает когда участок освободится, чтобы пройти дальше. Садовники должны работать одновременно со скоростями, определяемыми как параметры задачи. Прохождение через любой квадрат занимает некоторое время, которое задается константой, меньшей чем времена обработки и принимается за единицу времени. Создать приложение, моделирующее работу садовников. Каждого садовника представить отдельным процессом.

## Компиляция

`gcc ./*mark*/main.c utils.h utils.c -pthread -lrt`, где

`*mark*/main.c` - файл основной программы, выполненный на оценку 4 или 5 соответственно

`utils.h` - заголовочный файл

`utils.c` - файл с функциями, описывающими работу садовников.

## Заголовочный файл

Заголовочный файл содержит:
<ol>
<li>Подключение внешних библиотек
<li>Макросы идентификаторов
<li>Объявление структуры клетки поля
<li>Объвление глобальных переменных
</ol>

## Реализация задания

### На 4
Структура `cell` содержит текущее состояние (занято камнем или прудом, не обработано или указано каким работником обработано) и семафор, отвечающий за производимые в данной момент работы на клетке поля.
Перед выполнением основго кода программы записывается указатель на функцию, отрабатывающую сигнал, чтобы в будущем дочерний процесс не отрабатывал дважды сигнал `Ctrl+C`.
В начале работы программы выделяется разделяемая память `field`, в которой хранятся структуры `cell`, соответствующие каждой клетки поля. Затем генерируются название для семафоров и их создаение.
Далее генерируются клетки, которые будут заняты камнем или прудом.
После инициализации идет создание второго потока, передача прошлой функции отработки сигналов и запуск процесса работы на поле первым садовником.
Родительский поток запускает работу второго садовника и отвечает за обработку сигналов прерывания программы для освобождения ресурсов.

### На 5
Основным отличием программы на 5 является использование неименованных семафоров, способами инициализации и освобождения ресурсов. Для этого используются функции `sem_init` и `sem_destroy` соответственно.
