#ifndef _WM_STRINGS_H_
#define _WM_STRINGS_H_

#include <Arduino.h>

const char WM_HTTP_STYLE[] PROGMEM = "<style>"
	".c,body{text-align:center;font-family:sans-serif}"
	"body,textarea,input,select{border-radius: 0.3rem;font-size: 16px}"

	"div{padding:2px;font-size:1em;}"
	".container{margin:auto;width:95%}"
	// msg callout
	".msg{background: #def;border-left: 5px solid #59d;padding: 0.5em}"
	//".msg{padding:20px;margin:20px 0;border:1px solid #eee;border-left-width:5px;border-left-color:#777}
	".msg h3{text-align:center;margin-top:10px;margin-bottom:14px}"							// Info panel header
	".msg h4{margin-top:10px;margin-bottom:3px}"                           // Debug message panel titles.
	//.msg.P{border-left-color:#1fa3ec}.msg.P h4{color:#1fa3ec}
	//.msg.D{border-left-color:#dc3630}.msg.D h4{color:#dc3630}
	//.msg.S{border-left-color: #5cb85c}.msg.S h4{color: #5cb85c}"

	// panel border
	"fieldset{min-width:0;text-align:left;border-radius:0.5rem;margin-bottom:5px;padding: 0.4em 0.5em 0.2em 0.5em}"

	"textarea,input,select{box-sizing: border-box;outline: 0;border: 2px solid #ccc;padding: 5px;margin: 5px 0;width: 100%}"
	"textarea:focus,input:focus,select:focus{border-color: #5ab}"
	"input[type='checkbox']{float: right;width: 20px;margin: 0px}"

	// Media?? Just makes the frame jump around as the browser width changes?	
	//"@media(min-width:1200px){.container{width: 30%}}"
	//"@media(min-width:768px) and (max-width:1200px){.container{width: 50%}}"

	//buttons
	".btn a{text-decoration: none}"
	".btn,h2{font-size: 1.5em}"
	"h1{font-size: 2em}"
	".btn{background: #0ae;border-radius: 10px;border: 0;color: #fff;cursor: pointer;display: inline-block;margin: -3px 0;padding: 10px 0px 10px;width: 100%}"
	".btn:hover{background: #09d}"
	".btn:active,.btn:focus{background: #08b}"
	//links
	"a{color:#000;font-weight:700;text-decoration:none}a:hover{color:#1fa3ec;text-decoration:underline}"
	//buttons from wifimanager	
	"button{transition: 0s opacity;transition-delay: 3s;transition-duration: 0s;cursor: pointer;font-size:16px}"
	"button.D{background-color:#dc3630;color:#ffffff}"	// OTA Update Button.
	"button:active{opacity:50% !important;cursor:wait;transition-delay: 0s}"
	//Labels (Main text)
	"label {font-weight: bold}"
	"label>*{display: inline}"
	"form>*{display: block;margin-bottom: 5px}"
	
	// quality icons
	//".q{height:16px;margin:0;padding:0 5px;text-align:right;min-width:38px;float:right}"
	//".q.q-0:after{background-position-x:0}"
	//".q.q-1:after{background-position-x:-16px}"
	//".q.q-2:after{background-position-x:-32px}"
	//".q.q-3:after{background-position-x:-48px}"
	//".q.q-4:after{background-position-x:-64px}"
	//".q.l:before{background-position-x:-80px;padding-right:5px}"
	//".ql .q{float:left}"
	//".q:after,.q:before{content:'';width:16px;height:16px;display:inline-block;background-repeat:no-repeat;background-position: 16px 0;"
	//"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAAAQCAMAAADeZIrLAAAAJFBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADHJj5lAAAAC3RSTlMAIjN3iJmqu8zd7vF8pzcAAABsSURBVHja7Y1BCsAwCASNSVo3/v+/BUEiXnIoXkoX5jAQMxTHzK9cVSnvDxwD8bFx8PhZ9q8FmghXBhqA1faxk92PsxvRc2CCCFdhQCbRkLoAQ3q/wWUBqG35ZxtVzW4Ed6LngPyBU2CobdIDQ5oPWI5nCUwAAAAASUVORK5CYII=');}"

	// Encryption padlock Icon
	".q{float: right;width: 64px;text-align: right}"
	".l{background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==') no-repeat left center;background-size: 1em}"
	
	// Info Table
	".table {width: 100%}"
	".table td{padding:.2em;text-align:left}"
	".table tbody>:nth-child(2n-1){background:#ddd}"
	"</style>";


