
class RESOLVER{

	public:
		int
			resolverdecimal,
			resolverlist[12];
			
		unsigned int 
			resovlerdegrees;

	RESOLVER (int _resolver00, int _resolver01, int _resolver02, int _resolver03, int _resolver04, int _resolver05, int _resolver06, int _resolver07, int _resolver08, int _resolver09, int _resolver10, int _resolver11){
		
		resolverlist[ 0] = _resolver00;
		resolverlist[ 1] = _resolver01;
		resolverlist[ 2] = _resolver02;
		resolverlist[ 3] = _resolver03;
		resolverlist[ 4] = _resolver04;
		resolverlist[ 5] = _resolver05;
		resolverlist[ 6] = _resolver06;
		resolverlist[ 7] = _resolver07;
		resolverlist[ 8] = _resolver08;
		resolverlist[ 9] = _resolver09;
		resolverlist[10] = _resolver10;
		resolverlist[11] = _resolver11;

		for (int i = 0; i < 12; i ++){
			pinMode(resolverlist[i], INPUT_PULLUP);
		}
	}

	String readresolverdecimal(String _state){

		resolverdecimal = 0;

		for (int i = 0; i < 12; i++){
			
			if (_state.equals("L")){
				resolverdecimal |= (!digitalRead(resolverlist[i])) << i;
			}

			if (_state.equals("H")){
				resolverdecimal |= (digitalRead(resolverlist[i])) << i;
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

		resovlerdegrees = map(resolverdecimal, 0, 4096, 0, 36000);

		String degressstring = String(resovlerdegrees, DEC);

		for (int i = 0; i = 5 - degressstring.length(); i ++){
			degressstring = "0" + degressstring;
		}
		
		degressstring = degressstring.substring(0, 3) + "." + degressstring.substring(3, 5);

		return degressstring;
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

KEYBOARD key (A0, 3, 726, 485, 133, 311, 0);
// KEY sett: Analog, Offset, Select, Left, Up, Dow, Right

LiquidCrystal lcd(8, 14, 9, 4, 5, 6, 7);
// LCD pins: RS  RW  EN  D4 D5 D6 D7

void setup() {
    	
	Serial.begin(115200);
	lcd.begin(16, 2);

}

void loop() {
	
	lcd.clear();
	lcd.setCursor( 0, 0);
	lcd.print("Resolver");
	
	lcd.setCursor(15, 0);
	lcd.print(key.readKey());

	lcd.setCursor( 0, 1);
	lcd.print(deg.readresolverdecimal("H"));
	
	lcd.setCursor( 10, 1);
	lcd.print(deg.readresovlerdegrees("H"));

	delay(200);

}