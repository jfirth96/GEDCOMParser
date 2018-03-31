'use strict'

// C library API
const ffi = require( 'ffi' );

// Express App (Routes)
const express = require( "express" );
const app     = express();
const path    = require( "path" );
const fileUpload = require( 'express-fileupload' );

app.use( fileUpload() );

// Minimization
const fs = require( 'fs' );
const JavaScriptObfuscator = require( 'javascript-obfuscator' );

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// SQL modules
const mysql = require( 'mysql' );
let connection;

// Send HTML at root, do not change
app.get( '/', function( req, res ) {
  res.sendFile( path.join( __dirname + '/public/index.html' ) );
});

// Send Style, do not change
app.get( '/style.css', function( req, res ) {
    //Feel free to change the contents of style.css to prettify your Web app
    res.sendFile( path.join( __dirname + '/public/style.css' ) );
});

// Send obfuscated JS, do not change
app.get( '/index.js', function( req, res ) {
    fs.readFile( path.join( __dirname + '/public/index.js' ), 'utf8', function( err, contents ) {
        const minimizedContents = JavaScriptObfuscator.obfuscate( contents, {compact: true, controlFlowFlattening: true} );
        res.contentType( 'application/javascript' );
        res.send( minimizedContents._obfuscatedCode );
    });
});

//Respond to POST requests that upload files to uploads/ directory
app.post( '/upload', function( req, res ) {
    if (!req.files) {
        return res.status( 400 ).send( 'No files were uploaded.' );
    }
 
    let uploadFile = req.files.uploadFile;
 
    // Use the mv() method to place the file somewhere on your server
    uploadFile.mv( 'uploads/' + uploadFile.name, function( err ) {
        if (err) {
            return res.status( 500 ).send( err );
        }
        res.redirect( '/' );
    });
});

//Respond to GET requests for files in the uploads/ directory
app.get( '/uploads/:name', function( req , res ) {
    fs.stat( 'uploads/' + req.params.name, function( err, stat ) {
        console.log( err );
        if (err == null) {
            res.sendFile( path.join( __dirname + '/uploads/' + req.params.name ) );
        } else {
            res.send( '' );
        }
    });
});

//******************** Your code goes here ********************************************************************************

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

app.get( '/file', function( req, res ) {
    //console.log( req.query );

    // Form connection once, declared as global variable
    connection = mysql.createConnection({
        'host'     : 'dursley.socs.uoguelph.ca',
        'user'     : req.query.uname,
        'password' : req.query.pword,
        'database' : req.query.dbase
    });

    connection.connect();
    let query = "CREATE TABLE IF NOT EXISTS FILE (file_id INT NOT NULL AUTO_INCREMENT," + 
        " file_Name VARCHAR(60) NOT NULL," + 
        " source VARCHAR(250) NOT NULL," + 
        " version VARCHAR(10) NOT NULL," + 
        " encoding VARCHAR(10) NOT NULL," + 
        " sub_name VARCHAR(62) NOT NULL," + 
        " sub_addr VARCHAR(256)," + 
        " num_individials INT," + 
        " num_families INT," + 
        " PRIMARY KEY(file_id), UNIQUE (file_Name))";
    connection.query( query, function( err, rows, fields ) {
        if ( err ) {
            console.log( "Something went wrong. " + err );
            res.send({
                'resp': err
            })
        } else {
            console.log( "Successfully created table FILE" );
            res.send({
                resp: "Successfully created table FILE"
            });
        }
    });
});

app.get( '/individual', function( req, res ) {
    //console.log( req.query );

    let query = "CREATE TABLE IF NOT EXISTS INDIVIDUAL (ind_id INT NOT NULL AUTO_INCREMENT," + 
        " surname VARCHAR(256) NOT NULL," + 
        " given_name VARCHAR(256) NOT NULL," + 
        " sex VARCHAR(1)," + 
        " fam_size INT," + 
        " source_file INT," + 
        " FOREIGN KEY(source_file) REFERENCES FILE(file_id) ON DELETE CASCADE," + 
        " PRIMARY KEY(ind_id), UNIQUE(source_file, surname, given_name))";
    connection.query( query, function( err, rows, fields ) {
        if ( err ) {
            console.log( "Something went wrong. " + err );
            res.send({
                'resp': err
            })
        } else {
            console.log( "Successfully created table INDIVIDUAL" );
            res.send({
                resp: "Successfully created table INDIVIDUAL"
            });
        }
    });
});

app.get( '/storeAllFiles', function( req, res ) {
    //console.log( req.query );

    let query = "INSERT INTO FILE VALUES( null, '" +
        req.query.file_name + "', '" +
        req.query.source + "', '" +
        req.query.version + "', '" +
        req.query.encoding + "', '" +
        req.query.sub_name + "', '" +
        req.query.sub_addr + "', '" +
        req.query.num_individuals + "', '" +
        req.query.num_families  + "')";

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            if (err.code != "ER_DUP_ENTRY") {
                console.log( "Something went wrong. " + err );
                res.send({
                    error: "Something went wrong. " + err
                });    
            } else {
                console.log( "Duplicate entry, not inserting.");
                res.send({
                    error: "Duplicate entry, not inserting."
                });
            }
        } else {
            console.log( "Inserted into FILE successfully." );
            res.send({
                resp: "Inserted into FILE successfully."
            });
        }
    });
});

