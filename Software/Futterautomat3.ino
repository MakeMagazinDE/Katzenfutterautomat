#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const String Bezeichnung = "Katzenfutter";
const int Pin = 5;
const unsigned long Einschaltzeit = 15000; //in ms: legt Futtermenge fest
const char* ssid = "hbvdr"; //Hier die SSID Ihres WLAN eintragen
const char* password = "babybaer"; //Hier das dazugehörende Passwort

ESP8266WebServer server(80); 

void setup() {
  Serial.begin(115200);
  delay(10); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
  }
  Serial.print(ssid);
  Serial.println(" Verbindung hergestellt");   
  server.begin(); 
  Serial.print("Adresse : http://");   // Ausgabe der IP Adresse 
  Serial.print(WiFi.localIP());
  Serial.println("/");
  server.on("/", callHome);
  server.on("/set", callSet);
  pinMode(Pin,OUTPUT);
}

void callHome(){
  String content = "";
  content += "<!DOCTYPE HTML>";
  content += "<html>"; 
  content += "<head>"; 
  content += "<style>"; 
  content += ".wrapper {max-width:90%;margin:0 auto;text-align:center;}"; 
  content += ".btn {margin:15px;}";   
  content += "</style>"; 
  content += "</head>"; 
  content += "<body>"; 
  content += "<div class='wrapper'>"; 
  content += "<h2>Katzenfutter-Automat V1.0</h2>"; 
  content += "<br/><br/>"; 
  content += "<center>"; 
  content += "<input type='button' class= 'btn' value='"+Bezeichnung+"' onClick='sendRequest("+0+")'/>"; 
  String ipAddress = WiFi.localIP().toString();
  content += "</center>"; 
  content += "</div>"; 
  content += "<script>"; 
  content += "const url='http://"+ipAddress+"/set?';"; 
  content += "function sendRequest(button){"; 
  content += "  const Http = new XMLHttpRequest();";
  content += "  reqUrl=url+'btn='+button;";
  content += "  console.log(reqUrl);";
  content += "  Http.open('GET', reqUrl);";
  content += "  Http.send();";
  content += "}"; 
  content += "</script>"; 
  content += "</body>"; 
  content += "</html>"; 
  sendResult(content);
}

void callSet(){
  int buttonIndex = -1;
  String parameterName = server.argName(0);
  String parameterValue = server.arg(0);
  if(parameterName == "btn"){
     buttonIndex = parameterValue.toInt();
  }
  if(buttonIndex > -1){
    digitalWrite(Pin,HIGH);
    delay(Einschaltzeit);
    digitalWrite(Pin,LOW);
    sendResult("{\"msg\": \"Das Relais mit der Bezeichnung "+Bezeichnung+" ist aktiv!\"}");
  }  
}

void sendResult(String content){
  server.send(200, "text/html", content);  
}
 
void loop() {
  server.handleClient();
 }