const char WM_HTTP_SCRIPT[] PROGMEM = "<script>function c(l){"
		"document.getElementById('s').value=l.innerText||l.textContent;"
		"document.getElementById('p').focus();"
	//	"document.getElementById('s1').value=l.innerText||l.textContent;"
	//	"document.getElementById('p1').focus();"
	//	"document.getElementById('timezone').value=timezone.name();"
		"}</script>";

const char WM_HTTP_HEAD_START[] PROGMEM = "<!DOCTYPE html>"
	"<html lang='en'><head>"
	//"<meta name='format-detection' content='telephone=no'><meta charset='UTF-8'>"
	"<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
	"<title>{v}</title>";

//const char WM_HTTP_HEAD_END[] PROGMEM = "</head><body><div class='container'><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char WM_HTTP_HEAD_END[] PROGMEM = "</head><body><div class='container'><div style='text-align:left;display:block;min-width:260px;'>";

const char WM_HTTP_200[]      PROGMEM = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const char WM_HTTP_END[]      PROGMEM = "</div></body></html>";
const char WM_HTTP_HELP[]	  PROGMEM = "<small><p>pwrGenie uses ASYNC_WifiManager at</p><p><a href=\"https://github.com/khoih-prog/ESPAsync_WiFiManager\">https://github.com/khoih-prog/ESPAsync_WiFiManager</a></p></small>";

//										   "<form method='POST' action='/doUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
const char WM_HTTP_UPDATE[] PROGMEM = "<br/><form method='POST' action='/doUpdate' enctype='multipart/form-data' "
			"onchange=\"(function(el){document.getElementById('uploadbin').style.display = el.value=='' ? 'none' : 'initial';})(this)\">"
			"<input type='file' name='update' accept='.bin,application/octet-stream'>"
				"<button id='uploadbin' type='submit' class='h D'>Update</button></form>"
				"<small><a href='http://192.168.4.1/update' target='_blank'>* May not function inside captive portal, Open in browser http://192.168.4.1</a></small>";

const char WM_HTTP_UPDATE_FAIL[] 	PROGMEM = "<br/><div class='msg D'><strong>Update Failed!</strong><br/>Reboot device and try again</div>";
const char WM_HTTP_UPDATE_SUCCESS[] PROGMEM = "<br/><div class='msg S'><strong>Update Successful.</strong><br/>Device Rebooting now...</div>";

const char WM_META_AUTO_ROOT[] 		PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; url=/\">";
const char WM_META_AUTO_DEBUG[] 	PROGMEM = "<meta http-equiv=\"refresh\" content=\"10; url=/debug\">";

const char WM_HTTP_HEAD_CL[]        PROGMEM = "Content-Length";
const char WM_HTTP_HEAD_CT[]        PROGMEM = "text/html";
const char WM_HTTP_HEAD_CT2[]       PROGMEM = "text/plain";
const char WM_HTTP_HEAD_JSON[]      PROGMEM ="application/json";

//KH Add repeatedly used const
const char WM_HTTP_CACHE_CONTROL[]   = "Cache-Control";
const char WM_HTTP_NO_STORE[]        = "no-cache, no-store, must-revalidate";
const char WM_HTTP_PRAGMA[]          = "Pragma";
const char WM_HTTP_NO_CACHE[]        = "no-cache";
const char WM_HTTP_EXPIRES[]         = "Expires";
const char WM_HTTP_CORS[]            = "Access-Control-Allow-Origin";
const char WM_HTTP_CORS_ALLOW_ALL[]  = "*";

