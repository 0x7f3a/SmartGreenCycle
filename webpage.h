// ==========================================================
// WebApp សម្រាប់ Smart GreenCycle (ផ្ទុកក្នុង ESP32)
// Theme យកតាម smartfarm-app (3).html
// ចំណាំ: មិនប្រើ Google Fonts ព្រោះទូរស័ព្ទភ្ជាប់ WiFi ESP32 គ្មាន Internet
// ចង់មើល UI លើកុំព្យូទ័រ៖ Copy ចំណែក HTML ខាងក្រោមទៅជា .html ហើយបើកក្នុង Browser -> DEMO mode
// ==========================================================
#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="km">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<title>Smart GreenCycle</title>
<style>
/* Layout: app column on phones, top-nav dashboard on desktop (smartfarm theme) */
:root{
  --bg:#dfeaf1; --screen:#f7f9fb; --card:#ffffff; --line:#e3e9ee;
  --ink:#14202b; --muted:#5b6b78; --seg:#e9eef2;
  --blue:#2d7ff9; --green:#2fb457; --greenbg:#e3f5ea; --amber:#f08a1c; --amberbg:#fdf0e1; --red:#d9433b; --redbg:#fbe7e5; --off:#c5cfd8;
  --goodtx:#2b7d46; --warntx:#b45a07; --badtx:#b8322b;
  --font:'Noto Sans Khmer','Khmer OS Battambang','Khmer OS','Leelawadee UI',system-ui,sans-serif;
  --num:'Segoe UI','Roboto',system-ui,sans-serif;
}
@media (prefers-color-scheme:dark){
  :root:not([data-theme="light"]){
    --bg:#0c151c; --screen:#101a22; --card:#16232d; --line:#25343f;
    --ink:#e8eff4; --muted:#93a5b3; --seg:#1d2c37; --greenbg:#15332295; --amberbg:#3a2a1590; --redbg:#3a1a1890; --off:#3a4a56;
    --goodtx:#6fd391; --warntx:#f5b066; --badtx:#f2867f; color-scheme:dark;
  }
}
:root[data-theme="dark"]{
  --bg:#0c151c; --screen:#101a22; --card:#16232d; --line:#25343f;
  --ink:#e8eff4; --muted:#93a5b3; --seg:#1d2c37; --greenbg:#15332295; --amberbg:#3a2a1590; --redbg:#3a1a1890; --off:#3a4a56;
  --goodtx:#6fd391; --warntx:#f5b066; --badtx:#f2867f; color-scheme:dark;
}
*,*::before,*::after{box-sizing:border-box}
html,body{margin:0}
body{background:var(--bg);color:var(--ink);font-family:var(--font);line-height:1.6;-webkit-font-smoothing:antialiased;min-height:100vh}
button,input{font-family:inherit}
:focus-visible{outline:3px solid var(--blue);outline-offset:2px;border-radius:10px}
[hidden]{display:none!important}

.shell{display:flex;flex-direction:column;min-height:100vh;background:var(--screen)}
.main{flex:1;min-width:0}
.app-inner{padding:calc(env(safe-area-inset-top,0px) + 18px) 16px 24px}

.nav{order:2;position:sticky;bottom:0;background:var(--card);border-top:1px solid var(--line);display:flex;justify-content:space-around;padding:8px 8px calc(env(safe-area-inset-bottom,0px) + 8px);z-index:20}
.nav a{display:flex;flex-direction:column;align-items:center;gap:1px;font-size:.72rem;color:var(--muted);text-decoration:none;min-width:64px;padding:4px 0;border-radius:12px}
.nav a.cur{color:var(--blue);font-weight:600}
@media(min-width:860px){
  .shell{background:var(--bg)}
  .main{order:2;background:var(--screen);min-height:100vh}
  .app-inner{max-width:1000px;margin:0 auto;padding:30px 40px 48px}
  .nav{order:1;position:sticky;top:0;justify-content:flex-start;gap:4px;border-top:0;border-bottom:1px solid var(--line);padding:14px max(24px,calc((100% - 1000px)/2 + 40px))}
  .nav::before{content:"GreenCycle";font:700 1.15rem var(--num);padding:8px 22px 8px 0;margin-right:8px;color:var(--ink);border-right:1px solid var(--line)}
  .nav a{flex-direction:row;gap:8px;padding:9px 16px;font-size:.92rem;min-width:0}
  .nav a.cur,.nav a:hover{background:var(--seg)}
}

.top{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-bottom:14px}
.top h1{font-size:1.3rem;margin:0;font-weight:700;line-height:1.3}
.top small{display:block;color:var(--muted);font-size:.8rem}
.live{display:inline-flex;align-items:center;gap:6px;font:600 .85rem var(--num);color:var(--goodtx);white-space:nowrap}
.live b{width:9px;height:9px;border-radius:50%;background:var(--green);animation:pulse 1.8s infinite}
.live.off{color:var(--muted)} .live.off b{background:var(--off);animation:none}
@keyframes pulse{50%{opacity:.25}}
@media(min-width:860px){.top h1{font-size:1.7rem}}

