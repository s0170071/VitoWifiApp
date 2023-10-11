 #ifndef WEBPAGEH
 #define WEBPAGEH
 
/*
 * This is a small probing page. It allow you to enter a DP and get the response decoded for all possible data point types. 
 * Also, it allows you to quickly step forward to the next address. Handy for searching data points. 
 * NOTE: adjust the Address below 192.168.1.111 to whatever IP your heater has.
 * 
*/

const char probingwebpage[] PROGMEM = R"rawliteral( 
  
<html><script type="text/javascript" id="__gaOptOutExtension">window["_gaUserPrefs"] = { ioo : function() { return true; } }</script><head><script type="text/javascript">

      function incAddress(increment=1) {
         var value = parseInt(document.getElementById('address').value, 16);
         value = isNaN(value) ? 0 : value+increment;
         var valuestring = "0x"+ value.toString(16);
         var loc = "http://192.168.1.111/read?DP=" + valuestring + "&Type=all&ReadLen=4";
         document.getElementById('VitoDataIframe').src = loc;
         document.getElementById('address').value = valuestring;
      }

      function parseIframeValues(nameofiframe) {
         var allValues = document.getElementById(nameofiframe).contentWindow.document.body.innerHTML;
           document.getElementById('writeTempL').value = '-' ;
           document.getElementById('writeTempS').value = '-' ;
           document.getElementById('writeSTAT').value  = '-' ;
           document.getElementById('writeCountL').value  = '-' ;
           document.getElementById('writeCountS').value  = '-' ;
           document.getElementById('writeMode').value = '-' ;
           document.getElementById('writeHours').value  = '-' ;
           document.getElementById('writeCoP').value  = '-' ;

           document.getElementById('writeTempL').value = getValue('TempL', 'float', allValues)  ;
           document.getElementById('writeTempS').value = getValue('TempS', 'integer', allValues)  ;
           document.getElementById('writeSTAT').value = getValue('STAT', 'STAT', allValues)  ;
           document.getElementById('writeCountL').value = getValue('CountL', 'integer', allValues)  ;
           document.getElementById('writeCountS').value = getValue('CountS', 'integer', allValues)  ;
           document.getElementById('writeMode').value = getValue('Mode', 'integer', allValues)  ;
           document.getElementById('writeHours').value = getValue('Hours', 'float', allValues)  ;
           document.getElementById('writeCoP').value = getValue('CoP', 'float', allValues)  ;
           document.getElementById('VitoResultIframe').contentWindow.document.body.innerHTML='';
      }

      function getValue(Name="", Type="float", content="") {
            var rx = '';
            if ( Type == 'float')   {rx = Name+':\\s+([-+]?[0-9]+\\.[0-9]+)';}
            if ( Type == 'integer') {rx = Name+':\\s+([-+]?[0-9]+)';}            
            if ( Type == 'STAT') {rx = Name+':\\s+([a-z]+)';}
            var arr = content.match(rx);  
            if (arr[1] == 'false') {arr[1]='0'};
            if (arr[1] == 'true') {arr[1]='1'};
            return arr[1]; 
            }

      function btnWriteValue(Mode) {
         var addr = document.getElementById('address').value;
         var value = document.getElementById('write'+Mode).value;
         var LinkString = "http://192.168.1.111/write?DP="+addr+"&Type="+Mode+"&Value="+value; 
         console.log(LinkString)
         
         document.getElementById('VitoResultIframe').src = LinkString;

      }
      function striptext() {
         
         var content =  document.getElementById('VitoResultIframe').contentWindow.document.body.innerHTML;
         var arr = content.match('write\\s+([a-z]+)');  
         document.getElementById('VitoResultIframe').contentWindow.document.body.innerHTML= arr[1] ;
      }
   </script>


