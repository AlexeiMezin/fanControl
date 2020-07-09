// Подключаем библиотеку iarduino_OLED_txt.
#include <iarduino_OLED_txt.h>           
// * Подключаем библиотеку для работы с аппаратной шиной I2C.
#include <Wire.h>
//   Подключаем библиотеку для работы с энкодером I2C-flash.
#include <iarduino_I2C_Encoder.h>

// Подключаем библиотеку для работы с реле
#include <iarduino_I2C_Relay.h>

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// EEPROM support
#include <EEPROM.h>

#include "fc_misc.h"

//адрес энкодера
iarduino_I2C_Encoder enc(0x0A);

// адрес реле
iarduino_I2C_Relay relay(0x09);

// define Screen
iarduino_OLED_txt SCR(0x3C);
// define font Medium Rus
extern uint8_t MediumFontRus[];

//define sensors IDs
//  ОЖ
DeviceAddress addr1 = { 0x28, 0x73, 0xE0, 0x5B, 0x0B, 0x00, 0x00, 0x88 };
// Конд
DeviceAddress addr2 = { 0x28, 0x3B, 0x52, 0x5C, 0x0B, 0x00, 0x00, 0x16 };
// АКПП
DeviceAddress addr3 = { 0x28, 0x12, 0x57, 0x5C, 0x0B, 0x00, 0x00, 0xE4 };

/*
// Временные устройства:
DeviceAddress addr1 = { 0x28, 0xDC, 0x68, 0x5C, 0x0B, 0x00, 0x00, 0x83 };
DeviceAddress addr2 = { 0x28, 0x3B, 0x52, 0x5C, 0x0B, 0x00, 0x00, 0x16 };
DeviceAddress addr3 = { 0x28, 0xB5, 0xA9, 0x5B, 0x0B, 0x00, 0x00, 0xBF };
*/


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


// main SET func
void setVal(byte id);
void setMode(byte id);

/* main MENU decroption */

const String wordlist[] = 
{
	/* 0 */		"ВНИМАНИЕ  ",
	/* 1 */ 	"ОСНОВНОЙ  ",
	/* 2 */		"НАСТ      ",
	/* 3 */		"ОЖ        ",
	/* 4 */		"Конд      ",
	/* 5 */		"АКПП      ",
	/* 6 */		"Т1        ",
	/* 7 */		"Т2        ",
	/* 8 */		"Т3        ",
	/* 9 */		"Гист      ",
	/* 10 */	"Сигн      ",
	/* 11 */	"Реж.раб   ",
	/* 12 */	"Авто      ",
	/* 13 */	"Скор 1    ",
	/* 14 */	"Скор 2    ",
	/* 15 */	"Скор 3    "
};

