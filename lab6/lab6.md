# Пункт 2

```C
#include <iostm8s207.h>

void main(void) {
    // Настройка порта G
    PG_DDR |= (1 << 0);  // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0);  // Двухтактный выход
    PG_CR2 |= (1 << 0);  // Частота 10 МГц

    // Настройка таймера TIM1
    TIM1_PSCRH = 0x00;  // Предварительный делитель (деление на 1)
    TIM1_PSCRL = 0x00;  // Частота таймера = 2 МГц
    TIM1_ARRH = 0x03;   // Регистр автоперезагрузки (старший байт)
    TIM1_ARRL = 0xE8;   // Регистр автоперезагрузки (младший байт, 1000 в десятичной системе)
    TIM1_CR1 |= (1 << 0);  // Включаем таймер

    while (1) {
        // Ждем, пока счетчик таймера не достигнет 128
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) <= 0x80) {
            PG_ODR |= (1 << 0);  // Устанавливаем логическую «1» на порт G
        }
        // Ждем, пока счетчик таймера не станет больше 128
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) > 0x80) {
            PG_ODR &= ~(1 << 0);  // Устанавливаем логический «0» на порт G
        }
    }
}
```

# Пункт 3

## Характеристики полученного сигнала

Частота реального сигнала - 2 кГц, длительность импульса 60-65 мкс.

## Расчёт теоретических значений

Таймер настроен на частоту 2 МГц (предварительный делитель 1). Регистр автоперезагрузки (ARR) настроен на 1000 (0x03E8).

$$T = \frac{1}{f} = \frac{1}{2 \cdot 10^6 \text{ Гц}} = 0.5 \text{ мкс}$$

Условие внутри бесконечного цикла задаёт 128 тактов для логической "1" и оставшиеся (1000 - 128) = 872 такта для логической "0".
   
- Длительность "1" = 128 * 0.5 мкс = 64 мкс.
- Длительность "0" = 872 * 0.5 мкс = 436 мкс.
- Общий период = (Длительность "1" + Длительность "0") = 64 мкс + 436 мкс = 500 мкс.
- Частота сигнала = 1 / Общий период = 1 / (500 * 10^-6) = 2000 Гц = 2 кГц.

## Вывод

Теоретическая частота, рассчитанная по приведенным выше данным, составляет 2 кГц, что совпадает с частотой, зарегистрированной на осциллографе (2 кГц).

# Пункт 4

```C
#include <iostm8s207.h>

void main(void) {
    // Настройка порта G
    PG_DDR |= (1 << 0);  // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0);  // Двухтактный выход
    PG_CR2 |= (1 << 0);  // Частота 10 МГц

    // Настройка таймера TIM1
    TIM1_PSCRH = 0x00;  // Предварительный делитель (деление на 1)
    TIM1_PSCRL = 0x00;  // Частота таймера = 2 МГц
    TIM1_ARRH = 0x03;   // Регистр автоперезагрузки (старший байт)
    TIM1_ARRL = 0xE8;   // Регистр автоперезагрузки (младший байт, 1000 в десятичной системе)
    TIM1_CR1 |= (1 << 0);  // Включаем таймер

    while (1) {
        // Ждем, пока счетчик таймера не достигнет 64
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) <= 0x40) {
            PG_ODR |= (1 << 0);  // Устанавливаем логическую «1» на порт G
        }
        // Ждем, пока счетчик таймера не станет больше 64
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) > 0x40) {
            PG_ODR &= ~(1 << 0);  // Устанавливаем логический «0» на порт G
        }
    }
}
```

# Пункт 5

Чтобы увеличить период сигнала в 2 раза, нужно уменьшить частоту тактирования таймера в 2 раза. Это можно сделать, увеличив значение регистра предварительного делителя (`TIM1_PSCR`).

$$F_{TIM1} = \frac{F_{MCU}}{(PSCR + 1)}$$
где:
- $F_{MCU}$ — частота тактирования микроконтроллера (2 МГц по умолчанию).
- $PSCR$ — значение предварительного делителя.

Увеличим значение предварительного делителя с 0 до 1. Это уменьшит частоту таймера в 2 раза:
$$F_{TIM1} = \frac{2 \text{ МГц}}{(1 + 1)} = 1 \text{ МГц}$$

