$(document).ready(function() {
    // Page Loaded
    console.log("Page loaded successfully.");
    document.getElementById( "STATUS" ).value = "";
    document.getElementById( "EX_RESULT" ).value = "";
    document.getElementById( "QRY_IN" ).value = "";
    document.getElementById( "QRY_SEL" ).value = "SELECT ";
    document.getElementById( "DB_UNAME" ).value = "jfirth";
    document.getElementById( "DB_PWORD" ).value = "0880887";
    document.getElementById( "DB_DBASE" ).value = "jfirth";
    document.getElementById( "GD_file" ).value = "";
    document.getElementById( "GD_src" ).value = "";
    document.getElementById( "GD_ver" ).value = "";
    document.getElementById( "GD_sub" ).value = "";
    document.getElementById( "GD_subAdd" ).value = "";
    document.getElementById( "AD_givn" ).value = "";
    document.getElementById( "AD_surn" ).value = "";
    document.getElementById( "GD/A_givn" ).value = "";
    document.getElementById( "GD/A_surn" ).value = "";

    var overlay = document.getElementById("overlay");
    var popup = document.getElementById("popup");
    overlay.style.display = "block"; // make visible
    popup.style.display = "block";

    $('#DB_SUB').click( function( e ) {
        var json = {
            'uname': document.getElementById( "DB_UNAME" ).value,
            'pword': document.getElementById( "DB_PWORD" ).value,
            'dbase': document.getElementById( "DB_DBASE" ).value
        }
        if (json.uname == "" || json.pword == "" || json.dbase == "") {
            document.getElementById( "DB_UNAME" ).setAttribute( "style", "background-color: #ff0000" );
            document.getElementById( "DB_PWORD" ).setAttribute( "style", "background-color: #ff0000" );
            document.getElementById( "DB_DBASE" ).setAttribute( "style", "background-color: #ff0000" );
            return;
        }
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/file',
            data: json,
            success: function( response ) {
                console.log( response.resp );
                console.log( "Successfully connected to " + json.uname + "@dursley.socs.uoguelph.ca on the database <" + json.dbase + ">" );
                overlay.style.display = "none";
                popup.style.display = "none";
            },
            fail: function( error ) {
                console.log( error );
            }
        });

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/individual',
            data: json,
            success: function( response ) {
                console.log( response.resp );
                overlay.style.display = "none";
                popup.style.display = "none";
            },
            fail: function( error ) {
                console.log( error );
            }
        });
    });

    // On page-load AJAX
    $.ajax({
        type: 'get',            // Request type
        dataType: 'json',       // Data type
        url: '/getFiles',       // Server endpoint
        success: function( data ) {
            for (var i in data) {
                for (var j in data[i]) {
                    var file = data[i][j];
                    var ext = file.split( "." );
                    if (ext[1] == null || ext[1] != "ged") {
                        console.log( "<" + file + ">" + " not uploaded (incorrect extension)" );
                        continue;
                    }
                    addToFileLists( file );
                    document.getElementById( "STATUS" ).value = "Uploaded " + file + " to server.\n" + document.getElementById( "STATUS" ).value;
                    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
                    
                    $.ajax({
                        type: 'get',
                        dataType: 'json',
                        url: '/fileInfo',
                        data: {
                            'file': file
                        },
                        success: function( response ) {
                            updateFileView( response, file );
                        },
                        fail: function( error ) {
                            console.log( error );
                        }
                    });
                }
                
            }
        },
        fail: function( error ) {
            // Non-200 return, do something with error
            console.log( error ); 
        }
    });

    $('#CREATE').submit( function( e ) {
        e.preventDefault();
        var form = e.target;
        var elems = form.elements;
        var file = elems[0].value;
        var src = elems[1].value;
        var charSet = elems[2].value;
        var GEDCversion = elems[3].value;
        var subName = elems[4].value;
        var subAddr = elems[5].value;

        var ext = file.split( "." );
        if (ext[1] == null || ext[1] != "ged") {
            document.getElementById( "STATUS" ).value = "Invalid file extension, must be .ged\n" + document.getElementById( "STATUS" ).value;
            document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
            elems[0].value = "";
            return;
        }
        if (src.length == 0) {
			document.getElementById( "STATUS" ).value = "Must provide a 'Source' value.\n" + document.getElementById( "STATUS" ).value;
            document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
			return;
		}
		if (GEDCversion.length == 0) {
			document.getElementById( "STATUS" ).value = "Must provide a GEDCOM version.\n" + document.getElementById( "STATUS" ).value;
            document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
			return;
		} else {
			if (isNaN( GEDCversion) || Number( GEDCversion ) < 0) {
				document.getElementById( "STATUS" ).value = "GEDCOM version must be >= 0.\n" + document.getElementById( "STATUS" ).value;
				document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
				elems[3].value = "";
				return;
			}
		}
		if (subName.length == 0) {
			document.getElementById( "STATUS" ).value = "Must provide a Submitter name.\n" + document.getElementById( "STATUS" ).value;
            document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
			return;
		}

        var print = file + " " + src + " " + charSet + " " + GEDCversion + " " + subName + " " + subAddr;
        var data = {
            source: src,
            gedcVersion: GEDCversion,
            encoding: charSet,
            subName: subName,
            subAddress: subAddr
        };

        var json = JSON.stringify( data );

        var pckg = {
            'file': file,
            'json':json
        };

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/create',
            data: pckg,
            success: function( response ) {
                document.getElementById( "STATUS" ).value = response.error + "\n" + document.getElementById( "STATUS" ).value;
                document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );

                if (response.error == response.file + " already exists on the server.") {
                    return;
                }

                var temp = JSON.parse( response.json );
                var tempJSON = {
                    'file': response.file,
                    'source': temp.source,
                    'vers': temp.gedcVersion,
                    'charSet': temp.encoding,
                    'subName': temp.subName,
                    'subAddress': temp.subAddress,
                    'indivCount': 0,
                    'famCount': 0
                };
                updateFileView( tempJSON, response.file );
                addToFileLists( response.file );
            },
            fail: function( error ) {
                console.log( error );
            }
        });
    });

    $('#ADD').submit( function( e ) {
        e.preventDefault();
        
        var form = e.target;
        var elems = form.elements;
        var file = elems[2].value;
        var json = {
            'givenName': elems[0].value,
            'surname': elems[1].value
        };

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/addPerson',
            data: {
                'file': file,
                'json': json
            },
            success: function( response ) {
                document.getElementById( "STATUS" ).value = response.data + "\n" + document.getElementById( "STATUS" ).value;
                document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
                var table = document.getElementById( "FILE" );
                var rows = document.getElementById( "FILE" ).rows;
                for (var i = 1; i < rows.length - 1; i++) {
                    var cols = rows[i].cells;
                    if (cols[0].innerHTMl == response.file) {
                        var val = Number( cols[6].innerHTML );
                        val += 1;
                        cols[6].innerHTML = val;
                    }
                }
            },
            fail: function( error ) {
                console.log( error );
            }
        });
    });

    $('#FILE_SELECT3').change( function( e ) {
        var select = e.target;
        var file = select.value;

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/changeView',
            data: { 
                'file': file
            },
            success: function( response ) {
                var table = document.getElementById( "GEDCOM" );
                var rows = table.rows;
                var row1 = rows[0];
                for (var i = 1; i < rows.length; i++) {
                    rows[i].innerHTML = "";
                }
                
                for (var i = 0; i < response.length; i++) {
                    var row = table.insertRow( i + 1);
                    c1 = row.insertCell( 0 );
                    c2 = row.insertCell( 1 );
                    c3 = row.insertCell( 2 );
                    c4 = row.insertCell( 3 );

                    c1.innerHTML = response[i].givenName;
                    c2.innerHTML = response[i].surname;
                    c3.innerHTML = response[i].sex;
                }
            },
            fail: function( error ) {
                console.log( error );
            }
        })
    });
    
    $('#DESCEND').click( function( e ) {
        e.preventDefault();
        var form = document.getElementById( "descend" );
        var elems = form.elements;
        var givn = elems[0].value;
        var surn = elems[1].value;
        var gens = elems[2].value;
        var file = elems[3].value;
        
        var data = {
			'givenName': givn,
			'surname': surn
		};
		
		$.ajax({
			type: 'get',
			dataType: 'json',
			url: '/getDescend',
			data: {
				'ind': data,
				'file': file,
				'gens': gens
			},
			success: function( response ) {
				getDescend( response );
			},
			fail : function( error ) {
				console.log( error );
			}
		});
    });

	$('#CLEAR').click( function( e ) {
		document.getElementById( "STATUS" ).value = "";
	});

    $('#QRY_C').click( function( e ) {
        document.getElementById( "EX_RESULT" ).value = ""; 
    });
	
	$('#ANCEST').click( function( e ) {
		e.preventDefault();
        var form = document.getElementById( "descend" );
        var elems = form.elements;
        var givn = elems[0].value;
        var surn = elems[1].value;
        var gens = elems[2].value;
        var file = elems[3].value;
        
        var data = {
			'givenName': givn,
			'surname': surn
		};
		
		$.ajax({
			type: 'get',
			dataType: 'json',
			url: '/getAncestors',
			data: {
				'ind': data,
				'file': file,
				'gens': gens
			},
			success: function( response ) {
				getDescend( response );
			},
			fail : function( error ) {
				console.log( error );
			}
		});
	});

    $('#STORE_ALL').click( function( e ) {
        var table = document.getElementById( "FILE" );
        var rows = table.rows;

        for (let i = 1; i < rows.length - 1; i++) { // i = 1 because the first row needs to be skipped
            let cells = rows[i].cells;
            
            let json = {
                file_name: cells[0].innerText,
                source: cells[1].innerHTML,
                version: cells[2].innerHTML,
                encoding: cells[3].innerHTML,
                sub_name: cells[4].innerHTML,
                sub_addr: cells[5].innerHTML,
                num_individuals: cells[6].innerHTML,
                num_families: cells[7].innerHTML,
            }
            //console.log( json );

            $.ajax({
                type: 'get',
                dataType: 'json',
                url: '/storeAllFiles',
                data: json,
                success: function( response ) {
                    console.log( response );
                },
                fail: function( error ) {
                    if (error.code != "ER_DUP_ENTRY") {
                        console.log( "Something went wrong " + error );
                    }
                }
            });

            $.ajax({
                type: 'get',
                dataType: 'json',
                url: '/storeAllIndiv',
                data: {
                    'file': cells[0].innerText
                },
                success: function( response ) {
                    console.log( response );
                },
                fail: function( error ) {
                    if (error.code != "ER_DUP_ENTRY") {
                        console.log( "Something went wrong " + error );
                    }
                }
            });
        }
        let tmp = document.getElementById( "DB_STATUS" );
        tmp.click();
    });

    $('#CLEAR_ALL').click( function( e ) {
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/clearData',
            data: {
                table: 'INDIVIDUAL'
            },
            success: function( response ) {
                console.log( response );
            },
            fail: function( error ) {
                console.log( error );
            }
        });

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/clearData',
            data: {
                table: 'FILE'
            },
            success: function( response ) {
                console.log( response );
            },
            fail: function( error ) {
                console.log( error );
            }
        });
        let tmp = document.getElementById( "DB_STATUS" );
        tmp.click();
    });

    $('#DB_STATUS').click( function( e ) {
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/status',
            success: function( response ) {
                document.getElementById( "STATUS" ).value = "Database has " + response.file + " files and " + response.ind + " individuals.\n" + document.getElementById( "STATUS" ).value;
                document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
                document.getElementById( "EX_RESULT" ).value = "Database has " + response.file + " files and " + response.ind + " individuals.\n" + document.getElementById( "EX_RESULT" ).value;
                document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
            },
            fail: function( error ) {
                console.log( "Something went wrong. " + error );
            }
        });
    });

    $('#QRY_1').click( function( e ) {
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/allBySurname',
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value = ""; 
                for (rec of response) {
                    //let str = JSON.stringify( rec, null, 2 );
                    document.getElementById( "EX_RESULT" ).value = rec.given_name + " " + rec.surname + " " + rec.sex + "\n" + document.getElementById( "EX_RESULT" ).value;
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( error );
            }
        })
    });

    $('#QRY_2').click( function( e ) {
        let file = document.getElementById( "QRY_IN" ).value;
        if (file == "") {
            document.getElementById( "EX_RESULT" ).value = "No file name given" + "\n";
            document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
            return;
        }

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/allFromFile',
            data: {
                'file': file
            },
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value = ""; 
                for (rec of response) {
                    document.getElementById( "EX_RESULT" ).value = rec.given_name + " " + rec.surname + " " + rec.sex + "\n" + document.getElementById( "EX_RESULT" ).value;
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
                if (response.length == 0) {
                    document.getElementById( "EX_RESULT" ).value = "No results to show";
                }
            },
            fail: function( error ) {
                console.log( "Something went wrong. " + error );
            }
        });
    });

    $('#QRY_3').click( function( e ) {
        let gender = document.getElementById( "QRY_IN" ).value;
        if (gender == "") {
            document.getElementById( "EX_RESULT" ).value = "No argument given."; 
            return;
        } else if (gender == "M" || gender == "Male" || gender == "male") {
            gender = "M";
        } else if (gender == "F" || gender == "Female" || gender == "female") {
            gender = "F";
        } else {
            document.getElementById( "EX_RESULT" ).value = "Accepted gender formats: 'Male', 'male', 'M', 'Female', 'female', 'F'.\nPlease try again."; 
            document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
            return;
        }

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/allIndOfGender',
            data: {
                'sex': gender
            },
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value = "";
                for (let rec of response) {
                    document.getElementById( "EX_RESULT" ).value += rec.given_name + " " + rec.surname + " " + rec.sex + "\n";
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( "Something went wrong. " + error );
            }
        });
    });

    $('#QRY_4').click( function( e ) {
        let temp = document.getElementById( "QRY_IN" ).value;
        if (temp == "") {
            document.getElementById( "EX_RESULT" ).value = "No argument given."; 
            return;
        }

        let famLim = Number( temp );
        if (famLim < 0) {
            document.getElementById( "EX_RESULT" ).value = "Family limit must be >= 0."; 
            return;   
        }

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/allFilesFamLimit',
            data: {
                'limit': famLim
            },
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value = "";
                for (let rec of response) {
                    document.getElementById( "EX_RESULT" ).value += rec.file_Name + " - " + rec.num_families + " families\n";
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( "Something went wrong. " + error );
            }
        });
    });

    $('#QRY_5').click( function( e ) {
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/indivsWithFile',
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value = "";
                for (let rec of response) {
                    document.getElementById( "EX_RESULT" ).value += "<" + rec.file_Name + "> " + rec.given_name + " " + rec.surname + " " + rec.sex + "\n";
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( "Something went wrong. " + err );
            }
        })
    });

    $('#QRY_SUB').click( function( e ) {
        let query = document.getElementById( "QRY_SEL" ).value;

        if (query == "SELECT " || query == "SELECT" || query == "") {
            document.getElementById( "EX_RESULT" ).value = "Please fill in the query before submitting."; 
            return;
        }

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/userSELECT',
            data: {
                'select': query
            },
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value = "";
                for (let rec of response) {
                    document.getElementById( "EX_RESULT" ).value += JSON.stringify( rec, null, 2 ) + "\n";
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( "Something went wrong. " + error );
            }
        });
    });

    $('#QRY_HELP').click( function( e ) {
        document.getElementById( "EX_RESULT" ).value = "";
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/help',
            data: {
                table: 'FILE'
            },
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value += "----- FILE -----\n";
                document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                for (let rec of response) {
                    //console.log( rec );
                    document.getElementById( "EX_RESULT" ).value += "Column: " + rec.Field + " Type: " + rec.Type + " Null: " + rec.Null + " Key: "
                    + rec.Key + " Default: " + rec.Default + " Extra: " + rec.Extra + "\n";
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( error );
            }
        });

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/help',
            data: {
                table: 'INDIVIDUAL'
            },
            success: function( response ) {
                //console.log( response );
                document.getElementById( "EX_RESULT" ).value += "----- INDIVIDUAL -----\n";
                document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                for (let rec of response) {
                    //console.log( rec );
                    document.getElementById( "EX_RESULT" ).value += "Column: " + rec.Field + " Type: " + rec.Type + " Null: " + rec.Null + " Key: "
                    + rec.Key + " Default: " + rec.Default + " Extra: " + rec.Extra + "\n";
                    document.getElementById( "EX_RESULT" ).value.replace( /\r?\n/g, '<br />' );
                }
            },
            fail: function( error ) {
                console.log( error );
            }
        });
    });
});

