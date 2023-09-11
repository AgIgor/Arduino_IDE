#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

struct User {
  byte id;
  char name[16];
};

#define MAX_USERS 10
User users[MAX_USERS];
int userCount = 0;

byte adminTag[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };

enum MenuState {
  MENU_MAIN,
  MENU_ADD_USER,
  MENU_REMOVE_USER,
  MENU_VIEW_USERS
};

MenuState currentMenu = MENU_MAIN;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.print("   RFID System   ");
  delay(2000);
  lcd.clear();
}

void loop() {
  delay(100);
  switch (currentMenu) {
    case MENU_MAIN:
      showMainMenu();
      break;
    case MENU_ADD_USER:
      addUser();
      break;
    case MENU_REMOVE_USER:
      removeUser();
      break;
    case MENU_VIEW_USERS:
      viewUsers();
      break;
  }
}

void showMainMenu() {
  lcd.clear();
  lcd.print("1. Add User");
  lcd.setCursor(0, 1);
  lcd.print("2. Remove User");

  byte readCard[4];
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    memcpy(readCard, rfid.uid.uidByte, 4);
    if (checkAdminTag(readCard)) {
      currentMenu = MENU_VIEW_USERS;
    }
  }

  if (Serial.available()) {
    int choice = Serial.read() - '0';
    switch (choice) {
      case 1:
        currentMenu = MENU_ADD_USER;
        break;
      case 2:
        currentMenu = MENU_REMOVE_USER;
        break;
      case 3:
        currentMenu = MENU_VIEW_USERS;
        break;
      case 4:
        currentMenu = MENU_MAIN;
        break;
    }
  }
}

void addUser() {
  lcd.clear();
  lcd.print("Add User");
  lcd.setCursor(0, 1);
  lcd.print("Place card...");

  byte readCard[4];
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    memcpy(readCard, rfid.uid.uidByte, 4);
    if (!isCardRegistered(readCard)) {
      char name[16];
      lcd.clear();
      lcd.print("Enter name:");
      while (Serial.available() == 0) {
        // wait for input
      }
      Serial.readBytesUntil('\n', name, sizeof(name) - 1);
      addUserToMemory(readCard, name);
      lcd.clear();
      lcd.print("User added!");
      delay(2000);
    } else {
      lcd.clear();
      lcd.print("Card exists!");
      delay(2000);
    }
    currentMenu = MENU_MAIN;
  }
}

void removeUser() {
  lcd.clear();
  lcd.print("Remove User");
  lcd.setCursor(0, 1);
  lcd.print("Place card...");

  byte readCard[4];
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    memcpy(readCard, rfid.uid.uidByte, 4);
    if (isCardRegistered(readCard)) {
      removeUserFromMemory(readCard);
      lcd.clear();
      lcd.print("User removed!");
      delay(2000);
    } else {
      lcd.clear();
      lcd.print("Card not found!");
      delay(2000);
    }
    currentMenu = MENU_MAIN;
  }
}

void viewUsers() {
  lcd.clear();
  lcd.print("Users:");

  for (int i = 0; i < userCount; i++) {
    lcd.setCursor(0, i + 1);
    lcd.print(users[i].name);
  }

  byte readCard[4];
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    memcpy(readCard, rfid.uid.uidByte, 4);
    if (checkAdminTag(readCard)) {
      currentMenu = MENU_MAIN;
    }
  }

  if (Serial.available()) {
    int choice = Serial.read() - '0';
    if (choice == 4)
      currentMenu = MENU_MAIN;
  }
}

bool checkAdminTag(byte cardId[4]) {
  for (int i = 0; i < 4; i++) {
    if (cardId[i] != adminTag[i])
      return false;
  }
  return true;
}

bool isCardRegistered(byte cardId[4]) {
  for (int i = 0; i < userCount; i++) {
    if (memcmp(cardId, users[i].id, 4) == 0) {
      return true;
    }
  }
  return false;
}

void addUserToMemory(byte cardId[4], char name[16]) {
  if (userCount < MAX_USERS) {
    users[userCount].id = cardId;
    strncpy(users[userCount].name, name, sizeof(users[userCount].name) - 1);
    userCount++;
    writeUsersToEEPROM();
  }
}

void removeUserFromMemory(byte cardId[4]) {
  int index = -1;
  for (int i = 0; i < userCount; i++) {
    if (memcmp(cardId, users[i].id, 4) == 0) {
      index = i;
      break;
    }
  }
  if (index >= 0) {
    for (int i = index; i < userCount - 1; i++) {
      users[i] = users[i + 1];
    }
    userCount--;
    writeUsersToEEPROM();
  }
}

void writeUsersToEEPROM() {
  int addr = 0;
  for (int i = 0; i < userCount; i++) {
    EEPROM.put(addr, users[i]);
    addr += sizeof(User);
  }
}

void readUsersFromEEPROM() {
  int addr = 0;
  User tempUser;
  while (addr < EEPROM.length()) {
    EEPROM.get(addr, tempUser);
    users[userCount] = tempUser;
    userCount++;
    addr += sizeof(User);
  }
}
