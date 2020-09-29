#include "TimerOne.h" // koristi hardverski timer1 za finiju pwm kontrolu
                      // i/ili pokretanje periodicne funkcije prekida

const byte Pesaci_crveno = 13; //pin na kome je crvena led lampica za pesake
const byte Pesaci_zeleno = 12; //pin na kome je zelena led lampica za pesake
const byte Auta_crveno = 11; // pin na kome je crvena led lampica za auta
const byte Auta_zuto = 10; // pin na kome je zuta led lampica za auta
const byte Auta_zeleno = 9; // pin na kome je zelena led lampica za auta
const byte Poziv_pesaka = 2; // taster za poziv pesaka

int a = 1; // pinovi na kojima se prikljucuju segmenti sedmosegmentnog displeja
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;

const int PESACI_CRVENO_AUTA_ZELENO = 1;  // Stanje 1
const int PESACI_CRVENO_AUTA_ZUTO = 2;  // Stanje 2
const int PESACI_ZELENO_AUTA_CRVENO = 3;  // Stanje 3

const int PERIOD_ZUTOG_SVETLA = 20;  //Period trajanja zutog svetla
const int PERIOD_ZELENOG_SVETLA = 60;  //Period trajanja zelenog svetla za pesake

volatile int Stanje = PESACI_CRVENO_AUTA_ZELENO; // Promenljiva za trenutno stanje
volatile int Sledece_stanje = PESACI_ZELENO_AUTA_CRVENO; // Promenljiva za sledece stanje
volatile int Brojac = 0;  // Vremenski brojac
volatile byte Zahtev_za_promenu = 0; 

void Pusti_pesake(){  // Ukoliko je ukljuceno crveno za pesake i zeleno za vozila
		      // ili ako je ukljuceno zuto, a sledece stanje je crveno za vozila
	 	      // a zeleno za pesake
    if((Stanje == PESACI_CRVENO_AUTA_ZELENO) || 
        (Stanje == PESACI_CRVENO_AUTA_ZUTO
        && Sledece_stanje == PESACI_CRVENO_AUTA_ZELENO)){
            Zahtev_za_promenu = 1; // Zahtev za promenu menja vrednost
          }
  }

  void Podesavanje_vremena(){
      if (Brojac < 600){ // Proverava se vrednost brojaca
          Brojac += 1; // Vrednost brojaca se povecava za jedan
        }

	// Ukoliko je ukljuceno crveno svetlo za pesake,
	// i ukoliko je istekao period trajanja zelenog svetla
	// i ukoliko postoji zahtev za promenu
      if(Stanje == PESACI_CRVENO_AUTA_ZELENO
         && Zahtev_za_promenu && Brojac > PERIOD_ZELENOG_SVETLA){
            Brojac = 0; // Vrednost brojaca je sada nula
            Zahtev_za_promenu = 0; // Zahtev za promenu dobija vrednost nula
            Stanje = PESACI_CRVENO_AUTA_ZUTO; // Pali se zuto za automobile
            Sledece_stanje = PESACI_ZELENO_AUTA_CRVENO; // Pali se zeleno za pesake i crveno za auta
          }

	
	// Ukoliko je ukljuceno zuto svetlo i ukoliko je period trajanja zutog svetla istekao	

       if (Stanje == PESACI_CRVENO_AUTA_ZUTO && Brojac > PERIOD_ZUTOG_SVETLA){
              Brojac = 0;
              Stanje = Sledece_stanje; // Stanje postaje sledece stanje stanje
            }



	// Ukoliko je ukljuceno zeleno za pesake i ukoliko je prosao period
	// trajanja zelenog svetla

       if(Stanje == PESACI_ZELENO_AUTA_CRVENO && Brojac > PERIOD_ZELENOG_SVETLA){
          Brojac = 0; // Dobija vrednost nula
          Stanje = PESACI_CRVENO_AUTA_ZUTO; // Pali se crveno za pesake, zuto za auta
          Sledece_stanje = PESACI_CRVENO_AUTA_ZELENO; // Sledece stanje je pesacima crveno
						       // a vozilima zeleno
        }   
    }