#ifdef USE_AVAILABLE_PAGES
  const char WM_HTTP_AVAILABLE_PAGES[] PROGMEM = "<h3>Available Pages</h3>"
  "<table class='table'><thead><tr><th>Page</th><th>Function</th></tr></thead><tbody>"
  "<tr><td><a href='/'>/</a></td><td>Menu page.</td></tr>"
  "<tr><td><a href='/wifi'>/wifi</a></td><td>Enter WiFi Config Page with scan results.</td></tr>"
  "<tr><td><a href='/wifisave'>/wifisave</a></td><td>Save WiFi Config with supplied credentials.</td></tr>"
  "<tr><td><a href='/cfg'>/cfg</a></td><td>Enter Device functions Config Page.</td></tr>"
  "<tr><td><a href='/cfgsave'>/cfgsave</a></td><td>Save Device functions Configuration.</td></tr>"
  "<tr><td><a href='/debug'>/debug</a></td><td>Show device function last data values.</td></tr>"
  "<tr><td><a href='/close'>/close</a></td><td>Close the Config Portal.</td></tr>"
  "<tr><td><a href='/i1'>/i1</a></td><td>Info V1 page.</td></tr>"
  "<tr><td><a href='/i2'>/i1</a></td><td>Info V2 page.</td></tr>"
  "<tr><td><a href='/restart'>/restart</a></td><td>Reboot the device.</td></tr>"
  "<tr><td><a href='/erase'>/erase</a></td><td>Restore the firmware to default config values.</td></tr>"
  "<tr><td><a href='/update'>/update</a></td><td>Upload a new firmware OTA.</td></tr>"
  //"<tr><td><a href='/state'>/state</a></td><td>Current device state in JSON format. Interface for programmatic WiFi configuration.</td></tr>"
  //"<tr><td><a href='/scan'>/scan</a></td><td>Run a WiFi scan and return results in JSON format. Interface for programmatic WiFi configuration.</td></tr>"
  "</tbody></table>";
#else
  const char WM_HTTP_AVAILABLE_PAGES[] PROGMEM = "";
#endif

const char WM_FLDSET_START[]  PROGMEM = "<fieldset>";
const char WM_FLDSET_END[]    PROGMEM = "</fieldset>";

const char WM_HTTP_PORTAL_OPTIONS[] PROGMEM = "<br/>"
	"<form action='/cfg'  method='get'><button class='btn'>Configure Device</button></form><br/>\n"		// Config Device
	"<form action='/wifi' method='get'><button class='btn'>Configure WiFi</button></form><br/>\n"		// Config Wifi
	"<form action='/i1'   method='get'><button class='btn'>Info V1</button></form><br/>\n"				// Info1
	"<form action='/i2'   method='get'><button class='btn'>Info V2</button></form><br/>\n"				// Info2
	//"<form action='/close' method='get'><button class='btn'>Exit Portal</button></form><br/>\n"		// close portal
	"<hr><br/>\n"
	"<form action='/debug'   method='get'><button class='btn'>Debug</button></form><br/>\n"				// debug
	"<form action='/update'  method='get'><button class='btn'>Update</button></form><br/>\n"			// update
	"<form action='/restart' method='get' onsubmit='return confirm(\"Are you sure you want to REBOOT?\");'><button class='btn'>Restart</button></form><br/>\n"						// restart
	"<form action='/erase'   method='get' onsubmit='return confirm(\"Are you REAAALLY sure you want to ERASE the config?\");'><button class='btn'>* ERASE *</button></form><br/>\n"	// erase
	;

// Wifi quality icons:
//const char WM_HTTP_ITEM_QI[]          PROGMEM = "<div role='img' aria-label='{r}%' title='{r}%' class='q q-{q} {i} {h}'></div>"; // rssi icons
//const char WM_HTTP_ITEM_QP[]          PROGMEM = "<div class='q {h}'>{r}%</div>"; // rssi percentage {h} = hidden showperc pref
//const char WM_HTTP_ITEM[]             PROGMEM = "<div><a href='#p' onclick='c(this)' data-ssid='{V}'>{v}</a>{qi}{qp}</div>"; // {q} = HTTP_ITEM_QI, {r} = HTTP_ITEM_QP


const char WM_HTTP_ITEM[] PROGMEM = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char JSON_ITEM[] PROGMEM    = "{\"SSID\":\"{v}\", \"Encryption\":{i}, \"Quality\":\"{r}\"}";

