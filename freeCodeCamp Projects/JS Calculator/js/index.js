$(document).ready(function(){
  var startVal = ""; //nothing there
  var curString = startVal;
  $("#dispval").html("0");
  
  $("#acbut").click(function(e){
    e.preventDefault();
    curString = startVal;
    $("#dispval").html("0");
  });
  
  $("#cebut").click(function(e){
    e.preventDefault();
    curString = curString.slice(0, -1);
    if(curString.length > 0){
      $("#dispval").html(curString);
    }
    else{
      $("#dispval").html("0");
    }
  });
  
  $("#7but").click(function(e){
    e.preventDefault();
    curString += "7";
    $("#dispval").html(curString);
  });
  
  $("#8but").click(function(e){
    e.preventDefault();
    curString += "8";
    $("#dispval").html(curString);
  });
  
  $("#9but").click(function(e){
    e.preventDefault();
    curString += "9";
    $("#dispval").html(curString);
  });
  
  $("#divbut").click(function(e){
    e.preventDefault();
    if($.isNumeric(curString.slice(-1))){
      curString += "/";
    }
    $("#dispval").html(curString);
  });
  
  $("#mulbut").click(function(e){
    e.preventDefault();
    if($.isNumeric(curString.slice(-1))){
      curString += "*";
    }
    $("#dispval").html(curString);
  });
  
  $("#minbut").click(function(e){
    e.preventDefault();
    if($.isNumeric(curString.slice(-1))){
      curString += "-";
    }
    $("#dispval").html(curString);
  });
  
  $("#plusbut").click(function(e){
    e.preventDefault();
    if($.isNumeric(curString.slice(-1))){
      curString += "+";
    }
    $("#dispval").html(curString);
  });
  
  $("#4but").click(function(e){
    e.preventDefault();
    curString += "4";
    $("#dispval").html(curString);
  });
  
  $("#5but").click(function(e){
    e.preventDefault();
    curString += "5";
    $("#dispval").html(curString);
  });
  
  $("#6but").click(function(e){
    e.preventDefault();
    curString += "6";
    $("#dispval").html(curString);
  });
  
  $("#1but").click(function(e){
    e.preventDefault();
    curString += "1";
    $("#dispval").html(curString);
  });
  
  $("#2but").click(function(e){
    e.preventDefault();
    curString += "2";
    $("#dispval").html(curString);
  });
  
  $("#3but").click(function(e){
    e.preventDefault();
    curString += "3";
    $("#dispval").html(curString);
  });
  
  $("#0but").click(function(e){
    e.preventDefault();
    curString += "0";
    $("#dispval").html(curString);
  });
  
  $("#dotbut").click(function(e){
    e.preventDefault();
    curString += ".";
    $("#dispval").html(curString);
  });
  
  $("#eqbut").click(function(e){
    e.preventDefault();
    //curstring contains the input
    var i = 0;
    var args = [];
    var current = "";
    while(i < curString.length){
      if(curString[i] == '/' || curString[i] == '*' || curString[i] == '+' || curString[i] == '-'){
        //a symbol is found
        args.push(current);
        current = "";
        args.push(curString[i]);
      }
      else{
        current += curString[i];
      }
      
      if(i == curString.length - 1){
        args.push(current);
      }
      i++;
    }
    
    for(var i = 0; i < args.length; i++){
      if($.isNumeric(args[i])){
        if(args[i].indexOf('.') >= 0){
          args[i] = parseFloat(args[i]);
        }else{
          args[i] = parseInt(args[i]);
        }
      }
    }
    
    
    //work on error handling
    var value = args[0];
    var counter = 1;
    while(counter < args.length){
      if($.isNumeric(args[counter])){
        //error
        //the preceeding token was a num
        curString = "";
        $("#dispval").html("err");
      }
      else if(args[counter] == '*'){
        value = value * args[counter + 1];
        counter += 2;
      }
      else if(args[counter] == '/'){
        value = value / args[counter + 1];
        counter += 2;
      }
      else if(args[counter] == '-'){
        value = value - args[counter + 1];
        counter += 2;
      }
      else if(args[counter] == '+'){
        value = value + args[counter + 1];
        counter += 2;
      }
    }
    
    value = Math.ceil(100 * value) / 100;
    
    curString = value.toString();
    $("#dispval").html(curString);
    
    
  });
});