menuItem menu[] =
{
	/*			ID		pos		captN	txtN	Param	Up		Down	b1		b2		b3		func */
	/* 0 */		{0,		0,		0,		0,		255,	0,		0,		0,		0,		0,		NULL},		/* вентиляторы ОТКЛЮЧЕНЫ */
	/* 1 */		{1,		0,		1,		1,		255,	1,		1,		1,		1,		2,		NULL},		/* основной экран */
	/* 2 */		{2,		1,		2,		3,		255,	2,		3,		5,		1,		1,		NULL},		/* Настройка-ОЖ */
	/* 3 */		{3,		2,		2,		4,		255,	2,		4,		10,		1,		1,		NULL},		/* Настройка-Кондиционер */
	/* 4 */		{4,		3,		2,		5,		255,	3,		20,		15,		1,		1,		NULL},		/* Настройка-АКПП */
	/* 5 */		{5,		1,		3,		6,		0,		5,		6,		5,		2,		1,		setVal},	/* Настройка-ОЖ-Т1 */
	/* 6 */		{6,		2,		3,		7,		1,		5,		7,		6,		2,		1,		setVal},	/* Настройка-ОЖ-Т2 */
	/* 7 */		{7,		3,		3,		8,		2,		6,		8,		7,		2,		1,		setVal},	/* Настройка-ОЖ-Т3 */
	/* 8 */		{8,		4,		3,		9,		3,		7,		9,		8,		2,		1,		setVal},	/* Настройка-ОЖ-Гистерезис */
	/* 9 */		{9,		5,		3,		10,		4,		8,		9,		9,		2,		1,		setVal},	/* Настройка-ОЖ-Сигнал */
	/* 10 */	{10,	1,		4,		6,		5,		10,		11,		10,		3,		1,		setVal},	/* Настройка-Конд-Т1 */
	/* 11 */	{11,	2,		4,		7,		6,		10,		12,		11,		3,		1,		setVal},	/* Настройка-Конд-Т2 */
	/* 12 */	{12,	3,		4,		8,		7,		11,		13,		12,		3,		1,		setVal},	/* Настройка-Конд-Т3 */
	/* 13 */	{13,	4,		4,		9,		8,		12,		14,		13,		3,		1,		setVal},	/* Настройка-Конд-Гистерезис */
	/* 14 */	{14,	5,		4,		10,		9,		13,		14,		14,		3,		1,		setVal},	/* Настройка-Конд-Сигнал */
	/* 15 */	{15,	1,		5,		6,		10,		15,		16,		15,		4,		1,		setVal},	/* Настройка-АКПП-Т1 */
	/* 16 */	{16,	2,		5,		7,		11,		15,		17,		16,		4,		1,		setVal},	/* Настройка-АКПП-Т2 */
	/* 17 */	{17,	3,		5,		8,		12,		16,		18,		17,		4,		1,		setVal},	/* Настройка-АКПП-Т3 */
	/* 18 */	{18,	4,		5,		9,		13,		17,		19,		18,		4,		1,		setVal},	/* Настройка-АКПП-Гистерезис */
	/* 19 */	{19,	5,		5,		10,		14,		18,		19,		19,		4,		1,		setVal},	/* Настройка-АКПП-Сигнал */
	/* 20 */	{20,	4,		2,		11,		255,	4,		20,		21,		1,		1,		setMode},		/* Настройка-Режим работы */
	/* 21 */	{21,	1,		11,		12,		21,		21,		22,		21,		20,		1,		setMode},		/* Настройки-Режим работы-Авто */
	/* 22 */	{22,	2,		11,		13,		22,		21,		23,		22,		20,		1,		setMode},		/* Настройки-Режим работы-Скор1 */
	/* 23 */	{23,	3,		11,		14,		23,		22,		24,		23,		20,		1,		setMode},		/* Настройки-Режим работы-Скор2 */
	/* 24 */	{24,	4,		11,		15,		24,		23,		24,		24,		20,		1,		setMode}		/* Настройки-Режим работы-Скор3 */
};

void setMode(byte id)
{
	// режим "Авто"
	if(id == 21)
	{
		fanState = 0;
	}
	else
	{
		// режим ручной скорости
		fanState = id;		
	};
	fullRedraw = true;
	
	// и очистить кнопки, чтоб они из режима редактирования не попали в режим работы с меню
	b1 = b2 = b3 = false;
	up = 0;
	down = 0;
};

void setVal(byte id)
{
	Serial.print("Edit ID: ");
	Serial.println(id);
	bool quit = false;
	byte val = tempSettings[menu[id].param];
	
	dtostrf(val, 4, 0, s);
	SCR.print("<", FW*4, cursorScrPos);
	SCR.print(s);
	SCR.print(">");
	do
	{
		// читаем кнопки
		readButtons();
		if(up > 0)
		{
			// ограничим максимальный рост параметров числом 150
			if(val + up <= 150)
				val+= up;
			else
				val = 150;
		};
		if(down > 0)
		{
			// минимальное значение параметров 0, потому что температуры ниже нуля не интересны
			if(val - down >= 0)
				val-= down;
			else
				val = 0;
		};
		// перерисовка
		if((up > 0) | (down > 0))
		{
			dtostrf(val, 4, 0, s);
			SCR.print("<", FW*4, cursorScrPos);
			SCR.print(s);
			SCR.print(">");
		};

		if(b1)
		{
			// нажат ввод, подтвержение изменения параметра
			tempSettings[menu[id].param] = val;
			writeSettings(menu[id].param);
			quit = true;
		};
		if(b2 | b3)
		{
			// отмена
			quit = true;
		};
		
		if(!quit)
		{
			// чуть-чуть спим, чтоб не слишком быстро опрашивать крутилку
			delay(100);
		};
		
	}while(!quit);
	
	// после выхода из режима редактирования нужно перерисововать экран
	fullRedraw = true;

	// и очистить кнопки, чтоб они из режима редактирования не попали в режим работы с меню
	b1 = b2 = b3 = false;
	up = 0;
	down = 0;
}

