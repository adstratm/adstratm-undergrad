$(document).ready(function(){
  
  $.getJSON("http://api.forismatic.com/api/1.0/?method=getQuote&457653&format=jsonp&lang=en&jsonp=?", function(result){
      //$("#qpara").html(JSON.stringify(result));
      var body = result.quoteText;
      var author = result.quoteAuthor;
      if(!author)
      {
        author = "Anonymous";
      }
      
      $("#qpara").html(body);
      $("#qauth").html("- " + author);
      $("#tlink").attr('href', 'https://twitter.com/intent/tweet?hashtags=quotes&text=' + body + "-" + author);
      
    });
  
  $("#nquotbutton").click(function() {
    $.getJSON("http://api.forismatic.com/api/1.0/?method=getQuote&457653&format=jsonp&lang=en&jsonp=?", function(result){
      //$("#qpara").html(JSON.stringify(result));
      var body = result.quoteText;
      var author = result.quoteAuthor;
      if(!author)
      {
        author = "Anonymous";
      }
      
      $("#qpara").html(body);
      $("#qauth").html("- " + author);
      $("#tlink").attr('href', 'https://twitter.com/intent/tweet?hashtags=quotes&text=' + body + "-" + author);
    });
  });
});