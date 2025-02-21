// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <ArduinoJson.h>
// #include <LiquidCrystal_I2C.h>
// #include <Preferences.h>


// // declare all the scopes 
// void loadSavedSequence();

// // Access Point Settings
// const char* ap_ssid = "CoatingSystem";  // Name of the WiFi hotspot
// const char* ap_password = "coating123";  // Password for the WiFi hotspot
// const int MAX_CLIENTS = 4;              // Maximum number of simultaneous clients

// // Pin Definitions
// #define ENA_PIN 19  // Flour chamber speed control
// #define ENB_PIN 23  // Mixing motor speed control
// #define IN1_PIN 18  // Motor A direction
// #define IN2_PIN 17  // Motor A direction
// #define IN3_PIN 4   // Motor B direction
// #define IN4_PIN 16  // Motor B direction
// #define POT_PIN 33  // Potentiometer input
// #define BTN1_PIN 25 // Menu button
// #define BTN2_PIN 26 // Navigation button

// #define RELAY_PIN 13 // Egg nozzle control
// #define ALARM_PIN 32 // Completion alarm


// // Add error LED pin for visual feedback
// #define ERROR_LED_PIN 27



// // Add watchdog timeout configuration
// const unsigned long WATCHDOG_TIMEOUT = 10000;
// const unsigned long WIFI_CHECK_INTERVAL = 10000;

// // WiFi credentials
// // const char* wifi_ssid = "App-Website-IoT-engr-07059011222";       // Your WiFi network name
// const char* wifi_ssid = "web-apps-graphics-0705901g1222";       // Your WiFi network name

// // const char* wifi_password = "kingofboys";    // Your WiFi password

// const char* wifi_password = "........a";    // Your WiFi password

// bool wifiConnected = false;                    // Track WiFi connection status

// // Global objects
// LiquidCrystal_I2C lcd(0x27, 16, 2);
// AsyncWebServer   server(80);
// Preferences preferences;

// // Menu system states
// enum MenuState {
//   NORMAL_MODE,
//   MENU_MODE,
//   EDITING_MODE
// };

// enum MenuItem {
//   EGG_DURATION,
//   FLOUR_DURATION,
//   MIXING_DURATION,
//   START_STOP,
//   EXIT_MENU,
// };

// // Sequence storage structure
// struct ProcessSequence {
//   int eggNozzleDuration;
//   int flourChamberDuration;
//   int mixingDuration;
//   int flourSpeed;
//   int mixingSpeed;
// } savedSequence;

// // System state structure
// struct SystemState {
//   int flourSpeed = 0;
//   int mixingSpeed = 0;
//   bool eggNozzleActive = false;
//   bool isProcessing = false;
//   unsigned long eggNozzleTimer = 0;
//   unsigned long flourChamberTimer = 0;
//   unsigned long mixingTimer = 0;
  
//   // Process settings
//   int eggNozzleDuration = 5000;  // 5 seconds default
//   int flourChamberDuration = 10000;  // 10 seconds default
//   int mixingDuration = 15000;  // 15 seconds default
  
//   // Menu state
//   MenuState menuState = NORMAL_MODE;
//   MenuItem currentMenuItem = EGG_DURATION;
//   bool isEditing = false;


//   // Added safety monitoring
//   unsigned long lastWatchdogReset = 0;
//   unsigned long lastWifiCheck = 0;
//   int errorCount = 0;
//   const int MAX_ERRORS = 3;
  
//   // Added motor monitoring
//   int flourMotorCurrent = 0;
//   int mixingMotorCurrent = 0;
//   const int MOTOR_CURRENT_THRESHOLD = 1000;
// } state;

// // Button debouncing
// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 250;


// // Function declarations
// void loadSavedSequence();
// void saveSequence();
// void setupAPIEndpoints();
// void handleButtons();
// void handlePotentiometer();
// void startCoatingProcess();
// void stopCoatingProcess();
// void updateProcessTimers();
// void controlEggNozzle(bool state);
// void controlFlourChamber(int speed);
// void controlMixingMotor(int speed);
// void toggleMenuMode();
// void navigateMenu();
// void adjustTiming();
// void displayMenuItem();
// void updateLCDDisplay();
// void handleError(const char* errorMessage);
// bool performSafetyChecks();
// void loadSequenceToState();
// void saveStateToSequence();
// void emergencyStop();
// void runDiagnostics();
// void systemWatchdog();
// void safeLoop();
// void handleError(const char* errorMessage);



// IPAddress local_IP(192, 168, 0,148);     // Static IP for Wi-Fi mode
// IPAddress gateway(192, 168, 0, 1);
// IPAddress subnet(255, 255, 255, 0);

// IPAddress ap_IP(192, 168, 4, 1);         // Static IP for AP mode
// IPAddress ap_gateway(192, 168, 4, 1);
// IPAddress ap_subnet(255, 255, 255, 0);




// void setup() {
//   Serial.begin(9600);
//     pinMode(ERROR_LED_PIN, OUTPUT);
//   // Initialize pins
//   pinMode(ENA_PIN, OUTPUT);
//   pinMode(ENB_PIN, OUTPUT);
//   pinMode(IN1_PIN, OUTPUT);
//   pinMode(IN2_PIN, OUTPUT);
//   pinMode(IN3_PIN, OUTPUT);
//   pinMode(IN4_PIN, OUTPUT);
//   pinMode(POT_PIN, INPUT);
//   pinMode(BTN1_PIN, INPUT_PULLUP);
//   pinMode(BTN2_PIN, INPUT_PULLUP);