```C
#include <iostm8s207.h>

void main(void) {
    // Настройка порта G
    PG_DDR |= (1 << 0);  // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0);  // Двухтактный выход
    PG_CR2 |= (1 << 0);  // Частота 10 МГц

    // Настройка таймера TIM1
    TIM1_PSCRH = 0x00;  // Старший байт предварительного делителя
    TIM1_PSCRL = 0x01;  // Младший байт предварительного делителя (деление на 2)
    TIM1_ARRH = 0x03;   // Регистр автоперезагрузки (старший байт)
    TIM1_ARRL = 0xE8;   // Регистр автоперезагрузки (младший байт, 1000 в десятичной системе)
    TIM1_CR1 |= (1 << 0);  // Включаем таймер

    while (1) {
        // Ждем, пока счетчик таймера не достигнет 128
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) <= 0x80) {
            PG_ODR |= (1 << 0);  // Устанавливаем логическую «1» на порт G
        }
        // Ждем, пока счетчик таймера не станет больше 128
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) > 0x80) {
            PG_ODR &= ~(1 << 0);  // Устанавливаем логический «0» на порт G
        }
    }
}
```

## Ответ на вопрос

Длительность импульса увеличилась в 2 раза, так как частота таймера уменьшилась в 2 раза (за счет увеличения предварительного делителя). Это привело к увеличению времени счета таймера.

# Пункт 6

Чтобы уменьшить период сигнала в 2 раза, нужно уменьшить значение регистра автоперезагрузки (TIM1_ARR) в 2 раза. Это приведет к тому, что таймер будет считать до меньшего значения, и период сигнала уменьшится. Уменьшим значение регистра автоперезагрузки с 1000 до 500.

```C
#include <iostm8s207.h>

void main(void) {
    // Настройка порта G
    PG_DDR |= (1 << 0);  // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0);  // Двухтактный выход
    PG_CR2 |= (1 << 0);  // Частота 10 МГц

    // Настройка таймера TIM1
    TIM1_PSCRH = 0x00;  // Предварительный делитель (деление на 1)
    TIM1_PSCRL = 0x00;  // Частота таймера = 2 МГц
    TIM1_ARRH = 0x01;   // Регистр автоперезагрузки (старший байт)
    TIM1_ARRL = 0xF4;   // Регистр автоперезагрузки (младший байт, 500 в десятичной системе)
    TIM1_CR1 |= (1 << 0);  // Включаем таймер

    while (1) {
        // Ждем, пока счетчик таймера не достигнет 128
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) <= 0x80) {
            PG_ODR |= (1 << 0);  // Устанавливаем логическую «1» на порт G
        }
        // Ждем, пока счетчик таймера не станет больше 128
        while ((TIM1_CNTRH << 8 | TIM1_CNTRL) > 0x80) {
            PG_ODR &= ~(1 << 0);  // Устанавливаем логический «0» на порт G
        }
    }
}
```

## Ответ на вопрос

Длительность импульса уменьшилась в 2 раза, так как значение регистра автоперезагрузки уменьшилось в 2 раза. Это привело к уменьшению времени счета таймера.

# Пункт 7

Для формирования импульсов длительностью 20 мс нужно настроить таймер так, чтобы он переполнялся каждые 20 мс и использовать флаг переполнения (`UIF`) для переключения состояния порта G.

### Расчет значения для регистра автоперезагрузки (`TIM1_ARR`)

- Частота тактирования таймера: 2 МГц (по умолчанию).
- Период одного такта таймера: 0,5 мкс
- Для формирования импульса длительностью 20 мс:
$$\text{Количество тактов} = \frac{20 \text{ мс}}{0.5 \text{ мкс}} = 40000$$
- Значение для регистра автоперезагрузки:
$$\text{TIM1\_ARR} = 40000 - 1 = 39999 \quad (\text{так как счет начинается с 0})$$
- В шестнадцатеричной системе: 
$$39999_{10} = \text{0x9C3F}$$

