$(document).ready(function(){
  var temp = 0;
  var tempState = "c";
  //get the current location
  $.getJSON("http://freegeoip.net/json/", function(result){
    //$("#loctext").html(JSON.stringify(result));
    var country = result.country_code;
    var region = result.region_code;
    var city = result.city;
    var zip = result.zip_code;
    var lat = result.latitude;
    var long = result.longitude;
    $("#loctext").html(city + ", " + region + ", " + country);
    
    //now get the weather data
    var apiCall = "http://api.openweathermap.org/data/2.5/weather?lat=";
    apiCall += lat;
    apiCall += "&lon=";
    apiCall += long;
    apiCall += "&appid=45f11ff479090e1bcf42af66ce28b0c9";
    $.getJSON(apiCall, function(weather){
      var main = weather.weather[0].main;
      temp = weather.main.temp;
      temp = Math.round(temp);
      temp -= 270;
      var icon = weather.weather[0].icon;
      var iconLink = "http://openweathermap.org/img/w/";
      iconLink += icon;
      iconLink += ".png";
      //$("#weathtext").html(JSON.stringify(weather));
      $("#temptext").html(temp + "&#176" + " C");
      $("#condtext").html(main);
      $("#condimg").html("<img src='" + iconLink + "'/>");
    });
  });
  
  $("#tempbutton").click(function(){
    if(tempState == "c"){
      temp = Math.round(changeToF(temp));
      tempState = "f";
      $("#tempbutton").html("Change to Celsius");
      $("#temptext").html(temp + "&#176" + " F");
    }
    else{
      temp = Math.round(changeToC(temp));
      tempState = "c";
      $("#tempbutton").html("Change to Fahrenheit");
      $("#temptext").html(temp + "&#176" + " C");
    }
  });
});

function changeToF(cVal)
{
  var out = cVal * 9;
  out /= 5;
  out += 32;
  return out;
}

function changeToC(fVal)
{
  var out = fVal - 32;
  out *= 5;
  out /= 9;
  return out;
}