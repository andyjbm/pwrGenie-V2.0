#ifndef _WM_STRINGS_H_
#define _WM_STRINGS_H_

#include <Arduino.h>

const char WM_HTTP_STYLE[] 		PROGMEM = "<link rel='stylesheet' type='text/css' href='style.css'>";
const char WM_HTTP_STYLE_CSS[]  PROGMEM = 
	"body{font-family:sans-serif}" //text-align:center; - removed..?
	"body,textarea,input,select{border-radius: 0.3rem;font-size: 16px}"
	"div{padding:2px;font-size:1em;}"
	".container{margin:auto;width:95%}"
	".tdiv{text-align:left;display:block;min-width:260px;}"
	"p{margin-top:10px;margin-bottom:5px}" // Footer credits and version info. WM_HTTP_HELP, HTTP_PAGE_MAIN3
	".fr,.fh{float:right}.fl{float:left}.tar{text-align:right}" 		// div/Span float right, float left directives in HTTP_PAGE_MAIN1. fh is free heap class.
	".fh{margin-top:1.5em}"												// free heap.

	// msg callout
	".msg{background:#def;border-left:5px solid #59d;padding:0.5em}"  		// .msg Blue #1fa3ec default.
	".msg h3{text-align:center;margin-top:10px;margin-bottom:14px}"			// Info panel header
	".msg h4{margin-top:10px;margin-bottom:3px}"                            // <h4> isDebug message panel titles.
	//.msg.P{border-left-color:#1fa3ec}.msg.P h4{color:#1fa3ec}

	// Only used for FW Update result because it's the only one that can pass or fail.
	".msg.D{border-left-color:#dc3630}.msg.D h4{color:#dc3630}"				//D = Failed, Red
	".msg.S{border-left-color:#5cb85c}.msg.S h4{color:#5cb85c}"			    //S = Pass, Green

	// panel border
	"fieldset{min-width:0;text-align:left;border-radius:0.5rem;margin-bottom:5px;padding: 0.4em 0.5em 0.2em 0.5em}"

	"textarea,input,select{box-sizing:border-box;outline:0;border:2px solid #ccc;padding:5px;margin:5px 0;width:100%}"
	"textarea:focus,input:focus,select:focus{border-color:#5ab}"
	"input[type='checkbox']{float:right;width:20px;margin:0px}"

	// Media?? Just makes the frame jump around as the browser width changes?	
	//"@media(min-width:1200px){.container{width: 30%}}"
	//"@media(min-width:768px) and (max-width:1200px){.container{width: 50%}}"

	//buttons
	".btn a{text-decoration:none}"
	".btn,h2{font-size:1.5em}"
	"h1{font-size:2em;margin-top:.25em}" // Margin-top to allow free heap to line up opposite.
	".btn{background: #0ae;border-radius: 10px;border: 0;color: #fff;cursor: pointer;display: inline-block;margin: -3px 0;padding: 10px 0px 10px;width: 100%}"
	".btn:hover{background: #09d}"
	".btn:active,.btn:focus{background: #08b}"
	//links
	"a{color:#000;font-weight:700;text-decoration:none}a:hover{color:#1fa3ec;text-decoration:underline}"
	//buttons from wifimanager	
	"button{transition: 0s opacity;transition-delay: 3s;transition-duration: 0s;cursor: pointer;font-size:16px}"
	"button.D{background-color:#dc3630;color:#ffffff}"	// OTA Update Button.
	"button:active{opacity:50% !important;cursor:wait;transition-delay: 0s}"
	//Labels Config & Wifi Pages (Main text)
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
	".table tbody>:nth-child(2n-1){background:#ddd}";

const char WM_HTTP_SCRIPT[] PROGMEM = "<script>function c(l){"
		"document.getElementById('s').value=l.innerText||l.textContent;"
		"document.getElementById('p').focus();"
	//	"document.getElementById('s1').value=l.innerText||l.textContent;"
	//	"document.getElementById('p1').focus();"
	//	"document.getElementById('timezone').value=timezone.name();"
		"}</script>";

const char WM_HTTP_HEAD_START[] PROGMEM = "<!DOCTYPE html><html lang='en'><head>"
	//"<meta name='format-detection' content='telephone=no'><meta charset='UTF-8'>"
	"<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
	"<title>{v}</title>";

const char WM_HTTP_HEAD_END[] PROGMEM = "</head><body><div class='container'>";

const char WM_HTTP_200[]      PROGMEM = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const char WM_HTTP_END[]      PROGMEM = "</div></body></html>";
const char WM_HTTP_HELP[]	  PROGMEM = "<small><p>pwrGenie uses ASYNC_WifiManager at</p><p><a href=\"https://github.com/khoih-prog/ESPAsync_WiFiManager\">https://github.com/khoih-prog/ESPAsync_WiFiManager</a></p></small>";

