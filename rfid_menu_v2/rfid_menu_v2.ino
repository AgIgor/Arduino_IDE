#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 9

#define pin_up 3
#define pin_down 2
#define pin_ok 4

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instância do objeto MFRC522
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço do display LCD 16x2

enum Menu { CADASTRO, GERENCIAMENTO, REMOCAO };
Menu menuState = CADASTRO;
byte selectedID = 0;
byte adminID[4] = {0x16, 0x40, 0xC7, 0x73}; // ID da tag de administrador

void setup() {
  Serial.begin(9600);
  
  pinMode(pin_up, INPUT_PULLUP);
  pinMode(pin_down, INPUT_PULLUP);
  pinMode(pin_ok, INPUT_PULLUP);
  
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  RFID Menu  ");
  lcd.setCursor(0, 1);
  lcd.print("  Aguardando  ");
  delay(2000);
  lcd.clear();
  lcd.print("  Aproxime   ");
  lcd.setCursor(0, 1);
  lcd.print("a tag admin  ");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte newID[4];
    for (byte i = 0; i < 4; i++) {
      newID[i] = mfrc522.uid.uidByte[i];
    }

    if (isAdminTag(newID)) {
      handleAdminTag();
    } else {
      handleRegularTag(newID);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

boolean isAdminTag(byte* tagID) {
  for (byte i = 0; i < 4; i++) {
    if (tagID[i] != adminID[i])
      return false;
  }
  return true;
}

void handleAdminTag() {
  lcd.clear();
  lcd.print("Tag admin lida");
  delay(2000);
  while (true) {
    //delay(100);
    lcd.clear();
    lcd.print("  MENU ADMIN  ");
    lcd.setCursor(0, 1);
    lcd.print(" 1 - Cadastro ");
    lcd.setCursor(0, 2);
    lcd.print(" 2 - Gerenc.  ");
    lcd.setCursor(0, 3);
    lcd.print(" 3 - Remocao  ");

    byte button = checkButtons();

    if (button == 1) {
      menuState = CADASTRO;
      break;
    } else if (button == 2) {
      menuState = GERENCIAMENTO;
      break;
    } else if (button == 3) {
      menuState = REMOCAO;
      break;
    }
  }

  handleMenuState();
}

void handleRegularTag(byte* tagID) {
  lcd.clear();
  lcd.print(" Tag regular  ");
  delay(2000);
  while (true) {
    lcd.clear();
    lcd.print("  MENU TAG   ");
    lcd.setCursor(0, 1);
    lcd.print(" 1 - Ler ID  ");
    lcd.setCursor(0, 2);
    lcd.print(" 2 - Sair    ");

    byte button = checkButtons();

    if (button == 1) {
      lcd.clear();
      lcd.print(" ID:");
      lcd.setCursor(0, 1);
      printTagID(tagID);
      delay(2000);
      break;
    } else if (button == 2) {
      break;
    }
  }
}

void handleMenuState() {
  if (menuState == CADASTRO) {
    handleCadastro();
  } else if (menuState == GERENCIAMENTO) {
    handleGerenciamento();
  } else if (menuState == REMOCAO) {
    handleRemocao();
  }
}

void handleCadastro() {
  lcd.clear();
  lcd.print("  CADASTRO   ");
  lcd.setCursor(0, 1);
  lcd.print("Pressione OK");

  while (true) {
    byte button = checkButtons();

    if (button == 1) {
      saveTagID();
      lcd.clear();
      lcd.print("  Tag salva! ");
      delay(2000);
      break;
    }
  }
}

void handleGerenciamento() {
  lcd.clear();
  lcd.print("GERENCIAMENTO");
  lcd.setCursor(0, 1);
  lcd.print(" Selecione ID");

  byte numTags = EEPROM.read(0);
  byte selectedID = 1;

  while (true) {
    lcd.setCursor(0, 2);
    lcd.print("ID: ");
    lcd.print(selectedID);

    byte button = checkButtons();

    if (button == 1 && selectedID < numTags) {
      selectedID++;
    } else if (button == 2 && selectedID > 1) {
      selectedID--;
    } else if (button == 3) {
      lcd.clear();
      lcd.print("  ID selecionado");
      lcd.setCursor(0, 1);
      printTagID(selectedID);
      delay(2000);
      break;
    }
  }
}

void handleRemocao() {
  lcd.clear();
  lcd.print("  REMOCAO   ");
  lcd.setCursor(0, 1);
  lcd.print(" Selecione ID");

  byte numTags = EEPROM.read(0);
  byte selectedID = 1;

  while (true) {
    lcd.setCursor(0, 2);
    lcd.print("ID: ");
    lcd.print(selectedID);

    byte button = checkButtons();

    if (button == 1 && selectedID < numTags) {
      selectedID++;
    } else if (button == 2 && selectedID > 1) {
      selectedID--;
    } else if (button == 3) {
      removeTag(selectedID);
      lcd.clear();
      lcd.print("  ID removido! ");
      delay(2000);
      break;
    }
  }
}

byte checkButtons() {
  // Implemente a lógica para verificar os botões (pode variar de acordo com o hardware utilizado)
  // Retorne 1 para o botão UP, 2 para DOWN e 3 para OK.
  if(!digitalRead(pin_up)){
    delay(30);
    Serial.println("pin_up");
    return 1;
  }
  if(!digitalRead(pin_down)){
    delay(30);
    Serial.println("pin_down");
    return 2;
  }
  if(!digitalRead(pin_ok)){
    delay(30);
    Serial.println("pin_ok");
    return 3;
  }
}

void printTagID(byte* tagID) {
  for (byte i = 0; i < 4; i++) {
    if (tagID[i] < 0x10) {
      lcd.print("0");
    }
    lcd.print(tagID[i], HEX);
    lcd.print(" ");
  }
}

void saveTagID() {
  byte numTags = EEPROM.read(0);
  byte newTagID[4];

  for (byte i = 0; i < 4; i++) {
    newTagID[i] = mfrc522.uid.uidByte[i];
    EEPROM.write(numTags * 4 + i + 1, newTagID[i]);
  }

  EEPROM.write(0, numTags + 1);
}

void removeTag(byte tagIndex) {
  byte numTags = EEPROM.read(0);

  for (byte i = tagIndex; i < numTags; i++) {
    for (byte j = 0; j < 4; j++) {
      EEPROM.write(i * 4 + j + 1, EEPROM.read((i + 1) * 4 + j + 1));
    }
  }

  EEPROM.write(0, numTags - 1);
}
