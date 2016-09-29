
class RESOLVER{

	public:
		int
			resolverdecimal,
			resovlerdegrees,
			resolverpins[12];

	RESOLVER (int _resolver00, int _resolver01, int _resolver02, int _resolver03, int _resolver04, int _resolver05, int _resolver06, int _resolver07, int _resolver08, int _resolver09, int _resolver10, int _resolver11){
		
		resolverpins[ 0] = _resolver00;
		resolverpins[ 1] = _resolver01;
		resolverpins[ 2] = _resolver02;
		resolverpins[ 3] = _resolver03;
		resolverpins[ 4] = _resolver04;
		resolverpins[ 5] = _resolver05;
		resolverpins[ 6] = _resolver06;
		resolverpins[ 7] = _resolver07;
		resolverpins[ 8] = _resolver08;
		resolverpins[ 9] = _resolver09;
		resolverpins[10] = _resolver10;
		resolverpins[11] = _resolver11;

		for (int i = 0; i < 12; i ++){
			pinMode(resolverpins[i], INPUT_PULLUP);
		}
	}

	String readresolverdecimal(String _state){

		resolverdecimal = 0;

		for (int i = 0; i < 12; i++){
			
			if (_state.equals("L")){
				resolverdecimal |= (!digitalRead(resolverpins[i])) << i;
			}

			if (_state.equals("H")){
				resolverdecimal |= (digitalRead(resolverpins[i])) << i;
			}

		}

		String decimalstring = String(resolverdecimal);

		for (int i = 0; i = 4 - decimalstring.length(); i ++){
			decimalstring = "0" + decimalstring;
		}

		return decimalstring;
	
	}

	String readresovlerdegrees(String _state){
		
		readresolverdecimal (_state);

		resovlerdegrees = map(resolverdecimal, 0, 4096, 0, 3600);

		String degressstring = String(resovlerdegrees, DEC);

		for (int i = 0; i = 4 - degressstring.length(); i ++){
			degressstring = "0" + degressstring;
		}
		
		degressstring = degressstring.substring(0, 3) + "." + degressstring.substring(3, 4);

		return degressstring;
	}

};

class FREQUENC {
	
	public:
		int
			frequencypins[ 2],
			frequencyfs00[ 4] = {1, 1, 0, 0},
			frequencyfs01[ 4] = {1, 0, 1, 0},
			frequencylist[ 4] = {10, 12, 15, 20};
			

	FREQUENC (int _frequency00, int _frequency01){
		
		frequencypins[ 0] = _frequency00;
		frequencypins[ 1] = _frequency01;

		for (int i = 0; i < 2; i ++){
			
			pinMode(frequencypins[i], OUTPUT);
			digitalWrite(frequencypins[i], LOW);

		}
	}

	String selectfrequency(int _select){
		
		digitalWrite(frequencypins[ 0], frequencyfs00[_select]);
		digitalWrite(frequencypins[ 1], frequencyfs01[_select]);
	
		String frequencystring = String(frequencylist[_select]) + " kHz";
		
		return frequencystring;

	}

};

class KEYBOARD{

	public:
		int
			analogvalue,
			analoginput,
			analogoffset,
			keypress,
			keylist[ 5];


	KEYBOARD (int _analoginput, int _analogoffset, int _keyselect, int _keyleft, int _keyup, int _keydown, int _keyright){
		
		analoginput		= _analoginput;
		analogoffset	= _analogoffset;
		
		keylist[ 0]		= _keyselect;
		keylist[ 1]		= _keyleft;
		keylist[ 2]		= _keyup;
		keylist[ 3]		= _keydown;
		keylist[ 4]		= _keyright;

	}

	int readKey (){
		
		analogvalue = analogRead(analoginput);

		keypress = 0;

		for (int i = 0; i < 5; i ++){
			if (analogvalue >= (keylist[i] - analogoffset) && analogvalue <= (keylist[i] + analogoffset)){
				keypress = i + 1;
			}
		}

		return keypress;

	}

};

#include "LiquidCrystal.h"

RESOLVER deg (31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53);
// DEG pins: ^00 ^01 ^02 ^03 ^04 ^05 ^06 ^07 ^08 ^09 ^10 ^11

FREQUENC fre (22, 24);
// FRE pins: FS0 FS1

KEYBOARD key (A0, 3, 726, 485, 133, 311, 0);
// KEY sett: Analog, Offset, Select, Left, Up, Dow, Right

LiquidCrystal lcd(8, 14, 9, 4, 5, 6, 7);
// LCD pins: RS  RW  EN  D4 D5 D6 D7

//VARIABLES
unsigned long 
	selectionmillis = 0;

int 
	selectionstate;

void setup() {
    	
	lcd.begin(16, 2);

}

void loop() {
	
	lcd.clear();
	lcd.setCursor( 0, 0);
	lcd.print("Resolver");
	
	lcd.setCursor(10, 0);
	lcd.print(fre.selectfrequency(selectionstate));

	lcd.setCursor( 0, 1);
	lcd.print(deg.readresolverdecimal("H"));
	
	lcd.setCursor( 10, 1);
	lcd.print(deg.readresovlerdegrees("H"));

	lcd.setCursor(15, 1);
	lcd.print((char)223);

	frequencyselect();

	delay(100);

}

void frequencyselect(){

	int reading = key.readKey();

	if (reading == 1){
		if (selectionmillis < millis()){
			
			selectionmillis = millis() + 200;

			if (selectionstate < 3){
				selectionstate ++;
			}
			else{
				selectionstate = 0;
			}
			
		}
	}

}