//										   "<form method='POST' action='/doUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
const char WM_HTTP_UPDATE[] PROGMEM = "<br/><form method='POST' action='/doUpdate' enctype='multipart/form-data' "
			"onchange=\"(function(el){document.getElementById('uploadbin').style.display = el.value=='' ? 'none' : 'initial';})(this)\">"
			"<input type='file' name='update' accept='.bin,application/octet-stream'>"
				"<button id='uploadbin' type='submit' class='D'>Update</button></form>"
				"<small><a href='http://192.168.4.1/update' target='_blank'>* May not function inside captive portal, Open in browser http://192.168.4.1</a></small>";

const char WM_HTTP_UPDATE_FAIL[] 	PROGMEM = "<br/><div class='msg D'><strong>Update Failed!</strong><br/>Reboot device and try again</div>";
const char WM_HTTP_UPDATE_SUCCESS[] PROGMEM = "<br/><div class='msg S'><strong>Update Successful.</strong><br/>Device Rebooting now...</div>";

const char WM_META_AUTO_ROOT[] 		PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; url=/\">";			// 5 seconds
const char WM_META_AUTO_DEBUG_10s[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"10; url=/debug\">";	// 10 seconds
const char WM_META_AUTO_DEBUG_1s[] 	PROGMEM = "<meta http-equiv=\"refresh\" content=\"1; url=/debug\">";	// 1 second

const char WM_HTTP_HEAD_CL[]        PROGMEM = "Content-Length";
const char WM_HTTP_HEAD_CT[]        PROGMEM = "text/html";
const char WM_HTTP_HEAD_CT2[]       PROGMEM = "text/plain";
const char WM_HTTP_HEAD_JSON[]      PROGMEM = "application/json";

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
  "<tr><td><a href='/i'>/i</a></td><td>Info page.</td></tr>"
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
	"<form action='/i'    method='get'><button class='btn'>Info Page</button></form><br/>\n"			// Info
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
//const char JSON_ITEM[] PROGMEM    = "{\"SSID\":\"{v}\", \"Encryption\":{i}, \"Quality\":\"{r}\"}";

const char HTTP_HOMEBTN[]          PROGMEM = "<br>&nbsp;<form action='/' method='get'><button>Home</button></form>";
const char HTTP_FORM_LABEL[]       PROGMEM = "<label for='{i}'>{t}</label>";
const char HTTP_FORM_PARAM[]       PROGMEM = "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>\n"; // do not remove newline!
const char HTTP_FORM_BOOL_PARAM[]  PROGMEM = "<input type='checkbox' id='{i}' name='{n}' length='{l}' value='1' {c}><br/>";

const char HTTP_PAGE_MAIN1[]       PROGMEM = "<div class='tdiv'><small class='fh'>{fh}</small><h1>{t}</h1><h3><span class='fl'>{h}</span><span class='fr'>{i}</span></h3>";
const char HTTP_PAGE_MAIN2[]	   PROGMEM = "<table style='width:100%'><tr><td><b>Uptime:</b>{1}h {2}m {3}s</td><td><b>PSU:</b>{p}v</td><td class='tar'><b>FW:</b>{m}</td></tr></table></div>";
const char HTTP_PAGE_MAIN3[]	   PROGMEM = "<p><small>{h}</small></p>";
const char HTTP_FORM_POST[]        PROGMEM = "<form method='POST' action='{v}'>";
const char HTTP_FORM_PARAM_HEAD[]  PROGMEM = "Configure Device Parameters";
const char HTTP_FORM_WIFI_HEAD[]   PROGMEM = "Configure Device Wifi";
const char HTTP_FORM_INFO_HEAD[]   PROGMEM = "Device Info";
const char HTTP_FORM_END[]         PROGMEM = "<br/><br/><button type='submit'>Save</button></form>";

#if USE_ESP_WIFIMANAGER_NTP

  #include "utils/TZ.h"
  const char WM_HTTP_SCRIPT_NTP_MSG[] PROGMEM = "<p>Your Timezone is : <b><label id='timezone' name='timezone'></b><script>document.getElementById('timezone').innerHTML=timezone.name();document.getElementById('timezone').value=timezone.name();</script></p>";
  const char WM_HTTP_SCRIPT_NTP_HIDDEN[] PROGMEM = "<p><input type='hidden' id='timezone' name='timezone'><script>document.getElementById('timezone').innerHTML=timezone.name();document.getElementById('timezone').value=timezone.name();</script></p>";

  #if ESP8266
    #if !(USE_CLOUDFLARE_NTP)
      #undef USE_CLOUDFLARE_NTP
      #define USE_CLOUDFLARE_NTP      true
      
      #if (_ESPASYNC_WIFIMGR_LOGLEVEL_ > 3)
        //#warning Forcing USE_CLOUDFLARE_NTP for ESP8266 as low memory can cause blank page
      #endif
    #endif
  #endif


  #if USE_CLOUDFLARE_NTP
    const char WM_HTTP_SCRIPT_NTP[] PROGMEM = "<script src='https://cdnjs.cloudflare.com/ajax/libs/jstimezonedetect/1.0.7/jstz.min.js'></script><script>var timezone=jstz.determine();console.log('Your CloudFlare timezone is:' + timezone.name());document.getElementById('timezone').innerHTML = timezone.name();</script>";
  #else
    const char WM_HTTP_SCRIPT_NTP[] PROGMEM = "<script>(function(e){var t=function(){'use strict';var e='s',n=function(e){var t=-e.getTimezoneOffset();return t!==null?t:0},r=function(e,t,n){var r=new Date;return e!==undefined&&r.setFullYear(e),r.setDate(n),r.setMonth(t),r},i=function(e){return n(r(e,0,2))},s=function(e){return n(r(e,5,2))},o=function(e){var t=e.getMonth()>7?s(e.getFullYear()):i(e.getFullYear()),r=n(e);return t-r!==0},u=function(){var t=i(),n=s(),r=i()-s();return r<0?t+',1':r>0?n+',1,'+e:t+',0'},a=function(){var e=u();return new t.TimeZone(t.olson.timezones[e])},f=function(e){var t=new Date(2010,6,15,1,0,0,0),n={'America/Denver':new Date(2011,2,13,3,0,0,0),'America/Mazatlan':new Date(2011,3,3,3,0,0,0),'America/Chicago':new Date(2011,2,13,3,0,0,0),'America/Mexico_City':new Date(2011,3,3,3,0,0,0),'America/Asuncion':new Date(2012,9,7,3,0,0,0),'America/Santiago':new Date(2012,9,3,3,0,0,0),'America/Campo_Grande':new Date(2012,9,21,5,0,0,0),'America/Montevideo':new Date(2011,9,2,3,0,0,0),'America/Sao_Paulo':new Date(2011,9,16,5,0,0,0),'America/Los_Angeles':new Date(2011,2,13,8,0,0,0),'America/Santa_Isabel':new Date(2011,3,5,8,0,0,0),'America/Havana':new Date(2012,2,10,2,0,0,0),'America/New_York':new Date(2012,2,10,7,0,0,0),'Asia/Beirut':new Date(2011,2,27,1,0,0,0),'Europe/Helsinki':new Date(2011,2,27,4,0,0,0),'Europe/Istanbul':new Date(2011,2,28,5,0,0,0),'Asia/Damascus':new Date(2011,3,1,2,0,0,0),'Asia/Jerusalem':new Date(2011,3,1,6,0,0,0),'Asia/Gaza':new Date(2009,2,28,0,30,0,0),'Africa/Cairo':new Date(2009,3,25,0,30,0,0),'Pacific/Auckland':new Date(2011,8,26,7,0,0,0),'Pacific/Fiji':new Date(2010,11,29,23,0,0,0),'America/Halifax':new Date(2011,2,13,6,0,0,0),'America/Goose_Bay':new Date(2011,2,13,2,1,0,0),'America/Miquelon':new Date(2011,2,13,5,0,0,0),'America/Godthab':new Date(2011,2,27,1,0,0,0),'Europe/Moscow':t,'Asia/Yekaterinburg':t,'Asia/Omsk':t,'Asia/Krasnoyarsk':t,'Asia/Irkutsk':t,'Asia/Yakutsk':t,'Asia/Vladivostok':t,'Asia/Kamchatka':t,'Europe/Minsk':t,'Australia/Perth':new Date(2008,10,1,1,0,0,0)};return n[e]};return{determine:a,date_is_dst:o,dst_start_for:f}}();t.TimeZone=function(e){'use strict';var n={'America/Denver':['America/Denver','America/Mazatlan'],'America/Chicago':['America/Chicago','America/Mexico_City'],'America/Santiago':['America/Santiago','America/Asuncion','America/Campo_Grande'],'America/Montevideo':['America/Montevideo','America/Sao_Paulo'],'Asia/Beirut':['Asia/Beirut','Europe/Helsinki','Europe/Istanbul','Asia/Damascus','Asia/Jerusalem','Asia/Gaza'],'Pacific/Auckland':['Pacific/Auckland','Pacific/Fiji'],'America/Los_Angeles':['America/Los_Angeles','America/Santa_Isabel'],'America/New_York':['America/Havana','America/New_York'],'America/Halifax':['America/Goose_Bay','America/Halifax'],'America/Godthab':['America/Miquelon','America/Godthab'],'Asia/Dubai':['Europe/Moscow'],'Asia/Dhaka':['Asia/Yekaterinburg'],'Asia/Jakarta':['Asia/Omsk'],'Asia/Shanghai':['Asia/Krasnoyarsk','Australia/Perth'],'Asia/Tokyo':['Asia/Irkutsk'],'Australia/Brisbane':['Asia/Yakutsk'],'Pacific/Noumea':['Asia/Vladivostok'],'Pacific/Tarawa':['Asia/Kamchatka'],'Africa/Johannesburg':['Asia/Gaza','Africa/Cairo'],'Asia/Baghdad':['Europe/Minsk']},r=e,i=function(){var e=n[r],i=e.length,s=0,o=e[0];for(;s<i;s+=1){o=e[s];if(t.date_is_dst(t.dst_start_for(o))){r=o;return}}},s=function(){return typeof n[r]!='undefined'};return s()&&i(),{name:function(){return r}}},t.olson={},t.olson.timezones={'-720,0':'Etc/GMT+12','-660,0':'Pacific/Pago_Pago','-600,1':'America/Adak','-600,0':'Pacific/Honolulu','-570,0':'Pacific/Marquesas','-540,0':'Pacific/Gambier','-540,1':'America/Anchorage','-480,1':'America/Los_Angeles','-480,0':'Pacific/Pitcairn','-420,0':'America/Phoenix','-420,1':'America/Denver','-360,0':'America/Guatemala','-360,1':'America/Chicago','-360,1,s':'Pacific/Easter','-300,0':'America/Bogota','-300,1':'America/New_York','-270,0':'America/Caracas','-240,1':'America/Halifax','-240,0':'America/Santo_Domingo','-240,1,s':'America/Santiago','-210,1':'America/St_Johns','-180,1':'America/Godthab','-180,0':'America/Argentina/Buenos_Aires','-180,1,s':'America/Montevideo','-120,0':'Etc/GMT+2','-120,1':'Etc/GMT+2','-60,1':'Atlantic/Azores','-60,0':'Atlantic/Cape_Verde','0,0':'Etc/UTC','0,1':'Europe/London','60,1':'Europe/Berlin','60,0':'Africa/Lagos','60,1,s':'Africa/Windhoek','120,1':'Asia/Beirut','120,0':'Africa/Johannesburg','180,0':'Asia/Baghdad','180,1':'Europe/Moscow','210,1':'Asia/Tehran','240,0':'Asia/Dubai','240,1':'Asia/Baku','270,0':'Asia/Kabul','300,1':'Asia/Yekaterinburg','300,0':'Asia/Karachi','330,0':'Asia/Kolkata','345,0':'Asia/Kathmandu','360,0':'Asia/Dhaka','360,1':'Asia/Omsk','390,0':'Asia/Rangoon','420,1':'Asia/Krasnoyarsk','420,0':'Asia/Jakarta','480,0':'Asia/Shanghai','480,1':'Asia/Irkutsk','525,0':'Australia/Eucla','525,1,s':'Australia/Eucla','540,1':'Asia/Yakutsk','540,0':'Asia/Tokyo','570,0':'Australia/Darwin','570,1,s':'Australia/Adelaide','600,0':'Australia/Brisbane','600,1':'Asia/Vladivostok','600,1,s':'Australia/Sydney','630,1,s':'Australia/Lord_Howe','660,1':'Asia/Kamchatka','660,0':'Pacific/Noumea','690,0':'Pacific/Norfolk','720,1,s':'Pacific/Auckland','720,0':'Pacific/Tarawa','765,1,s':'Pacific/Chatham','780,0':'Pacific/Tongatapu','780,1,s':'Pacific/Apia','840,0':'Pacific/Kiritimati'},typeof exports!='undefined'?exports.jstz=t:e.jstz=t})(this);</script><script>var timezone=jstz.determine();console.log('Your Timezone is:' + timezone.name());document.getElementById('timezone').innerHTML = timezone.name();</script>";
  #endif

#else
  const char WM_HTTP_SCRIPT_NTP_MSG[]     PROGMEM   = "";
  const char WM_HTTP_SCRIPT_NTP_HIDDEN[]  PROGMEM   = "";
  const char WM_HTTP_SCRIPT_NTP[]         PROGMEM   = "";
#endif

#if ( (_WIFIMGR_LOGLEVEL_ > 3) && DISPLAY_STORED_CREDENTIALS_IN_CP )
  #warning Enable DISPLAY_STORED_CREDENTIALS_IN_CP
#endif

#if DISPLAY_STORED_CREDENTIALS_IN_CP
  const char WM_HTTP_WIFIFORM_START[] PROGMEM = "<form method='get' action='wifisave'><fieldset>"
    "<div><label for='s'>SSID</label><input value='[[ssid]]' id='s' name='s' length=32 placeholder='SSID'></div>"
    "<div><label for='p'>Password</label><input value='[[pwd]]' id='p' name='p' length=64 placeholder='password'></div>"
    "<div><label for='s1'>SSID1</label><input value='[[ssid1]]' id='s1' name='s1' length=32 placeholder='SSID1'></div>"
    "<div><label for='s2'>Password</label><input value='[[pwd1]]' id='p1' name='p1' length=64 placeholder='password1'></div>"
    "</fieldset>";
#else
  const char WM_HTTP_WIFIFORM_START[] PROGMEM = "<form method='get' action='wifisave'><fieldset><div><label>SSID</label><input id='s' name='s' length=32 placeholder='SSID'><div></div></div><div><label>Password</label><input id='p' name='p' length=64 placeholder='password'><div></div></div><div><label>SSID1</label><input id='s1' name='s1' length=32 placeholder='SSID1'><div></div></div><div><label>Password</label><input id='p1' name='p1' length=64 placeholder='password1'><div></div></div></fieldset>";
#endif

const char WM_HTTP_WIFIFORM_LABEL_BEFORE[]  PROGMEM = "<div><label for='{i}'>{p}</label><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}></div>";
const char WM_HTTP_WIFIFORM_LABEL_AFTER[]   PROGMEM = "<div><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}><label for='{i}'>{p}</label></div>";
//const char WM_HTTP_WIFIFORM_PARAM[]       PROGMEM = "<input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}>";

const char WM_HTTP_CONFIGFORM_LABEL[]       PROGMEM = "<label for='{i}'>{p}</label>";
const char WM_HTTP_CONFIGFORM_PARAM[]       PROGMEM = "<input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}>";
const char WM_HTTP_CONFIGFORM_BOOL_PARAM[]  PROGMEM = "<input type='checkbox' id='{i}' name='{n}' length='{l}' value='1' {c}>";
const char WM_HTTP_CONFIGFORM_SELECTION_PARAM[] PROGMEM = "<select id='{i}' name='{n}' length='{l}'>{o}</select>";
const char WM_HTTP_CONFIGFORM_SEL_OPTIONS[] PROGMEM =     "<option value=\"{o}\"{s}>{d}</option>";

const char WM_HTTP_FORM_END[]               PROGMEM = "<button class='btn' type='submit'>Save</button></form>";

const char WM_HTTP_WIFISAVED[]    PROGMEM = "<div class='msg'><h3>Credentials Saved</h3><br>Attempting to connect to the {x}/{x1} network. Wait around 10 seconds then check <a href='/'>if it's OK.</a> <p/>The {v} AP will run on the same WiFi channel of the {x}/{x1} AP. You may have to manually reconnect to the {v} AP.</div>";
const char WM_HTTP_CONFIGSAVED[]  PROGMEM = "<div class='msg'><h3>Configuration Saved!</h3></div>";
const char WM_HTTP_NOTFOUND[]  PROGMEM = "<div class='msg'><h3>Request Not Found!</h3>{nf}</div>";

// Info html
// @todo remove html elements from progmem, repetatve strings
#ifdef ESP32
	const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp32</h3><hr><dl>";
  	//const char HTTP_INFO_lastreset[]  PROGMEM = "<td>Last reset reason</td><td>CPU0: {1}<br/>CPU1: {2}</td>";
  	const char HTTP_INFO_aphost[]     PROGMEM = "<td>Access Point Hostname</td><td>{1}</td>";
    const char HTTP_INFO_psrsize[]    PROGMEM = "<td>PSRAM Size</td><td>{1} bytes</td>";
	const char HTTP_INFO_temp[]       PROGMEM = "<td>Temperature</td><td>{1} C&deg; / {2} F&deg;</td></tr><tr><td>Hall</td><td>{3}</td>";
#else
	const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp8266</h3><hr>";
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
const char HTTP_INFO_uptime[]     PROGMEM = "<td>Uptime</td><td>{1}h {2}m {3}s</td>";
const char HTTP_INFO_chipid[]     PROGMEM = "<td>Chip ID</td><td>{1}</td>";
const char HTTP_INFO_chipOUI[]	  PROGMEM = "<td>Chip OUI</td><td>{1}</td>";
const char HTTP_INFO_chipmodel[]  PROGMEM = "<td>Chip Model</td><td>{1} Rev {2}</td>";
const char HTTP_INFO_fchipid[]    PROGMEM = "<td>Flash Chip ID</td><td>{1}</td>";
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
const char HTTP_INFO_aboutarduino[] PROGMEM = "<td>Arduino</td><td>{1}</td>";
const char HTTP_INFO_aboutsdk[]     PROGMEM = "<td>ESP-SDK/IDF</td><td>{1}</td>";
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

const char favIconHeader[] PROGMEM = "HTTP/1.1 200 OK\nContent-Type: image/ico\nConnection: close\n\n";
// Base64 version:
//const char favicon[]       PROGMEM = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAtdJREFUOE+lk1toVGcUhb99zsxkGs8MU2ZGTag1TUJuEDGJomPAggZiqDrFS6sQ9aWWgiJSlT60eTIv0odCGWjM5SEXaUNKwYJaNYX6UGNIYlBJSdJqItYQw4hmZnKZTM755UzaqrR56n762Zu99r8WawmAUdNRIpYZAUKIuFHKbv+7RECpeaBHafrxxJXa38SoaisWXfWK0/CoxVlQ1n/uWkphWQrRNBwuA5VKxJUpm8Rb3Xodp1GlUollrkJ8JgXJRXDqsGiCruH1ByAV7xZPdeucQtzKsrCvJBdMdF3D7dJBIP5snty1Ps5+VEHlulXcfxzj664hrvVNoOkyL94d7cqyTBKxZBr57SyD6HSS2cQCNuXy4iD9ze+nf3dvNEppQYDniQWywhewEETb2qLKCvwc2J5LeYGfbeXZaa57Pu/m4uVRxq4eIWe1Qd4HnTz4/Sn+t7wEfW7GJuK4nDrClkZVUbKS/qYwc0mTusgtasNFrM/3U3PmKle+rObTSC9fNfThzjLSGjscgkPXlkSlskltKAnS1xhmb93P/NDYj6csi9hPh2m4OMwn4aJ/AHjDwccflnL+dCX5B7t4MBFbAqiweTaFqWsZoP7sDTbvyKenYTeH6m9wYl8JG4uCvLP/O8YHJjh2IkTkZIii2u8ZeTi9BFBWGOB2y5JQI+PPKczxpYUK1LRRmPMmQ+1707O7I1HWFQbS75z9nTx8kviLQnGQvqYw33bfJzfbw6OpGU5Fenk8NYOZMslb66P+6Aa2lK7ijz+nOddxh18GJ3G5bBErm1WodCU3v9nFzs+ucenHYfBmoLkdGJkuQBF7zUgWaILHm4GIII53W+by1/jcDadCfNE8wK+Dk2SucKJp8pozbcVNpdBE0O2ZaLbtbSO1XTe1zKqZaJQMw0WGS182S68iitOAVKJbjPfaimVR9Vr6Co+YswjLJPHvbdEQR+bLMP3fOL8AMWcuzi8HP64AAAAASUVORK5CYII=";

const uint8_t favicon[] PROGMEM = {
  0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 
0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 
0x61, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00, 
0x02, 0xD7, 0x49, 0x44, 0x41, 0x54, 0x38, 0x4F, 0xA5, 0x93, 0x5B, 0x68, 0x54, 0x67, 0x14, 0x85, 
0xBF, 0x7D, 0xCE, 0xCC, 0x64, 0x1A, 0xCF, 0x0C, 0x53, 0x66, 0x46, 0x4D, 0xA8, 0x35, 0x4D, 0x42, 
0x6E, 0x10, 0x31, 0x89, 0xA2, 0x63, 0xC0, 0x82, 0x06, 0x62, 0xA8, 0x3A, 0xC5, 0x4B, 0xAB, 0x10, 
0xF5, 0xA5, 0x96, 0x82, 0x22, 0x52, 0x95, 0x3E, 0xB4, 0x79, 0x32, 0x2F, 0xD2, 0x87, 0x42, 0x19, 
0x68, 0xCC, 0xE5, 0x21, 0x17, 0x69, 0x43, 0x4A, 0xC1, 0x82, 0x5A, 0x35, 0x85, 0xFA, 0x50, 0x63, 
0x48, 0x62, 0x50, 0x49, 0x49, 0xD2, 0x6A, 0x22, 0xD6, 0x10, 0xC3, 0x88, 0x66, 0x66, 0x72, 0x99, 
0x4C, 0xCE, 0xF9, 0xE5, 0x4C, 0xDA, 0xAA, 0xB4, 0x79, 0xEA, 0x7E, 0xFA, 0xD9, 0x9B, 0xBD, 0xF6, 
0xBF, 0x16, 0x6B, 0x09, 0x80, 0x51, 0xD3, 0x51, 0x22, 0x96, 0x19, 0x01, 0x42, 0x88, 0xB8, 0x51, 
0xCA, 0x6E, 0xFF, 0xBB, 0x44, 0x40, 0xA9, 0x79, 0xA0, 0x47, 0x69, 0xFA, 0xF1, 0xC4, 0x95, 0xDA, 
0xDF, 0xC4, 0xA8, 0x6A, 0x2B, 0x16, 0x5D, 0xF5, 0x8A, 0xD3, 0xF0, 0xA8, 0xC5, 0x59, 0x50, 0xD6, 
0x7F, 0xEE, 0x5A, 0x4A, 0x61, 0x59, 0x0A, 0xD1, 0x34, 0x1C, 0x2E, 0x03, 0x95, 0x4A, 0xC4, 0x95, 
0x29, 0x9B, 0xC4, 0x5B, 0xDD, 0x7A, 0x1D, 0xA7, 0x51, 0xA5, 0x52, 0x89, 0x65, 0xAE, 0x42, 0x7C, 
0x26, 0x05, 0xC9, 0x45, 0x70, 0xEA, 0xB0, 0x68, 0x82, 0xAE, 0xE1, 0xF5, 0x07, 0x20, 0x15, 0xEF, 
0x16, 0x4F, 0x75, 0xEB, 0x9C, 0x42, 0xDC, 0xCA, 0xB2, 0xB0, 0xAF, 0x24, 0x17, 0x4C, 0x74, 0x5D, 
0xC3, 0xED, 0xD2, 0x41, 0x20, 0xFE, 0x6C, 0x9E, 0xDC, 0xB5, 0x3E, 0xCE, 0x7E, 0x54, 0x41, 0xE5, 
0xBA, 0x55, 0xDC, 0x7F, 0x1C, 0xE3, 0xEB, 0xAE, 0x21, 0xAE, 0xF5, 0x4D, 0xA0, 0xE9, 0x32, 0x2F, 
0xDE, 0x1D, 0xED, 0xCA, 0xB2, 0x4C, 0x12, 0xB1, 0x64, 0x1A, 0xF9, 0xED, 0x2C, 0x83, 0xE8, 0x74, 
0x92, 0xD9, 0xC4, 0x02, 0x36, 0xE5, 0xF2, 0xE2, 0x20, 0xFD, 0xCD, 0xEF, 0xA7, 0x7F, 0x77, 0x6F, 
0x34, 0x4A, 0x69, 0x41, 0x80, 0xE7, 0x89, 0x05, 0xB2, 0xC2, 0x17, 0xB0, 0x10, 0x44, 0xDB, 0xDA, 
0xA2, 0xCA, 0x0A, 0xFC, 0x1C, 0xD8, 0x9E, 0x4B, 0x79, 0x81, 0x9F, 0x6D, 0xE5, 0xD9, 0x69, 0xAE, 
0x7B, 0x3E, 0xEF, 0xE6, 0xE2, 0xE5, 0x51, 0xC6, 0xAE, 0x1E, 0x21, 0x67, 0xB5, 0x41, 0xDE, 0x07, 
0x9D, 0x3C, 0xF8, 0xFD, 0x29, 0xFE, 0xB7, 0xBC, 0x04, 0x7D, 0x6E, 0xC6, 0x26, 0xE2, 0xB8, 0x9C, 
0x3A, 0xC2, 0x96, 0x46, 0x55, 0x51, 0xB2, 0x92, 0xFE, 0xA6, 0x30, 0x73, 0x49, 0x93, 0xBA, 0xC8, 
0x2D, 0x6A, 0xC3, 0x45, 0xAC, 0xCF, 0xF7, 0x53, 0x73, 0xE6, 0x2A, 0x57, 0xBE, 0xAC, 0xE6, 0xD3, 
0x48, 0x2F, 0x5F, 0x35, 0xF4, 0xE1, 0xCE, 0x32, 0xD2, 0x1A, 0x3B, 0x1C, 0x82, 0x43, 0xD7, 0x96, 
0x44, 0xA5, 0xB2, 0x49, 0x6D, 0x28, 0x09, 0xD2, 0xD7, 0x18, 0x66, 0x6F, 0xDD, 0xCF, 0xFC, 0xD0, 
0xD8, 0x8F, 0xA7, 0x2C, 0x8B, 0xD8, 0x4F, 0x87, 0x69, 0xB8, 0x38, 0xCC, 0x27, 0xE1, 0xA2, 0x7F, 
0x00, 0x78, 0xC3, 0xC1, 0xC7, 0x1F, 0x96, 0x72, 0xFE, 0x74, 0x25, 0xF9, 0x07, 0xBB, 0x78, 0x30, 
0x11, 0x5B, 0x02, 0xA8, 0xB0, 0x79, 0x36, 0x85, 0xA9, 0x6B, 0x19, 0xA0, 0xFE, 0xEC, 0x0D, 0x36, 
0xEF, 0xC8, 0xA7, 0xA7, 0x61, 0x37, 0x87, 0xEA, 0x6F, 0x70, 0x62, 0x5F, 0x09, 0x1B, 0x8B, 0x82, 
0xBC, 0xB3, 0xFF, 0x3B, 0xC6, 0x07, 0x26, 0x38, 0x76, 0x22, 0x44, 0xE4, 0x64, 0x88, 0xA2, 0xDA, 
0xEF, 0x19, 0x79, 0x38, 0xBD, 0x04, 0x50, 0x56, 0x18, 0xE0, 0x76, 0xCB, 0x92, 0x50, 0x23, 0xE3, 
0xCF, 0x29, 0xCC, 0xF1, 0xA5, 0x85, 0x0A, 0xD4, 0xB4, 0x51, 0x98, 0xF3, 0x26, 0x43, 0xED, 0x7B, 
0xD3, 0xB3, 0xBB, 0x23, 0x51, 0xD6, 0x15, 0x06, 0xD2, 0xEF, 0x9C, 0xFD, 0x9D, 0x3C, 0x7C, 0x92, 
0xF8, 0x8B, 0x42, 0x71, 0x90, 0xBE, 0xA6, 0x30, 0xDF, 0x76, 0xDF, 0x27, 0x37, 0xDB, 0xC3, 0xA3, 
0xA9, 0x19, 0x4E, 0x45, 0x7A, 0x79, 0x3C, 0x35, 0x83, 0x99, 0x32, 0xC9, 0x5B, 0xEB, 0xA3, 0xFE, 
0xE8, 0x06, 0xB6, 0x94, 0xAE, 0xE2, 0x8F, 0x3F, 0xA7, 0x39, 0xD7, 0x71, 0x87, 0x5F, 0x06, 0x27, 
0x71, 0xB9, 0x6C, 0x11, 0x2B, 0x9B, 0x55, 0xA8, 0x74, 0x25, 0x37, 0xBF, 0xD9, 0xC5, 0xCE, 0xCF, 
0xAE, 0x71, 0xE9, 0xC7, 0x61, 0xF0, 0x66, 0xA0, 0xB9, 0x1D, 0x18, 0x99, 0x2E, 0x40, 0x11, 0x7B, 
0xCD, 0x48, 0x16, 0x68, 0x82, 0xC7, 0x9B, 0x81, 0x88, 0x20, 0x8E, 0x77, 0x5B, 0xE6, 0xF2, 0xD7, 
0xF8, 0xDC, 0x0D, 0xA7, 0x42, 0x7C, 0xD1, 0x3C, 0xC0, 0xAF, 0x83, 0x93, 0x64, 0xAE, 0x70, 0xA2, 
0x69, 0xF2, 0x9A, 0x33, 0x6D, 0xC5, 0x4D, 0xA5, 0xD0, 0x44, 0xD0, 0xED, 0x99, 0x68, 0xB6, 0xED, 
0x6D, 0x23, 0xB5, 0x5D, 0x37, 0xB5, 0xCC, 0xAA, 0x99, 0x68, 0x94, 0x0C, 0xC3, 0x45, 0x86, 0x4B, 
0x5F, 0x36, 0x4B, 0xAF, 0x22, 0x8A, 0xD3, 0x80, 0x54, 0xA2, 0x5B, 0x8C, 0xF7, 0xDA, 0x8A, 0x65, 
0x51, 0xF5, 0x5A, 0xFA, 0x0A, 0x8F, 0x98, 0xB3, 0x08, 0xCB, 0x24, 0xF1, 0xEF, 0x6D, 0xD1, 0x10, 
0x47, 0xE6, 0xCB, 0x30, 0xFD, 0xDF, 0x38, 0xBF, 0x00, 0x31, 0x67, 0x2E, 0xCE, 0x2F, 0x07, 0x3F, 
0xAE, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

#endif