//   pinMode(RELAY_PIN, OUTPUT);
//   pinMode(ALARM_PIN, OUTPUT);





//   // Initialize LCD
//   lcd.init();
//   lcd.backlight();
//   lcd.clear();
//   lcd.print("System Ready");

//   // Initialize preferences
//   preferences.begin("coating", false);
  
//   // Load saved sequence
//   loadSavedSequence();


//   WiFi.mode(WIFI_STA);  // Set ESP32 as Station (Client)
//   WiFi.config(local_IP, gateway, subnet);
//   WiFi.begin(wifi_ssid, wifi_password);
  
//   unsigned long startAttemptTime = millis();
//   while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
//       Serial.print(".");
//       delay(500);
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//       Serial.println("\nConnected to Wi-Fi");
//       Serial.print("IP Address: ");
//       Serial.println(WiFi.localIP());
//   } else {
//       Serial.println("\nWi-Fi Failed! Starting AP Mode...");
      
//       WiFi.softAP(ap_ssid, ap_password);
//       WiFi.softAPConfig(ap_IP, ap_gateway, ap_subnet);
      
//       Serial.print("AP IP Address: ");
//       Serial.println(WiFi.softAPIP());
//   }



//   // Setup API endpoints
//   setupAPIEndpoints();
//     runDiagnostics();
// }

// void loadSavedSequence() {
//   savedSequence.eggNozzleDuration = preferences.getInt("eggDur", 5000);
//   savedSequence.flourChamberDuration = preferences.getInt("flourDur", 10000);
//   savedSequence.mixingDuration = preferences.getInt("mixDur", 15000);
//   savedSequence.flourSpeed = preferences.getInt("flourSpd", 128);
//   savedSequence.mixingSpeed = preferences.getInt("mixSpd", 255);
// }

// void saveSequence() {
//   preferences.putInt("eggDur", savedSequence.eggNozzleDuration);
//   preferences.putInt("flourDur", savedSequence.flourChamberDuration);
//   preferences.putInt("mixDur", savedSequence.mixingDuration);
//   preferences.putInt("flourSpd", savedSequence.flourSpeed);
//   preferences.putInt("mixSpd", savedSequence.mixingSpeed);
// }

// void setupAPIEndpoints() {
//   // Get system status
//   server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     StaticJsonDocument<200> doc;
//     doc["flourSpeed"] = state.flourSpeed;
//     doc["mixingSpeed"] = state.mixingSpeed;
//     doc["eggNozzleActive"] = state.eggNozzleActive;
//     doc["isProcessing"] = state.isProcessing;
    
//     String response;
//     serializeJson(doc, response);
//     request->send(200, "application/json", response);
//   });

//   // Start coating process
//   server.on("/api/start", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (!state.isProcessing) {
//       startCoatingProcess();
//       request->send(200, "text/plain", "Process started");
//     } else {
//       request->send(400, "text/plain", "Process already running");
//     }
//   });

//   // Stop coating process
//   server.on("/api/stop", HTTP_POST, [](AsyncWebServerRequest *request) {
//     stopCoatingProcess();
//     request->send(200, "text/plain", "Process stopped");
//   });

//   // Get saved sequence
//   server.on("/api/sequence", HTTP_GET, [](AsyncWebServerRequest *request) {
//     StaticJsonDocument <200> doc;
//     doc["eggDuration"] = savedSequence.eggNozzleDuration;
//     doc["flourDuration"] = savedSequence.flourChamberDuration;
//     doc["mixingDuration"] = savedSequence.mixingDuration;
//     doc["flourSpeed"] = savedSequence.flourSpeed;
//     doc["mixingSpeed"] = savedSequence.mixingSpeed;
    
//     String response;
//     serializeJson(doc, response);
//     request->send(200, "application/json", response);
//   });

//   // Save sequence
//   server.on("/api/sequence", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("eggDuration", true)) {
//       savedSequence.eggNozzleDuration = request->getParam("eggDuration", true)->value().toInt();
//     }
//     if (request->hasParam("flourDuration", true)) {
//       savedSequence.flourChamberDuration = request->getParam("flourDuration", true)->value().toInt();
//     }
//     if (request->hasParam("mixingDuration", true)) {
//       savedSequence.mixingDuration = request->getParam("mixingDuration", true)->value().toInt();
//     }
//     if (request->hasParam("flourSpeed", true)) {
//       savedSequence.flourSpeed = request->getParam("flourSpeed", true)->value().toInt();
//     }
//     if (request->hasParam("mixingSpeed", true)) {
//       savedSequence.mixingSpeed = request->getParam("mixingSpeed", true)->value().toInt();
//     }
    
//     saveSequence();
//     request->send(200, "text/plain", "Sequence saved");
//   });

//   // Manual control endpoints
//   server.on("/api/manual/egg", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("state", true)) {
//       bool nozzleState = request->getParam("state", true)->value() == "1";
//       controlEggNozzle(nozzleState);
//       request->send(200, "text/plain", "Egg nozzle updated");
//     }
//   });

