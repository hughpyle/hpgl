// Serial interface for HPGL plotter


#define BAUDRATE 9600
#define PLOTTER Serial1
#define USB Serial
#define XON 0x11
#define XOFF 0x13
#define SELFTEST_PIN 4


// selftest routine
static const char *selftest = "IN;SP1;PU2541,3296;PD2541,3126;PD2647,3126;PD2775,3254;PD2775,3509;PD2584,3700;PD1819,3700;PD1819,3296;PD2541,3296;PU2541,2680;PD2541,2510;PD2647,2510;PD2775,2638;PD2775,2935;PD2647,3063;PD1819,3063;PD1819,2680;PD2541,2680;PU2775,2043;PD2775,2447;PD1819,2447;PD1819,2043;PD2775,2043;PU2541,4507;PD2541,3934;PD2669,3934;PD2775,4061;PD2775,4656;PD2541,4911;PD1819,4911;PD1819,4507;PD2541,4507;PU2223,3870;PD2392,4018;PD2392,4443;PD2201,4443;PD2201,4252;PD2053,4252;PD2053,4443;PD1946,4443;PD1819,4337;PD1819,4018;PD1968,3870;PD2223,3870;PU2775,5548;PD2775,5888;PD2541,6121;PD2392,6121;PD2392,5718;PD2541,5718;PD2541,5548;PD2775,5548;PU2541,5080;PD2775,5314;PD2775,5484;PD1819,5484;PD1819,5314;PD2053,5080;PD2541,5080;PU2053,5548;PD2053,5718;PD2159,5718;PD2159,6121;PD2053,6121;PD1819,5888;PD1819,5548;PD2053,5548;PU3136,6291;PD3136,6673;PD1819,6673;PD1819,6291;PD3136,6291;PU2541,6928;PD2541,6737;PD2669,6737;PD2775,6865;PD2775,7077;PD2541,7332;PD1819,7332;PD1819,6928;PD2541,6928;PU3157,7502;PD3157,7905;PD2902,7905;PD2902,7502;PD3157,7502;PU2775,7502;PD2775,7905;PD1819,7905;PD1819,7502;PD2775,7502;PU2541,8734;PD2541,8564;PD2647,8564;PD2775,8691;PD2775,8925;PD2562,9137;PD1819,9137;PD1819,8734;PD2541,8734;PU2775,8096;PD2775,8500;PD1819,8500;PD1819,8096;PD2775,8096;PU2371,9774;PD2371,9965;PD2541,9965;PD2541,9774;PD2775,9774;PD2775,10114;PD2541,10348;PD2201,10348;PD2201,9774;PD2371,9774;PU2541,9307;PD2775,9562;PD2775,9711;PD1819,9711;PD1819,9562;PD2053,9307;PD2541,9307;PU2053,9774;PD2053,10305;PD1946,10305;PD1819,10199;PD1819,9774;PD2053,9774;PU;SP0;PG;";


// XON-XOFF flow control
bool isActive = true;


void setup() {
  // Initialization
  PLOTTER.begin(BAUDRATE);
  USB.begin(BAUDRATE);
  pinMode(SELFTEST_PIN, INPUT_PULLUP);
  // TODO hardware button to print the selftest
}


void loop() {
  int data;

  if (PLOTTER.available() > 0) {
    // Plotter has something to say.
    // Look for XON and XOFF flow control instructions.
    data = PLOTTER.read();
    if (data == XON) {
      isActive = true;
    }
    else if (data == XOFF) {
      isActive = false;
    }
    else {
      USB.write(data);
    }
  }

  // Set the LED depending on whether we're active
  digitalWrite(LED_BUILTIN, isActive ? HIGH : LOW);

  if (USB.available() > 0 && isActive) {
    // read the next thing and send it to the plotter
    data = USB.read();
    PLOTTER.write(data);
  }

}