// чтение температуры 
void getTemp()
{
	// запрашиваем температуру
	sensors.requestTemperatures();
	ch1_temp = sensors.getTempC(addr1);
	ch2_temp = sensors.getTempC(addr2);
	ch3_temp = sensors.getTempC(addr3);
	// временно установим фейковые температуры для третьего канала
	//float t;
	//t = random(-300, 1200);
	//t = random(-300, 800);
	//ch3_temp = t/10.0;
};

// чтение настроек из EEPROM
void readSettings(void)
{
	// тут надо будет дописать чтение из памяти
	// а пока забиваем статическими значениями

	/*
	// ОЖ
	tempSettings[0] = 30;
	tempSettings[1] = 32;
	tempSettings[2] = 34;
	tempSettings[3] = 1;
	tempSettings[4] = 35;

	// Кондиционер
	tempSettings[5] = 30;
	tempSettings[6] = 32;
	tempSettings[7] = 34;
	tempSettings[8] = 1;
	tempSettings[9] = 35;

	// АКПП
	tempSettings[10] = 95;
	tempSettings[11] = 100;
	tempSettings[12] = 105;
	tempSettings[13] = 2;
	tempSettings[14] = 110;
	*/
	// чтение из EEPROM
	byte i;
	for(i = 0; i < nChnl * nParam; i++)
	{
		tempSettings[i] = EEPROM.read(i);
		/*
		Serial.print("EEPROM read pos = ");
		Serial.print(i);
		Serial.print(" val = ");
		Serial.println(tempSettings[i]);
		*/
		
	};
	
};

// запись настроек в память
void writeSettings(byte i)
{
	// запись в память
	EEPROM.update(i, tempSettings[i]);
	/*
	Serial.print("Write memory, pos = ");
	Serial.print(i);
	Serial.print(", val = ");
	Serial.println(tempSettings[i]);
	*/
	
};