//   server.on("/api/manual/flour", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("speed", true)) {
//       int speed = request->getParam("speed", true)->value().toInt();
//       controlFlourChamber(speed);
//       request->send(200, "text/plain", "Flour chamber updated");
//     }
//   });

//   server.on("/api/manual/mixing", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("speed", true)) {
//       int speed = request->getParam("speed", true)->value().toInt();
//       controlMixingMotor(speed);
//       request->send(200, "text/plain", "Mixing motor updated");
//     }
//   });
//   server.on("/welcome", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String html = "<html><head><title>ESP32 Welcome</title></head>";
//     html += "<body><h1>Welcome to ESP32 Server</h1>";
//     html += "<p>Control and monitor your device from here.</p></body></html>";
//     request->send(200, "text/html", html);
// });

//   server.begin();
// }


// // void handleButtons() {
// //   if (millis() - lastDebounceTime < debounceDelay) return;

// //   if (digitalRead(BTN1_PIN) == LOW) {  // Menu button
// //     lastDebounceTime = millis();
// //     toggleMenuMode();
// //   }
  
// //   if (digitalRead(BTN2_PIN) == LOW) {  // Select/Confirm button
// //     lastDebounceTime = millis();
// //     if (state.menuState == MENU_MODE) {
// //       // Enter editing mode for the current menu item
// //       state.menuState = EDITING_MODE;
// //       state.isEditing = true;
// //     } else if (state.menuState == EDITING_MODE) {
// //       // Confirm the edited value and return to menu mode
// //       state.menuState = MENU_MODE;
// //       state.isEditing = false;
// //       saveStateToSequence();  // Save changes
// //     }
// //     displayMenuItem();
// //   }
// // }

// void handleButtons() {
//   if (millis() - lastDebounceTime < debounceDelay) return;

//   // BTN1 (Menu/Navigation)
//   if (digitalRead(BTN1_PIN) == LOW) {
//     lastDebounceTime = millis();
//     switch (state.menuState) {
//       case NORMAL_MODE:
//         state.menuState = MENU_MODE;
//         state.currentMenuItem = EGG_DURATION;
//         break;
        
//       case MENU_MODE:
//         // Cycle through menu items
//         state.currentMenuItem = static_cast<MenuItem>((static_cast<int>(state.currentMenuItem) + 1) % 5);
//         if (state.currentMenuItem == EXIT_MENU) {
//           state.menuState = NORMAL_MODE;
//         }
//         break;
        
//       case EDITING_MODE:
//         // Return to menu mode without saving
//         state.menuState = MENU_MODE;
//         state.isEditing = false;
//         break;
//     }
//     displayMenuItem();
//   }
  
//   // BTN2 (Select/Confirm)
//   if (digitalRead(BTN2_PIN) == LOW) {
//     lastDebounceTime = millis();
    
//     if (state.menuState == MENU_MODE) {
//       if (state.currentMenuItem == START_STOP) {
//         // Handle start/stop action
//         if (!state.isProcessing) {
//           startCoatingProcess();
//           lcd.clear();
//           lcd.print("Process Started");
//           delay(1000);
//           state.menuState = NORMAL_MODE;
//         } else {
//           stopCoatingProcess();
//           lcd.clear();
//           lcd.print("Process Stopped");
//           delay(1000);
//           state.menuState = NORMAL_MODE;
//         }
//       } else if (state.currentMenuItem != EXIT_MENU) {
//         // Enter editing mode for other menu items
//         state.menuState = EDITING_MODE;
//         state.isEditing = true;
//       }
//     } else if (state.menuState == EDITING_MODE) {
//       // Confirm the edited value and return to menu mode
//       state.menuState = MENU_MODE;
//       state.isEditing = false;
//       saveStateToSequence();
//     }
//     displayMenuItem();
//   }
// }
// void loop() {
//   safeLoop();
//   delay(10);
// }





// // Modified potentiometer handling to include menu value adjustment
// void handlePotentiometer() {
//   int potValue = analogRead(POT_PIN);
  
//   if (state.menuState == EDITING_MODE) {
//     // Use potentiometer for menu value adjustment
//     switch (state.currentMenuItem) {
//       case EGG_DURATION:
//         state.eggNozzleDuration = map(potValue, 0, 4095, 1000, 30000);
//         break;
//       case FLOUR_DURATION:
//         state.flourChamberDuration = map(potValue, 0, 4095, 1000, 60000);
//         break;
//       case MIXING_DURATION:
//         state.mixingDuration = map(potValue, 0, 4095, 1000, 120000);
//         break;
        
//       default:
//         break;
//     }
//     displayMenuItem();
//   } else if (state.menuState == NORMAL_MODE) {
//     // Normal potentiometer behavior for flour speed
//     state.flourSpeed = map(potValue, 0, 4095, 0, 255);
//     if (state.isProcessing) {
//       controlFlourChamber(state.flourSpeed);
//     }
//   }
// }

// void startCoatingProcess() {
//   state.isProcessing = true;
//   state.eggNozzleTimer = millis();
//   state.flourChamberTimer = millis();
//   state.mixingTimer = millis();
  
//   controlEggNozzle(true);
//   controlFlourChamber(state.flourSpeed);
//   controlMixingMotor(255);
// }