function addToFileLists( file ) {
    var list1 = document.getElementById( 'FILE_SELECT1' );
    var list2 = document.getElementById( 'FILE_SELECT2' );
    var list3 = document.getElementById( 'FILE_SELECT3' );
    var option1 = document.createElement( 'option' );
    var option2 = document.createElement( 'option' );
    var option3 = document.createElement( 'option' );
    option1.text = file;
    option2.text = file;
    option3.text = file;
    list1.add( option1 );
    list2.add( option2 );
    list3.add( option3 );
}

function getDescend( objArray ) {
    var table = document.getElementById( "GEN_LIST" );
    table.innerHTML = "";

    var row;
    var c1;
    var c2;
    var c3;
    
    if (objArray.length == 0) {
		table.innerHTML = "No individuals to display.";
		return;
	}

	for (var i in objArray) {
		for (var j in objArray[i]) {
			row = table.insertRow( j );
			c1 = row.insertCell( 0 );
			c2 = row.insertCell( 1 );
			c3 = row.insertCell( 2 );
			c1.innerHTML = "<tr>" + objArray[i][j].givenName + "</tr>";
			c2.innerHTML = "<tr>" + objArray[i][j].surname + "</tr>";
			var t = Number( i ) + 1;
			c3.innerHTML = "<tr>" + t + "</tr>";;
		}
	}
	var row = table.insertRow( 0 );
    var c1 = row.insertCell( 0 );
    var c2 = row.insertCell( 1 );
    var c3 = row.insertCell( 2 );
    c1.innerHTML = "<th><b>First Name</b></th>";
    c2.innerHTML = "<th><b>Last Name</b></th>";
    c3.innerHTML = "<th><b>Generation</b></th>";
}