void redraw(void)
{
	byte n, nn, p, pp;
	switch(ID)
	{
		case 0:
			// WARNING screen
			if(fullRedraw)
			{
				SCR.invScr(true);
				SCR.clrScr();
				SCR.print("ВНИМАНИЕ", OLED_C, 3);
				SCR.print("ОТКЛЮЧЕНО!", OLED_C, 5);
				fullRedraw = false;
			}
			break;
		case 1: //main screen
			
    		//if(prevID != ID) //нужна полная перерисовка экрана
    		if(fullRedraw)
    		{	
    			SCR.invScr(false);
    			SCR.clrScr();
    			SCR.print("ОЖ  ", OLED_L, 3);
    			SCR.print("Конд", OLED_L, 5);
    			SCR.print("АКПП", OLED_L, 7);
    			fullRedraw = false;
    		}

   			switch(fanState)
   			{
   				// режим Авто, вентилятор выключен
   				case 0:
   					SCR.print("-------- А", OLED_L, 1);
   				break;
   				// режим Авто, вентилятор включен
   				case 1:
   					SCR.print("##       ", OLED_L, 1);
   					SCR.print(lastReason);
   				break;
   				case 2:
   					SCR.print("##-##    ", OLED_L, 1);
   					SCR.print(lastReason);
   				break;	
   				case 3:
   					SCR.print("##-##-## ", OLED_L, 1);
   					SCR.print(lastReason);
   				break;
   				// ручные режимы
   				case 22:
   					SCR.print("##       ", OLED_L, 1);
   					SCR.print("Р");
   				break;
   				case 23:
   					SCR.print("##-##    ", OLED_L, 1);
   					SCR.print("Р");
   				break;
   				case 24:
   					SCR.print("##-##-## ", OLED_L, 1);
   					SCR.print("Р");
   				break;
    				
    				
   			};
    		//SCR.print("##-##-## ", OLED_L, 1); // отображение скорости вентилятора

    		//SCR.print("##-##-## ", OLED_L, 1); // отображение скорости вентилятора
    		//SCR.print(1); // канал, последним включивший вентиллятор, 

    		dtostrf(ch1_temp, 5, 1, s);
    		SCR.print(s, FW*5, 3);

			dtostrf(ch2_temp, 5, 1, s);
    		SCR.print(s, FW*5, 5);

    		dtostrf(ch3_temp, 5, 1, s);
    		SCR.print(s, FW*5, 7);
			break;
		/*
		case 2:
		case 3:
		case 4:
		case 5: // Подменю 1
		*/
		default:

			if((prevID != ID) | fullRedraw)
			{
				if(fullRedraw)
				{
					SCR.invScr(false);
					SCR.clrScr();
					// заголовок
					//SCR.invText(true);
					SCR.print("-= ", OLED_L, 1);
					//SCR.print(wordlist[menu[ID].captN], OLED_C, 1);
					SCR.print(wordlist[menu[ID].captN]);
					SCR.print(" =-", OLED_R, 1);
					//SCR.invText(false);
					fullRedraw = false;
				};
				
				//SCR.print(wordlist[menu[ID].captN], OLED_C, 1);
				
				// позиционируем "окно"
				if(menuCurPos > WP+2)
					WP++;
				if(menuCurPos < WP)
					WP--;
				switch(menuCurPos - WP)
				{
					case 0: // верхний пункт меню
						cursorScrPos = 3;
						SCR.invText(true);
						SCR.print(wordlist[menu[ID].txtN], OLED_L, 3);
						if(menu[ID].param < 20)
						{
							dtostrf(tempSettings[menu[ID].param], 5, 0, s);
	    					SCR.print(s, FW*5, 3);
						}
						else
						{
							// рисуем строку "Авто" и текущий режим "авто", ставим галочку
							if((menu[ID].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 3);
							};
							if(menu[ID].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 3);
							};
						};
						SCR.invText(false);
						// next line
						n = menu[ID].down;
						SCR.print(wordlist[menu[n].txtN], OLED_L, 5);
						if(menu[n].param < 20)
						{
							dtostrf(tempSettings[menu[n].param], 5, 0, s);
							SCR.print(s, FW*5, 5);
						}
						else
						{
							if((menu[n].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 5);
							};
							if(menu[n].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 5);
							};
						};
						// next next
						nn = menu[n].down;
						SCR.print(wordlist[menu[nn].txtN], OLED_L, 7);
						if(menu[nn].param < 20)
						{
							dtostrf(tempSettings[menu[nn].param], 5, 0, s);
							SCR.print(s, FW*5, 7);
						}
						else
						{
							if((menu[nn].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 7);
							};
							if(menu[nn].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 7);
							};
						};
	
					break;
					case 1: //средний пункт
						p = menu[ID].up;
						SCR.print(wordlist[menu[p].txtN], OLED_L, 3);
						if(menu[p].param < 20)
						{
							dtostrf(tempSettings[menu[p].param], 5, 0, s);
							SCR.print(s, FW*5, 3);
						}
						else
						{
							if((menu[p].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 3);
							};
							if(menu[p].param == fanState)
							{ 
		    					SCR.print("# ", OLED_R, 3);
							};
						};

						cursorScrPos = 5;
						SCR.invText(true);
						SCR.print(wordlist[menu[ID].txtN], OLED_L, 5);
						if(menu[ID].param < 20)
						{
							dtostrf(tempSettings[menu[ID].param], 5, 0, s);
							SCR.print(s, FW*5, 5);
						}
						else
						{
							if((menu[ID].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 5);
							};
							if(menu[ID].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 5);
							};
						};
						SCR.invText(false);
	
						n = menu[ID].down;
						SCR.print(wordlist[menu[n].txtN], OLED_L, 7);
						if(menu[n].param < 20)
						{
							dtostrf(tempSettings[menu[n].param], 5, 0, s);
							SCR.print(s, FW*5, 7);
						}
						else
						{
							if((menu[n].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 7);
							};
							if(menu[n].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 7);
							};
						};
	
					break;
					case 2: // нижний пункт
						p = menu[ID].up;
						pp = menu[p].up;
						
						SCR.print(wordlist[menu[pp].txtN], OLED_L, 3);
						if(menu[pp].param < 20)
						{
							dtostrf(tempSettings[menu[pp].param], 5, 0, s);
							SCR.print(s, FW*5, 3);
						}
						else
						{
							if((menu[pp].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 3);
							};
							if(menu[pp].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 3);
							};
						};
	
						SCR.print(wordlist[menu[p].txtN], OLED_L, 5);
						if(menu[p].param < 20)
						{
							dtostrf(tempSettings[menu[p].param], 5, 0, s);
							SCR.print(s, FW*5, 5);
						}
						else
						{
							if((menu[p].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 5);
							};
							if(menu[p].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 5);
							};
						};
	
						cursorScrPos = 7;
						SCR.invText(true);
						SCR.print(wordlist[menu[ID].txtN], OLED_L, 7);
						if(menu[ID].param < 20)
						{
							dtostrf(tempSettings[menu[ID].param], 5, 0, s);
							SCR.print(s, FW*5, 7);
						}
						else
						{
							if((menu[ID].param == 21) & (fanState < 4))
							{
		    					SCR.print("# ", OLED_R, 7);
							};
							if(menu[ID].param == fanState)
							{
		    					SCR.print("# ", OLED_R, 7);
							};
						};
						SCR.invText(false);
					break;
				};
				
				
				break;
			};
	}
	prevID = ID;
}