// void stopCoatingProcess() {
//   state.isProcessing = false;
//   controlEggNozzle(false);
//   controlFlourChamber(0);
//   controlMixingMotor(0);
//   digitalWrite(ALARM_PIN, HIGH);
//   delay(1000);
//   digitalWrite(ALARM_PIN, LOW);
// }

// void updateProcessTimers() {
//   unsigned long currentTime = millis();
  
//   if (state.eggNozzleActive && (currentTime - state.eggNozzleTimer >= state.eggNozzleDuration)) {
//     controlEggNozzle(false);
//   }
  
//   if (currentTime - state.flourChamberTimer >= state.flourChamberDuration) {
//     controlFlourChamber(0);
//   }
  
//   if (currentTime - state.mixingTimer >= state.mixingDuration) {
//     stopCoatingProcess();
//   }
// }

// void controlEggNozzle(bool nozzleState) {
//   digitalWrite(RELAY_PIN, nozzleState==true?HIGH:LOW);
//   state.eggNozzleActive = nozzleState;
// }


// void controlFlourChamber(int speed) {
//   analogWrite(ENA_PIN, speed);
//   digitalWrite(IN1_PIN, speed > 0);
//   digitalWrite(IN2_PIN, 0);
// }

// void controlMixingMotor(int speed) {
//   analogWrite(ENB_PIN, speed);
//   digitalWrite(IN3_PIN, speed > 0);
//   digitalWrite(IN4_PIN, 0);
// }




// // Modified menu navigation to use BTN2
// void toggleMenuMode() {
//   switch (state.menuState) {
//     case NORMAL_MODE:
//       state.menuState = MENU_MODE;
//       state.currentMenuItem = EGG_DURATION;
//       break;
      
//     case MENU_MODE:
//       // Cycle through menu items
//       state.currentMenuItem = static_cast<MenuItem>((static_cast<int>(state.currentMenuItem) + 1) % 5);
//       if (state.currentMenuItem == EXIT_MENU) {
//         state.menuState = NORMAL_MODE;
//       }
//       break;
      
//     case EDITING_MODE:
//       // Return to menu mode without saving
//       state.menuState = MENU_MODE;
//       state.isEditing = false;
//       break;
//   }
//   displayMenuItem();
// }


// // void navigateMenu() {
// //   switch (state.menuState) {
// //     case MENU_MODE:
// //       state.currentMenuItem = static_cast<MenuItem>((static_cast<int>(state.currentMenuItem) + 1) % 4);
// //       displayMenuItem();
// //       break;
      
// //     case EDITING_MODE:
// //       switch (state.currentMenuItem) {
// //         case EGG_DURATION:
// //           state.eggNozzleDuration = constrain(state.eggNozzleDuration + 1000, 1000, 30000);
// //           break;
// //         case FLOUR_DURATION:
// //           state.flourChamberDuration = constrain(state.flourChamberDuration + 1000, 1000, 60000);
// //           break;
// //         case MIXING_DURATION:
// //           state.mixingDuration = constrain(state.mixingDuration + 1000, 1000, 120000);
// //           break;
// //         default:
// //           break;
// //       }
// //       displayMenuItem();
// //       break;
// //   }
// // }
// void navigateMenu() {
//   switch (state.menuState) {
//     case MENU_MODE:
//       // Update to include START_STOP in navigation
//       state.currentMenuItem = static_cast<MenuItem>((static_cast<int>(state.currentMenuItem) + 1) % 5);
//       displayMenuItem();
//       break;
      
//     case EDITING_MODE:
//       switch (state.currentMenuItem) {
//         case EGG_DURATION:
//           state.eggNozzleDuration = constrain(state.eggNozzleDuration + 1000, 1000, 30000);
//           break;
//         case FLOUR_DURATION:
//           state.flourChamberDuration = constrain(state.flourChamberDuration + 1000, 1000, 60000);
//           break;
//         case MIXING_DURATION:
//           state.mixingDuration = constrain(state.mixingDuration + 1000, 1000, 120000);
//           break;
//         case START_STOP:
//           // Toggle process state when START_STOP is selected
//           if (!state.isProcessing) {
//             startCoatingProcess();
//             lcd.clear();
//             lcd.print("Process Started");
//             delay(1000);
//             state.menuState = NORMAL_MODE;
//           } else {
//             stopCoatingProcess();
//             lcd.clear();
//             lcd.print("Process Stopped");
//             delay(1000);
//             state.menuState = NORMAL_MODE;
//           }
//           break;
//         default:
//           break;
//       }
//       displayMenuItem();
//       break;
      
//     case NORMAL_MODE:
//       // No navigation in normal mode
//       break;
//   }
// }
// // void adjustTiming() {
// //   switch (state.menuState) {
// //     case MENU_MODE:
// //       if (state.currentMenuItem != EXIT_MENU) {
// //         state.menuState = EDITING_MODE;
// //         state.isEditing = true;
// //         displayMenuItem();
// //       }
// //       break;
      
