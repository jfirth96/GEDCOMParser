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

// Set up functions from my library
// We create a new object called sharedLib and the C functions become its methods
let sharedLib = ffi.Library( 'sharedLib', {
    //return type first, argument list second
    //for void input type, leave argumrnt list empty
    'GEDCOMtoJSON': [ 'string', [ 'string' ] ],
    'JSONtoGEDCOMWrap': [ 'int', [ 'string', 'string' ] ],
    'addPersonWrap': ['int', [ 'string', 'string' ] ],
    'indivListToJSON': [ 'string', [ 'string' ] ]
});

// call functions with sharedLib.functionName()

//Sample endpoint
app.get('/getFiles', function( req , res ) {
    var fileList = fs.readdirSync( './uploads/' );
    res.send({
        file: fileList
    });
});

app.get('/fileInfo', function( req, res ) {
    //var p = JSON.parse( req.query );
    //console.log( req.query );

    var t = sharedLib.GEDCOMtoJSON( req.query.file );
    //console.log( req.query.file );
    //console.log( t );
    res.send( t );
});

app.get('/create', function( req, res ) {
    console.log( req.query );

    var ret = {
        'json': req.query.json,
        'file': req.query.file,
        error: "N/A"
    };
    //console.log( ret );
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
    //console.log( req.query );

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
    console.log( ret );
    res.send({
      'data': ret
    });
});

app.get( '/changeView', function( req, res ) {
    //console.log( req.query );

    var obj = sharedLib.indivListToJSON( req.query.file );
    console.log( obj );
    res.send( obj );
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);