'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// SQL modules
const mysql = require('mysql');

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

let sharedLib = ffi.Library( './sharedLib', {
    //return type first, argument list second
    //for void input type, leave argumrnt list empty
    'GEDCOMtoJSON': [ 'string', [ 'string' ] ],
    'JSONtoGEDCOMWrap': [ 'int', [ 'string', 'string' ] ],
    'addPersonWrap': ['int', [ 'string', 'string' ] ],
    'indivListToJSON': [ 'string', [ 'string' ] ],
    'getDescendantsWrap': [ 'string', [ 'string', 'string', 'int' ] ],
    'getAncestorsWrap': [ 'string', [ 'string', 'string', 'int' ] ]
});

app.get('/getFiles', function( req , res ) {
    var fileList = fs.readdirSync( './uploads/' );
    res.send({
        file: fileList
    });
});

app.get('/fileInfo', function( req, res ) {
    var t = sharedLib.GEDCOMtoJSON( req.query.file );
    res.send( t );
});

app.get('/create', function( req, res ) {
    var ret = {
        'json': req.query.json,
        'file': req.query.file,
        error: "N/A"
    };
    
    var fileList = fs.readdirSync( './uploads/' );
    for (var i in fileList) {
        if (fileList[i] == req.query.file) {
            ret.error = fileList[i] + " already exists on the server.";
            return res.send(ret);
        }
    }

    var obj = sharedLib.JSONtoGEDCOMWrap( req.query.json, req.query.file );

    if (obj < 0) {
        ret.error = "Could not create <" + req.query.file + "> in uploads/";
    } else {
        ret.error = "Successfully created <" + req.query.file + "> in uploads/";
    }
    res.send( ret );
});

app.get( '/addPerson', function( req, res ) {
    var str = JSON.stringify( req.query.json );
    var obj = sharedLib.addPersonWrap( str, req.query.file );
    
    var ret;
    if (obj == -1) {
        ret = "Unable to parse file to add individual.";
    } else if (obj == -2) {
        ret = "Unable to create the individual.";
    } else if (obj == -3) {
        ret = "Person already exists in given file.";
    } else if (obj == -4) {
        ret = "Unable to validate object.";
    } else if (obj == -5) {
        ret = "Unable to write the file.";
    } else {
        ret = "Successfully added " + req.query.json.givenName + " " + req.query.json.surname + " to <" + req.query.file + ">";
    }
    
    res.send({
      'data': ret
    });
});

app.get( '/changeView', function( req, res ) {
    var obj = sharedLib.indivListToJSON( req.query.file );
    res.send( obj );
});

app.get( '/getDescend', function( req, res ) {
	var json = JSON.stringify( req.query.ind );
	
	var descendList = sharedLib.getDescendantsWrap( req.query.file, json, req.query.gens );
	
	var json = JSON.parse( descendList );
	
	res.send( json );
});

app.get( '/getAncestors', function( req, res ) {
	var json = JSON.stringify( req.query.ind );
	
	var genList = sharedLib.getAncestorsWrap( req.query.file, json, req.query.gens );
	
	var json = JSON.parse( genList );
	res.send( json );
});

app.get( '/credentials', function( req, res ) {
  const connection = mysql.createConnection({
      'host'     : 'dursley.socs.uoguelph.ca',
      'user'     : req.query.uname,
      'password' : req.query.pword,
      'database' : req.query.uname
  });

  connection.connect();
  res.send( req.query );
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