// //     case EDITING_MODE:
// //       switch (state.currentMenuItem) {
// //         case EGG_DURATION:
// //           state.eggNozzleDuration = constrain(state.eggNozzleDuration - 1000, 1000, 30000);
// //           break;
// //         case FLOUR_DURATION:
// //           state.flourChamberDuration = constrain(state.flourChamberDuration - 1000, 1000, 60000);
// //           break;
// //         case MIXING_DURATION:
// //           state.mixingDuration = constrain(state.mixingDuration - 1000, 1000, 120000);
// //           break;
// //         default:
// //           break;
// //       }
// //       displayMenuItem();
// //       break;
// //   }
// // }
// // Update adjustTiming to handle START_STOP
// void adjustTiming() {
//   switch (state.menuState) {
//     case MENU_MODE:
//       if (state.currentMenuItem != EXIT_MENU && state.currentMenuItem != START_STOP) {
//         state.menuState = EDITING_MODE;
//         state.isEditing = true;
//         displayMenuItem();
//       } else if (state.currentMenuItem == START_STOP) {
//         // Handle start/stop action
//         if (!state.isProcessing) {
//           startCoatingProcess();
//           lcd.clear();
//           lcd.print("Process Started");
//         } else {
//           stopCoatingProcess();
//           lcd.clear();
//           lcd.print("Process Stopped");
//         }
//         delay(1000);
//         state.menuState = NORMAL_MODE;
//       }
//       break;
      
//     case EDITING_MODE:
//       switch (state.currentMenuItem) {
//         case EGG_DURATION:
//           state.eggNozzleDuration = constrain(state.eggNozzleDuration - 1000, 1000, 30000);
//           break;
//         case FLOUR_DURATION:
//           state.flourChamberDuration = constrain(state.flourChamberDuration - 1000, 1000, 60000);
//           break;
//         case MIXING_DURATION:
//           state.mixingDuration = constrain(state.mixingDuration - 1000, 1000, 120000);
//           break;
//         default:
//           break;
//       }
//       displayMenuItem();
//       break;
      
//     case NORMAL_MODE:
//       // No adjustment in normal mode
//       break;
//   }
// }

// // Update displayMenuItem to include the start/stop option
// void displayMenuItem() {
//   lcd.clear();
//   lcd.setCursor(0, 0);
  
//   switch (state.currentMenuItem) {
//     case EGG_DURATION:
//       lcd.print("Egg Duration:");
//       lcd.setCursor(0, 1);
//       lcd.print(state.eggNozzleDuration / 1000);
//       lcd.print(" seconds");
//       break;
      
//     case FLOUR_DURATION:
//       lcd.print("Flour Duration:");
//       lcd.setCursor(0, 1);
//       lcd.print(state.flourChamberDuration / 1000);
//       lcd.print(" seconds");
//       break;
      
//     case MIXING_DURATION:
//       lcd.print("Mixing Duration:");
//       lcd.setCursor(0, 1);
//       lcd.print(state.mixingDuration / 1000);
//       lcd.print(" seconds");
//       break;
      
//     case START_STOP:
//       lcd.print(state.isProcessing ? "Stop Process?" : "Start Process?");
//       lcd.setCursor(0, 1);
//       lcd.print("Press BTN2");
//       break;
      
//     case EXIT_MENU:
//       lcd.print("Exit Menu");
//       lcd.setCursor(0, 1);
//       break;
//   }
  
//   if (state.isEditing) {
//     lcd.setCursor(15, 0);
//     lcd.print("*");
//   }
// }

// // void updateLCDDisplay() {
// //   if (state.menuState == NORMAL_MODE) {
// //     lcd.clear();
// //     lcd.setCursor(0, 0);
// //     lcd.print("Speed: ");
// //     lcd.print(state.flourSpeed);
    
// //     lcd.setCursor(0, 1);
// //     if (state.isProcessing) {
// //      lcd.print("Running ");
// //       // Display remaining time for current phase
// //       unsigned long currentTime = millis();
// //       if (state.eggNozzleActive) {
// //         lcd.setCursor(12, 1);
// //         lcd.print((state.eggNozzleDuration - (currentTime - state.eggNozzleTimer)) / 1000);
// //       } else if (currentTime - state.flourChamberTimer < state.flourChamberDuration) {
// //         lcd.setCursor(12, 1);
// //         lcd.print((state.flourChamberDuration - (currentTime - state.flourChamberTimer)) / 1000);
// //       } else {
// //         lcd.setCursor(12, 1);
// //         lcd.print((state.mixingDuration - (currentTime - state.mixingTimer)) / 1000);
// //       }
// //     } else {
// //       lcd.print("Ready");
// //     }
// //   }
// // }
// void updateLCDDisplay() {
//   if (state.menuState == NORMAL_MODE) {
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Speed: ");
//     lcd.print(state.flourSpeed);
    
//     lcd.setCursor(0, 1);
//     if (state.isProcessing) {
//       lcd.print("Running ");
//       // Display remaining time for current phase
//       unsigned long currentTime = millis();
//       if (state.eggNozzleActive) {
//         lcd.setCursor(8, 1);
//         lcd.print("E:");
//         lcd.print((state.eggNozzleDuration - (currentTime - state.eggNozzleTimer)) / 1000);
//       } else if (currentTime - state.flourChamberTimer < state.flourChamberDuration) {
//         lcd.setCursor(8, 1);
//         lcd.print("F:");
//         lcd.print((state.flourChamberDuration - (currentTime - state.flourChamberTimer)) / 1000);
//       } else {
//         lcd.setCursor(8, 1);
//         lcd.print("M:");
//         lcd.print((state.mixingDuration - (currentTime - state.mixingTimer)) / 1000);
//       }
//     } else {
//       lcd.print("press menu ");
//     }
//   }
// }
// // // Error handling helper function

