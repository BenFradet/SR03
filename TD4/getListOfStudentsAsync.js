function getListOfStudents(argument, requestType) {
    if(requestType.toLowerCase() != 'post' && 
            requestType.toLowerCase() != 'get') {
        alert('not a valid http request type, must be post or get');
    } else {
        $.ajax({
            url: 'http://tuxa.sme.utc/~sr03/phpinfo/studentdata.php',
            type: requestType,
            data: { debutnom: argument },
            dataType: 'xml'
        })
        .done(function(data, textStatus, xhr) {
            if(data) {
                var content = '<tr><th>FirstName</th><th>LastName</th></tr>';
                $(data).find('utcstudent').each(function() {
                    var firstName = $(this).find('firstname').text();
                    var lastName = $(this).find('lastname').text();
                    content += '<tr><td>' + firstName + '</td><td>' +
                        lastName + '</td></tr>';
                });
                $('#result').html('<table border="1" style="width:300px">' + 
                    content + '</table>');
            } else {
                alert('error, status: ' + textStatus);
            }
        })
        .fail(function(xhr, textStatus, errorThrown) {
            alert('Status: ' + textStatus + ', error thrown: ' + errorThrown);
        });
    }
}

function displayData() {
    if($('#chaine').val().length > 2 && $('#chaine').val().length <= 30) {
        getListOfStudents($('#chaine').val(), 'get');
    } else {
        $('#result').html('');
    }
}

var str = '';
$(document).ready(function() {
    function mainLoop() {
        if(str != $('#chaine').val()) {
            str = $('#chaine').val();
            getListOfStudents(str, 'get');
        }
    }

    setTimeout(mainLoop, 300);
    setInterval(mainLoop, 300);
});