const char HTTP_HOMEBTN[]          PROGMEM = "<br>&nbsp;<form action='/' method='get'><button>Home</button></form>";
const char HTTP_FORM_LABEL[]       PROGMEM = "<label for='{i}'>{t}</label>";
const char HTTP_FORM_PARAM[]       PROGMEM = "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>\n"; // do not remove newline!
const char HTTP_FORM_BOOL_PARAM[]  PROGMEM = "<input type='checkbox' id='{i}' name='{n}' length='{l}' value='1' {c}><br/>";

//const char HTTP_ROOT_MAIN[]        PROGMEM = "<h1>{t}</h1><h3 style='font-size: 1.1em'>{v}</h3>";
const char HTTP_PAGE_MAIN1[]       PROGMEM = "<h1>{t}</h1><h3><span style='float:left'>{h}</span><span style='float:right'>{i}</span></h3>";
const char HTTP_PAGE_MAIN2[]	   PROGMEM = "<table style='width:100%'><tr><td><b>Uptime:</b>{1} Min {2} Sec</td><td><b>PSU:</b>{p}v</td><td style='text-align:right'><b>FW:</b>{m}</td></tr></table></div>";
const char HTTP_FORM_POST[]        PROGMEM = "<form method='POST' action='{v}'>";
const char HTTP_FORM_PARAM_HEAD[]  PROGMEM = "Configure Device Parameters";
const char HTTP_FORM_WIFI_HEAD[]   PROGMEM = "Configure Device Wifi";
const char HTTP_FORM_INFO_HEAD[]   PROGMEM = "Device Info";
const char HTTP_FORM_END[]         PROGMEM = "<br/><br/><button type='submit'>Save</button></form>";


// Info html
// @todo remove html elements from progmem, repetatve strings
#ifdef ESP32
	const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp32</h3><hr><dl>";
	const char HTTP_INFO_chiprev[]    PROGMEM = "<td>Chip Rev</td><td>{1}</td>";
  	//const char HTTP_INFO_lastreset[]  PROGMEM = "<td>Last reset reason</td><td>CPU0: {1}<br/>CPU1: {2}</td>";
  	const char HTTP_INFO_aphost[]     PROGMEM = "<td>Access Point Hostname</td><td>{1}</td>";
    const char HTTP_INFO_psrsize[]    PROGMEM = "<td>PSRAM Size</td><td>{1} bytes</td>";
	const char HTTP_INFO_temp[]       PROGMEM = "<td>Temperature</td><td>{1} C&deg; / {2} F&deg;</td></tr><tr><td>Hall</td><td>{3}</td>";
#else
	const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp8266</h3><hr>";
	const char HTTP_INFO_fchipid[]    PROGMEM = "<td>Flash Chip ID</td><td>{1}</td>";
	const char HTTP_INFO_corever[]    PROGMEM = "<td>Core Version</td><td>{1}</td>";
	const char HTTP_INFO_bootver[]    PROGMEM = "<td>Boot Version</td><td>{1}</td>";
	const char HTTP_INFO_lastreset[]  PROGMEM = "<td>Last reset reason</td><td>{1}</td>";
	const char HTTP_INFO_flashsize[]  PROGMEM = "<td>Real Flash Size</td><td>{1} bytes</td>";
	const char HTTP_INFO_apssid[]     PROGMEM = "<td>Access Point SSID</td><td>{1}</td>";
	const char HTTP_INFO_autoconx[]   PROGMEM = "<td>Autoconnect</td><td>{1}</td>";

#endif