</head><body><div id="duden-mentor_root"><div id="mentor-button-container" style="position: absolute; top: 0px; left: 0px; display: block;"><div id="mentor-overlay" style="position: absolute; pointer-events: none;"><div style="position: absolute; right: 10px; bottom: 10px; z-index: 9999; pointer-events: all; cursor: pointer;"><img alt="" src="moz-extension://9e0bcae1-84ef-4ccf-9c21-1db0843a500b/assets/icons/mentor.svg" width="24"></div></div></div><div class="overlay-background" style="display: none;"><div class="overlay-container"><div class="overlay-menu"><button class="knob knob--small" disabled="">Änderungen übernehmen</button><button class="knob knob--small knob--primary">Abbrechen</button></div><iframe src="moz-extension://9e0bcae1-84ef-4ccf-9c21-1db0843a500b/page/iFrame.html?parent=http://192.168.1.111" style="border: medium none; flex: 1 1 0%;"></iframe></div></div></div>
   <h1>Vito WO1B data read</h1>
   <form>
      <input type="button" id="myBtn" onclick="incAddress(-1)" value="<">
     <input type="text" id="address" value="0x0100" onload="incAddress(0)/">
      <input type="button" id="myBtn" onclick="incAddress(1)" value=">">
   </form>

   <iframe id="VitoDataIframe" src="http://192.168.1.111/read?DP=0x0100&amp;Type=all&amp;ReadLen=4" onload="parseIframeValues('VitoDataIframe')" hidden=""> </iframe>
   <br>
   <a src="Github" href="https://github.com/openv/openv/issues/420">Github Datapoints </a>
   <br>
   <a src="Github" href="http://192.168.1.30:8080/basicui/app?sitemap=Heizung ">OH Sitemap</a>

   <script type="text/javascript">
      var input = document.getElementById('address')// If the user presses the "Enter" key on the keyboard
      input.addEventListener("keypress", function(event) {
            if (event.key === "Enter") { event.preventDefault();   incAddress(0);  }
         } ); 
   </script>

   <h1>Vito WO1B data write</h1>
   <table id="table">
   <tbody><tr><td>TempL</td>   <td><input type="text" id="writeTempL" value="0x0100"> </td>  <td> <input type="button" id="myBtnWriteTempL" onclick="btnWriteValue ('TempL')" value="Write!">   </td></tr>
   <tr><td>TempS</td>   <td><input type="text" id="writeTempS" value="0x0100"> </td>  <td> <input type="button" id="myBtnWriteTempS" onclick="btnWriteValue ('TempS')" value="Write!">   </td></tr>
   <tr><td>STAT</td>    <td><input type="text" id="writeSTAT" value="0x0100"> </td>   <td> <input type="button" id="myBtnWriteSTAT" onclick="btnWriteValue ('STAT')" value="Write!">    </td></tr>
   <tr><td>CountL</td>  <td><input type="text" id="writeCountL" value="0x0100"> </td> <td> <input type="button" id="myBtnWriteCountL" onclick="btnWriteValue ('CountL')" value="Write!">  </td></tr>
   <tr><td>CountS</td>  <td><input type="text" id="writeCountS" value="0x0100"> </td> <td> <input type="button" id="myBtnWriteCountS" onclick="btnWriteValue ('CountS')" value="Write!">  </td></tr>
   <tr><td>Mode</td>    <td><input type="text" id="writeMode" value="0x0100"> </td>   <td> <input type="button" id="myBtnWriteMode" onclick="btnWriteValue ('Mode')" value="Write!">    </td></tr>
   <tr><td>Hours</td>   <td><input type="text" id="writeHours" value="0x0100"> </td>  <td> <input type="button" id="myBtnWriteHours" onclick="btnWriteValue ('Hours')" value="Write!">   </td></tr>
   <tr><td>CoP</td>     <td><input type="text" id="writeCoP" value="0x0100"> </td>    <td> <input type="button" id="myBtnWriteCoP" onclick="btnWriteValue ('CoP')" value="Write!">      </td></tr>
   </tbody></table>
   <br><br><br>
   <iframe id="VitoResultIframe" onload="striptext()">  </iframe>
 

</body></html>
  
)rawliteral";
  
#endif