void setup() {
	delay(500);                                   
    Serial.begin(9600);                          
    while(!Serial){;}                            

    // init encoder
    enc.begin(); 

    //init relay
    relay.begin();
    relay.reset();

    //init screen
    SCR.begin();
    SCR.setFont(MediumFontRus);
    FW = SCR.getFontWidth();


	// инициализируем пин кнопки отключения и управления пищалкой
	pinMode(offButtonPin, INPUT_PULLUP);
	//pinMode(PD3, INPUT_PULLUP);
	//offButton = false;
	pinMode(zummerPin, OUTPUT);
	digitalWrite(zummerPin, LOW);
	

	// Start up the library
	sensors.begin();

	// set the resolution to 9 bit per device
  	sensors.setResolution(addr1, TEMPERATURE_PRECISION);
  	sensors.setResolution(addr2, TEMPERATURE_PRECISION);
	
	// зачитывам параметры из памяти
	readSettings();

	// инициализируем начальные температуры
	ch1_temp = ch2_temp = ch3_temp = -127;
    // ready to go!
    //redraw();
    fullRedraw = true;
}

int t, t1;

void readButtons(void)
{
	b1 = b2 = b3 = false;
	// хак из-за идиотской системы распознавание нажатия кнопок
	t = t1;
	t1 = enc.getButton(KEY_TIME_PRESSED);
	if(enc.getButton(KEY_RELEASED))
	{
		if(t < 200)
			b1 = true;
		if((t >= 200)&(t < 1000))
			b2 = true;
		if(t >= 1000)
			b3 = true;
	};
	down = enc.getEncoder(ENC_TURN_LEFT);
	up = enc.getEncoder(ENC_TURN_RIGHT);
	
	// если нажата кнопка отключения!
	if(digitalRead(offButtonPin) == LOW)
	{
		// если до этого был другой режим, то переходим в режим "красной кнопки"
		if(ID != 0)
		{
			ID = 0;
			// сохраняем тот режим, который был до этого
			prevFanState = fanState;
			// переходим в режим отключенных вентиляторов
			fanState = 255;
			// врубаем пищалку
			zummer = true;
			//digitalWrite(zummerPin, HIGH);
			fullRedraw = true;
		};	
	}
	else
	{
		// если до этого был режим выключения
		if(ID == 0)
		{
			// экран в стартовый режим
			ID = 1;
			// возвращаем режим вентиляторов
			fanState = prevFanState;
			//выключаем пищалку
			zummer = false;
			fullRedraw = true;
		};
	};
};

// управление пищалкой

void sound()
{
	if(zummer)
		digitalWrite(zummerPin, HIGH);
	else
		digitalWrite(zummerPin, LOW);
}

