# fanControl
Arduino-based 3 temperature channel 3 speed fan controller for car
Трехскоростной контроллер вентиляторов охлаждения двигателя с тремя независимыми каналами температуры.

I want to replace engine fan clutch with two electric fans. System should have 3 "speeds" (one fan on, two fans in serial, 2 fans in parallel). and 3 separate and independant temperature channels: engine coolant temp, AT oil temp, AC condenser temp.

I use 1-wire tempperature sensors ds18b20, arduino as a main control board, 4chnl (3 used 1 spare) )solid state relay shiled for operating 3 electro-mechanical 12v relays, two electric fans, OLED display and rotary encoder and a couple of buttons for user interface module. With this module one can see current temperatures, fan modes and set 3 activation temperatures, hysteresis and warning threshold temperature for each channel. Also one can manually turn off all fans (useful for fords or deep enought mud pits).

If you for some reason are intertsting in this project just let me know. I am too lazy to upload complete fotos/docs/schematics/mech-CAD files. Only sorce code for the sake of backup :)



Я хочу заменить вискомуфту на двигателе на два электрических вентилятора. Они должны иметь три "скорости" (один вентиялтор, два последовательно, два параллельно). При этом должно быть три незавимых канала измерения температуры: температура двигателя, температура маслорадиатора АКПП, температура радиатора кондиционера.

И использую цифровые датчики температуры ds18b20 с интрефейсом 1-wire. Управление делается на основе arduino, 4-канальной платы твердотельных реле (используется 3 канала, 1 запасной), трех 12-вольтовых электро-механических реле, котоыре подключают два вентилятора. Из OLED-экрана, энкодера-крутилки и пары кнопок собран интерфейсный блок, через который можно настраивать три температуры срабатывания, гистерезис и критическую температуру для включания сигнализации о перегреве. Так же есть кнопка принудительного отключения вентиляторов. Это полезно при преодолении бродов и глбоких луж.

Если тебе вдруг почему-то заинтересовали подробности этого проекта, напиши мне. Тогда я преодолею лень и выложу схемы, чертежи-модели, фото и прочую инфу, а не только исходный код на этот бэкап-сервер.
