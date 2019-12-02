var express = require("express");
var app = express();

var pathtofrontend = '/lfjs/'

/* serves main page */
app.get("/", function(req, res) {

  res.sendFile(__dirname+ pathtofrontend + 'index.html')
});

app.post("/user/add", function(req, res) { 
  /* some server side logic */
  res.send("OK");
});

/* serves all the static files */
app.get(/^(.+)$/, function(req, res){ 
  console.log('static file request : ' + req.params[0]);
  res.sendFile( __dirname +  pathtofrontend + req.params[0]); 
});

var port = process.env.PORT || 5000;
app.listen(port, function() {
  console.log("Listening on " + port);
});
