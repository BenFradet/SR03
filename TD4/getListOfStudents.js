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
                $('#textBox').html(data);
            } else {
                alert('error, status: ' + textStatus);
            }
        })
        .fail(function(xhr, textStatus, errorThrown) {
            alert('Status: ' + textStatus + ', error thrown: ' + errorThrown);
        });
    }
}
