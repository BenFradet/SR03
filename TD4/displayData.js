function displayData(textBox, display) {
    var text = $(textBox).val();
    var div = $(display);
    div.html("Bonjour " + text);
}