// анализируем температуру с выставляем желаемую скорость вентиляторов 
void setFanState()
{
	// если в состоянии "выключено", то сразу выходим
	if(ID == 0)
	{
		fanState = 255;
		return;
	}

	// выключаем пищалку
	zummer = false;

	// анализируем какую скорость хочет каждый канал
	// канал 1
	// если температура выше критической, включить пищалку
	if(ch1_temp >= tempSettings[4])
	{
		zummer = true;
	};

	// если температура больше Т3 или больше критической, включить скорость 3
	if(ch1_temp > tempSettings[2])
	{
		ch1_w = 3;
	}
	else // температура меньше Т3
	{
		// если тепература больше Т2
		if(ch1_temp > tempSettings[1])
		{
			// если температура больше чем Т3-гистерезис, и при этом был запрошен третий режим
			if((ch1_temp > (tempSettings[2] - tempSettings[3])) & (ch1_w == 3))
				// сохранить третий режим
				ch1_w = 3;
			else
				// запросить второй режим
				ch1_w = 2;
		}
		else
		{
			// если температура больше Т1
			if(ch1_temp > tempSettings[0])
			{
				//если температура больше Т2-гистерезис, и была запрошена вторая скорость
				if((ch1_temp > (tempSettings[1] - tempSettings[3])) & (ch1_w == 2))
					// сохранить второй режим
					ch1_w = 2;
				else
					// запосить первый режим
					ch1_w = 1;
			}
			else
				// температура ниже Т1, но надо проверить гистерезис
				if((ch1_temp > (tempSettings[0] - tempSettings[3]))&(ch1_w == 1))
					ch1_w = 1;
				else
					// температура ниже Т1 и вышли за пределы гистерезиса, значит нужна нулевая скорость
					ch1_w = 0;
		}
	}

	// канал 2
	// если температура выше критической, включить пищалку
	if(ch2_temp >= tempSettings[9])
		zummer = true;

	// если температура больше Т3 или больше критической, включить скорость 3
	if(ch2_temp > tempSettings[7])
	{
		ch2_w = 3;
	}
	else // температура меньше Т3
	{
		// если тепература больше Т2
		if(ch2_temp > tempSettings[6])
		{
			// если температура больше чем Т3-гистерезис, и при этом был запрошен третий режим
			if((ch2_temp > (tempSettings[7] - tempSettings[8])) & (ch2_w == 3))
				// сохранить третий режим
				ch2_w = 3;
			else
				// запросить второй режим
				ch2_w = 2;
		}
		else
		{
			// если температура больше Т1
			if(ch2_temp > tempSettings[5])
			{
				//если температура больше Т2-гистерезис, и была запрошена вторая скорость
				if((ch2_temp > (tempSettings[6] - tempSettings[8])) & (ch2_w == 2))
					// сохранить второй режим
					ch2_w = 2;
				else
					// запосить первый режим
					ch2_w = 1;
			}
			else
				// температура ниже Т1, но надо проверить гистерезис
				if((ch2_temp > (tempSettings[5] - tempSettings[8]))&(ch2_w == 1))
					ch2_w = 1;
				else
					// температура ниже Т1 и вышли за пределы гистерезиса, значит нужна нулевая скорость
					ch2_w = 0;
		}
	}

	// канал 3
	// если температура выше критической, включить пищалку
	if(ch3_temp >= tempSettings[14])
		zummer = true;

	// если температура больше Т3 или больше критической, включить скорость 3
	if(ch3_temp > tempSettings[12])
	{
		ch3_w = 3;
	}
	else // температура меньше Т3
	{
		// если тепература больше Т2
		if(ch3_temp > tempSettings[11])
		{
			// если температура больше чем Т3-гистерезис, и при этом был запрошен третий режим
			if((ch3_temp > (tempSettings[12] - tempSettings[13])) & (ch3_w == 3))
				// сохранить третий режим
				ch3_w = 3;
			else
				// запросить второй режим
				ch3_w = 2;
		}
		else
		{
			// если температура больше Т1
			if(ch3_temp > tempSettings[10])
			{
				//если температура больше Т2-гистерезис, и была запрошена вторая скорость
				if((ch3_temp > (tempSettings[11] - tempSettings[13])) & (ch3_w == 2))
					// сохранить второй режим
					ch3_w = 2;
				else
					// запосить первый режим
					ch3_w = 1;
			}
			else
				// температура ниже Т1, но надо проверить гистерезис
				if((ch3_temp > (tempSettings[10] - tempSettings[13]))&(ch3_w == 1))
					ch3_w = 1;
				else
					// температура ниже Т1 и вышли за пределы гистерезиса, значит нужна нулевая скорость
					ch3_w = 0;
		}
	}

	// если ручной режим, то ничего не меняем независимо от температуры
	if(fanState > 21)
		return;

	// анализируем требования канала, выбираем максимальное требование, и выставлем причину включения и скорость включения
	// выходим из функции при первом максимальном значении, значения прочих каналов игнорируем
	if(ch1_w == 3)
	{
		fanState = 3;
		lastReason = 1;
		return;
	};
	if(ch2_w == 3)
	{
		fanState = 3;
		lastReason = 2;
		return;
	};
	if(ch3_w == 3)
	{
		fanState = 3;
		lastReason = 3;
		return;
	};
	if(ch1_w == 2)
	{
		fanState = 2;
		lastReason = 1;
		return;
	};
	if(ch2_w == 2)
	{
		fanState = 2;
		lastReason = 2;
		return;
	};
	if(ch3_w == 2)
	{
		fanState = 2;
		lastReason = 3;
		return;
	};
	if(ch1_w == 1)
	{
		fanState = 1;
		lastReason = 1;
		return;
	};
	if(ch2_w == 1)
	{
		fanState = 1;
		lastReason = 2;
		return;
	};
	if(ch3_w == 1)
	{
		fanState = 1;
		lastReason = 3;
		return;
	};
	// ни один канал не запросил работу вентилятора
	fanState = 0;
	lastReason = 0;
	return;
	
}