const char HTTP_INFO_memsmeter[]  PROGMEM = "<td><progress value='{1}' max='{2}'></progress></td>";
const char HTTP_INFO_memsketch[]  PROGMEM = "<td>Memory - Sketch Size</td><td>Used / Total bytes<br/>{1} / {2}";
const char HTTP_INFO_freeheap[]   PROGMEM = "<td>Memory - Free Heap</td><td>{1} bytes available</td>";
const char HTTP_INFO_wifihead[]   PROGMEM = "<td>WiFi mode</td><td>{1}</td>";
const char HTTP_INFO_uptime[]     PROGMEM = "<td>Uptime</td><td>{1} Mins {2} Secs</td>";
const char HTTP_INFO_chipid[]     PROGMEM = "<td>Chip ID</td><td>{1}</td>";
const char HTTP_INFO_idesize[]    PROGMEM = "<td>Flash Size</td><td>{1} bytes</td>";
const char HTTP_INFO_sdkver[]     PROGMEM = "<td>SDK Version</td><td>{1}</td>";
const char HTTP_INFO_cpufreq[]    PROGMEM = "<td>CPU Frequency</td><td>{1}MHz</td>";
const char HTTP_INFO_apip[]       PROGMEM = "<td>Access Point IP</td><td>{1}</td>";
const char HTTP_INFO_apmac[]      PROGMEM = "<td>Access Point MAC</td><td>{1}</td>";
const char HTTP_INFO_apbssid[]    PROGMEM = "<td>BSSID</td><td>{1}</td>";
const char HTTP_INFO_stassid[]    PROGMEM = "<td>Station SSID</td><td>{1}</td>";
const char HTTP_INFO_staip[]      PROGMEM = "<td>Station IP</td><td>{1}</td>";
const char HTTP_INFO_stagw[]      PROGMEM = "<td>Station Gateway</td><td>{1}</td>";
const char HTTP_INFO_stasub[]     PROGMEM = "<td>Station Subnet</td><td>{1}</td>";
const char HTTP_INFO_dnss[]       PROGMEM = "<td>DNS Server</td><td>{1}</td>";
const char HTTP_INFO_host[]       PROGMEM = "<td>Hostname</td><td>{1}</td>";
const char HTTP_INFO_stamac[]     PROGMEM = "<td>Station MAC</td><td>{1}</td>";
const char HTTP_INFO_conx[]       PROGMEM = "<td>Connected</td><td>{1}</td>";

const char HTTP_INFO_aboutver[]     PROGMEM = "<td>WiFiManager</td><td>{1}</td>";
//const char HTTP_INFO_aboutarduino[] PROGMEM = "<td>Arduino</td><td>{1}</td>";
//const char HTTP_INFO_aboutsdk[]     PROGMEM = "<td>ESP-SDK/IDF</td><td>{1}</td>";
const char HTTP_INFO_aboutdate[]    PROGMEM = "<td>Build Date</td><td>{1}</td>";


const char S_brand[]              PROGMEM = "pwrGenie Manager";
const char S_y[]                  PROGMEM = "Yes";
const char S_n[]                  PROGMEM = "No";
const char S_enable[]             PROGMEM = "Enabled";
const char S_disable[]            PROGMEM = "Disabled";
const char S_GET[]                PROGMEM = "GET";
const char S_POST[]               PROGMEM = "POST";
const char S_NA[]                 PROGMEM = "Unknown";
const char S_titlewifi[]          PROGMEM = "Config wifi";
const char S_titlewifisaved[]     PROGMEM = "wifi Saved";
const char S_titleinfo1[]         PROGMEM = "Info1";
const char S_titleinfo2[]         PROGMEM = "Info2";
const char S_titleparam[]         PROGMEM = "Config Device";
const char S_titleparamsaved[]    PROGMEM = "Config Saved";
const char S_uri[]                PROGMEM = "URI: ";
const char S_method[]             PROGMEM = "\nMethod: ";
const char S_args[]               PROGMEM = "\nArguments: ";
const char S_parampre[]           PROGMEM = "param_";

//These for getInfoData()
const char T_1[]                  PROGMEM = "{1}"; // @token 1
const char T_2[]                  PROGMEM = "{2}"; // @token 2
const char T_3[]                  PROGMEM = "{3}"; // @token 2
//
const char T_I[]                  PROGMEM = "{I}"; // @token I
const char T_c[]                  PROGMEM = "{c}"; // @token c
const char T_i[]                  PROGMEM = "{i}"; // @token i
const char T_n[]                  PROGMEM = "{n}"; // @token n
const char T_p[]                  PROGMEM = "{p}"; // @token p
const char T_t[]                  PROGMEM = "{t}"; // @token t
const char T_l[]                  PROGMEM = "{l}"; // @token l
const char T_v[]                  PROGMEM = "{v}"; // @token v
const char T_m[]				  PROGMEM = "{m}"; // this is getting obvious...
const char T_h[]				  PROGMEM = "{h}"; 
#endif