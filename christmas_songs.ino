// WiFi and web server libraries
#include <WiFi.h>
#include <WebServer.h>

#define USE_AP true                 
const char* AP_SSID   = "Christmas Ligths";
const char* AP_PASS   = "12345678"; 
const char* STA_SSID  = "YOUR_WIFI";
const char* STA_PASS  = "YOUR_PASSWORD";

#define BUZZER 18

#define LED_COUNT 6
const int LED_PINS[LED_COUNT] = {13, 14, 25, 26, 27, 32};

#define REST 0
#define A3 220
#define B3 247
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define FS4 370
#define G4 392
#define A4 440
#define B4 494
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784

// Stores the note and how long it will play
struct Note {
  int frequency;
  int beats;   
};

Note jingle[] = {
  {E4,2},{E4,2},{E4,4},
  {E4,2},{E4,2},{E4,4},
  {E4,2},{G4,2},{C4,3},{D4,1},
  {E4,8},
  {F4,2},{F4,2},{F4,3},{F4,1},
  {F4,2},{E4,2},{E4,2},{E4,1},{E4,1},
  {E4,2},{D4,2},{D4,2},{E4,2},
  {D4,4},{G4,4},
  {E4,2},{E4,2},{E4,4},
  {E4,2},{E4,2},{E4,4},
  {E4,2},{G4,2},{C4,3},{D4,1},
  {E4,8},
  {F4,2},{F4,2},{F4,3},{F4,1},
  {F4,2},{E4,2},{E4,2},{E4,1},{E4,1},
  {G4,2},{G4,2},{F4,2},{D4,2},
  {C4,8}
};

Note merry[] = {
  {D4,2},
  {G4,2},{G4,1},{A4,1},{G4,1},{FS4,1},
  {E4,2},{E4,2},{E4,2},
  {A4,2},{A4,1},{B4,1},{A4,1},{G4,1},
  {FS4,2},{D4,2},{D4,2},
  {B4,2},{B4,1},{C5,1},{B4,1},{A4,1},
  {G4,2},{E4,2},{D4,1},{D4,1},
  {E4,2},{A4,2},{FS4,2},
  {G4,6}
};

Note silent[] = {
  {G4,3},{A4,1},{G4,2},{E4,6},
  {G4,3},{A4,1},{G4,2},{E4,6},
  {D5,4},{D5,2},{B4,6},
  {C5,4},{C5,2},{G4,6},
  {A4,4},{A4,2},{C5,3},{B4,1},{A4,2},
  {G4,3},{A4,1},{G4,2},{E4,6},
  {A4,4},{A4,2},{C5,3},{B4,1},{A4,2},
  {G4,3},{A4,1},{G4,2},{E4,6},
  {D5,4},{D5,2},{F5,3},{D5,1},{B4,2},
  {C5,6},{E5,6},
  {C5,3},{G4,1},{E4,2},
  {G4,3},{F4,1},{D4,2},
  {C4,6}
};

Note joy[] = {
  {C5,3},{B4,2},{A4,1},{G4,4},
  {F4,3},{E4,1},{D4,2},{C4,4},
  {G4,2},{A4,4},{A4,2},{B4,4},{B4,2},
  {C5,6},{REST,2},
  {C5,2},{C5,2},{B4,2},{A4,2},
  {G4,3},{G4,1},{F4,2},{E4,2},
  {C5,2},{C5,2},{B4,2},{A4,2},
  {G4,3},{G4,1},{F4,2},{E4,2},
  {E4,2},{E4,2},{E4,2},{E4,2},
  {E4,1},{F4,1},{G4,4},
  {F4,1},{E4,1},{D4,2},{D4,2},{D4,2},
  {D4,1},{E4,1},{F4,4},
  {E4,1},{D4,1},{C4,2},
  {C5,3},{A4,1},{G4,3},{F4,1},{E4,2},{F4,2},{E4,2},
  {D4,2},{C4,8}
};

Note deck[] = {
  {G4,3},{F4,1},{E4,2},{D4,2},{C4,2},{D4,2},{E4,2},{C4,2},
  {D4,3},{E4,1},{F4,2},{D4,2},{E4,3},{D4,1},{C4,4},
  {G4,3},{F4,1},{E4,2},{D4,2},{C4,2},{D4,2},{E4,2},{C4,2},
  {D4,3},{E4,1},{F4,2},{D4,2},{E4,3},{D4,1},{C4,4},
  {D4,3},{E4,1},{F4,2},{D4,2},{E4,3},{F4,1},{G4,4},
  {E4,3},{F4,1},{G4,4},{E4,3},{F4,1},{G4,4},
  {G4,3},{A4,1},{G4,2},{F4,2},{E4,2},{D4,2},{C4,4},
  {D4,3},{E4,1},{F4,2},{D4,2},{E4,3},{D4,1},{C4,4}
};

