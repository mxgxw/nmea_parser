/*
    Finite state machine NMEA Parser
    Copyright (C) 2015  Mario Gomez <mario.gomez-at-teubi.co>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
    
int s = 0;
int p1 = 0;
int p2 = 0;
char latitude[11] = "0000.0000N";
char longitude[12] = "00000.0000W";

void setup() {
  Serial.begin(115200);
  Serial1.begin(4800); // Cambiar la salida correspondiente
}

// Finite-State Machine NMEA parser
boolean nmea_parser(char c) {
  boolean parsed = false;
  switch(c) {
    case '$':
      if(s==0) {
        s++;
        p1=p2=0;
      }
      break;
    case 'g':
    case 'G':
      if(s==1||s==3||s==4)
        s++;
      else
        s=0;
      break;
    case 'p':
    case 'P':
      if(s==2)
        s++;
      else
        s=0;
      break;
    case 'a':
    case 'A':
      if(s==5)
        s++;
      else
        s=0;
      break;
    case ',':
      if(s==6||s==8||s==10||s==12||s==14)
        s++;
      else
        s==0;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if(s==9||s==10) {
        if(p1>9) {
          p1=0;
        }
        latitude[p1++] = c;
      } else if(s==13||s==14) {
        if(p2>10) {
          p2=0;
        }
        longitude[p2++] = c;
      }
      break;
    case '.':
      if(s==7||s==9||s==13) {
        if(s==9) {
          latitude[4] = '.';
          p1++;
        }
        if(s==13) {
          longitude[5] = '.';
          p2++;
        }
        s++;
      } else
        s=0;
      break;
    case 's':
    case 'S':
    case 'n':
    case 'N':
      if(s==11) {
        latitude[9] = c;
        s++;
      } else
        s = 0;
      break;
    case 'e':
    case 'E':
    case 'w':
    case 'W':
      if(s==15) {
        longitude[10] = c;
        parsed = true;
      }
      s = 0;
      break;
    default:
      s = 0;
      break;
  }
  return parsed;
}

char c;

long wait = 0;

void loop() {
  if(Serial1.available()) {
    c = Serial1.read();
    if(nmea_parser(c)) {
      Serial.println("\r\nNMEA data decoded:");
      Serial.print("Latitude: ");
      Serial.println(latitude);
      Serial.print("Longitude: ");
      Serial.println(longitude);
    }
  }
}
