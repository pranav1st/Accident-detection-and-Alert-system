// GPS Module
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#define RXD2 16
#define TXD2 17
TinyGPSPlus gps;

// Wifi Module
#include <Wifi.h>
#include <HTTPClient.h>

// Temperature Module
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

// ADXL345
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Program
const char *ssid = "<-- Enter WIFI SSID -->";
const char *password = "<-- Enter Password -->";
String tobesend;
String tempStr;
String gpsStr;
String touchStr;
String accelStr;

String gpsCollect(void){
  bool recebido = false;
 
  while (Serial1.available()) {
     char cIn = Serial1.read();
     recebido = gps.encode(cIn);
  }
  
  // Serial.print("Without check: ");
  // Serial.print(" | alt: ");
  // Serial.print(gps.altitude.feet());
  // Serial.print(" | satellites: ");
  // Serial.println(gps.satellites.value());
  // Serial.print(" | latitude: ");
  // Serial.println(gps.location.lat(),6);  
  // Serial.print(" | longitude: ");
  // Serial.println(gps.location.lng(),6);  

   if (gps.location.isUpdated() && gps.altitude.isUpdated())
   {
    // Serial.print("Updated");
    // Serial.print("D/M/A: ");
    // Serial.print(gps.date.value());
    // Serial.print(" | alt: ");
    // Serial.print(gps.altitude.feet());
    // Serial.print(" | satellites: ");
    // Serial.println(gps.satellites.value());
    // Serial.print(" | latitude: ");
    // Serial.println(gps.location.lat(),6);  
    // Serial.print(" | longitude: ");
    // Serial.println(gps.location.lng(),6);  
   }
  else
  {
    // Serial.print("not updated");
  }
  return("%0AAltitude:+"+String(gps.altitude.feet())+"%0ASatellites:+"+String(gps.satellites.value())+"%0ALatitude:+"+String(gps.location.lat(),6)+"%0ALongitude:+"+String(gps.location.lng(),6));
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  // Serial.println("------------------------------------");
  // Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  // Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  // Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  // Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  // Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  // Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  // Serial.println("------------------------------------");
  // Serial.println("");
  delay(500);
}
 
void displayDataRate(void)
{
  // Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      // Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      // Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      // Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      // Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      // Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      // Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      // Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      // Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      // Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      // Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      // Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      // Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      // Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      // Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      // Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      // Serial.print  ("0.10 "); 
      break;
    default:
      // Serial.print  ("???? "); 
      break;
  }  
  // Serial.println(" Hz");  
}
 
void displayRange(void)
{
  // Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      // Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      // Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      // Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      // Serial.print  ("2 "); 
      break;
    default:
      // Serial.print  ("?? "); 
      break;
  }  
  // Serial.println(" g");  
}

void setup()
{
	// Serial.begin(115200);
	delay(100);
	// Serial.println();
	// Serial.println();
	// Serial.print("Connecting to ");
	// Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		// Serial.print(".");
	}
	// Serial.println("");
	// Serial.println("WiFi connected");
	// Serial.println("IP address: ");
	// Serial.println(WiFi.localIP());

  // ADXL345
	#ifndef ESP8266
	// while (!Serial); // for Leonardo/Micro/Zero
	#endif
	// Serial.println("Accelerometer Test"); Serial.println("");
	if(!accel.begin())  // Init sensor
	{
		// Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
		while(1);
	}
	accel.setRange(ADXL345_RANGE_16_G);   // Setting Range
	// accel.setRange(ADXL345_RANGE_8_G);
	// accel.setRange(ADXL345_RANGE_4_G);
	// accel.setRange(ADXL345_RANGE_2_G);
	displaySensorDetails();   // Display some basic information on this sensor
	// displayDataRate();        // Display additional settings (outside the scope of sensor_t)
	// displayRange();
	// Serial.println("");

  //GPS
  
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2); //gps baud
}

void loop()
{
	delay(1000);
	// Serial.print("Connecting to ");
	// Serial.println("Signal");
	HTTPClient http; // Declare an object of class HTTPClient
	tobesend = "https://api.callmebot.com/signal/send.php?";
	tobesend = tobesend + "phone=+91<-- Enter 10 digit phone number after country code-->";    // Eg: "phone=+91XXXXXXXXXX"

	//Temperature Sensor
	tempStr = "Temperature:%0AIn+Fahrenheit+-+" + String(temprature_sens_read())+"F%0A";      // Temperature in Fahrenheit degrees
	// Serial.print((temprature_sens_read() - 32) / 1.8);
	// Serial.println("+C");
	tempStr = tempStr +"In+Celsius+-+"+ String((temprature_sens_read() - 32) / 1.8) + "C%0A"; // Temperature in Celsius degrees

	// Touch Pin - Port D4
	touchStr = String(touchRead(T0));

  // ADXL345
	sensors_event_t event; 
	accel.getEvent(&event);

  static float x_accel_prev = 0;
	static float y_accel_prev = 0;
	static float z_accel_prev = 10;   // For flat surface

	float x_accel_curr = event.acceleration.x;
	float y_accel_curr = event.acceleration.y;
	float z_accel_curr = event.acceleration.z;

  float x_accel_read = x_accel_curr - x_accel_prev;
  float y_accel_read = y_accel_curr - y_accel_prev;
  float z_accel_read = z_accel_curr - z_accel_prev;

	// Serial.print("X: "); Serial.print(x_accel_read); Serial.print("  ");
	// Serial.print("Y: "); Serial.print(y_accel_read); Serial.print("  ");
	// Serial.print("Z: "); Serial.print(z_accel_read); Serial.print("  ");
  // Serial.println("m/s^2 ");

  x_accel_prev = x_accel_curr;
	y_accel_prev = y_accel_curr;
	z_accel_prev = z_accel_curr;

  accelStr = "Accelerometer+Reading:%0AX:"+String(x_accel_read)+"%0AY:"+String(y_accel_read)+"%0AZ:"+String(z_accel_read)+"%0A";

	
  tobesend = tobesend + "&text="+tempStr+"Touch+Reading:+"+touchStr+"%0A"+accelStr;
  // Serial.print(gpsCollect());
  // Final message URL
	// Serial.println(tobesend);

  // Condition to send message
  if (abs(x_accel_read) > 4 || abs(y_accel_read) > 4 || abs(z_accel_read) > 4){
    gpsStr = gpsCollect();
    tobesend = tobesend + gpsStr;
  	tobesend = tobesend + "&apikey=<-- Enter API Key -->";    // Eg: &apikey=XXXXXX
    http.begin(tobesend);      // Sending message
    int httpCode = http.GET(); // Send the request
    if (httpCode > 0)
    {									   // Check the returning code
      String payload = http.getString(); // Get the request response payload
      // Serial.println(payload);		   // Print the response payload
    }
    http.end(); // Close connection
  }
}	