.tabs{display:flex;gap:4px;background:var(--seg);border-radius:999px;padding:4px;margin-bottom:14px}
.tabs button{flex:1;border:0;background:transparent;font:500 .9rem var(--font);padding:7px 0;border-radius:999px;color:var(--muted);cursor:pointer;min-height:40px}
.tabs button[aria-pressed="true"]{background:var(--blue);color:#fff}
@media(min-width:860px){.tabs{max-width:460px}}

.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.tile{background:var(--card);border:1px solid var(--line);border-radius:20px;padding:12px 14px;min-width:0}
.tile.hide{display:none}
.tile small{display:block;color:var(--muted);font-size:.85rem}
.v{font-family:var(--num);font-weight:600;font-size:1.9rem;line-height:1.35;font-variant-numeric:tabular-nums}
.v span{font-size:.85rem;color:var(--muted);font-weight:500;margin-left:3px}
.wide{grid-column:1/-1}
.chip{display:inline-block;font-size:.78rem;font-weight:600;padding:1px 10px;border-radius:99px;background:var(--seg);color:var(--muted)}
.chip.good{background:var(--greenbg);color:var(--goodtx)}
.chip.warn{background:var(--amberbg);color:var(--warntx)}
.chip.bad{background:var(--redbg);color:var(--badtx)}
.bar{height:10px;border-radius:99px;background:var(--seg);overflow:hidden;margin:6px 0 4px;position:relative}
.bar i{display:block;height:100%;width:0;border-radius:99px;background:var(--blue);transition:width .6s}
.bar em{position:absolute;top:0;bottom:0;width:2px;background:var(--ink);opacity:.35}
.status{font-size:.85rem;font-weight:500}
.chart svg{display:block;width:100%;height:90px;margin-top:4px}
.chart .cap{display:flex;justify-content:space-between;font-size:.8rem;color:var(--muted)}
@media(min-width:860px){
  .grid{grid-template-columns:repeat(3,1fr);gap:14px}
  .grid .tile{padding:18px 20px}
  .chart svg{height:130px}
}

/* water quality + flow diagram */
.wq{display:flex;gap:12px;align-items:center}
.wq .dot{width:44px;height:44px;border-radius:14px;display:grid;place-items:center;flex:none;background:var(--seg);color:var(--muted)}
.wq.good .dot{background:var(--green);color:#fff}
.wq.bad .dot{background:var(--amber);color:#fff}
.wq h3{margin:0;font-size:1rem;line-height:1.4}
.wq p{margin:0;font-size:.85rem;color:var(--muted)}
.flow{overflow-x:auto;margin-top:10px}
.flow svg{display:block;width:100%;height:auto}
.flow .bx{fill:var(--seg);stroke:var(--line)}
.flow .bx.on{fill:var(--greenbg);stroke:var(--green)}
.flow text{fill:var(--ink);font:500 12px var(--font)}
.flow text.s{fill:var(--muted);font-size:10.5px}
.flow .p{fill:none;stroke:var(--off);stroke-width:3;stroke-linecap:round}
.flow .p.on{stroke:var(--blue);stroke-dasharray:6 6;animation:flow .8s linear infinite}
.flow .rl{font:700 11px var(--num);fill:var(--muted)}
.flow .rl.on{fill:var(--blue)}
.flow .water{fill:var(--blue);opacity:.35;transition:all .6s}
@keyframes flow{to{stroke-dashoffset:-12}}

h2{font-size:1.05rem;margin:22px 2px 10px;display:flex;justify-content:space-between;align-items:baseline;gap:8px}
h2 span{font-size:.8rem;font-weight:500;color:var(--muted)}

.alert{border-radius:16px;padding:10px 14px;font-size:.88rem;margin-bottom:10px;display:flex;gap:10px;align-items:center;justify-content:space-between}
.alert.warn{background:var(--amberbg);color:var(--warntx)}
.alert.bad{background:var(--redbg);color:var(--badtx)}
.alert.info{background:var(--seg);color:var(--muted)}
.alert button{border:0;border-radius:99px;padding:6px 14px;background:var(--card);color:var(--ink);font-weight:600;cursor:pointer;flex:none}

.relays{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.relay{background:var(--card);border:1px solid var(--line);border-radius:20px;padding:12px 14px;display:flex;flex-direction:column;gap:10px;transition:background .2s,border-color .2s;min-width:0}
.relay.on{background:var(--greenbg);border-color:color-mix(in srgb,var(--green) 55%,var(--line))}
.rtop{display:flex;align-items:center;justify-content:space-between}
.ric{width:38px;height:38px;border-radius:12px;background:var(--seg);display:grid;place-items:center;color:var(--muted)}
.relay.on .ric{background:var(--green);color:#fff}
.relay h3{margin:0;font-size:.98rem;line-height:1.4}
.relay p{margin:0;font-size:.8rem;color:var(--muted)}
.relay .lock{color:var(--badtx);font-weight:600}
.sw{position:relative;width:52px;height:30px;border-radius:99px;background:var(--off);border:0;cursor:pointer;padding:0;transition:background .2s;flex:none}
.sw::after{content:"";position:absolute;top:3px;left:3px;width:24px;height:24px;border-radius:50%;background:#fff;box-shadow:0 1px 3px rgba(0,0,0,.3);transition:transform .2s}
.sw[aria-checked="true"]{background:var(--green)}
.sw[aria-checked="true"]::after{transform:translateX(22px)}
.sw[aria-disabled="true"]{opacity:.45;cursor:not-allowed}
@media(min-width:860px){.relays{grid-template-columns:repeat(4,1fr);gap:14px}}

.cfg{background:var(--card);border:1px solid var(--line);border-radius:20px;padding:14px}
.cfg fieldset{border:0;margin:0 0 12px;padding:0}
.cfg legend{font-weight:600;font-size:.92rem;margin-bottom:6px;padding:0}
.row{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.cfg label{display:flex;flex-direction:column;font-size:.8rem;color:var(--muted);gap:2px}
.cfg input{font:600 1rem var(--num);padding:8px 10px;border:1px solid var(--line);border-radius:12px;background:var(--screen);color:var(--ink);width:100%}
.btn{border:0;border-radius:99px;padding:10px 22px;background:var(--blue);color:#fff;font-weight:600;cursor:pointer;min-height:44px}
.cal{font:.8rem var(--num);color:var(--muted);margin-top:10px}
@media(min-width:860px){.cfg form{display:grid;grid-template-columns:repeat(2,1fr);gap:0 28px}.cfg .act{grid-column:1/-1}}

footer{text-align:center;font-size:.78rem;color:var(--muted);margin-top:22px}
.toast{position:fixed;left:50%;bottom:calc(env(safe-area-inset-bottom,0px) + 96px);transform:translateX(-50%) translateY(20px);background:#14202b;color:#fff;padding:9px 16px;border-radius:99px;font-size:.9rem;opacity:0;pointer-events:none;transition:.25s;z-index:30;max-width:90vw;text-align:center}
.toast.on{opacity:1;transform:translateX(-50%) translateY(0)}
@media(min-width:860px){.toast{bottom:24px}}
@media(prefers-reduced-motion:reduce){*{animation:none!important;transition:none!important}}
</style>
</head>
<body>
<div class="shell">
<nav class="nav" aria-label="មីនុយ">
  <a href="#home" class="cur"><svg width="22" height="22" viewBox="0 0 24 24" fill="currentColor"><path d="M12 3l9 8h-3v9h-5v-6h-2v6H6v-9H3z"/></svg>ទំព័រដើម</a>
  <a href="#water"><svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"><path d="M12 3s6 6.5 6 11a6 6 0 11-12 0c0-4.5 6-11 6-11z"/></svg>ទឹក</a>
  <a href="#ctrl"><svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><rect x="3" y="7" width="18" height="10" rx="5"/><circle cx="16" cy="12" r="2.5"/></svg>បញ្ជា</a>
  <a href="#cfg"><svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M4 6h10M18 6h2M4 12h4M12 12h8M4 18h12M20 18h0"/><circle cx="16" cy="6" r="2"/><circle cx="10" cy="12" r="2"/><circle cx="18" cy="18" r="2"/></svg>កំណត់</a>
</nav>
<div class="main">
<div class="app-inner">
  <div class="top" id="home">
    <div><h1>Smart GreenCycle</h1><small>ប្រព័ន្ធដាំបន្លែវៃឆ្លាតប្រកបដោយចីរភាព · ត្រកួន</small></div>
    <span class="live" id="live"><b></b><span id="liveTx">Live</span></span>
  </div>

  <div id="alerts"></div>

  <div class="tabs" role="group" aria-label="ត្រងទិន្នន័យ">
    <button aria-pressed="true" data-f="all">ទាំងអស់</button>
    <button aria-pressed="false" data-f="air">អាកាស</button>
    <button aria-pressed="false" data-f="soil">ដី</button>
    <button aria-pressed="false" data-f="water">ទឹក</button>
  </div>

  <div class="grid">
    <div class="tile" data-g="air"><small>សីតុណ្ហភាព · DHT11</small><div class="v"><b id="vt">--</b><span>°C</span></div><span class="chip" id="ct">--</span></div>
    <div class="tile" data-g="air"><small>សំណើមខ្យល់ · DHT11</small><div class="v"><b id="vh">--</b><span>%</span></div><span class="chip" id="ch">--</span></div>

    <div class="tile wide" data-g="soil">
      <small>សំណើមដី · ផើង 60×40 cm</small>
      <div class="v"><b id="vs">--</b><span>%</span></div>
      <div class="bar"><i id="sfill"></i><em id="smark"></em></div>
      <div class="status" id="sstat">--</div>
    </div>

    <div class="tile" data-g="water"><small>pH · ធុងទឹកច្រោះ</small><div class="v"><b id="vph">--</b></div><span class="chip" id="cph">--</span></div>
    <div class="tile" data-g="water"><small>EC · ធុងទឹកច្រោះ</small><div class="v"><b id="vec">--</b><span>mS/cm</span></div><span class="chip" id="cec">--</span></div>

    <div class="tile wide chart" data-g="water">
      <small>pH ទឹកច្រោះ (ក្រោយបើកទំព័រ) · ផ្ទៃពណ៌បៃតង = អាចស្រោចបាន</small>
      <svg viewBox="0 0 300 90" preserveAspectRatio="none" aria-hidden="true">
        <rect id="band" x="0" width="300" fill="var(--green)" fill-opacity=".12"/>
        <path id="line" fill="none" stroke="var(--blue)" stroke-width="2.2" stroke-linejoin="round" stroke-linecap="round" vector-effect="non-scaling-stroke"/>
      </svg>
      <div class="cap"><span id="capL">pH 4</span><span>ឥឡូវ</span></div>
    </div>

    <div class="tile wide" data-g="water" id="water">
      <div class="wq" id="wqBox">
        <div class="dot"><svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 3s6 6.5 6 11a6 6 0 11-12 0c0-4.5 6-11 6-11z"/></svg></div>
        <div><h3 id="wqT">--</h3><p id="wqS">--</p></div>
      </div>
      <div class="flow">
        <svg viewBox="0 0 400 168" role="img" aria-label="ដ្យាក្រាមលំហូរទឹក">
          <!-- pipes -->
          <path class="p on" d="M46 38 V58"/>
          <path class="p on" d="M82 73 H110"/>
          <path class="p" id="p4" d="M194 73 H214 V88 H232"/>
          <path class="p" id="p3" d="M194 135 H232"/>
          <path class="p" id="p1" d="M292 95 H320"/>
          <path class="p" id="p2" d="M292 135 H320"/>
          <text class="rl" id="l4" x="206" y="66">R4</text>
          <text class="rl" id="l3" x="202" y="128">R3</text>
          <text class="rl" id="l1" x="296" y="88">R1</text>
          <text class="rl" id="l2" x="296" y="128">R2</text>
          <!-- boxes -->
          <rect class="bx" x="10" y="8" width="72" height="30" rx="8"/><text x="46" y="28" text-anchor="middle">ផ្ទះបាយ</text>
          <rect class="bx" x="10" y="58" width="72" height="30" rx="8"/><text x="46" y="78" text-anchor="middle">ធុងច្រោះ</text>
          <rect class="bx" id="bS" x="110" y="52" width="84" height="42" rx="8"/><text x="152" y="70" text-anchor="middle">ធុងស្តុក</text><text class="s" x="152" y="86" text-anchor="middle">pH · EC</text>
          <rect class="bx" x="110" y="120" width="84" height="30" rx="8"/><text x="152" y="140" text-anchor="middle">ទឹកស្អាត</text>
          <clipPath id="tc"><rect x="232" y="60" width="60" height="100" rx="8"/></clipPath>
          <rect class="bx" x="232" y="60" width="60" height="100" rx="8"/>
          <rect class="water" id="tw" x="232" y="160" width="60" height="0" clip-path="url(#tc)"/>
          <text x="262" y="78" text-anchor="middle">ធុងស្រោច</text><text class="s" id="tl" x="262" y="150" text-anchor="middle">--%</text>
          <rect class="bx" id="b1" x="320" y="78" width="74" height="34" rx="8"/><text x="357" y="99" text-anchor="middle">គល់ដំណាំ</text>
          <rect class="bx" id="b2" x="320" y="118" width="74" height="34" rx="8"/><text x="357" y="139" text-anchor="middle">ចំហាយ</text>
        </svg>
      </div>
    </div>

    <div class="tile wide" data-g="water">
      <small>កម្រិតទឹក · ធុងស្រោច</small>
      <div class="v"><b id="vl">--</b><span>%</span></div>
      <div class="bar"><i id="lfill"></i></div>
      <div class="status" id="lstat">--</div>
    </div>
  </div>

  <h2 id="ctrl">របៀបបញ្ជា <span id="up">ដំណើរការ --:--:--</span></h2>
  <div class="tabs" role="group" aria-label="Mode">
    <button aria-pressed="true" id="mAuto">Auto · ស្វ័យប្រវត្តិ</button>
    <button aria-pressed="false" id="mMan">Manual · ដោយដៃ</button>
  </div>

  <h2>Relay ទាំង 4 <span id="rcount">បើក 0 / 4</span></h2>
  <div class="relays" id="relays"></div>

  <h2 id="cfg">ការកំណត់ Auto <span>រក្សាទុកក្នុង ESP32</span></h2>
  <div class="cfg">
    <form id="cfgForm">
      <fieldset><legend>គុណភាពទឹកច្រោះ (ប្រើ R4)</legend>
        <div class="row">
          <label for="phMin">pH អប្បបរមា<input id="phMin" name="phMin" type="number" step="0.1" min="0" max="14"></label>
          <label for="phMax">pH អតិបរមា<input id="phMax" name="phMax" type="number" step="0.1" min="0" max="14"></label>
          <label for="ecMax">EC អតិបរមា (mS/cm)<input id="ecMax" name="ecMax" type="number" step="0.1" min="0.1" max="5"></label>
        </div>
      </fieldset>
      <fieldset><legend>ស្រោចតាមគល់ (R1)</legend>
        <div class="row">
          <label for="soilLow">ចាប់ផ្តើមពេលដី &lt; (%)<input id="soilLow" name="soilLow" type="number" min="0" max="100"></label>
          <label for="soilHigh">ឈប់ពេលដី ≥ (%)<input id="soilHigh" name="soilHigh" type="number" min="0" max="100"></label>
        </div>
      </fieldset>
      <fieldset><legend>បាញ់ចំហាយ (R2)</legend>
        <div class="row">
          <label for="tempHigh">ពេលក្តៅជាង (°C)<input id="tempHigh" name="tempHigh" type="number" step="0.5" min="20" max="50"></label>
          <label for="humLow">ឬសំណើមខ្យល់ &lt; (%)<input id="humLow" name="humLow" type="number" min="0" max="100"></label>
        </div>
      </fieldset>
      <fieldset><legend>បញ្ចូលទឹកធុងស្រោច (R3/R4)</legend>
        <div class="row">
          <label for="levelLow">ចាប់ផ្តើមពេល &lt; (%)<input id="levelLow" name="levelLow" type="number" min="11" max="94"></label>
          <label for="levelHigh">ឈប់ពេល ≥ (%)<input id="levelHigh" name="levelHigh" type="number" min="11" max="94"></label>
        </div>
      </fieldset>
      <div class="act"><button class="btn" type="submit">រក្សាទុក</button>
        <div class="cal" id="cal">Calibrate: pH -- mV · EC -- mV · Level -- mV</div></div>
    </form>
  </div>

  <footer>គម្រោង STEM · សាលាវិទ្យាល័យកំពង់ធំ · Smart GreenCycle</footer>
</div>
</div>
</div>
<div class="toast" id="toast" role="status" aria-live="polite"></div>

<script>
(function(){
  var $=function(i){return document.getElementById(i)};
  var DEMO=location.protocol==='file:'||location.search.indexOf('demo')>=0;
  var cfg={phMin:6,phMax:7.5,ecMax:2,soilLow:40,soilHigh:65,tempHigh:33,humLow:50,levelLow:30,levelHigh:85};
  var S=null, hist=[], lastOk=0;
  var ICON={
    drip:'<path d="M12 3s6 6.5 6 11a6 6 0 11-12 0c0-4.5 6-11 6-11z"/>',
    mist:'<path d="M7 16a4 4 0 01-.5-8A5.5 5.5 0 0117 7a3.5 3.5 0 01.5 7"/><path d="M8 20v.01M12 18v.01M16 20v.01M12 22v.01"/>',
    tap:'<path d="M4 9h8a4 4 0 014 4v2M4 6v6M16 18v1M18 20v1M14 20v1"/>',
    cycle:'<path d="M20 12a8 8 0 01-14 5.3M4 12a8 8 0 0114-5.3M18 3v4h-4M6 21v-4h4"/>'
  };
  var RL=[
    {name:'ម៉ូទ័របាញ់តាមគល់',sub:'R1 · Drip',icon:ICON.drip,lock:'ទឹកធុងស្រោចតិចពេក'},
    {name:'ម៉ូទ័របាញ់ចំហាយ',sub:'R2 · Mist',icon:ICON.mist,lock:'ទឹកធុងស្រោចតិចពេក'},
    {name:'វ៉ាល់ទឹកស្អាត',sub:'R3 · ទៅធុងស្រោច',icon:ICON.tap,lock:'ធុងស្រោចពេញ'},
    {name:'វ៉ាល់ទឹកច្រោះ',sub:'R4 · ទៅធុងស្រោច',icon:ICON.cycle,lock:'ធុងស្រោចពេញ'}
  ];

  var tt;
  function toast(m){var t=$('toast');t.textContent=m;t.classList.add('on');clearTimeout(tt);tt=setTimeout(function(){t.classList.remove('on')},2000)}
  function chip(id,cls,txt){var c=$(id);c.className='chip '+cls;c.textContent=txt}

  /* ---------- DEMO (បើកឯកសារលើកុំព្យូទ័រ) ---------- */
  var D={t:31.5,h:58,dht:true,soil:44,ph:6.9,ec:1.1,lvl:52,wq:true,src:0,auto:true,warm:0,fault:false,r:[0,0,0,0],blk:[0,0,0,0],mv:[1480,310,1090],up:'00:12:40',_s:760};
  function demo(path){
    var q={};(path.split('?')[1]||'').split('&').forEach(function(kv){var a=kv.split('=');if(a[0])q[a[0]]=decodeURIComponent(a[1]||'')});
    if(path.indexOf('/config')===0) return cfg;
    if(path.indexOf('/save')===0){for(var k in q) cfg[k]=+q[k]; return {ok:true,cfg:cfg}}
    if(path.indexOf('/mode')===0) D.auto=q.m==='auto';
    if(path.indexOf('/relay')===0){if(D.auto) return {ok:false,err:'Mode Auto: សូមប្តូរទៅ Manual សិន'}; D.r[q.id-1]=q.state==='1'?1:0}
    if(path.indexOf('/reset')===0) D.fault=false;
    if(path.indexOf('/status')===0){
      var r=function(a){return (Math.random()-.5)*a};
      D.t=Math.min(38,Math.max(26,D.t+r(.6))); D.h=Math.min(85,Math.max(40,D.h+r(2)));
      D.ph=Math.min(8.4,Math.max(5.4,D.ph+r(.25))); D.ec=Math.min(2.6,Math.max(.4,D.ec+r(.12)));
      D.soil=Math.max(20,Math.min(80,D.soil+(D.r[0]?2:-.4)));
      D.lvl=Math.max(5,Math.min(98,D.lvl+(D.r[2]||D.r[3]?3:0)-(D.r[0]?1:0)-(D.r[1]?.5:0)));
      D.wq=D.ph>=cfg.phMin&&D.ph<=cfg.phMax&&D.ec<=cfg.ecMax;
      if(D.auto){
        if(D.src===0&&D.lvl<cfg.levelLow) D.src=D.wq?1:2; else if(D.lvl>=cfg.levelHigh) D.src=0;
        D.r=[D.soil<cfg.soilLow?1:(D.soil>=cfg.soilHigh?0:D.r[0]), (D.t>cfg.tempHigh||D.h<cfg.humLow)?1:0, D.src===2?1:0, D.src===1?1:0];
      }
      D.blk=[D.lvl<10,D.lvl<10,D.lvl>=95,D.lvl>=95].map(Number);
      D._s+=2; var s=D._s; D.up=[s/3600|0,(s/60|0)%60,s%60].map(function(x){return (x<10?'0':'')+x}).join(':');
      D.mv=[Math.round((1.5-(D.ph-7)*.18)*1000),Math.round(D.ec/3.58*1000),Math.round(100+D.lvl*19)];
    }
    return JSON.parse(JSON.stringify(D));
  }
  function api(path){
    if(DEMO) return Promise.resolve(demo(path));
    return fetch(path,{cache:'no-store'}).then(function(r){return r.json()});
  }

  /* ---------- Relay cards ---------- */
  var cards=[];
  RL.forEach(function(r,i){
    var d=document.createElement('div'); d.className='relay';
    d.innerHTML='<div class="rtop"><div class="ric"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">'+r.icon+'</svg></div>'+
      '<button class="sw" id="sw'+(i+1)+'" role="switch" aria-checked="false" aria-label="'+r.name+'"></button></div>'+
      '<div><h3>'+r.name+'</h3><p class="st">'+r.sub+'</p></div>';
    d.querySelector('.sw').addEventListener('click',function(){
      if(!S) return;
      if(S.auto){toast('សូមប្តូរទៅ Manual សិន ទើបបញ្ជាដោយដៃបាន');return}
      var on=!S.r[i];
      api('/relay?id='+(i+1)+'&state='+(on?1:0)).then(function(d){
        if(d.ok===false){toast(d.err||'បញ្ជា Relay មិនបាន');return}
        render(d);
        if(on&&d.blk[i]) toast(r.name+' ត្រូវបានការពារ: '+r.lock);
        else toast(r.name+(on?' បានបើក':' បានបិទ'));
      }).catch(function(){toast('ភ្ជាប់ ESP32 មិនបាន')});
    });
    $('relays').appendChild(d); cards.push(d);
  });

  /* ---------- Render ---------- */
  function setPipe(id,on){$('p'+id).classList.toggle('on',!!on);$('l'+id).classList.toggle('on',!!on)}
  function render(d){
    S=d; lastOk=Date.now();
    // air
    if(d.dht){
      $('vt').textContent=d.t.toFixed(1); $('vh').textContent=Math.round(d.h);
      d.t>cfg.tempHigh?chip('ct','warn','ក្តៅ'):chip('ct','good','ធម្មតា');
      d.h<cfg.humLow?chip('ch','warn','ស្ងួត'):chip('ch','good','ធម្មតា');
    } else {$('vt').textContent='--';$('vh').textContent='--';chip('ct','bad','DHT11 error');chip('ch','bad','DHT11 error')}
    // soil
    var p=Math.round(d.soil); $('vs').textContent=p; $('sfill').style.width=p+'%'; $('smark').style.left=cfg.soilLow+'%';
    var st=$('sstat'),f=$('sfill');
    if(p<cfg.soilLow){st.textContent='ដីស្ងួត ត្រូវស្រោចទឹក';st.style.color='var(--warntx)';f.style.background='var(--amber)'}
    else if(p<80){st.textContent='សំណើមល្អ';st.style.color='var(--goodtx)';f.style.background='var(--green)'}
    else{st.textContent='ដីសើមខ្លាំង';st.style.color='var(--blue)';f.style.background='var(--blue)'}
    // pH / EC
    $('vec').textContent=d.ec.toFixed(2);
    if(d.phok===false){$('vph').textContent='ERR';chip('cph','bad','Sensor pH មានបញ្ហា')}
    else{$('vph').textContent=d.ph.toFixed(2);
    if(d.ph<cfg.phMin) chip('cph','warn','ទាប (អាស៊ីត)'); else if(d.ph>cfg.phMax) chip('cph','warn','ខ្ពស់ (បាស)'); else chip('cph','good','ល្អ');
    hist.push(d.ph); if(hist.length>90) hist.shift(); draw();}
    d.ec>cfg.ecMax?chip('cec','warn','ប្រៃ/កខ្វក់ពេក'):chip('cec','good','ល្អ');
    // water quality
    var box=$('wqBox'); box.className='wq '+(d.wq?'good':'bad');
    $('wqT').textContent=d.wq?'ទឹកច្រោះ អាចយកមកស្រោចបាន':'ទឹកច្រោះ មិនទាន់ល្អ → ប្រើទឹកស្អាត';
    $('wqS').textContent=d.src===1?'កំពុងបញ្ចូលទឹកច្រោះទៅធុងស្រោច (R4)':d.src===2?'កំពុងបញ្ចូលទឹកស្អាតទៅធុងស្រោច (R3)':(d.wq?'ពេលធុងស្រោចខ្វះទឹក នឹងប្រើទឹកច្រោះ (R4)':'ពេលធុងស្រោចខ្វះទឹក នឹងប្រើទឹកស្អាត (R3)');
    $('bS').classList.toggle('on',!!d.wq);
    setPipe(1,d.r[0]);setPipe(2,d.r[1]);setPipe(3,d.r[2]);setPipe(4,d.r[3]);
    $('b1').classList.toggle('on',!!d.r[0]); $('b2').classList.toggle('on',!!d.r[1]);
    var h=Math.round(d.lvl); $('tw').setAttribute('y',160-h); $('tw').setAttribute('height',h); $('tl').textContent=h+'%';
    // level
    $('vl').textContent=h; $('lfill').style.width=h+'%';
    var ls=$('lstat'),lf=$('lfill');
    if(h<10){ls.textContent='ទឹកជិតអស់ · ហាមម៉ូទ័រដំណើរការ';ls.style.color='var(--badtx)';lf.style.background='var(--red)'}
    else if(h<cfg.levelLow){ls.textContent='ទឹកខ្វះ · ត្រូវបញ្ចូលទឹក';ls.style.color='var(--warntx)';lf.style.background='var(--amber)'}
    else{ls.textContent='គ្រប់គ្រាន់';ls.style.color='var(--goodtx)';lf.style.background='var(--blue)'}
    // mode
    $('mAuto').setAttribute('aria-pressed',d.auto); $('mMan').setAttribute('aria-pressed',!d.auto);
    $('up').textContent='ដំណើរការ '+d.up;
    // relays
    var n=0;
    cards.forEach(function(c,i){
      var on=!!d.r[i]; if(on) n++;
      c.className='relay'+(on?' on':'');
      var sw=c.querySelector('.sw'); sw.setAttribute('aria-checked',on); sw.setAttribute('aria-disabled',d.auto);
      var stx=c.querySelector('.st');
      stx.innerHTML=RL[i].sub+' · '+(on?'ON':'OFF')+(d.auto?' · Auto':'')+(d.blk[i]?'<br><span class="lock">🔒 '+RL[i].lock+'</span>':'');
    });
    $('rcount').textContent='បើក '+n+' / 4';
    $('cal').textContent='Calibrate: pH Po '+d.mv[0]+' mV · EC '+d.mv[1]+' mV · Level '+d.mv[2]+' mV';
    // alerts
    var a='';
    if(DEMO) a+='<div class="alert info">DEMO · ទិន្នន័យក្លែងធ្វើ (បើកពី ESP32 ដើម្បីឃើញទិន្នន័យពិត)</div>';
    if(d.fault) a+='<div class="alert bad"><span>បញ្ចូលទឹកយូរពេក តែធុងស្រោចមិនឡើង។ សូមពិនិត្យ Valve ទុយោ និងប្រភពទឹក។</span><button type="button" id="rst">កំណត់ឡើងវិញ</button></div>';
    if(d.phok===false) a+='<div class="alert bad">Sensor pH អានមិនបាន (Po '+d.mv[0]+' mV)។ ពិនិត្យខ្សែ Probe ឬដក EC Probe ចេញពីធុង។ ប្រព័ន្ធប្រើទឹកស្អាតជំនួស។</div>';
    if(d.warm>0) a+='<div class="alert warn">កំពុងរង់ចាំ Sensor pH/EC ស្ងប់ ('+d.warm+'s) មុនចាប់ផ្តើម Auto</div>';
    $('alerts').innerHTML=a;
    if($('rst')) $('rst').onclick=function(){api('/reset').then(render)};
  }

  function draw(){
    var w=300,h=90,mn=4,mx=10,y=function(v){return h-4-((Math.min(mx,Math.max(mn,v))-mn)/(mx-mn))*(h-8)};
    $('band').setAttribute('y',y(cfg.phMax).toFixed(1)); $('band').setAttribute('height',(y(cfg.phMin)-y(cfg.phMax)).toFixed(1));
    if(hist.length<2){$('line').setAttribute('d','');return}
    $('line').setAttribute('d',hist.map(function(v,i){return (i?'L':'M')+(i*w/(hist.length-1)).toFixed(1)+' '+y(v).toFixed(1)}).join(' '));
    $('capL').textContent='pH 4–10 · '+hist.length+' ចំណុច';
  }

  function poll(){
    api('/status').then(render).catch(function(){});
    var off=Date.now()-lastOk>6000;
    $('live').classList.toggle('off',off); $('liveTx').textContent=off?'Offline':'Live';
  }

  /* ---------- Mode ---------- */
  function setMode(m){api('/mode?m='+m).then(function(d){render(d);toast(d.auto?'ប្តូរទៅ Auto':'ប្តូរទៅ Manual')}).catch(function(){toast('ភ្ជាប់ ESP32 មិនបាន')})}
  $('mAuto').onclick=function(){setMode('auto')};
  $('mMan').onclick=function(){setMode('manual')};

  /* ---------- Settings ---------- */
  function fillCfg(){for(var k in cfg) if($(k)) $(k).value=cfg[k]}
  $('cfgForm').addEventListener('submit',function(e){
    e.preventDefault();
    var q=[];for(var k in cfg) q.push(k+'='+encodeURIComponent($(k).value));
    api('/save?'+q.join('&')).then(function(r){
      if(r.ok){cfg=r.cfg;fillCfg();toast('បានរក្សាទុក');if(S)render(S)} else toast(r.err||'តម្លៃមិនត្រឹមត្រូវ');
    }).catch(function(){toast('ភ្ជាប់ ESP32 មិនបាន')});
  });

  /* ---------- Tabs + nav ---------- */
  var tabs=document.querySelectorAll('.tabs[aria-label="ត្រងទិន្នន័យ"] button'), tiles=document.querySelectorAll('.grid .tile');
  tabs.forEach(function(b){b.addEventListener('click',function(){
    tabs.forEach(function(x){x.setAttribute('aria-pressed',x===b)});
    tiles.forEach(function(t){t.classList.toggle('hide',b.dataset.f!=='all'&&t.dataset.g!==b.dataset.f)});
  })});
  var navs=document.querySelectorAll('.nav a');
  navs.forEach(function(a){a.addEventListener('click',function(){navs.forEach(function(x){x.classList.toggle('cur',x===a)})})});

  api('/config').then(function(c){cfg=c;fillCfg()}).catch(fillCfg).then(poll);
  setInterval(poll,2000);
})();
</script>
</body>
</html>)rawliteral";