// // // Safety check function
// // / Enhanced safety checks
// bool performSafetyChecks() {
//   bool isSystemSafe = true;
  
//   // Check error count
//   if (state.errorCount >= state.MAX_ERRORS) {
//     handleError("Too Many Errors");
//     isSystemSafe = false;
//   }
  
//   // Check WiFi status
//   if (WiFi.softAPgetStationNum() == 0 && state.isProcessing) {
//     handleError("WiFi Disconnected");
//     isSystemSafe = false;
//   }
  
//   return isSystemSafe;
// }

// // Helper function to load sequence into active state
// void loadSequenceToState() {
//   state.eggNozzleDuration = savedSequence.eggNozzleDuration;
//   state.flourChamberDuration = savedSequence.flourChamberDuration;
//   state.mixingDuration = savedSequence.mixingDuration;
//   state.flourSpeed = savedSequence.flourSpeed;
//   state.mixingSpeed = savedSequence.mixingSpeed;
// }

// // Helper function to save current state to sequence
// void saveStateToSequence() {
//   savedSequence.eggNozzleDuration = state.eggNozzleDuration;
//   savedSequence.flourChamberDuration = state.flourChamberDuration;
//   savedSequence.mixingDuration = state.mixingDuration;
//   savedSequence.flourSpeed = state.flourSpeed;
//   savedSequence.mixingSpeed = state.mixingSpeed;
//   saveSequence();
// }

// // Emergency stop function
// void emergencyStop() {
//   // Immediately stop all operations
//   stopCoatingProcess();
//   state.isProcessing = false;
//   state.eggNozzleActive = false;
  
//   // Alert user
//   digitalWrite(ALARM_PIN, HIGH);
//   lcd.clear();
//   lcd.print("EMERGENCY STOP");
//   lcd.setCursor(0, 1);
//   // lcd.print("System Halted");
  
//   delay(2000);
//   digitalWrite(ALARM_PIN, LOW);
  
//   // Require system reset or manual restart
//   while (digitalRead(BTN1_PIN) == HIGH) {
//     delay(100);  // Wait for user acknowledgment
//   }
  
//   // Reset system
//   setup();
// }

// // System diagnostics function
// void runDiagnostics() {
//   lcd.clear();
//   lcd.print("Diagnostics...");
  
//   // Test motors
//   lcd.setCursor(0, 1);
//   lcd.print("Testing Motors");
//   controlFlourChamber(128);
//   delay(1000);
//   controlFlourChamber(0);
//   controlMixingMotor(128);
//   delay(1000);
//   controlMixingMotor(0);
  
//   // Test egg nozzle
//   lcd.setCursor(0, 1);
//   lcd.print("Testing Nozzle");
//   controlEggNozzle(true);
//   delay(500);
//   controlEggNozzle(false);
  
//   // Test alarm
//   lcd.setCursor(0, 1);
//   lcd.print("Testing Alarm");
//   digitalWrite(ALARM_PIN, HIGH);
//   delay(500);
//   digitalWrite(ALARM_PIN, LOW);
  
//   lcd.clear();
//   lcd.print("Diagnostics OK");
//   delay(1000);
// }

// // Watchdog function to prevent system hanging
// void systemWatchdog() {
//   static unsigned long lastWatchdogReset = 0;
//   static const unsigned long WATCHDOG_TIMEOUT = 10000; // 5 seconds
  
//   if (millis() - lastWatchdogReset > WATCHDOG_TIMEOUT) {
//     if (state.isProcessing) {
//       // If system appears hung during processing, emergency stop
//       handleError("Watchdog Timeout");
//       // emergencyStop();
//     }
//   }
//   lastWatchdogReset = millis();
// }



// void safeLoop() {
//   unsigned long currentMillis = millis();
  
//   // Watchdog check
//   // if (currentMillis - state.lastWatchdogReset > WATCHDOG_TIMEOUT) {
//   //   handleError("Watchdog Timeout");
//   //   emergencyStop();
//   //   return;
//   // }
//     // unsigned long currentMillis = millis();
  
//   // Check WiFi status periodically
//   if (currentMillis - state.lastWifiCheck > WIFI_CHECK_INTERVAL) {
//     if (wifiConnected && WiFi.status() != WL_CONNECTED) {
//       // Try to reconnect to WiFi if connection is lost
//       WiFi.begin(wifi_ssid, wifi_password);
//       // Wait briefly for connection
//       delay(500);
//       if (WiFi.status() != WL_CONNECTED) {
//         // If reconnection fails, start AP mode
//         WiFi.mode(WIFI_AP);
//         WiFi.softAP(ap_ssid, ap_password, 1, 0, MAX_CLIENTS);
//         wifiConnected = false;
//       }
//     }
//     state.lastWifiCheck = currentMillis;
//   }
//   // // WiFi status check
//   // if (currentMillis - state.lastWifiCheck > WIFI_CHECK_INTERVAL) {
//   //   Serial.printf("Connected clients: %d\n", WiFi.softAPgetStationNum());
//   //   state.lastWifiCheck = currentMillis;
//   // }
  
