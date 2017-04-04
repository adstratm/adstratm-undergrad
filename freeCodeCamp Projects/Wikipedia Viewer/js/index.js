$(document).ready(function () {
  // what to do in the event of a button press
  $('#subbut').click(function (e) {
    e.preventDefault()
    var query = $('#quer').val()
    // alert(query);
    var jsonq = 'https://en.wikipedia.org/w/api.php?action=opensearch&search='
    jsonq += query
    jsonq += '&limit=10&namespace=0&format=json&origin=*'

    $.getJSON(jsonq, function (result) {
      // $("#results").html(JSON.stringify(result));

      // first get the data from the query
      var numOfResults = result[1].length

      var qtitles = []
      for (var t = 0; t < 10; t++) {
        qtitles[t] = result[1][t]
      }

      var qdescrips = []
      for (var u = 0; u < 10; u++) {
        qdescrips[u] = result[2][u]
      }

      var qlinks = []
      for (var v = 0; v < 10; v++) {
        qlinks[v] = result[3][v]
      }

      // first clear all of the results

      var titles = ['#firstt', '#secondt', '#thirdt', '#fourtht', '#fiftht', '#sixtht', '#seventht', '#eightht', '#nintht', '#tentht']

      var descrips = ['#firstdesc', '#seconddesc', '#thirddesc', '#fourthdesc', '#fifthdesc', '#sixthdesc', '#seventhdesc', '#eighthdesc', '#ninthdesc', '#tenthdesc']

      var links = ['#firstlink', '#secondlink', '#thirdlink', '#fourthlink', '#fifthlink', '#sixthlink', '#seventhlink', '#eighthlink', '#ninthlink', '#tenthlink']

      for (var i = 0; i < descrips.length; i++) {
        $(titles[i], descrips[i]).html('')
        $(links[i]).attr('href', '')
      }

      for (var j = 0; j < numOfResults; j++) {
        $(titles[j]).html(qtitles[j])
        $(descrips[j]).html(qdescrips[j] + '<hr>')
        $(links[j]).attr('href', qlinks[j])
      }
    })
  })
})
