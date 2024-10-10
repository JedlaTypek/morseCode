#include <Arduino.h>

#define LED_PIN 2
#define BUZZER_PIN 3

#define COUNT_OF_CHARS 36

#define DOT_TIME 150
#define DASH_TIME 300
#define SPACE_TIME 600

String chars[COUNT_OF_CHARS][2] = {
  {"A", ".-"},
  {"B", "-..."},
  {"C", "-.-."},
  {"D", "-.."},
  {"E", "."},
  {"F", "..-."},
  {"G", "--."},
  {"H", "...."},
  {"I", ".."},
  {"J", ".---"},
  {"K", "-.-"},
  {"L", ".-.."},
  {"M", "--"},
  {"N", "-."},
  {"O", "---"},
  {"P", ".--."},
  {"Q", "--.-"},
  {"R", ".-."},
  {"S", "..."},
  {"T", "-"},
  {"U", "..-"},
  {"V", "...-"},
  {"W", ".--"},
  {"X", "-..-"},
  {"Y", "-.--"},
  {"Z", "--.."},
  {"1", ".----"},
  {"2", "..---"},
  {"3", "...--"},
  {"4", "....-"},
  {"5", "....."},
  {"6", "-...."},
  {"7", "--..."},
  {"8", "---.."},
  {"9", "----."},
  {"0", "-----"}
};

String encodeMorse(String input) {
  input.toUpperCase();
  String morse = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    if(c == ' '){
      morse += "/";
    }
    else if (c == '.' || c == '?' || c == '!') {
      morse += "//";
    } else{
      for (int j = 0; j < COUNT_OF_CHARS; j++) {
        if (chars[j][0] == String(c)) {
          morse += chars[j][1];
          morse += "/";
        }
      }
    }
  }
  if(morse[morse.length()-2] != '/'){
      morse += "/";
    }
  return morse;
}

String getInput() {
  String input = "";
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read(); // Přečte jeden znak
      if (c == '\n') { // Pokud uživatel stiskne Enter, ukončí čtení
        break;
      } else if (c == '\b' || c == 127) { // Detekce Backspace (127 je standardní hodnota pro Delete)
        if (input.length() > 0) {
          input.remove(input.length() - 1); // Smaže poslední znak
          Serial.print("\b \b"); // Posune kurzor zpět, smaže znak na terminálu
        }
      } else {
        input += c; // Přidá znak do vstupu
        Serial.print(c); // Vypíše znak zpět na terminál
      }
    }
  }
  input.trim(); // Odstraní mezery a nové řádky na začátku a konci
  return input;
}

void playMorse(String morse) {
  for (int i = 0; i < morse.length(); i++) {
    char c = morse[i];
    if (c == '.') {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(DOT_TIME);
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      delay(DOT_TIME);
    } else if (c == '-') {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(DASH_TIME);
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      delay(DOT_TIME);
    } else if (c == '/') {
      delay(SPACE_TIME);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);  
  Serial.begin(9600);
}

void loop() {
  Serial.print("Enter a string to convert it in the morse code: ");
  String input = getInput(); // Zavolá funkci pro získání vstupu
  Serial.println("\nEnterer string (english characters only!): " + input);
  String morse = encodeMorse(input); // Zavolá funkci pro zakódování vstupu do morseovy abecedy
  Serial.println("Morse code: " + morse);
  playMorse(morse); // Zavolá funkci pro přehrání morseovy abecedy
}