//   try {
//     // if (!performSafetyChecks()) {
//     //   // emergencyStop();
//     //   return;
//     // }
    
//     handleButtons();
//     handlePotentiometer();
    
//     if (state.isProcessing) {
//       updateProcessTimers();
//     }
    
//     updateLCDDisplay();
    
//     state.lastWatchdogReset = currentMillis;
    
//   } catch (...) {
//     handleError("System Error");
//     // emergencyStop();
//   }
// }



// // Enhanced error handling
// void handleError(const char* errorMessage) {
//   Serial.println(errorMessage);
//   digitalWrite(ERROR_LED_PIN, HIGH);
//   state.errorCount++;
  
//   lcd.clear();
//   lcd.print("Error:");
//   lcd.setCursor(0, 1);
//   lcd.print(errorMessage);
  
//   // Log error to preferences
//   preferences.putString("lastError", errorMessage);
//   preferences.putInt("errorCount", state.errorCount);
  
//   delay(2000);
//   digitalWrite(ERROR_LED_PIN, LOW);
//   lcd.clear();
// }







//neew dumps


// void updateProcessTimers() {
//   unsigned long currentTime = millis();
  
//   // Handle egg nozzle timing
//   if (state.eggNozzleActive && (currentTime - state.eggNozzleTimer >= state.eggNozzleDuration)) {
//     controlEggNozzle(false);
//   }
  
//   // Handle flour chamber timing
//   static bool flourChamberActive = false;
//   if (state.isProcessing) {
//     // Initial flour addition
//     if (!flourChamberActive && (currentTime - state.flourChamberTimer < state.flourChamberDuration)) {
//       flourChamberActive = true;
//       controlFlourChamber(state.flourSpeed);
//     }
//     // Stop flour chamber after duration
//     else if (flourChamberActive && (currentTime - state.flourChamberTimer >= state.flourChamberDuration)) {
//       flourChamberActive = false;
//       controlFlourChamber(0);
//     }
    
//     // Handle interval-based flour addition
//     if (state.flourInterval > 0 && !flourChamberActive && 
//         (currentTime - state.lastFlourAddition >= state.flourInterval)) {
//       // Start new flour addition cycle
//       state.flourChamberTimer = currentTime;
//       state.lastFlourAddition = currentTime;
//       flourChamberActive = true;
//       controlFlourChamber(state.flourSpeed);
//     }
//   }
  
//   // Handle egg intervals during mixing
//   if (state.isProcessing && !state.eggNozzleActive && 
//       state.eggInterval > 0 && (currentTime - state.lastEggAddition >= state.eggInterval)) {
//     controlEggNozzle(true);
//     state.lastEggAddition = currentTime;
//     state.eggNozzleTimer = currentTime;
//   }
  
//   // Check if total mixing duration is complete
//   if (state.isProcessing && (currentTime - state.mixingTimer >= state.mixingDuration)) {
//     stopCoatingProcess();
//   }
// }


// void updateProcessTimers() {
//   if (!state.isProcessing) {
//     return;  // Exit if not processing
//   }
  
//   unsigned long currentTime = millis();
  
//   // Debug output
//   Serial.print("Flour motor state: ");
//   Serial.println(flourMotorRunning ? "Running" : "Stopped");
  
//   // Handle egg nozzle timing
//   if (state.eggNozzleActive && (currentTime - state.eggNozzleTimer >= state.eggNozzleDuration)) {
//     Serial.println("Stopping egg nozzle");
//     controlEggNozzle(false);
//   }
  
//   // Handle flour chamber timing
//   if (flourMotorRunning) {
//     if (currentTime - state.flourChamberTimer >= state.flourChamberDuration) {
//       Serial.println("Stopping flour chamber - duration complete");
//       controlFlourChamber(0);
//       flourMotorRunning = false;
//     }
//   } else if (state.flourInterval > 0 && 
//              (currentTime - state.lastFlourAddition >= state.flourInterval)) {
//     // Start new flour cycle
//     Serial.println("Starting new flour cycle");
//     state.flourChamberTimer = currentTime;
//     state.lastFlourAddition = currentTime;
//     controlFlourChamber(state.flourSpeed);
//     flourMotorRunning = true;
//   }
  
//   // Handle egg intervals
//   if (!state.eggNozzleActive && state.eggInterval > 0 && 
//       (currentTime - state.lastEggAddition >= state.eggInterval)) {
//     Serial.println("Adding egg based on interval");
//     controlEggNozzle(true);
//     state.lastEggAddition = currentTime;
//     state.eggNozzleTimer = currentTime;
//   }
  
//   // Check for process completion
//   if (currentTime - state.mixingTimer >= state.mixingDuration) {
//     Serial.println("Process complete - stopping all components");
//     stopCoatingProcess();
//   }
// }



// void updateLCDDisplay() {
//   if (state.menuState == NORMAL_MODE) {
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("mixing Speed: ");
//     lcd.print(state.mixingSpeed);
    