function createGED() {
    var file = document.getElementById( "GD_file" ).value;
    var src = document.getElementById( "GD_src" ).value;
    var chrSet = document.getElementById( "GD_char" ).value;
    var vers = document.getElementById( "GD_ver" ).value;
    var subName = document.getElementById( "GD_sub" ).value;
    var subAddress = document.getElementById( "GD_subAdd" ).value;

    var fileView = document.getElementById( "FILE" );
    var row = fileView.insertRow( 1 );
    
    var c1 = row.insertCell(0);
    var c2 = row.insertCell(1);
    var c3 = row.insertCell(2);
    var c4 = row.insertCell(3);
    var c5 = row.insertCell(4);
    var c6 = row.insertCell(5);
    var c7 = row.insertCell(6);
    var c8 = row.insertCell(7);
    c1.innerHTML = "<a href = " + "/uploads/" + file + " action = '/uploads/:'" + file + ">" + file + "</a>";
    c2.innerHTML = src;
    c3.innerHTML = vers;
    c4.innerHTML = chrSet;
    c5.innerHTML = subName;
    c6.innerHTML = subAddress;
    c7.innerHTML = "0";
    c8.innerHTML = "0";
    console.log( "Updated File View Panel" );

    addToFileLists( file );
    document.getElementById( "STATUS" ).value = "Created " + file + " successfully.\n" + document.getElementById( "STATUS" ).value;
    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
    console.log( "Created GEDCOM" );
}

function updateFileView( GED_JSON, file ) {
    var fileView = document.getElementById( "FILE" );
    var row = fileView.insertRow( 1 );
    
    var c1 = row.insertCell(0);
    var c2 = row.insertCell(1);
    var c3 = row.insertCell(2);
    var c4 = row.insertCell(3);
    var c5 = row.insertCell(4);
    var c6 = row.insertCell(5);
    var c7 = row.insertCell(6);
    var c8 = row.insertCell(7);
    c1.innerHTML = "<a href = " + "/uploads/" + GED_JSON.file + ">" + GED_JSON.file + "</a>";
    c2.innerHTML = GED_JSON.source;
    c3.innerHTML = GED_JSON.vers;
    c4.innerHTML = GED_JSON.charSet;
    c5.innerHTML = GED_JSON.subName;
    c6.innerHTML = GED_JSON.subAddress;
    c7.innerHTML = GED_JSON.indivCount;
    c8.innerHTML = GED_JSON.famCount;
    console.log( "Updated File View Panel" );
}