// Song information
struct Song {
  const char* name;
  const Note* notes;
  size_t length;
  int beatTime;   
};

#define COUNT(a) (sizeof(a) / sizeof(a[0]))

Song songs[] = {
  {"Jingle Bells",                  jingle,  COUNT(jingle),  140},
  {"We Wish You a Merry Christmas", merry,   COUNT(merry),   160},
  {"Silent Night",                  silent,  COUNT(silent),  330},
  {"Joy to the World",              joy,     COUNT(joy),     150},
  {"Deck the Halls",                deck,    COUNT(deck),    150}
};
const int SONG_COUNT = sizeof(songs) / sizeof(songs[0]);

int ledMode = 1;
int chasePos = 0;

void ledsOff() {
  for (int i = 0; i < LED_COUNT; i++) digitalWrite(LED_PINS[i], LOW);
}

int pitchToLed(int freq) {
  if (freq < 312) return 0;   
  if (freq < 360) return 1;   
  if (freq < 416) return 2;   
  if (freq < 467) return 3;   
  if (freq < 555) return 4;   
  return 5;                   
}

void ledsForNote(int freq) {
  ledsOff();
  if (ledMode == 0 || freq == REST) return;

  if (ledMode == 1) {                 
    digitalWrite(LED_PINS[pitchToLed(freq)], HIGH);
  } else if (ledMode == 2) {          
    digitalWrite(LED_PINS[chasePos], HIGH);
    chasePos = (chasePos + 1) % LED_COUNT;
  } else if (ledMode == 3) {          
    int top = pitchToLed(freq);
    for (int i = 0; i <= top; i++) digitalWrite(LED_PINS[i], HIGH);
  }
}

int    currentSong = 0;        
bool   playing = false;
bool   paused  = false;
size_t noteIndex = 0;
bool   inGap = false;
bool   playAll = false;        
int    speedPct = 100;         
unsigned long soundEnd = 0, slotEnd = 0;
unsigned long beatsDone = 0;
unsigned long songTotal[SONG_COUNT];

void startNote() {
  const Song& s = songs[currentSong];
  const Note& n = s.notes[noteIndex];
  unsigned long dur = (unsigned long)n.beats * s.beatTime * 100UL / speedPct;
  unsigned long now = millis();

  if (n.frequency != REST) {
    tone(BUZZER, n.frequency);
    soundEnd = now + dur * 85 / 100;
  } else {
    noTone(BUZZER);
    soundEnd = now + dur;
  }
  ledsForNote(n.frequency);
  slotEnd = now + dur;
  inGap = false;
}

void startSong(int id) {
  if (id < 0 || id >= SONG_COUNT) return;
  noTone(BUZZER);
  ledsOff();
  currentSong = id;
  noteIndex = 0;
  beatsDone = 0;
  chasePos = 0;
  playing = true;
  paused = false;
  startNote();
}

void stopSong() {
  noTone(BUZZER);
  ledsOff();
  playing = false;
  paused = false;
  noteIndex = 0;
  beatsDone = 0;
}

void togglePause() {
  if (!playing) {
    startSong(currentSong);
  } else if (paused) {
    paused = false;
    startNote();               
  } else {
    paused = true;
    noTone(BUZZER);
    ledsOff();
  }
}

void updatePlayer() {
  if (!playing || paused) return;
  unsigned long now = millis();

  if (!inGap && now >= soundEnd) {
    noTone(BUZZER);
    ledsOff();                 
    inGap = true;
  }

  if (now >= slotEnd) {
    beatsDone += songs[currentSong].notes[noteIndex].beats;
    noteIndex++;
    if (noteIndex >= songs[currentSong].length) {
      noTone(BUZZER);
      ledsOff();
      if (playAll) {
        startSong((currentSong + 1) % SONG_COUNT);
      } else {
        playing = false;
        noteIndex = 0;
        beatsDone = 0;
      }
    } else {
      startNote();
    }
  }
}

WebServer server(80);

