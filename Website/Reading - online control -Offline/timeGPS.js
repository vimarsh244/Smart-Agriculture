//not actually required.
var x = document.getElementById("gpsShow");

function getLocation() {
  if (navigator.geolocation) {
    navigator.geolocation.watchPosition(showPosition);
  } else { 
    x.innerHTML = "Geolocation is not supported by this browser.";
  }
}
    
function showPosition(position) {
    var strT='Latitude: ' + position.coords.latitude + 
    '<br>Longitude: ' + position.coords.longitude;
	document.getElementById("gpsShow").innerHTML = strT;
}