```C
#include <iostm8s207.h>

#define IS_TIMER_OVERFLOW() (TIM1_SR1 & (1 << 0))

void main(void) {
    // Настройка порта G
    PG_DDR |= (1 << 0);  // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0);  // Двухтактный выход
    PG_CR2 |= (1 << 0);  // Частота 10 МГц

    // Настройка таймера TIM1
    TIM1_PSCRH = 0x00;  // Предварительный делитель (деление на 1)
    TIM1_PSCRL = 0x00;  // Частота таймера = 2 МГц
    TIM1_ARRH = 0x9C;   // Регистр автоперезагрузки (старший байт)
    TIM1_ARRL = 0x3F;   // Регистр автоперезагрузки (младший байт) 39999 в десятичной системе
    TIM1_CR1 |= (1 << 0);  // Включаем таймер

    while (1) {
        // Ждем переполнения таймера (флаг UIF)
        if (IS_TIMER_OVERFLOW()) {
            TIM1_SR1 &= ~(1 << 0);
            PG_ODR ^= (1 << 0);
        }
    }
}
```

# Пункт 8

Для создания задержки в микросекундах будем использовать таймер TIM1. Учитывая, что частота тактирования таймера составляет 2 МГц (период одного такта — 0,5 мкс), мы можем настроить таймер для отсчета нужного количества микросекунд.

```C
#include <iostm8s207.h>
#include <stdint.h>

#define PW 500
#define NW 200
#define IS_TIMER_OVERFLOW() (TIM1_SR1 & (1 << 0))

/**
 * @brief Задержка в микросекундах с использованием таймера TIM1.
 * 
 * @param a Количество микросекунд для задержки.
 * @note Эта функция имеет оверхед около 30 мкс.
 */
void delay_us(uint16_t a)
{
    --a;                    // Уменьшаем значение на 1 для корректной работы
    TIM1_CR1 |= 0x04;       // Активируем предварительную загрузку (ARPE)
    TIM1_PSCRH = 0x00;      // Устанавливаем старший байт делителя
    TIM1_PSCRL = 0x01;      // Устанавливаем младший байт делителя
    TIM1_EGR |= 0x01;       // Генерация обновления
    TIM1_ARRH = a >> 8;     // Загружаем старший байт регистра автоперезагрузки
    TIM1_ARRL = a & 0xFF;   // Загружаем младший байт регистра автоперезагрузки
    TIM1_CR1 &= ~0x04;      // Выключаем ARPE
    TIM1_CR1 |= 0x09;       // Включаем таймер в режиме one-pulse mode
    while (!IS_TIMER_OVERFLOW()) 
        // Ждем переполнения таймера
        ;            
    TIM1_SR1 = 0x00;        // Сбрасываем флаг обновления
}

void main(void)
{
    // Настройка порта G
    PG_DDR |= (1 << 0); // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0); // Двухтактный выход
    PG_CR2 |= (1 << 0); // Частота 10 МГц

    while (1)
    {
        PG_ODR |= (1 << 0);
        delay_us(PW);
        PG_ODR &= ~(1 << 0);
        delay_us(NW);
    }
}
```

## Причины завышенной задержки на малых значениях

Реализованная функция `delay_us` использует таймер для аппаратного отсчёта, однако при коротких значениях задержки (например, 20 мкс) фиксированное дополнительное время (програмнный оверхед) становятся существенной частью общего времени. То есть, кроме запрошенного периода (`a` тактов), на выполнение функции уходит какое-то фиксированное время на:
- Запись в регистры управления и прескейлера  
- Генерацию события обновления (`TIM1EGR`)  
- Переключение битов, готовящих таймер к работе  
- Ожидание выставления флага `UIF`

При большой задержке этот «накладной» эффект незначителен, а при 20 мкс он может сравняться с основным временем задержки, что и приводит к измеренному значению около 46 мкс.


# Пункт 9

Для создания задержки в миллисекундах будем использовать ту же функцию delay_us, но с умножением на 1000. Это позволит создать задержку в миллисекундах, используя уже реализованную микросекундную задержку. Вместо 1000 передаём 970, чтобы компенсировать постоянную погрешность, полученную в предыдущем пункте.