//     lcd.setCursor(0, 1);
//     if (state.isProcessing) {
//       lcd.print("Running ");
//       // Display remaining time for current phase
//       unsigned long currentTime = millis();
//       if (state.eggNozzleActive) {
//         lcd.setCursor(8, 1);
//         lcd.print("E:");
//         lcd.print((state.eggNozzleDuration - (currentTime - state.eggNozzleTimer)) / 1000);
//       } else if (currentTime - state.flourChamberTimer < state.flourChamberDuration) {
//         lcd.setCursor(8, 1);
//         lcd.print("F:");
//         lcd.print((state.flourChamberDuration - (currentTime - state.flourChamberTimer)) / 1000);
//       } else {
//         lcd.setCursor(8, 1);
//         lcd.print("M:");
//         lcd.print((state.mixingDuration - (currentTime - state.mixingTimer)) / 1000);
//       }
//     } else {
//       lcd.print("press menu ");
//     }
//   }
// }
// / Enhanced display function to show intervals


// void updateLCDDisplay() {
//   if (state.menuState == NORMAL_MODE) {
//     lcd.clear();
//     lcd.setCursor(0, 0);
    
//     // Show active process status
//     if (state.isProcessing) {
//       unsigned long currentTime = millis();
      
//       // First row shows current phase
//       if (state.eggNozzleActive) {
//         lcd.print("Egg Active ");
//         lcd.print((state.eggNozzleDuration - (currentTime - state.eggNozzleTimer)) / 1000);
//         lcd.print("s");
//       } else if (currentTime - state.flourChamberTimer < state.flourChamberDuration) {
//         lcd.print("Flour Active ");
//         lcd.print((state.flourChamberDuration - (currentTime - state.flourChamberTimer)) / 1000);
//         lcd.print("s");
//       } else {
//         lcd.print("Mixing ");
//         lcd.print((state.mixingDuration - (currentTime - state.mixingTimer)) / 1000);
//         lcd.print("s");
//       }
      
//       // Second row shows next interval
//       lcd.setCursor(0, 1);
//       unsigned long nextEgg = state.eggInterval - (currentTime - state.lastEggAddition);
//       unsigned long nextFlour = state.flourInterval - (currentTime - state.lastFlourAddition);
      
//       if (nextEgg < nextFlour) {
//         lcd.print("Next Egg: ");
//         lcd.print(nextEgg / 1000);
//         lcd.print("s");
//       } else {
//         lcd.print("Next Flour: ");
//         lcd.print(nextFlour / 1000);
//         lcd.print("s");
//       }
//     } else {
//       lcd.print("Ready");
//       lcd.setCursor(0, 1);
//       lcd.print("Press Menu");
//     }
//   }
// }


// // Modified motor control functions
// void controlFlourChamber(int speed) {
//   analogWrite(ENA_PIN, abs(speed));  // Use absolute value for speed
  
//   if (speed > 0) {
//     digitalWrite(IN1_PIN, LOW);
//     digitalWrite(IN2_PIN, HIGH);
//   } 
//   else if (speed < 0) {
//     digitalWrite(IN1_PIN, HIGH);
//     digitalWrite(IN2_PIN, LOW);
//   }
//   else {
//     digitalWrite(IN1_PIN, LOW);
//     digitalWrite(IN2_PIN, LOW);
//   }
// }

// void controlMixingMotor(int speed) {
//   analogWrite(ENB_PIN, abs(speed));
  
//   if (speed > 0) {
//     digitalWrite(IN3_PIN, LOW);
//     digitalWrite(IN4_PIN, HIGH);
//   }
//   else if (speed < 0) {
//     digitalWrite(IN3_PIN, HIGH);
//     digitalWrite(IN4_PIN, LOW);
//   }
//   else {
//     digitalWrite(IN3_PIN, LOW);
//     digitalWrite(IN4_PIN, LOW);
//   }
// }



// void handlePotentiometer() {
//   int potValue = analogRead(POT_PIN);
//   potValue = constrain(potValue, 0, 4095);
  
//   if (state.menuState == EDITING_MODE) {
//     // Use unsigned long for timing values to prevent negative readings
//     unsigned long mappedValue;
//     switch (state.currentMenuItem) {
//       case EGG_DURATION:
//         mappedValue = map(potValue, 0, 4095, 1000, 600000);
//         state.eggNozzleDuration = mappedValue;
//         break;
//       case FLOUR_DURATION:
//         mappedValue = map(potValue, 0, 4095, 1000, 600000);
//         state.flourChamberDuration = mappedValue;
//         break;
//       case MIXING_DURATION:
//         mappedValue = map(potValue, 0, 4095, 1000, 1200000);
//         state.mixingDuration = mappedValue;
//         break;
//       case EGG_INTERVAL:
//         mappedValue = map(potValue, 0, 4095, 1000, 600000);
//         state.eggInterval = mappedValue;
//         break;
//       case FLOUR_INTERVAL:
//         mappedValue = map(potValue, 0, 4095, 1000, 600000);
//         state.flourInterval = mappedValue;
//         break;
//       default:
//         break;
//     }
//     displayMenuItem();
//   } else if (state.menuState == NORMAL_MODE) {
//     state.mixingSpeed = map(potValue, 0, 4095, 0, 255);
//     if (state.isProcessing) {
//       controlMixingMotor(state.mixingSpeed);
//     }
//   }
// }