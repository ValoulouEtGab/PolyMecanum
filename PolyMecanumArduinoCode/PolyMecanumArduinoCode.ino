#include <NewPing.h>
#include<SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);


//Moteur Avant Gauche
const int ENAVG = 2; //enable avant gauche...
const int INAVG1 = 23;
const int INAVG2 = 24;

//Moteur Avant Droite
const int ENAVD = 3;
const int INAVD1 = 26;
const int INAVD2 = 27;

//Moteur Arriere Gauche
const int ENARG = 4;
const int INARG1 = 29;
const int INARG2 = 30;

//Moteur Arriere Droite
const int ENARD = 5;
const int INARD1 = 32;
const int INARD2 = 33;


//Capteur Ultrason Avant
const int EchoAV = 42;
const int TrigAV = 43;

//Capteur Ultrason Arriere
const int EchoAR = 48;
const int TrigAR = 49;

//Capteur Ultrason Gauche
const int EchoG = 44;
const int TrigG = 45;

//Capteur Ultrason Droite
const int EchoD = 46;
const int TrigD = 47;

NewPing sonarAV(TrigAV,EchoAV);
NewPing sonarAR(TrigAR,EchoAR);
NewPing sonarG(TrigG,EchoG);
NewPing sonarD(TrigD,EchoD);

//Capteur IR
const int IRG = 6;
const int IRC = 7;
const int IRD = 8;


float distAV = 0;
float distAR = 0;
float distG = 0;
float distD = 0;
int vitesse = 100; //vitesse entre 0 et 255
String(message);
int distMIN = 26; // distance obstacle minimale en cm


void setup() {
  //Moteurs
  pinMode(ENAVG, OUTPUT);
  pinMode(INAVG1, OUTPUT);
  pinMode(INAVG2, OUTPUT);
  pinMode(ENAVD, OUTPUT);
  pinMode(INAVD1, OUTPUT);
  pinMode(INAVD2, OUTPUT);
  pinMode(ENARG, OUTPUT);
  pinMode(INARG1, OUTPUT);
  pinMode(INARG2, OUTPUT);
  pinMode(ENARD, OUTPUT);
  pinMode(INARD1, OUTPUT);
  pinMode(INARD2, OUTPUT);

  // Bluetooth
  Serial.begin(9600);
  delay(100);
  Serial.println("PolyMecanum ready !");
  BlueT.begin(9600);
  delay(100);

  // Allimentation 5V capteur ultrason arriere et droite
  pinMode(52, OUTPUT);
  digitalWrite(52, HIGH);
  pinMode(40, OUTPUT);
  digitalWrite(40, HIGH);

  // Allimentation 5V IR
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
  digitalWrite(34, HIGH);
  digitalWrite(35, LOW);
  digitalWrite(36, HIGH);
  digitalWrite(37, LOW);
  digitalWrite(38, HIGH);
  digitalWrite(39, LOW);
}