function getKey( filename, func ) {
    let query = "SELECT file_id FROM FILE WHERE file_Name = '" + filename + "'";

    connection.query( query, function( err, rows, fields ) {
        // call passed in function
        if (err || rows[0] == null) {
            func( err, null );
        } else {
            func( null, rows[0]['file_id'] );
        }
    });
}

app.get( '/storeAllIndiv', function( req, res ) {
    //console.log( req.query );

    let indivs = sharedLib.indivListToJSON( req.query.file );
    let indivList = JSON.parse( indivs );

    getKey( req.query.file, function( error, key ) {
        if (error) {
            console.log( error );
        } else {
            for (let ind of indivList) {
                //console.log( ind );
                let query = "INSERT INTO INDIVIDUAL VALUES( null, '" +
                    ind.surname + "', '" +
                    ind.givenName + "', '" +
                    ind.sex + "', '0', '" +
                    key + "')";

                //console.log( query );

                connection.query( query, function( err, rows, fields ) {
                    if (err) {
                        if (err.code != "ER_DUP_ENTRY") {
                            console.log( "Something went wrong. " + err );
                            /*res.send({
                                error: "Something went wrong. " + err
                            });*/   
                        } else {
                            console.log( "Duplicate entry, not inserting.");
                            /*res.send({
                                error: "Duplicate entry, not inserting."
                            });*/
                        }
                    } else {
                        console.log( "Inserted into INDIVIDUAL successfully." );
                        /*res.send({
                            resp: "Inserted into INDIVIDUAL successfully."
                        });*/
                    }
                });
            }
        }
    });
    res.send({
        resp: "Finished getKey()"
    });
});

app.get( '/clearData', function( req, res ) {
    let query = "DELETE FROM " + req.query.table;

    connection.query( query, function( err, rows, fields ) {
        connection.query( "ALTER TABLE " + req.query.table + " AUTO_INCREMENT = 1", function( err, rows, fields ) {} );
        if (err) {
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            res.send({
                resp: "Deleted rows from " + req.query.table
            })
        }
    });
});

app.get( '/status', function( req, res ) {
    let query = "SELECT file_Name, num_individials as file_list FROM FILE";

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            console.log( "Something went wrong. " + err );
        } else {
            let fileCnt = 0;
            let indCnt = 0;
            for (let row of rows) {
                fileCnt++;
                indCnt += row['file_list'];
            }
            res.send({
                file: fileCnt,
                ind: indCnt
            });
        }
    });
});

app.get( '/allBySurname', function( req, res ) {
    let query = "SELECT DISTINCT given_name, surname, sex FROM INDIVIDUAL ORDER BY surname DESC, given_name DESC";

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            res.send( rows );
        }
    });
});

app.get( '/allFromFile', function( req, res ) {
    getKey( req.query.file, function( error, key ) {
        if (error) {
            console.log( "Something went wrong. " + error );
        } else {
            let query = "SELECT DISTINCT given_name, surname, sex FROM INDIVIDUAL WHERE source_file = '" + key + "'";

            connection.query( query, function( err, rows, fields ) {
                if (err) {
                    console.log( "Something went wrong. " + err );
                    res.send( {
                        err: "Something went wrong. " + err
                    });
                } else {
                    
                    res.send( rows );
                }
            });
        }
    });
});

app.get( '/help', function( req, res ) {
    let query = "DESCRIBE " + req.query.table;

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            console.log( "Something went wrong. " + err );
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            //console.log( rows );
            res.send( rows );
        }
    });
});

app.get( '/allIndOfGender', function( req, res ) {
    let query = "SELECT DISTINCT given_name, surname, sex FROM INDIVIDUAL WHERE sex = '" + req.query.sex + "'";

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            console.log( "Something went wrong. " + err );
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            //console.log( rows );
            res.send( rows );
        }
    });
});

app.get( '/allFilesFamLimit', function( req, res ) {
    let query = "SELECT file_Name, num_families FROM FILE WHERE num_families >= '" + req.query.limit + "'";

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            console.log( "Something went wrong. " + err );
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            //console.log( rows );
            res.send( rows );
        }
    });
});

app.get( '/indivsWithFile', function( req, res ) {
    let query = "SELECT given_name, surname, sex, file_Name FROM FILE, INDIVIDUAL WHERE (FILE.file_id = INDIVIDUAL.source_file) ORDER BY file_Name";

    connection.query( query, function( err, rows, fields ) {
        if (err) {
            console.log( "Something went wrong. " + err );
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            //console.log( rows );
            res.send( rows );
        }
    });
});

app.get( '/userSELECT', function( req, res ) {
    let query = req.query.select;

    connection.query( query, function( err, rows, fields ){
        if (err) {
            console.log( "Something went wrong. " + err );
            res.send({
                err: "Something went wrong. " + err
            });
        } else {
            //console.log( rows );
            res.send( rows );
        }
    });
});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
