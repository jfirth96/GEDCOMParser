// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // Page Loaded
    console.log("Page loaded successfully.");

    // For Module 1 stub
    loadDummyData();

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
                    addToFileLists( file );
                    dummyUpdateFileView( file );
                    document.getElementById( "STATUS" ).value = "Uploaded " + file + " to server.\n" + document.getElementById( "STATUS" ).value;
                    document.getElementById( "STATUS" ).value.replace( /\r?\n/g, '<br />' );
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
        console.log( e );
        var form = e.target;
        var elems = form.elements;
        console.log( elems );
        var file = elems[0].value;
        var src = elems[1].value;
        var charSet = elems[2].value;
        var GEDCversion = elems[3].value;
        var subName = elems[4].value;
        var subAddr = elems[5].value;

        // Test stuff
        var print = file + " " + src + " " + charSet + " " + GEDCversion + " " + subName + " " + subAddr;
        console.log( print );
        var data = {
            source: src,
            gedcVersion: GEDCversion,
            encoding: charSet,
            subName: subName,
            subAddress: subAddr
        };

        var json = JSON.stringify( data );
        //console.log( json );

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/create',
            data: json,
            success: function( response ) {
                console.log( "Data is: " );
                console.log( response );
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
    var option1 = document.createElement( 'option' );
    var option2 = document.createElement( 'option' );
    option1.text = file;
    option2.text = file;
    list1.add( option1 );
    list2.add( option2 );
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
    dummyUpdateFileView( file );
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

function dummyUpdateFileView( filename ) {
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
    c1.innerHTML = "<a href = " + "/uploads/" + filename + ">" + filename + "</a>";
    c2.innerHTML = "CIS2750GenealogyApp";
    c3.innerHTML = "5.5";
    c4.innerHTML = "ASCII";
    c5.innerHTML = "John Smith";
    c6.innerHTML = "N/A";
    c7.innerHTML = "8";
    c8.innerHTML = "3";
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

    c1.innerHTML = "<a " + 'onclick = dummyDownload("exampleFile.ged")>exampleFile.ged</a>';
    c2.innerHTML = "CIS2750GenealogyApp";
    c3.innerHTML = "5.5";
    c4.innerHTML = "ASCII";
    c5.innerHTML = "Jackson Firth";
    c6.innerHTML = "N/A";
    c7.innerHTML = "10";
    c8.innerHTML = "5";
    console.log( "Updated File View Panel" );
}