void loop() {
  if (Serial.available()) {  // Recupere les messages sur le Moniteur serie (pour la configuration AT)
    BlueT.write(Serial.read());
    delay(3);
  }
  
  // Recupere la distance obstacles
  distAV = sonarAV.ping_cm();
  distAR = sonarAR.ping_cm();
  distG = sonarG.ping_cm();
  distD = sonarD.ping_cm();
  delay(10);

  /*
  Serial.println("&&&&&&&&&&&&&&&&&&&&&");
  Serial.println(distAV);
  Serial.println(distAR);
  Serial.println(distG);
  Serial.println(distD);
  delay(100);*/
  
  // si le capteur ne detecte rien il renvoie 0 (donc erreur)
  if (distAV == 0){
    distAV = 300;
  }
  if (distAR == 0){
    distAR = 300;
  }
  if (distG == 0){
    distG = 300;
  }
  if (distD == 0){
    distD = 300;
  }

  if (distAV <= distMIN and (message == "a" or message == "e" or message == "f")){
    analogWrite(ENAVG,0);
    analogWrite(ENAVD,0);
    analogWrite(ENARG,0);
    analogWrite(ENARD,0);
    message = "z";
  }
  else if (distAR <= distMIN and (message == "b" or message == "g" or message == "h")){
    analogWrite(ENAVG,0);
    analogWrite(ENAVD,0);
    analogWrite(ENARG,0);
    analogWrite(ENARD,0);
    message = "z";
  }
  else if (distG <= distMIN and (message == "c" or message == "e" or message == "g")){
    analogWrite(ENAVG,0);
    analogWrite(ENAVD,0);
    analogWrite(ENARG,0);
    analogWrite(ENARD,0);
    message = "z";
  }
  else if (distD <= distMIN and (message == "d" or message == "f" or message == "h")){
    analogWrite(ENAVG,0);
    analogWrite(ENAVD,0);
    analogWrite(ENARG,0);
    analogWrite(ENARD,0);
    message = "z";
  }
  
  
  if(BlueT.available()){
    message = (char)BlueT.read(); // On recupere le message sous forme de caractere
    delay(6);
    
    if (message == "A"){ // Sert à indiquer que les caracteres qui vont suivre "A" (attention) est un nombre
      String newVitesseSring = "";
      while (message != "F"){ // Tant qu'on a pas recu de "F" (fin du nombre)
        delay(6);
        message = (char)BlueT.read(); // Recupere 1 caractere
        delay(6);
        // Verifie si le caractere est un chiffre pour former le nombre correctement
        if (message == "0" or message == "1" or message == "2" or message == "3" or message == "4"
        or message == "5" or message == "6" or message == "7" or message == "8" or message == "9"){
          newVitesseSring = newVitesseSring + message;
        }
      }
      Serial.println("Vitesse changee a : "+newVitesseSring+"/255");
      
      int newVitesse = newVitesseSring.toInt(); // Recupere la valeur (int) de la vitesse
      if (newVitesse >= 0 and newVitesse <= 255){ // Verifie si la vitesse correspond
        vitesse = newVitesse; // Change la vitesse
      }
    }
    
    else if (message == "a" and distAV > distMIN){
      Serial.println("Avancer");

      // Sens des moteurs
      digitalWrite(INAVG1, LOW);
      digitalWrite(INAVG2, HIGH);
      digitalWrite(INAVD1, HIGH);
      digitalWrite(INAVD2, LOW);
      digitalWrite(INARG1, HIGH);
      digitalWrite(INARG2, LOW);
      digitalWrite(INARD1, LOW);
      digitalWrite(INARD2, HIGH);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
      analogWrite(ENARD,vitesse);
    }
    
    else if (message == "b" and distAR > distMIN){
      Serial.println("Reculer");

      // Sens des moteurs
      digitalWrite(INAVG1, HIGH);
      digitalWrite(INAVG2, LOW);
      digitalWrite(INAVD1, LOW);
      digitalWrite(INAVD2, HIGH);
      digitalWrite(INARG1, LOW);
      digitalWrite(INARG2, HIGH);
      digitalWrite(INARD1, HIGH);
      digitalWrite(INARD2, LOW);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "c" and distG > distMIN){
      Serial.println("Gauche");

      // Sens des moteurs
      digitalWrite(INAVG1, LOW);
      digitalWrite(INAVG2, HIGH);
      digitalWrite(INAVD1, LOW);
      digitalWrite(INAVD2, HIGH);
      digitalWrite(INARG1, LOW);
      digitalWrite(INARG2, HIGH);
      digitalWrite(INARD1, LOW);
      digitalWrite(INARD2, HIGH);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "d" and distD > distMIN){
      Serial.println("Droite");

      // Sens des moteurs
      digitalWrite(INAVG1, HIGH);
      digitalWrite(INAVG2, LOW);
      digitalWrite(INAVD1, HIGH);
      digitalWrite(INAVD2, LOW);
      digitalWrite(INARG1, HIGH);
      digitalWrite(INARG2, LOW);
      digitalWrite(INARD1, HIGH);
      digitalWrite(INARD2, LOW);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "e" and distAV > distMIN and distG > distMIN){
      Serial.println("Diagonale Avant Gauche");

      // Sens des moteurs
      digitalWrite(INAVG1, LOW);
      digitalWrite(INAVG2, HIGH);
      digitalWrite(INARD1, LOW);
      digitalWrite(INARD2, HIGH);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "f" and distAV > distMIN and distD > distMIN){
      Serial.println("Diagonale Avant Droite");

      // Sens des moteurs
      digitalWrite(INAVD1, HIGH);
      digitalWrite(INAVD2, LOW);
      digitalWrite(INARG1, HIGH);
      digitalWrite(INARG2, LOW);

      // Vitesse des moteurs
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
    }

    else if (message == "g" and distAR > distMIN and distG > distMIN){
      Serial.println("Diagonale Arriere Gauche");

      // Sens des moteurs
      digitalWrite(INAVD1, LOW);
      digitalWrite(INAVD2, HIGH);
      digitalWrite(INARG1, LOW);
      digitalWrite(INARG2, HIGH);

      // Vitesse des moteurs
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
    }

    else if (message == "h" and distAR > distMIN and distD > distMIN){
      Serial.println("Diagonale Arriere Droite");

      // Sens des moteurs
      digitalWrite(INAVG1, HIGH);
      digitalWrite(INAVG2, LOW);
      digitalWrite(INARD1, HIGH);
      digitalWrite(INARD2, LOW);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "i"){
      Serial.println("Tourner Gauche");

      // Sens des moteurs
      digitalWrite(INAVG1, HIGH);
      digitalWrite(INAVG2, LOW);
      digitalWrite(INAVD1, HIGH);
      digitalWrite(INAVD2, LOW);
      digitalWrite(INARG1, LOW);
      digitalWrite(INARG2, HIGH);
      digitalWrite(INARD1, LOW);
      digitalWrite(INARD2, HIGH);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "j"){
      Serial.println("Tourner Droite");
      
      // Sens des moteurs
      digitalWrite(INAVG1, LOW);
      digitalWrite(INAVG2, HIGH);
      digitalWrite(INAVD1, LOW);
      digitalWrite(INAVD2, HIGH);
      digitalWrite(INARG1, HIGH);
      digitalWrite(INARG2, LOW);
      digitalWrite(INARD1, HIGH);
      digitalWrite(INARD2, LOW);

      // Vitesse des moteurs
      analogWrite(ENAVG,vitesse);
      analogWrite(ENAVD,vitesse);
      analogWrite(ENARG,vitesse);
      analogWrite(ENARD,vitesse);
    }

    else if (message == "z"){
      Serial.println("STOP");
      // Vitesse des moteurs
      analogWrite(ENAVG,0);
      analogWrite(ENAVD,0);
      analogWrite(ENARG,0);
      analogWrite(ENARD,0);
    }

    else if (message == "S"){
      Serial.println("Suiveur de ligne active");
      String memoire = "";
      while (message != "s"){
        if(BlueT.available()){
          message = (char)BlueT.read(); // On recupere le message sous forme de caractere
          delay(3);
        }
        
        // Lecture des valeurs des capteurs IR
        int valeurIRG = digitalRead(IRG);
        int valeurIRC = digitalRead(IRC);
        int valeurIRD = digitalRead(IRD);

        // Sens des moteurs
        digitalWrite(INAVG1, LOW);
        digitalWrite(INAVG2, HIGH);
        digitalWrite(INAVD1, HIGH);
        digitalWrite(INAVD2, LOW);
        digitalWrite(INARG1, HIGH);
        digitalWrite(INARG2, LOW);
        digitalWrite(INARD1, LOW);
        digitalWrite(INARD2, HIGH);

        
        // Recupere la distance obstacles
        distAV = sonarAV.ping_cm();
        delay(10);
        // si le capteur ne detecte rien il renvoie 0 (donc erreur)
        if (distAV == 0){
          distAV = 300;
        }
        
        float coefVitesse = 0.4;


        if (distAV <= distMIN){
          Serial.println("Obstacle -> STOP");
          // Vitesse des moteurs
          analogWrite(ENAVG, 0);
          analogWrite(ENAVD, 0);
          analogWrite(ENARG, 0);
          analogWrite(ENARD, 0);
        }

        // Si le capteur central détecte la bande noire alors avancer droit
        else if (valeurIRG == LOW && valeurIRC == HIGH && valeurIRD == LOW){
          Serial.println("NOIR Centre -> Avancer tout droit");
          // Vitesse des moteurs
          analogWrite(ENAVG, vitesse);
          analogWrite(ENAVD, vitesse);
          analogWrite(ENARG, vitesse);
          analogWrite(ENARD, vitesse);
        }
        
        // Si le capteur de gauche détecte la bande noire alors tourner légèrement à gauche
        else if (valeurIRG == HIGH && valeurIRC == LOW && valeurIRD == LOW){
          memoire = "Gauche";
          Serial.println("NOIR Gauche -> Tourner légèrement à gauche");
          // Vitesse des moteurs
          analogWrite(ENAVG, vitesse * coefVitesse); // Réduire la vitesse du moteur
          analogWrite(ENAVD, vitesse);
          analogWrite(ENARG, vitesse * coefVitesse);
          analogWrite(ENARD, vitesse);
        }
        
        // Si le capteur de droite détecte la bande noire alors tourner légèrement à droite
        else if (valeurIRG == LOW && valeurIRC == LOW && valeurIRD == HIGH){
          memoire = "Droite";
          Serial.println("NOIR Droite -> Tourner légèrement à droite");
          // Vitesse des moteurs
          analogWrite(ENAVG, vitesse);
          analogWrite(ENAVD, vitesse * coefVitesse); // Réduire la vitesse
          analogWrite(ENARG, vitesse);
          analogWrite(ENARD, vitesse * coefVitesse);
        }

        else if (memoire == "Gauche"){ // Perdu à droite
          Serial.println("Perdu à droite");
          // Vitesse des moteurs
          analogWrite(ENAVG, 0);
          analogWrite(ENAVD, vitesse);
          analogWrite(ENARG, 0);
          analogWrite(ENARD, vitesse);
        }

        else if (memoire == "Droite"){ // Perdu à gauche
          Serial.println("Perdu à gauche");
          // Vitesse des moteurs
          analogWrite(ENAVG, vitesse);
          analogWrite(ENAVD, 0);
          analogWrite(ENARG, vitesse);
          analogWrite(ENARD, 0);
        }
      }
      Serial.println("Suiveur de ligne desactive");
      // Vitesse des moteurs STOP
      analogWrite(ENAVG, 0);
      analogWrite(ENAVD, 0);
      analogWrite(ENARG, 0);
      analogWrite(ENARD, 0);
    }
  }

  
  /*if (distAV > distMIN and distAR > distMIN and distG > distMIN and distD > distMIN) {
    // Acceleration moteur à la vitesse = ...
    for (int v = 40; v < vitesse; v=v + 1){
      analogWrite(ENAVG,v);
      analogWrite(ENAVD,v);
      analogWrite(ENARG,v);
      analogWrite(ENARD,v);
      delay(10); // Modifie le temps d'acceleration
      }
    }*/
     
}
