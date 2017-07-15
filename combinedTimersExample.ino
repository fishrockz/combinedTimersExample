#include <PulseEvent.h>
#include <UnevenTime.h>

#include <math.h> 

//#include "EventObjects.h"
// Simple loopback test: create 1 output to transmit
// test pulses, and 1 input to receive the pulses
PulseEventOutput myOut;
PulseEventInput myIn;
UnevenTimeTriger UnevenTimerBlock;


uint32_t reversvalueA [] = { 
10,
40,
500,
4,
300,
20,
50,
20,
5,
20,

};

int LEDSTATE;
int trigerPin=12;

void blockFunction(EventBaseObject * ThisFunctionsInfoPointer){
	
	Serial.println ("this is the fast function");
	
	if (LEDSTATE==1){LEDSTATE=0;}else{LEDSTATE=1;}
	digitalWrite(ledPin, LEDSTATE);
	

}

void generalFunction(EventBaseObject * ThisFunctionsInfoPointer){
	digitalWrite(trigerPin, 0);
	Serial.println ("this is the slow function");

	if (LEDSTATE==1){LEDSTATE=0;}else{LEDSTATE=1;}
	digitalWrite(ledPin, LEDSTATE);
	

}



int count=0;
int state=0;
void PulseEventFunction(EventBaseObject * ThisFunctionsInfoPointer){
PulseEventObject* ThisFunctionsInfo = static_cast<PulseEventObject*>(ThisFunctionsInfoPointer);



	int  num;

	num = ThisFunctionsInfo->available();

	//Serial.println(num);

	if (num > 0) {
		count = count + 1;

		for (int i=1; i <= num; i++) {
			float val = ThisFunctionsInfo->read(i);

			if (i==3){
				int curstate = round(val/100);
				if (state!=curstate){
					Serial.print("EventFunction called: State changed: ");
					Serial.println(curstate);
					state=curstate;
					reversvalueA[4]=state*10;
					LEDSTATE=1;digitalWrite(ledPin, LEDSTATE);
					digitalWrite(trigerPin, 1);
					UnevenTimerBlock.StartTimer(reversvalueA,(uint32_t)10,generalFunction,blockFunction);
				}
			}
		}
		//Serial.println();
	}else{
		Serial.println("num==0");
	}
	//delay(500);
}


void setup() {

	pinMode(ledPin, OUTPUT);
	pinMode(trigerPin, OUTPUT);
	
	while(!Serial){};delay(200);Serial.println("setting off A");

	myOut.begin(9);  // connect pins 9 and 10 together...
	myIn.begin(10, PulseEventFunction);
	myOut.write(1, 600.03);
	myOut.write(2, 1500);
	myOut.write(3, 759.24);
	// slots 4 and 5 will default to 1500 us
	myOut.write(6, 1234.56);

	Serial.println("setting off B");
}


int output=0;
void loop() {
//Serial.println("going");

	delay(1000);
	myOut.write(3, 1000 + 60.*output);// as PulseEventFunction rounds to the nearst 100ms and 60 isnt a multipule of 100 you get a paten on the scope of between 1 to 3 repeted pulses then a gap of a second or so. also some of the pulse train changes according to this value
	output++;
	if (output>20) output=0;

}
