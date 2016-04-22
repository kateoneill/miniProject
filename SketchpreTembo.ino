#define SOUND_SENSOR       8           /* sound sensor pin */          
#define BUZZER             6            /* LED pin */
#define THRESHOLD_VALUE    300          /* Depending on your LaunchPad’s ADC resolution you may have to adjust the threshold */
 
#define ON                 HIGH                    /* led on */
#define OFF                LOW                     /* led off */
#define _handle_led(x)     digitalWrite(BUZZER, x)    /* handle led */

#include <Bridge.h>
#include <Temboo.h>
#include "kateoneill.h" // contains Temboo account information
 
/* Global Variables */
int sound_value = 0;

// Note that for additional security and reusability, you could
// use #define statements to specify these values in a .h file.

// your Gmail username, formatted as a complete email address, eg "bob.smith@gmail.com"
const String GMAIL_USER_NAME = "kateooneill@gmail.com";

// your Gmail App-Specific Password
const String GMAIL_PASSWORD = "hwhiikvkqgnfsqlw";

// the email address you want to send the email to, eg "jane.doe@temboo.com"
const String TO_EMAIL_ADDRESS = "s00165383@mail.itsligo.ie";


boolean success = false; // a flag to indicate whether we've sent the email yet or not
 
void setup() {
 
    /* Initialize led pin */
    pinMode(BUZZER, OUTPUT);  
    digitalWrite(BUZZER, LOW);

      Serial.begin(9600);

  // for debugging, wait until a serial console is connected
  delay(4000);
  //while(!Serial);

  Bridge.begin();
  
}
 
void loop() {
 
    /* read the sound value */
    sound_value = analogRead(SOUND_SENSOR);         
    /* if the value is larger than threshold, turn on led */
    if(sound_value > THRESHOLD_VALUE) {
        _handle_led(ON); 
        delay(5000);
        _handle_led(OFF);


    Serial.println("Running SendAnEmail...");
  
    TembooChoreo SendEmailChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    SendEmailChoreo.begin();
    
    // set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Google > Gmail > SendEmail)
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
 

    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Google/Gmail/SendEmail/ 
    // for complete details about the inputs for this Choreo

    // the first input is your Gmail email address
    SendEmailChoreo.addInput("Username", GMAIL_USER_NAME);
    // next is your Gmail App-Specific password.
    SendEmailChoreo.addInput("Password", GMAIL_PASSWORD);
    // who to send the email to
    SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS);
    // then a subject line
    SendEmailChoreo.addInput("Subject", "ALERT: Intruder");

     // next comes the message body, the main content of the email   
    SendEmailChoreo.addInput("MessageBody", "Somebody has triggered the alarm!");

    // tell the Choreo to run and wait for the results. The 
    // return code (returnCode) will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = SendEmailChoreo.run();

    // a return code of zero (0) means everything worked
    if (returnCode == 0) {
        Serial.println("Success! Email sent!");
        success = true;
    } else {
      // a non-zero return code means there was an error
      // read and print the error message
      while (SendEmailChoreo.available()) {
        char c = SendEmailChoreo.read();
        Serial.print(c);
      }
    } 
    SendEmailChoreo.close();

    // do nothing for the next 5 seconds
    delay(5000);
  }
    //}
   // _handle_led(OFF);
}