```C
#include <iostm8s207.h>
#include <stdint.h>

#define PW 10
#define NW 200
#define IS_TIMER_OVERFLOW() (TIM1_SR1 & (1 << 0))

/**
 * @brief Задержка в микросекундах с использованием таймера TIM1.
 * 
 * @param us Количество микросекунд для задержки.
 * @note Эта функция имеет оверхед около 30 мкс.
 */
void delay_us(uint16_t us)
{
    --us;                   // Уменьшаем значение на 1 для корректной работы
    TIM1_CR1 |= 0x04;       // Активируем предварительную загрузку (ARPE)
    TIM1_PSCRH = 0x00;      // Устанавливаем старший байт делителя
    TIM1_PSCRL = 0x01;      // Устанавливаем младший байт делителя
    TIM1_EGR |= 0x01;       // Генерация обновления
    TIM1_ARRH = us >> 8;    // Загружаем старший байт регистра автоперезагрузки
    TIM1_ARRL = us & 0xFF;  // Загружаем младший байт регистра автоперезагрузки
    TIM1_CR1 &= ~0x04;      // Выключаем ARPE
    TIM1_CR1 |= 0x09;       // Включаем таймер в режиме one-pulse mode
    while (!IS_TIMER_OVERFLOW()) 
        // Ждем переполнения таймера
        ;            
    TIM1_SR1 = 0x00;        // Сбрасываем флаг обновления
}

/**
 * @brief Задержка в миллисекнудах с использованием таймера TIM1.
 * 
 * @param ms Количество миллисекунд для задержки.
 */
void delay_ms(uint16_t ms) {
    uint16_t i;
    for (i = 0; i < ms; i++) {
        delay_us(970);  // 1000 мкс = 1 мс; учитываем оверхед 30 мкс
    }
}

void main(void)
{
    // Настройка порта G
    PG_DDR |= (1 << 0); // Настройка нулевой линии порта G как выход
    PG_CR1 |= (1 << 0); // Двухтактный выход
    PG_CR2 |= (1 << 0); // Частота 10 МГц

    while (1)
    {
        PG_ODR |= (1 << 0);
        delay_ms(PW);
        PG_ODR &= ~(1 << 0);
        delay_ms(NW);
    }
}
```

# Пункт 10

```C
#include "main.h"

/**
 * @brief Обработчик прерывания таймера TIM1 (переполнение).
 *
 * @note Эта функция вызывается при переполнении таймера TIM1.
 *       Инвертирует нулевой бит порта G и сбрасывает флаг переполнения.
 */
@far @interrupt void TIM1_IRQHandler(void)
{
    // Инверсия состояния: если бит 0 установлен, сбрасываем его, иначе – устанавливаем.
    PG_ODR ^= (1 << 0);     // Инверсия нулевого пина порта G

    TIM1_SR1 &= ~(1 << 0);  // Очистка флага переполнения (UIF)
}

/**
 * @brief Инициализация таймера TIM1 для генерации прерываний с заданной частотой.
 *
 * @param period_us Период переполнения таймера в микросекундах.
 */
void tim1_init(uint16_t period_us)
{
    // Для 2 МГц тактирования, делитель 1, такт = 0,5 мкс
    // Для period_us микросекунд, нужно period_us / 0.5 = period_us * 2 такта.
    // Поэтому, устанавливаем ARR = period_us * 2 - 1.
    uint16_t arr_value = period_us * 2 - 1;

    TIM1_CR1 &= ~0x01;  // Выключаем таймер, чтобы безопасно настроить его
    TIM1_CR1 |= 0x04;   // Активируем предварительную загрузку (ARPE)
    TIM1_PSCRH = 0x00;  // Устанавливаем старший байт делителя
    TIM1_PSCRL = 0x00;  // Устанавливаем младший байт делителя (делитель 1)
    TIM1_ARRH = arr_value >> 8;
    TIM1_ARRL = arr_value & 0xFF;
    TIM1_EGR |= 0x01;       // Генерация обновления, чтобы применить новые значения ARR и PSC
    TIM1_IER |= (1 << 0);   // Разрешаем прерывание по переполнению (UIE)
    TIM1_CR1 &= ~0x04;      // Выключаем ARPE после настройки
    TIM1_CR1 |= 0x01;       // Включаем таймер
}

void main(void)
{
    // Настройка порта G: настраиваем вывод PG0 для индикации работы таймера.
    PG_DDR |= (1 << 0);  // PG0 - выход
    PG_CR1 |= (1 << 0);  // PG0 - двухтактный режим (push-pull)
    PG_CR2 |= (1 << 0);  // PG0 - максимальная скорость (10 МГц)

    // Инициализируем таймер TIM1 для генерации прерываний каждые 5 мс.
    tim1_init(5000);
    // Разрешаем глобальные прерывания (Run Interrupt Mode).
    _asm("rim");
}
```