void setup(){
    pinMode(Pesaci_crveno, OUTPUT); // Led diode postavljene kao izlaz
    pinMode(Pesaci_zeleno, OUTPUT);
    pinMode(Auta_crveno, OUTPUT);
    pinMode(Auta_zuto, OUTPUT);
    pinMode(Auta_zeleno, OUTPUT);

    pinMode(a, OUTPUT); // Sedmosegmentni displej postavljen kao izlaz
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);
    pinMode(e, OUTPUT);
    pinMode(f, OUTPUT);
    pinMode(g, OUTPUT);

    pinMode(Poziv_pesaka, INPUT_PULLUP); // Pin tastera je konfigurisan kao ulazni pin
    attachInterrupt(digitalPinToInterrupt(Poziv_pesaka), Pusti_pesake, FALLING);
		// Ova komanda povezuje funkciju Pusti_pesake sa prekidom koji generise silazna 
		// ivica na pinu 2 za koji je vezan taster
    Timer1.initialize(100000); // Inicijalizacija timer1 i podesavanje perioda trajanja prekida na 1 sec
    Timer1.attachInterrupt(Podesavanje_vremena);
	// Funkciju Podesavanje_vremena postavlja za funkciju prekida nakon isteka prekida
  }

  void prikazBroja(int broj){ 
      if (broj !=1 && broj != 4)
      digitalWrite(a, HIGH); //segment a se ne pali u slucajevima sa brojem 1 i 4

      if (broj !=5 && broj != 6) // segment b se ne pali u slucajevima sa brojem 5 i 6
      digitalWrite(b, HIGH);

      if (broj != 2) // segment c se ne pali u slucaju sa brojem 2
      digitalWrite(c, HIGH);

      if (broj !=1 && broj != 4 && broj !=7)
      digitalWrite(d, HIGH); // segment b se ne pali u slucajevima sa brojem 1,4 i 7

      if (broj == 2 || broj == 6 || broj == 8 || broj == 0)
      digitalWrite(e, HIGH); //segment e se pali u slucajevima sa brojem 0,2,6,8

      if (broj !=1 && broj != 2 && broj != 3 && broj != 7)
      digitalWrite(f, HIGH);  //segment f se ne pali u slucajevima sa brojem 1,2,3 i 7

      if (broj !=0 && broj != 1 && broj !=7) // segment g se ne pali u slucajevima sa brojem 1 i 7
      digitalWrite(g, HIGH);
    }

    void ugasi(){ // funkcija koja gasi sve segmente da sedmosegmentnom displeju
        digitalWrite(a, LOW);
        digitalWrite(b, LOW);
        digitalWrite(c, LOW);
        digitalWrite(d, LOW);
        digitalWrite(e, LOW);
        digitalWrite(f, LOW);
        digitalWrite(g, LOW);
      }

      void tajmer(){ // funkcija brojaca sedmosegmentnog displeja
		     // krece od broja 6 do 0 nakon cega se gasi uz pomoc funkcije
                    // za gasenje
          for (int i = 6; i>=0; i--){
              prikazBroja(i);
              delay(1000);
              ugasi();
            }
        }

void loop() {
  switch(Stanje){ // Na osnovu stanja odredjene led diode se pale ili gase
      case PESACI_CRVENO_AUTA_ZELENO:
        digitalWrite(Pesaci_crveno, HIGH);
        digitalWrite(Pesaci_zeleno, LOW);
        digitalWrite(Auta_crveno, LOW);
        digitalWrite(Auta_zuto, LOW);
        digitalWrite(Auta_zeleno, HIGH);
        break;

        case PESACI_CRVENO_AUTA_ZUTO:
        digitalWrite(Pesaci_crveno, HIGH);
        digitalWrite(Pesaci_zeleno, LOW);
        digitalWrite(Auta_crveno, LOW);
        digitalWrite(Auta_zuto, HIGH);
        digitalWrite(Auta_zeleno, LOW);
        break;

        case PESACI_ZELENO_AUTA_CRVENO:
        digitalWrite(Pesaci_crveno, LOW);
        digitalWrite(Pesaci_zeleno, HIGH);
        digitalWrite(Auta_crveno, HIGH);
        digitalWrite(Auta_zuto, LOW);
        tajmer(); // pozivanje funkcije za paljenje segmosegmentnog displeja
        digitalWrite(Auta_zeleno, LOW);
        break;
    }
}