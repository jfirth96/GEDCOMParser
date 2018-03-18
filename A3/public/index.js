// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // Page Loaded
    console.log("Page loaded successfully.");

    // For Module 1 stub
    //loadDummyData();

    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/getFiles',   //The server endpoint we are connecting to
        success: function( data ) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */

            for (var i in data) {
                console.log( data[i] );
                for (var j in data[i]) {
                    var file = data[i][j];
                    var ext = file.split( "." );
                    if (ext[1] == null || ext[1] != "ged") {
                        console.log( "<" + file + ">" + " not uploaded (incorrect extension)" );
                        continue;
                    }
                    addToFileLists( file );
                    //dummyUpdateFileView( file );
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
                            console.log( response );
                            console.log( "succ: " + file );
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

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#CREATE').submit( function( e ) {
        e.preventDefault();
        //console.log( e );
        var form = e.target;
        var elems = form.elements;
        //console.log( elems );
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
            return;
        }

        // Test stuff
        var print = file + " " + src + " " + charSet + " " + GEDCversion + " " + subName + " " + subAddr;
        //console.log( print );
        var data = {
            source: src,
            gedcVersion: GEDCversion,
            encoding: charSet,
            subName: subName,
            subAddress: subAddr
        };

        var json = JSON.stringify( data );
        console.log( json );

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
                console.log( response );
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
                console.log( "failure" );
                console.log( error );
            }
        });
    });

    $('#ADD').submit( function( e ) {
        e.preventDefault();
        //console.log( e );

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
                //console.log( response.data );
                document.getElementById( "STATUS" ).value = response.data + "\n" + document.getElementById( "STATUS" ).value;
                document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
                var table = document.getElementById( "FILE" );
                var rows = document.getElementById( "FILE" ).rows;
                console.log( rows );
                for (var i = 1; i < rows.length - 1; i++) {
                    var cols = rows[i].cells;
                    console.log( cols );
                    console.log( cols[6].innerHTML );
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
        //console.log( file );

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/changeView',
            data: { 
                'file': file
            },
            success: function( response ) {
                // get info about file
                console.log( response );
                var table = document.getElementById( "GEDCOM" );
                var rows = table.rows;
                var row1 = rows[0];
                for (var i = 1; i < rows.length; i++) {
                    rows[i].innerHTML = "";
                }
                /*var c1 = row1.insertCell( 0 );
                var c2 = row1.insertCell( 1 );
                var c3 = row1.insertCell( 2 );
                var c4 = row1.insertCell( 3 );
                c1.innerHTML = "<th><b>Given Name</b></th>";
                c2.innerHTML = "<th><b>Surname</b></th>";
                c3.innerHTML = "<th><b>Sex</b></th>";
                c4.innerHTML = "<th><b>Family Size</b></th>";*/

                for (var i = 0; i < response.length; i++) {
                    console.log( response[i] );
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
    //console.log( "Added " + file + " to drop-down lists." );
}

function addIndividual() {
    var given = document.getElementById( "AD_givn" ).value;
    var surname = document.getElementById( "AD_surn" ).value;
    var file = document.getElementById( "FILE_SELECT1" ).value;
    document.getElementById( "STATUS" ).value = "Created new individual " + given + " " + surname + " in file: " + file + "\n" + document.getElementById( "STATUS" ).value;
    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
    console.log( "Added individual." );
}

function getDescend() {
    var file = document.getElementById( "FILE_SELECT2" ).value;

    var table = document.getElementById( "GEN_LIST" );
    table.innerHTML = "";

    var row = table.insertRow( 0 );
    var c1 = row.insertCell( 0 );
    var c2 = row.insertCell( 1 );
    var c3 = row.insertCell( 2 );
    c1.innerHTML = "<th><b>First Name</b></th>";
    c2.innerHTML = "<th><b>Last Name</b></th>";
    c3.innerHTML = "<th><b>Generation</b></th>";

    row = table.insertRow( 1 );
    c1 = row.insertCell(0);
    c2 = row.insertCell(1);
    c3 = row.insertCell( 2 );
    c1.innerHTML = "<tr>Duncan</tr>";
    c2.innerHTML = "<tr>Firth</tr>";
    c3.innerHTML = "<tr>1</tr>";

    row = table.insertRow( 2 );
    c1 = row.insertCell(0);
    c2 = row.insertCell(1);
    c3 = row.insertCell( 2 );
    c1.innerHTML = "<tr>Norman</tr>";
    c2.innerHTML = "<tr>Firth</tr>";
    c3.innerHTML = "<tr>2</tr>";
}

function uploadFile( file ) {
    document.getElementById( "STATUS" ).value = "Uploaded " + file + " to server.\n" + document.getElementById( "STATUS" ).value;
    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
    //dummyUpdateFileView( file );
    addToFileLists( file );
    console.log( "Updated System Status Box" );
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
    console.log( GED_JSON );
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

function dummyDownload( file ) {
    document.getElementById( "STATUS" ).value = "Downloaded " + file + " from the server.\n" + document.getElementById( "STATUS" ).value;
    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
    console.log( "Updated System Status Box" );
}

function loadDummyData() {
    document.getElementById( "STATUS" ).value = "";
    document.getElementById( "STATUS" ).value = "System ready for use.\n" + document.getElementById( "STATUS" ).value;
    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
    console.log( "Updated System Status Box" );

    var GEDview = document.getElementById( "GEDCOM" );
    var row = GEDview.insertRow( 1 );
    var c1 = row.insertCell(0);
    var c2 = row.insertCell(1);
    var c3 = row.insertCell(2);
    var c4 = row.insertCell(3);
    c1.innerHTML = "Jackson";
    c2.innerHTML = "Firth";
    c3.innerHTML = "Male";
    c4.innerHTML = "5";
    console.log( "Updated GEDCOM View Panel" );

    // var fileView = document.getElementById( "FILE" );
    // var row = fileView.insertRow( 1 );
    // var c1 = row.insertCell(0);
    // var c2 = row.insertCell(1);
    // var c3 = row.insertCell(2);
    // var c4 = row.insertCell(3);
    // var c5 = row.insertCell(4);
    // var c6 = row.insertCell(5);
    // var c7 = row.insertCell(6);
    // var c8 = row.insertCell(7);

    // c1.innerHTML = "<a " + 'onclick = dummyDownload("exampleFile.ged")>exampleFile.ged</a>';
    // c2.innerHTML = "CIS2750GenealogyApp";
    // c3.innerHTML = "5.5";
    // c4.innerHTML = "ASCII";
    // c5.innerHTML = "Jackson Firth";
    // c6.innerHTML = "N/A";
    // c7.innerHTML = "10";
    // c8.innerHTML = "5";
    // console.log( "Updated File View Panel" );
}