const char PAGE_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Christmas Songs/title>
<style>
  :root{--bg:#121212;--card:#1c1c1e;--fg:#f5f5f5;--mute:#8e8e93;--line:#2c2c2e;--acc:#e5484d}
  *{box-sizing:border-box;-webkit-tap-highlight-color:transparent}
  body{margin:0;background:var(--bg);color:var(--fg);
       font-family:-apple-system,system-ui,"Helvetica Neue",sans-serif;
       max-width:420px;margin:0 auto;padding:28px 22px 40px}
  .top{font-size:11px;letter-spacing:.14em;text-transform:uppercase;color:var(--mute);text-align:center}

  .art{width:230px;height:230px;margin:26px auto 24px;border-radius:24px;
       background:linear-gradient(145deg,#2a2a2e,#17171a);
       display:flex;align-items:center;justify-content:center;
       box-shadow:0 20px 40px rgba(0,0,0,.45)}
  .disc{width:170px;height:170px;border-radius:50%;
        background:repeating-radial-gradient(circle,#0c0c0c 0 3px,#1a1a1a 3px 6px);
        position:relative;animation:spin 5s linear infinite;animation-play-state:paused}
  .disc::after{content:"";position:absolute;inset:60px;border-radius:50%;background:var(--acc);
               box-shadow:0 0 0 6px #0c0c0c}
  .playing .disc{animation-play-state:running}
  @keyframes spin{to{transform:rotate(360deg)}}

  #title{font-size:22px;font-weight:600;text-align:center;margin:0}
  #sub{font-size:14px;color:var(--mute);text-align:center;margin:6px 0 22px}

  .bar{height:4px;background:var(--line);border-radius:2px;overflow:hidden}
  #fill{height:100%;width:0;background:var(--fg);border-radius:2px;transition:width .5s linear}

  .ctrl{display:flex;align-items:center;justify-content:center;gap:34px;margin:26px 0 22px}
  .ctrl button{background:none;border:0;color:var(--fg);padding:0;cursor:pointer;display:flex}
  .ctrl button:active{opacity:.5}
  .ctrl svg{width:30px;height:30px;fill:currentColor}
  .ctrl .main{width:68px;height:68px;border-radius:50%;background:var(--fg);color:var(--bg);
              align-items:center;justify-content:center}
  .ctrl .main svg{width:30px;height:30px}

  .opts{display:flex;justify-content:space-between;align-items:center;
        font-size:13px;color:var(--mute);margin-bottom:6px}
  .chips{display:flex;gap:6px}
  .chip{background:none;border:1px solid var(--line);color:var(--mute);border-radius:14px;
        padding:6px 12px;font:inherit;font-size:12px;cursor:pointer}
  .chip.on{color:var(--bg);background:var(--fg);border-color:var(--fg)}
  input[type=range]{width:100%;accent-color:var(--fg);margin:6px 0 26px}

  .list{background:var(--card);border-radius:18px;overflow:hidden}
  .song{display:flex;align-items:center;width:100%;padding:15px 18px;background:none;border:0;
        border-bottom:1px solid var(--line);color:var(--fg);font:inherit;font-size:16px;
        text-align:left;cursor:pointer}
  .song:last-child{border-bottom:0}
  .song:active{background:#262629}
  .song .n{width:28px;color:var(--mute);font-size:13px}
  .song .t{flex:1}
  .song.on .t,.song.on .n{color:var(--acc)}
  .eq{display:none;gap:2px;align-items:flex-end;height:14px}
  .eq i{width:3px;background:var(--acc);height:4px;animation:eq .8s ease-in-out infinite;animation-play-state:paused}
  .eq i:nth-child(2){animation-delay:.2s}.eq i:nth-child(3){animation-delay:.4s}
  .song.on .eq{display:flex}
  .playing .song.on .eq i{animation-play-state:running}
  @keyframes eq{0%,100%{height:3px}50%{height:14px}}
</style></head><body>
<div class="top">Now Playing</div>
<div class="art"><div class="disc"></div></div>
<h1 id="title">Christmas Songs</h1>
<div id="sub">&nbsp;</div>
<div class="bar"><div id="fill"></div></div>

<div class="ctrl">
  <button onclick="send('/prev')" aria-label="Previous">
    <svg viewBox="0 0 24 24"><path d="M6 6h2v12H6zM9.5 12L18 6v12z"/></svg></button>
  <button class="main" onclick="send('/toggle')" aria-label="Play/Pause">
    <svg viewBox="0 0 24 24"><path id="ico" d="M8 5v14l11-7z"/></svg></button>
  <button onclick="send('/next')" aria-label="Next">
    <svg viewBox="0 0 24 24"><path d="M16 6h2v12h-2zM6 18V6l8.5 6z"/></svg></button>
</div>

<div class="opts">
  <span>Speed <span id="spd">100</span>%</span>
  <span class="chips">
    <button class="chip" id="ledBtn" onclick="send('/led')">Lights: Pitch</button>
    <button class="chip" id="allBtn" onclick="send('/all')">Auto-next</button>
  </span>
</div>
<input type="range" min="50" max="200" step="5" value="100" id="sl"
       oninput="document.getElementById('spd').textContent=this.value"
       onchange="send('/speed?pct='+this.value)">

<div class="list">
)rawliteral";

const char PAGE_TAIL[] PROGMEM = R"rawliteral(
</div>
<script>
var PLAY="M8 5v14l11-7z", PAUSE="M6 5h4v14H6zM14 5h4v14h-4z", dragging=false;
var LEDN=['Off','Pitch','Chase','Bar'];
var sl=document.getElementById('sl');
sl.addEventListener('input',function(){dragging=true});
sl.addEventListener('change',function(){dragging=false});

function send(u){ fetch(u).then(refresh); }
function refresh(){
  fetch('/status').then(r=>r.json()).then(s=>{
    var run = s.playing && !s.paused;
    document.body.classList.toggle('playing', run);
    document.getElementById('title').textContent = s.name;
    document.getElementById('sub').textContent =
      'Track '+(s.id+1)+' of '+s.count+(s.paused?' \u00b7 Paused':(s.playing?'':' \u00b7 Stopped'));
    document.getElementById('fill').style.width = (s.progress/10)+'%';
    document.getElementById('ico').setAttribute('d', run ? PAUSE : PLAY);
    document.querySelectorAll('.song').forEach((b,i)=>b.classList.toggle('on', i===s.id));
    document.getElementById('allBtn').classList.toggle('on', s.all);
    var lb=document.getElementById('ledBtn');
    lb.textContent='Lights: '+LEDN[s.led];
    lb.classList.toggle('on', s.led>0);
    if(!dragging){ sl.value=s.speed; document.getElementById('spd').textContent=s.speed; }
  });
}
refresh(); setInterval(refresh, 700);
</script></body></html>
)rawliteral";

void handleRoot() {
  String html = FPSTR(PAGE_HEAD);
  for (int i = 0; i < SONG_COUNT; i++) {
    html += "<button class='song' onclick=\"send('/play?id=" + String(i) + "')\">";
    html += "<span class='n'>" + String(i + 1) + "</span>";
    html += "<span class='t'>" + String(songs[i].name) + "</span>";
    html += "<span class='eq'><i></i><i></i><i></i></span></button>";
  }
  html += FPSTR(PAGE_TAIL);
  server.send(200, "text/html", html);
}

void handlePlay() {
  if (server.hasArg("id")) startSong(server.arg("id").toInt());
  server.send(200, "text/plain", "ok");
}

void handleToggle() {
  togglePause();
  server.send(200, "text/plain", "ok");
}

void handleNext() {
  startSong((currentSong + 1) % SONG_COUNT);
  server.send(200, "text/plain", "ok");
}

void handlePrev() {
  startSong((currentSong + SONG_COUNT - 1) % SONG_COUNT);
  server.send(200, "text/plain", "ok");
}

void handleAll() {
  playAll = !playAll;
  server.send(200, "text/plain", "ok");
}

void handleLed() {
  ledMode = (ledMode + 1) % 4;
  if (ledMode == 0) ledsOff();
  server.send(200, "text/plain", "ok");
}

void handleSpeed() {
  if (server.hasArg("pct")) {
    speedPct = constrain(server.arg("pct").toInt(), 50, 200);
  }
  server.send(200, "text/plain", "ok");
}

void handleStatus() {
  unsigned long prog = 0;
  if (playing && songTotal[currentSong] > 0) {
    prog = beatsDone * 1000UL / songTotal[currentSong];
  }
  String json = "{\"id\":" + String(currentSong) +
                ",\"name\":\"" + songs[currentSong].name + "\"" +
                ",\"count\":" + String(SONG_COUNT) +
                ",\"playing\":" + (playing ? "true" : "false") +
                ",\"paused\":" + (paused ? "true" : "false") +
                ",\"all\":" + (playAll ? "true" : "false") +
                ",\"led\":" + String(ledMode) +
                ",\"speed\":" + String(speedPct) +
                ",\"progress\":" + String(prog) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);

  
  for (int i = 0; i < LED_COUNT; i++) pinMode(LED_PINS[i], OUTPUT);
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(120);
    digitalWrite(LED_PINS[i], LOW);
  }

  if (USE_AP) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("AP started. Open http:
    Serial.println(WiFi.softAPIP());
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.begin(STA_SSID, STA_PASS);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.print("\nConnected. Open http:
    Serial.println(WiFi.localIP());
  }

  
  for (int i = 0; i < SONG_COUNT; i++) {
    songTotal[i] = 0;
    for (size_t j = 0; j < songs[i].length; j++) songTotal[i] += songs[i].notes[j].beats;
  }

  server.on("/", handleRoot);
  server.on("/play", handlePlay);
  server.on("/toggle", handleToggle);
  server.on("/next", handleNext);
  server.on("/prev", handlePrev);
  server.on("/all", handleAll);
  server.on("/led", handleLed);
  server.on("/speed", handleSpeed);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();
  updatePlayer();

  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    lastPrint = millis();
    Serial.print("Open http:
    Serial.println(USE_AP ? WiFi.softAPIP() : WiFi.localIP());
  }
}
