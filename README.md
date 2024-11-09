# Универсальный драйвер для работы с матричной клавиатурой

Проект тестировался на железе, схема которого приведена в scheme.pdf.
Матричная клавиатура размером 3*4 подключена к плате BluePill с микроконтроллером STM32F103C8 (GPIOB).
Питание платы оcуществляется от USB.
Программирование и отладка осуществляется через ST-Link.
ST-Link должен иметь вывод SWO для вывода отладочной информации (printf()).

>Проект создан в IDE Keil v5.