# Пункт 11

```C
#include "main.h"

#define PWM_PERIOD 200              // Период программного ШИМ
volatile uint8_t pwm_counter = 0;   // Счётчик тактов ШИМ (0 .. PWM_PERIOD-1)
volatile uint8_t pwm_value   = 0;   // Текущее значение скважности (0 .. PWM_PERIOD)
volatile int8_t  direction   = 1;   // Направление изменения: +1 - увеличение, -1 - уменьшение

/**
 * @brief Обработчик прерывания таймера TIM1 (переполнение).
 *
 * @note Эта функция вызывается при переполнении таймера TIM1.
 *       Инвертирует нулевой бит порта G и сбрасывает флаг переполнения.
 */
@far @interrupt void TIM1_IRQHandler(void)
{
    pwm_counter++;  // Переход к следующему такту ШИМ
    if (pwm_counter >= PWM_PERIOD)
    {
        pwm_counter = 0;
        
        // Обновление значения скважности
        pwm_value += direction;
        if ((pwm_value == PWM_PERIOD) || (pwm_value == 0))
        {
            direction = -direction;
        }
    }
    
    // Генерация программного ШИМ на PG0
    if (pwm_counter < pwm_value)
    {
        PG_ODR = 0x01;
    }
    else
    {
        PG_ODR = 0x00;
    }
}

/**
 * @brief Инициализация таймера TIM1 для генерации прерываний с заданной частотой.
 *
 * @param period_us Период переполнения таймера в микросекундах.
 */
void tim1_init(uint32_t period_us)
{
    // Для 2 МГц тактирования, делитель 1, такт = 0,5 мкс
    // Для period_us микросекунд, нужно period_us / 0.5 = period_us * 2 такта.
    // Поэтому, устанавливаем ARR = period_us * 2 - 1.
    uint16_t arr_value = period_us * 2 - 1;

    TIM1_CR1 &= ~0x01;  // Выключаем таймер, чтобы безопасно настроить его
    TIM1_CR1 |= 0x04;   // Активируем предварительную загрузку (ARPE)
    TIM1_PSCRH = 0x00;  // Устанавливаем старший байт делителя
    TIM1_PSCRL = 0x00;  // Устанавливаем младший байт делителя (делитель 1)
    TIM1_ARRH = arr_value >> 8;
    TIM1_ARRL = arr_value & 0xFF;
    TIM1_EGR |= 0x01;       // Генерация обновления, чтобы применить новые значения ARR и PSC
    TIM1_IER |= (1 << 0);   // Разрешаем прерывание по переполнению (UIE)
    TIM1_CR1 &= ~0x04;      // Выключаем ARPE после настройки
    TIM1_CR1 |= 0x01;       // Включаем таймер
}

void main(void)
{
    // Настройка порта G: настраиваем вывод PG0 для работы со светодиодом.
    PG_DDR |= (1 << 0);  // PG0 - выход
    PG_CR1 |= (1 << 0);  // PG0 - двухтактный режим (push-pull)
    PG_CR2 |= (1 << 0);  // PG0 - максимальная скорость (10 МГц)

    // Инициализируем таймер TIM1 для генерации прерываний каждые 5 мс.
    tim1_init(5000);

    // Разрешаем глобальные прерывания (Run Interrupt Mode).
    _asm("rim");
}
```

# Пункт 12

```C

```

# Пункт 13

```C

```

# Пункт 14

```C

```

# Пункт 16

```C

```

# Пункт 17

```C

```