void fanOnOff()
{
	switch(fanState)
	{
		// выключены
		case 0:
		case 255:
			relay.digitalWrite(1, 0);
			relay.digitalWrite(2, 0);
			relay.digitalWrite(3, 0);
			break;
		// первая скорость
		case 1:
		case 22:
			relay.digitalWrite(1, 1);
			relay.digitalWrite(2, 0);
			relay.digitalWrite(3, 0);
			break;
		// вторая скорость
		case 2:
		case 23:
			relay.digitalWrite(1, 1);
			relay.digitalWrite(2, 1);
			relay.digitalWrite(3, 0);
			break;
		// третья скорость
		case 3:
		case 24:
			relay.digitalWrite(1, 1);
			relay.digitalWrite(2, 0);
			relay.digitalWrite(3, 1);
			break;
	};
		
}

void loop()
{	
	redraw();
	//prevID = ID;
	getTemp();
	setFanState();
	fanOnOff();
	sound();
	readButtons();	

	// сначала обрабтываем кнопки, потом крутилки
	if(b1 || b2 || b3)
	{
		prevID = ID;
		Serial.println("button pressed");
		if(b1) //enter
		{
			// если есть переход в подменю, то есть уход с текущего пункта
			if(ID != menu[ID].b1)
			{
				// при входе в подменю курсор на первой строке, "окно" с начала меню
				menuCurPos = 1;
				WP=1;
				ID = menu[ID].b1;
			}
			// провеяем, надо ли изменять параметры!
			else
			{
				// на всякий случай
				if(menu[ID].func != NULL)
				{
					Serial.println("Call editor!!!!");
					// вызываем редактор!
					menu[ID].func(ID);
				};
			};
		};
		if(b2) // переход на меню выше
		{
			// на всякий случай проверяем, что есть переход
			if(ID != menu[ID].b2)
			{
				ID = menu[ID].b2;
				// возвращаемся на тот пункт, из которого попали в подменю
				menuCurPos = menu[ID].pos;
				// устанавливаем "окно"
				// верхний пункт меню
				if(menu[ID].up == menu[ID].id)
					WP = 1;
				else
					// нижний пункт меню
					if(menu[ID].down == menu[ID].id)
						WP = menuCurPos - 2;
					else
						// где-то посередине меню
						WP = menuCurPos - 1;
			};
			// если перехода нет/переход сам на себя, то ничего и не делать
			//ID = menu[ID].b2;
		};
		if(b3) // total exit
		{
			ID = menu[ID].b3;
			menuCurPos = 1;
			WP = 1;
		};
		Serial.print("ID = ");
		Serial.println(ID);
		fullRedraw = true;
	}
	else
	{
		if(up > 0)
		{
			prevID = ID;
			// идем по меню вверх
			// на верхнем пункте идти некуда, поэтому позиция в меню не меняется
			if(ID != menu[ID].up)
				menuCurPos--;
			ID = menu[ID].up;	
			Serial.print("ID = ");
			Serial.println(ID);
		}
		else
			if(down > 0)
			{
				prevID = ID;
				if (ID != menu[ID].down)
					menuCurPos++;
				ID = menu[ID].down;
				Serial.print("ID = ");
				Serial.println(ID);
			}
	}
	
	
	delay(100);

}
