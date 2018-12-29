#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_GPS_SHIELD
#include <OneSheeld.h>
 

// Set up the Http calls

// Car Locator
HttpRequest DollyRequest("https://dolly-e7b16.firebaseio.com/Cars.json");

// Delay Call
//HttpRequest DelayCall("https://dolly-e7b16.firebaseio.com/delay.json");
HttpRequest DelayCall("https://radiant-dusk-84643.herokuapp.com/delay");


// Call Button Initialisation
int buttonPin = 8;
int buttonState = 0;


//String longitude;
//String latitude;

float lat;
float lon;

char charlat [10];
char charlon [10];


char gps_Reading[100];

void setup() {
  
  OneSheeld.begin();

  pinMode(buttonPin, INPUT);

  gps_Reading[0] = 0;
  
   
  Terminal.println("Setup has begun");
   
  DollyRequest.addHeader("Authorization", "fHjNQzSVdq8eSs7ZjdZ64n5UUqIP7QKyAhrK5r3g");
  DollyRequest.addHeader("Content-Type", "application/json");  
  DollyRequest.addHeader("Accept-Language", "en_EN");
    
  DollyRequest.setOnSuccess(&onSuccess);
  DollyRequest.setOnFailure(&onFailure);
   
  /* Subscribe to response errors. */
  DollyRequest.getResponse().setOnError(&onResponseError);

  gps_Reading[0] = 0;

  Internet.setOnError(&onInternetError);

  Terminal.println("Finished the setup");
   
}
 
void loop()
{

  gps_Reading[0] = 0;

  buttonState = digitalRead(buttonPin);
  Terminal.println(buttonState);
  
  if (buttonState == HIGH)
  {

      //SMS.send("0894589018", "Hello"); // send sms to number
      //pinMode(buttonPin, INPUT);
      
     Terminal.println("Button Pressed");

     DelayCall.addRawData("Call from the Button");
     Internet.performPost(DelayCall);
     
     OneSheeld.delay(3000);
     buttonState = 0;

  }

  //Terminal.println("In Loop");
  //OneSheeld.delay(3000);

  // Get the Latitude and Longitude
  lat = GPS.getLatitude();
  lon = GPS.getLongitude();
  
  // Convert geo coordinates to char to send to the server  
  dtostrf(lat, 7, 5, charlat); 
  dtostrf(lon, 7, 5, charlon);

  strcat(gps_Reading, "{\"id\": \"car1\",");
  strcat(gps_Reading, "\"lat\": ");
  strcat(gps_Reading, charlat);
  strcat(gps_Reading, ", \"lng\": ");
  strcat(gps_Reading, charlon);
  strcat(gps_Reading, "}");
  
  DollyRequest.addRawData(gps_Reading);
  
 
  Terminal.println(gps_Reading); 
 
  Internet.performPost(DollyRequest);
  OneSheeld.delay(3000);
  
}
  
void onSuccess (HttpResponse & response)
{
 
 Terminal.println("Dolly Request succeeded");
  
}

void onFailure(HttpResponse & response)
{
  Terminal.println("Failure");
  Terminal.println(response.getStatusCode( ));
  Terminal.println(response.getTotalBytesCount( ));
}

 void onInternetError(int requestId, int errorNumber)
{
  /* Print out error Number.*/
  Terminal.print("Request id:");
  Terminal.println(requestId);
  Terminal.print("Internet error:");
  switch(errorNumber)
  {
    case REQUEST_CAN_NOT_BE_FOUND: Terminal.println("REQUEST_CAN_NOT_BE_FOUND");break;
    case NOT_CONNECTED_TO_NETWORK: Terminal.println("NOT_CONNECTED_TO_NETWORK");break;
    case URL_IS_NOT_FOUND: Terminal.println("URL_IS_NOT_FOUND");break;
    case ALREADY_EXECUTING_REQUEST: Terminal.println("ALREADY_EXECUTING_REQUEST");break;
    case URL_IS_WRONG: Terminal.println("URL_IS_WRONG");break;
  }
}

void onResponseError(int errorNumber)
{
  /* Print out error Number.*/
  Terminal.print("Response error:");
  switch(errorNumber)
  {
    case INDEX_OUT_OF_BOUNDS: Terminal.println("INDEX_OUT_OF_BOUNDS");break;
    case RESPONSE_CAN_NOT_BE_FOUND: Terminal.println("RESPONSE_CAN_NOT_BE_FOUND");break;
    case HEADER_CAN_NOT_BE_FOUND: Terminal.println("HEADER_CAN_NOT_BE_FOUND");break;
    case NO_ENOUGH_BYTES: Terminal.println("NO_ENOUGH_BYTES");break;
    case REQUEST_HAS_NO_RESPONSE: Terminal.println("REQUEST_HAS_NO_RESPONSE");break;
    case SIZE_OF_REQUEST_CAN_NOT_BE_ZERO: Terminal.println("SIZE_OF_REQUEST_CAN_NOT_BE_ZERO");break;
    case UNSUPPORTED_HTTP_ENTITY: Terminal.println("UNSUPPORTED_HTTP_ENTITY");break;
    case JSON_KEYCHAIN_IS_WRONG: Terminal.println("JSON_KEYCHAIN_IS_WRONG");break;
  }
}
